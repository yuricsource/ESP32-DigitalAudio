#pragma once

#include <cstdint>
#include "semaphore.hpp"
#include "CircularBuffer.h"
#include "AudioSnapshot.h"

namespace AudioInterfaces 
{
    using Utilities::CircularBuffer;

    class AudioCircularBuffer
    {
    public:
        AudioCircularBuffer();
        static AudioCircularBuffer& Instance();
        void FeedAudio(int16_t* buffer, size_t len);
        bool GetSnapshot(CircularBuffer<int16_t>* buffer);
        bool GetSnapshot(AudioSnapshot* audio);

    private:
        CircularBuffer<int16_t> _audioCircularBuffer;
    }; 

}