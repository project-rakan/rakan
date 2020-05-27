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

  /**
  * Default constructor.
  * 
  * @param    id              the id to identify this node as
  * @param    county          the county this node resides in
  * @param    destrict        the district this node resides in
  * @param    total_pop       the total population this node contains
  * @param    majority_pop    the population that is of the majority race
  * @param    minority_pop    the population that is of the minority races
  */
  Node(const uint32_t id,
       const uint32_t county,
       const uint32_t district,
       const uint32_t total_pop,
       const uint32_t majority_pop,
       const uint32_t minority_pop);

  /**
   * Default destructor.
   */
  ~Node();

  /**
   * Operator == for equality check.
   * 
   * @param     other     the other node to check for equality
   * 
   * @return true iff all fields in the nodes are the same, false otherwise
   */
  bool operator== (const Node& other) const;


  /////////////////////////////////////////////////////////////////////////////
  // Accessors
  /////////////////////////////////////////////////////////////////////////////

  /**
   * Gets the ID of this node.
   * 
   * @return the node id
   */
  uint32_t GetID() { return id_; }

  /**
   * Gets the county this node resides in.
   * 
   * @return the county
   */
  uint32_t GetCounty() { return county_; }

  /**
   * Gets the district this node resides in.
   * 
   * @return the district
   */
  uint32_t GetDistrict() { return district_; }

  /**
   * Gets the total population of this node.
   * 
   * @return the total population
   */
  uint32_t GetTotalPop() { return total_pop_; }

  /**
   * Gets the majority population of this node.
   * 
   * @return the majority population
   */
  uint32_t GetMajorityPop() { return majority_pop_; }

  /**
   * Gets the minority population of this node.
   * 
   * @return the majority population
   */
  uint32_t GetMinorityPop() { return minority_pop_; }

  /**
   * Gets the set of neighbors of this node.
   * 
   * @return a pointer to the set of neighbors
   */
  unordered_set<uint32_t>* GetNeighbors() { return neighbors_; }
  
  /////////////////////////////////////////////////////////////////////////////
  // Mutators
  /////////////////////////////////////////////////////////////////////////////

  /**
   * Adds the specified neighbor to the neighbor list.
   * 
   * @param     neighbor    the id of the neighbor to add
   * 
   * @return true iff the neighbor is not already in the neighbor list, false
   *         otherwise
   */
  bool AddNeighbor(const uint32_t neighbor);

  /**
   * Sets the county to the specified county.
   * 
   * @param         county        the new county
   */
  void SetCounty(const uint32_t county) { county_ = county; }

  /**
   * Sets the district to the specified county.
   * 
   * @param         district      the new district
   */
  void SetDistrict(const uint32_t district) { district_ = district; }

  /**
   * Sets the total population to the specified val.
   * 
   * @param         val     the new total population
   */
  void SetTotalPop(const uint32_t val) { total_pop_ = val; }

  /**
   * Sets the majority population to the specified val.
   * 
   * @param         val     the new majority population
   */
  void SetMajorityPop(const uint32_t val) { majority_pop_ = val; }

  /**
   * Sets the minority population to the specified val.
   * 
   * @param         val     the new minority population
   */
  void SetMinorityPop(const uint32_t val) { minority_pop_ = val; }

 private:
  // The unique node ID.
  uint32_t id_;

  // The county this node resides in.
  uint32_t county_;

  // The district this node resides in.
  uint32_t district_;

  // The total population of this node.
  uint32_t total_pop_;

  // The total majority population of this node.
  uint32_t majority_pop_;

  // The total minority population of this node.
  uint32_t minority_pop_;

  // The set of neighbors this node has.
  unordered_set<uint32_t> *neighbors_;

  // Needed for populating data structures in graph from file.
  friend class Runner;
  friend class Graph;
};        // class Node

}         // namespace rakan

#endif    // SRC_NODE_H_
