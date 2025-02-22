#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <span>
#include <string>
#include <vector>

#include "FileIO.h"
#include "TestIO.h"

#define INPUT_BIT_COUNT 128
#define KEY_SIZE INPUT_BIT_COUNT / 8

int main()
{
    std::vector<uint8_t> plaintext;
    plaintext.reserve(KEY_SIZE);
    fileBufferASCII("./data/plaintext.txt", plaintext);

    std::vector<uint8_t> subkeys;
    subkeys.reserve(KEY_SIZE * 2);
    fileBufferHex("./data/subkey_example.txt", subkeys);
    
    test_msg_buffer(plaintext);
    test_subkey_buffer(subkeys);

    return 0;
}
