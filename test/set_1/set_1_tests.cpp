#include "conversion.h"
#include "decoding.h"
#include "encoding.h"
#include "set_1.h"
#include "gtest/gtest.h"

#include <fstream>

// Set 1 - Challenge 1
// Convert hex to base64
// The string:

// 49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d
// Should produce:

// SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t
// So go ahead and make that happen. You'll need to use this code for the rest
// of the exercises.

// Method:
// 1. Convert hex string intto a byte array
// 2. Encode the hex byte array into base64 byte array
// 3. Convert base64 byte array into string
// 4. Compare string to challenge answer
// 5. Convert answer string into base64 byte array
// 6. Compare answer string base64 byte array against hex string byte array
TEST(Set1Tests, challenge_1) {
  // Original hex string
  std::string hex_str = {
      "49276d206b696c6c696e6720796f757220627261696e206c696b65206"
      "120706f69736f6e6f7573206d757368726f6f6d"};

  // Answer string
  std::string answer_str = {
      "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t"};

  // 1. Convert hex string into a byte array
  std::vector<uint8_t> buffer{0};
  size_t size = (hex_str.size() + 2 - 1) / 2;
  buffer.resize(size);
  conversion::convert_hex_string_to_bytes(hex_str.data(), buffer.data(), size);

  // 2. Encode the hex byte array into a base64 byte array
  auto base64_byte_array =
      encoding::base64::encode_byte_array(buffer.data(), size);

  // 3. Convert base64 byte array into a string
  auto base64_string = conversion::convert_base64_bytes_to_string(
      base64_byte_array.data(), base64_byte_array.size());

  // 4. Compare string to challenge answer
  ASSERT_EQ(base64_string, answer_str);

  // 5. Convert answer string into base 64 byte array
  std::vector<uint8_t> answer_str_base64_byte_array;
  answer_str_base64_byte_array.resize(answer_str.size());
  conversion::convert_base64_string_to_bytes(
      answer_str.data(), answer_str_base64_byte_array.data());

  // 6. Compare answer string base64 byte array against hex string byte array
  ASSERT_EQ(base64_byte_array, answer_str_base64_byte_array);
}

// Fixed XOR
// Write a function that takes two equal-length buffers and produces their XOR
// combination.

// If your function works properly, then when you feed it the string:

// 1c0111001f010100061a024b53535009181c
// ... after hex decoding, and when XOR'd against:

// 686974207468652062756c6c277320657965
// ... should produce:

// 746865206b696420646f6e277420706c6179
TEST(Set1Tests, challenge_2) {
  std::string str_1 = "1c0111001f010100061a024b53535009181c";
  std::string str_2 = "686974207468652062756c6c277320657965";
  std::string str;
  int err = fixed_xor(str_1, str_2, str);
  ASSERT_FALSE(err);
  ASSERT_EQ(str, "746865206b696420646f6e277420706c6179");
}

// Single-byte XOR cipher
// The hex encoded string:

// 1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736
// ... has been XOR'd against a single character. Find the key, decrypt the
// message.

// You can do this by hand. But don't: write code to do it for you.

// How? Devise some method for "scoring" a piece of English plaintext. Character
// frequency is a good metric. Evaluate each output and choose the one with the
// best score.
TEST(Set1Tests, challenge_3) {
  CipherResult result;
  int err = single_byte_xor_cipher(
      "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736",
      result);
  ASSERT_FALSE(err);
  ASSERT_EQ(result.message, "Cooking MC's like a pound of bacon");
  ASSERT_EQ(result.key, "X");
}

// Detect single-character XOR
// One of the 60-character strings in this file has been encrypted by
// single-character XOR.

// Find it.

// (Your code from #3 should help.)
TEST(Set1Tests, challenge_4) {
  std::string line;
  // Open the file
  std::ifstream challenge_4_file("../files/set_1_challenge_4.txt");
  if (challenge_4_file.fail()) {
    ASSERT_FALSE(true);
  }
  std::vector<CipherResult> cipher_vec;
  // Gather all the best CipherResults from each line
  while (getline(challenge_4_file, line)) {
    CipherResult res;
    int err = single_byte_xor_cipher(line, res);
    ASSERT_FALSE(err);
    cipher_vec.push_back(res);
  }
  // Close the file now we're done with it
  challenge_4_file.close();

  // Now search for the highest character count in the cipher results.
  CipherResult highest_cipher;
  for (const auto &ci : cipher_vec) {
    if (ci.character_count > highest_cipher.character_count) {
      std::cout << ci.message << "\n";
      highest_cipher = ci;
    }
  }
  ASSERT_EQ(highest_cipher.message, "Now that the party is jumping\n");
  ASSERT_EQ(highest_cipher.key, "5");
}

// Implement repeating-key XOR
// Here is the opening stanza of an important work of the English language:

// Burning 'em, if you ain't quick and nimble
// I go crazy when I hear a cymbal

// Encrypt it, under the key "ICE", using repeating-key XOR.

// In repeating-key XOR, you'll sequentially apply each byte of the key; the
// first byte of plaintext will be XOR'd against I, the next C, the next E,
// then I again for the 4th byte, and so on.

// It should come out to:

// 0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272
// a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f
// Encrypt a bunch of stuff using your repeating-key XOR function. Encrypt
// your mail. Encrypt your password file. Your .sig file. Get a feel for it. I
// promise, we aren't wasting your time with this.
TEST(Set1Tests, challenge_5) {
  std::string stanza{"Burning 'em, if you ain't quick and nimble\nI go crazy "
                     "when I hear a cymbal"};

  std::string key{"ICE"};

  std::string encoded_stanza;

  // Convert strings to raw hex strings
  auto hex_line = conversion::convert_string_to_hex_string(stanza);
  auto hex_key = conversion::convert_string_to_hex_string(key);

  int err = repeating_key_xor(hex_line, hex_key, encoded_stanza);
  if (err) {
    ASSERT_FALSE(err);
  }

  ASSERT_EQ("0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324"
            "272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165"
            "286326302e27282f",
            encoded_stanza);
}

// There's a file here. It's been base64'd after being encrypted with
// repeating-key XOR.

// Decrypt it.

// Files are in $ROOT/files/

TEST(Set1Tests, challenge_6) {
  std::string line;
  // Open the file
  std::ifstream challenge_6_file("../files/set_1_challenge_6.txt");
  if (challenge_6_file.fail()) {
    ASSERT_FALSE(true);
  }
  std::string base64_str;
  // Construct the whole base64 string
  while (getline(challenge_6_file, line)) {
    base64_str.append(line);
  }
  // Close the file now we're doing with it
  challenge_6_file.close();

  // Find the key!
  CipherResult res;
  auto err = break_repeating_key_xor(base64_str, res);
  ASSERT_FALSE(err);
  ASSERT_EQ(res.key, "Terminator X: Bring the noise");

  // Let's convert the encrypted message now we've found the key
  std::vector<uint8_t> base64_bytes(base64_str.size());
  conversion::convert_base64_string_to_bytes(base64_str.data(),
                                             base64_bytes.data());
  auto decoded_bytes = decoding::base64::decode_byte_array(base64_bytes.data(),
                                                           base64_bytes.size());
  auto hex_str = conversion::convert_bytes_to_hex_string(decoded_bytes.data(),
                                                         decoded_bytes.size());
  auto key_hex_str = conversion::convert_string_to_hex_string(res.key.data());

  // Use the repeating key xor method to decrypt the message
  std::string str_res;
  err = repeating_key_xor(hex_str, key_hex_str, str_res);
  ASSERT_FALSE(err);

  // Convert the decrypted hex string into bytes
  std::vector<uint8_t> decrypted_bytes;
  decrypted_bytes.resize((str_res.size() + 1) / 2);
  conversion::convert_hex_string_to_bytes(
      str_res.data(), decrypted_bytes.data(), decrypted_bytes.size());

  // Construct the decrypted message
  std::string decrypted_message{decrypted_bytes.begin(), decrypted_bytes.end()};

  // Open up the challenge_6_answer file
  std::ifstream challenge_6_answer("../files/set_1_challenge_6_answer.txt");
  if (challenge_6_answer.fail()) {
    ASSERT_FALSE(true);
  }
  std::string answer_str;
  // Construct the answer string
  while (getline(challenge_6_answer, line)) {
    answer_str.append(line);
    answer_str.append("\n");
  }
  // Close the file now we're finished
  challenge_6_answer.close();

  // Assert if the answer and decrypted message don't match
  ASSERT_EQ(decrypted_message, answer_str);
}

TEST(Set1Tests, challenge_7) {
  std::string result;
  std::string line;
  // Open the file
  std::ifstream challenge_7_file("../files/set_1_challenge_7.txt");
  if (challenge_7_file.fail()) {
    ASSERT_FALSE(true);
  }
  std::string base64_str;
  // Construct the whole base64 string
  while (getline(challenge_7_file, line)) {
    base64_str.append(line);
  }
  // Close the file now we're doing with it
  challenge_7_file.close();

  // Decode the input string
  std::vector<uint8_t> output_buffer;
  output_buffer.resize(base64_str.size());
  conversion::convert_base64_string_to_bytes(base64_str.c_str(),
                                             output_buffer.data());
  auto decoded_bytes = decoding::base64::decode_byte_array(
      output_buffer.data(), output_buffer.size());

  // Decrypte the input string with the key "YELLOW SUBMARINE"
  aes_128_ecb_decrypt(std::string{decoded_bytes.begin(), decoded_bytes.end()},
                      "YELLOW SUBMARINE", result);

  // Open up the challenge_7_answer file
  std::ifstream challenge_7_answer("../files/set_1_challenge_7_answer.txt");
  if (challenge_7_answer.fail()) {
    ASSERT_FALSE(true);
  }

  std::string answer_str;
  // Construct the answer string
  while (getline(challenge_7_answer, line)) {
    answer_str.append(line);
    answer_str.append("\n");
  }

  // Remove final newline
  answer_str.pop_back();

  // Close the file now we're finished
  challenge_7_answer.close();

  // Assert if the answer and decrypted message don't match
  ASSERT_EQ(result, answer_str);
}

// Detect AES in ECB mode
// In this file are a bunch of hex-encoded ciphertexts.

// One of them has been encrypted with ECB.

// Detect it.

// Remember that the problem with ECB is that it is stateless and deterministic;
// the same 16 byte plaintext block will always produce the same 16 byte//
// ciphertext.
TEST(Set1Tests, challenge_8) {
  std::string line;
  // Open the file
  std::ifstream challenge_8_file("../files/set_1_challenge_8.txt");
  if (challenge_8_file.fail()) {
    ASSERT_FALSE(true);
  }
  std::string ecb_str;
  // Iterate over each line and find the one that is ECB encrypted
  while (getline(challenge_8_file, line)) {
    int res{0};
    is_ecb_encryption(std::vector<uint8_t>(line.begin(), line.end()), 16, res);
    if (res) {
      ecb_str = line;
    }
  }
  // Close the file now we're done with it
  challenge_8_file.close();
  ASSERT_EQ(ecb_str,
            "d880619740a8a19b7840a8a31c810a3d08649af70dc06f4fd5d2d69c744cd283e2"
            "dd052f6b641dbf9d11b0348542bb5708649af70dc06f4fd5d2d69c744cd2839475"
            "c9dfdbc1d46597949d9c7e82bf5a08649af70dc06f4fd5d2d69c744cd28397a93e"
            "ab8d6aecd566489154789a6b0308649af70dc06f4fd5d2d69c744cd283d403180c"
            "98c8f6db1f2a3f9c4040deb0ab51b29933f2c123c58386b06fba186a");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
