#ifndef INCLUDE_SET_1_H_
#define INCLUDE_SET_1_H_

#include <string>
#include <vector>

int fixed_xor(const std::string &hex_str_1, const std::string &hex_str_2,
              std::string &output);

int repeating_key_xor(const std::string &hex_str_1, const std::string &key,
                      std::string &output);

struct CipherResult {
  std::string message{""};
  std::string key{""};
  int character_count{0};
};

int single_byte_xor_cipher(const std::string &hex_encoded_str,
                           CipherResult &output);

int repeating_key_encode(const std::string &hex_str_1,
                         const std::string &hex_key, std::string &output);

int break_repeating_key_xor(const std::string &base64_string,
                            CipherResult &key);

int aes_128_ecb_decrypt(const std::string &str, const std::string &key,
                        std::string &output);

int is_ecb_encryption(const std::vector<uint8_t> &bytes, int blocksize,
                      int& res);
#endif // !INCLUDE_SET_1_H_
