#include <inttypes.h>

#include "../src/Runner.h"
#include "../src/Graph.h"
#include "../src/Node.h"

#include "gtest/gtest.h"

namespace rakan {

TEST(Test_Runner, TestBFS) {
  Graph *g = new Graph;
  Runner r;
  unordered_set<Node *> unused;

  // 4 nodes, all in district 1
  Node *n1 = new Node(1, 1, 1, 0, 0, 0);
  Node *n2 = new Node(2, 1, 1, 0, 0, 0);
  Node *n3 = new Node(3, 1, 1, 0, 0, 0);
  Node *n4 = new Node(4, 1, 1, 0, 0, 0);
  unused.insert(n1);
  unused.insert(n2);
  unused.insert(n3);
  unused.insert(n4);

  g->AddNode(n1);
  g->AddNode(n2);
  g->AddNode(n3);
  g->AddNode(n4);

  // Add an edge from n1 to n2. BFS on n1 should find n2,
  // BFS on n2 should find n1.
  g->AddEdge(n1, n2);
  ASSERT_EQ(r.BFS(n1, unused), n2);
  ASSERT_EQ(r.BFS(n2, unused), n1);
}

}
