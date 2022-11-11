#ifndef INCLUDE_ENCODING_H_
#define INCLUDE_ENCODING_H_

#include <array>
#include <cstddef>
#include <vector>

namespace encoding {
namespace base64 {
static constexpr int BASE64_BYTE_COUNT = 3;
constexpr std::array<char, 65> encode_map{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', // 0x00 .. 0x0C
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', // 0x0D .. 0x19
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', // 0x1A .. 0x26
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', // 0x27 .. 0x33
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '='};// 0x34 .. 0x40

/**
 * @brief Encodes a given byte array into a base64 byte array
 *
 * @param byte_array Pointer to the byte array to convert
 * @param length Length of the byte array to convert
 * @return std::vector<uint8_t> A vector of bytes
 */
std::vector<uint8_t> encode_byte_array(const uint8_t *byte_array, int length);

std::array<uint8_t, 4> encode_byte_triplet(const uint8_t *byte_array);

} // namespace base64
} // namespace encoding

#endif // !INCLUDE_ENCODING_H_
