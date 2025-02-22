#include <span>

#include "AES.h"

using namespace std;

int aes(std::span<uint8_t> message, std::span<uint8_t> key, const KeyType key_type)
{
    /*
    if key is encryption, whenever doing add_key() we must first generate a new subkey
    otherwise we use the subkey given in key list
    */
    // First AddKey (not to be repeated)
    // add_key(key);

    // Complete first round
    // aes_round();

    return 0;
}

int generate_subkey(std::span<uint8_t> subkey)
{
    // generate subkey from previous set of subkeys for next round
    return 0;
}
