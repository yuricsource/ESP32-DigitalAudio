#pragma once

#include <cstdint>
#include "semaphore.hpp"
#include "CircularBuffer.h"
#include "TimeLimit.h"
#include "DigitalMicrophone.h"

namespace AudioInterfaces 
{
using Utilities::CircularBuffer;
using Hal::TimeLimit;
class AudioSnapshot
{
public:
    AudioSnapshot() : _audioBuffer(0,0)
    {
    }

    AudioSnapshot(CircularBuffer<int16_t>& audioBuffer) : 
    _audioBuffer(audioBuffer)
    {
        _timeLimit.Reset();
    }

    bool IsBufferReady(uint32_t timeSample)
    {
        return _timeLimit.IsTimeUp(timeSample);
    }

    bool IsBufferStillValid()
    {
        // The buffer size if Mic Sample Rate * Seconds
        // If the buffer is 2 seconds old, it is already invalid
        uint32_t time = _audioBuffer.Size() / Hal::DigitalMicrophone::SampleRate;
        return !_timeLimit.IsTimeUp(time * 1000);
    }

    uint32_t Read(int16_t* buffer, size_t len)
    {
        if (!IsBufferStillValid())
            return 0;
        return _audioBuffer.Read(buffer, len);
    }

    AudioSnapshot& operator=(const AudioSnapshot& other)
    {
        // Guard self assignment
        if (this == &other)
            return *this;
            
        // std::memcpy(this, &other, sizeof(AudioSnapshot));
        this->_audioBuffer = other._audioBuffer;
        this->_timeLimit = other._timeLimit;
        return *this;
    }

private:
    CircularBuffer<int16_t> _audioBuffer;
    TimeLimit _timeLimit;
};

// static_assert(sizeof(AudioSnapshot) == 48, "Wrong size AudioSnapshot, compilation error!");
}