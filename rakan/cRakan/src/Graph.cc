#include "./Graph.h"

#include <inttypes.h>       // for uint32_t
#include <stdio.h>          // for FILE *, stderr
#include <string>           // for std::string
#include <unordered_set>    // for std::unordered_set
#include <unordered_map>    // for std::unordered_map
#include <vector>           // for std::vector

#include "./ReturnCodes.h"     // for return status
#include "./Node.h"           // for Node class

using std::string;
using std::unordered_set;
using std::unordered_map;

namespace rakan {

///////////////////////////////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////////////////////////////

Graph::Graph(const uint32_t num_nodes,
             const uint32_t num_districts)
    : num_nodes_(num_nodes),
      num_districts_(num_districts),
      state_pop_(0) {
  uint32_t i;
  nodes_ = new Node*[num_nodes_];
  districts_in_county_ = new unordered_map<uint32_t, unordered_set<uint32_t> *>;

  nodes_in_district_ = new unordered_set<uint32_t>*[num_districts_];
  for (i = 0; i < num_districts_; i++) {
    nodes_in_district_[i] = new unordered_set<uint32_t>;
  }

  nodes_on_perim_ = new unordered_set<uint32_t>*[num_districts_];
  for (i = 0; i < num_districts_; i++) {
    nodes_on_perim_[i] = new unordered_set<uint32_t>;
  }

  perim_nodes_to_neighbors_ =
        new unordered_map<uint32_t, unordered_set<uint32_t> *>*[num_districts_];
  for (i = 0; i < num_districts_; i++) {
    perim_nodes_to_neighbors_[i] = 
        new unordered_map<uint32_t, unordered_set<uint32_t> *>;
  }

  crossing_edges_ = new unordered_set<Edge, EdgeHash>;
  pop_of_district_ = new uint32_t[num_districts_];
  min_pop_of_district_ = new uint32_t[num_districts_];
  maj_pop_of_district_ = new uint32_t[num_districts_];

  for (i = 0; i < num_districts_; i++) {
    pop_of_district_[i] = 0;
    min_pop_of_district_[i] = 0;
    maj_pop_of_district_[i] = 0;
  }

  added_ids_ = new unordered_set<uint32_t>;
}

Graph::~Graph() {
  uint32_t i;

  // Delete non-arrays.
  delete crossing_edges_;
  delete added_ids_;

  // Delete non-pointer arrays.
  delete[] pop_of_district_;
  delete[] min_pop_of_district_;

  // Delete all node pointers in nodes_.
  unordered_set<uint32_t>::iterator itr; 
  for (itr = added_ids_->begin(); itr != added_ids_->end(); itr++) {
    delete nodes_[*itr];
  }
  delete[] nodes_;

  // Delete all set pointers in nodes_in_district_.
  for (i = 0; i < num_districts_; i++) {
    delete nodes_in_district_[i];
  }
  delete[] nodes_in_district_;

  // Delete all sets in districts_in_county_.
  for (i = 0; i < districts_in_county_->size(); i++) {
    delete (*districts_in_county_)[i];
  }
  delete districts_in_county_;

  // Delete all set pointers in nodes_on_perim_.
  for (i = 0; i < num_districts_; i++) {
    delete nodes_on_perim_[i];
  }
  delete[] nodes_on_perim_;

  // Delete all map pointers in perim_nodes_to_neighbors_.
  for (i = 0; i < num_districts_; i++) {
    delete perim_nodes_to_neighbors_[i];
  }
  delete[] perim_nodes_to_neighbors_;
}


///////////////////////////////////////////////////////////////////////////////
// Graph mutators
///////////////////////////////////////////////////////////////////////////////

bool Graph::AddNode(Node *node) {
  if (node->id_ > num_nodes_) {
    return false;
  }
  if (added_ids_->find(node->id_) != added_ids_->end()) {
    return false;
  }
  nodes_[node->id_] = node;
  state_pop_ += node->total_pop_;
  added_ids_->insert(node->id_);
  if (districts_in_county_->find(node->county_) ==
      districts_in_county_->end()) {
    (*districts_in_county_)[node->county_] = new unordered_set<uint32_t>;
  }
  if (node->district_ < num_districts_ && 
      (*districts_in_county_)[node->county_]->find(node->district_) ==
      (*districts_in_county_)[node->county_]->end()) {
    (*districts_in_county_)[node->county_]->insert(node->district_);
  }
  return true;
}

bool Graph::AddNode(const uint32_t id,
                    const uint32_t county,
                    const uint32_t majority_pop,
                    const uint32_t minority_pop) {
  if (id > num_nodes_) {
    return false;
  }
  if (added_ids_->find(id) != added_ids_->end()) {
    return false;
  }
  added_ids_->insert(id);
  state_pop_ += majority_pop + minority_pop;
  nodes_[id] = new Node(id, county, majority_pop, minority_pop);
  if (districts_in_county_->find(county) == districts_in_county_->end()) {
    (*districts_in_county_)[county] = new unordered_set<uint32_t>;
  }
  return true;
}

bool Graph::AddEdge(uint32_t node1, uint32_t node2) {
  nodes_[node1]->AddNeighbor(node2);
  nodes_[node2]->AddNeighbor(node1);
  return true;
}

bool Graph::MarkCrossingEdge(uint32_t node1, uint32_t node2) {
  Edge e(node1, node2);
  if (nodes_[node1]->district_ != nodes_[node2]->district_ &&
      ContainsEdge(node1, node2) &&
      crossing_edges_->find(e) == crossing_edges_->end()) {
    crossing_edges_->insert(e);
    return true;
  }
  return false;
}

void Graph::AddStatePop(uint32_t val) {
  state_pop_ += val;
}

bool Graph::AddNodeToDistrict(uint32_t node_id, uint32_t district) {
  if (nodes_in_district_[district]->find(node_id) !=
      nodes_in_district_[district]->end()) {
    return false;
  }
  nodes_[node_id]->district_ = district;
  nodes_in_district_[district]->insert(node_id);
  pop_of_district_[district] += nodes_[node_id]->GetTotalPop();
  maj_pop_of_district_[district] += nodes_[node_id]->GetMajorityPop();
  min_pop_of_district_[district] += nodes_[node_id]->GetMajorityPop();
  return true;
}

bool Graph::RemoveNodeFromDistrict(uint32_t node_id, uint32_t district) {
  if (nodes_in_district_[district]->find(node_id) ==
      nodes_in_district_[district]->end()) {
    return false;
  }
  nodes_in_district_[district]->erase(node_id);
  pop_of_district_[district] -= nodes_[node_id]->GetTotalPop();
  maj_pop_of_district_[district] -= nodes_[node_id]->GetMajorityPop();
  min_pop_of_district_[district] -= nodes_[node_id]->GetMinorityPop();
  nodes_[node_id]->district_ = num_districts_ + 1;
  return true;
}

bool Graph::AddNodeToDistrictPerim(uint32_t node_id, uint32_t district) {
  Node *node = nodes_[node_id];
  if (nodes_on_perim_[node->district_]->find(node->id_) !=
      nodes_on_perim_[node->district_]->end()) {
    return false;
  }
  nodes_on_perim_[node->district_]->insert(node->id_);

  unordered_map<uint32_t, unordered_set<uint32_t> *> *map;
  map = perim_nodes_to_neighbors_[district];
  if (map->find(node->id_) != map->end()) {
    return false;
  }
  map->insert({node->id_, node->neighbors_});
  return true;
}

bool Graph::RemoveNodeFromDistrictPerim(uint32_t node_id, uint32_t district) {
  Node *node = nodes_[node_id];
  if (nodes_on_perim_[district]->find(node->id_) ==
      nodes_on_perim_[district]->end()) {
    return false;
  }
  nodes_on_perim_[district]->erase(node->id_);

  unordered_map<uint32_t, unordered_set<uint32_t> *> *map;
  map = perim_nodes_to_neighbors_[district];
  if (map->find(node->id_) == map->end()) {
    return false;
  }
  map->erase(node->id_);
  
  return true;
}

bool Graph::UpdatePerimNode(Node *node) {
  if (node->id_ > num_nodes_ || node->district_ > num_districts_) {
    return false;
  }
  unordered_set<uint32_t> crossing_neighbors;
  for (auto &neighbor : *node->neighbors_) {
    if (neighbor > num_nodes_) {
      return false;
    }
    Edge e(node->id_, neighbor);
    if (nodes_[neighbor]->district_ != node->district_) {
      crossing_neighbors.insert(neighbor);
      if (crossing_edges_->find(e) == crossing_edges_->end()) {
        crossing_edges_->insert(e);
        if (!IsPerimNode(neighbor)) {
          AddNodeToDistrictPerim(neighbor, nodes_[neighbor]->district_);
        }
      }
    } else if (crossing_edges_->find(e) != crossing_edges_->end()) {
      crossing_edges_->erase(e);
      UpdatePerimNode(nodes_[neighbor]);
    }
  }
  if (crossing_neighbors.empty()) {
    perim_nodes_to_neighbors_[node->district_]->erase(node->id_);
  }
  return true;
}


///////////////////////////////////////////////////////////////////////////////
// Queries
///////////////////////////////////////////////////////////////////////////////

bool Graph::ContainsNode(const uint32_t id) const {
  return (added_ids_->find(id) != added_ids_->end());
}

bool Graph::ContainsEdge(const uint32_t node1_id,
                         const uint32_t node2_id) const {
  Node *node1 = nodes_[node1_id];
  return (node1->neighbors_->find(node2_id) != node1->neighbors_->end());
}

bool Graph::NodeExistsInDistrict(const uint32_t node_id,
                                 const uint32_t district) const {
  if (district >= num_districts_) {
    return false;
  }
  return (nodes_in_district_[district]->find(node_id) !=
          nodes_in_district_[district]->end());
}

bool Graph::IsPerimNode(const uint32_t node_id) const {
  if (perim_nodes_to_neighbors_[nodes_[node_id]->district_]->find(node_id) ==
      perim_nodes_to_neighbors_[nodes_[node_id]->district_]->end()) {
        return false;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

Node* Graph::GetNode(const uint32_t id) const {
  if (id > num_nodes_ || id < 0) {
    return nullptr;
  }
  return nodes_[id];
}

Node** Graph::GetNodes() const {
  return nodes_;
}

uint32_t Graph::GetNumNodes() const {
  return num_nodes_;
}

uint32_t Graph::GetNumDistricts() const {
  return num_districts_;
}

uint32_t Graph::GetStatePop() const {
  return state_pop_;
}

unordered_set<uint32_t>*
    Graph::GetNodesInDistrict(const uint32_t district) const {
  if (district > num_districts_ || district < 0) {
    return nullptr;
  }
  return nodes_in_district_[district];
}

unordered_set<uint32_t>* Graph::GetPerimNodes(uint32_t district) const {
  if (district > num_districts_ || district < 0) {
    return nullptr;
  }
  return nodes_on_perim_[district];
}

unordered_set<uint32_t>*
    Graph::GetPerimNodeNeighbors(const uint32_t district,
                                 const uint32_t node) const {
  if (district > num_districts_ || node > num_nodes_ ||
      !NodeExistsInDistrict(node, district) || !IsPerimNode(node)) {
    return nullptr;
  }
  return (*perim_nodes_to_neighbors_[district]->find(node)).second;
}

unordered_set<Edge, EdgeHash>* Graph::GetCrossingEdges() {
  return crossing_edges_;
}

int32_t Graph::GetDistrictPop(const uint32_t district) const {
  if (district > num_districts_ || district < 0) {
    return -1;
  }
  return pop_of_district_[district];
}

int32_t Graph::GetMinorityPop(const uint32_t district) const {
  if (district > num_districts_ || district < 0) {
    return -1;
  }
  return min_pop_of_district_[district];
}

}     // namespace rakan
