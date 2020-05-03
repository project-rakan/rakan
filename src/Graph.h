#ifndef GRAPH_H_
#define GRAPH_H_

#include <bits/stdc++.h>    // for std::unordered_set
#include <inttypes.h>       // for uint32_t
#include <stdio.h>          // for FILE *
#include <memory>           // for std::unique_ptr
#include <unordered_map>    // for std::unordered_map
#include <vector>           // for std::vector

#include "Node.h"           // for Node class
#include "Reader.h"         // for reading index file

using std::unordered_set;
using std::unordered_map;
using std::vector;

namespace rakan {

// Global reader.
Reader reader_();

class Graph {
 public:
  /////////////////////////////////////////////////////////////////////////////
  // Constructors and destructors
  /////////////////////////////////////////////////////////////////////////////

  // Default constructor.
  Graph();

  // Default destructor. Also destructs nodes on this graph.
  ~Graph();

  /////////////////////////////////////////////////////////////////////////////
  // Graph mutating methods
  /////////////////////////////////////////////////////////////////////////////

  // Loads the graph from the given file if supplied. If not,
  // loads an empty graph.
  //
  // Arguments:
  //  - file: a path to the index file to load from; if nullptr,
  //          loads an empty graph
  //
  // Returns:
  //  - true iff loading successful
  //  - false otherwise
  bool LoadGraph(const FILE *file = nullptr);

  // Adds a node to this graph.
  //
  // Arguments:
  //  - node: the node to add
  //
  // Returns:
  //  - true iff adding node successful
  //  - false if node cannot be added
  bool AddNode(const Node& node);

  // Adds an edge between the two supplied nodes. If either node does not
  // exist, adds nodes before adding edge.
  //
  // Arguments:
  //  - node1: the neighbor of node2
  //  - node2: the neighbor of node1
  //
  // Returns:
  //  - true iff adding edge successful
  //  - false if the edge cannot be added
  bool AddEdge(Node& node1, Node& node2);

  /////////////////////////////////////////////////////////////////////////////
  // Query methods
  /////////////////////////////////////////////////////////////////////////////

  // Queries whether or not the node exists in the graph.
  //
  // Arguments:
  //  - node: the node to test for existence
  //
  // Returns:
  //  - true iff the node exists on the graph
  //  - false otherwise
  bool ContainsNode(const Node& node) const;

  // Queries whether or not an edge exists between the two nodes.
  //
  // Arguments:
  //  - node1: the first node to test for an edge relationship
  //  - node2: the second node to test for an edge relationship
  //
  // Returns:
  //  - true iff the nodes exist and an edge exists between them
  //  - false otherwise
  bool ContainsEdge(const Node& node1, const Node& node2) const;

  // Queries whether or not the node exists in the district.
  //
  // Arguments:
  //  - node: the node to test for existence
  //  - district: the district to test whether it contains node
  //
  // Returns:
  //  - true iff the node exists on the graph and is in distric
  //  - false otherwise
  bool NodeExistsInDistrict(const Node& node, uint32_t district) const;

  /////////////////////////////////////////////////////////////////////////////
  // Accessor methods
  /////////////////////////////////////////////////////////////////////////////

  // Gets the set of nodes in the given district.
  //
  // Arguments:
  //  - district: the district to get the nodes from
  //
  // Returns:
  //  - a pointer to the set of nodes in the district
  unordered_set<int>* GetNodesInDistrict(const uint32_t district) const;

  // Gets the set of nodes on the given district's perimeter.
  //
  // Arguments:
  //  - district: the district to get the nodes on the perimeter from
  //
  // Returns:
  //  - a pointer to the set of nodes on the district perimeter
  unordered_set<int>* GetPerimNodes(const uint32_t district) const;

  // Gets the set of neighbors of the node. Assumes the node is on the
  // perimeter of the given district.
  //
  // Arguments:
  //  - district: the district that the perim_node resides in
  //  - node: the node to get the neighbors of
  //
  // Returns:
  //  - a pointer to the set of neighbor nodes of perim_node
  unordered_set<int>* GetPerimNodeNeighbors(const uint32_t district,
                                            const uint32_t node) const;

  // Gets the total population of the given district.
  //
  // Arguments:
  //  - district: the district to get the total population of
  //
  // Returns:
  //  - the total population of the given district
  uint32_t GetTotalPop(const uint32_t district) const;

  // Gets the total minority population of the given district.
  //
  // Arguments:
  //  - district: the district to get the total miniroty population of
  //
  // Returns:
  //  - the total minority population of the given district
  uint32_t GetMinorityPop(const uint32_t district) const;

  /////////////////////////////////////////////////////////////////////////////
  // Mutator methods
  /////////////////////////////////////////////////////////////////////////////

  void SetAlpha(const double val) { alpha_ = val; }
  void SetBeta(const double val) { beta_ = val; }
  void SetGamma(const double val) { gamma_ = val; }
  void SetEta(const double val) { eta_ = val; }

 private:
  uint32_t num_nodes_;
  uint32_t num_districts_;
  bool is_empty_;

  // An array of all the nodes on this graph. The index of the array is
  // the node ID.
  Node *nodes_;

  // An array of pointers to sets. The index of the array
  // is the district ID, and the pointer at the index points
  // to a set of nodes in that district.
  unordered_set<int> *nodes_in_district_;

  // An array of pointers to sets. The index of the array
  // is the district ID, and the pointer at the index points
  // to a set of nodes on the perimeter of that district.
  unordered_set<int> *nodes_on_perim_;

  // An array of maps. The index of the map is the district
  // ID. Each district map stores node IDs as keys, and each
  // node must be on the perimeter of the district. Corresponding
  // to each key contains a set of node IDs that are neighbors
  // to the perimeter node.
  unordered_map<int, unordered_set<int> *> *perim_nodes_to_neighbors_;
  
  // A map of the district ID to another map of the demographics
  // in that district.
  unordered_map<int, unordered_map<string, int> *> *demographics;

  // An array of populations. The index of the array is the district
  // ID. The value at that index corresponds to the population in
  // that district.
  uint32_t *pop_by_district_;

  // An array of minority populations. The index of the array is
  // the district ID. The value at that index corresponds to the
  // minority population in that district.
  uint32_t *min_pop_by_district_;

  // Metric parameters.
  double alpha_;
  double beta_;
  double gamma_;
  double eta_;

  // Needed for populating data structures in graph from file.
  friend class Node;
  friend class Reader;
};        // class Graph

}         // namespace rakan

#endif    // GRAPH_H_
