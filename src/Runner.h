#ifndef SRC_RUNNER_H_
#define SRC_RUNNER_H_

#include <bits/stdc++.h>      // for std::unordered_set
#include <inttypes.h>         // for uint32_t
#include <stdio.h>            // for FILE *, fread, fseek
#include <string>
#include <unordered_map>      // for std::unordered_map
#include <utility>            // for std::pair
#include <vector>             // for std::vector

#include "./Graph.h"          // for Graph class
#include "./Node.h"           // for Node class
#include "./Queue.h"

using std::string;
using std::pair;
using std::unordered_set;
using std::unordered_map;

namespace rakan {

class Runner {
 public:
  Runner(Queue * queue) : num_steps_(0), queue_(queue) {
    changes_ = new unordered_map<int, int>;
  }

  // for testing
  uint16_t LoadGraph(Graph *graph) { graph_ = graph; }

  uint16_t LoadGraph(FILE *file);

  uint16_t SetDistricts(unordered_map<uint32_t, uint32_t> *map);

  uint16_t SeedDistricts();

  uint16_t PopulateGraphData();

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
  * Walks along the graph this Runner has loaded. Implements
  * the Metropolis-Hastings algorithm on the graph a given
  * number of times. Walk is triggered by a request with a
  * specified GUID.
  * 
  * @param    num_steps     The number of steps to take in
  *                         this walk
  * @param    guid          The GUID associated with the
  *                         request that started this walk
  * 
  * @return a sum of all the scores accumulated during the
  *         walk
  */
  double Walk(int num_steps, string guid);

  /*
  * Queries whether or not a district is empty.
  * 
  * @param  district   The district to query
  * 
  * @return true iff the district is empty
  */
  bool IsEmptyDistrict(int district);

  /*
  * Queries whether or not the district that the proposed node
  * is in will be severed once the proposed node is removed.
  * 
  * @param    proposed_node   The node that will be hypothe-
  *                           tically removed from its district
  * 
  * @return true iff the district will be severed
  */
  bool IsDistrictSevered(Node *proposed_node);

  /*
  * Queries whether or not a path exists between the start node
  * and the target node. Path is only valid if all nodes traversed
  * in the path are in the same district.
  *
  * @param   start   The node to start the search at
  * @param   target  The node to look for
  *
  * @return true iff a path exists between start and target
  *         and nodes traversed belong in the same district
  */
  bool DoesPathExist(Node *start, Node *target);

  /*
  * Submits the list of changes to the queue.
  *
  * @param   changes   The map of changes this Runner has
  *                    made since the last step
  */
  void SubmitToQueue(unordered_map<int, int> *changes);

  /*
  * Returns the graph loaded by this Runner.
  * 
  * @return the pointer pointing to this graph
  */
  Graph *GetGraph() { return graph_; }

 private:
  // The graph that is loaded and evaluated by this Runner.
  Graph *graph_;

  // The queue this Runner is hooked up to. This runner will solely
  // communicate with this queue.
  Queue * queue_;

  // The GUID of the job currently in progress.
  string guid_;
  
  // A map of the changes that have been made since the last walk.
  // Maps from a node ID to a district ID and assumes that this change
  // is new.
  unordered_map<int, int> *changes_;

  // The number of steps to take per walk.
  int num_steps_;

  // Variables to keep track of the scores of the current map.
  double score_;
  double compactness_score_;
  double distribution_score_;
  double border_score_;
  double vra_score_;
};        // class Runner

}         // namespace rakan

#endif    // SRC_RUNNER_H_
