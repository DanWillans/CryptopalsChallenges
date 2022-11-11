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

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}