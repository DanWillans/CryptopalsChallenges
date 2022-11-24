#include <set_1.h>

static const std::string set_2_challenge_12_random_key{"XAEGDHFIE123HAKD"};

int aes_128_cbc_decrypt(const std::string &str, const std::string &key,
                        const std::string &iv, std::string &output);

int aes_128_cbc_encrypt(const std::string &str, const std::string &key,
                        const std::string &iv, std::string &output);

void generate_random_aes_128_key(std::string &key);

// Mode 0 == Random - Mode 1 == ECB - Mode 2 == CBC
int random_aes_128_encryption(const std::string &str, std::string &output,
                              int &mode);

int ecb_encryption_consistent_key(const std::string &str, std::string &output);

class ECBOracle {
public:
  ECBOracle();
  int encrypt(const std::string &data_to_encrypt, std::string &output);

private:
  std::string key_;
  std::string padding_;
  std::string unknown_str_{
      "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4g"
      "YmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQg"
      "eW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK"};
};