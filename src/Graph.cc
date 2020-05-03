#include "./Graph.h"

#include <bits/stdc++.h>    // for std::unordered_set
#include <inttypes.h>      // for uint32_t
#include <stdio.h>         // for FILE *
#include <unordered_map>   // for std::unordered_map
#include <vector>          // for std::vector

#include "Reader.h"        // for reading index file

using std::unordered_set;

namespace rakan {

Graph::Graph() : alpha_(0), beta_(0), gamma_(0), eta_(0) {}

Graph::~Graph() {
  delete reader_;
  delete nodes_;
  delete nodes_in_district_;
  delete nodes_on_perim_;
  delete perim_nodes_to_neighbors_;
  delete demographics;
}

Graph::LoadGraph(uint32_t num_districts, FILE *file) {
  num_districts_ = num_districts;

  // Only load the graph if the given index file is not null.
  if (file != nullptr) {
    nodes_in_district_ = new unordered_set[num_districts_];
    
  }
}

}     // namespace rakan