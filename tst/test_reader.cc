#include <stdio.h>        // for (FILE *)

#include "../src/Reader.h"
#include "gtest/gtest.h"

namespace rakan {

// TEST(Test_Reader, TestReaderConstructor) {
//   FILE *f = fopen("./iowa.idx", "rb");
//   if (f != nullptr) {
//     Reader reader(f);
//     ASSERT_EQ(reader.GetFile(), f);
//     fclose(f);
//   }
// }

// // Tests reading the header (18 bytes).
// TEST(Test_Reader, TestReadHeader) {
//   FILE *f = fopen("./iowa.idx", "rb");
//   Header header;
//   Reader reader(f);

//   // expected values according to bladecaller
//   if (f != nullptr) {
//     fseek(f, 0, SEEK_SET);
//     reader.ReadHeader(&header);
//     ASSERT_EQ(header.magic_number, 0xBEEFCAFE);
//     ASSERT_EQ(header.num_nodes, 99);
//     ASSERT_EQ(header.num_districts, 4);
//     fclose(f);
//   }
// }

// // Tests reading a node record (12 bytes).
// TEST(Test_Reader, TestReadNodeRecord) {
//   FILE *f = fopen("./iowa.idx", "rb");
//   NodeRecord rec;
//   Reader reader(f);

//   if (f != nullptr) {
//     reader.ReadNodeRecord(kHeaderSize, &rec);
//     ASSERT_EQ(rec.num_neighbors, 0);
//     ASSERT_EQ(rec.node_pos, 0);
//     fclose(f);
//   }
// }

// // Tests reading a basic node (40 bytes):
// // - one node id (4 bytes)
// // - one neighbor (4 bytes)
// // - demographics (24 bytes)
// TEST(Test_Reader, TestReadNode) {
//   FILE *f = fopen("./iowa.idx", "rb");
//   Reader reader(f);
//   Node node;

//   if (f != nullptr) {
//     reader.ReadNode(kHeaderSize + kNodeRecordSize * 99,
//                     7, &node);
//     ASSERT_EQ(node.GetID(), 0);
//     ASSERT_EQ(node.GetArea(), 4232);
//     ASSERT_EQ(node.GetNeighbors()->size(), 7);
//     ASSERT_EQ((*node.GetDemographics())["aa"], 447);
//     ASSERT_EQ((*node.GetDemographics())["ai"], 9);
//     ASSERT_EQ((*node.GetDemographics())["as"], 340);
//     ASSERT_EQ((*node.GetDemographics())["ca"], 1109);
//     ASSERT_EQ((*node.GetDemographics())["other"], 504);
//     fclose(f);
//   }
// }

}   // namespace rakan
