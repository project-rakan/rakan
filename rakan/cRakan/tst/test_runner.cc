#include <inttypes.h>

#include <iostream>

#include "../src/Runner.h"
#include "../src/Graph.h"
#include "../src/Node.h"

#include "gtest/gtest.h"

namespace rakan {

Runner* Generate4x4Map() {
  Runner *r = new Runner(16, 4);
  for (uint32_t i = 0; i < 17; i++) {
    r->add_node(i, 0, 0, 0);
  }
  r->add_edge(0, 1);
  r->add_edge(1, 2);
  r->add_edge(2, 3);
  r->add_edge(0, 4);
  r->add_edge(1, 5);
  r->add_edge(2, 6);
  r->add_edge(3, 7);
  r->add_edge(4, 5);
  r->add_edge(5, 6);
  r->add_edge(6, 7);
  r->add_edge(4, 8);
  r->add_edge(5, 9);
  r->add_edge(6, 10);
  r->add_edge(7, 11);
  r->add_edge(8, 9);
  r->add_edge(9, 10);
  r->add_edge(10, 11);
  r->add_edge(8, 12);
  r->add_edge(9, 13);
  r->add_edge(10, 14);
  r->add_edge(11, 15);
  r->add_edge(12, 13);
  r->add_edge(13, 14);
  r->add_edge(14, 15);

  vector<uint32_t> districts(16);

  // First quadrant in district 0
  districts[0] = 0;
  districts[1] = 0;
  districts[4] = 0;
  districts[5] = 0;

  // Second quadrant in district 1
  districts[2] = 1;
  districts[3] = 1;
  districts[6] = 1;
  districts[7] = 1;

  // Third quadrant in district 2
  districts[8] = 2;
  districts[9] = 2;
  districts[12] = 2;
  districts[13] = 2;

  // Fourth quadrant in district 3
  districts[10] = 3;
  districts[11] = 3;
  districts[14] = 3;
  districts[15] = 3;

  r->set_districts(districts);
  return r;
}

Runner* GenerateHMap() {
  Runner *r = new Runner(7, 3);
  for (uint32_t i = 0; i < 7; i++) {
    r->add_node(i, 0, 0, 0);
  }
  r->add_edge(0, 1);
  r->add_edge(1, 2);
  r->add_edge(1, 3);
  r->add_edge(4, 5);
  r->add_edge(5, 6);
  r->add_edge(3, 5);

  vector<uint32_t> districts(7);
  districts[0] = 0;
  districts[1] = 0;
  districts[2] = 0;
  districts[3] = 1;
  districts[4] = 2;
  districts[5] = 2;
  districts[6] = 2;

  r->set_districts(districts);
  return r;
}

Runner* GenerateHourglassMap() {
  Runner *r = new Runner(7, 3);
  for (uint32_t i = 0; i < 7; i++) {
    r->add_node(i, 0, 0, 0);
  }
  r->add_edge(0, 1);
  r->add_edge(0, 3);
  r->add_edge(1, 2);
  r->add_edge(1, 3);
  r->add_edge(2, 3);
  r->add_edge(3, 4);
  r->add_edge(3, 5);
  r->add_edge(3, 6);
  r->add_edge(4, 5);
  r->add_edge(5, 6);

  vector<uint32_t> districts(7);
  districts[0] = 0;
  districts[1] = 1;
  districts[2] = 1;
  districts[3] = 0;
  districts[4] = 0;
  districts[5] = 2;
  districts[6] = 2;

  r->set_districts(districts);
  return r;
}


TEST(Test_Runner, TestBFSOneEdge) {
  unordered_set<uint32_t> unused;
  Graph *g;
  Node *n0, *n1;

  for (uint32_t i = 1; i <= 4; i++) {
    unused.insert(i);
  }

  // r1 has 2 precincts {n0 and n1}.
  // n0 <-> n1
  Runner r1(2, 1);
  r1.add_node(0, 1, 0, 0);
  r1.add_node(1, 1, 0, 0);
  r1.add_edge(0, 1);
  n0 = r1.GetGraph()->GetNode(0);
  n1 = r1.GetGraph()->GetNode(1);

  // Start BFS at n0, should find n1.
  unused.erase(0);
  ASSERT_EQ(r1.BFS(n0, &unused), n1);
  unused.insert(0);

  // Start BFS at n1, should find n0.
  unused.erase(1);
  ASSERT_EQ(r1.BFS(n1, &unused), n0);

  // Erase both nodes from used. BFS should return nullptr.
  unused.clear();
  ASSERT_EQ(r1.BFS(n0, &unused), nullptr);
  ASSERT_EQ(r1.BFS(n1, &unused), nullptr);
}

TEST(Test_Runner, TestBFSTwoEdges) {
  unordered_set<uint32_t> unused;
  Graph *g;
  Node *n0, *n1, *n2;

  for (uint32_t i = 0; i < 3; i++) {
    unused.insert(i);
  }

  // r2 has 3 precincts {n0, n1, n2}.
  // n0 <-> n1
  // n1 <-> n2
  Runner r2(3, 1);
  r2.add_node(0, 1, 0, 0);
  r2.add_node(1, 1, 0, 0);
  r2.add_node(2, 1, 0, 0);
  r2.add_edge(0, 1);
  r2.add_edge(1, 2);

  n0 = r2.GetGraph()->GetNode(0);
  n1 = r2.GetGraph()->GetNode(1);
  n2 = r2.GetGraph()->GetNode(2);
  
  // Start BFS at n1, should find n2, then n0.
  unused.erase(1);
  ASSERT_EQ(r2.BFS(n1, &unused), n2);
  unused.erase(2);
  ASSERT_EQ(r2.BFS(n1, &unused), n0);
}

TEST(Test_Runner, TestBFSThreeEdges) {
  unordered_set<uint32_t> unused;
  Graph *g;
  Node *n0, *n1, *n2, *n3;

  for (uint32_t i = 0; i < 4; i++) {
    unused.insert(i);
  }

  // r3 has 4 precincts {n0, n1, n2, n3}.
  // n0 <-> n1
  // n0 <-> n3
  // n1 <-> n2
  // n1 <-> n3
  Runner r3(4, 1);
  r3.add_node(0, 1, 0, 0);
  r3.add_node(1, 1, 0, 0);
  r3.add_node(2, 1, 0, 0);
  r3.add_node(3, 1, 0, 0);
  r3.add_edge(0, 1);
  r3.add_edge(0, 3);
  r3.add_edge(1, 2);
  r3.add_edge(1, 3);

  n0 = r3.GetGraph()->GetNode(0);
  n1 = r3.GetGraph()->GetNode(1);
  n2 = r3.GetGraph()->GetNode(2);
  n3 = r3.GetGraph()->GetNode(3);
  
  // Start BFS at n0, should find n3, then n1, then n2.
  unused.erase(0);
  ASSERT_EQ(r3.BFS(n0, &unused), n3);
  unused.erase(3);
  ASSERT_EQ(r3.BFS(n0, &unused), n1);
  unused.erase(1);
  ASSERT_EQ(r3.BFS(n0, &unused), n2);
  
  unused.clear();
  for (uint32_t i = 0; i < 4; i++) {
    unused.insert(i);
  }

  // Start BFS at n1, should find n3, then n0, then n2.
  unused.erase(1);
  ASSERT_EQ(r3.BFS(n1, &unused), n3);
  unused.erase(3);
  ASSERT_EQ(r3.BFS(n1, &unused), n0);
  unused.erase(0);
  ASSERT_EQ(r3.BFS(n1, &unused), n2);

  unused.clear();
  for (uint32_t i = 0; i < 4; i++) {
    unused.insert(i);
  }

  // Start BFS at n3, should find n1, then n0, then n2, then nullptr.
  unused.erase(3);
  ASSERT_EQ(r3.BFS(n3, &unused), n1);
  unused.erase(1);
  ASSERT_EQ(r3.BFS(n3, &unused), n0);
  unused.erase(0);
  ASSERT_EQ(r3.BFS(n3, &unused), n2);
  unused.erase(2);
  ASSERT_EQ(r3.BFS(n3, &unused), nullptr);
}

TEST(Test_Runner, TestBFSDisconnectedEdges) {
  unordered_set<uint32_t> unused;
  Graph *g;
  Node *n0, *n1, *n2, *n3;

  for (uint32_t i = 0; i < 4; i++) {
    unused.insert(i);
  }

  // r4 has 4 precincts {n0, n1, n2, n3}.
  // n0 <-> n1
  // n2 <-> n3
  Runner r4(4, 1);
  r4.add_node(0, 1, 0, 0);
  r4.add_node(1, 1, 0, 0);
  r4.add_node(2, 1, 0, 0);
  r4.add_node(3, 1, 0, 0);
  r4.add_edge(0, 1);
  r4.add_edge(2, 3);

  n0 = r4.GetGraph()->GetNode(0);
  n1 = r4.GetGraph()->GetNode(1);
  n2 = r4.GetGraph()->GetNode(2);
  n3 = r4.GetGraph()->GetNode(3);
  
  // Start BFS at n0, should find n1, then nullptr.
  unused.erase(0);
  ASSERT_EQ(r4.BFS(n0, &unused), n1);
  unused.erase(1);
  ASSERT_EQ(r4.BFS(n0, &unused), nullptr);
  unused.insert(0);

  // Start BFS at n1, should find n0, then nullptr.
  ASSERT_EQ(r4.BFS(n1, &unused), n0);
  unused.erase(0);
  ASSERT_EQ(r4.BFS(n1, &unused), nullptr);

  // Start BFS at n2, should find n3, then nullptr.
  unused.erase(2);
  ASSERT_EQ(r4.BFS(n2, &unused), n3);
  unused.erase(3);
  ASSERT_EQ(r4.BFS(n2, &unused), nullptr);
  unused.insert(2);

  // Start BFS at n3, should find n2, then nullptr.
  ASSERT_EQ(r4.BFS(n3, &unused), n2);
  unused.erase(2);
  ASSERT_EQ(r4.BFS(n3, &unused), nullptr);
}

TEST(Test_Runner, TestBFSNoEdges) {
  unordered_set<uint32_t> unused;
  Graph *g;
  Node *n0, *n1, *n2, *n3;

  for (uint32_t i = 0; i < 4; i++) {
    unused.insert(i);
  }

  Runner r5(4, 1);
  r5.add_node(0, 1, 0, 0);
  r5.add_node(1, 1, 0, 0);
  r5.add_node(2, 1, 0, 0);
  r5.add_node(3, 1, 0, 0);

  n0 = r5.GetGraph()->GetNode(0);
  n1 = r5.GetGraph()->GetNode(1);
  n2 = r5.GetGraph()->GetNode(2);
  n3 = r5.GetGraph()->GetNode(3);

  unused.erase(0);
  ASSERT_EQ(r5.BFS(n0, &unused), nullptr);
  unused.insert(0);

  unused.erase(1);
  ASSERT_EQ(r5.BFS(n1, &unused), nullptr);
  unused.insert(1);

  unused.erase(2);
  ASSERT_EQ(r5.BFS(n2, &unused), nullptr);
  unused.insert(2);

  unused.erase(3);
  ASSERT_EQ(r5.BFS(n3, &unused), nullptr);
}

TEST(Test_Runner, TestRedistrict4x4Map) {
  Runner *r = Generate4x4Map();
  Graph *g = r->GetGraph();
  Node *victim_node, *idle_node;
  unordered_set<Edge, EdgeHash> *crossing_edges;
  unordered_set<Edge, EdgeHash> expected_crossing_edges;
  expected_crossing_edges.emplace(1, 2);
  expected_crossing_edges.emplace(5, 6);
  expected_crossing_edges.emplace(4, 8);
  expected_crossing_edges.emplace(5, 9);
  expected_crossing_edges.emplace(6, 10);
  expected_crossing_edges.emplace(7, 11);
  expected_crossing_edges.emplace(9, 10);
  expected_crossing_edges.emplace(13, 14);

  crossing_edges = g->GetCrossingEdges();
  ASSERT_EQ(crossing_edges->size(), expected_crossing_edges.size());

  // Node 1 currently in district 0
  victim_node = g->GetNode(1);
  idle_node = g->GetNode(2);
  ASSERT_EQ(victim_node->GetDistrict(), 0);

  // Redistrict node 1 -> node 2
  r->Redistrict(victim_node, idle_node);
  ASSERT_EQ(victim_node->GetDistrict(), 1);
  ASSERT_EQ(g->GetPerimNodeNeighbors(0, 1), nullptr);

  Edge e1(1, 2);
  expected_crossing_edges.erase(e1);
  expected_crossing_edges.emplace(0, 1);
  expected_crossing_edges.emplace(1, 5);
  ASSERT_EQ(crossing_edges->size(), expected_crossing_edges.size());
  unordered_set<Edge, EdgeHash>::iterator itr = expected_crossing_edges.begin();
  for (uint32_t i = 0; i < expected_crossing_edges.size(); i++) {
    Edge expected = *itr;
    ASSERT_NE(crossing_edges->find(expected), crossing_edges->end());
    Edge actual = *crossing_edges->find(expected);
    ASSERT_EQ(expected, expected);
    itr++;
  }
  ASSERT_EQ(g->GetNodesInDistrict(0)->size(), 3);
  ASSERT_EQ(g->GetNodesInDistrict(1)->size(), 5);
  ASSERT_EQ(g->GetNodesInDistrict(2)->size(), 4);
  ASSERT_EQ(g->GetNodesInDistrict(3)->size(), 4);

  // Node 9 currently in district 2
  victim_node = g->GetNode(9);
  idle_node = g->GetNode(5);
  ASSERT_EQ(victim_node->GetDistrict(), 2);

  // Redistrict node 9 -> node 5
  r->Redistrict(victim_node, idle_node);
  ASSERT_EQ(victim_node->GetDistrict(), 0);
  ASSERT_EQ(g->GetPerimNodeNeighbors(2, 9), nullptr);

  Edge e2(5, 9);
  expected_crossing_edges.erase(e2);
  expected_crossing_edges.emplace(8, 9);
  expected_crossing_edges.emplace(9, 13);
  ASSERT_EQ(crossing_edges->size(), expected_crossing_edges.size());
  itr = expected_crossing_edges.begin();
  for (uint32_t i = 0; i < expected_crossing_edges.size(); i++) {
    Edge expected = *itr;
    ASSERT_NE(crossing_edges->find(expected), crossing_edges->end());
    Edge actual = *crossing_edges->find(expected);
    ASSERT_EQ(expected, expected);
    itr++;
  }
  ASSERT_EQ(g->GetNodesInDistrict(0)->size(), 4);
  ASSERT_EQ(g->GetNodesInDistrict(1)->size(), 5);
  ASSERT_EQ(g->GetNodesInDistrict(2)->size(), 3);
  ASSERT_EQ(g->GetNodesInDistrict(3)->size(), 4);

  // Node 13 currently in district 2
  victim_node = g->GetNode(13);
  idle_node = g->GetNode(9);
  ASSERT_EQ(victim_node->GetDistrict(), 2);

  // Redistrict node 13 -> node 9
  r->Redistrict(victim_node, idle_node);
  ASSERT_EQ(victim_node->GetDistrict(), 0);
  ASSERT_EQ(g->GetPerimNodeNeighbors(2, 13), nullptr);
  ASSERT_NE(g->GetPerimNodeNeighbors(2, 12), nullptr);

  Edge e3(9, 13);
  expected_crossing_edges.erase(e3);
  expected_crossing_edges.emplace(12, 13);
  ASSERT_EQ(crossing_edges->size(), expected_crossing_edges.size());
  itr = expected_crossing_edges.begin();
  for (uint32_t i = 0; i < expected_crossing_edges.size(); i++) {
    Edge expected = *itr;
    ASSERT_NE(crossing_edges->find(expected), crossing_edges->end());
    Edge actual = *crossing_edges->find(expected);
    ASSERT_EQ(expected, expected);
    itr++;
  }
  ASSERT_EQ(g->GetNodesInDistrict(0)->size(), 5);
  ASSERT_EQ(g->GetNodesInDistrict(1)->size(), 5);
  ASSERT_EQ(g->GetNodesInDistrict(2)->size(), 2);
  ASSERT_EQ(g->GetNodesInDistrict(3)->size(), 4);

  // Node 7 currently in district 1
  victim_node = g->GetNode(7);
  idle_node = g->GetNode(11);
  ASSERT_EQ(victim_node->GetDistrict(), 1);

  // Redistrict node 7 -> node 11
  r->Redistrict(victim_node, idle_node);
  ASSERT_EQ(victim_node->GetDistrict(), 3);
  ASSERT_EQ(g->GetPerimNodeNeighbors(1, 7), nullptr);
  ASSERT_NE(g->GetPerimNodeNeighbors(3, 7), nullptr);

  Edge e4(7, 11);
  expected_crossing_edges.erase(e4);
  expected_crossing_edges.emplace(3, 7);
  expected_crossing_edges.emplace(6, 7);
  ASSERT_EQ(crossing_edges->size(), expected_crossing_edges.size());
  itr = expected_crossing_edges.begin();
  for (uint32_t i = 0; i < expected_crossing_edges.size(); i++) {
    Edge expected = *itr;
    ASSERT_NE(crossing_edges->find(expected), crossing_edges->end());
    Edge actual = *crossing_edges->find(expected);
    ASSERT_EQ(expected, expected);
    itr++;
  }
  ASSERT_EQ(g->GetNodesInDistrict(0)->size(), 5);
  ASSERT_EQ(g->GetNodesInDistrict(1)->size(), 4);
  ASSERT_EQ(g->GetNodesInDistrict(2)->size(), 2);
  ASSERT_EQ(g->GetNodesInDistrict(3)->size(), 5);

  // Redistrict node 7 -> node 3
  idle_node = g->GetNode(3);
  r->Redistrict(victim_node, idle_node);
  ASSERT_EQ(victim_node->GetDistrict(), 1);
  ASSERT_EQ(g->GetPerimNodeNeighbors(3, 7), nullptr);
  ASSERT_NE(g->GetPerimNodeNeighbors(1, 7), nullptr);
  ASSERT_EQ(g->GetPerimNodeNeighbors(3, 7), nullptr);

  Edge e5(3, 7);
  Edge e6(6, 7);
  expected_crossing_edges.erase(e5);
  expected_crossing_edges.erase(e6);
  expected_crossing_edges.emplace(7, 11);
  ASSERT_EQ(crossing_edges->size(), expected_crossing_edges.size());
  itr = expected_crossing_edges.begin();
  for (uint32_t i = 0; i < expected_crossing_edges.size(); i++) {
    Edge expected = *itr;
    ASSERT_NE(crossing_edges->find(expected), crossing_edges->end());
    Edge actual = *crossing_edges->find(expected);
    ASSERT_EQ(expected, expected);
    itr++;
  }
  ASSERT_EQ(g->GetNodesInDistrict(0)->size(), 5);
  ASSERT_EQ(g->GetNodesInDistrict(1)->size(), 5);
  ASSERT_EQ(g->GetNodesInDistrict(2)->size(), 2);
  ASSERT_EQ(g->GetNodesInDistrict(3)->size(), 4);
}

TEST(Test_Runner, TestRedistrictHourglassMap) {
  Runner *r = GenerateHourglassMap();
  Graph *g = r->GetGraph();
  Node *victim_node, *idle_node;
  unordered_set<Edge, EdgeHash> *crossing_edges;
  unordered_set<Edge, EdgeHash> expected_crossing_edges;
  expected_crossing_edges.emplace(0, 1);
  expected_crossing_edges.emplace(1, 3);
  expected_crossing_edges.emplace(2, 3);
  expected_crossing_edges.emplace(3, 5);
  expected_crossing_edges.emplace(3, 6);
  expected_crossing_edges.emplace(4, 5);
  crossing_edges = g->GetCrossingEdges();
  ASSERT_EQ(crossing_edges->size(), expected_crossing_edges.size());

  // Node 3 currently in district 0, try to move it into district 1
  // Should NOT be allowed because district 0 will be disconnected
  victim_node = g->GetNode(3);
  idle_node = g->GetNode(1);
  ASSERT_FALSE(r->IsValidRedistricting(victim_node, idle_node));
  ASSERT_EQ(victim_node->GetDistrict(), 0);

  // Node 1 currently in district 1
  victim_node = g->GetNode(1);
  idle_node = g->GetNode(3);

  // Redistrict node 1 -> node 3
  ASSERT_TRUE(r->IsValidRedistricting(victim_node, idle_node));
  r->Redistrict(victim_node, idle_node);
  ASSERT_EQ(victim_node->GetDistrict(), 0);
  ASSERT_EQ(g->GetPerimNodeNeighbors(1, 1), nullptr);
  ASSERT_EQ(g->GetPerimNodeNeighbors(0, 0), nullptr);
  ASSERT_NE(g->GetPerimNodeNeighbors(0, 1), nullptr);

  Edge e1(0, 1);
  Edge e2(1, 3);
  expected_crossing_edges.erase(e1);
  expected_crossing_edges.erase(e2);
  expected_crossing_edges.emplace(1, 2);
  ASSERT_EQ(crossing_edges->size(), expected_crossing_edges.size());
  unordered_set<Edge, EdgeHash>::iterator itr = expected_crossing_edges.begin();
  for (uint32_t i = 0; i < expected_crossing_edges.size(); i++) {
    Edge expected = *itr;
    ASSERT_NE(crossing_edges->find(expected), crossing_edges->end());
    Edge actual = *crossing_edges->find(expected);
    ASSERT_EQ(expected, expected);
    itr++;
  }
  ASSERT_EQ(g->GetNodesInDistrict(0)->size(), 4);
  ASSERT_EQ(g->GetNodesInDistrict(1)->size(), 1);
  ASSERT_EQ(g->GetNodesInDistrict(2)->size(), 2);

  // Node 2 currently in district 1, try to move it into district 0
  // Should NOT be allowed because district 1 will be empty
  victim_node = g->GetNode(2);
  idle_node = g->GetNode(1);
  ASSERT_FALSE(r->IsValidRedistricting(victim_node, idle_node));
  ASSERT_EQ(victim_node->GetDistrict(), 1);

  // Node 3 currently in district 0, try to move it into district 2
  // Should NOT be allowed because district 0 will be disconnected
  victim_node = g->GetNode(3);
  idle_node = g->GetNode(5);
  ASSERT_FALSE(r->IsValidRedistricting(victim_node, idle_node));
  ASSERT_EQ(victim_node->GetDistrict(), 0);

  // Node 4 currently in district 0
  victim_node = g->GetNode(4);
  idle_node = g->GetNode(5);

  // Redistrict node 4 -> node 5
  ASSERT_TRUE(r->IsValidRedistricting(victim_node, idle_node));
  r->Redistrict(victim_node, idle_node);
  ASSERT_EQ(victim_node->GetDistrict(), 2);
  ASSERT_EQ(g->GetPerimNodeNeighbors(0, 4), nullptr);
  ASSERT_NE(g->GetPerimNodeNeighbors(2, 4), nullptr);
  ASSERT_NE(g->GetPerimNodeNeighbors(0, 3), nullptr);
  ASSERT_NE(g->GetPerimNodeNeighbors(2, 5), nullptr);

  Edge e3(4, 5);
  expected_crossing_edges.erase(e3);
  expected_crossing_edges.emplace(3, 4);
  ASSERT_EQ(crossing_edges->size(), expected_crossing_edges.size());
  itr = expected_crossing_edges.begin();
  for (uint32_t i = 0; i < expected_crossing_edges.size(); i++) {
    Edge expected = *itr;
    ASSERT_NE(crossing_edges->find(expected), crossing_edges->end());
    Edge actual = *crossing_edges->find(expected);
    ASSERT_EQ(expected, expected);
    itr++;
  }
  ASSERT_EQ(g->GetNodesInDistrict(0)->size(), 3);
  ASSERT_EQ(g->GetNodesInDistrict(1)->size(), 1);
  ASSERT_EQ(g->GetNodesInDistrict(2)->size(), 3);

  // Node 5 currently in district 2, try to move it into district 0
  // Should NOT be allowed because district 2 will be disconnected
  victim_node = g->GetNode(5);
  idle_node = g->GetNode(3);
  ASSERT_FALSE(r->IsValidRedistricting(victim_node, idle_node));
  ASSERT_EQ(victim_node->GetDistrict(), 2);
}

TEST(Test_Runner, TestSeveredDistrictSimple) {
  // r has 7 precincts, 3 districts
  // draws out an H shape
  Runner *r = GenerateHMap();
  ASSERT_TRUE(r->IsDistrictSevered(r->GetGraph()->GetNode(0), 1));
  ASSERT_TRUE(r->IsDistrictSevered(r->GetGraph()->GetNode(1), 1));
  ASSERT_TRUE(r->IsDistrictSevered(r->GetGraph()->GetNode(2), 1));
  ASSERT_FALSE(r->IsDistrictSevered(r->GetGraph()->GetNode(3), 0));
  ASSERT_FALSE(r->IsDistrictSevered(r->GetGraph()->GetNode(3), 2));
  ASSERT_TRUE(r->IsDistrictSevered(r->GetGraph()->GetNode(4), 0));
  ASSERT_TRUE(r->IsDistrictSevered(r->GetGraph()->GetNode(5), 1));
  ASSERT_TRUE(r->IsDistrictSevered(r->GetGraph()->GetNode(6), 0));
}

TEST(Test_Runner, TestIsValidRedistricting) {
  // r has 16 precincts, 4 districts
  // draws out a 4x4 square with 4 quadrants
  Runner *r = Generate4x4Map();
  Graph *g = r->GetGraph();

  // Redistricting to self
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(0), g->GetNode(0)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(2), g->GetNode(2)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(11), g->GetNode(11)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(13), g->GetNode(13)));

  // Same districts
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(0), g->GetNode(1)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(0), g->GetNode(4)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(5), g->GetNode(1)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(5), g->GetNode(4)));

  // Just crossing, no severedness
  ASSERT_TRUE(r->IsValidRedistricting(g->GetNode(1), g->GetNode(2)));
  ASSERT_TRUE(r->IsValidRedistricting(g->GetNode(5), g->GetNode(6)));
  ASSERT_TRUE(r->IsValidRedistricting(g->GetNode(4), g->GetNode(8)));
  ASSERT_TRUE(r->IsValidRedistricting(g->GetNode(5), g->GetNode(9)));
  ASSERT_TRUE(r->IsValidRedistricting(g->GetNode(6), g->GetNode(10)));
  ASSERT_TRUE(r->IsValidRedistricting(g->GetNode(7), g->GetNode(11)));
  ASSERT_TRUE(r->IsValidRedistricting(g->GetNode(9), g->GetNode(10)));
  ASSERT_TRUE(r->IsValidRedistricting(g->GetNode(13), g->GetNode(14)));

  // No edge
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(0), g->GetNode(2)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(1), g->GetNode(3)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(5), g->GetNode(8)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(5), g->GetNode(10)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(6), g->GetNode(11)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(10), g->GetNode(13)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(8), g->GetNode(10)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(12), g->GetNode(14)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(0), g->GetNode(15)));
}

TEST(Test_Runner, TestSeedSimple) {
  // r has 2 precincts, 1 district
  Runner r(2, 1);
  r.add_node(0, 1, 0, 0);
  r.add_node(1, 1, 0, 0);
  r.add_edge(0, 1);
  r.seed();

  Graph *g = r.GetGraph();
  ASSERT_EQ(g->GetNodesInDistrict(0)->size(), 2);
  ASSERT_EQ(g->GetNode(0)->GetDistrict(), 0);
  ASSERT_EQ(g->GetNode(1)->GetDistrict(), 0);
}

TEST(Test_Runner, TestSeedMedium) {
  // r has 6 precincts, 3 district
  // n0 <-> n1
  // n2 <-> n3
  // n4 <-> n5
  Runner r(6, 3);
  r.add_node(0, 1, 0, 0);
  r.add_node(1, 1, 0, 0);
  r.add_node(2, 1, 0, 0);
  r.add_node(3, 1, 0, 0);
  r.add_node(4, 1, 0, 0);
  r.add_node(5, 1, 0, 0);
  r.add_edge(0, 1);
  r.add_edge(2, 3);
  r.add_edge(4, 5);

  unordered_set<Node *> seed_nodes;
  Graph *g = r.GetGraph();
  seed_nodes.insert(g->GetNode(0));
  seed_nodes.insert(g->GetNode(2));
  seed_nodes.insert(g->GetNode(4));
  g->AddNodeToDistrict(0, 0);
  g->AddNodeToDistrict(2, 1);
  g->AddNodeToDistrict(4, 2);

  r.SpawnDistricts(&seed_nodes);
  ASSERT_EQ(g->GetNodesInDistrict(0)->size(), 2);
  ASSERT_EQ(g->GetNodesInDistrict(1)->size(), 2);
  ASSERT_EQ(g->GetNodesInDistrict(2)->size(), 2);

  ASSERT_EQ(g->GetNode(0)->GetDistrict(), g->GetNode(1)->GetDistrict());
  ASSERT_EQ(g->GetNode(2)->GetDistrict(), g->GetNode(3)->GetDistrict());
  ASSERT_EQ(g->GetNode(4)->GetDistrict(), g->GetNode(5)->GetDistrict());
}

TEST(Test_Runner, TestWalkOneStep) {
  Runner *r = Generate4x4Map();
  Graph *g;
  vector<vector<uint32_t>> all_maps;
  vector<uint32_t> map;
  uint32_t i;
  unordered_map<uint32_t, vector<uint32_t>> district_nodes;

  r->Walk(1, 0, 0, 0, 0);
  g = r->GetGraph();
  all_maps = r->getMaps();
  map = all_maps[all_maps.size() - 1];

  ASSERT_TRUE(r->IsDistrictConnected(0));
  ASSERT_TRUE(r->IsDistrictConnected(1));
  ASSERT_TRUE(r->IsDistrictConnected(2));
  ASSERT_TRUE(r->IsDistrictConnected(3));

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());
  ASSERT_EQ(district_nodes[3].size(), g->GetNodesInDistrict(3)->size());

  Runner *r1 = GenerateHourglassMap();
  r1->Walk(1, 0, 0, 0, 0);
  g = r1->GetGraph();
  all_maps = r1->getMaps();
  map = all_maps[all_maps.size() - 1];
  district_nodes.clear();

  ASSERT_TRUE(r1->IsDistrictConnected(0));
  ASSERT_TRUE(r1->IsDistrictConnected(1));
  ASSERT_TRUE(r1->IsDistrictConnected(2));

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());
}

TEST(Test_Runner, TestWalkTwoSteps) {
  Runner *r = Generate4x4Map();
  Graph *g;
  vector<vector<uint32_t>> all_maps;
  vector<uint32_t> map;
  uint32_t i;
  unordered_map<uint32_t, vector<uint32_t>> district_nodes;

  r->Walk(2, 0, 0, 0, 0);
  g = r->GetGraph();
  all_maps = r->getMaps();
  map = all_maps[all_maps.size() - 1];

  ASSERT_TRUE(r->IsDistrictConnected(0));
  ASSERT_TRUE(r->IsDistrictConnected(1));
  ASSERT_TRUE(r->IsDistrictConnected(2));
  ASSERT_TRUE(r->IsDistrictConnected(3));

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());
  ASSERT_EQ(district_nodes[3].size(), g->GetNodesInDistrict(3)->size());

  Runner *r1 = GenerateHourglassMap();
  r1->Walk(2, 0, 0, 0, 0);
  g = r1->GetGraph();
  all_maps = r1->getMaps();
  map = all_maps[all_maps.size() - 1];
  district_nodes.clear();

  ASSERT_TRUE(r1->IsDistrictConnected(0));
  ASSERT_TRUE(r1->IsDistrictConnected(1));
  ASSERT_TRUE(r1->IsDistrictConnected(2));

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());
}

TEST(Test_Runner, TestWalkThreeSteps) {
  Runner *r = Generate4x4Map();
  Graph *g;
  vector<vector<uint32_t>> all_maps;
  vector<uint32_t> map;
  uint32_t i;
  unordered_map<uint32_t, vector<uint32_t>> district_nodes;

  r->Walk(3, 0, 0, 0, 0);
  g = r->GetGraph();
  all_maps = r->getMaps();
  map = all_maps[all_maps.size() - 1];

  ASSERT_TRUE(r->IsDistrictConnected(0));
  ASSERT_TRUE(r->IsDistrictConnected(1));
  ASSERT_TRUE(r->IsDistrictConnected(2));
  ASSERT_TRUE(r->IsDistrictConnected(3));

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());
  ASSERT_EQ(district_nodes[3].size(), g->GetNodesInDistrict(3)->size());

  Runner *r1 = GenerateHourglassMap();
  r1->Walk(3, 0, 0, 0, 0);
  g = r1->GetGraph();
  all_maps = r1->getMaps();
  map = all_maps[all_maps.size() - 1];
  district_nodes.clear();

  ASSERT_TRUE(r1->IsDistrictConnected(0));
  ASSERT_TRUE(r1->IsDistrictConnected(1));
  ASSERT_TRUE(r1->IsDistrictConnected(2));

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());
}

TEST(Test_Runner, TestWalkTenSteps) {
  Runner *r = Generate4x4Map();
  Graph *g;
  vector<vector<uint32_t>> all_maps;
  vector<uint32_t> map;
  uint32_t i;
  unordered_map<uint32_t, vector<uint32_t>> district_nodes;

  r->Walk(10, 0, 0, 0, 0);
  g = r->GetGraph();
  all_maps = r->getMaps();
  map = all_maps[all_maps.size() - 1];

  ASSERT_TRUE(r->IsDistrictConnected(0));
  ASSERT_TRUE(r->IsDistrictConnected(1));
  ASSERT_TRUE(r->IsDistrictConnected(2));
  ASSERT_TRUE(r->IsDistrictConnected(3));

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());
  ASSERT_EQ(district_nodes[3].size(), g->GetNodesInDistrict(3)->size());

  Runner *r1 = GenerateHourglassMap();
  r1->Walk(10, 0, 0, 0, 0);
  g = r1->GetGraph();
  all_maps = r1->getMaps();
  map = all_maps[all_maps.size() - 1];
  district_nodes.clear();

  ASSERT_TRUE(r1->IsDistrictConnected(0));
  ASSERT_TRUE(r1->IsDistrictConnected(1));
  ASSERT_TRUE(r1->IsDistrictConnected(2));

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());
}

TEST(Test_Runner, TestWalkHundredSteps) {
  Runner *r = Generate4x4Map();
  Graph *g;
  vector<vector<uint32_t>> all_maps;
  vector<uint32_t> map;
  uint32_t i;
  unordered_map<uint32_t, vector<uint32_t>> district_nodes;

  r->Walk(100, 0, 0, 0, 0);
  g = r->GetGraph();
  all_maps = r->getMaps();
  map = all_maps[all_maps.size() - 1];

  ASSERT_TRUE(r->IsDistrictConnected(0));
  ASSERT_TRUE(r->IsDistrictConnected(1));
  ASSERT_TRUE(r->IsDistrictConnected(2));
  ASSERT_TRUE(r->IsDistrictConnected(3));

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());
  ASSERT_EQ(district_nodes[3].size(), g->GetNodesInDistrict(3)->size());

  Runner *r1 = GenerateHourglassMap();
  r1->Walk(100, 0, 0, 0, 0);
  g = r1->GetGraph();
  all_maps = r1->getMaps();
  map = all_maps[all_maps.size() - 1];
  district_nodes.clear();

  ASSERT_TRUE(r1->IsDistrictConnected(0));
  ASSERT_TRUE(r1->IsDistrictConnected(1));
  ASSERT_TRUE(r1->IsDistrictConnected(2));

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());
}

}
