#pragma once

#include <cstdint>
#include <array>
#include "TimeLimit.h"
#include "WavReader.h"
namespace AudioInterfaces 
{

using Hal::TimeLimit;
using Utilities::WavReader;

class AudioRequest
{
public:
    AudioRequest(const char* fileName);
    ~AudioRequest();
    void RepeatTime(uint32_t mSeconds);
    void RepeatCount(uint8_t count);
    size_t Read(int16_t* buffer, size_t len);

private:
    static constexpr uint8_t FileNameSize = 30;
    WavReader _audioFile;
    std::array<char, FileNameSize> _fileName;
    bool _repeat = false;
    uint8_t _count = 0;
    TimeLimit _timeLimit = {};
    uint32_t _timeout = 0;
};
// static_assert(sizeof(AudioRequest) == 28, "Wrong size AudioSnapshot, compilation error!");
}