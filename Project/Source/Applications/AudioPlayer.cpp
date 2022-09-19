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

namespace Applications
{
using cpp_freertos::Ticks;
using Utilities::WavReader;

AudioPlayer::AudioPlayer() : cpp_freertos::Thread("PLYASVC", configPLAYERSVC_STACK_DEPTH, 3)
{  
}

bool AudioPlayer::playAudioFile(const char* audioFilePath, uint8_t volume)
{
    WavReader file(audioFilePath);

    if (!file.Available())
        return false;

    int16_t buffer[BufferSize] = {};
    Hal::I2sSpeaker& speaker = Hal::Hardware::Instance()->GetI2sSpeaker();
    
    speaker.Start();
    for(;;)
    {
        size_t read = file.Read(buffer, BufferSize);
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

void AudioPlayer::Run()
{
    Logger::LogInfo(Logger::LogSource::Player, "CORE %d | Audio Player Service Started. [%d]",
        GetCore(), sizeof(AudioPlayer));

    Delay(Ticks::MsToTicks(100));
    playAudioFile("/spiffs/ready.wav", 50);

    for(;;)
    {
        Delay(Ticks::MsToTicks(1000));
    }
}

} // namespace Applications
