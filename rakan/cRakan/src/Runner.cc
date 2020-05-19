#include "./Runner.h"

#include <math.h>               // for pow(), log(), fmin()
#include <inttypes.h>           // for uint32_t, etc.
#include <stdio.h>              // for FILE *, fopen, fseek, fread, etc.
#include <stdlib.h>             // for rand()

#include <iostream>

#include <algorithm>            // for find()
#include <chrono>               // for system_clock:now()
#include <queue>                // for queue
#include <random>               // for uniform_real_distribution()
#include <unordered_set>        // for std::unordered_set
#include <utility>              // for std::pair
#include <vector>               // for std::vector

#include "./ReturnCodes.h"      // for SUCCESS, READ_FAIL, SEEK_FAIL, etc.
#include "./Graph.h"            // for class Graph
#include "./Node.h"             // for class Node
#include "./Reader.h"           // for class Reader, structs

using battledance::MapJobUpdate;
using std::queue;
using std::uniform_real_distribution;
using std::unordered_map;
using std::unordered_set;
using std::vector;

namespace rakan {

//////////////////////////////////////////////////////////////////////////////
// Construction / Initialization
//////////////////////////////////////////////////////////////////////////////

uint16_t Runner::LoadGraph(FILE *file) {
  Reader reader(file);
  Header header;
  NodeRecord rec;
  uint32_t i, res, record_offset, node_offset;

  res = reader.ReadHeader(&header);
  if (res != SUCCESS) {
    return res;
  }

  graph_ = new Graph(header.num_nodes, header.num_districts, 0);

  record_offset = kHeaderSize;
  node_offset = kHeaderSize + kNodeRecordSize * header.num_nodes;

  for (i = 0; i < header.num_nodes; i++) {
    Node *node = new Node;

    res = reader.ReadNodeRecord(record_offset, &rec);
    if (res != SUCCESS) {
      return res;
    }

    res = reader.ReadNode(node_offset + rec.node_pos,
                          rec.num_neighbors,
                          node);
    if (res != SUCCESS) {
      return res;
    }

    graph_->AddNode(node);
    graph_->AddStatePop(node->GetTotalPop());
    record_offset += kNodeRecordSize;
  }

  return SUCCESS;
}

uint16_t Runner::SetDistricts(unordered_map<uint32_t, uint32_t> *map) {
  for (int i = 0; i < graph_->num_nodes_; i++) {
    graph_->nodes_[i]->district_ = (*map)[graph_->nodes_[i]->id_];
  }

  return SUCCESS;
}

uint16_t Runner::SeedDistricts() {
  uint32_t i;
  int32_t prev_random_index, random_index;
  vector<uint32_t> random_indexes;
  unordered_set<Node *> unused, seed_nodes;
  map<int, Node *> last_found;
  Node *found_node, *seed_node;

  for (i = 0; i < graph_->num_nodes_; i++) {
    unused.insert(graph_->nodes_[i]);
  }

  for (i = 0; i < graph_->num_districts_; i++) {
    random_index = rand() % graph_->num_nodes_;
    if (std::find(random_indexes.begin(),
                  random_indexes.end(),
                  random_index) == random_indexes.end()) {
      seed_node = graph_->nodes_[random_index];
      seed_node->SetDistrict(i);
      seed_nodes.insert(seed_node);
      random_indexes.push_back(random_index);
      changes_->insert({seed_node->id_, i});
    } else {
      i--;
    }
  }

  for (auto &node : seed_nodes) {
    unused.erase(node);
    last_found[node->district_] = node;
  }

  while (unused.size() > 0) {
    uint32_t check = unused.size();
    for (int i = 0; i < graph_->num_districts_; i++) {
      found_node = BFS(last_found[i], &unused);
      if (found_node != nullptr) {
        found_node->SetDistrict(i);
        unused.erase(found_node);
        last_found[i] = found_node;
        changes_->insert({found_node->id_, i});
      }
    }
    if (unused.size() == check) {
      return SEED_FAILED;
    }
  }

  SubmitToQueue(changes_);

  return SUCCESS;
}

uint16_t Runner::PopulateGraphData() {
  unordered_map<int, unordered_set<uint32_t> *> *map;
  Node *current_node, *neighbor_node;
  uint32_t i, current_district;

  for (i = 0; i < graph_->num_nodes_; i++) {
    current_node = graph_->nodes_[i];
    current_district = current_node->district_;
    graph_->AddNodeToDistrict(current_node, current_district);

    for (auto &neighbor_id : *current_node->neighbors_) {
      neighbor_node = graph_->nodes_[neighbor_id];
      if (neighbor_node->district_ != current_district) {
        if (std::find(graph_->perim_edges_->begin(),
                      graph_->perim_edges_->end(),
                      std::make_pair<int, int>(neighbor_id, i))
                      == graph_->perim_edges_->end()) {
          graph_->perim_edges_->push_back({i, neighbor_id});
        }
        graph_->nodes_on_perim_[current_district]->insert(i);

        map = graph_->perim_nodes_to_neighbors_[current_district];
        if (map->find(i) == map->end()) {
          (*map)[i] = new unordered_set<uint32_t>;
        }
        (*map)[i]->insert(neighbor_id);
      }
    }
  }

  return SUCCESS;
}

uint16_t Runner::StartMapJob(StartMapJobRequest *request, uint32_t num_steps) {
  FILE *file;
  request_ = request;

  if ((file = fopen("../tst/iowa.idx", "rb")) == nullptr) {
    return INVALID_FILE;
  }
  if (LoadGraph(file) != SUCCESS) {
    return LOAD_FAILED;
  }
  if (SeedDistricts() != SUCCESS) {
    return SEED_FAILED;
  }
  if (PopulateGraphData() != SUCCESS) {
    return POPULATE_FAILED;
  }
  Walk(num_steps);
  fclose(file);
  delete graph_;

  return SUCCESS;
}


//////////////////////////////////////////////////////////////////////////////
// Scoring
//////////////////////////////////////////////////////////////////////////////

double Runner::ScoreCompactness() {
  unordered_map<int, unordered_set<int>> perim_nodes_to_neighbors;
  uint32_t i, num_foreign_neighbors = 0;
  double current_score = 0, sum = 0;

  for (i = 0; i < graph_->num_districts_; i++) {
    for (auto &pair : *graph_->perim_nodes_to_neighbors_[i]) {
      num_foreign_neighbors += pair.second->size();
    }
    current_score = pow(num_foreign_neighbors, 2) /
                    graph_->nodes_in_district_[i]->size();
    sum += current_score;
  }

  compactness_score_ = sum;
  return compactness_score_;
}

double Runner::ScorePopulationDistribution() {
  uint32_t i, total_pop, avg_pop;
  double sum = 0;
  
  total_pop = graph_->state_pop_;
  avg_pop = total_pop / graph_->num_districts_;

  for (i = 0; i < graph_->num_districts_; i++) {
    sum += pow((graph_->pop_of_district_[i] - avg_pop), 1);
  }

  distribution_score_ = sum / graph_->num_districts_;
  return distribution_score_;
}

double Runner::ScoreExistingBorders() {
  border_score_ = 0;
  return border_score_;
}

double Runner::ScoreVRA() {
  uint32_t i;
  double sum = 0, min_pop_percentage;

  for (i = 0; i < graph_->num_districts_; i++) {
    min_pop_percentage = ((double) graph_->GetMinorityPop(i)) /
                          ((double)graph_->GetDistrictPop(i));
    if ((0.5 - min_pop_percentage) > 0) {
      sum += min_pop_percentage;
    }
  }

  vra_score_ = sum;
  return vra_score_;
}

double Runner::LogScore() {
  score_ = (alpha_ * ScoreCompactness()
          + beta_ * ScorePopulationDistribution()
          + gamma_ * ScoreExistingBorders()
          + eta_ * ScoreVRA());
  return score_;
}


//////////////////////////////////////////////////////////////////////////////
// Algorithms
//////////////////////////////////////////////////////////////////////////////

double Runner::MetropolisHastings() {
  double old_score, new_score, ratio;
  uint32_t random_index, random_number, old_district, new_district;
  std::pair<int, int> edge;
  Node *node;
  bool is_valid = false, accepted = false;

  // random number generators initialization
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  uniform_real_distribution<double> index(0, graph_->perim_edges_->size());
  uniform_real_distribution<double> number(0, graph_->perim_edges_->size());
  uniform_real_distribution<double> decimal_number(0, 1);

  while (!is_valid) {
    random_index = floor(index(generator));
    edge.first = (*graph_->perim_edges_)[random_index].first;
    edge.second = (*graph_->perim_edges_)[random_index].second;

    random_number = floor(number(generator));
    if (random_number > graph_->perim_edges_->size() / 2) {
      node = graph_->nodes_[edge.first];
      new_district = graph_->nodes_[edge.second]->district_;
    } else {
      node = graph_->nodes_[edge.second];
      new_district = graph_->nodes_[edge.first]->district_;
    }

    old_district = node->district_;
    is_valid = !IsEmptyDistrict(old_district) && !IsDistrictSevered(node);
    is_valid &= (graph_->nodes_[edge.first]->district_ !=
                 graph_->nodes_[edge.second]->district_);
    is_valid &= (edge.first != edge.second);
  }

  old_score = LogScore();
  new_score = Redistrict(node, new_district);

  if (new_score > old_score) {
    ratio = decimal_number(generator);
    if (ratio <= (old_score / new_score)) {
      Redistrict(node, old_district);
      score_ = old_score;
    } else {
      score_ = new_score;
      accepted = true;
    }
  } else {
    score_ = new_score;
    accepted = true;
  }
  
  (*changes_)[node->id_] = node->district_;
  num_steps_++;

  if (accepted) {
    SubmitToQueue(changes_);
  }

  return old_score - new_score;
}

double Runner::Redistrict(Node *node, int new_district) {
  int old_district = node->district_;

  graph_->RemoveNodeFromDistrict(node, old_district);
  graph_->RemoveNodeFromDistrictPerim(node, old_district);

  vector<pair<int, int>>::iterator iter = graph_->perim_edges_->begin();
  vector<pair<int, int>> *new_perim_edges = new vector<pair<int, int>>;
  while (iter != graph_->perim_edges_->end()) {
    pair<int, int> edge = *iter;
    if (edge.first != node->id_ && edge.second != node->id_) {
      new_perim_edges->push_back(edge);
    }
    iter++;
  }
  delete graph_->perim_edges_;
  graph_->perim_edges_ = new_perim_edges;

  graph_->AddNodeToDistrict(node, new_district);
  graph_->AddNodeToDistrictPerim(node, new_district);

  for (auto &neighbor_id : *node->neighbors_) {
    if (graph_->nodes_[neighbor_id]->district_ != new_district) {
        graph_->perim_edges_->push_back({node->id_, neighbor_id});
    }
  }

  return LogScore();
}

double Runner::Walk(int num_steps) {
  int sum = 0;

  for (int i = 0; i < num_steps; i++) {
    sum += MetropolisHastings();
  }

  return sum;
}


//////////////////////////////////////////////////////////////////////////////
// Queries
//////////////////////////////////////////////////////////////////////////////

bool Runner::IsEmptyDistrict(int old_district) {
  return graph_->nodes_in_district_[old_district]->size() <= 1;
}

bool Runner::IsDistrictSevered(Node *proposed_node) {
  unordered_map<int, vector<Node *>> map;
  Node *start;
  int old_district = proposed_node->district_;
  proposed_node->district_ = graph_->num_districts_ + 1;

  for (auto &neighbor : *proposed_node->neighbors_) {
    map[graph_->nodes_[neighbor]->district_].
                                      push_back(graph_->nodes_[neighbor]);
  }

  for (auto &pair : map) {
    for (int i = 0; i < map[pair.first].size() - 1; i++) {
      if (!DoesPathExist(map[pair.first][i], map[pair.first][i+1])) {
        proposed_node->district_ = old_district;
        return true;
      }
    }
  }

  proposed_node->district_ = old_district;
  return false;
}

bool Runner::DoesPathExist(Node *start, Node *target) {
  queue<Node *> q;
  unordered_set<Node *> processed;
  Node *current_node;
  q.push(start);
  
  while (!q.empty()) {
    current_node = q.front();
    q.pop();

    if (current_node == target) {
      return true;
    }
    processed.insert(current_node);

    for (auto &neighbor : *current_node->neighbors_) {
      if (graph_->nodes_[neighbor]->district_ == current_node->district_ &&
          std::find(processed.begin(),
                    processed.end(),
                    graph_->nodes_[neighbor]) == processed.end()) {
        q.push(graph_->nodes_[neighbor]);
      }
    }
  }

  return false;
}


 //////////////////////////////////////////////////////////////////////////////
 // Helpers
 //////////////////////////////////////////////////////////////////////////////

void Runner::SubmitToQueue(unordered_map<int, int> *changes) {
  MapJobUpdate *update = new MapJobUpdate;
  char *state = new char[2];
  state[0] = request_->state[0];
  state[1] = request_->state[1];
  char *guid = new char[6];
  for (int i = 0; i < 6; i++) {
    guid[i] = request_->guid[i];
  }
  update->state = state;
  update->guid = guid;
  update->map = changes;
  update->alpha = alpha_;
  update->beta = beta_;
  update->gamma = gamma_;
  update->eta = eta_;
  update->compactness = compactness_score_;
  update->distribution = distribution_score_;
  update->borderRespect = border_score_;
  update->vra = vra_score_;
  changes->clear();
}

Node *Runner::BFS(Node *start, unordered_set<Node *> *set) {
  Node *current_node;
  unordered_set<Node *> processed;
  queue<Node *> q;
  q.push(start);

  while (!q.empty()) {
    current_node = q.front();
    q.pop();

    if (set->find(current_node) != set->end()) {
      return current_node;
    }

    processed.insert(current_node);
    for (auto neighbor : *current_node->GetNeighbors()) {
      if (std::find(processed.begin(),
                    processed.end(),
                    graph_->GetNode(neighbor)) == processed.end()) {
        q.push(graph_->GetNode(neighbor));
      }
    }
  }

  return nullptr;
}

}   // namespace rakan
