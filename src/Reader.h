#ifndef READER_H_
#define READER_H_

#include <stdio.h>       // for (FILE *)
#include <inttypes.h>    // for uint32_t, etc.
#include <string>        // for std::string

#include "./ReaderUtils.h"

using std::string;

namespace rakan {

// A header struct that contains all data in the header
// section of the index file.
//
// The header should be the first 18 bytes of the file.
typedef struct header_struct {
  uint32_t magic_number;    // 4 bytes
  uint32_t checksum;        // 4 bytes
  char state[2];            // 2 bytes
  uint32_t num_nodes;       // 4 bytes
  uint32_t num_districts;   // 4 bytes
} Header;

// A node record struct that contains all data in a
// single node record. The number of node records
// should be specified in the header.
//
// A node record should each be 8 bytes.
typedef struct node_record_struct {
  uint32_t num_neighbors;   // 4 bytes
  uint32_t node_pos;        // 4 bytes
} NodeRecord;

class Reader {
 public:
  // Default constructor.
  Reader();

  // Default destructor.
  ~Reader();

  // Reads the file and loads information onto graph.
  ReadFileToGraph(const FILE *file, Graph *graph);

 private:
  // Needed for unit tests.
  friend class Test_Reader;

  DISALLOW_COPY_AND_ASSIGN(FileIndexReader);
};        // class Reader

}         // namespace rakan

#endif    // READER_H_
