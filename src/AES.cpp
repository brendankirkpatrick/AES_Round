#include <algorithm>
#include <cassert>
#include <iostream>
#include <span>

#include "AES.h"
#include "FileIO.h"
#include "MatrixUtil.h"

#include "tables.inl"

using namespace std;

constexpr size_t KEY_SIZE = 16;

int aes(std::span<uint8_t> message, std::span<uint8_t> key, const KeyType key_type)
{
    if(key_type == KeyType::ENCRYPTION_KEY)
    {
        add_key(message, key);
    } else if(key_type == KeyType::SUBKEY_GIVEN)
    {
        span<uint8_t> first_key = key.subspan(KEY_SIZE * 0, KEY_SIZE);
        add_key(message, first_key);
    } else
    {
        cerr << "Inelligible key_type. Should be ENCRYPTION_KEY or SUBKEY_GIVEN.\n";
        return 1;
    }

    // Loop for additional rounds
    {
        const size_t round = 1;

        sub_bytes(message);
        shift_rows(message);
        mix_columns(message);
        if(key_type == KeyType::ENCRYPTION_KEY)
        {
            add_key(message, key);
            generate_subkey(key, round);
            writeFileBuffer("./data/result_subkey.txt", key, "Generated Subkey: "); 
        } else if(key_type == KeyType::SUBKEY_GIVEN)
        {
            if(key.size() < KEY_SIZE * round + KEY_SIZE)
            {
                cerr << "Error. Less Subkeys than rounds. Cannot complete AddKey." << '\n';
                return 2;
            }
            span<uint8_t> next_key = key.subspan(KEY_SIZE * round, KEY_SIZE);
            add_key(message, next_key);
        }
    }

    return 0;
}


/*
    Add Key XOR's each respective byte of message with subkey
*/
int add_key(span<uint8_t> message, span<uint8_t> subkey)
{
    assert(message.size() == 16);
    assert(subkey.size() == 16);
    assert(message.size() == subkey.size());

    cout << "|AddKey|\n------------\n";

    auto msg_it = message.begin();
    auto key_it = subkey.begin();

    while(msg_it != message.end() && key_it != subkey.end())
    {
        *msg_it ^= *key_it;
        ++msg_it;
        ++key_it;
    }

    print_matrix(message);
    return 0;
}


/*
    Sub Bytes performs SBox conversion to each byte of message
*/
int sub_bytes(std::span<uint8_t> message)
{
    cout << "|SBox|\n------------\n";

    auto msg_it = message.begin();
    while(msg_it != message.end())
    {
        *msg_it = SBox(*msg_it);
        ++msg_it;
    }

    print_matrix(message);
    return 0;
}


/*
    Shift Rows Steps:
        - Transpose matrix to perform row-wise rotation on column vectors
        - Rotate rows
        - Transpose matrix to undo previous transpose
*/
int shift_rows(std::span<uint8_t> message)
{
    assert(message.size() == 16);
    cout << "|Shift Rows|\n------------\n";

    transpose(message);
    std::rotate(message.begin() + 4, message.begin() + 5, message.begin() + 8);
    std::rotate(message.begin() + 8, message.begin() + 10, message.begin() + 12);
    std::rotate(message.begin() + 12, message.begin() + 15, message.begin() + 16);
    transpose(message);

    print_matrix(message);

    return 0;
}


/*
    Mix Columns requires operations on bytes in GF(2^8)

    Steps:
        - Create temporary buffer to copy message to 
        - Perform matrix multiplication with MixColMatrix in GF(2^8)
            - Additionis performed using the XOR operation
*/
int mix_columns(std::span<uint8_t> message) 
{
    assert(message.size() == 16);
    cout << "|Mix Columns|\n------------\n";

    uint8_t msg_buf[message.size()];

    for(int col = 0; col < 4; col++)
    {
        for(int row = 0; row < 4; row++)
        {
            // Treat XOR operation as matrix addition in GF(2^8) 
            msg_buf[4 * col + row] = 
                gf28_mult(MixColMatrix(4 * row + 0), message[4 * col + 0]) ^ 
                gf28_mult(MixColMatrix(4 * row + 1), message[4 * col + 1]) ^ 
                gf28_mult(MixColMatrix(4 * row + 2), message[4 * col + 2]) ^
                gf28_mult(MixColMatrix(4 * row + 3), message[4 * col + 3]);
        }
    }

    for(int i = 0; i < message.size(); i++)
    {
        message[i] = msg_buf[i];
    }

    print_matrix(message);
    return 0;
}


/*
    Generate Subkey involves calculating columns of subkey matrix
    for columns w0 - w3

    w4 = w0 XOR g(w3)
    w5 = w4 XOR w1
    w6 = w5 XOR w2
    w7 = w6 XOR w3

    g(w) Steps: 
        - Perform 1 byte rotation left
        - Perform SBox on each byte
        - XOR bytes with round constant

*/
int generate_subkey(std::span<uint8_t> subkey, const size_t round) 
{ 
    assert(subkey.size() == 16);

    span<uint8_t> w0 = subkey.subspan(0, 4);
    span<uint8_t> w1 = subkey.subspan(4, 4);
    span<uint8_t> w2 = subkey.subspan(8, 4);
    span<uint8_t> w3 = subkey.subspan(12, 4);

    uint8_t g_w[4]; 
    std::copy(w3.begin(), w3.end(), g_w);

    // Steps for g(w)
    std::rotate(g_w, g_w + 1, g_w + 4);
    std::transform(g_w, g_w + 4, g_w, &SBox);
    g_w[0] ^= RoundConstant(round);

    // XOR a and b, modifying and returning a
    auto XOR = [](span<uint8_t> a, span<uint8_t> b) -> span<uint8_t>
    {
        auto a_it = a.begin();
        auto b_it = b.begin();
        while(a_it != a.end() && b_it != b.end())
        {
            *a_it ^= *b_it;
            ++a_it;
            ++b_it;
        }
        return a;
    };

    span<uint8_t> w4 = XOR(w0, g_w);
    span<uint8_t> w5 = XOR(w1, w4);
    span<uint8_t> w6 = XOR(w2, w5); 
    XOR(w3, w6);

    return 0;
}
