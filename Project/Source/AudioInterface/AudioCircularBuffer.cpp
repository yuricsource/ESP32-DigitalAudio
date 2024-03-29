#include "SingletonHolder.h"
#include "thread.hpp"
#include "ticks.hpp"
#include "mutex.hpp"
#include "Logger.h"
#include "AudioCircularBuffer.h"
#include "DigitalMicrophone.h"

namespace AudioInterfaces 
{

using Utilities::SingletonHolder;

namespace {
	static SingletonHolder<AudioCircularBuffer> sh;
}

AudioCircularBuffer::AudioCircularBuffer() : 
    _audioCircularBuffer(Hal::DigitalMicrophone::SampleRate * 2)
{
}

bool AudioCircularBuffer::GetSnapshot(AudioSnapshot* audio)
{
    if (audio == nullptr)
        return false;

    AudioSnapshot temp(_audioCircularBuffer);
    *audio = temp;
  
    return true;
}

bool AudioCircularBuffer::GetSnapshot(CircularBuffer<int16_t>* buffer)
{
    if (buffer == nullptr)
        return false;
    *buffer = _audioCircularBuffer;

    return memcmp(buffer, &_audioCircularBuffer, sizeof(_audioCircularBuffer)) == 0;
}

void AudioCircularBuffer::FeedAudio(int16_t* buffer, size_t len)
{
    if (buffer == nullptr || len == 0)
        return;

     if (_audioCircularBuffer.Free() < len)
         _audioCircularBuffer.Skip(len - _audioCircularBuffer.Free());
    
    _audioCircularBuffer.Write(buffer, len);
}

AudioCircularBuffer& AudioCircularBuffer::Instance()
{
	return *sh.Get();
}

} // namespace SPG::HAL::AudioInterfaces
