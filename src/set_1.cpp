#include "set_1.h"
#include "decoding.h"
#include "openssl/evp.h"
#include "utilities.h"
#include <cfloat>
#include <conversion.h>
#include <deque>
#include <encoding.h>
#include <ios>
#include <iostream>
#include <openssl/crypto.h>
#include <set>
#include <sstream>
#include <string>

namespace {
// For every valid char increment the count
int valid_character_count(std::string str) {
  int count = 0;
  for (size_t i = 0; i < str.size(); ++i) {
    char c = str[i];
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ') {
      count++;
    } else {
      count--;
    }
  }
  return count;
}

int get_keysize_score(const std::string &str, int keysize, double &score) {
  // length represents how many keys of length keysize can fit into the str
  // twice. keysize*2 because keysize is the count of characters and each
  // characters equals 2 hex chars in the string
  int hex_length = keysize * 2;
  // If we can't fit two hex_lengths into str then we can't sample this keysize
  if (static_cast<size_t>(hex_length) > str.size() / 2) {
    return -2;
  }
  // The amount of samples we can use are the length of the str divided by 2
  // amounts of hex_length bytes
  size_t samples = str.size() / (2 * hex_length);
  // We need an even amount of samples because we need to compare 2 of each.
  // If it's uneven then reduce by 1.
  if (samples % 2) {
    samples--;
  }
  score = 0;
  // Iterate over the samples.
  // Create correct size substrs, get the edit distance and add to the score.
  for (size_t i = 0; i < samples; ++i) {
    auto str_1 = str.substr(i * hex_length, hex_length);
    auto str_2 = str.substr((i * hex_length) + hex_length, hex_length);
    int edit_distance{0};
    int err = edit_distance_hex_string(str_1, str_2, edit_distance);
    if (err) {
      std::cerr << "Failed to get edit distance\n";
      return -1;
    }
    score += edit_distance;
  }
  // Average the score over the amount of samples
  score /= samples;
  // Normalise by dividing by the keysize.
  score /= keysize;
  return 0;
}

} // namespace

int calculate_keysize(uint start, uint end, const std::string &str,
                      int &keysize) {
  if ((end - start) > (str.size() / 2)) {
    std::cerr << "Range is too big for size of str data\n";
    return -1;
  }
  double min_score{DBL_MAX};
  for (size_t i = start; i < end; ++i) {
    double score;
    int err = get_keysize_score(str, i, score);
    if (err == -1) {
      std::cerr << "Failed to get keysize score\n";
      return -1;
    }
    if (score < min_score) {
      std::cout << "Minimum score: " << score << " - keysize: " << i << "\n";
      min_score = score;
      keysize = i;
    }
  }
  return 0;
}

int fixed_xor(const std::string &hex_str_1, const std::string &hex_str_2,
              std::string &output) {
  // Both strings need to be the same size
  if (hex_str_1.size() != hex_str_2.size()) {
    std::cerr << "Both strings need to be the same size\n";
    return -1;
  }

  // Create output buffers
  size_t size = (hex_str_1.size() + 2 - 1) / 2;
  std::vector<uint8_t> buffer_1;
  std::vector<uint8_t> buffer_2;
  buffer_1.resize(size);
  buffer_2.resize(size);

  // Converts hex strings to bytes
  conversion::convert_hex_string_to_bytes(hex_str_1.data(), buffer_1.data(),
                                          size);

  conversion::convert_hex_string_to_bytes(hex_str_2.data(), buffer_2.data(),
                                          size);

  // XOR the two buffers
  std::vector<uint8_t> output_buffer;
  output_buffer.resize(size);
  for (size_t i = 0; i < buffer_1.size(); ++i) {
    output_buffer[i] = buffer_1[i] ^ buffer_2[i];
  }

  // Convert output_buffer to hex string and return
  output = conversion::convert_bytes_to_hex_string(output_buffer.data(),
                                                   output_buffer.size());
  return 0;
}

int repeating_key_xor(const std::string &hex_str, const std::string &hex_key,
                      std::string &output) {
  // Create output buffers
  size_t hex_str_size = (hex_str.size() + 2 - 1) / 2;
  size_t hex_key_size = (hex_key.size() + 2 - 1) / 2;
  std::vector<uint8_t> buffer_1;
  std::vector<uint8_t> buffer_2;
  buffer_1.resize(hex_str_size);
  buffer_2.resize(hex_key_size);

  // Converts hex strings to bytes
  conversion::convert_hex_string_to_bytes(hex_str.data(), buffer_1.data(),
                                          hex_str_size);

  conversion::convert_hex_string_to_bytes(hex_key.data(), buffer_2.data(),
                                          hex_key_size);

  // XOR the two buffers repeating the hex key.
  std::vector<uint8_t> output_buffer;
  output_buffer.resize(hex_str_size);
  for (size_t i = 0; i < buffer_1.size(); ++i) {
    output_buffer[i] = buffer_1[i] ^ buffer_2[i % buffer_2.size()];
  }

  // Convert output_buffer to hex string and return
  output = conversion::convert_bytes_to_hex_string(output_buffer.data(),
                                                   output_buffer.size());
  return 0;
}

int single_byte_xor_cipher(const std::string &hex_encoded_str,
                           CipherResult &output) {
  // Iterate over all 256 characters and XOR them.
  // Then use a function to analyse if the probability of the XOR'd string is
  // text. If it is high in score it's more than likely our key.
  for (int i = 0; i < 255; ++i) {

    // There is probably a better way to do this hex creation from an integer
    std::stringstream stream;
    stream << std::hex << i;
    std::string hex_str(stream.str());
    if (hex_str.size() == 1) {
      hex_str.insert(hex_str.begin(), '0');
    }

    // Make hex_string the same size as single_byte_xor_cipher
    for (size_t i = 2; i < hex_encoded_str.size(); i += 2) {
      if (i == hex_encoded_str.size() - 1) {
        hex_str.push_back(hex_str[0]);
        break;
      }
      hex_str.push_back(hex_str[0]);
      hex_str.push_back(hex_str[1]);
    }

    // Calculate the fixed xor for this key
    std::string res;
    int err = fixed_xor(hex_encoded_str, hex_str, res);
    if (err) {
      std::cout << "Failed to fixed_xor" << std::endl;
      return -1;
    }

    // Convert the output back into bytes
    std::vector<uint8_t> buffer;
    buffer.resize(res.size() / 2);
    conversion::convert_hex_string_to_bytes(res.data(), buffer.data(),
                                            buffer.size());

    // Create string from raw bytes and check valid characters
    std::string str_res{(const char *)buffer.data(), buffer.size()};
    int count = valid_character_count(str_res);

    // Update output if we've found a higher count
    if (count > output.character_count) {
      output.character_count = count;
      output.message = str_res;
      output.key = static_cast<char>(i);
    }
  }
  return 0;
}

int break_repeating_key_xor(const std::string &base64_string,
                            CipherResult &return_cipher) {
  // Decode base64_string
  std::vector<uint8_t> buffer;
  buffer.resize(base64_string.size());
  conversion::convert_base64_string_to_bytes(base64_string.data(),
                                             buffer.data());
  auto byte_array =
      decoding::base64::decode_byte_array(buffer.data(), buffer.size());

  // Calculate back into hex string
  auto str = conversion::convert_bytes_to_hex_string(byte_array.data(),
                                                     byte_array.size());

  int keysize{0};
  // Calculate the size of the key
  int err = calculate_keysize(2, 40, str, keysize);
  if (err) {
    std::cerr << "Failed to get keysize\n";
    return -1;
  }

  // Now we know the size we can break the cipher text bytes into chunks of that
  // key size.
  std::vector<std::vector<uint8_t>> base64_keysize_byte_chunks;
  // Decode base64 str in bytes
  std::vector<uint8_t> base64_bytes(base64_string.size());
  conversion::convert_base64_string_to_bytes(base64_string.data(),
                                             base64_bytes.data());
  auto decoded_bytes = decoding::base64::decode_byte_array(base64_bytes.data(),
                                                           base64_bytes.size());
  for (size_t i = 0; i < decoded_bytes.size(); i += keysize) {
    // Check that we're not going over the end of the string
    if (static_cast<size_t>(i) < decoded_bytes.size() - keysize) {
      base64_keysize_byte_chunks.push_back(std::vector<uint8_t>(
          decoded_bytes.begin() + i, decoded_bytes.begin() + i + keysize));
    } else {
      base64_keysize_byte_chunks.push_back(
          std::vector<uint8_t>(decoded_bytes.begin() + i, decoded_bytes.end()));
    }
  }

  // Now transpose the chunks.
  // Take every nth byte from each chunk and put into a string
  std::vector<std::vector<uint8_t>> transpose_chunks;
  transpose_chunks.resize(keysize);
  for (int byte_pos = 0; byte_pos < keysize; ++byte_pos) {
    for (size_t i = 0; i < base64_keysize_byte_chunks.size(); ++i) {
      transpose_chunks[byte_pos].push_back(
          base64_keysize_byte_chunks[i][byte_pos]);
    }
  }

  // Now for every string calculate the single-byte key
  for (size_t i = 0; i < transpose_chunks.size(); ++i) {
    CipherResult result;
    auto hex_str = conversion::convert_bytes_to_hex_string(
        transpose_chunks[i].data(), transpose_chunks[i].size());
    int err = single_byte_xor_cipher(hex_str, result);
    if (err) {
      std::cerr << "Failed to find single_byte_xor_cipher\n";
      return -1;
    }
    return_cipher.key.append(result.key);
  }

  return 0;
}

int aes_128_ecb_decrypt(const std::string &str, const std::string &key,
                        std::string &output) {
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    return -1;
  }
  // Don't set the key yet to check the key and iv lengths
  int err = EVP_CipherInit(ctx, EVP_aes_128_ecb(), nullptr, nullptr, 0);
  if (!err) {
    std::cerr << "Failed to call CipherInit without key\n";
    return -1;
  }
  // aes_128_ecb has a key length of 16 bytes
  OPENSSL_assert(EVP_CIPHER_CTX_key_length(ctx) == 16);
  // iv length == 0 because of ECB I think?
  OPENSSL_assert(EVP_CIPHER_CTX_iv_length(ctx) == 0);
  // Set the key now
  err = EVP_CipherInit(ctx, nullptr,
                       reinterpret_cast<const unsigned char *>(key.c_str()),
                       nullptr, 0);
  if (!err) {
    std::cerr << "Failed to call CipherInit\n";
    return -1;
  }
  // Set padding to true so that we get the partially filled data blocks
  err = EVP_CIPHER_CTX_set_padding(ctx, 0);
  if (!err) {
    std::cerr << "Failed to set padding to ctx\n";
    return -1;
  }
  // Now decrypt the str
  int output_len{0};
  std::vector<unsigned char> output_buffer;
  output_buffer.resize(str.size());
  err = EVP_CipherUpdate(ctx, output_buffer.data(), &output_len,
                         reinterpret_cast<const unsigned char *>(str.c_str()),
                         str.size());
  if (!err) {
    std::cerr << "Failed to call CipherUpdate\n";
    return -1;
  }
  output =
      std::string{output_buffer.begin(), output_buffer.begin() + output_len};
  return 0;
}

int aes_128_ecb_encrypt(const std::string &str, const std::string &key,
                        std::string &output) {
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) {
    return -1;
  }
  // Don't set the key yet to check the key and iv lengths
  int err = EVP_CipherInit(ctx, EVP_aes_128_ecb(), nullptr, nullptr, 1);
  if (!err) {
    std::cerr << "Failed to call CipherInit without key\n";
    return -1;
  }
  // aes_128_ecb has a key length of 16 bytes
  OPENSSL_assert(EVP_CIPHER_CTX_key_length(ctx) == 16);
  // iv length == 0 because of ECB I think?
  OPENSSL_assert(EVP_CIPHER_CTX_iv_length(ctx) == 0);
  // Set the key now
  err = EVP_CipherInit(ctx, nullptr,
                       reinterpret_cast<const unsigned char *>(key.c_str()),
                       nullptr, 1);
  if (!err) {
    std::cerr << "Failed to call CipherInit\n";
    return -1;
  }
  // Set padding to true so that we get the partially filled data blocks
  err = EVP_CIPHER_CTX_set_padding(ctx, 0);
  if (!err) {
    std::cerr << "Failed to set padding to ctx\n";
    return -1;
  }
  // Now decrypt the str
  int output_len{0};
  std::vector<unsigned char> output_buffer;
  output_buffer.resize(str.size());
  err = EVP_CipherUpdate(ctx, output_buffer.data(), &output_len,
                         reinterpret_cast<const unsigned char *>(str.c_str()),
                         str.size());
  if (!err) {
    std::cerr << "Failed to call CipherUpdate\n";
    return -1;
  }
  output =
      std::string{output_buffer.begin(), output_buffer.begin() + output_len};
  return 0;
}

int is_ecb_encryption(const std::vector<uint8_t> &bytes, int blocksize,
                      int &res) {
  if (bytes.size() % blocksize) {
    std::cerr << "Blocksize doesn't fit into the amount of bytes correctly\n";
    return -1;
  }
  std::set<std::vector<uint8_t>> set;
  // Create vectors for each bocksize and put into set
  for (size_t i = 0; i < bytes.size(); i += blocksize) {
    std::vector<uint8_t> vec{bytes.begin() + i, bytes.begin() + i + blocksize};
    set.insert(vec);
  }
  // If each block isn't unique then we can be pretty certain that it's ECB
  // encryption
  if (set.size() != (bytes.size() / blocksize)) {
    res = 1;
  } else {
    res = 0;
  }
  return 0;
}