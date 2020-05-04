#ifndef READER_H_
#define READER_H_

#include <stdio.h>            // for (FILE *)
#include <inttypes.h>         // for uint32_t, etc.
#include <string>             // for std::string

#include "./Graph.h"          // for Graph class
#include "./Node.h"           // for Node class
#include "./ReaderUtils.h"    // for DISALLOW_COPY_AND_ASSIGN

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

  // Default destructor.
  ~Reader() {}

  // Reads the file and loads information onto graph.
  //
  // Arguments:
  //  - file: the index file to read from
  //  - graph: the graph to populate
  //
  // Returns:
  //  - SUCCESS if all reading and loading successful
  //  - INVALID_FILE if the file cannot be read
  //  - INVALID_GRAPH if the graph cannot be read
  uint16_t ReadFileToGraph(const FILE *file, Graph *graph);

 private:
  // The file currently we're reading.
  FILE *file_;

  // Reads the header of the file. Assumes valid arguments.
  //
  // Returns:
  //  - a populated Header struct
  Header ReadHeader() const;

  // Allocates memory for the data structures used in graph.
  //
  // Arguments:
  //  - num_nodes: the total number of nodes to populate
  //  - num_districts: the total number of districts to populate
  //  - graph: the graph to populate
  void InitGraph(const uint32_t num_nodes,
                 const uint32_t num_districts,
                 Graph *graph);

  // Reads the node records in the file. Assumes valid arguments.
  //
  // Arguments:
  //  - num_nodes: the number of node records in the file
  //
  // Returns:
  //  - a vector of populated NodeRecord structs
  vector<NodeRecord> ReadNodeRecords(uint32_t num_nodes) const;

  // Reads the node at offset bytes in the file. Uses a return
  // parameter.
  //
  // Arguments:
  //  - offset: the offset in the file where the node begins
  //  - node: a return parameter, to populate with information
  void ReadNode(uint32_t offset, Node *node) const;

  // Needed for unit tests.
  friend class Test_Reader;

  DISALLOW_COPY_AND_ASSIGN();
};        // class Reader

}         // namespace rakan

#endif    // READER_H_
