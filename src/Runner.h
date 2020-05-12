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

  double ScoreCompactness();

  double ScorePopulationDistribution();

  double ScoreExistingBorders();

  double ScoreVRA();

  double LogScore();

  bool IsValid();

  double MetropolisHastings();

  uint16_t MakeMove(Node *node, int new_district_id);

  double Walk(int num_steps, string guid);

  bool IsEmptyDistrict(int old_district);

  bool IsDistrictSevered(Node *proposed_node);

  bool DoesPathExist(Node *start, Node *target);

  Graph *GetGraph() { return graph_; }

 private:
  Queue * queue_;
  string guid_;
  int num_steps_;
  unordered_map<int, int> *changes_;
  Graph *graph_;
  double score_;
};        // class Runner

}         // namespace rakan

#endif    // SRC_RUNNER_H_
