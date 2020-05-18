#include "../src/Node.h"

#include <inttypes.h>

#include <unordered_set>

#include "gtest/gtest.h"

namespace rakan {

// Creates a node with no neighbors
TEST(Test_Node, TestNodeCreation) {
    Node n(12345, 2);
    ASSERT_EQ(n.GetID(), 12345);
    ASSERT_EQ(n.GetDistrict(), 2);

    unordered_set<uint32_t> *neighbors = new unordered_set<uint32_t>;
    neighbors->insert(1234);
    neighbors->insert(1235);
    neighbors->insert(1236);
    Node m(123456, 3, neighbors);
    ASSERT_EQ(m.GetID(), 123456);
    ASSERT_EQ(m.GetDistrict(), 3);
    ASSERT_EQ(m.GetNeighbors(), neighbors);
}

// Creates a node with one neighbors
TEST(Test_Node, TestOneNeighbor) {
    unordered_set<uint32_t> *neighbors = new unordered_set<uint32_t>;
    neighbors->insert(1234);
    Node m(123456, 3, neighbors);
    ASSERT_EQ(m.GetID(), 123456);
    ASSERT_EQ(m.GetDistrict(), 3);
    ASSERT_EQ(m.GetNeighbors(), neighbors);
    ASSERT_EQ(m.GetNeighbors()->size(), neighbors->size());
}

// Creates a node with many neighbors
TEST(Test_Node, TestManyNeighbors) {
    unordered_set<uint32_t> *neighbors = new unordered_set<uint32_t>;
    for (uint32_t i = 0; i < 100; i++) {
        neighbors->insert(i);
    }
    Node m(123456, 3, neighbors);
    ASSERT_EQ(m.GetID(), 123456);
    ASSERT_EQ(m.GetDistrict(), 3);
    ASSERT_EQ(m.GetNeighbors(), neighbors);
    ASSERT_EQ(m.GetNeighbors()->size(), neighbors->size());
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
    unordered_set<uint32_t> neighbors;

    Node n(4, 2);
    ASSERT_EQ(n.GetNeighbors()->size(), 0);
    Node m(0, 5); 
    n.AddNeighbor(m);
    ASSERT_EQ(n.GetNeighbors()->size(), 1);
    
    // Tests adding many nodes with 
    Node a(1, 5);
    n.AddNeighbor(a);
    ASSERT_EQ(n.GetNeighbors()->size(), 2);
    Node b(2, 5);
    n.AddNeighbor(b);
    ASSERT_EQ(n.GetNeighbors()->size(), 3);
    Node c(3, 5);
    n.AddNeighbor(c);
    ASSERT_EQ(n.GetNeighbors()->size(), 4);
    Node d(4, 5);
    n.AddNeighbor(d);
    ASSERT_EQ(n.GetNeighbors()->size(), 5);
    Node e(5, 5);
    n.AddNeighbor(e);
    ASSERT_EQ(n.GetNeighbors()->size(), 6);
    Node f(6, 5);
    n.AddNeighbor(f);
    ASSERT_EQ(n.GetNeighbors()->size(), 7);
    Node g(7, 5);
    n.AddNeighbor(g);
    ASSERT_EQ(n.GetNeighbors()->size(), 8);
    Node h(8, 5);
    n.AddNeighbor(h);
    ASSERT_EQ(n.GetNeighbors()->size(), 9);
    Node i(9, 5);
    n.AddNeighbor(i);
    ASSERT_EQ(n.GetNeighbors()->size(), 10);
    Node j(10, 5);
    n.AddNeighbor(j);
    ASSERT_EQ(n.GetNeighbors()->size(), 11);
    
    neighbors.insert(m.GetID());
    neighbors.insert(a.GetID());
    neighbors.insert(b.GetID());
    neighbors.insert(c.GetID());
    neighbors.insert(d.GetID());
    neighbors.insert(e.GetID());
    neighbors.insert(f.GetID());
    neighbors.insert(g.GetID());
    neighbors.insert(h.GetID());
    neighbors.insert(i.GetID());
    neighbors.insert(j.GetID());
    ASSERT_EQ(n.GetNeighbors()->size(), neighbors.size());
}

// Tests to see if node stores demographic population correctly.
TEST(Test_Node, TestSetPop) {
    unordered_map<string, uint32_t> *map;
    Node n(4, 2);
    n.SetTotalPop(5);
    n.SetAAPop(1);
    n.SetAIPop(1);
    n.SetASPop(1);
    n.SetCAPop(1);
    n.SetOtherPop(1);
    map = n.GetDemographics();
    ASSERT_EQ((*map)["aa"], 1);
    ASSERT_EQ((*map)["total"], 5);
    ASSERT_EQ((*map)["ai"], 1);
    ASSERT_EQ((*map)["as"], 1);
    ASSERT_EQ((*map)["ca"], 1);
    ASSERT_EQ((*map)["other"], 1);
    ASSERT_EQ(n.GetTotalPop(), 5);
    ASSERT_EQ(n.GetTotalPop(), 5);
}
}   // namespace rakan
