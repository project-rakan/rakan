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

// Test adding two nodes with the same id and checking if
// second is not added.
TEST(Test_Graph, TestUniqueAdd) {
  Graph g(2, 1, 0);
  Node n0(0);
  Node n0dupe(0);
  g.AddNode(&n0);

  ASSERT_EQ(g.AddNode(&n0dupe), false);
}

/*
 * Test changing four demographic scores.
 */
TEST(Test_Graph, TestScores) {
  Graph g(2, 1, 0);
  g.SetAlpha(1.0);
  g.SetBeta(2.0);
  g.SetGamma(3.0);
  g.SetEta(4.0);

  ASSERT_EQ(g.GetAlpha(), 1.0);
  ASSERT_EQ(g.GetBeta(), 2.0);
  ASSERT_EQ(g.GetGamma(), 3.0);
  ASSERT_EQ(g.GetEta(), 4.0);
}

}   // namespace rakan
