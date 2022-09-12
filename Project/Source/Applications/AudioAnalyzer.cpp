#include "AudioAnalyzer.h"
#include "FreeRtosTaskConfig.h"
#include "Hardware.h"
#include "DebugAssert.h"
#include "thread.hpp"
#include "ticks.hpp"
#include "ApplicationMgr.h"
#include "AudioCircularBuffer.h"
#include "TimeLimit.h"

namespace Applications
{
using cpp_freertos::Ticks;
using Hal::TimeLimit;
using AudioInterfaces::AudioCircularBuffer;

AudioAnalyzer::AudioAnalyzer() : cpp_freertos::Thread("AUDASVC", configAISVC_STACK_DEPTH, 3)
{
    _mic = &Hardware::Instance()->GetDigitalMic();
}

bool AudioAnalyzer::detectTrigger(int16_t* buffer, size_t bufferSize)
{
    for(uint32_t i = 0; i < bufferSize; i++)
    {
        if (buffer[i] > ThresholdLevel)
            return true; 
    }

    return false;
}

void AudioAnalyzer::Run()
{
    MenuService* displayTask = &Applications::ApplicationMgr::Instance()->GetMenu();
    AIService* aiService = &Applications::ApplicationMgr::Instance()->GetAI();

    // Trigger timer
    TimeLimit triggerTimeLimit = {};
    Logger::LogInfo(Logger::LogSource::Audio, "CORE %d | Audio Analyzer Service Started", GetCore());
 
    for(;;)
    {
        Delay(Ticks::MsToTicks(10));
        uint16_t read = _mic->GetAudioBuffer(_tempAudioBuffer, TempBufferSize);
        displayTask->DisplayAudio(_tempAudioBuffer, read);

        if (triggerTimeLimit.IsTimeUp(1000) && detectTrigger(_tempAudioBuffer, read))
        {
            triggerTimeLimit.Reset();
            // Logger::LogInfo(Logger::LogSource::Audio, "Trigger Detected");
            aiService->GetAudioSnapshot();
        }
        AudioInterfaces::AudioCircularBuffer::Instance().FeedAudio(_tempAudioBuffer, read);
    }
}

} // namespace Applications
