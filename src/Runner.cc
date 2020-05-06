#include "Runner.h"

#include <inttypes.h>
#include <iostream>

#include "ErrorCodes.h"
#include "Graph.h"
#include "Node.h"
#include "Reader.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Usage: ./" << argv[0] << " filepath" << endl;
  }

  // Open index file as a binary.
  FILE *file = fopen(argv[1], "rb");
  if (file == nullptr) {
    perror("Cannot open file: ");
    return EXIT_FAILURE;
  }

  // Initialize this engine.
  Init(file);
}

namespace rakan {

void Init(FILE *file) {
  Reader reader(file);
  Header header;
  vector<NodeRecord *> node_recs;

  if (reader.ReadHeader(&header) != SUCCESS) {
    perror("ReadHeader failed");
    return EXIT_FAILURE;
  }

  for (int i = 0; i < header.num_nodes; i++) {
    NodeRecord *rec = new NodeRecord;
    if (reader.ReadNodeRecord(offset, rec) != SUCCESS) {
      perror("ReadNodeRecord failed");
      return EXIT_FAILURE;
    }
    node_recs[i] = rec;
    offset += sizeof(NodeRecord);
  }
}

}
