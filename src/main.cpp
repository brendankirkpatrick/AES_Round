#include <algorithm>
#include <cstring>
#include <iterator>
#include <span>
#include <string>
#include <vector>

#include "AES.h"
#include "FileIO.h"

using namespace std;

constexpr int INPUT_BIT_COUNT = 128;
constexpr int KEY_SIZE = INPUT_BIT_COUNT / 8;
constexpr int ROUNDS = 1;
constexpr int KEY_COUNT = ROUNDS + 1;

const char* getCmdOption(span<char*> args, const std::string& option)
{
    const auto it = std::find(args.begin(), args.end(), option);
    if (it != args.end() && std::next(it) != args.end())
    {
        return *std::next(it);
    }
    return nullptr;
}

int main(int argc, char* argv[])
{
    span args(argv, static_cast<size_t>(argc));

    const char* plaintext_file = getCmdOption(args, "-pf");
    const char* encryption_file = getCmdOption(args, "-ef");
    const char* subkey_file = getCmdOption(args, "-sf");

    KeyType key_type;

    std::vector<uint8_t> plaintext;
    plaintext.reserve(KEY_SIZE);
    if(plaintext_file)
    {
        fileBufferASCII(plaintext_file, plaintext);
    }
    else
    {
        fileBufferASCII("./data/plaintext.txt", plaintext);
    }

    std::vector<uint8_t> key;
    if(encryption_file)
    {
        key.reserve(KEY_SIZE);
        fileBufferHex(encryption_file, key);
        key_type = ENCRYPTION_KEY;
    }
    else
    {
        key.reserve(KEY_SIZE * KEY_COUNT);
        key_type = SUBKEY_GIVEN;
        if(subkey_file)
        {
            fileBufferHex(subkey_file, key);
        }
        else
        {
            fileBufferHex("./data/subkey_example.txt", key);
        }
    }

    aes(plaintext, key, key_type);
    
    return 0;
}
