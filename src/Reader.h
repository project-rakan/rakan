#ifndef READER_H_
#define READER_H_

#include <iostream>

#include <arpa/inet.h>      // For htonl(), ntohl()
#include <stdio.h>            // for (FILE *)
#include <inttypes.h>         // for uint32_t, etc.
#include <string>             // for std::string

#include "./Graph.h"          // for Graph class
#include "./Node.h"           // for Node class
// #include "./ReaderUtils.h"    // for DISALLOW_COPY_AND_ASSIGN

using rakan::Graph;
using rakan::Node;
using std::string;

// Macro to convert from network to host byte order.
#define ntohll(x) \
  ( ((uint64_t) (ntohl((uint32_t)((x << 32) >> 32))) << 32) |   \
    ntohl(((uint32_t)(x >> 32))) )

// Macro to convert from host to network byte order.
#define htonll(x) (ntohll(x))

namespace rakan {

// A header struct that contains all data in the header
// section of the index file.
//
// The header should be the first 18 bytes of the file.
typedef struct header_struct {
  uint32_t magic_number;    // 4 bytes
  uint32_t checksum;        // 4 bytes
  char state[2];            // 2 bytes
  int32_t num_nodes;       // 4 bytes
  int32_t num_districts;   // 4 bytes

  void ToHostFormat() {
    printf("magic_number, little endian = %02x\n", magic_number);
    magic_number = htonl(magic_number);
    printf("magic_number, big endian = %02x\n", magic_number);
    printf("checksum, little endian = %02x\n", checksum);
    checksum = htonl(checksum);
    printf("checksum, big endian = %02x\n", checksum);
    printf("num_nodes, little endian = %02x\n", num_nodes);
    num_nodes = htonl(num_nodes);
    printf("num_nodes, big endian = %02x\n", num_nodes);
    printf("num_districts, little endian = %02x\n", num_districts);
    num_districts = htonl(num_districts);
    printf("num_districts, big endian = %02x\n", num_districts);
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

  uint16_t ReadNode(NodeRecord& record, Node *node) const;

 private:
  // The file we're currently reading.
  FILE *file_;

  // Needed for unit tests.
  friend class Test_Reader;

  // DISALLOW_COPY_AND_ASSIGN();
};        // class Reader

}         // namespace rakan

#endif    // READER_H_
