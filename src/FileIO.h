#ifndef FILEIO_H
#define FILEIO_H

#include <vector>

/*
    Reads file and fills byte buffer with hex values, ignoring the new-line
    characters. 

    [in] file: cstring filepath for file to read
    [out] byte_buffer: vector buffer to read file hex-values to
    [return] int: possible error code
*/
int fileBufferHex(const char* file, std::vector<uint8_t>& byte_buffer);


/*
    Reads file and fills buffer with ASCII characters, ignoring the final new-line
    character. Only the final new-line is ignored, so for messages on multiple lines
    it is included in the message buffer.

    [in] file: cstring filepath for file to read
    [out] byte_buffer: vector buffer to read file bytes to
    [return] int: possible error code
*/

int fileBufferASCII(const char* file, std::vector<uint8_t>& byte_buffer);

#endif
