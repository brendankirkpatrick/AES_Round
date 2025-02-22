#ifndef AES_H
#define AES_H

#include <cstdint>
#include <span>

enum KeyType
{
    ENCRYPTION_KEY,
    SUBKEY_GIVEN
};

/*
    Begins round one of AES (including initial AddKey)
    Round 1 operations:
        - S-box (substitute each byte in State)
        - Shift Rows
        - Mix Columns
        - AddKey
    Note: Doesn't include second subkey from subkey_example.txt within the buffer
          since subkey needs to be generated regardless from other steps
    
    [out] message: encrypts message from within the inputted buffer
    [out] key: uses encryption key to generage subkeys
    [in] key_type: denotes whether input key is 128 bit encryption key or buffer containing
               full list of subkeys to be used when generating algorithm
    [return] int: possible error code

*/
int aes(std::span<uint8_t> message, std::span<uint8_t> key, const KeyType key_type);


/*
    Generate next subkey from current subkey using Key Schedule algorithm

    [out] subkey: subkey  
    [return] int: possible error code
*/
int generate_subkey(std::span<uint8_t> subkey);

#endif
