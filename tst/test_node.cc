#include "../src/Node.h"
#include "../src/Graph.h"

#include "gtest/gtest.h"

namespace rakan {

// Creates a node with no neighbors
TEST(Test_Node, TestNodeCreation) {
    Node n(12345, 2);
    ASSERT_EQ(n.GetID(), 12345);
    ASSERT_EQ(n.GetDistrict(), 2);

    unordered_set<uint32_t>* neighbors;
    neighbors->insert(1234);
    neighbors->insert(1235);
    neighbors->insert(1236);
    Node m(123456, 3, neighbors);
    ASSERT_EQ(n.GetID(), 12345);
    ASSERT_EQ(n.GetDistrict(), 3);
    ASSERT_EQ(n.GetNeighbors(), neighbors);
}

// Creates a node with one neighbors
TEST(Test_Node, TestOneNeighbor) {
    unordered_set<uint32_t>* neighbors;
    neighbors->insert(1234);
    Node m(123456, 3, neighbors);
    ASSERT_EQ(m.GetID(), 12345);
    ASSERT_EQ(m.GetDistrict(), 3);
    ASSERT_EQ(m.GetNeighbors(), neighbors);
}

// Creates a node with many neighbors
TEST(Test_Node, TestManyNeighbors) {
    unordered_set<uint32_t>* neighbors;
    for (uint32_t i = 0; i < 100; i++) {
        neighbors->insert(i);
    }
    Node m(123456, 3, neighbors);
    ASSERT_EQ(m.GetID(), 12345);
    ASSERT_EQ(m.GetDistrict(), 3);
    ASSERT_EQ(m.GetNeighbors(), neighbors);
}

// Creates a node with no neighbors
TEST(Test_Node, TestNoNeighbors) {
    Node n(4, 2);
    ASSERT_EQ(n.GetID(), 4);
    ASSERT_EQ(n.GetDistrict(), 2);
    ASSERT_EQ(n.GetNeighbors()->size(), 0);
}

// Test the AddNeighbor method to check if neighbors grows when called.
TEST(Test_Node, TestAddNeighbors) {
    Node n(4, 2);
    ASSERT_EQ(n.GetID(), 4);
    ASSERT_EQ(n.GetDistrict(), 2);
    ASSERT_EQ(n.GetNeighbors()->size(), 0);
    Node m(3, 5); 
    n.AddNeighbor(m);
    ASSERT_EQ(n.GetNeighbors()->size(), 1);
    // Make sure that the relationship is bidirectional
    ASSERT_EQ(m.GetNeighbors()->size(), 1);
    

    // Tests adding many nodes with 

}

// Creates a demographics map and assigns it to the node's demographic field.
TEST(Test_Node, TestDemographics) {
    
}

}   // namespace rakan
