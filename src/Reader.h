#ifndef SRC_READER_H_
#define SRC_READER_H_

#include <inttypes.h>         // for uint32_t, etc.
#include <stdio.h>            // for FILE *

#include "src/Graph.h"          // for Graph class
#include "src/Node.h"           // for Node class

using rakan::Graph;
using rakan::Node;

namespace rakan {

extern const uint32_t kMagicNumber;
extern const uint32_t kHeaderSize;
extern const uint32_t kNodeRecordSize;

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
  Reader() {}

  // File constructor.
  //
  // Arguments:
  //  - file: the file to read from
  explicit Reader(FILE *file) : file_(file) {}

  // Default destructor.
  ~Reader() {}

  // Reads the header of the file if it is not nullptr.
  //
  // Arguments:
  //  - header: the return parameter to be filled with the header
  //
  // Returns:
  //  - SUCCESS if all reading and loading successful
  //  - INVALID_FILE if the file cannot be read
  //  - READ_FAILED if reading file failed
  uint16_t ReadHeader(Header *header);

  // Reads the node records in the file.
  //
  // Arguments:
  //  - offset: the offset to start reading the node record
  //  - record: the return parameter to be filled with the node record
  //
  // Returns:
  //  - SUCCESS if all reading and loading successful
  //  - INVALID_FILE if the file cannot be read
  //  - SEEK_FAILED if seeking to offset failed
  //  - READ_FAILED if reading file failed
  uint16_t ReadNodeRecord(const uint32_t offset, NodeRecord *record);

  uint16_t ReadNode(const uint32_t offset,
                    const uint32_t num_neighbors,
                    Node *node);

 private:
  // The file we're currently reading.
  FILE *file_;

  uint32_t ToHostFormat(uint32_t x);

  // Needed for unit tests.
  friend class Test_Reader;
};        // class Reader

}         // namespace rakan

#endif    // SRC_READER_H_
