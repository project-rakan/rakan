#ifndef SRC_RUNNER_H_
#define SRC_RUNNER_H_

#include <bits/stdc++.h>      // for std::unordered_set
#include <inttypes.h>         // for uint32_t
#include <stdio.h>            // for FILE *, fread, fseek
#include <unordered_map>      // for std::unordered_map
#include <vector>             // for std::vector

#include "./ErrorCodes.h"     // for error codes
#include "./Graph.h"          // for Graph class
#include "./Node.h"           // for Node class

using std::unordered_set;
using std::unordered_map;

namespace rakan {

class Runner {
 public:
  Runner();

  Graph* LoadGraph(FILE *file);

  void SeedDistricts();

  void Verify(uint16_t result);

  double ScoreCompactness(uint32_t num_nodes, uint32_t num_districts);

  double ScorePopulationDistribution(uint32_t *pop_by_district,
                                     uint32_t num_districts);

  double ScoreExistingBorders();

  double ScoreVRA();

  double LogScore();

  void MetropolisHastings();

  void Walk();
};        // class Runner

}         // namespace rakan

#endif    // SRC_RUNNER_H_
