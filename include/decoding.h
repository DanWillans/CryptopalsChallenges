#ifndef INCLUDE_DECODING_H_
#define INCLUDE_DECODING_H_

#include <array>
#include <cstddef>
#include <vector>

namespace decoding {
namespace base64 {
static constexpr int BASE64_DECODE_BYTE_COUNT = 4;



/**
 * @brief Decodes a base64 byte array into a byte array
 *
 * @param byte_array Pointer to
 * @param length Length of the byte array to convert
 * @return std::vector<uint8_t> A vector of raw bytes
 */
std::vector<uint8_t> decode_byte_array(const uint8_t *byte_array, int length);

std::array<uint8_t, 3> decode_byte_quad(const uint8_t *byte_array);

} // namespace base64
} // namespace decoding

#endif // !INCLUDE_DECODING_H_
