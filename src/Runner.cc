#include "Runner.h"

#include <assert.h>         // for assert()
#include <inttypes.h>       // for uint32_t, etc.
#include <iostream>         // for cerr
#include <stdio.h>          // for FILE *, fopen, fseek, fread, etc.
#include <vector>           // for std::vector

#include "ErrorCodes.h"     // for SUCCESS, READ_FAIL, SEEK_FAIL, etc.
#include "Graph.h"          // for class Graph
#include "Node.h"           // for class Node
#include "Reader.h"         // for class Reader, structs

using std::cerr;
using std::endl;
using std::vector;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Usage: ./" << argv[0] << " filepath" << endl;
  }

  // Open index file as a binary.
  FILE *file = fopen(argv[1], "rb");
  if (file == nullptr) {
    return EXIT_FAILURE;
  }

  // Initialize this engine.
  Graph *graph = rakan::Init(file);

  // Clean up and exit.
  fclose(file);
  return EXIT_SUCCESS;
}

namespace rakan {

Graph* Init(FILE *file) {
  uint32_t i, record_offset, node_offset;
  Graph *graph;
  Reader reader(file);
  Header header;
  NodeRecord rec;

  Verify(reader.ReadHeader(&header));
  graph = new Graph(header.num_nodes, header.num_districts);

  // Calculate the offsets of the first NodeRecord and Node.
  // Refer to the index file design for actual number of bytes.
  record_offset = kHeaderSize;
  node_offset = kHeaderSize + kNodeRecordSize * header.num_nodes;

  // Read all the nodes and populate the graph with them.
  for (i = 0; i < header.num_nodes; i++) {
    Node *node = new Node;
    Verify(reader.ReadNodeRecord(record_offset, &rec));
    Verify(reader.ReadNode(node_offset + rec.node_pos,
                           rec.num_neighbors,
                           node));
    graph->AddNode(*node);
    record_offset += kNodeRecordSize;
  }

  return graph;
}

void Verify(uint16_t result) {
  if (result != SUCCESS) {
    exit(EXIT_FAILURE);
  }
}

}
