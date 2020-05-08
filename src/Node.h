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
  Node();

  // Constructor with a unique ID.
  explicit Node(const uint32_t id);

  // Constructor with a unique ID and a district ID.
  Node(const uint32_t id, const uint32_t district);

  // Constructor with a unique ID, a district ID, and a set
  // of neighbors. Assumes the neighbors are dynamically
  // allocated via new.
  Node(const uint32_t id,
       const uint32_t district,
       unordered_set<uint32_t> *neighbors);

  // Destructor.
  ~Node() { delete neighbors_; delete demographics_; }

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

  uint32_t GetID() { return id_; }

  uint32_t GetArea() { return area_; }

  unordered_set<uint32_t>* GetNeighbors() { return neighbors_; }

  unordered_map<string, uint32_t>* GetDemographics() { return demographics_; }

  uint32_t GetTotalPop() { return (*demographics_)["total"]; }
  uint32_t GetMinPop() { return (*demographics_)["total"] - (*demographics_)["ca"]; }
  
  /////////////////////////////////////////////////////////////////////////////
  // Mutators
  /////////////////////////////////////////////////////////////////////////////

  // Adds other node as a neighbor to this node. Also adds
  // this node as a neighbor to the other node. The relationship
  // is bi-directional.
  //
  // Arguments:
  //  - other: the neighbor to this node
  //
  // Returns:
  //  - true iff the relationship is new
  //  - false otherwise
  bool AddNeighbor(const Node& other);

  void SetDistrict(const uint32_t district) { district_ = district; }

  // Sets the total population in this node to be val.
  void SetTotalPop(const uint32_t val) {
    demographics_->insert({"total", val});
  }

  // Sets the African American population in this node to
  // be val.
  void SetAAPop(const uint32_t val) {
    demographics_->insert({"aa", val});
  }

  // Sets the American Indian population in this node to
  // be val.
  void SetAIPop(const uint32_t val) {
    demographics_->insert({"ai", val});
  }

  // Sets the Asian population in this node to be val.
  void SetASPop(const uint32_t val) {
    demographics_->insert({"as", val});
  }

  // Sets the Caucasian population in this node to be val.
  void SetCAPop(const uint32_t val) {
    demographics_->insert({"cs", val});
  }

  // Sets the other population in this node to be val.
  void SetOtherPop(const uint32_t val) {
    demographics_->insert({"other", val});
  }

 private:
  // The unique node ID.
  uint32_t id_;

  // The area of this node.
  uint32_t area_;

  // The district this node resides in.
  uint32_t district_;

  // The set of neighbors this node has.
  unordered_set<uint32_t> *neighbors_;

  // A demographics map.
  unordered_map<string, uint32_t> *demographics_;

  // Needed for populating data structures in graph from file.
  friend class Runner;
  friend class Graph;
  friend class Reader;
};        // class Node

}         // namespace rakan

#endif    // SRC_NODE_H_
