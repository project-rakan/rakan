#include "../src/Graph.h"
namespace rakan {

/* 
 * Constructor Checks
 */

// Creates a graph with no file passed in (should be empty)
Test(Test_Reader, TestEmptyGraph) {
}

// Creates a graph from a simple file.
// Tests if creation was correct
TEST(Test_Reader, TestBasicCreation) {
}

// Creates a graph from a complex file.
// Tests if creation was correct
TEST(Test_Reader, TestComplexCreation) {
}

/* 
 * Nodes and Edges Checks
 */

// Test simple add node by adding a single node with no neighbors
Test(Test_Reader, TestAddSimple) {
}

// Test adding multiple nodes
Test(Test_Reader, TestAddMultiple) {
}

// Test adding two nodes with the same id and checking if
// second is not added.
Test(Test_Reader, TestUniqueAdd) {
}

// Check to see if the outlining of a simple precinct checks out
Test(Test_Reader, TestPerimeter) {
}

/*
 * Test changing four demographic scores.
 */
TEST(Test_Reader, TestScores) {
}

}   // namespace rakan
