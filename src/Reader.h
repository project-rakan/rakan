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
// A node record should each be 12 bytes.
typedef struct node_record_struct {
  uint32_t num_vertices;    // 4 bytes
  uint32_t num_neighbors;   // 4 bytes
  uint32_t node_pos;        // 4 bytes
} NodeRecord;

class Reader {
 public:
  // Two argument constructor.
  //
  // Arguments:
  //  - fpath: the path to the index file
  explicit Reader(const string& fpath, bool validate = true);

  // Manufactures and returns a HeaderReader for this index file.
  // A HeaderReader is a derived class of Reader that is specialized
  // to read the header of the index file.
  HeaderReader* NewHeaderReader();

  // Manufactures and returns a NodeReader for this index file. A NodeReader
  // is a derived class of reader that is specialized to read a node in the
  // index file.
  NodeReader* NewNodeReader();

 protected:
  // A pointer to the file to read.
  FILE *file_;

  // A cached copy of the file header.
  Header header;

 private:
  // Needed for unit tests.
  friend class Test_Reader;

  DISALLOW_COPY_AND_ASSIGN(FileIndexReader);
};        // class Reader

}         // namespace rakan

#endif    // READER_H_
