#include "src/Runner.h"

#include <math.h>               // for pow(), log(), fmin()
#include <inttypes.h>           // for uint32_t, etc.
#include <stdio.h>              // for FILE *, fopen, fseek, fread, etc.
#include <stdlib.h>             // for rand()

#include <iostream>             // for cerr
#include <unordered_set>        // for std::unordered_set
#include <vector>               // for std::vector

#include "src/ErrorCodes.h"     // for SUCCESS, READ_FAIL, SEEK_FAIL, etc.
#include "src/Graph.h"          // for class Graph
#include "src/Node.h"           // for class Node
#include "src/Reader.h"         // for class Reader, structs

using std::cerr;
using std::endl;
using std::vector;

namespace rakan {

static Node* BFS(const Node *start, unordered_set<Node *> *set);

uint16_t Runner::LoadPreMadeGraph(unordered_map<uint32_t, uint32_t> *map) {
  return SUCCESS;
}

uint16_t Runner::LoadGraph(FILE *file) {
  Reader reader(file);
  Header header;
  NodeRecord rec;
  uint32_t i, res, record_offset, node_offset;

  res = reader.ReadHeader(&header);
  if (res != SUCCESS) {
    cerr << "ReadHeader failed: error code " << res << endl;
    return res;
  }

  graph_ = new Graph(header.num_nodes, header.num_districts, 0);

  // Calculate the offsets of the first NodeRecord and Node.
  // Refer to the index file design for actual number of bytes.
  record_offset = kHeaderSize;
  node_offset = kHeaderSize + kNodeRecordSize * header.num_nodes;

  // Read all the nodes and populate the graph with them.
  for (i = 0; i < header.num_nodes; i++) {
    Node *node = new Node;

    res = reader.ReadNodeRecord(record_offset, &rec);
    if (res != SUCCESS) {
      cerr << "ReadNodeRecord failed: error code " << res << endl;
      return res;
    }

    res = reader.ReadNode(node_offset + rec.node_pos,
                          rec.num_neighbors,
                          node);
    if (res != SUCCESS) {
      cerr << "ReadNode failed: error code " << res << endl;
      return res;
    }

    graph_->AddNode(node);
    graph_->AddStatePop(node->GetTotalPop());
    record_offset += kNodeRecordSize;
  }

  return SUCCESS;
}

uint16_t Runner::SeedDistricts() {
  uint32_t i, random_node_index, num_nodes, num_districts;
  unordered_set<Node *> unused, used;
  unordered_map<int, Node*> seed_nodes;
  Node *seed_node, *node, *found_node;

  num_districts = graph_->GetNumDistricts();
  num_nodes = graph_->GetNumNodes();

  // iterate through the array of nodes and put them into the set
  for (i = 0; i < num_nodes; i++) {
    unused.insert(graph_->GetNode(i));
  }

  // iterate through all the possible districts
  // randomly assign a node to be the seed node of that district
  for (i = 0; i < num_districts; i++) {
    // get a random node index
    random_node_index = rand() % num_nodes;
    node = graph_->GetNode(random_node_index);

    // this node is not already a seed node
    if (used.find(node) == used.end()) {
      node->SetDistrict(i);
      seed_nodes.insert({i, node});
    } else {
      i--;
    }
  }

  // for (auto node : seed_nodes) {
  //   unused.erase(node);
  // }

  while (used.size() < num_nodes) {
    // for all possible districts…
    for (i = 0; i < num_districts; i++) {
      seed_node = seed_nodes.find(i)->second;
      found_node = BFS(seed_node, &unused);
      if (found_node != nullptr) {
        found_node->SetDistrict(i);
        unused.erase(node);
      }
    }
  }

  return SUCCESS;
}

static Node* BFS(const Node *start, unordered_set<Node *> *set) {
  return nullptr;
}

double Runner::ScoreCompactness() {
  // graph_->GetNumNodes() / graph_->GetDistrictArea());

  // TODO

  return 0;
}

double Runner::ScorePopulationDistribution() {
  uint32_t i, total_pop, avg_pop;
  double sum;
  
  total_pop = graph_->GetStatePop();
  avg_pop = total_pop / graph_->GetNumDistricts();

  for (i = 0; i < graph_->GetNumDistricts(); i++) {
    sum += pow((graph_->GetDistrictPop(i) - avg_pop), 2);
  }

  return sum / total_pop;
}

double Runner::ScoreExistingBorders() {
  // TODO
  return 0;
}

double Runner::ScoreVRA() {
  uint32_t i, min_pop_percentage;
  double sum;

  for (i = 0; i < graph_->GetNumDistricts(); i++) {
    min_pop_percentage = graph_->GetMinorityPop(i) / graph_->GetDistrictPop(i);
    sum += fmin(0, 0.5 - min_pop_percentage);
  }

  return sum;
}

double Runner::LogScore() {
  return (graph_->alpha_ * ScoreCompactness()
        + graph_->beta_ * ScorePopulationDistribution()
        + graph_->gamma_ * ScoreExistingBorders()
        + graph_->eta_ * ScoreVRA());
}

}   // namespace rakan
