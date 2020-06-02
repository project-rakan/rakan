#include <inttypes.h>

#include "../src/Graph.h"
#include "../src/Node.h"

#include "gtest/gtest.h"

namespace rakan {
static Graph *createSimpleGraph();

static Graph *createBigGraph();

static Graph* GraphNxN(uint32_t n);
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
    delete g;
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
    delete g;
}

TEST(Test_Graph, TestDistrictPopulationBasic) {
    // test with a basic 4x4 grid
    Graph* g = GraphNxN(4);
    ASSERT_TRUE(g->GetDistrictPop(0) == 40);
    ASSERT_TRUE(g->GetDistrictPop(1) == 40);
    ASSERT_TRUE(g->GetDistrictPop(2) == 40);
    ASSERT_TRUE(g->GetDistrictPop(3) == 40);
    ASSERT_TRUE(g->GetMinorityPop(0) == 20);
    ASSERT_TRUE(g->GetMinorityPop(1) == 20);
    ASSERT_TRUE(g->GetMinorityPop(2) == 20);
    ASSERT_TRUE(g->GetMinorityPop(3) == 20);
    delete g;
}

TEST(Test_Graph, TestDistrictPopulationHuge) {
    // test with a 100x100 grid
    Graph* g = GraphNxN(100);
    ASSERT_TRUE(g->GetDistrictPop(0) == 25000);
    ASSERT_TRUE(g->GetDistrictPop(1) == 25000);
    ASSERT_TRUE(g->GetDistrictPop(2) == 25000);
    ASSERT_TRUE(g->GetDistrictPop(3) == 25000);
    ASSERT_TRUE(g->GetMinorityPop(0) == 12500);
    ASSERT_TRUE(g->GetMinorityPop(1) == 12500);
    ASSERT_TRUE(g->GetMinorityPop(2) == 12500);
    ASSERT_TRUE(g->GetMinorityPop(3) == 12500);
    delete g;
}

TEST(Test_Graph, TestPerimNodesSimple) {
    Graph* g = GraphNxN(4);
    // Every node should have two neighbors on the edge and 
    // four neighbors within the square
    for (uint32_t i = 0; i < g->GetNumNodes(); i++) {
        ASSERT_TRUE(g->IsPerimNode(i));
    }
    delete g;
}

TEST(Test_Graph, TestPerimNodesComplex) {
    Graph* g = GraphNxN(100);
    // Every node should have two neighbors on the edge and 
    // four neighbors within the square
    for (uint32_t i = 0; i < g->GetNumNodes(); i++) {
        Node* n = g->GetNode(i);
        std::unordered_set<uint32_t> * neighbors = n->GetNeighbors();
        uint32_t Num_Neighbors = neighbors->size();
        if (Num_Neighbors == 3 || Num_Neighbors == 2) {
            // These are on the edges of the grid and should be
            // perim nodes
            ASSERT_TRUE(g->IsPerimNode(i));
        }
        // Go through all of the neighbors and see if there is a neighbor with
        // a different district number. if so, this is a perim node.
        std::unordered_set<uint32_t>::iterator itr = neighbors->begin();
        for (; itr != neighbors->end(); itr++) {
            if (g->GetNode(*itr)->GetDistrict() != n->GetDistrict()) {
                ASSERT_TRUE(g->IsPerimNode(i));
            }
        }
    }
    delete g;
}

TEST(Test_Graph, TestEdgeMarkComplex) {
    Graph* g = GraphNxN(50);
    unordered_set<edge, edge_hash>* Crossing_Edges = g->GetCrossingEdges();
    // Every node should have two neighbors on the edge and 
    // four neighbors within the square
    unordered_set<edge, edge_hash>::iterator iter;
    for (uint32_t i = 0; i < g->GetNumNodes(); i++) {
        Node* n = g->GetNode(i);
        std::unordered_set<uint32_t> * neighbors = n->GetNeighbors();
        // Go through all of the neighbors and see if there is a neighbor with
        // a different district number. This means that there should be a crossing
        // edge marked between them.
        std::unordered_set<uint32_t>::iterator itr = neighbors->begin();
        for (; itr != neighbors->end(); itr++) {
            if (g->GetNode(*itr)->GetDistrict() != n->GetDistrict()) {
                bool check = false;
                for (iter = Crossing_Edges->begin(); iter != Crossing_Edges->end(); iter++) {
                    if ((iter->node1 == *itr && iter->node2 == i) || 
                        (iter->node2 == *itr && iter->node1 == i)) {
                            check = true;
                        }
                }
                ASSERT_TRUE(check);
            }
        }
    }
    delete g;
}

TEST(Test_Graph, TestEdgeMarkSimple) {
    Graph* g = GraphNxN(4);
    unordered_set<edge, edge_hash>* Crossing_Edges = g->GetCrossingEdges();
    // Every node should have two neighbors on the edge and 
    // four neighbors within the square
    unordered_set<edge, edge_hash>::iterator iter;
    for (uint32_t i = 0; i < g->GetNumNodes(); i++) {
        Node* n = g->GetNode(i);
        std::unordered_set<uint32_t> * neighbors = n->GetNeighbors();
        // Go through all of the neighbors and see if there is a neighbor with
        // a different district number. This means that there should be a crossing
        // edge marked between them.
        std::unordered_set<uint32_t>::iterator itr = neighbors->begin();
        for (; itr != neighbors->end(); itr++) {
            if (g->GetNode(*itr)->GetDistrict() != n->GetDistrict()) {
                bool check = false;
                for (iter = Crossing_Edges->begin(); iter != Crossing_Edges->end(); iter++) {
                    if ((iter->node1 == *itr && iter->node2 == i) || 
                        (iter->node2 == *itr && iter->node1 == i)) {
                            check = true;
                        }
                }
                ASSERT_TRUE(check);
            }
        }
    }
    delete g;
}

TEST(Test_Graph, TestEdgeMarkMassive) {
    Graph* g = GraphNxN(150);
    unordered_set<edge, edge_hash>* Crossing_Edges = g->GetCrossingEdges();
    // Every node should have two neighbors on the edge and 
    // four neighbors within the square
    unordered_set<edge, edge_hash>::iterator iter;
    for (uint32_t i = 0; i < g->GetNumNodes(); i++) {
        Node* n = g->GetNode(i);
        std::unordered_set<uint32_t> * neighbors = n->GetNeighbors();
        // Go through all of the neighbors and see if there is a neighbor with
        // a different district number. This means that there should be a crossing
        // edge marked between them.
        std::unordered_set<uint32_t>::iterator itr = neighbors->begin();
        for (; itr != neighbors->end(); itr++) {
            if (g->GetNode(*itr)->GetDistrict() != n->GetDistrict()) {
                bool check = false;
                for (iter = Crossing_Edges->begin(); iter != Crossing_Edges->end(); iter++) {
                    if ((iter->node1 == *itr && iter->node2 == i) || 
                        (iter->node2 == *itr && iter->node1 == i)) {
                            check = true;
                        }
                }
                ASSERT_TRUE(check);
            }
        }
    }
    delete g;
}


TEST(Test_Graph, TestCorrectErrorPerim) {
    Graph* g = GraphNxN(26);
    uint32_t index = 27;
    for (uint32_t j = 0; j < 11; j++) {
        for(uint32_t i = 27; i < 38; i++) {
            ASSERT_FALSE(g->IsPerimNode(i + (j*26)));
        }
        for (uint32_t i = 40; i < 51; i++) {
            ASSERT_FALSE(g->IsPerimNode(i + (j*26)));
        }
    }
    for (uint32_t j = 13; j < 24; j++) {
        for(uint32_t i = 27; i < 38; i++) {
            ASSERT_FALSE(g->IsPerimNode(i + (j*26)));
        }
        for (uint32_t i = 40; i < 51; i++) {
            ASSERT_FALSE(g->IsPerimNode(i + (j*26)));
        }
    }

    // Add all along top row
    for (uint32_t i = 0; i < 26; i++) {
        Node *n1 = g->GetNode(i);
        ASSERT_FALSE(g->AddNodeToDistrictPerim(i, n1->GetDistrict()));
    }
    // Add all along bottom row
    for (uint32_t i = (26*26) - 26; i < 26*26; i++) {
        Node *n1 = g->GetNode(i);
        ASSERT_FALSE(g->AddNodeToDistrictPerim(i, n1->GetDistrict()));
    }
    // Add all along leftmost column
    for (uint32_t i = 0; i <= (26*26) - 26; i += 26) {
        Node *n1 = g->GetNode(i);
        ASSERT_FALSE(g->AddNodeToDistrictPerim(i, n1->GetDistrict()));
    }
    // Add all along rightmost column
    for (uint32_t i = (26 - 1); i < (26*26); i += 26) {
        Node *n1 = g->GetNode(i);
        ASSERT_FALSE(g->AddNodeToDistrictPerim(i, n1->GetDistrict()));
    }

    for (int j = 0; j < 2; j++) {
        // Remove all along top row
        for (uint32_t i = 0; i < 26; i++) {
            Node *n1 = g->GetNode(i);
            if (j == 0) {
                ASSERT_TRUE(g->RemoveNodeFromDistrictPerim(i, n1->GetDistrict()));
            } else {
                ASSERT_FALSE(g->RemoveNodeFromDistrictPerim(i, n1->GetDistrict()));
            }
        }
        // Remove all along bottom row
        for (uint32_t i = (26*26) - 26; i < 26*26; i++) {
            Node *n1 = g->GetNode(i);
            if (j == 0) {
                ASSERT_TRUE(g->RemoveNodeFromDistrictPerim(i, n1->GetDistrict()));
            } else {
                ASSERT_FALSE(g->RemoveNodeFromDistrictPerim(i, n1->GetDistrict()));
            }
        }
        // Remove all along leftmost column
        for (uint32_t i = 0; i <= (26*26) - 26; i += 26) {
            Node *n1 = g->GetNode(i);
            if (j == 0 && i != 0 && i != 650) {
                ASSERT_TRUE(g->RemoveNodeFromDistrictPerim(i, n1->GetDistrict()));
            } else {
                ASSERT_FALSE(g->RemoveNodeFromDistrictPerim(i, n1->GetDistrict()));
            }
        }
        // Remove all along rightmost column
        for (uint32_t i = 25; i < (26*26); i += 26) {
            Node *n1 = g->GetNode(i);
            if (j == 0 && i != 25 && i != 675) {
                ASSERT_TRUE(g->RemoveNodeFromDistrictPerim(i, n1->GetDistrict()));
            } else {
                ASSERT_FALSE(g->RemoveNodeFromDistrictPerim(i, n1->GetDistrict()));
            }
        }
    }

    delete g;
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

// Creates a NxN grid where every node is connected 
// vertically and horizontally. splits into 4 districts, and
// n must be even and greater than 2.
static Graph* GraphNxN(uint32_t n) {
    Graph *g = new Graph(n * n, 4);
    // Add in all nodes, each with 10 people inside them
    for (uint32_t i = 0; i < n * n; i++) {
        g->AddNode(i, i % 10, 5, 5);
    }
    // Go through each of the precincts and assign it to a district
    uint32_t counter = 0;
    uint32_t district = 0;
    for (uint32_t i = 0; i < n ; i++) {
        for (uint32_t j = 0; j < n / 2; j++) {
            g->AddNodeToDistrict(counter, district);
            counter++;
        }
        if (district == 0) {
            district = 1;
        } else {
            district = 0;
        }
    }
    district = 2;
    for (uint32_t i = 0; i < n ; i++) {
        for (uint32_t j = 0; j < n / 2; j++) {
            g->AddNodeToDistrict(counter, district);
            counter++;
        }
        if (district == 2) {
            district = 3;
        } else {
            district = 2;
        }
    }

    // Attach every node horizontally
    for (uint32_t step = 0; step < n; step++) {
        for (uint32_t i = 0; i < n - 1; i++) {
            uint32_t first = i + (step * n);
            uint32_t second = (i + 1) + (step * n);
            Node *n1 = g->GetNode(first);
            Node *n2 = g->GetNode(second);
            g->AddEdge(first, second);
            if (n1->GetDistrict() != n2->GetDistrict()) {
                g->MarkCrossingEdge(first, second);
                g->AddNodeToDistrictPerim(first, n1->GetDistrict());
                g->AddNodeToDistrictPerim(second, n2->GetDistrict());
            }
        }
    }
    // Attach every node vertically
    for (uint32_t offset = 0; offset < n; offset++) {
        for (uint32_t i = 0; i < n - 1; i++) {
            uint32_t first = offset + (i * n);
            uint32_t second = offset + ((i + 1) * n);
            Node *n1 = g->GetNode(first);
            Node *n2 = g->GetNode(second);
            g->AddEdge(first, second);
            if (n1->GetDistrict() != n2->GetDistrict()) {
                g->MarkCrossingEdge(first, second);
                // Will add corner nodes twice to perim list but
                // should not add to actual list and just return false
                g->AddNodeToDistrictPerim(first, n1->GetDistrict());
                g->AddNodeToDistrictPerim(second, n2->GetDistrict());
            }
        }
    }
    // Add all outmost edge nodes as Perim nodes

    // Add all along top row
    for (uint32_t i = 0; i < n; i++) {
        Node *n1 = g->GetNode(i);
        g->AddNodeToDistrictPerim(i, n1->GetDistrict());
    }
    // Add all along bottom row
    for (uint32_t i = (n*n) - n; i < n*n; i++) {
        Node *n1 = g->GetNode(i);
        g->AddNodeToDistrictPerim(i, n1->GetDistrict());
    }
    // Add all along leftmost column
    for (uint32_t i = 0; i <= (n*n) - n; i += n) {
        Node *n1 = g->GetNode(i);
        g->AddNodeToDistrictPerim(i, n1->GetDistrict());
    }
    // Add all along rightmost column
    for (uint32_t i = (n - 1); i < (n*n); i += n) {
        Node *n1 = g->GetNode(i);
        g->AddNodeToDistrictPerim(i, n1->GetDistrict());
    }
    // Say that n = 4, then the graph would be
    // organized as the following:
    // 0 - 1 - 2 - 3
    // | 0 |   | 1 |
    // 4 - 5 - 6 - 7
    // |   |   |   |
    // 8 - 9 -10 - 11
    // | 2 |   | 3 |
    // 12- 13- 14- 15
    // 
    // Where the graph nodes are fully formed and marked such that
    // district perimeters, crossing edges are all marked accordingly.
    //
    // Note that no matter the size, district number remains static at 4.
    // Thus, the district pattern will remain consistent as a window style
    // like shown above.
    return g;
}

}   // namespace rakan
