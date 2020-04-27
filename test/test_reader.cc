#include "../src/Reader.h"

namespace rakan {

// Tests reading the header (18 bytes).
TEST(Test_Reader, TestHeader) {
}

// Tests reading a node record (12 bytes).
Test(Test_Reader, TestNodeRecord) {
}

// Tests reading a basic node (40 bytes):
// - one node id (4 bytes)
// - one vertex (8 bytes)
// - one neighbor (4 bytes)
// - demographics (24 bytes)
Test(Test_Reader, TestNodeBasic) {
}

// Tests reading a "medium" node (52 bytes):
// - one node id (4 bytes)
// - two vertices (16 bytes)
// - two neighbors (8 bytes)
// - demographics (24 bytes)
Test(Test_Reader, TestNodeMedium) {
}

// Tests reading a "large" node (92 bytes):
// - one node id (4 bytes)
// - five vertices (40 bytes)
// - five neighbors (20 bytes)
// - demographics (24 bytes)
Test(Test_Reader, TestNodeLarge) {
}

// Tests reading a simple file.
Test(Test_Reader, TestFileSimple) {
}

// Tests reading a moderate file.
Test(Test_Reader, TestFileModerate) {
}

// Tests reading a complex file.
Test(Test_Reader, TestFileComplex) {
}

}   // namespace rakan
