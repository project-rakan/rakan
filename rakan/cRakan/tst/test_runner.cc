#include <inttypes.h>

#include "../src/Runner.h"
#include "../src/Graph.h"
#include "../src/Node.h"

#include "gtest/gtest.h"

namespace rakan {

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

}
