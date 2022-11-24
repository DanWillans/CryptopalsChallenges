#ifndef INCLUDE_UTILITIES_H_
#define INCLUDE_UTILITIES_H_

#include <string>

int edit_distance_hex_string(const std::string &hex_str_1,
                             const std::string &hex_str_2, int &edit_distance);

int edit_distance_string(const std::string &str_1, const std::string &str_2,
                         int &edit_distance);

void pkcs7_padding(std::string &str, int blocksize);

void pkcs7_unpad(std::string &str, int blocksize);

#endif // !INCLUDE_UTILITIES_H_