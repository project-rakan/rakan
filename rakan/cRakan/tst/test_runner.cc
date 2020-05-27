#include <inttypes.h>

#include "../src/Runner.h"
#include "../src/Graph.h"
#include "../src/Node.h"

#include "gtest/gtest.h"

namespace rakan {

TEST(Test_Runner, TestBFS) {
  unordered_set<uint32_t> unused;
  Graph *g;
  Node *n1, *n2, *n3, *n4;

  unused.insert(1);
  unused.insert(2);
  unused.insert(3);
  unused.insert(4);

  // r1 has 2 precincts, n1 and n2. They're each other's neighbors.
  Runner r1(2, 1);
  r1.add_node(1, 1, 0, 0);
  r1.add_node(2, 1, 0, 0);
  r1.add_edge(1, 2);
  n1 = r1.GetGraph()->GetNode(1);
  n2 = r1.GetGraph()->GetNode(2);

  // Start BFS at n1, should find n2.
  unused.erase(1);
  ASSERT_EQ(r1.BFS(n1, &unused), n2);
  unused.insert(1);

  // Start BFS at n2, should find n1.
  unused.erase(2);
  ASSERT_EQ(r1.BFS(n2, &unused), n1);

  // Erase both nodes from used. BFS should return nullptr.
  unused.erase(1);
  ASSERT_EQ(r1.BFS(n1, &unused), nullptr);
}

}
