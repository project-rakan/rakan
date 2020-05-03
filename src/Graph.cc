#include "./Graph.h"

#include <bits/stdc++.h>    // for std::unordered_set
#include <inttypes.h>       // for uint32_t
#include <stdio.h>          // for FILE *, stderr
#include <unordered_map>    // for std::unordered_map
#include <vector>           // for std::vector

#include "ErrorCodes.h"     // for error codes
#include "Node.h"           // for Node class
#include "Reader.h"         // for reading index file

using std::unordered_set;
using std::unordered_map;

namespace rakan {

Graph::Graph() :
    num_nodes_(0),
    num_districts_(0),
    is_empty(true),
    alpha_(0),
    beta_(0),
    gamma_(0),
    eta_(0) {}

Graph::~Graph() {
  // Non-arrays.
  delete demographics;

  // Arrays.
  delete[] nodes_;
  delete[] nodes_in_district_;
  delete[] nodes_on_perim_;
  delete[] perim_nodes_to_neighbors_;
}

bool Graph::LoadGraph(FILE *file) {
  return reader_.ReadFileToGraph(file, this) == SUCCESS;
}

bool Graph::AddNode(const Node& node) {
  if (node.id_ > num_nodes_) {
    return false;
  }

  nodes_[node.id_] = node;
  return true;
}

bool Graph::AddEdge(Node& node1, Node& node2) {
  if (!ContainsNode(node1)) {
    AddNode(node1);
  }

  if (!ContainsNode(node2)) {
    AddNode(node2);
  }

  node1.neighbors_->push_back(node2.id_);
  node2.neighbors_->push_back(node1.id_);
  return true;
}

bool Graph::ContainsNode(const Node& node) {
  return nodes_[node.id_] == node;    // uses == in Node.cc
}

bool Graph::ContainsEdge(const Node& node1, const Node& node2) {
  // TODO
}

bool Graph::NodeExistsInDistrict(const Node& node, uint32_t district) {
  // TODO
}

unordered_set<int>* Graph::GetNodesInDistrict(uint32_t district) {
  // TODO
}

unordered_set<int>* Graph::GetPerimNodes(uint32_t district) {
  // TODO
}

unordered_set<int>* Graph::GetPerimNodeNeighbors(uint32_t district,
                                                 uint32_t perim_node) {
  // TODO
}

uint32_t GetTotalPop(uint32_t district);

uint32_t GetMinorityPop(uint32_t district);

}     // namespace rakan