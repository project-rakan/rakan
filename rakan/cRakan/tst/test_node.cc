#include "../src/Node.h"

#include <inttypes.h>

#include <unordered_set>

#include "gtest/gtest.h"

namespace rakan {

// Creates a node with no neighbors and
// adds a few neighbors
TEST(Test_Node, TestNodeCreation) {
    Node n(12345, 2, 3, 4);
    ASSERT_EQ(n.GetID(), 12345);
    ASSERT_EQ(n.GetCounty(), 2);
    ASSERT_EQ(n.GetTotalPop(), 7);
    ASSERT_EQ(n.GetMajorityPop(), 3);
    ASSERT_EQ(n.GetMinorityPop(), 4);


    n.AddNeighbor(1234);
    n.AddNeighbor(1235);
    n.AddNeighbor(1236);
    unordered_set<uint32_t>* neighbors = n.GetNeighbors();
    ASSERT_EQ(neighbors->size(), 3);
    ASSERT_EQ(neighbors->find(1234) != neighbors->end(), true);
    ASSERT_EQ(neighbors->find(1235) != neighbors->end(), true);
    ASSERT_EQ(neighbors->find(1236) != neighbors->end(), true);
}

// Creates a node with many neighbors
TEST(Test_Node, TestManyNeighbors) {
    Node m(12345, 2, 3, 4);
    for (uint32_t i = 0; i < 100; i++) {
        m.AddNeighbor(i);
    }
    unordered_set<uint32_t>* neighbors = m.GetNeighbors();
    for (uint32_t i = 0; i < 100; i++) {
        ASSERT_EQ(neighbors->find(i) != neighbors->end(), true);
    }
    ASSERT_EQ(m.GetNeighbors()->size(), 100);
}

// Tests to see if node stores population correctly.
TEST(Test_Node, TestSetPop) {
    Node n(65, 5, 7, 3);
    ASSERT_EQ(n.GetMajorityPop(), 7);
    ASSERT_EQ(n.GetMinorityPop(), 3);
    ASSERT_EQ(n.GetTotalPop(), 10);
    n.SetMajorityPop(50);
    ASSERT_EQ(n.GetMajorityPop(), 50);
    ASSERT_EQ(n.GetMinorityPop(), 3);
    ASSERT_EQ(n.GetTotalPop(), 10);
    n.SetMinorityPop(30);
    ASSERT_EQ(n.GetMajorityPop(), 50);
    ASSERT_EQ(n.GetMinorityPop(), 30);
    ASSERT_EQ(n.GetTotalPop(), 10);
    n.SetTotalPop(80);
    ASSERT_EQ(n.GetMajorityPop(), 50);
    ASSERT_EQ(n.GetMinorityPop(), 30);
    ASSERT_EQ(n.GetTotalPop(), 80);
}
}