#ifndef SRC_NODE_H_
#define SRC_NODE_H_

#include <inttypes.h>         // for uint32_t
#include <string>             // for std::string
#include <unordered_set>      // for std::unordered_set
#include <unordered_map>      // for std::unordered_map

using std::string;
using std::unordered_set;
using std::unordered_map;

namespace rakan {

class Node {
 public:
  /////////////////////////////////////////////////////////////////////////////
  // Constructors, destructors, and operators
  /////////////////////////////////////////////////////////////////////////////

  // Default constructor.
  Node(const uint32_t id,
       const uint32_t county,
       const uint32_t majority_pop,
       const uint32_t minority_pop);

  // Destructor.
  ~Node();

  // Operator == for equality check.
  //
  // Arguments:
  //  - other: the other node to check for equality
  //
  // Returns:
  //  - true iff all fields in the nodes are the same
  //  - false otherwise
  bool operator== (const Node& other) const;

  /////////////////////////////////////////////////////////////////////////////
  // Accessors
  /////////////////////////////////////////////////////////////////////////////

  // Returns the district this node resides in.
  uint32_t GetDistrict() { return district_; }

  uint32_t GetCounty() { return county_; }

  uint32_t GetID() { return id_; }

  uint32_t GetTotalPop() { return total_pop_; }
  uint32_t GetMajorityPop() { return majority_pop_; }
  uint32_t GetMinorityPop() { return minority_pop_; }


  unordered_set<uint32_t>* GetNeighbors() { return neighbors_; }
  
  /////////////////////////////////////////////////////////////////////////////
  // Mutators
  /////////////////////////////////////////////////////////////////////////////

  // Adds other node as a neighbor to this node.
  bool AddNeighbor(const uint32_t neighbor);

  void SetCounty(const uint32_t county) { county_ = county; }

  void SetDistrict(const uint32_t district) { district_ = district; }

  // Sets the total population in this node to be val.
  void SetTotalPop(const uint32_t val) {
    total_pop_ = val;
  }

  void SetMajorityPop(const uint32_t val) {
    majority_pop_ = val;
  }

  void SetMinorityPop(const uint32_t val) {
    minority_pop_ = val;
  }

 private:
  // The unique node ID.
  uint32_t id_;

  // The county this node resides in.
  uint32_t county_;

  // The district this node resides in.
  uint32_t district_;

  uint32_t total_pop_;

  uint32_t majority_pop_;

  uint32_t minority_pop_;

  // The set of neighbors this node has.
  unordered_set<uint32_t> *neighbors_;

  // Needed for populating data structures in graph from file.
  friend class Runner;
  friend class Graph;
};        // class Node

}         // namespace rakan

#endif    // SRC_NODE_H_
