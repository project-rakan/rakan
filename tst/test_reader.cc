#include <stdio.h>        // for (FILE *)

#include "../src/Reader.h"
#include "gtest/gtest.h"

namespace rakan {

TEST(Test_Reader, TestReaderConstructor) {
  FILE *f = fopen("./iowa.idx", "rb");
  if (f != nullptr) {
    Reader reader(f);
    ASSERT_EQ(reader.GetFile(), f);
    fclose(f);
  }
}

// Tests reading the header (18 bytes).
TEST(Test_Reader, TestReadHeader) {
  FILE *f = fopen("./iowa.idx", "rb");
  Header header;
  Reader reader(f);

  // expected values according to bladecaller
  if (f != nullptr) {
    fseek(f, 0, SEEK_SET);
    reader.ReadHeader(&header);
    ASSERT_EQ(header.magic_number, 0xBEEFCAFE);
    ASSERT_EQ(header.num_nodes, 99);
    ASSERT_EQ(header.num_districts, 4);
    fclose(f);
  }
}

// Tests reading a node record (12 bytes).
TEST(Test_Reader, TestReadNodeRecord) {
  FILE *f = fopen("./iowa.idx", "rb");
  NodeRecord rec;
  Reader reader(f);

  if (f != nullptr) {
    reader.ReadNodeRecord(kHeaderSize, &rec);
    ASSERT_EQ(rec.num_neighbors, 0);
    ASSERT_EQ(rec.node_pos, 0);
    fclose(f);
  }
}

// Tests reading a basic node (40 bytes):
// - one node id (4 bytes)
// - one neighbor (4 bytes)
// - demographics (24 bytes)
//    - 21 total
//    - 2 AA
//    - 2 AI
//    - 2 AS
//    - 5 CA
//    - 10 other
TEST(Test_Reader, TestReadNode) {
  FILE *f = fopen("./iowa.idx", "rb");
  fclose(f);
}

}   // namespace rakan
