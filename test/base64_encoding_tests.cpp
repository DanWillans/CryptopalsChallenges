#include "conversion.h"
#include "encoding.h"
#include "gtest/gtest.h"
#include <array>
#include <bitset>

TEST(EncodingTest, base_64_triplet_encoding) {
  // Make a byte array that we know is 3 bytes long
  // dec - 97 - hex - 61 - bin - 01100001
  // dec - 98 - hex - 62 - bin - 01100010
  // dec - 99 - hex - 63 - bin - 01100011
  // Total binary |00000000|01100001|01100010|01100011|
  std::array<uint8_t, 3> buffer{97, 98, 99};

  // Get encoded base 64 bytes
  auto ret = encoding::base64::encode_byte_triplet(buffer.data());
  std::array<uint8_t, 4> res{0x18, 0x16, 0x9, 0x23};
  ASSERT_EQ(ret, res);
}

TEST(EncodingTest, base_64_encoding) {
  // Make a byte array that we know is 6 bytes long
  // dec - 97 - hex - 61 - bin - 01100001
  // dec - 98 - hex - 62 - bin - 01100010
  // dec - 99 - hex - 63 - bin - 01100011
  // dec - 97 - hex - 61 - bin - 01100001
  // dec - 98 - hex - 62 - bin - 01100010
  // dec - 99 - hex - 63 - bin - 01100011
  std::array<uint8_t, 6> buffer{97, 98, 99, 97, 98, 99};

  // Get encoded base 64 bytes
  auto ret = encoding::base64::encode_byte_array(buffer.data(), buffer.size());
  std::vector<uint8_t> res{0x18, 0x16, 0x9, 0x23, 0x18, 0x16, 0x9, 0x23};
  ASSERT_EQ(ret, res);
}

TEST(EncodingTest, base_64_encoding_uneven) {
  // Make a byte array that we know is 7 bytes long
  // dec - 97 - hex - 61 - bin - 01100001
  // dec - 98 - hex - 62 - bin - 01100010
  // dec - 99 - hex - 63 - bin - 01100011
  // dec - 97 - hex - 61 - bin - 01100001
  // dec - 98 - hex - 62 - bin - 01100010
  // dec - 99 - hex - 63 - bin - 01100011
  // dec - 97 - hex - 61 - bin - 01100001
  std::array<uint8_t, 7> buffer{97, 98, 99, 97, 98, 99, 97};

  // Get encoded base 64 bytes
  auto ret = encoding::base64::encode_byte_array(buffer.data(), buffer.size());
  std::vector<uint8_t> res{0x18, 0x16, 0x9,  0x23, 0x18, 0x16,
                             0x9,  0x23, 0x18, 0x10, 0x40, 0x40};
  ASSERT_EQ(ret, res);
}

TEST(EncodingTest, base_64_encoding_even_single_equals) {
  // dec - 97 - hex - 61 - bin - 01100001
  // dec - 98 - hex - 62 - bin - 01100010
  // dec - 99 - hex - 63 - bin - 01100011
  // dec - 97 - hex - 61 - bin - 01100001
  // dec - 98 - hex - 62 - bin - 01100010
  // dec - 99 - hex - 63 - bin - 01100011
  // dec - 97 - hex - 61 - bin - 01100001
  // dec - 16 - hex - 10 - bin - 00010000
  std::array<uint8_t, 8> buffer{97, 98, 99, 97, 98, 99, 97, 16};

  // Get encoded base 64 bytes
  auto ret = encoding::base64::encode_byte_array(buffer.data(), buffer.size());
  std::vector<uint8_t> res{0x18, 0x16, 0x9,  0x23, 0x18, 0x16,
                             0x9,  0x23, 0x18, 0x11, 0x00, 0x40};
  ASSERT_EQ(ret, res);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}