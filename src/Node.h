#ifndef NODE_H_
#define NODE_H_

#include <string>           // for std::string
#include <unordered_map>    // for std::unordered_map
#include <vector>           // for std::vector

using std::string;
using std::unordered_map;
using std::vector;

namespace rakan {

class Node {
 public:
  // Default constructor.
  Node();

  // Constructor with a unique ID.
  Node(int id);

  // Constructor with a unique ID and a district ID.
  Node(int id, int district);

  // Constructor with a unique ID, a district ID, and a list
  // of neighbors.
  Node(int id, int district, vector<int>& neighbors);

  // Destructor.
  ~Node();

  // Adds a neighbor to this node. A neighbor relationship
  // is bi-directional.
  //
  // Arguments:
  //  - other: the neighbor to add, also adds this node to
  //           the other's list of neighbors
  //
  // Returns:
  //  - true iff the relationship is new
  //  - false otherwise
  bool AddNeighbor(Node& other);

  // Returns the district this node resides in.
  int get_district() { return district_; }

 private: 
  // The unique node ID.
  int id_;

  // The district this node resides in.
  int district_;

  // The list of neighbors this node has.
  vector<int> neighbors_;

  // A demographics map.
  unordered_map<string, int> demographics;
};        // class Node

}         // namespace rakan

#endif    // NODE_H_