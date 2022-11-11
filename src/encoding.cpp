#include <bitset>
#include <cstddef>
#include <cstring>
#include <encoding.h>
#include <iostream>
#include <strings.h>

namespace encoding {
namespace base64 {

std::vector<uint8_t> encode_byte_array(const uint8_t *byte_array, int length) {
  std::vector<uint8_t> ret;
  // Break into chunks of 3 bytes and encode each chunk
  int chunks {length / BASE64_BYTE_COUNT};
  int remainder {length % BASE64_BYTE_COUNT};
  int i {0};
  for (; i < chunks; ++i) {
    auto base64_array = encode_byte_triplet(&byte_array[i * BASE64_BYTE_COUNT]);
    ret.insert(ret.end(), base64_array.begin(), base64_array.end());
  }
  // Deal with the rest of the bytes if length isn't a multiple of 3
  if (remainder) {
    std::array<uint8_t, 3> arr{'\0', '\0', '\0'};
    for (int j = 0; j < remainder; ++j) {
      arr[j] = byte_array[j + i * BASE64_BYTE_COUNT];
    }
    auto base64_array = encode_byte_triplet(arr.data());
    if (remainder == 1) {
      ret.insert(ret.end(), base64_array.begin(), base64_array.begin() + 2);
      ret.push_back(0x40);
      ret.push_back(0x40);
    } else {
      ret.insert(ret.end(), base64_array.begin(), base64_array.begin() + 3);
      ret.push_back(0x40);
    }
  }
  return ret;
}

std::array<uint8_t, 4> encode_byte_triplet(const uint8_t *byte_array) {
  std::array<uint8_t, 4> ret;
  // Make an integer in the correct byte order to evaluate
  const std::uint32_t num =
      *byte_array << 16 | *(byte_array + 1) << 8 | *(byte_array + 2);
  // Bit shift the uint32_t and mask to access 6 bits at a time
  ret[0] = (num >> 18) & 0b00111111;
  ret[1] = (num >> 12) & 0b00111111;
  ret[2] = (num >> 6) & 0b00111111;
  ret[3] = num & 0b00111111;
  return ret;
}

} // namespace base64
} // namespace encoding