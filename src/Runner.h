#include "./Reader.h"

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

///////////////////////////////////////////////////////////////////////////////
// Building methods
///////////////////////////////////////////////////////////////////////////////
void Init();

void SeedDistricts();

///////////////////////////////////////////////////////////////////////////////
// Scoring methods
///////////////////////////////////////////////////////////////////////////////
void ScoreCompactness();

void ScorePopulationDistribution();

void ScoreExistingBorders();

void ScoreVRA();

void LogScore();

void MetropolisHastings();

void Walk();

///////////////////////////////////////////////////////////////////////////////
// Queue methods
///////////////////////////////////////////////////////////////////////////////
void MessageQueue();

void RetrieveQueue();

}
