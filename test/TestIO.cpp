#include <iostream>
#include <string>
#include <vector>

#include "TestIO.h"

using namespace std;

void test_msg_buffer(const std::vector<uint8_t>& buf)
{
    const string message = "Two One Nine Two";

    if(buf.size() != message.size())
    {
        cerr << "Test[Message_Buffer_0] Failed!\n";
        cerr << "\tSubkey buffer size (" << buf.size() 
        << ") is not the same size as message (" << 
        message.size() << ")!" << '\n';
        return;
    }

    bool success = true;
    for(size_t i = 0; i < message.size(); i++)
    {
        if(message[i] != buf.at(i))
        {
            cerr << "Test[Message_Buffer_1] Failed!\n";
            cerr << "\tPlaintext buffer index " 
            << i 
            << " is [" 
            << buf.at(i) 
            << "] but should be [" 
            << message[i]
            << "]\n";
            success = false;
        }
    }
    if(!success) return;
    cout << "Test[Message_Buffer] Passed!\n";
}

void test_subkey_buffer(const std::vector<uint8_t>& buf) 
{
    const string key = "5468617473206d79204b756e67204675e232fcf191129188b159e4e6d679a293";

    uint8_t keys[32];
    for(unsigned i = 0; i < 32; i++)
    {
        const string char_pair= key.substr(i * 2, 2);
        keys[i] = static_cast<uint8_t>(stoi(char_pair, 0, 16));
    }

    if(buf.size() != sizeof(keys))
    {
        cout << "Test [SubkeyBuffer_0] Failed!" << '\n';
        cerr << "\tSubkey buffer size (" << buf.size() 
        << ") is not the same size as subkey strings (" << 
        sizeof(keys) << ")!" << '\n';
        return;
    }

    bool success = true;
    for(size_t i = 0; i < sizeof(keys); i++)
    {
        if(keys[i] != buf.at(i))
        {
            cerr << "Test[Subkey_Buffer_1] Failed!\n";
            cerr << "\tSubkey buffer index " 
            << i << std::hex
            << " is [" 
            << buf.at(i) 
            << "] but should be [" 
            << keys[i]
            << "]\n";
            success = false;
        }
    }
    if(!success) return;

    cout << "Test[Subkey_Buffer] Passed!\n";
}
