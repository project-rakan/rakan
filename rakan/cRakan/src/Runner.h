#ifndef SRC_RUNNER_H_
#define SRC_RUNNER_H_
 
#include <inttypes.h>         // for uint32_t, uint16_t, etc.

#include <string>             // for std::string
#include <unordered_map>      // for std::unordered_map
#include <unordered_set>      // for std::unordered_set
#include <map>                // for std::map

#include "./Graph.h"          // for Graph class
#include "./Node.h"           // for Node class

using std::string;
using std::unordered_set;
using std::unordered_map;
using std::map;

namespace rakan {

class Runner {
 public:

 //////////////////////////////////////////////////////////////////////////////
 // Construction / Initialization
 //////////////////////////////////////////////////////////////////////////////
  /*
  * Default constructor.
  */
  Runner();

  Runner(uint32_t num_precincts, uint32_t num_districts);

  void add(uint32_t node_id, uint32_t county, uint32_t minority, uint32_t majority);

  void add_edge(uint32_t node_one, uint32_t node_two);

  // Returns a vector of vectors of uint32_t, where each vector
  // represents the changes made to the graph in each step of 
  // the walk.
  vector<vector<uint32_t>&>& getMaps();
  /*
  * Sets the district assignments according to the given vector.
  * Vector is interpreted as the node_id corresponding to the index
  * of the district number inside of the vector.
  * 
  * @param    districts     a vector of uint32_t where each index of
  *                         the vector corresponds to the node id.
  * 
  * @return SUCCESS if all assignment successful; the appropriate
  *         error code otherwise
  */
  void set_districts(vector<uint32_t>& districts);

  /*
  * Generates random seeds on the current graph. Randomly selects
  * a number of nodes to be the "center" of each district and assigns
  * other nodes reachable from the seed nodes to the respective
  * district.
  * 
  */
  void SeedDistricts();

  /*
  * Populates the graph's data structures.
  * 
  */
  void populate();

  /*
  * returns a vector of maps representing the scoring for each of the
  * maps generated so far in the walk.
  */
  vector<map<string, double>>& getScores();



 //////////////////////////////////////////////////////////////////////////////
 // Scoring
 //////////////////////////////////////////////////////////////////////////////

  /*
  * Scores the current graph according to the compactness function. 
  * Score is related to but unaffected by the parameter alpha.
  * 
  * @return the compactness score of the current graph
  */
  double ScoreCompactness();

  /*
  * Scores the current graph according to the population distribution
  * function. Score is related to but unaffected by the parameter beta.
  * 
  * @return the population distribution score of the current graph
  */
  double ScorePopulationDistribution();

  /*
  * Scores the current graph according to how closely it resembles
  * existing borders. Score is related to but unaffected by the
  * parameter gamma.
  * 
  * @return the existing-border score of the current graph
  */
  double ScoreExistingBorders();

  /*
  * Scores the current graph according to the Voter Rights Act
  * function. Score is related to but unaffected by the parameter
  * eta.
  * 
  * @return the VRA score of the current graph
  */
  double ScoreVRA();

  /*
  * Logs the score of the current graph. Takes the metrics given
  * into account (e.g. alpha, beta, gamma, eta).
  * 
  * @return the score of the current graph
  */
  double LogScore();


 //////////////////////////////////////////////////////////////////////////////
 // Algorithms
 //////////////////////////////////////////////////////////////////////////////

  /*
  * Implementation of the Metropolis-Hastings algorithm. Randomly
  * selects a precinct on the border of a district, attempts to
  * reassign that node to a neighbor district, and evaluates the
  * score of that redistricting.
  * 
  * @return the score of the random redistricting
  */
  double MetropolisHastings();

  /*
  * Makes a redistrcting move on the given node. Removes
  * the node from its old district and into the given district.
  * 
  * @param    node          The node to make the move on
  * @param    new_district  The new district ID to move node into
  * 
  * @return the score of this redistricting
  */
  double Redistrict(Node *node, int new_district);

  /*
  * Walks along the graph this Runner has loaded. Implements the
  * Metropolis-Hastings algorithm on the graph a given number of times.
  * 
  * @param    num_steps     The number of steps to take in this walk
  * 
  * @return a sum of all the scores accumulated during the walk
  */
  double Walk(uint32_t num_steps, double alpha, double beta, double gamma, double eta);



 //////////////////////////////////////////////////////////////////////////////
 // Queries
 //////////////////////////////////////////////////////////////////////////////

  /*
  * Queries whether or not a district is empty.
  * 
  * @param  district   The district to query
  * 
  * @return true iff the district is empty
  */
  bool IsEmptyDistrict(int district);

  /*
  * Queries whether or not the district that the proposed node is in will be
  * severed once the proposed node is removed.
  * 
  * @param    proposed_node   The node that will be hypothetically removed
  *                           from its district
  * 
  * @return true iff the district will be severed
  */
  bool IsDistrictSevered(Node *proposed_node);

  /*
  * Queries whether or not a path exists between the start node and the target
  * node. Path is only valid if all nodes traversed in the path are in the
  * same district.
  *
  * @param   start   The node to start the search at
  * @param   target  The node to look for
  *
  * @return true iff a path exists between start and target and nodes traversed
  * belong in the same district
  */
  bool DoesPathExist(Node *start, Node *target);


 //////////////////////////////////////////////////////////////////////////////
 // Helpers
 //////////////////////////////////////////////////////////////////////////////

  /*
  * A static helper function that implements BFS on the graph. Searches for
  * any node that exists in the given set from start.
  *
  * @param    start     The node to start the traversal from
  * @param    set       The set of target nodes
  *
  * @return the pointer to the first node found in set; nullptr if not found
  */
  Node *BFS(Node *start, unordered_set<Node *> *set);


 private:
  // The graph that is loaded and evaluated by this Runner.
  Graph *graph_;

  // A map of the changes that have been made since the last walk.
  // Maps from a node ID to a district ID and assumes that this change
  // is new.
  unordered_map<int, int> *changes_;

  // The number of steps to take per walk.
  int num_steps_;

  // A vector containing multiple vectors of uint32_t where each vector
  // represents the state of the map after a single step in the walk.
  vector<vector <uint32_t> *>* walk_changes_;

  // Variables to keep track of the scores of the current map.
  double score_;
  double compactness_score_;
  double distribution_score_;
  double border_score_;
  double vra_score_;

  // Weights of scoring metrics.
  double alpha_;
  double beta_;
  double gamma_;
  double eta_;
};        // class Runner

}         // namespace rakan

#endif    // SRC_RUNNER_H_
