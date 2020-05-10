#include <stdio.h>        // for (FILE *)

#include "../src/Reader.h"
#include "gtest/gtest.h"

namespace rakan {

TEST(Test_Reader, TestConstructor) {
  FILE *f1 = fopen("./test_files/small.idx", "r");
  Reader small_reader(f1);
  ASSERT_EQ(small_reader.GetFile(), f1);

  FILE *f2 = fopen("./test_files/medium.idx", "r");
  Reader med_reader(f2);
  ASSERT_EQ(med_reader.GetFile(), f2);

  FILE *f3 = fopen("./test_files/large.idx", "r");
  Reader large_reader(f3);
  ASSERT_EQ(large_reader.GetFile(), f3);

  FILE *f4 = fopen("./test_files/empty.idx", "r");
  Reader empty_reader(f4);
  ASSERT_EQ(empty_reader.GetFile(), f4);
}

// Tests reading the header (18 bytes).
TEST(Test_Reader, TestHeader) {
  Header header;
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
