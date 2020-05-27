#ifndef SRC_GRAPH_H_
#define SRC_GRAPH_H_

#include <inttypes.h>       // for uint32_t
#include <stdio.h>          // for FILE *

#include <unordered_set>    // for std::unordered_set
#include <unordered_map>    // for std::unordered_map
#include <utility>          // for std::pair
#include <vector>           // for std::vector

#include "./Node.h"         // for Node class

using std::pair;
using std::unordered_set;
using std::unordered_map;
using std::vector;

namespace rakan {

class Graph {
 public:

  /////////////////////////////////////////////////////////////////////////////
  // Constructors and destructors
  /////////////////////////////////////////////////////////////////////////////

  /*
  * Default constructor. Used ONLY for testing.
  */
  Graph() = default;

  /*
  * Supplies the number of nodes and districts on this graph. Number of nodes,
  * districts, and state population must be non-negative.
  * 
  * @param    num_nodes       the number of nodes on this graph, must be >= 0
  * @param    num_districts   the number of districts on this graph, must
  *                           be >= 0
  * @param    state_pop       the population on this graph
  */
 Graph(const uint32_t num_nodes,
       const uint32_t num_districts,
       const uint32_t state_pop);

  /*
  * Default destructor. Also destructs nodes on this graph.
  */
  ~Graph();

  /////////////////////////////////////////////////////////////////////////////
  // Graph mutators
  /////////////////////////////////////////////////////////////////////////////

  /*
  * Adds a node to this graph.
  * 
  * @param    id
  * @param    county
  * @param    majority_pop
  * @param    minority_pop
  * 
  * @return true iff adding node successful, false otherwise
  */
  bool AddNode(const uint32_t id,
               const uint32_t county,
               const uint32_t majority_pop,
               const uint32_t minority_pop);

  /*
  * Adds an edge between the two supplied nodes. If either node does not
  * exist, adds nodes before adding edge. Assumes both nodes are on the graph.
  * 
  * @param    node1    the neighbor of node2
  * @param    node2    the neighbor of node1
  * 
  * @return true iff adding edge successful, false otherwise
  */
  bool AddEdge(uint32_t node1, uint32_t node2);

  /*
  * Adds to the state population.
  * 
  * @param    val   the value to add to the state population
  */
  void AddStatePop(uint32_t val);

  /*
  * Adds the given node to the district. Does NOT remove node from its old
  * district. Updates the population and demographics of the district
  * accordingly.
  * 
  * @param      node        the node to add
  * @param      district    the district to add the node to
  * 
  * @return true iff node does not already belong in district and addition
  *         successful, false otherwise
  */
  bool AddNodeToDistrict(uint32_t node, int district);

  /*
  * Removes the given node from the given district. Node must exist in district
  * before removal. Updates the population and demographics of the district
  * accordingly. Node will belong to a non-existent district afterwards.
  * 
  * @param      node        the node to remove
  * @param      district    the district to remove node from
  * 
  * @return true iff node exists in district and removal successful, false
  *         otherwise
  */
  bool RemoveNodeFromDistrict(uint32_t node, int district);

  /*
  * Adds the given node to the given district's set of perim nodes. Node must
  * exist in the district before addition.
  * 
  * @param      node        the perimeter node to add
  * @param      district    the district to add the perimeter node to
  * 
  * @return true iff node exists in district and addition to perim nodes list
  *         successful, false otherwise
  */
  bool AddNodeToDistrictPerim(uint32_t node, int district);

  /*
  * Removes the given node from the given district's set of perim nodes. Node
  * must be on the perimeter of the district before removal.
  * 
  * @param      node        the perimeter node to remove
  * @param      district    the district to remove the perimeter node from
  * 
  * @return true iff node is a perimeter node in given district and removal
  *         successful, false otherwise
  */
  bool RemoveNodeFromDistrictPerim(uint32_t node, int district);


  /////////////////////////////////////////////////////////////////////////////
  // Queries
  /////////////////////////////////////////////////////////////////////////////

  /*
  * Queries whether or not the node exists in the graph.
  * 
  * @param    node    the node to test for existence
  * 
  * @return true iff the node exists on the graph, false otherwise
  */
  bool ContainsNode(const Node *node) const;

  /*
  * Queries whether or not an edge exists between the two nodes.
  * 
  * @param    node1   the first node to test for an edge relationship
  * @param    node2   the second node to test for an edge relationship
  * 
  * @return true iff the nodes exist and an edge exists between them, false
  *         otherwise
  */
  bool ContainsEdge(const uint32_t node1, const uint32_t node2) const;

  /*
  * Queries whether or not the node exists in the district.
  * 
  * @param    node        the node to test for existence
  * @param    district    the district to test whether it contains node
  * 
  * @return true iff the node exists on the graph and is in distric, false
  *         otherwise
  */
  bool NodeExistsInDistrict(const uint32_t node, uint32_t district) const;


  /////////////////////////////////////////////////////////////////////////////
  // Accessors
  /////////////////////////////////////////////////////////////////////////////

  /*
  * Gets the node on this graph by its ID.
  * 
  * @param    id    the id of the node to get
  * 
  * @return a pointer to the node if it exists on this graph; nullptr if the
  *         node does not exist
  */
  Node* GetNode(const uint32_t id) const;

  /*
  * Gets the array of nodes on this graph.
  * 
  * @return the array of nodes on this graph; length is the num_nodes_ field
  */
  Node** GetNodes() const;

  /*
  * Gets the number of nodes on this graph.
  * 
  * @return the number of nodes on this graph as an unsigned 32-bit int
  */
  uint32_t GetNumNodes() const;

  /*
  * Gets the number of districts on this graph.
  * 
  * @return the number of districts on this graph as an unsigned 32-bit int
  */
  uint32_t GetNumDistricts() const;

  /*
  * Gets the state population of this graph.
  * 
  * @return the state population of this graph as an unsigned 32-bit int
  */
  uint32_t GetStatePop() const;

  /*
  * Gets the set of nodes in the given district.
  * 
  * @param    district      the district to get the nodes from
  * 
  * @return a pointer to the set of nodes in the district; nullptr if the
  *         district does not exist
  */
  unordered_set<int>* GetNodesInDistrict(const uint32_t district) const;

  /*
  * Gets the set of nodes on the given district's perimeter.
  * 
  * @param   district    the district to get the nodes on the perimeter from
  * 
  * @return a pointer to the set of nodes on the district perimeter; nullptr
  *         if the district does not exist
  */
  unordered_set<int>* GetPerimNodes(const uint32_t district) const;

  /*
  * Gets the set of neighbors of the node. Assumes the node is on the
  * perimeter of the given district.
  * 
  * @param    district    the district that the perim_node resides in
  * @param    node        the node to get the neighbors of
  * 
  * @return a pointer to the set of neighbor nodes of perim_node; nullptr
  *         if either the district or node does not exist
  */
  unordered_set<uint32_t>* GetPerimNodeNeighbors(const uint32_t district,
                                                 const uint32_t node) const;

  /*
  * Gets the total population of the given district.
  * 
  * @param    district    the district to get the total population of
  * 
  * @return the total population of the given district; -1 if the district
  *         does not exist
  */
  int32_t GetDistrictPop(const uint32_t district) const;

  /*
  * Gets the total minority population of the given district.
  * 
  * @param    district    the district to get the total miniroty population of
  * 
  * @return the total minority population of the given district; -1 if the
  *         district does not exist
  */
  int32_t GetMinorityPop(const uint32_t district) const;

 private:
  // The number of nodes on this graph.
  uint32_t num_nodes_;

  // The number of districts on this graph.
  uint32_t num_districts_;

  // The total state population of this graph.
  uint32_t state_pop_;

  // An array of all the nodes on this graph. The index of the array is
  // the node ID.
  Node **nodes_;

  // An array of pointers to sets. The index of the array
  // is the district ID, and the pointer at the index points
  // to a set of nodes in that district.
  unordered_set<int> **nodes_in_district_;

  // An array of pointers to sets. The index of the array
  // is the district ID, and the pointer at the index points
  // to a set of nodes on the perimeter of that district.
  unordered_set<int> **nodes_on_perim_;

  vector<pair<int, int>> *perim_edges_;

  // An array of maps. The index of the map is the district
  // ID. Each district map stores node IDs as keys, and each
  // node must be on the perimeter of the district. Corresponding
  // to each key contains a set of node IDs that are neighbors
  // to the perimeter node.
  unordered_map<int, unordered_set<uint32_t> *> **perim_nodes_to_neighbors_;

  // An array of populations. The index of the array is the district
  // ID. The value at that index corresponds to the population in
  // that district.
  uint32_t *pop_of_district_;

  // An array of majority populations. The index of the array is
  // the district ID. The value at that index corresponds to the
  // majority population in that district.
  uint32_t *maj_pop_of_district_;

  // An array of minority populations. The index of the array is
  // the district ID. The value at that index corresponds to the
  // minority population in that district.
  uint32_t *min_pop_of_district_;

  // Needed for populating data structures in graph from file.
  friend class Runner;
};        // class Graph

}         // namespace rakan

#endif    // SRC_GRAPH_H_