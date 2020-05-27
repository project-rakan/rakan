#include <inttypes.h>

#include "../src/Runner.h"
#include "../src/Graph.h"
#include "../src/Node.h"

#include "gtest/gtest.h"

namespace rakan {

TEST(Test_Runner, TestBFS) {
  uint32_t found_neighbor_id;
  unordered_set<Node *> unused;

  // 4 nodes in their respective county #s
  Node *n1 = new Node(1, 1, 0, 0);
  Node *n2 = new Node(2, 2, 0, 0);
  Node *n3 = new Node(3, 3, 0, 0);
  Node *n4 = new Node(4, 4, 0, 0);
  unused.insert(n1);
  unused.insert(n2);
  unused.insert(n3);
  unused.insert(n4);

  // r1 has 4 precincts, 1 district
  Runner r1(4, 1);
  n1->AddNeighbor(2);
  n2->AddNeighbor(1);
  // found_neighbor_id = r1.BFS(n1, &unused)->GetID();
  // ASSERT_EQ(found_neighbor_id, 2);
  // found_neighbor_id = r1.BFS(n2, &unused)->GetID();
  // ASSERT_EQ(found_neighbor_id, 1);
}

}
