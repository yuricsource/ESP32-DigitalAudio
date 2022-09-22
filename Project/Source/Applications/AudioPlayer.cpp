#include "AudioPlayer.h"
#include "FreeRtosTaskConfig.h"
#include "Hardware.h"
#include "DebugAssert.h"
#include "thread.hpp"
#include "ticks.hpp"
#include "ApplicationMgr.h"
#include "AudioCircularBuffer.h"
#include "TimeLimit.h"
#include "WavReader.h"
#include "I2sSpeaker.h"
#include "CommonTypes.h"

namespace Applications
{
using cpp_freertos::Ticks;
using Utilities::WavReader;

AudioPlayer::AudioPlayer() : cpp_freertos::Thread("PLYASVC", configPLAYERSVC_STACK_DEPTH, 3),
_audioList(QueueLength, sizeof(AudioFileRequest))
{  
}

bool AudioPlayer::playAudioFile(Common::Name& audioFileName, uint8_t volume)
{
    int16_t buffer[BufferSize] = {};
    Hal::I2sSpeaker& speaker = Hal::Hardware::Instance()->GetI2sSpeaker();
    AudioRequest audioRequest(audioFileName.data());
    speaker.Start();
    for(;;)
    {
        size_t read = audioRequest.Read(buffer, BufferSize);
        if (read == 0)
            break;

        if (volume != 100)
        {
            for (uint16_t i = 0; i < read / sizeof(int16_t); i++)
                buffer[i] = (buffer[i] * volume) / 100;
        }
        size_t written = 0;
        speaker.Play(buffer, read, &written);
    }
    speaker.Stop();
    return true;
}

void AudioPlayer::PlayFile(const char* fileName, uint8_t volume)
{
    uint16_t len = strlen(fileName) + 1;
    if (len > sizeof(Common::Name) || len == 1)
    {
        Logger::LogError(Logger::LogSource::Player, "PlayFile | Invaid Audio Request. Len %d, max %d",
            len, sizeof(Common::Name));
        return;
    }
    
    AudioFileRequest audioRequest = {};
    memcpy(audioRequest.Name.data(), fileName, len);
    audioRequest.Volume = volume;
    _audioList.Enqueue(&audioRequest);
}

void AudioPlayer::Run()
{
    Logger::LogInfo(Logger::LogSource::Player, "CORE %d | Audio Player Service Started. [%d]",
        GetCore(), sizeof(AudioPlayer));

    Delay(Ticks::MsToTicks(100));

    for(;;)
    {
        // Waits until a audio request happens
        AudioFileRequest audioRequest;
        if (_audioList.Dequeue(&audioRequest, Ticks::MsToTicks(5000)) == false)
			continue;

        playAudioFile(audioRequest.Name, audioRequest.Volume);
    }
}

} // namespace Applications
