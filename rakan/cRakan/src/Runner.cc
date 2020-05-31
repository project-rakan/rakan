#include "./Runner.h"

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

#ifndef TEST_MODE
#define SEED std::chrono::system_clock::now().time_since_epoch().count()
#endif

using std::queue;
using std::uniform_int_distribution;
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
  walk_changes_ = new vector<vector <uint32_t> *>;
  scores_ = new vector<map <string, double> *>;
  generator_ = new std::default_random_engine;
  generator_->seed(SEED);
}

Runner::Runner(uint32_t num_precincts, uint32_t num_districts) {
  graph_ = new Graph(num_precincts, num_districts);
  walk_changes_ = new vector<vector <uint32_t> *>;
  scores_ = new vector<map <string, double> *>;
  generator_ = new std::default_random_engine;
  generator_->seed(SEED);
}

Runner::~Runner() {
  if (graph_ != nullptr) {
    delete graph_;
  }
  for (uint32_t i = 0; i < walk_changes_->size(); i++) {
    delete (*walk_changes_)[i];
  }
  delete walk_changes_;
  for (uint32_t i = 0; i < scores_->size(); i++) {
    delete (*scores_)[i];
  }
  delete scores_;
}

void Runner::add_node(uint32_t node_id,
                      uint32_t county,
                      uint32_t majority_population,
                      uint32_t minority_population) {
  graph_->AddNode(node_id, county, majority_population, minority_population);
}

bool Runner::add_edge(uint32_t node_one, uint32_t node_two) {
  if (graph_->ContainsNode(node_one) && 
      graph_->ContainsNode(node_two)) {
      graph_->AddEdge(node_one, node_two);
      return true;
  }
  return false;
}

bool Runner::set_districts(vector<uint32_t>& districts) {
  map<string, double> *scores = new map<string, double>;

  if (districts.size() != graph_->num_nodes_) {
    return false;
  }

  for (uint32_t i = 0; i < districts.size(); i++) {
    if (districts[i] >= graph_->num_districts_) {
      return false;
    }
    Node *node = graph_->nodes_[i];
    if (graph_->NodeExistsInDistrict(node->id_, node->district_)) {
      graph_->RemoveNodeFromDistrict(node->id_, node->district_);
    }
    graph_->AddNodeToDistrict(node->id_, districts[i]);
  }

  populate();
  (*scores)["total"] = LogScore();
  (*scores)["compact"] = compactness_score_;
  (*scores)["distribution"] = distribution_score_;
  (*scores)["border"] = border_score_;
  (*scores)["vra"] = vra_score_;
  scores_->push_back(scores);

  return true;
}

unordered_set<Node *>* Runner::GenerateRandomSeeds() {
  unordered_set<Node *> *seed_nodes = new unordered_set<Node *>;
  Node *seed_node;
  uint32_t i, random_index;
  vector<uint32_t> random_indexes;
  vector<uint32_t> *changes = new vector<uint32_t>(graph_->num_nodes_);
  uniform_int_distribution<uint32_t> index(0, graph_->num_nodes_ - 1);

  for (i = 0; i < graph_->num_districts_; i++) {
    random_index = index(*generator_);
    if (std::find(random_indexes.begin(),
                  random_indexes.end(),
                  random_index) == random_indexes.end()) {
      graph_->AddNodeToDistrict(random_index, i);
      seed_node = graph_->nodes_[random_index];
      seed_nodes->insert(seed_node);
      random_indexes.push_back(random_index);
      (*changes)[random_index] = i;
    } else {
      i--;
    }
  }

  if (!walk_changes_->empty()) {
    walk_changes_->clear();
  }
  walk_changes_->push_back(changes);

  return seed_nodes;
}

bool Runner::SpawnDistricts(unordered_set<Node *> *seed_nodes) {
  unordered_set<uint32_t> unused;
  unordered_map<int, Node *> last_found;
  Node *node, *found_node;
  vector<uint32_t> *changes = nullptr;
  map<string, double> *scores = new map<string, double>;
  unordered_map<uint32_t, vector<uint32_t> *> queues;
  unordered_set<uint32_t> *processed = new unordered_set<uint32_t>;
  uint32_t i, check;

  if (!walk_changes_->empty()) {
    changes = (*walk_changes_)[0];
  }

  for (i = 0; i < graph_->num_nodes_; i++) {
    node = graph_->nodes_[i];
    if (seed_nodes->find(node) != seed_nodes->end()) {
      unused.erase(i);
      last_found[node->district_] = node;
      queues[node->district_] = new vector<uint32_t>;
      queues[node->district_]->push_back(i);
    } else {
      graph_->nodes_[i]->district_ = graph_->num_districts_ + 1;
      unused.insert(i);
    }
  }

  while (unused.size() > 0) {
    check = unused.size();
    for (i = 0; i < graph_->num_districts_; i++) {
      found_node = BFS(queues[i], processed, i, &unused);
      if (found_node != nullptr) {
        graph_->AddNodeToDistrict(found_node->id_, i);
        unused.erase(found_node->id_);
        last_found[i] = found_node;
        if (changes != nullptr) {
          (*changes)[found_node->id_] = i;
        }
      }
    }
    if (unused.size() == check) {
      return false;
    }
  }

  populate();
  (*scores)["total"] = LogScore();
  (*scores)["compact"] = compactness_score_;
  (*scores)["distribution"] = distribution_score_;
  (*scores)["border"] = border_score_;
  (*scores)["vra"] = vra_score_;
  scores_->push_back(scores);

  return true;
}

bool Runner::seed() {
  return SpawnDistricts(GenerateRandomSeeds());
}

void Runner::populate() {
  unordered_map<uint32_t, unordered_set<uint32_t> *> *map;
  Node *current_node, *neighbor_node;
  uint32_t i, current_district;

  for (i = 0; i < graph_->num_nodes_; i++) {
    current_node = graph_->nodes_[i];
    current_district = current_node->district_;

    for (auto &neighbor_id : *current_node->neighbors_) {
      neighbor_node = graph_->nodes_[neighbor_id];
      if (neighbor_node->district_ != current_district) {
        graph_->MarkCrossingEdge(i, neighbor_id);
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
  double sum = 0;

  for (auto pair : *graph_->num_districts_in_county_) {
    sum += pow(pair.second - 1, 2);
  }

  border_score_ = sum;
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
  uint32_t i, random_index, random_number, old_district;
  Edge edge;
  Node *victim_node, *idle_node;
  vector<uint32_t> *changes = new vector<uint32_t>;
  map<string, double> *scores = new map<string, double>;
  bool is_valid = false, accepted = false;

  // random number generators initialization
  uniform_int_distribution<uint32_t> index(0, graph_->crossing_edges_->size() - 1);
  uniform_int_distribution<uint32_t> number(0, graph_->crossing_edges_->size() - 1);
  uniform_real_distribution<double> decimal_number(0, 1);

  while (!is_valid) {
    random_index = index(*generator_);
    i = 0;
    unordered_set<Edge, EdgeHash>::iterator itr =
        graph_->crossing_edges_->begin();
    while (i < random_index) {
      itr++;
      i++;
    }
    edge = *itr;

    random_number = floor(number(*generator_));
    if (random_number > graph_->crossing_edges_->size() / 2) {
      victim_node = graph_->nodes_[edge.node1];
      idle_node = graph_->nodes_[edge.node2];
    } else {
      victim_node = graph_->nodes_[edge.node2];
      idle_node = graph_->nodes_[edge.node1];
    }

    old_district = victim_node->district_;
    is_valid = IsValidRedistricting(victim_node, idle_node);
  }

  old_score = LogScore();
  new_score = Redistrict(victim_node, idle_node);

  if (new_score > old_score) {
    ratio = decimal_number(*generator_);
    if (ratio <= (old_score / new_score)) {
      victim_node->district_ = old_district;
      Redistrict(victim_node, victim_node);
      score_ = old_score;
    } else {
      score_ = new_score;
      accepted = true;
    }
  } else {
    score_ = new_score;
    accepted = true;
  }
  
  if (accepted) {
    for (i = 0; i < graph_->num_nodes_; i++) {
      changes->push_back(graph_->nodes_[i]->district_);
    }
    walk_changes_->push_back(changes);
    (*scores)["total"] = score_;
    (*scores)["compact"] = compactness_score_;
    (*scores)["distribution"] = distribution_score_;
    (*scores)["border"] = border_score_;
    (*scores)["vra"] = vra_score_;
    scores_->push_back(scores);
  }

  return new_score - old_score;
}

double Runner::Redistrict(Node *victim_node, Node *idle_node) {
  int old_district = victim_node->district_;
  int new_district = idle_node->district_;

  graph_->RemoveNodeFromDistrict(victim_node->id_, old_district);
  graph_->RemoveNodeFromDistrictPerim(victim_node->id_, old_district);
  graph_->AddNodeToDistrict(victim_node->id_, new_district);
  graph_->AddNodeToDistrictPerim(victim_node->id_, new_district);
  graph_->UpdatePerimNode(victim_node);
  graph_->UpdatePerimNode(idle_node);

  return LogScore();
}

double Runner::Walk(uint32_t num_steps,
                    double alpha, double beta, double gamma, double eta) {
  uint32_t sum = 0, accepted_steps = 0;
  alpha_ = alpha;
  beta_ = beta;
  gamma_ = gamma;
  eta_ = eta;

  for (uint32_t i = 0; i < num_steps; i++) {
    sum += MetropolisHastings();
    if (walk_changes_->size() <= accepted_steps) {
      i--;
    }
  }

  return sum;
}


//////////////////////////////////////////////////////////////////////////////
// Queries
//////////////////////////////////////////////////////////////////////////////

bool Runner::IsEmptyDistrict(int old_district) {
  return graph_->nodes_in_district_[old_district]->size() <= 1;
}

bool Runner::IsDistrictSevered(Node *proposed_node, uint32_t new_district) {
  unordered_map<int, vector<Node *>> map;
  uint32_t old_district = proposed_node->district_;
  uint32_t neighbor_district, i;

  if (proposed_node->district_ == new_district) {
    return false;
  }

  proposed_node->district_ = graph_->num_districts_ + 1;

  for (auto &neighbor : *proposed_node->neighbors_) {
    neighbor_district = graph_->nodes_[neighbor]->district_;
    if (neighbor_district != graph_->num_districts_ + 1) {
      map[graph_->nodes_[neighbor]->district_].
            push_back(graph_->nodes_[neighbor]);
    }
  }

  for (auto &pair : map) {
    for (i = 0; i < map[pair.first].size() - 1; i++) {
      if (!DoesPathExist(map[pair.first][i], map[pair.first][i+1])) {
        proposed_node->district_ = old_district;
        return true;
      }
    }
  }

  graph_->AddNodeToDistrict(proposed_node->id_, new_district);
  unordered_set<uint32_t> *nodes = graph_->GetNodesInDistrict(new_district);
  unordered_set<uint32_t>::iterator itr = nodes->begin();
  Node *first, *second;

  if (nodes->size() > 1) {
    while (1) {
      first = graph_->GetNode(*itr);
      if (itr == nodes->end() || ++itr == nodes->end()) {
        break;
      }
      second = graph_->GetNode(*itr);
      if (!DoesPathExist(first, second)) {
        graph_->RemoveNodeFromDistrict(proposed_node->id_, new_district);
        proposed_node->district_ = old_district;
        return true;
      }
    }
  }
  graph_->RemoveNodeFromDistrict(proposed_node->id_, new_district);

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
  for (uint32_t i = 0; i < walk_changes_->size(); i++) {
    vector<uint32_t> inner_vector = *((*walk_changes_)[i]);
    outer_vector.push_back(inner_vector);
  }
  return outer_vector;
}

vector<map<string, double>> Runner::getScores() {
  vector<map<string, double>> outer_vector;
  for (uint32_t i = 0; i < scores_->size(); i++) {
    map<string, double> inner_map = *((*scores_)[i]);
    outer_vector.push_back(inner_map);
  }
  return outer_vector;
}


//////////////////////////////////////////////////////////////////////////////
// Helpers
//////////////////////////////////////////////////////////////////////////////

bool Runner::IsValidRedistricting(Node *node1, Node *node2) {
  // Nodes must be different and in different districts.
  if (node1 == node2 || node1->district_ == node2->district_) {
    return false;
  }
  // Graph must contain nodes.
  if (!graph_->ContainsNode(node1->id_) || !graph_->ContainsNode(node2->id_)) {
    return false;
  }
  // Graph must contain bi-directional edges between both nodes.
  if (!graph_->ContainsEdge(node1->id_, node2->id_) || 
      !graph_->ContainsEdge(node2->id_, node1->id_)) {
    return false;
  }
  // Old district must not be empty after redistricting.
  if (IsEmptyDistrict(node1->district_)) {
    return false;
  }
  // Old district and new district must not be severed after redistricting.
  if (IsDistrictSevered(node1, node2->district_)) {
    return false;
  }
  return true;
}

bool Runner::IsDistrictConnected(uint32_t district_id) {
  unordered_set<uint32_t> *nodes = graph_->GetNodesInDistrict(district_id);
  unordered_set<uint32_t>::iterator itr = nodes->begin();
  Node *node, *neighbor_node;
  
  for (uint32_t i = 0; i < nodes->size() - 1; i++) {
    if (itr == nodes->end()) {
      break;
    }
    node = graph_->nodes_[*itr];
    itr++;
    if (itr == nodes->end()) {
      break;
    }
    neighbor_node = graph_->nodes_[*itr];
    if (!DoesPathExist(node, neighbor_node)) {
      return false;
    }
  }

  return true;
}

Node *Runner::BFS(vector<uint32_t> *q,
                  unordered_set<uint32_t> *processed,
                  uint32_t district,
                  unordered_set<uint32_t> *set) {
  uint32_t current_node_id;
  Node *current_node;

  while (!q->empty()) {
    current_node_id = q->front();

    if (set->find(current_node_id) != set->end()) {
      return graph_->GetNode(current_node_id);
    }

    current_node = graph_->GetNode(current_node_id);
    q->erase(std::find(q->begin(), q->end(), current_node_id));
    processed->insert(current_node_id);

    if (graph_->nodes_[current_node_id]->district_ == district || 
        graph_->nodes_[current_node_id]->district_ == graph_->num_districts_ + 1) {
      for (auto neighbor : *graph_->GetNode(current_node_id)->GetNeighbors()) {
        if (std::find(processed->begin(), processed->end(), neighbor) == processed->end() &&
            (graph_->nodes_[neighbor]->district_ == district ||
            graph_->nodes_[neighbor]->district_ == graph_->num_districts_ + 1) &&
            std::find(q->begin(), q->end(), neighbor) == q->end()) {
          q->push_back(neighbor);
        }
      }
    }
  }

  return nullptr;
}

}   // namespace rakan
