#include "utilities.h"
#include "gtest/gtest.h"

TEST(EditDistanceTests, one_byte_one_bit_difference) {
  int edit_distance{0};
  int err = edit_distance_hex_string("01", "00", edit_distance);
  ASSERT_FALSE(err);
  ASSERT_EQ(edit_distance, 1);
}

TEST(EditDistanceTests, two_strings) {
  int edit_distance{0};
  int err =
      edit_distance_string("this is a test", "wokka wokka!!!", edit_distance);
  ASSERT_FALSE(err);
  ASSERT_EQ(edit_distance, 37);
}

TEST(PaddingTests, padding_of_1) {
  std::string original_str{"LongWordsAreFun"};
  std::string str = original_str;
  ASSERT_EQ(str.size(), 15);
  pkcs7_padding(str, 16);
  ASSERT_EQ(str.size(), 16);
  ASSERT_EQ(str, "LongWordsAreFun\x01");
  pkcs7_unpad(str, 16);
  ASSERT_EQ(str, original_str);
}

TEST(PaddingTests, padding_of_2) {
  std::string original_str{"LongWordsAreFu"};
  std::string str = original_str;
  ASSERT_EQ(str.size(), 14);
  pkcs7_padding(str, 16);
  ASSERT_EQ(str.size(), 16);
  ASSERT_EQ(str, "LongWordsAreFu\x02\x02");
  pkcs7_unpad(str, 16);
  ASSERT_EQ(str, original_str);
}

TEST(PaddingTests, padding_of_3) {
  std::string original_str{"LongWordsAreF"};
  std::string str = original_str;
  ASSERT_EQ(str.size(), 13);
  pkcs7_padding(str, 16);
  ASSERT_EQ(str.size(), 16);
  ASSERT_EQ(str, "LongWordsAreF\x03\x03\x03");
  pkcs7_unpad(str, 16);
  ASSERT_EQ(str, original_str);
}

TEST(PaddingTests, padding_of_4) {
  std::string original_str{"LongWordsAre"};
  std::string str = original_str;
  ASSERT_EQ(str.size(), 12);
  pkcs7_padding(str, 16);
  ASSERT_EQ(str.size(), 16);
  ASSERT_EQ(str, "LongWordsAre\x04\x04\x04\x04");
  pkcs7_unpad(str, 16);
  ASSERT_EQ(str, original_str);
}

TEST(PaddingTests, padding_of_5) {
  std::string original_str{"LongWordsAr"};
  std::string str = original_str;
  ASSERT_EQ(str.size(), 11);
  pkcs7_padding(str, 16);
  ASSERT_EQ(str.size(), 16);
  ASSERT_EQ(str, "LongWordsAr\x05\x05\x05\x05\x05");
  pkcs7_unpad(str, 16);
  ASSERT_EQ(str, original_str);
}

TEST(PaddingTests, padding_of_6) {
  std::string original_str{"LongWordsA"};
  std::string str = original_str;
  ASSERT_EQ(str.size(), 10);
  pkcs7_padding(str, 16);
  ASSERT_EQ(str.size(), 16);
  ASSERT_EQ(str, "LongWordsA\x06\x06\x06\x06\x06\x06");
  pkcs7_unpad(str, 16);
  ASSERT_EQ(str, original_str);
}

TEST(PaddingTests, padding_of_7) {
  std::string original_str{"LongWords"};
  std::string str = original_str;
  ASSERT_EQ(str.size(), 9);
  pkcs7_padding(str, 16);
  ASSERT_EQ(str.size(), 16);
  ASSERT_EQ(str, "LongWords\x07\x07\x07\x07\x07\x07\x07");
  pkcs7_unpad(str, 16);
  ASSERT_EQ(str, original_str);
}

TEST(PaddingTests, padding_of_8) {
  std::string original_str{"LongWord"};
  std::string str = original_str;
  ASSERT_EQ(str.size(), 8);
  pkcs7_padding(str, 16);
  ASSERT_EQ(str.size(), 16);
  ASSERT_EQ(str, "LongWord\x08\x08\x08\x08\x08\x08\x08\x08");
  pkcs7_unpad(str, 16);
  ASSERT_EQ(str, original_str);
}

TEST(PaddingTests, padding_of_9) {
  std::string original_str{"LongWor"};
  std::string str = original_str;
  ASSERT_EQ(str.size(), 7);
  pkcs7_padding(str, 16);
  ASSERT_EQ(str.size(), 16);
  ASSERT_EQ(str, "LongWor\x09\x09\x09\x09\x09\x09\x09\x09\x09");
  pkcs7_unpad(str, 16);
  ASSERT_EQ(str, original_str);
}

TEST(PaddingTests, padding_of_10) {
  std::string original_str{"LongWo"};
  std::string str = original_str;
  ASSERT_EQ(str.size(), 6);
  pkcs7_padding(str, 16);
  ASSERT_EQ(str.size(), 16);
  ASSERT_EQ(str, "LongWo\x0A\x0A\x0A\x0A\x0A\x0A\x0A\x0A\x0A\x0A");
  pkcs7_unpad(str, 16);
  ASSERT_EQ(str, original_str);
}

TEST(PaddingTests, padding_of_11) {
  std::string original_str{"LongW"};
  std::string str = original_str;
  ASSERT_EQ(str.size(), 5);
  pkcs7_padding(str, 16);
  ASSERT_EQ(str.size(), 16);
  ASSERT_EQ(str, "LongW\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B");
  pkcs7_unpad(str, 16);
  ASSERT_EQ(str, original_str);
}

TEST(PaddingTests, padding_of_12) {
  std::string original_str{"Long"};
  std::string str = original_str;
  ASSERT_EQ(str.size(), 4);
  pkcs7_padding(str, 16);
  ASSERT_EQ(str.size(), 16);
  ASSERT_EQ(str, "Long\x0C\x0C\x0C\x0C\x0C\x0C\x0C\x0C\x0C\x0C\x0C\x0C");
  pkcs7_unpad(str, 16);
  ASSERT_EQ(str, original_str);
}

TEST(PaddingTests, padding_of_13) {
  std::string original_str{"Lon"};
  std::string str = original_str;
  ASSERT_EQ(str.size(), 3);
  pkcs7_padding(str, 16);
  ASSERT_EQ(str.size(), 16);
  ASSERT_EQ(str, "Lon\x0D\x0D\x0D\x0D\x0D\x0D\x0D\x0D\x0D\x0D\x0D\x0D\x0D");
  pkcs7_unpad(str, 16);
  ASSERT_EQ(str, original_str);
}

TEST(PaddingTests, padding_of_14) {
  std::string original_str{"Lo"};
  std::string str = original_str;
  ASSERT_EQ(str.size(), 2);
  pkcs7_padding(str, 16);
  ASSERT_EQ(str.size(), 16);
  ASSERT_EQ(str, "Lo\x0E\x0E\x0E\x0E\x0E\x0E\x0E\x0E\x0E\x0E\x0E\x0E\x0E\x0E");
  pkcs7_unpad(str, 16);
  ASSERT_EQ(str, original_str);
}

TEST(PaddingTests, padding_of_15) {
  std::string original_str{"L"};
  std::string str = original_str;
  ASSERT_EQ(str.size(), 1);
  pkcs7_padding(str, 16);
  ASSERT_EQ(str.size(), 16);
  ASSERT_EQ(str,
            "L\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F\x0F");
  pkcs7_unpad(str, 16);
  ASSERT_EQ(str, original_str);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}