#include "src/Graph.h"

#include <inttypes.h>       // for uint32_t
#include <stdio.h>          // for FILE *, stderr
#include <string>           // for std::string
#include <unordered_set>    // for std::unordered_set
#include <unordered_map>    // for std::unordered_map
#include <vector>           // for std::vector

#include "ErrorCodes.h"     // for error codes
#include "Node.h"           // for Node class

using std::string;
using std::unordered_set;
using std::unordered_map;

namespace rakan {

Graph::Graph(const uint32_t num_nodes, const uint32_t num_districts)
    : is_empty_(true),
      num_nodes_(num_nodes),
      num_districts_(num_districts),
      state_pop_(0) {
  // Initialize array of pointers fields.
  nodes_ = new Node*[num_nodes_];
  nodes_in_district_ = new unordered_set<int>*[num_districts_];
  area_of_district_ = new unordered_set<int>*[num_districts_];
  nodes_on_perim_ = new unordered_set<int>*[num_districts_];
  perim_nodes_to_neighbors_ =
                new unordered_map<int, unordered_set<int> *>*[num_districts_];

  // Initialize other array fields.
  demographics_ = new unordered_map<int, unordered_map<string, int> *>;
  pop_by_district_ = new uint32_t[num_districts_];
  min_pop_by_district_ = new uint32_t[num_districts_];
}

Graph::~Graph() {
  uint32_t i;

  // Delete non-array.
  delete demographics_;

  // Delete non-pointer arrays.
  delete[] pop_by_district_;
  delete[] min_pop_by_district_;

  // Delete all node pointers in nodes_.
  for (i = 0; i < num_nodes_; i++) {
    delete nodes_[i];
  }
  delete[] nodes_;

  // Delete all set pointers in nodes_in_district_.
  for (i = 0; i < num_districts_; i++) {
    delete nodes_in_district_[i];
  }
  delete[] nodes_in_district_;

  // Delete all set pointers in nodes_on_perim_.
  for (i = 0; i < num_districts_; i++) {
    delete nodes_on_perim_[i];
  }
  delete[] nodes_on_perim_;

  // Delete all map pointers in perim_nodes_to_neighbors_.
  for (i = 0; i < num_districts_; i++) {
    // Delete the set pointer.
    for (auto& kv : *perim_nodes_to_neighbors_[i]) {
      delete kv.second;
    }
    // Delete the map pointer.
    delete perim_nodes_to_neighbors_[i];
  }
  delete[] perim_nodes_to_neighbors_;
}

bool Graph::AddNode(const Node& node) {
  if (node.id_ > num_nodes_) {
    return false;
  }

  nodes_[node.id_] = &node;
  return true;
}

bool Graph::AddEdge(Node *node1, Node *node2) {
  if (!ContainsNode(*node1)) {
    AddNode(*node1);
  }

  if (!ContainsNode(*node2)) {
    AddNode(*node2);
  }

  return node1->AddNeighbor(*node2);
}

bool Graph::ContainsNode(const Node& node) const {
  return *nodes_[node.id_] == node;
}

bool Graph::ContainsEdge(const Node& node1, const Node& node2) const {
  return (node1.neighbors_->find(node2.id_)
          != node1.neighbors_->end());
}

bool Graph::NodeExistsInDistrict(const Node& node,
                                 const uint32_t district) const {
  return (nodes_in_district_[district]->find(node.id_)
          != nodes_in_district_[district]->end());
}

Node* Graph::GetNode(const uint32_t id) const {
  if (id > num_nodes_ || id < 0) {
    return nullptr;
  }
  return nodes_[id];
}

unordered_set<int>* Graph::GetNodesInDistrict(const uint32_t district) const {
  return nodes_in_district_[district];
}

unordered_set<int>* Graph::GetPerimNodes(uint32_t district) const {
  return nodes_on_perim_[district];
}

unordered_set<int>* Graph::GetPerimNodeNeighbors(const uint32_t district,
                                                 const uint32_t node) const {
  return (*perim_nodes_to_neighbors_[district]->find(node)).second;
}

uint32_t Graph::GetDistrictPop(const uint32_t district) const {
  return pop_by_district_[district];
}

uint32_t Graph::GetMinorityPop(const uint32_t district) const {
  return min_pop_by_district_[district];
}

unordered_set<int>** Graph::GetAllNodesPerDistrict() const {
  return nodes_in_district;
}

}     // namespace rakan
