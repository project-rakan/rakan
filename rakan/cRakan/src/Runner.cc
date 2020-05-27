#include "./Runner.h"

///////////////////////////////////////////////////////////////
#include <iostream>     // FOR DEBUGGING, REMOVE FOR PRODUCTION
///////////////////////////////////////////////////////////////

#include <math.h>               // for pow(), log(), fmin()
#include <inttypes.h>           // for uint32_t, etc.
#include <stdlib.h>             // for rand()

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

using std::queue;
using std::uniform_real_distribution;
using std::unordered_map;
using std::unordered_set;
using std::vector;

namespace rakan {

//////////////////////////////////////////////////////////////////////////////
// Construction / Initialization
//////////////////////////////////////////////////////////////////////////////

Runner::Runner() {
  graph_ = new Graph;
  changes_ = new unordered_map<int, int>;
  // each subsequent sub-vector will be initialized in the walk method.
  walk_changes_ = new vector<vector <uint32_t> *>;
  scores_ = new vector<map <string, double> *>;
}

Runner::Runner(uint32_t num_precincts, uint32_t num_districts) {
  graph_ = new Graph(num_precincts, num_districts, 0);
  changes_ = new unordered_map<int, int>;
  // each subsequent sub-vector will be initialized in the walk method.
  walk_changes_ = new vector<vector <uint32_t> *>;
  scores_ = new vector<map <string, double> *>;
}

Runner::~Runner() {
  if (graph_ != nullptr) {
    delete graph_;
  }
  delete changes_;
  // TODO: delete inner vectors if heap allocated
  delete walk_changes_;
  delete scores_;
}

void Runner::add_node(uint32_t node_id,
                      uint32_t county,
                      uint32_t majority_population,
                      uint32_t minority_population) {
  graph_->AddNode(node_id, county, majority_population, minority_population);
}

bool Runner::add_edge(uint32_t node_one, uint32_t node_two) {
  if (graph_->ContainsNode(graph_->GetNode(node_one)) && 
      graph_->ContainsNode(graph_->GetNode(node_two))) {
      graph_->AddEdge(node_one, node_two);
      return true;
  }
  return false;
}

bool Runner::set_districts(vector<uint32_t>& districts) {
  if (districts.size() != graph_->GetNumDistricts()) {
    return false;
  }

  for (int i = 0; i < districts.size(); i++) {
    graph_->nodes_[i]->district_ = districts[i];
  }

  return true;
}

bool Runner::seed() {
  uint32_t i;
  int32_t prev_random_index, random_index;
  vector<uint32_t> random_indexes;
  unordered_set<uint32_t> unused;
  unordered_set<Node *> seed_nodes;
  unordered_map<int, Node *> last_found;
  Node *found_node, *seed_node;

  for (i = 0; i < graph_->num_nodes_; i++) {
    unused.insert(i);
  }

  for (i = 0; i < graph_->num_districts_; i++) {
    random_index = rand() % graph_->num_nodes_;
    if (std::find(random_indexes.begin(),
                  random_indexes.end(),
                  random_index) == random_indexes.end()) {
      seed_node = graph_->nodes_[random_index];
      unused.erase(random_index);
      seed_node->SetDistrict(i);
      seed_nodes.insert(seed_node);
      random_indexes.push_back(random_index);
      changes_->insert({seed_node->id_, i});
    } else {
      i--;
    }
  }

  for (auto &node : seed_nodes) {
    last_found[node->district_] = node;
  }

  while (unused.size() > 0) {
    uint32_t check = unused.size();
    for (int i = 0; i < graph_->num_districts_; i++) {
      found_node = BFS(last_found[i], &unused);
      if (found_node != nullptr) {
        found_node->SetDistrict(i);
        unused.erase(found_node->GetID());
        last_found[i] = found_node;
        changes_->insert({found_node->id_, i});
      }
    }
    if (unused.size() == check) {
      return false;
    }
  }

  return true;
}

void Runner::populate() {
  unordered_map<int, unordered_set<uint32_t> *> *map;
  Node *current_node, *neighbor_node;
  uint32_t i, current_district;

  for (i = 0; i < graph_->num_nodes_; i++) {
    current_node = graph_->nodes_[i];
    current_district = current_node->district_;
    graph_->AddNodeToDistrict(current_node->id_, current_district);

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

  return old_score - new_score;
}

double Runner::Redistrict(Node *node, int new_district) {
  int old_district = node->district_;

  graph_->RemoveNodeFromDistrict(node->id_, old_district);
  graph_->RemoveNodeFromDistrictPerim(node->id_, old_district);

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

  graph_->AddNodeToDistrict(node->id_, new_district);
  graph_->AddNodeToDistrictPerim(node->id_, new_district);

  for (auto &neighbor_id : *node->neighbors_) {
    if (graph_->nodes_[neighbor_id]->district_ != new_district) {
        graph_->perim_edges_->push_back({node->id_, neighbor_id});
    }
  }

  return LogScore();
}

double Runner::Walk(uint32_t num_steps, double alpha, double beta, double gamma, double eta) {
  uint32_t sum = 0;
  alpha_ = alpha;
  beta_ = beta;
  gamma_ = gamma;
  eta_ = eta;

  for (uint32_t i = 0; i < num_steps; i++) {
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

vector<vector<uint32_t>> Runner::getMaps() {
  vector<vector<uint32_t>> outer_vector;
  for (int i = 0; i < walk_changes_->size(); i++) {
    vector<uint32_t> inner_vector = *((*walk_changes_)[i]);
    outer_vector.push_back(inner_vector);
  }
  return outer_vector;
}

vector<map<string, double>> Runner::getScores() {
  vector<map<string, double>> outer_vector;
  for (int i = 0; i < scores_->size(); i++) {
    map<string, double> inner_map = *((*scores_)[i]);
    outer_vector.push_back(inner_map);
  }
  return outer_vector;
}


 //////////////////////////////////////////////////////////////////////////////
 // Helpers
 //////////////////////////////////////////////////////////////////////////////

Node *Runner::BFS(Node *start, unordered_set<uint32_t> *set) {
  Node *current_node;
  unordered_set<Node *> processed;
  queue<Node *> q;
  q.push(start);

  while (!q.empty()) {
    current_node = q.front();
    q.pop();

    if (set->find(current_node->id_) != set->end()) {
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
