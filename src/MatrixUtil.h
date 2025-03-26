#ifndef UTIL_H
#define UTIL_H

#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <span>

/*
    Given a 1D matrix, print to std out.

    [in] mat: matrix to print
    [return] void
*/
inline void print_matrix(std::span<uint8_t> mat)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            std::cout << std::hex << std::setw(2) << std::setfill('0');
            std::cout << static_cast<int>(mat[4 * j + i]) << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}


/*
    Given a 1D square matrix, transpose rows and columns in-place.

    [out] buf: matrix to transpose
    [return] void
*/
constexpr inline void transpose(std::span<uint8_t> buf)
{
    const int n = std::sqrt(buf.size());
    assert(std::floor(n) == n);

    for(int i = 0; i < n; ++i)
    {
        for(int j = i + 1; j < n; ++j)
        {
            std::swap(buf[n * i + j], buf[n * j + i]);
        }
    }
}


/*
    Do multiplication in GF(2^8) between two bytes.

    [in] a, b: Bytes to multiply in GF(2^8)
    [return] uint8_t: Multiplication result
*/
[[nodiscard]] constexpr inline uint8_t gf28_mult(uint8_t a, uint8_t b)
{
    uint8_t c = 0;
    while(a && b)
    {
        if(b & 1)
        {
            // if b is odd, then add to c
            c ^= a;
        }

        if(a & 0x80)
        {
            // XOR with the AES Rijndael primitive polynomial x^8 + x^4 + x^3 + x + 1
            a = (a << 1) ^ 0x11b;
        } 
        else
        {
            // equivalent to a*2
            a <<= 1;
        }
        b >>= 1;
    }
    return c;
}


#endif
