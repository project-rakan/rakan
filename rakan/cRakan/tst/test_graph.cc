#include <inttypes.h>

#include "../src/Graph.h"
#include "../src/Node.h"

#include "gtest/gtest.h"

namespace rakan {

/* 
 * Constructor Checks
 */
TEST(Test_Graph, TestConstructor) {
  Graph g(1, 2);
  ASSERT_EQ(g.GetNumNodes(), 1);
  ASSERT_EQ(g.GetNumDistricts(), 2);
  ASSERT_EQ(g.GetStatePop(), 0);
}

/* 
 * Nodes and Edges Checks
 */

// Test simple add node by adding a single node.
TEST(Test_Graph, TestAddSimple) {
    // Check the graph state first
    Graph g(1, 1);
    ASSERT_EQ(g.AddNode(50, 1, 50, 30), false);
    ASSERT_EQ(g.GetStatePop(), 0);
    ASSERT_EQ(g.AddNode(0, 1, 40, 30), true);
    ASSERT_EQ(g.GetStatePop(), 70);
    ASSERT_EQ(g.GetNumNodes(), 1);
    ASSERT_EQ(g.GetNumNodes(), 1);
    ASSERT_EQ(g.ContainsNode(0), true);
    ASSERT_EQ(g.ContainsNode(50), false);

    Node* n = g.GetNode(0);

    // Sanity check node in graph
    ASSERT_EQ(n->GetCounty(), 1);
    ASSERT_EQ(n->GetID(), 0);
    ASSERT_EQ(n->GetTotalPop(), 70);
    ASSERT_EQ(n->GetMajorityPop(), 40);
    ASSERT_EQ(n->GetMinorityPop(), 30);
}

// Check multiple adds then contains
TEST(Test_Graph, TestMultipleContains) {

}


Graph *createSimpleGraph() {
    Graph g(5, 3);
    for (uint32_t i = 0; i < 5; i++) {
        g.AddNode(i, i % 3, i, i);
    }
    for (int i = 1; i < 4; i++) {
        g.AddEdge(i, i - 1);
    }
}
// A helper method to create a graph
Graph *createBigGraph() {
    Graph g(60, 7);
    for (uint32_t i = 0; i < 60; i++) {
        g.AddNode(i, i % 10, i, i);
    }
    for (int i = 1; i < 59; i++) {
        g.AddEdge(i, i - 1);
        g.AddEdge(i, i + 1);
    }

}
}   // namespace rakan
