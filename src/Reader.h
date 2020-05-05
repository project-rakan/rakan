#ifndef READER_H_
#define READER_H_

#include <iostream>

#include <netinet/in.h>       // For htonl(), ntohl()
#include <stdio.h>            // for (FILE *)
#include <inttypes.h>         // for uint32_t, etc.
#include <string>             // for std::string

#include "./Graph.h"          // for Graph class
#include "./Node.h"           // for Node class
#include "./ReaderUtils.h"    // for RidOfZeros, ValidateCheckSum

using rakan::Graph;
using rakan::Node;
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

  uint32_t RidOfZeros(uint32_t x) {
    uint32_t copy = x;
    char *byte = (char *) &x;

    for (int i = 0; i < 8; i++) {
      if (*byte == 0x0) {
        copy = copy >> 8;
      } else {
        break;
      }
      byte++;
    }

    return copy;
  }

  void ToHostFormat() {
    magic_number = RidOfZeros(ntohl(magic_number));
    checksum = RidOfZeros(ntohl(checksum));
    num_nodes = RidOfZeros(ntohl(num_nodes));
    num_districts = RidOfZeros(ntohl(num_districts));
  }
} Header;

// A node record struct that contains all data in a
// single node record. The number of node records
// should be specified in the header.
//
// A node record should each be 8 bytes.
typedef struct node_record_struct {
  uint32_t num_neighbors;   // 4 bytes
  uint32_t node_pos;        // 4 bytes

  uint32_t RidOfZeros(uint32_t x) {
    uint32_t copy = x;
    char *byte = (char *) &x;

    // printf("reading x = %02x\n", x);

    for (int i = 0; i < 8; i++) {
      // printf("reading byte = %02x\n", *byte);
      if (*byte == 0x0) {
        copy = copy >> 8;
        // printf("copy is now = %02x\n", copy);
      } else {
        break;
      }
      byte++;
    }
    
    return copy;
  }

  void ToHostFormat() {
    num_neighbors = RidOfZeros(ntohl(num_neighbors));
    node_pos = RidOfZeros(ntohl(node_pos));
  }
} NodeRecord;

class Reader {
 public:
  // Default constructor.
  //
  // Arguments:
  //  - file: the file to read from
  Reader(FILE *file = nullptr) : file_(file) {}

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
  uint16_t ReadHeader(Header *header) const;

  // Reads the node records in the file. Fills the 
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
  uint16_t ReadNodeRecord(const uint32_t offset, NodeRecord *record) const;

  uint16_t ReadNode(const uint32_t offset, NodeRecord& record, Node *node) const;

 private:
  // The file we're currently reading.
  FILE *file_;

  // Needed for unit tests.
  friend class Test_Reader;

  // DISALLOW_COPY_AND_ASSIGN();
};        // class Reader

}         // namespace rakan

#endif    // READER_H_
