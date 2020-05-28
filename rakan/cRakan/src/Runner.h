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

  /**
  * Default constructor. Used ONLY for testing.
  */
  Runner();

  /**
   * Constructs a Runner instance that will run on a graph with num_precincts
   * number of precincts and num_districts number of districts.
   * 
   * @param       num_precincts   the number of precincts on the running graph
   * @param       num_districts   the number of districts on the running graph
   */
  Runner(uint32_t num_precincts, uint32_t num_districts);

  /**
   * Destructor.
   */
  ~Runner();

  /**
   * Adds a new node to the graph this Runner instance runs on.
   * 
   * @param   node_id               the node id of the node to add
   * @param   county                the county the to be added node resides in
   * @param   majority_population   the majority population in the node
   * @param   minority_population   the minority population in the node
   */
  void add_node(uint32_t node_id,
                uint32_t county,
                uint32_t majority_population,
                uint32_t minority_population);

  /**
   * Adds an edge from node_one to node_two and from node_two to node_one
   * on the graph this Runner runs on. Two nodes must be on the graph.
   * 
   * @param       node_one      the node to add an edge from node_two to
   * @param       node_two      the node to add an edge from node_one to
   * 
   * @return true iff node_one and node_two are on the graph; false otherwise
   */
  bool add_edge(uint32_t node_one, uint32_t node_two);

  /**
  * Sets the district assignments according to the given vector. Vector is
  * interpreted as the node_id corresponding to the index of the district
  * number inside of the vector. Input vector must align with the number of
  * districts on the Runner graph.
  * 
  * @param    districts     a vector of uint32_t where each index of the vector
  *                         corresponds to the node id
  * 
  * @return true iff the input vector is valid
  */
  bool set_districts(vector<uint32_t>& districts);

  /**
  * Generates random seeds on the current graph. Randomly selects a number of
  * nodes to be the "center" of each district and assigns other nodes reachable
  * from the seed nodes to the respective district.
  * 
  * @return true iff seeding is successful (i.e, all precincts are reachable);
  *         false otherwise
  */
  bool seed();

  /**
  * Populates the graph's data structures. Prerequisites to this method depend
  * on the job.
  * 
  * If job is to generate a random seeding, walk on the randomly seeded graph,
  * and return the score of the graph, `add_node()`, `add_edge`, and `seed()`
  * must be called beforehand.
  * 
  * If job is to return the score of the graph without random seeding, then
  * `add_node()`, `add_edge()`, and `set_districts()` must be called
  * beforehand.
  */
  void populate();

  /**
   * Randomly selects num_districts number of indexes on the graph. Selects
   * those seeds as seed nodes to their distinct districts and returns the
   * set of seed nodes.
   * 
   * @return a pointer to a set of randomly generated seed nodes
   */
  unordered_set<Node *>* GenerateRandomSeeds();

  /**
   * Spawns districts on the graph by traversing the given seed nodes via BFS.
   * 
   * @param         seed_nodes        the pointer to the set of seed nodes
   * 
   * @return true iff all precincts are connected; false otherwise
   */
  bool SpawnDistricts(unordered_set<Node *> *seed_nodes);


 //////////////////////////////////////////////////////////////////////////////
 // Scoring
 //////////////////////////////////////////////////////////////////////////////

  /**
  * Scores the current graph according to the compactness function. 
  * Score is related to but unaffected by the parameter alpha.
  * 
  * @return the compactness score of the current graph
  */
  double ScoreCompactness();

  /**
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

  /**
  * Scores the current graph according to the Voter Rights Act
  * function. Score is related to but unaffected by the parameter
  * eta.
  * 
  * @return the VRA score of the current graph
  */
  double ScoreVRA();

  /**
  * Logs the score of the current graph. Takes the metrics given
  * into account (e.g. alpha, beta, gamma, eta).
  * 
  * @return the score of the current graph
  */
  double LogScore();


 //////////////////////////////////////////////////////////////////////////////
 // Algorithms
 //////////////////////////////////////////////////////////////////////////////

  /**
  * Implementation of the Metropolis-Hastings algorithm. Randomly
  * selects a precinct on the border of a district, attempts to
  * reassign that node to a neighbor district, and evaluates the
  * score of that redistricting.
  * 
  * @return the score of the random redistricting
  */
  double MetropolisHastings();

  /**
  * Makes a redistrcting move on the given node. Removes
  * the node from its old district and into the given district.
  * 
  * @param    node          the node to make the move on
  * @param    new_district  the new district ID to move node into
  * 
  * @return the score of this redistricting
  */
  double Redistrict(Node *node, int new_district);

  /**
  * Walks along the graph this Runner has loaded. Implements the
  * Metropolis-Hastings algorithm on the graph a given number of times.
  * 
  * @param    num_steps     the number of steps to take in this walk
  * @param    alpha         the weight to assign to the compactness score
  * @param    beta          the weight to assign to the population distribution
  *                         score
  * @param    gamma         the weight to assign to the existing border
  *                         respectfulness score
  * @param    eta           the weight to assign to the VRA score
  * 
  * @return a sum of all the scores accumulated during the walk
  */
  double Walk(uint32_t num_steps,
              double alpha, double beta, double gamma, double eta);


 //////////////////////////////////////////////////////////////////////////////
 // Queries
 //////////////////////////////////////////////////////////////////////////////

  /**
  * Queries whether or not a district is empty.
  * 
  * @param  district   the district to query
  * 
  * @return true iff the district is empty
  */
  bool IsEmptyDistrict(int district);

  /**
  * Queries whether or not the district that the proposed node is in will be
  * severed once the proposed node is removed.
  * 
  * @param    proposed_node   the node that will be hypothetically removed
  *                           from its district
  * 
  * @return true iff the district will be severed
  */
  bool IsDistrictSevered(Node *proposed_node);

  /**
  * Queries whether or not a path exists between the start node and the target
  * node. Path is only valid if all nodes traversed in the path are in the
  * same district.
  *
  * @param        start        the node to start the search at
  * @param        target       the node to look for
  *
  * @return true iff a path exists between start and target and nodes traversed
  * belong in the same district
  */
  bool DoesPathExist(Node *start, Node *target);

  /**
   * Gets the graph this instance runs on.
   * 
   * @return a pointer to the internal graph
   */
  Graph *GetGraph() { return graph_; }

  /**
   * Gets the list of changes of the graph on every step of the walk. The
   * changes are represented as a vector of uint32_t, where each index of the
   * vector represents the precinct ID, and the value at the index represents
   * the new district ID of that precinct.
   * 
   * @return a vector of vector of uint32_t, representing changes at every step
   */
  vector<vector<uint32_t>> getMaps();

  /**
  * Gets the list of scores for every step of the walk. The scores consist of
  * the total score, the compactness score, the population distribution score,
  * the existing-border respectfulness score, and the VRA score. These are
  * stored in a map from score name to score value.
  * 
  * @returns a vector of maps representing the scoring for each of the
  *          maps generated so far in the walk.
  */
  vector<map<string, double>> getScores();

 //////////////////////////////////////////////////////////////////////////////
 // Helpers
 //////////////////////////////////////////////////////////////////////////////

  /**
  * A helper function that implements BFS on the graph. Searches for any node
  * that exists in the given set from start.
  *
  * @param    start     the node to start the traversal from
  * @param    set       the set of target nodes
  *
  * @return the pointer to the first node found in set; nullptr if not found
  */
  Node *BFS(Node *start, unordered_set<uint32_t> *set);


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
  vector<vector <uint32_t> *> *walk_changes_;

  // A map containing all of the scores for each of the maps generated
  // in the walk, where each index here in the outer vector corresponds
  // to the map in the same index in walk_changes_
  vector<map <string, double> *> *scores_;

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
