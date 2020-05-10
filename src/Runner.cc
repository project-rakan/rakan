#include "./Runner.h"

#include <math.h>               // for pow(), log(), fmin()
#include <inttypes.h>           // for uint32_t, etc.
#include <stdio.h>              // for FILE *, fopen, fseek, fread, etc.
#include <stdlib.h>             // for rand()

#include <algorithm>            // for find()
#include <chrono>               // for system_clock:now()
#include <iostream>             // for cerr
#include <queue>                // for queue
#include <random>               // for uniform_real_distribution()
#include <unordered_set>        // for std::unordered_set
#include <utility>              // for std::pair
#include <vector>               // for std::vector

#include "./ReturnCodes.h"     // for SUCCESS, READ_FAIL, SEEK_FAIL, etc.
#include "./Graph.h"          // for class Graph
#include "./Node.h"           // for class Node
#include "./Reader.h"         // for class Reader, structs

using std::cerr;
using std::endl;
using std::vector;
using std::queue;
using std::unordered_map;
using std::unordered_set;

static Node* BFS(Graph *graph, Node *start, unordered_set<Node *> *set);

namespace rakan {

uint16_t Runner::SetDistricts(unordered_map<uint32_t, uint32_t> *map) {
  for (int i = 0; i < graph_->num_nodes_; i++) {
    graph_->nodes_[i]->district_ = (*map)[graph_->nodes_[i]->id_];
  }

  return SUCCESS;
}

uint16_t Runner::LoadGraph(FILE *file) {
  Reader reader(file);
  Header header;
  NodeRecord rec;
  uint32_t i, res, record_offset, node_offset;

  res = reader.ReadHeader(&header);
  if (res != SUCCESS) {
    cerr << "ReadHeader failed: error code " << res << endl;
    return res;
  }

  graph_ = new Graph(header.num_nodes, header.num_districts, 0);

  // Calculate the offsets of the first NodeRecord and Node.
  // Refer to the index file design for actual number of bytes.
  record_offset = kHeaderSize;
  node_offset = kHeaderSize + kNodeRecordSize * header.num_nodes;

  // Read all the nodes and populate the graph with them.
  for (i = 0; i < header.num_nodes; i++) {
    Node *node = new Node;

    res = reader.ReadNodeRecord(record_offset, &rec);
    if (res != SUCCESS) {
      cerr << "ReadNodeRecord failed: error code " << res << endl;
      return res;
    }

    res = reader.ReadNode(node_offset + rec.node_pos,
                          rec.num_neighbors,
                          node);
    if (res != SUCCESS) {
      cerr << "ReadNode failed: error code " << res << endl;
      return res;
    }

    graph_->AddNode(node);
    graph_->AddStatePop(node->GetTotalPop());
    record_offset += kNodeRecordSize;
  }

  return SUCCESS;
}

uint16_t Runner::SeedDistricts() {
  uint32_t i;
  int32_t prev_random_index, random_index;
  unordered_set<Node *> unused, seed_nodes;
  Node *found_node, *seed_node;

  // iterate through the array of nodes and put them into the set
  for (i = 0; i < graph_->num_nodes_; i++) {
    unused.insert(graph_->nodes_[i]);
  }

  // iterate through all the possible districts
  // randomly assign a node to be the seed node of that district
  prev_random_index = -1;
  for (i = 0; i < graph_->num_districts_; i++) {
    // get a random node index
    random_index = rand() % graph_->num_nodes_;
    if (random_index != prev_random_index) {
      seed_node = graph_->nodes_[random_index];
      seed_nodes.insert(seed_node);
    }
    prev_random_index = random_index;
  }

  for (auto &node : seed_nodes) {
    unused.erase(node);
  }

  while (unused.size() > 0) {
    uint32_t check = unused.size();
    for (auto &seed_node : seed_nodes) {
      found_node = BFS(graph_, seed_node, &unused);
      if (found_node != nullptr) {
        found_node->SetDistrict(seed_node->GetDistrict());
        unused.erase(found_node);
      }
    }
    if (unused.size() == check) {
      return SEEDING_FAILED;
    }
  }

  return SUCCESS;
}

uint16_t Runner::PopulateGraphData() {
  Node *current_node, *neighbor_node;
  uint32_t i, current_district;

  // O(N)
  for (i = 0; i < graph_->num_nodes_; i++) {
    current_node = graph_->nodes_[i];
    current_district = current_node->district_;

    // insert the current node into its appropriate district set
    graph_->nodes_in_district_[current_district]->insert(current_node->id_);

    // update demographics
    graph_->pop_of_district_[current_district] += current_node->GetTotalPop();
    graph_->min_pop_of_district_[current_district] += current_node->GetMinPop();

    // O(N)
    for (auto &neighbor_id : *current_node->neighbors_) {
      neighbor_node = graph_->nodes_[neighbor_id];
      // if the neighbor is in a different district, current_node
      // is on the perimeter
      if (neighbor_node->district_ != current_district) {
        if (std::find(graph_->perim_edges_->begin(),
                      graph_->perim_edges_->end(),
                      std::make_pair<int, int>(neighbor_id, i))
                      == graph_->perim_edges_->end()) {
          graph_->perim_edges_->push_back({i, neighbor_id});
        }
        graph_->nodes_on_perim_[current_district]->insert(i);
        (*graph_->perim_nodes_to_neighbors_[current_district])[i]
                                                        ->insert(neighbor_id);
      }
    }
  }

  return SUCCESS;
}

double Runner::ScoreCompactness() {
  uint32_t i, num_foreign_neighbors = 0;
  double current_score, best_score = INFINITY, worst_score = -INFINITY;

  for (i = 0; i < graph_->num_districts_; i++) {
    for (auto &pair : *graph_->perim_nodes_to_neighbors_[i]) {
      num_foreign_neighbors += pair.second->size();
    }
    current_score = num_foreign_neighbors / graph_->nodes_in_district_[i]->size();
    worst_score = fmax(worst_score, current_score);
    best_score = fmin(best_score, current_score);
  }

  return (worst_score - best_score) / 2;
}

double Runner::ScorePopulationDistribution() {
  uint32_t i, total_pop, avg_pop;
  double sum;
  
  total_pop = graph_->state_pop_;
  avg_pop = total_pop / graph_->num_districts_;

  for (i = 0; i < graph_->num_districts_; i++) {
    sum += pow((graph_->pop_of_district_[i] - avg_pop), 2);
  }

  return sum / total_pop;
}

double Runner::ScoreExistingBorders() {
  // TODO
  return 0;
}

double Runner::ScoreVRA() {
  uint32_t i, min_pop_percentage;
  double sum;

  for (i = 0; i < graph_->num_districts_; i++) {
    min_pop_percentage = graph_->GetMinorityPop(i) / graph_->GetDistrictPop(i);
    sum += fmin(0, 0.5 - min_pop_percentage);
  }

  return sum;
}

double Runner::LogScore() {
  score_ = (graph_->alpha_ * ScoreCompactness()
        + graph_->beta_ * ScorePopulationDistribution()
        + graph_->gamma_ * ScoreExistingBorders()
        + graph_->eta_ * ScoreVRA());
  return score_;
}

double Runner::MetropolisHastings() {
  double old_score, new_score, ratio;
  uint32_t random_index, random_number, old_district, new_district;
  std::pair<int, int> edge;
  Node *node;

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_real_distribution<double> index(0, graph_->perim_edges_->size());

  random_index = floor(index(generator));
  edge = (*graph_->perim_edges_)[random_index];

  std::uniform_real_distribution<double> number(0.0, 1.0);
  random_number = floor(number(generator));
  if (random_number > graph_->perim_edges_->size() / 2) {
    node = graph_->nodes_[edge.first];
    new_district = graph_->nodes_[edge.second]->id_;
  } else {
    node = graph_->nodes_[edge.second];
    new_district = graph_->nodes_[edge.first]->id_;
  }

  old_score = score_;
  old_district = node->district_;
  new_score = MakeMove(node, new_district);

  if (new_score > old_score) {
    std::uniform_real_distribution<double> decimal_number(0.0, 1.0);
    ratio = decimal_number(generator);
    if (ratio <= (old_score / new_score)) {
      MakeMove(node, old_district);
      score_ = old_score;
    }

    num_steps_++;
    if (num_steps_ >= 100) {
      // TODO: SEND CHANGES TO QUEUE
    }
  }

  score_ = new_score;

  return old_score - new_score;
}

double Runner::MakeMove(Node *node, int new_district_id) {
  unordered_set<int> *neighbors;

  // remove the node from its current district
  graph_->nodes_in_district_[node->district_]->erase(node->id_);
  // remove the node from the set of perim nodes
  graph_->nodes_on_perim_[node->district_]->erase(node->id_);
  neighbors = (*graph_->perim_nodes_to_neighbors_[node->district_])[node->id_];
  graph_->perim_nodes_to_neighbors_[node->district_]->erase(node->id_);

  // update old district population now that node is removed
  graph_->pop_of_district_[node->id_] -= (*node->demographics_)["total"];
  graph_->min_pop_of_district_[node->id_] -= ((*node->demographics_)["total"] - (*node->demographics_)["ca"]);

  // add the node to its new district
  node->district_ = new_district_id;
  graph_->nodes_in_district_[node->district_]->insert(node->id_);
  graph_->nodes_on_perim_[node->district_]->insert(node->id_);
  graph_->perim_nodes_to_neighbors_[node->district_]->insert({node->id_, neighbors});

  // update new district population
  graph_->pop_of_district_[node->id_] += (*node->demographics_)["total"];
  graph_->min_pop_of_district_[node->id_] += ((*node->demographics_)["total"] - (*node->demographics_)["ca"]);

  // return the score of this redistricting
  return LogScore();
}

double Runner::Walk(int num_steps) {
  int sum = 0;

  for (int i = 0; i < num_steps; i++) {
    sum += MetropolisHastings();
  }

  return sum;
}

}   // namespace rakan

static Node* BFS(Graph *graph, Node *start, unordered_set<Node *> *set) {
  Node *current_node;
  queue<Node *> q;
  q.push(start);

  while (!q.empty()) {
    current_node = q.front();

    if (set->find(current_node) != set->end()) {
      return current_node;
    }

    for (auto neighbor : *current_node->GetNeighbors()) {
      q.push(graph->GetNode(neighbor));
    }
  }

  return nullptr;
}
