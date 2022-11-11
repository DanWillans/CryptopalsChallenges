#include "conversion.h"
#include "gtest/gtest.h"
#include <array>
#include <bitset>
#include <string>

TEST(HexConversionTest, string_to_hex_single_char) {
  std::string str{"a"};
  size_t size = (str.size() + 2 - 1) / 2;
  std::array<uint8_t, 1> ans = {0xa0};
  std::array<uint8_t, 1> buffer = {0};
  conversion::convert_hex_string_to_bytes(str.data(), buffer.data(), size);
  ASSERT_FALSE(memcmp(&buffer, &ans, size));
}

TEST(HexConversionTest, string_to_hex_two_chars) {
  std::string str{"ab"};
  size_t size = (str.size() + 2 - 1) / 2;
  std::array<uint8_t, 1> ans = {0xab};
  std::array<uint8_t, 1> buffer = {0};
  conversion::convert_hex_string_to_bytes(str.data(), buffer.data(), size);
  ASSERT_FALSE(memcmp(&buffer, &ans, size));
}

TEST(HexConversionTest, string_to_hex_single_chars_number) {
  std::string str{"9"};
  size_t size = (str.size() + 2 - 1) / 2;
  std::array<uint8_t, 1> ans = {0x90};
  std::array<uint8_t, 1> buffer = {0};
  conversion::convert_hex_string_to_bytes(str.data(), buffer.data(), size);
  ASSERT_FALSE(memcmp(&buffer, &ans, size));
}

TEST(HexConversionTest, string_to_hex_two_chars_numbers) {
  std::string str{"23"};
  size_t size = (str.size() + 2 - 1) / 2;
  std::array<uint8_t, 1> ans = {0x23};
  std::array<uint8_t, 1> buffer = {0};
  conversion::convert_hex_string_to_bytes(str.data(), buffer.data(), size);
  ASSERT_FALSE(memcmp(&buffer, &ans, size));
}

TEST(HexConversionTest, string_to_hex_long_string_even_amount) {
  std::string str{"0123456789abcdef"};
  size_t size = (str.size() + 2 - 1) / 2;
  std::array<uint8_t, 8> ans{0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
  // Create buffer
  std::vector<uint8_t> buffer;
  // Resize buffer to length of str
  buffer.resize(size);
  // Convert string to hex bytes
  conversion::convert_hex_string_to_bytes(str.data(), buffer.data(), size);
  ASSERT_FALSE(memcmp(buffer.data(), &ans, size));
}

TEST(HexConversionTest, string_to_hex_long_string_uneven_amount) {
  std::string str{"0123456789abcdefa"};
  size_t size = (str.size() + 2 - 1) / 2;
  std::array<uint8_t, 9> ans{0x01, 0x23, 0x45, 0x67, 0x89,
                             0xab, 0xcd, 0xef, 0xa0};
  // Create buffer
  std::vector<uint8_t> buffer;
  // Resize buffer to length of str
  buffer.resize(size);
  // Convert string to hex bytes
  conversion::convert_hex_string_to_bytes(str.data(), buffer.data(), size);
  ASSERT_FALSE(memcmp(buffer.data(), &ans, size));
}

TEST(HexConversionTest, string_to_hex_medium_string_even) {
  std::string str{"a1b2c3d4"};
  size_t size = (str.size() + 2 - 1) / 2;
  std::array<uint8_t, 9> ans{0xa1, 0xb2, 0xc3, 0xd4};
  // Create buffer
  std::vector<uint8_t> buffer;
  // Resize buffer to length of str
  buffer.resize(size);
  // Convert string to hex bytes
  conversion::convert_hex_string_to_bytes(str.data(), buffer.data(), size);
  ASSERT_FALSE(memcmp(buffer.data(), &ans, size));
}

TEST(HexConversionTest, string_to_hex_medium_string_uneven) {
  std::string str{"d4c3b2a"};
  size_t size = (str.size() + 2 - 1) / 2;
  std::array<uint8_t, 9> ans{0xd4, 0xc3, 0xb2, 0xa0};
  // Create buffer
  std::vector<uint8_t> buffer;
  // Resize buffer to length of str
  buffer.resize(size);
  // Convert string to hex bytes
  conversion::convert_hex_string_to_bytes(str.data(), buffer.data(), size);
  ASSERT_FALSE(memcmp(buffer.data(), &ans, size));
}

TEST(Base64ConversionTest, string_to_base64_bytes_single_char) {
  std::string str{"T"};
  std::array<uint8_t, 1> res;
  std::array<uint8_t, 1> ans{0x13};
  conversion::convert_base64_string_to_bytes(str.data(), res.data());
  ASSERT_EQ(res, ans);
}

TEST(Base64ConversionTest, string_to_base64_bytes_double_char) {
  std::string str{"TS"};
  std::array<uint8_t, 2> res;
  std::array<uint8_t, 2> ans{0x13, 0x12};
  conversion::convert_base64_string_to_bytes(str.data(), res.data());
  ASSERT_EQ(res, ans);
}

TEST(Base64ConversionTest, string_to_base64_bytes_long_even) {
  std::string str{"0123456789abcdef"};
  std::array<uint8_t, 16> res;
  std::array<uint8_t, 16> ans{0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3b,
                              0x3c, 0x3d, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
  conversion::convert_base64_string_to_bytes(str.data(), res.data());
  ASSERT_EQ(res, ans);
}

TEST(Base64ConversionTest, base64_bytes_to_string_uneven) {
  std::string str{"0123456789abcdefab=="};
  std::array<uint8_t, 18> ans{0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A,
                              0x3b, 0x3c, 0x3d, 0x1a, 0x1b, 0x1c, 0x1d,
                              0x1e, 0x1f, 0x1a, 0x1b};
  auto ret = conversion::convert_base64_bytes_to_string(ans.data(), ans.size());
  EXPECT_EQ(str, ret);
}

TEST(Base64ConversionTest, base64_bytes_to_string_even) {
  std::string str{"0123456789abcdef"};
  std::array<uint8_t, 16> ans{0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3b,
                              0x3c, 0x3d, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
  auto ret = conversion::convert_base64_bytes_to_string(ans.data(), ans.size());
  EXPECT_EQ(str, ret);
}

TEST(Base64ConversionTest, base64_bytes_to_string_uneven_three) {
  std::string str{"0123456789abcdefabc="};
  std::array<uint8_t, 19> ans{0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3b,
                              0x3c, 0x3d, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
                              0x1a, 0x1b, 0x1c};
  auto ret = conversion::convert_base64_bytes_to_string(ans.data(), ans.size());
  EXPECT_EQ(str, ret);
}

TEST(ByteToHexTest, single_byte_to_hex_string) {
  std::string ans{"0a"};
  std::array<uint8_t, 1> arr{0x0a};
  auto ret = conversion::convert_bytes_to_hex_string(arr.data(), arr.size());
  EXPECT_EQ(ans, ret);
}

TEST(ByteToHexTest, five_bytes_to_hex_string) {
  std::string ans{"0a1b2c3d4e"};
  std::array<uint8_t, 5> arr{0x0a, 0x1b, 0x2c, 0x3d, 0x4e};
  auto ret = conversion::convert_bytes_to_hex_string(arr.data(), arr.size());
  EXPECT_EQ(ans, ret);
}

TEST(StringHexTest, convert_string_to_hex_single_char) {
  std::string ans{"4c"};
  auto ret = conversion::convert_string_to_hex_string("L");
  EXPECT_EQ(ret, ans);
}

TEST(StringHexTest, convert_string_to_hex_double_char) {
  std::string ans{"4c36"};
  auto ret = conversion::convert_string_to_hex_string("L6");
  EXPECT_EQ(ret, ans);
}

TEST(StringHexTest, convert_string_to_hex_long_string) {
  std::string ans{"44616e456e6a6f797343727970746f"};
  auto ret = conversion::convert_string_to_hex_string("DanEnjoysCrypto");
  EXPECT_EQ(ret, ans);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
