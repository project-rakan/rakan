#include "./Reader.h"

#include <bits/stdc++.h>    // for std::unordered_set
#include <inttypes.h>       // for uint32_t
#include <stdio.h>          // for FILE *, stderr
#include <unordered_map>    // for std::unordered_map
#include <vector>           // for std::vector

#include "./ErrorCodes.h"     // for error codes
#include "./Graph.h"          // for Graph class
#include "./Node.h"           // for Node class
#include "./ReaderUtils.h"    // for utils: CRC, magic_num

using std::unordered_set;
using std::unordered_map;

namespace rakan {

uint16_t Reader::ReadFileToGraph(const FILE *file, Graph *graph) const {
  if (file == nullptr) {
    return INVALID_FILE;
  }

  if (graph == nullptr) {
    return INVALID_GRAPH;
  }

  file_ = file;
  
  Header header = ReadHeader();

  nodes_in_district_ = new unordered_set[num_districts_];
  nodes_on_perim = new unordered_set[num_districts_];
  perim_nodes_to_neighbors_ = new unordered_set[num_districts_];
  demographics = new unordered_map();
}

}     // namespace rakan
