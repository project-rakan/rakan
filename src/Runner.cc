#include "Runner.h"

#include <bits/stdc++.h>    // for std::unordered_set
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

uint16_t SeedDistricts(Graph *graph) {
  
  // 1. Put all nodes into a set 𝒰, and assign all nodes to belong to a non-existent
  //    district (such as -1 or # of districts + 1)
  // 2. For each district 𝑑 in possible districts:
  //   a. Select a random element in 𝒰 and remove it from 𝒰 and assign it to district 𝑑
  //   b. These are now called the “district 𝑑’s seed node”
  // 3. While 𝒰 is not empty:
  //   a. For each district 𝑑 in possible districts:
  //     i. Resume or start a BFS starting from the seed until it finds a node 𝑆 that exists in 𝒰
  //     ii. Remove 𝑆 from 𝒰
  //     iii. Set 𝑆’s district to 𝑑
  //     iv. Pause BFS at this node, and move onto the next district
  // Note this algorithm will run infinitely if for any two nodes,
  // a path between the two is not possible. Either a check at the beginning
  // of this method is required, or a check that the size of 𝒰 changes every
  // iteration of step 3 is required.

  uint32_t i;
  unordered_set<Node *> set;
  Vector<Node *> linkage;

  // Move all of the nodes inside of the graph into a set, 
  uint32_t districts = graph->GetNumDistricts();
  for (i = 0; i < graph->GetNumNodes(); i++) {
    Node* node = graph->GetNode(i);
    node->SetDistrict(districts + 1);
    set.insert(graph->GetNode(i));
    linkage.insert(linkage.begin(), graph->GetNode(i));
  }

  uint32_t randomNumber;
  uint32_t lastrand = 0;
  // Create a seeding for each district to be able to begin a BFS
  // search from that district to create a random redistricting.
  for (i = 1; i <= districts; i++) {
    randomNumber = (rand() % graph->GetNumNodes()) + 1;
    linkage[randomNumber]->SetDistrict(i);
    set.erase(linkage[randomNumber]);
    linkage[randomNumber] = linkage[set.size() - 1];
    linkage.pop_back();
  }

  // create a possible redistricting using our random seeding.
  i = 1;
  uint32_t size = set.size();
  while(set.size() > 0) {
    // BFS from seed until a node still in the original set is found.
    
    // Check to ensure that there is no endless looping.
    if (size - 1 == set.size()) {
      size = set.size();
    }
  }
  

  return SUCCESS;
}

}   // namespace rakan
