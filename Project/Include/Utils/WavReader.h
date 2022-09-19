#ifndef __wav_file_reader_h__
#define __wav_file_reader_h__

#include <stdio.h>

namespace Utilities
{

class WavReader
{
private:
    FILE* _file;
    uint32_t _offset = 0;
    void seek(uint32_t offiset);
public:
    WavReader(const char *fileName);
    bool Open(const char *fileName);
    WavReader();
    ~WavReader();
    bool Available();
    void Reset();
    
    size_t Read(int16_t* buffer, size_t length);
};

}
#endif