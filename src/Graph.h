#ifndef GRAPH_H_
#define GRAPH_H_

#include <inttypes.h>      // for uint32_t
#include <stdio.h>         // for FILE *
#include <unordered_map>   // for std::unordered_map
#include <vector>          // for std::vector

using std::unordered_map;
using std::vector;

namespace rakan {

class Graph {
 public:
  // Default constructor.
  Graph();

  // Default destructor. Also destructs nodes on this graph.
  ~Graph();

  // Loads the graph from the given file if supplied. If not,
  // loads an empty graph.
  //
  // Arguments:
  //  - num_districts: the maximum number of districts this graph
  //                   can have
  //  - file: a path to the index file to load from; if nullptr,
  //          loads an empty graph
  //
  // Returns:
  //  - true iff loading successful
  //  - false otherwise
  bool LoadGraph(uint32_t num_districts, FILE *file = nullptr);

  // Adds a node to this graph.
  //
  // Arguments:
  //  - node: the node to add
  //
  // Returns:
  //  - true iff adding node successful
  //  - false if node already exists
  bool AddNode(Node& node);

  // Adds an edge between the two supplied nodes. Both nodes
  // must exist on the graph.
  //
  // Arguments:
  //  - node1: the neighbor of node2
  //  - node2: the neighbor of node1
  //
  // Returns:
  //  - true iff adding edge successful
  //  - false if the edge already exists
  bool AddEdge(Node& node1, Node& node2);

  // Returns a vector of all nodes on this graph.
  vector<Node>& get_nodes() { return nodes_; }

  // Mutators for metric parameters.
  //
  // Arguments:
  //  - val: the value to set the corresponding metric
  //         parameter to
  void set_alpha(int val) { alpha_ = val; }
  void set_beta(int val) { beta_ = val; }
  void set_gamma(int val) { gamma_ = val; }
  void set_eta(int val) { eta_ = val; }

 private:
  // A vector of all the nodes on this graph.
  vector<Node> nodes_;

  // A map of the district ID to the nodes in it.
  unordered_map<int, int> nodes_in_district_;

  // A map of the district ID to the nodes on its perimeter.
  unordered_map<int, int> nodes_on_perim_;

  // A map of the district ID to another map of the nodes
  // on the perimeter of that district to the list of neighbors
  // in another district.
  unordered_map<int, unordered_map<int, vector<int>>>
                                    perim_nodes_to_neighbors_;
  
  // A map of the district ID to another map of the demographics
  // in that district.
  unordered_map<int, unordered_map<string, int>> demographics;

  // Metric parameters.
  double alpha_;
  double beta_;
  double gamma_;
  double eta_;
};        // class Graph

}         // namespace rakan

#endif    // GRAPH_H_
