#ifndef NODE_H_
#define NODE_H_

#include <string>           // for std::string
#include <unordered_set>    // for std::unordered_set
#include <unordered_map>    // for std::unordered_map
#include <vector>           // for std::vector

using std::string;
using std::unordered_set;
using std::unordered_map;
using std::vector;

namespace rakan {

class Node {
 public:
  // Default constructor.
  Node();

  // Constructor with a unique ID.
  Node(const int id);

  // Constructor with a unique ID and a district ID.
  Node(const int id, const int district);

  // Constructor with a unique ID, a district ID, and a list
  // of neighbors.
  Node(const int id, const int district, const unordered_set<int>& neighbors);

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

  // Returns the district this node resides in.
  uint32_t get_district() { return district_; }

  // Sets the total population in this node to be val.
  void set_tot_population(const uint32_t val) {
    demographics_->insert(std::make_pair<>("tot", val));
  }

  // Sets the African American population in this node to
  // be val.
  void set_aa_population(const uint32_t val) {
    demographics_->insert(std::make_pair<>("aa", val));
  }

  // Sets the American Indian population in this node to
  // be val.
  void set_ai_population(const uint32_t val) {
    demographics_->insert(std::make_pair<>("ai", val));
  }

  // Sets the Asian population in this node to be val.
  void set_as_population(const uint32_t val) {
    demographics_->insert(std::make_pair<>("as", val));
  }

  // Sets the Caucasian population in this node to be val.
  void set_ca_population(const uint32_t val) {
    demographics_->insert(std::make_pair<>("cs", val));
  }

  // Sets the other population in this node to be val.
  void set_o_population(const uint32_t val) {
    demographics_->insert(std::make_pair<>("o", val));
  }

 private: 
  // The unique node ID.
  uint32_t id_;

  // The district this node resides in.
  uint32_t district_;

  // The set of neighbors this node has.
  unordered_set<uint32_t> *neighbors_;

  // A demographics map.
  unordered_map<string, uint32_t> *demographics_;

  // Needed for populating data structures in graph from file.
  friend class Graph;
  friend class Reader;
};        // class Node

// Adds the two nodes as neighbors. The relationship is
// bi-directional.
//
// Arguments:
//  - node1: the neighbor to node2
//  - node2: the neighbor to node1
//
// Returns:
//  - true iff the relationship is new
//  - false otherwise
bool AddNeighbor(Node& node1, Node& node2);

}         // namespace rakan

#endif    // NODE_H_