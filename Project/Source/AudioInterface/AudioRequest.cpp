#include "AudioRequest.h"
#include "Logger.h"
#include "string.h"

namespace AudioInterfaces 
{
using Utilities::Logger;

AudioRequest::AudioRequest(const char* fileName)
{
    if (!_audioFile.Open(fileName))
    {
        Logger::LogError(Logger::LogSource::Audio, 
            "AudioRequest | File %s doesn't exist", fileName);
        return ;
    }
}

void AudioRequest::RepeatTime(uint32_t mSeconds)
{
    _timeout = mSeconds;
    if (_timeout != 0)
        _timeLimit.Reset();
}

void AudioRequest::RepeatCount(uint8_t count)
{
    _count = count;
    if (_count != 0)
        _repeat = true;
}

size_t AudioRequest::Read(int16_t* buffer, size_t len)
{
    size_t read = _audioFile.Read(buffer, len);
    
    if (_timeout != 0 && _timeLimit.IsTimeUp(_timeout))
    {
        _timeout = 0;
        return 0;
    }

    if (_repeat && _count == 0)
    {
        _repeat = false;
        return 0;
    }

    if ((_timeout != 0 || _repeat) && read == 0)
    {
        _audioFile.Reset();
        read = _audioFile.Read(buffer, len);
        if (_count != 0)
            _count--;
    }
    return read;
}

AudioRequest::~AudioRequest()
{

}

}