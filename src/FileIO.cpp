#include <fstream>
#include <iostream>
#include <vector>
#include <string_view>

#include "FileIO.h"

using namespace std;

constexpr int CHAR_SIZE = sizeof(char);

int fileBufferASCII(const string_view file, vector<uint8_t>& byte_buffer)
{
    ifstream fin(file.data(), ios_base::in | ios_base::binary);
    if(!fin.is_open())
    {
        cerr << "Could not open input file: " << file << '\n';
        return 1;
    }

    uint8_t byte;
    while(fin.read(reinterpret_cast<char*>(&byte), CHAR_SIZE).good())
    {
        byte_buffer.push_back(byte);
    }
    // remove final endline character
    byte_buffer.pop_back();

    return 0;
}

int fileBufferHex(const string_view file, vector<uint8_t>& byte_buffer)
{
    ifstream fin(file.data(), ios_base::in | ios_base::binary);
    if(!fin.is_open())
    {
        cerr << "Could not open input file: " << file << '\n';
        return 1;
    }

    // For hex conversion string must be null terminated
    char byte[3];
    byte[2] = '\0';
    char* end = nullptr;

    uint8_t hex_val;
    while(fin.read(reinterpret_cast<char*>(byte), CHAR_SIZE * 2).good())
    {
        if(byte[0] == '\n')
        {
            byte[0] = byte[1];
            fin.read(reinterpret_cast<char*>(&byte[1]), CHAR_SIZE);
        } else if(byte[1] == '\n')
        {
            cerr << "File byte alignment error.\n";
            return 2;
        }
        hex_val = static_cast<uint8_t>(strtoul(byte, &end, 16));
        byte_buffer.push_back(hex_val);
    }

    return 0;
}

int writeFileBuffer(const string_view file, 
                    std::span<uint8_t> buffer, 
                    const string_view message)
{
    std::ofstream fout(file.data(), std::ios::binary);

    const char* hexDigitTable = "0123456789abcdef";
    char hex_values[buffer.size() * 2];

    cout << message;

    for(int i = 0; i < buffer.size(); i++)
    {
        hex_values[2 * i] = static_cast<char>(hexDigitTable[(buffer[i] & 0xf0) >> 4]);
        hex_values[2 * i + 1] = static_cast<char>(hexDigitTable[buffer[i] & 0x0f]);
        if(!message.empty())
        {
            cout << hex_values[2 * i] << hex_values[2 * i + 1];
        }
    }
    if(!message.empty())
    {
        cout << '\n';
    }

    if(fout)
    {
        fout.write(hex_values, sizeof(hex_values));
        fout.put('\n');
    }
    return 0;
}
