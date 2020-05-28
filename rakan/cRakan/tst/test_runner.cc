#include <inttypes.h>

#include "../src/Runner.h"
#include "../src/Graph.h"
#include "../src/Node.h"

#include "gtest/gtest.h"

namespace rakan {

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

}
