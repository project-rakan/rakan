#include "../src/Reader.h"

#include <stdio.h>    // for (FILE *)

namespace rakan {

TEST(Test_Reader, TestConstructor) {
  Reader small_reader("./test_files/small.idx", true);
  FILE *f1 = fopen("./test_files/small.idx", "r");
  ASSERT_EQ(small_reader.file, f1);

  Reader med_reader("./test_files/medium.idx", true);
  FILE *f2 = fopen("./test_files/medium.idx", "r");
  ASSERT_EQ(med_reader.file, f2);

  Reader large_reader("./test_files/large.idx", true);
  FILE *f3 = fopen("./test_files/large.idx", "r");
  ASSERT_EQ(large_reader.file, f3);

  Reader empty_reader("./test_files/empty.idx", true);
  FILE *f4 = fopen("./test_files/empty.idx", "r");
  ASSERT_EQ(empty_reader.file, f4);
}

// Tests reading the header (18 bytes).
TEST(Test_Reader, TestHeader) {
  Header header;
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
