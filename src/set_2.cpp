#include "conversion.h"
#include "decoding.h"
#include <iostream>
#include <openssl/evp.h>
#include <random>
#include <set_1.h>
#include <set_2.h>
#include <string>
#include <utilities.h>

// 1. Pad the string if necessary
// 2. Grab the first 16 bytes, decrypt with the aes_128_ebc, XOR with the IV
// key. This is our first plain text block.
// 3. Iterate over each 16 bytes, decrypt with the key, XOR with the previous
// CipherText and iterate again
// 4. Output final CipherText
int aes_128_cbc_decrypt(const std::string &str, const std::string &key,
                        const std::string &iv, std::string &output) {
  // Pad the string if we need to
  std::string str_cpy = str;
  if (str.size() % 16) {
    pkcs7_padding(str_cpy, 16);
  }

  // Grab first 16 bytes of CipherText and decrypt it.
  std::string decrypt_output;
  int err = aes_128_ecb_decrypt(str_cpy.substr(0, 16), key, decrypt_output);
  if (err) {
    std::cerr << "Failed to decrypt initial 16 bytes";
    return -1;
  }

  // XOR with initialisation vector and add to the output
  std::string plain_text;
  auto hex_str_1 = conversion::convert_string_to_hex_string(decrypt_output);
  auto hex_str_2 = conversion::convert_string_to_hex_string(iv);
  err = fixed_xor(hex_str_1, hex_str_2, plain_text);
  if (err) {
    std::cerr << "Failed to XOR";
    return -1;
  }
  std::string converted_plain_text;
  converted_plain_text.resize((plain_text.size() + 1) / 2);
  conversion::convert_hex_string_to_bytes(
      plain_text.c_str(), (uint8_t *)converted_plain_text.c_str(),
      converted_plain_text.size());
  output.append(converted_plain_text);

  for (size_t i = 16; i < str_cpy.size(); i += 16) {
    std::string pre_ciphertext = str_cpy.substr(i - 16, 16);
    err = aes_128_ecb_decrypt(str_cpy.substr(i, 16), key, decrypt_output);
    if (err) {
      std::cerr << "Failed to decrypt 16 bytes\n";
      return -1;
    }
    auto hex_str_1 = conversion::convert_string_to_hex_string(decrypt_output);
    auto hex_str_2 = conversion::convert_string_to_hex_string(pre_ciphertext);
    err = fixed_xor(hex_str_1, hex_str_2, plain_text);
    if (err) {
      std::cerr << "Failed to fixed_xor\n";
      return -1;
    }
    conversion::convert_hex_string_to_bytes(
        plain_text.c_str(), (uint8_t *)converted_plain_text.c_str(),
        converted_plain_text.size());
    output.append(converted_plain_text);
  }
  return 0;
};

// 1. Pad the string if necessary
// 2. Grab the first 16 bytes, XOR with the IV, enccrypt with the aes_128_ebc.
// This is our first cipher text block.
// 3. Iterate over each 16 bytes, XOR with the previous ciphertext, encrypt with
// the key and iterate again.
// 4. Output final CipherText
int aes_128_cbc_encrypt(const std::string &str, const std::string &key,
                        const std::string &iv, std::string &output) {
  // Pad the string if we need to
  std::string str_cpy = str;
  if (str.size() % 16) {
    pkcs7_padding(str_cpy, 16);
  }

  // XOR with initialisation vector and add to the output
  std::string xor_output;
  auto hex_str_1 =
      conversion::convert_string_to_hex_string(str_cpy.substr(0, 16));
  auto hex_str_2 = conversion::convert_string_to_hex_string(iv);
  int err = fixed_xor(hex_str_1, hex_str_2, xor_output);
  if (err) {
    std::cerr << "Failed to XOR";
    return -1;
  }
  std::string xor_bytes;
  xor_bytes.resize((xor_output.size() + 1) / 2);
  conversion::convert_hex_string_to_bytes(
      xor_output.c_str(), (uint8_t *)xor_bytes.c_str(), xor_bytes.size());

  // Grab first 16 bytes of CipherText and encrypt it.
  std::string encrypt_output;
  err = aes_128_ecb_encrypt(xor_bytes, key, encrypt_output);
  if (err) {
    std::cerr << "Failed to decrypt initial 16 bytes";
    return -1;
  }
  output.append(encrypt_output);

  for (size_t i = 16; i < str_cpy.size(); i += 16) {
    std::string pre_ciphertext = output.substr(i - 16, 16);
    std::string plain_text = str_cpy.substr(i, 16);
    hex_str_1 = conversion::convert_string_to_hex_string(plain_text);
    hex_str_2 = conversion::convert_string_to_hex_string(pre_ciphertext);
    err = fixed_xor(hex_str_1, hex_str_2, xor_output);
    if (err) {
      std::cerr << "Failed to fixed_xor\n";
      return -1;
    }
    conversion::convert_hex_string_to_bytes(
        xor_output.c_str(), (uint8_t *)xor_bytes.c_str(), xor_bytes.size());
    err = aes_128_ecb_encrypt(xor_bytes, key, encrypt_output);
    if (err) {
      std::cerr << "Failed to decrypt 16 bytes\n";
      return -1;
    }
    output.append(encrypt_output);
  }
  return 0;
}

void generate_random_aes_128_key(std::string &key) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist255(
      0, 255); // distribution in range [1, 6]
  key.clear();
  for (int i = 0; i < 16; ++i) {
    key.push_back(dist255(rng));
  }
}

int random_aes_128_encryption(const std::string &str, std::string &output,
                              int &mode) {
  // Generate random key
  std::string key;
  generate_random_aes_128_key(key);

  // If the mode isn't set then choose at random
  if (mode == 0) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist2(
        1, 2); // distribution in range [1, 6]
    mode = dist2(rng);
  }

  // Append 5-10 bytes
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist10(
      5, 10); // distribution in range [1, 6]
  auto pre_bytes = dist10(rng);
  auto post_bytes = dist10(rng);
  std::string str_cpy;
  for (size_t i = 0; i < pre_bytes; ++i) {
    str_cpy.push_back('0');
  }
  str_cpy.append(str);
  for (size_t i = 0; i < post_bytes; ++i) {
    str_cpy.push_back('0');
  }

  if (mode == 1) {
    int err = aes_128_ecb_encrypt(str_cpy, key, output);
    if (err) {
      std::cerr << "Failed to call aes_128_ecb_encrypt\n";
      return -1;
    }
  } else if (mode == 2) {
    std::string iv;
    generate_random_aes_128_key(iv);
    int err = aes_128_cbc_encrypt(str_cpy, key, iv, output);
    if (err) {
      std::cerr << "Failed to call aes_128_cbc_encrypt\n";
      return -1;
    }

  } else {
    std::cerr << "Invalid mode set\n";
    return -1;
  }
  return 0;
}

ECBOracle::ECBOracle() {
  generate_random_aes_128_key(key_);
  std::vector<uint8_t> buffer;
  buffer.resize(unknown_str_.size());
  conversion::convert_base64_string_to_bytes(unknown_str_.c_str(),
                                             buffer.data());
  auto bytes =
      decoding::base64::decode_byte_array(buffer.data(), buffer.size());
  padding_ = std::string(bytes.begin(), bytes.end());
}

int ECBOracle::encrypt(const std::string &data_to_encrypt,
                       std::string &output) {
  std::string str_cpy = data_to_encrypt;
  str_cpy.append(padding_);
  pkcs7_padding(str_cpy, 16);
  int err = aes_128_ecb_encrypt(str_cpy, key_, output);
  if (err) {
    std::cerr << "Failed to encrypt using aes_128_ecb_encrypt\n";
    return -1;
  }
  return 0;
}