#include <inttypes.h>

#include "../src/Graph.h"
#include "../src/Node.h"

#include "gtest/gtest.h"

namespace rakan {

/* 
 * Constructor Checks
 */

// Creates a graph with valid arguments.
// Tests if creation was correct
TEST(Test_Graph, TestConstructor) {
  Graph g(1, 2, 3);
  ASSERT_EQ(g.GetNumNodes(), 1);
  ASSERT_EQ(g.GetNumDistricts(), 2);
  ASSERT_EQ(g.GetStatePop(), 3);
}

/* 
 * Nodes and Edges Checks
 */

// Test simple add node by adding a single node with no neighbors
TEST(Test_Graph, TestAddSimple) {
  Graph g(1, 1, 0);
  Node n(0);
  g.AddNode(&n);

  ASSERT_EQ(g.GetNumNodes(), 1);
  ASSERT_EQ(g.GetNode(0), &n);
}

// Test adding multiple nodes
TEST(Test_Graph, TestAddMultiple) {
  Graph g(2, 1, 0);
  Node n0(0);
  Node n1(1);
  g.AddNode(&n0);
  g.AddNode(&n1);

  ASSERT_EQ(g.GetNumNodes(), 1);
  ASSERT_EQ(g.GetNode(0), &n0);
  ASSERT_EQ(g.GetNode(1), &n1);
}

}   // namespace rakan
