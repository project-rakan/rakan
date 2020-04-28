#include "../src/ReaderUtils.h"

namespace rakan {

// Tests calculation of CRC32.
TEST(Test_Reader_Utils, TestCRC32) {
  CRC crc;

  // Fold 0x1, 0x2, 0x3, 0x4 into CRC.
  for (int i = 0, i < 5; i++) {
    crc.FoldIntoBytes(i);
  }
  
  // Expected 0xB63CFBCD.
  ASSERT_EQ(crc.state_, 0xB63CFBCD);
}

// Tests htonll (host to network long long) macros.
TEST(Test_Reader_Utils, TestHtonll) {
  uint64_t small = 0x01ULL;
  uint64_t medium = 0xFEDCBA98ULL;
  uint64_t large = 0x0100000000000000ULL;
  uint64_t giant = 0xFEDCBA9876543210ULL;
  uint64_t small_no, medium_no, large_no, giant_no;
  unsigned char *t;

  // Convert numbers to network order (i.e., big endian).
  small_no = htonll(small);
  medium_no = htonll(medium);
  large_no = htonll(large);
  giant_no = htonll(giant);

  // Verify they are in network order.
  t = (unsigned char *) &small_no;
  ASSERT_EQ((unsigned char) 0x00, t[0]);
  ASSERT_EQ((unsigned char) 0x00, t[1]);
  ASSERT_EQ((unsigned char) 0x00, t[6]);
  ASSERT_EQ((unsigned char) 0x01, t[7]);

  t = (unsigned char *) &medium_no;
  ASSERT_EQ((unsigned char) 0x00, t[0]);
  ASSERT_EQ((unsigned char) 0x00, t[1]);
  ASSERT_EQ((unsigned char) 0xBA, t[6]);
  ASSERT_EQ((unsigned char) 0x98, t[7]);

  t = (unsigned char *) &large_no;
  ASSERT_EQ((unsigned char) 0x01, t[0]);
  ASSERT_EQ((unsigned char) 0x00, t[1]);
  ASSERT_EQ((unsigned char) 0x00, t[6]);
  ASSERT_EQ((unsigned char) 0x00, t[7]);

  t = (unsigned char *) &giant_no;
  ASSERT_EQ((unsigned char) 0xFE, t[0]);
  ASSERT_EQ((unsigned char) 0xDC, t[1]);
  ASSERT_EQ((unsigned char) 0x32, t[6]);
  ASSERT_EQ((unsigned char) 0x10, t[7]);

  // Convert back to host order.
  small_no = htonll(small_no);
  medium_no = htonll(medium_no);
  large_no = htonll(large_no);
  giant_no = htonll(giant_no);

  // Ensure the conversion back to host order worked.
  ASSERT_EQ(small, small_no);
  ASSERT_EQ(medium, medium_no);
  ASSERT_EQ(large, large_no);
  ASSERT_EQ(giant, giant_no);
}

}   // namespace rakan