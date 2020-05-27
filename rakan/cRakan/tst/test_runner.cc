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
  Node *n1 = new Node(1, 1, 0, 0);
  Node *n2 = new Node(2, 1, 0, 0);
  Node *n3 = new Node(3, 1, 0, 0);
  Node *n4 = new Node(4, 1, 0, 0);
  unused.insert(n1);
  unused.insert(n2);
  unused.insert(n3);
  unused.insert(n4);
}

}
