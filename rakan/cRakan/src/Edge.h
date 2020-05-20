#ifndef SRC_EDGE_H_
#define SRC_EDGE_H_

#include <inttypes.h>         // for uint32_t
#include <string>             // for std::string
#include <unordered_set>      // for std::unordered_set
#include <unordered_map>      // for std::unordered_map
#include "./Node.h"             // for Node access

using std::string;
using std::unordered_set;
using std::unordered_map;

namespace rakan {

class Edge {
 public:
  /////////////////////////////////////////////////////////////////////////////
  // Constructors, destructors, and operators
  /////////////////////////////////////////////////////////////////////////////

  // Default constructor.
  Edge() = default;

  // Takes in the two nodes to be stored for usage later, as well
  // as the districts that each node belongs in.
  // 
  // Arguments: 
  // - one: The precinct id of the first node to be added to the edge
  // - two: The precinct id of the second node to be added to the edge
  // - dist_one: The district num of the first node to be added to the edge
  // - dist_two: The district num of the second node to be added to the edge
  Edge(const uint32_t one,
       const uint32_t two,
       const uint32_t dist_one,
       const uint32_t dist_two);
       
  // Takes in the two nodes to be stored for usage later. Sets cross_ to false
  // by default.
  Edge(const uint32_t one, const uint32_t two);

  // Destructor.
  ~Edge() { }

  // Operator == for equality check.
  //
  // Arguments:
  //  - other: the other edge to check for equality
  //
  // Returns:
  //  - true iff all fields in the edges are the same
  //  - false otherwise
  bool operator== (const Edge& other) const;

  /////////////////////////////////////////////////////////////////////////////
  // Accessors
  /////////////////////////////////////////////////////////////////////////////

  // Returns whether or not the two nodes are in separate districts.
  bool Crosses() const { return cross_; }

  const uint32_t GetNodeOne() const { return node_one_; }

  const uint32_t GetNodeTwo() const { return node_two_; }
  
  /////////////////////////////////////////////////////////////////////////////
  // Mutators
  /////////////////////////////////////////////////////////////////////////////

  // Adds 
  //
  // Arguments:
  //  - one: first node id to be assigned to edge.
  //  - two: second node id to be assigned to edge.
  //  - cross: Whether the two nodes are in the same district.
  //
  // Returns:
  //  - true iff the relationship is new
  //  - false otherwise
  bool ChangeConnection(uint32_t one, uint32_t two, bool cross);

 private:
  bool cross_;
  uint32_t node_one_;
  uint32_t node_two_;

  // Needed for populating data structures in graph from file.
  friend class Runner;
  friend class Graph;
  friend class Reader;
  friend class Node;
};        // class Edge

}         // namespace rakan

#endif    // SRC_EDGE_H_

