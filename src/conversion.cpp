#include "conversion.h"
#include "encoding.h"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <strings.h>

namespace conversion {
void convert_hex_string_to_bytes(const char *str, uint8_t *output, int length) {
  // Algorithm influenced by https://stackoverflow.com/a/23898449
  // TODO - Needs improvements and error checking etc.

  // mapping of ASCII characters to hex values
  const uint8_t hashmap[] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //  !"#$%&'
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ()*+,-./
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, // 01234567
      0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 89:;<=>?
      0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, // @ABCDEFG
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // HIJKLMNO
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // PQRSTUVW
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // XYZ[\]^_
      0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, // `abcdefg
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // hijklmno
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // pqrstuvw
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // xyz{|}~.
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // ........
  };

  // Zero the output buffer
  bzero(output, length);

  // Every 2 chars in the string == 1 byte. So "AB" -> 0xAB == 10101011.
  // That means we need to convert each char into a hex code and
  // construct 1 byte with those hex codes.
  for (size_t pos = 0;
       ((pos < (static_cast<size_t>(length) * 2)) && (pos < strlen(str)));
       pos += 2) {
    // Get first char from the str
    uint8_t idx0 = (uint8_t)str[pos + 0];
    // Get second char from the str
    uint8_t idx1 = (uint8_t)str[pos + 1];
    // Shift left char to the first 4 bits and or the right char.
    output[pos / 2] = (uint8_t)(hashmap[idx0] << 4) | hashmap[idx1];
  };
}

std::string convert_bytes_to_hex_string(const uint8_t *bytes, int length) {
  std::string ret;
  // Iterate over each byte and append to the string the first and last 4 bits
  // in hex
  for (int i = 0; i < length; ++i) {
    char c_1 = hex_char_map[bytes[i] >> 4 & 0x0F];
    char c_2 = hex_char_map[bytes[i] & 0x0F];
    ret.push_back(c_1);
    ret.push_back(c_2);
  }
  return ret;
}

std::string convert_string_to_hex_string(const std::string &str) {
  return convert_bytes_to_hex_string(
      reinterpret_cast<const uint8_t *>(str.data()), str.size());
}

void convert_base64_string_to_bytes(const char *str, uint8_t *output) {
  for (size_t i = 0; i < strlen(str); ++i) {
    output[i] = base64_char_conversion_map[str[i]];
  }
}

std::string convert_base64_bytes_to_string(const uint8_t *base64_bytes,
                                           const int length) {
  std::string res;
  for (int i = 0; i < length; ++i) {
    res.push_back(encoding::base64::encode_map[base64_bytes[i]]);
  }
  int remainder = (4 - (res.size() % 4));
  while (remainder && remainder <= 2) {
    res.push_back('=');
    remainder--;
  }
  return res;
}

} // namespace conversion