#ifndef FILEIO_H
#define FILEIO_H

#include <vector>
#include <span>
#include <string_view>

/*
    Reads file and fills byte buffer with hex values, ignoring the new-line
    characters. 

    [in] file: cstring filepath for file to read
    [out] byte_buffer: vector buffer to read file hex-values to
    [return] int: possible error code
*/
int fileBufferHex(const std::string_view file, std::vector<uint8_t>& byte_buffer);


/*
    Reads file and fills buffer with ASCII characters, ignoring the final new-line
    character. Only the final new-line is ignored, so for messages on multiple lines
    it is included in the message buffer.

    [in] file: cstring filepath for file to read
    [out] byte_buffer: vector buffer to read file bytes to
    [return] int: possible error code
*/

int fileBufferASCII(const std::string_view file, std::vector<uint8_t>& byte_buffer);


/*
    Write contents of buffer to file, converting Hex values in the buffer
    to ASCII values.

    [in] file: cstring filepath for file to write to
    [in] buffer: hex buffer to write to file
    [in] message: message to get printed to terminal if supplied
    [return] int: possible error code
*/
int writeFileBuffer(const std::string_view file, 
                    std::span<uint8_t> buffer, 
                    const std::string_view message);


#endif
