
#include <bits/stdc++.h>      // for std::unordered_set
#include <inttypes.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>             // for std::string
#include <unordered_map>      // for std::unordered_map>
#include <vector>

#include "../src/ReturnCodes.h"
#include "../src/Reader.h"
#include "../src/Node.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::unordered_map;
using std::unordered_set;
using rakan::Header;
using rakan::Reader;
using rakan::NodeRecord;
using rakan::Node;

void PrintHeader(Header *header) {
  cout << "Header = {" << endl;
  cout << "    magic_number = " << std::hex << header->magic_number << endl;
  cout << "    checksum = " << std::hex << header->checksum << endl;
  cout << "    state = " << header->state[0] << header->state[1] << endl;
  cout << "    num_nodes = " << std::dec << header->num_nodes << endl;
  cout << "    num_districts = " << std::dec << header->num_districts << endl;
  cout << "}" << endl;
}

void PrintNodeRecord(NodeRecord *record) {
  cout << "NodeRecord = {" << endl;
  cout << "    num_neighbors = " << record->num_neighbors << endl;
  cout << "    node_pos = " << record->node_pos << endl;
  cout << "}" << endl;
}

void PrintSet(unordered_set<uint32_t> *set) {
  for (auto& element : *set) {
    cout << element << ", ";
  }
}

void PrintMap(unordered_map<string, uint32_t> *map) {
  for (auto element : *map) {
    cout << "        " << element.first << ": " << element.second << endl;
  }
 }

void PrintNode(Node *node) {
  cout << "Node = {" << endl;
  cout << "    id = " << node->GetID() << endl;
  cout << "    area_ = " << node->GetArea() << endl;
  cout << "    neighbors = {";
  PrintSet(node->GetNeighbors());
  cout << "}" << endl;
  cout << "    demographics = {" << endl;
  PrintMap(node->GetDemographics());
  cout << "    }" << endl;
  cout << "}" << endl;
}

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

  Reader reader(file);

  Header header;
  if (reader.ReadHeader(&header) != SUCCESS) {
    perror("ReadHeader failed");
    return EXIT_FAILURE;
  }
  PrintHeader(&header);

  uint32_t offset = sizeof(Header);
  vector<NodeRecord *> node_recs(header.num_nodes);
  for (int i = 0; i < header.num_nodes; i++) {
    NodeRecord *rec = new NodeRecord;
    if (reader.ReadNodeRecord(offset, rec) != SUCCESS) {
      perror("ReadNodeRecord failed");
      return EXIT_FAILURE;
    }
    node_recs[i] = rec;
    offset += sizeof(NodeRecord);
  }

  for (int i = 0; i < 3; i++) {
    PrintNodeRecord(node_recs[i]);
  }

  offset = sizeof(Header) + sizeof(NodeRecord) * header.num_nodes;
  vector<Node *> nodes(header.num_nodes);
  for (int i = 0; i < 3; i++) {
    Node *node = new Node;
    NodeRecord *rec = node_recs[i];
    if (reader.ReadNode(offset, *rec, node) != SUCCESS) {
      perror("ReadNode failed");
      return EXIT_FAILURE;
    }
    nodes[i] = node;
  }

  for (int i = 0; i < 3; i++) {
    PrintNode(nodes[i]);
  }

  fclose(file);
  return EXIT_SUCCESS;
}
