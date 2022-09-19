#ifndef __wav_file_reader_h__
#define __wav_file_reader_h__

#include <stdio.h>

namespace Utilities
{

class WavReader
{
private:
    FILE* _file;

public:
    WavReader(const char *fileName);
    ~WavReader();
    bool Available();
    void Reset();
    
    size_t Read(int16_t* buffer, size_t length);
};

}
#endif