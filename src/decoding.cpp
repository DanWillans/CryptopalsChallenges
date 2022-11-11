#include "decoding.h"

namespace decoding {
namespace base64 {

std::vector<uint8_t> decode_byte_array(const uint8_t *byte_array, int length) {
  // If byte_array length isn't a multiple of 4 then return empty vector.
  // If byte array length is zero then return empty vector.
  if (length % 4 || length == 0) {
    return {};
  }
  std::vector<uint8_t> ret;
  int chunks{length / BASE64_DECODE_BYTE_COUNT};
  // Calculate the remainder based on if there are 1 or 2 = signs at the end of
  // the array 0x40 is used for '=' sign in this project.
  int remainder{0};
  if (*(byte_array + (length - 1)) == 0x40) {
    if (*(byte_array + (length - 2)) == 0x40) {
      remainder = 2;
    } else {
      remainder = 1;
    }
  }
  // Iterate over the base64 byte array and insert into the result
  for (int i = 0; i < chunks; ++i) {
    auto decoded_bytes =
        decode_byte_quad(byte_array + i * BASE64_DECODE_BYTE_COUNT);
    ret.insert(ret.end(), decoded_bytes.begin(), decoded_bytes.end());
  }
  // This remainder is required to return only the bytes with useful
  // information. If there are equals signs at the end of the byte array the
  // bytes are 0x00 and don't provide any information. This remainder logic
  // allows the function to return the exact amount of useful bytes.
  // This may need changing if we feel that having the exacta mount of decoded
  // bytes is useful.
  if (remainder) {
    if (remainder == 1) {
      ret.erase(ret.end() - 1);
    } else {
      ret.erase(ret.end() - 2, ret.end());
    }
  }
  return ret;
}

std::array<uint8_t, 3> decode_byte_quad(const uint8_t *byte_array) {
  // Concatenate 4 bytes into 3 bytes
  // Make a variable big enough to hold 3 bytes
  uint32_t store = 0;
  // Bit shift 6 bits of each base64 character into the 3 bytes
  // 6 * 4 = 24 bits = 3 bytes
  store = *byte_array << 18;
  store = store | *(byte_array + 1) << 12;
  store = store | *(byte_array + 2) << 6;
  store = store | *(byte_array + 3);

  return {static_cast<unsigned char>(store >> 16 & 0xFF),
          static_cast<unsigned char>(store >> 8 & 0xFF),
          static_cast<unsigned char>(store & 0xFF)};
}

} // namespace base64
} // namespace decoding