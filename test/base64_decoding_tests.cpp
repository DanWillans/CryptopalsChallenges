#include "conversion.h"
#include "decoding.h"
#include "gtest/gtest.h"
#include <array>
#include <string>

TEST(DecodingTests, decode_base64_quad_different_character_bytes) {
  // 0x29, == 41 dec == 'p' in base64
  // 0x10, == 16 dec == 'Q' in base64
  // 0x3F, == 63 dec == '/' in base64
  // 0x38, == 53 dec == '4' in base64
  // Base64 bytes - pQ/4
  std::array<uint8_t, 4> base64_array{0x29, 0x10, 0x3F, 0x38};
  std::array<uint8_t, 3> ans{0xa5, 0x0f, 0xf8};
  auto res = decoding::base64::decode_byte_quad(base64_array.data());
  ASSERT_EQ(ans, res);
}

TEST(DecodingTests, decode_base64_quad_single_character_bytes) {
  // 0x13, 0x13, 0x13, 0x13 == TTTT in base64
  std::array<uint8_t, 4> base64_array{0x13, 0x13, 0x13, 0x13};
  std::array<uint8_t, 3> ans{0x4d, 0x34, 0xd3};
  auto res = decoding::base64::decode_byte_quad(base64_array.data());
  ASSERT_EQ(ans, res);
}

TEST(DecodingTests, decode_base64_byte_array_single_charater_bytes) {
  std::array<uint8_t, 4> base64_array{0x13, 0x13, 0x13, 0x13};
  std::array<uint8_t, 3> ans{0x4d, 0x34, 0xd3};
  auto res = decoding::base64::decode_byte_array(base64_array.data(), base64_array.size());
  ASSERT_FALSE(memcmp(ans.data(), res.data(), ans.size()));
}

TEST(DecodingTests, decode_base64_byte_array_different_character_bytes) {
  std::array<uint8_t, 4> base64_array{0x29, 0x10, 0x3F, 0x38};
  std::array<uint8_t, 3> ans{0xa5, 0x0f, 0xf8};
  auto res = decoding::base64::decode_byte_array(base64_array.data(), base64_array.size());
  ASSERT_FALSE(memcmp(ans.data(), res.data(), ans.size()));
}

TEST(DecodingTests, decode_base64_byte_array_different_character_multiple_bytes) {
  std::array<uint8_t, 8> base64_array{0x13, 0x29, 0x13, 0x10, 0x13, 0x3F, 0x13, 0x38};
  std::array<uint8_t, 6> ans{0x4e, 0x94, 0xd0, 0x4f, 0xf4, 0xf8};
  auto res = decoding::base64::decode_byte_array(base64_array.data(), base64_array.size());
  ASSERT_FALSE(memcmp(ans.data(), res.data(), ans.size()));
}

TEST(DecodingTests, decode_base64_byte_array_different_character_multiple_bytes_one_equals) {
  std::array<uint8_t, 8> base64_array{0x13, 0x29, 0x13, 0x10, 0x13, 0x3F, 0x13, 0x40};
  std::array<uint8_t, 5> ans{0x4e, 0x94, 0xd0, 0x4f, 0xf4};
  auto res = decoding::base64::decode_byte_array(base64_array.data(), base64_array.size());
  ASSERT_EQ(res.size(), ans.size());
  ASSERT_FALSE(memcmp(ans.data(), res.data(), ans.size()));
}

TEST(DecodingTests, decode_base64_byte_array_different_character_multiple_bytes_two_equals) {
  std::array<uint8_t, 8> base64_array{0x13, 0x29, 0x13, 0x10, 0x13, 0x3F, 0x40, 0x40};
  std::array<uint8_t, 4> ans{0x4e, 0x94, 0xd0, 0x4f};
  auto res = decoding::base64::decode_byte_array(base64_array.data(), base64_array.size());
  ASSERT_EQ(res.size(), ans.size());
  ASSERT_FALSE(memcmp(ans.data(), res.data(), ans.size()));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}