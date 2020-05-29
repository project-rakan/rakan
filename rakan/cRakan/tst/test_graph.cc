#include <inttypes.h>

#include "../src/Graph.h"
#include "../src/Node.h"

#include "gtest/gtest.h"

namespace rakan {
static Graph *createSimpleGraph();

static Graph *createBigGraph();
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
    Graph *g = createSimpleGraph();
    ASSERT_EQ(g->ContainsNode(5), false);
    ASSERT_EQ(g->ContainsNode(2), true);
    ASSERT_EQ(g->ContainsNode(3), true);
    ASSERT_EQ(g->ContainsNode(1), true);
    ASSERT_EQ(g->ContainsNode(0), true);
    ASSERT_EQ(g->ContainsNode(4), true);
    ASSERT_EQ(g->ContainsNode(6), false);
    g->AddNode(6, 2, 2, 2);
    ASSERT_EQ(g->ContainsNode(6), true);
}


// Check the neighbor web to make sure that all the edges are good
TEST(Test_Graph, TestEdgeSimple) {
    Graph *g = createSimpleGraph();
    ASSERT_EQ(g->ContainsEdge(2, 3), true);
    ASSERT_EQ(g->ContainsEdge(3, 2), true);
    ASSERT_EQ(g->ContainsEdge(0, 3), false);
    ASSERT_EQ(g->ContainsEdge(3, 0), false);
    for (uint32_t i = 1; i < 4; i++) {
        ASSERT_EQ(g->ContainsEdge(i, i - 1), true);
        ASSERT_EQ(g->ContainsEdge(i - 1, i), true);
    }
    for (uint32_t i = 0; i < 5; i++) {
         unordered_set<uint32_t>* neighbors = g->GetNode(i)->GetNeighbors();
         if (i == 0  || i == 4) {
            ASSERT_EQ(neighbors->size(), 1);
        } else {
            ASSERT_EQ(neighbors->size(), 2);
        }
        for (unordered_set<uint32_t>::iterator itr = neighbors->begin();
        itr != neighbors->end(); itr++) {
            Node *n = g->GetNode(*itr);
            unordered_set<uint32_t>* sub = n->GetNeighbors();
            if (*itr == 0  || *itr == 4) {
                ASSERT_EQ(sub->size(), 1);
            } else {
                ASSERT_EQ(sub->size(), 2);
            }
        }
    }
    delete g;
}

TEST(Test_Graph, TestEdgeComplex) {
    Graph *g = createBigGraph();
    ASSERT_EQ(g->ContainsEdge(2, 3), true);
    ASSERT_EQ(g->ContainsEdge(3, 2), true);
    ASSERT_EQ(g->ContainsEdge(0, 3), false);
    ASSERT_EQ(g->ContainsEdge(3, 0), false);
    for (uint32_t i = 1; i < 59; i++) {
        ASSERT_EQ(g->ContainsEdge(i, i - 1), true);
        ASSERT_EQ(g->ContainsEdge(i - 1, i), true);
    }
    for (uint32_t i = 0; i < 60; i++) {
         unordered_set<uint32_t>* neighbors = g->GetNode(i)->GetNeighbors();
         if (i == 0  || i == 59) {
            ASSERT_EQ(neighbors->size(), 1);
        } else {
            ASSERT_EQ(neighbors->size(), 2);
        }
        for (unordered_set<uint32_t>::iterator itr = neighbors->begin();
        itr != neighbors->end(); itr++) {
            Node *n = g->GetNode(*itr);
            unordered_set<uint32_t>* sub = n->GetNeighbors();
            if (n->GetID() == 0  || n->GetID() == 59) {
                ASSERT_EQ(sub->size(), 1);
            } else {
                ASSERT_EQ(sub->size(), 2);
            }
        }
    }
    delete g;
}

TEST(Test_Graph, TestDistrictsExists) {
    Graph *g = createBigGraph();
    for (uint32_t i = 0; i < 60; i++) {
        g->AddNodeToDistrict(i, i % 3);
    }
    for (uint32_t i = 0; i < 60; i++) {
        ASSERT_EQ(g->NodeExistsInDistrict(i, i % 3), true);
    }
    delete g;
}

TEST(Test_Graph, TestDistrictRemoval) {
    Graph *g = createBigGraph();
    for (uint32_t i = 1; i < 59; i++) {
        g->AddNodeToDistrict(i, i % 3);
    }
    for (uint32_t i = 1; i < 59; i++) {
        g->RemoveNodeFromDistrict(i, i % 3);
        ASSERT_EQ(g->NodeExistsInDistrict(i, i % 3), false);
    }
}

// helper method to create a simple graph, 
// must destruct graph when finished
static Graph *createSimpleGraph() {
    Graph *g = new Graph(7, 3);
    for (uint32_t i = 0; i < 5; i++) {
        g->AddNode(i, i % 3, i, i);
    }
    for (uint32_t i = 1; i < 4; i++) {
        g->AddEdge(i, i - 1);
        g->AddEdge(i, i + 1);
    }
    return g;
}
// A helper method to create a graph
static Graph *createBigGraph() {
    Graph *g = new Graph(60, 4);
    for (uint32_t i = 0; i < 60; i++) {
        g->AddNode(i, i % 10, i, i);
    }
    for (uint32_t i = 1; i < 59; i++) {
        g->AddEdge(i, i - 1);
        g->AddEdge(i, i + 1);
    }
    return g;
}
}   // namespace rakan
