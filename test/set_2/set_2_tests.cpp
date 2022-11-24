#include "encoding.h"
#include "set_1.h"
#include "set_2.h"
#include "utilities.h"
#include "gtest/gtest.h"
#include <conversion.h>
#include <decoding.h>
#include <fstream>
#include <unordered_map>

// A block cipher transforms a fixed-sized block (usually 8 or 16 bytes) of
// plaintext into ciphertext. But we almost never want to transform a single
// block; we encrypt irregularly-sized messages.

// One way we account for irregularly-sized messages is by padding, creating a
// plaintext that is an even multiple of the blocksize. The most popular padding
// scheme is called PKCS#7.

// So: pad any block to a specific block length, by appending the number of
// bytes of padding to the end of the block. For instance,

// "YELLOW SUBMARINE"
// ... padded to 20 bytes would be:

// "YELLOW SUBMARINE\x04\x04\x04\x04"
TEST(Set2Tests, challenge_9) {
  std::string str{"YELLOW SUBMARINE"};
  pkcs7_padding(str, 20);
  ASSERT_EQ(str, "YELLOW SUBMARINE\x04\x04\x04\x04");
}

// Implement CBC mode
// CBC mode is a block cipher mode that allows us to encrypt irregularly-sized
// messages, despite the fact that a block cipher natively only transforms
// individual blocks.

// In CBC mode, each ciphertext block is added to the next plaintext block
// before the next call to the cipher core.

// The first plaintext block, which has no associated previous ciphertext block,
// is added to a "fake 0th ciphertext block" called the initialization vector,
// or IV.

// Implement CBC mode by hand by taking the ECB function you wrote earlier,
// making it encrypt instead of decrypt (verify this by decrypting whatever you
// encrypt to test), and using your XOR function from the previous exercise to
// combine them.

// The file here is intelligible (somewhat) when CBC decrypted against "YELLOW
// SUBMARINE" with an IV of all ASCII 0 (\x00\x00\x00 &c)
TEST(Set2Tests, challenge_10) {
  std::string line;
  // Open the file
  std::ifstream challenge_2_file(
      "/home/ANT.AMAZON.COM/dwillans/devel/CryptopalsChallenges/files/"
      "set_2_challenge_10.txt");
  if (challenge_2_file.fail()) {
    ASSERT_FALSE(true);
  }
  std::string base64_str;
  // Construct the whole base64 string
  while (getline(challenge_2_file, line)) {
    base64_str.append(line);
  }
  // Close the file now we're done with it
  challenge_2_file.close();

  // Decode the input string
  std::vector<uint8_t> output_buffer;
  output_buffer.resize(base64_str.size());
  conversion::convert_base64_string_to_bytes(base64_str.c_str(),
                                             output_buffer.data());
  auto decoded_bytes = decoding::base64::decode_byte_array(
      output_buffer.data(), output_buffer.size());

  std::string result;
  std::string iv;
  // String can't be constructed with null terminating characters so we need to
  // push_back into an already constructed string
  for (int i = 0; i < 16; ++i) {
    iv.push_back('\x00');
  }
  // Now decrypt the string using aes_128_cbc
  int err = aes_128_cbc_decrypt(
      std::string{decoded_bytes.begin(), decoded_bytes.end()},
      "YELLOW SUBMARINE", iv, result);
  ASSERT_FALSE(err);

  // Open up the set_1_challenge_7_answer file.
  std::ifstream challenge_8_answer("../files/set_1_challenge_7_answer.txt");
  if (challenge_8_answer.fail()) {
    ASSERT_FALSE(true);
  }

  std::string answer_str;
  // Construct the answer string
  while (getline(challenge_8_answer, line)) {
    answer_str.append(line);
    answer_str.append("\n");
  }

  // Remove final newline
  answer_str.pop_back();

  // Close the file now we're finished
  challenge_8_answer.close();
  ASSERT_EQ(answer_str, result);

  // This isn't part of the challenge but whilst we're here let's just check our
  // aes_128_cbc encryption works as well. Re-encrypt the decrypted message and
  // compare to the original base64 text
  std::string encrypted_output;
  err = aes_128_cbc_encrypt(result, "YELLOW SUBMARINE", iv, encrypted_output);

  // Convert to base64 and compare agains the challenge_2_file
  auto encrypted_bytes = encoding::base64::encode_byte_array(
      (uint8_t *)encrypted_output.data(), encrypted_output.size());

  auto base64_answer = conversion::convert_base64_bytes_to_string(
      encrypted_bytes.data(), encrypted_bytes.size());

  ASSERT_EQ(base64_answer, base64_str);
}

TEST(Set2Tests, challenge_11) {
  std::string random_key_1;
  std::string random_key_2;
  generate_random_aes_128_key(random_key_1);
  generate_random_aes_128_key(random_key_2);
  ASSERT_EQ(random_key_1.size(), 16);
  ASSERT_EQ(random_key_2.size(), 16);
  // There is a chance this check could fail but very unlikely
  ASSERT_NE(random_key_1, random_key_2);
  // Create initial string
  std::string str{"ABCDEFGHIJKLMNOP"};
  str.append(str);
  // String will now be 64 bytes
  str.append(str);

  // Now encrypt the str with a random key and ECB
  std::string output;
  int mode = 1;
  random_aes_128_encryption(str, output, mode);

  // Check if it is encrypted with ecb or not.
  int is_ecb;
  int err = is_ecb_encryption(
      std::vector<uint8_t>(output.begin(), output.end()), 16, is_ecb);
  ASSERT_FALSE(err);
  ASSERT_TRUE(is_ecb);

  // Now encrypt the str with a random key and CBC
  output.clear();
  mode = 2;
  random_aes_128_encryption(str, output, mode);

  // Check if it is encrypted with ecb or not
  err = is_ecb_encryption(std::vector<uint8_t>(output.begin(), output.end()),
                          16, is_ecb);
  ASSERT_FALSE(err);
  ASSERT_FALSE(is_ecb);

  // Randomise the encryption
  output.clear();
  mode = 0;
  random_aes_128_encryption(str, output, mode);

  // Check if it is encrypted with ecb or not
  err = is_ecb_encryption(std::vector<uint8_t>(output.begin(), output.end()),
                          16, is_ecb);
  ASSERT_FALSE(err);

  // If the mode was set to ECB in the random encryption function then is_ecb
  // output from is_ecb_encryption will be true.
  ASSERT_EQ(mode == 1 ? 1 : 0, is_ecb);
}

TEST(Set2Tests, challenge_12) {
  ECBOracle oracle;

  // Step 1.
  // Let's discover the blocksize by encrypting with the same byte.
  // Once the ciphertext stops changing we know that the first block is
  // filled and consequently the block size
  int block_size{0};
  std::string old_ans;
  int err = oracle.encrypt("A", old_ans);
  ASSERT_FALSE(err);
  for (int i = 2; i < 20; ++i) {
    std::string temp_str(i, 'A');
    std::string ans;
    err = oracle.encrypt(temp_str, ans);
    ASSERT_FALSE(err);
    // Assume that the block size is at least greater than 4 long
    if (ans.substr(0, 4) == old_ans.substr(0, 4)) {
      block_size = i - 1;
      break;
    }
    old_ans = ans;
  }
  // Ok we discovered the block_size is 16.
  ASSERT_EQ(block_size, 16);

  // Step 2.
  // Let's check and confirm we're using ECB
  int result{0};
  err = oracle.encrypt(std::string(64, 'A'), old_ans);
  err = is_ecb_encryption(std::vector<uint8_t>(old_ans.begin(), old_ans.end()),
                          block_size, result);
  ASSERT_TRUE(result);
  ASSERT_FALSE(err);

  // Step 3/4/5/6
  // Now we need to try and decode the secret padding the oracle appends onto
  // our str. We will do this by using a blocksize that is one byte short. The
  // oracle will append the first byte of the unknown string onto the end of the
  // first block. If we encrypt this we can get the first byte of the unknown
  // string encrypted.
  // We can discover what character the encrypted byte represents by making a
  // dictionary of all possible characters and comparing the outputs. We can
  // then make up the final string by manipulating the input string.

  // Get the length of the extra padding by encrypting an empty message
  std::string encrypt_length;
  oracle.encrypt("", encrypt_length);

  // Now lets iterate for the length of the padding and compare to the map to
  // construct the unknown string
  std::string final_ans;
  for (size_t block = 1; block < encrypt_length.size() / block_size + 1;
       ++block) {
    std::string block_str;
    for (int i = 0; i < block_size; ++i) {
      // Create test str reducing in size based on the position in the block
      // size
      std::string test_str(block_size - i - 1, 'A');
      std::string test_str_output;
      int err = oracle.encrypt(test_str, test_str_output);
      ASSERT_FALSE(err);
      std::unordered_map<std::string, char> byte_map;
      // Now iterate over each possible char and compare against the original
      // encryption to see if we have a match
      for (int j = 0; j < 256; ++j) {
        std::string map_str(block_size - i - 1, 'A');
        map_str.append(final_ans);
        map_str.append(block_str);
        map_str.push_back(j);
        std::string output;
        int err = oracle.encrypt(map_str, output);
        ASSERT_FALSE(err);
        byte_map.insert(
            {output.substr(0, block * block_size), static_cast<char>(j)});
        if (byte_map.find(test_str_output.substr(0, block * block_size)) !=
            byte_map.end()) {
          block_str.push_back(
              byte_map[test_str_output.substr(0, block * block_size)]);
          break;
        }
      }
    }
    final_ans.append(block_str);
  }

  // Unpad the final string
  pkcs7_unpad(final_ans, block_size);

  // Open up the set_2_challenge_12_answer file.
  std::ifstream challenge_12_answer("../files/set_2_challenge_12_answer.txt");
  if (challenge_12_answer.fail()) {
    ASSERT_FALSE(true);
  }

  std::string answer_str;
  std::string line;
  // Construct the answer string
  while (getline(challenge_12_answer, line)) {
    answer_str.append(line);
    answer_str.append("\n");
  }

  // Close the file now we're finished
  challenge_12_answer.close();
  ASSERT_EQ(final_ans, answer_str);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
