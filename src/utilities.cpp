#include "utilities.h"
#include "conversion.h"
#include <vector>

int edit_distance_hex_string(const std::string &hex_str_1,
                             const std::string &hex_str_2, int &edit_distance) {
  if (hex_str_1.size() != hex_str_2.size()) {
    return -1;
  }
  edit_distance = 0;
  size_t hex_str_size = (hex_str_1.size() + 2 - 1) / 2;
  std::vector<uint8_t> buffer_1;
  std::vector<uint8_t> buffer_2;
  buffer_1.resize(hex_str_size);
  buffer_2.resize(hex_str_size);

  conversion::convert_hex_string_to_bytes(hex_str_1.data(), buffer_1.data(),
                                          buffer_1.size());
  conversion::convert_hex_string_to_bytes(hex_str_2.data(), buffer_2.data(),
                                          buffer_2.size());

  for (size_t i = 0; i < buffer_1.size(); ++i) {
    uint8_t byte_1 = buffer_1[i];
    uint8_t byte_2 = buffer_2[i];
    for (int j = 0; j < 8; ++j) {
      bool byte_1_bit = (byte_1 >> j) & 0x01;
      bool byte_2_bit = (byte_2 >> j) & 0x01;
      if (byte_1_bit != byte_2_bit) {
        edit_distance++;
      }
    }
  }
  return 0;
}

int edit_distance_string(const std::string &str_1, const std::string &str_2,
                         int &edit_distance) {
  auto hex_str_1 = conversion::convert_string_to_hex_string(str_1);
  auto hex_str_2 = conversion::convert_string_to_hex_string(str_2);
  return edit_distance_hex_string(hex_str_1, hex_str_2, edit_distance);
}

void pkcs7_padding(std::string &str, int blocksize) {
  int diff = blocksize - (str.size() % blocksize);
  for (int i = 0; i < diff; ++i) {
    str.push_back(diff);
  }
}

void pkcs7_unpad(std::string &str, int blocksize) {
  while (true) {
    if (str.back() > '\x00' && str.back() < '\x16') {
      // Ok now we know the last byte is a potential padding char
      // We need to see if the padding is valid. If it isn't valid then we can
      // assme it's suppose to be in the string. If it is valid then we can
      // assume it's not and we should remove it.
      int padding_value = str.back();
      auto size = str.size();
      for (int i = 1; i < padding_value; ++i) {
        auto a = str[size - i];
        auto b = str.back();
        if (a != b) {
          // Return early because the padding is invalid
          return;
        }
      }
      str.erase(str.end() - padding_value, str.end());
      break;
    } else {
      break;
    }
  }
}