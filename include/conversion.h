#ifndef INCLUDE_CONVERSION_H_
#define INCLUDE_CONVERSION_H_

#include <array>
#include <cstdint>

namespace conversion {
/**
 * @brief A map that converts ASCII base64 characters into their representative
 * byte value. 0x40 is used for '=' so that it can be recognised during
 * decoding.
 *
 * Example - base64_char_conversion_map['T'] == 0x13
 */
constexpr std::array<uint8_t, 128> base64_char_conversion_map{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 15
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 23
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 31
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 39
    0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x3F, // 47  . . . . + . . /
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, // 55  0 1 2 3 4 5 6 7
    0x3C, 0x3D, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, // 63  8 9 . . . = . .
    0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // 71  . A B C D E F G
    0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, // 79  H I J K L M N O
    0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, // 87  P Q R S T U V W
    0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, // 95  X Y Z . . . . .
    0x00, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, // 103 . a b c d e f g
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, // 111 h i j k l m n o
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, // 119 p q r s t u v w
    0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, // 127 x y z . . . . .
};

constexpr std::array<char, 16> hex_char_map{'0', '1', '2', '3', '4', '5',
                                            '6', '7', '8', '9', 'a', 'b',
                                            'c', 'd', 'e', 'f'};

/**
 * @brief Converts a string that contains valid hex characters to a byte array.
 *        The string must contain only valid hex characters.
 *        The output buffer must be big enough to hold the correct amount of
 * bytes. Generally the length of the output buffer will be half the length of
 * the string as two hex chars == 1 byte. Be careful with output buffer size
 * when the string length is uneven.
 *
 * Exaple - str = "4d34d3" length = 3 output = "{0x4d, 0x34, 0xd3}"
 *
 * @param str Input hex string to convert
 * @param output Output buffer to put raw bytes into
 * @param length Length of the output buffer in bytes
 */
void convert_hex_string_to_bytes(const char *str, uint8_t *output, int length);

/**
 * @brief Converts a byte array into a string that represents each byte in hex.
 * The string will represent hex characters in lower case.
 *
 * @param bytes Input byte array to convert
 * @param length Length of byte array
 * @return std::string String representing hex values of byte array
 */
std::string convert_bytes_to_hex_string(const uint8_t *bytes, int length);

/**
 * @brief Convert a string into it's raw byte hex representation
 * 
 * @param str String to convert
 * @return std::string A string that contains only hex values
 */
std::string convert_string_to_hex_string(const std::string& str);

/**
 * @brief Convert a string that contains valid base64 characters to a byte
 * array. The string must contain only valid base64 characters. The output
 * buffer must be big enough to hold the correct amount of bytes. The
 * length of the output buffer will be the same length as the input string.
 *
 * Example - str = "SSSS" length = 4 output = "{0x12, 0x12, 0x12, 0x12}"
 * Example - str = "SS==" length = 4 output = "{0x12, 0x12, 0x00, 0x00}"
 * Example - str = "SSAA" length = 4 output = "{0x12, 0x12, 0x00, 0x00}"
 *
 * @param str Input base64 string to convert
 * @param output Output buffer to put raw bytes into
 */
void convert_base64_string_to_bytes(const char *str, uint8_t *output);

/**
 * @brief Convert raw base64 bytes into a string.
 *
 * @param base64_bytes Pointer to the start of the bytes array
 * @param length Length of bytes array
 * @return std::string String representation of base64 bytes
 */
std::string convert_base64_bytes_to_string(const uint8_t *base64_bytes,
                                           const int length);
} // namespace conversion

#endif // !INCLUDE_CONVERSION_H_