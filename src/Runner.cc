#include "src/Runner.h"

#include <math.h>           // for pow(), log(), fmin()
#include <inttypes.h>       // for uint32_t, etc.
#include <stdio.h>          // for FILE *, fopen, fseek, fread, etc.

#include <iostream>         // for cerr
#include <unordered_set>    // for std::unordered_set
#include <vector>           // for std::vector

#include "ErrorCodes.h"     // for SUCCESS, READ_FAIL, SEEK_FAIL, etc.
#include "Graph.h"          // for class Graph
#include "Node.h"           // for class Node
#include "Reader.h"         // for class Reader, structs

using std::cerr;
using std::endl;
using std::vector;

namespace rakan {

uint16_t Runner::LoadGraph(unordered_map<uint32_t, uint32_t> *map) {
  map::size_type size;
  // The number of precincts in the graph
  size = map->size();
  uint32_t num_nodes = (uint32_t) size;
  // Iterator to count all of the districts in the 
  map::iterator it = map.begin();

  unordered_set<uint32_t> districts;
  while(it != map.end()) {
    districts.insert(it->second);
    
    it++;
  }
  uint32_t num_districts = districts.size();
  graph_ = new Graph(num_nodes, num_districts);
  it = map.begin();
  while(it != map.end()) {
    Node* n = new Node(it->first);
    
    it++;
  }
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

  graph_ = new Graph(header.num_nodes, header.num_districts);

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

    graph->AddNode(*node);
    graph->AddStatePop(node->GetTotalPop());
    record_offset += kNodeRecordSize;
  }

  return SUCCESS;
}

uint16_t Runner::SeedDistricts() {
  // 1. Put all nodes into a set 𝒰, and assign all nodes to belong to a
  //    non-existent district (such as -1 or # of districts + 1)
  // 2. For each district 𝑑 in possible districts:
  //   a. Select a random element in 𝒰 and remove it from 𝒰 and assign
  //      it to district 𝑑
  //   b. These are now called the “district 𝑑’s seed node”
  // 3. While 𝒰 is not empty:
  //   a. For each district 𝑑 in possible districts:
  //     i. Resume or start a BFS starting from the seed until it finds
  //        a node 𝑆 that exists in 𝒰
  //     ii. Remove 𝑆 from 𝒰
  //     iii. Set 𝑆’s district to 𝑑
  //     iv. Pause BFS at this node, and move onto the next district
  // Note this algorithm will run infinitely if for any two nodes,
  // a path between the two is not possible. Either a check at the beginning
  // of this method is required, or a check that the size of 𝒰 changes every
  // iteration of step 3 is required.

  uint32_t i, size, districts, random_num;
  unordered_set<Node *> set;

  // Move all of the nodes inside of the graph into a set,
  districts = graph_->GetNumDistricts();
  for (i = 0; i < graph_->GetNumNodes(); i++) {
    Node* node = graph_->GetNode(i);
    node->SetDistrict(districts + 1);
    set.insert(graph_->GetNode(i));
    linkage.insert(linkage.begin(), graph_->GetNode(i));
  }

  // Create a seeding for each district to be able to begin a BFS
  // search from that district to create a random redistricting.
  for (i = 1; i <= districts; i++) {
    random_num = (rand_r() % graph_->GetNumNodes()) + 1;
    linkage[random_num]->SetDistrict(i);
    set.erase(linkage[random_num]);
    linkage[random_num] = linkage[set.size() - 1];
    linkage.pop_back();
  }

  // create a possible redistricting using our random seeding.
  i = 1;
  size = set.size();
  while (set.size() > 0) {
    // BFS from seed until a node still in the original set is found.
    Node* = BFS(graph, i);
    // Check to ensure that there is no endless looping.
    if (size - 1 == set.size()) {
      size = set.size();
      i++;
    } else {
      break;
    }
  }

  return SUCCESS;
}

Node* Runner::BFS(Graph* graph, uint32_t district_num) {
  graph->
}

double Runner::ScoreCompactness() {
  graph_->GetNumNodes() / graph_->GetDistrictArea());

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
