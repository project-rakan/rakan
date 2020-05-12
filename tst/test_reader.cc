#include <stdio.h>        // for (FILE *)

#include "../src/Reader.h"
#include "gtest/gtest.h"

namespace rakan {

FILE *f = fopen("./iowa.idx", "rb");

TEST(Test_Reader, TestConstructor) {
  Reader reader(f);
  ASSERT_EQ(reader.GetFile(), f);
}

// Tests reading the header (18 bytes).
TEST(Test_Reader, TestHeader) {
  Header header;
  Reader reader(f);

  // expected values according to bladecaller
  reader.ReadHeader(&header);
  ASSERT_EQ(header.magic_number, 0xBEEFCAFE);
  ASSERT_EQ(header.num_nodes, 99);
  ASSERT_EQ(header.num_districts, 4);
}

// Tests reading a node record (12 bytes).
TEST(Test_Reader, TestNodeRecord) {
}

// Tests reading a basic node (40 bytes):
// - one node id (4 bytes)
// - one vertex (8 bytes)
// - one neighbor (4 bytes)
// - demographics (24 bytes)
TEST(Test_Reader, TestNodeBasic) {
}

// Tests reading a "medium" node (52 bytes):
// - one node id (4 bytes)
// - two vertices (16 bytes)
// - two neighbors (8 bytes)
// - demographics (24 bytes)
TEST(Test_Reader, TestNodeMedium) {
}

// Tests reading a "large" node (92 bytes):
// - one node id (4 bytes)
// - five vertices (40 bytes)
// - five neighbors (20 bytes)
// - demographics (24 bytes)
TEST(Test_Reader, TestNodeLarge) {
}

// Tests reading a simple file.
TEST(Test_Reader, TestFileSimple) {
}

// Tests reading a moderate file.
TEST(Test_Reader, TestFileModerate) {
}

// Tests reading a complex file.
TEST(Test_Reader, TestFileComplex) {
}

}   // namespace rakan
