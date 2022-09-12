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

    printf("_audioCircularBuffer.Used() : %d\n", _audioCircularBuffer.Used());

    AudioSnapshot temp(_audioCircularBuffer);
    *audio = temp;
    
    // uint8_t* audioP = (uint8_t*)audio;
    // printf("\n\naudio Size %d\n\n", sizeof(AudioSnapshot));
    // for(uint32_t i = 0; i < sizeof(AudioSnapshot); i++)
    // {
    //     printf("%d ", audioP[i]);
    // }
    // audioP = (uint8_t*)&temp;
    // printf("\n\ntemp \n\n");
    // for(uint32_t i = 0; i < sizeof(AudioSnapshot); i++)
    // {
    //     printf("%d ", audioP[i]);
    // }
    // printf("\n\nmemcmp(audio, &temp, sizeof(AudioSnapshot) %d \n\n", memcmp(audio, &temp, sizeof(AudioSnapshot)));
    
    _audioCircularBuffer.Print();
    
    return true;
}

bool AudioCircularBuffer::GetSnapshot(CircularBuffer<int16_t>* buffer)
{
    if (buffer == nullptr)
        return false;
    // printf("_audioCircularBuffer.Used() : %d\n", _audioCircularBuffer.Used());
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
    // printf("FeedAudio.Used() : %d\n", _audioCircularBuffer.Used());
    // _audioCircularBuffer.Print();
}

AudioCircularBuffer& AudioCircularBuffer::Instance()
{
	return *sh.Get();
}

} // namespace SPG::HAL::AudioInterfaces
