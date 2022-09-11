#include "AIService.h"
#include "FreeRtosTaskConfig.h"
#include "Hardware.h"
#include "DebugAssert.h"
#include "thread.hpp"
#include "ticks.hpp"
#include "AudioSnapshot.h"
#include "AudioCircularBuffer.h"

namespace Applications
{
using Hal::Hardware;
using cpp_freertos::Ticks;
using AudioInterfaces::AudioSnapshot;
using AudioInterfaces::AudioCircularBuffer;

AIService::AIService() : cpp_freertos::Thread("AISVC", configAISVC_STACK_DEPTH, 3),
_audioTrigger(4, sizeof(AudioSnapshot))
{
}

void AIService::Run()
{
    Logger::LogInfo(Logger::LogSource::AI, "CORE %d | AI Service Started", GetCore());
    DebugAssertMessage(true, "This is a example of assert message");
    AudioSnapshot audioTigger;
    for(;;)
    {
        // Waits until a trigger happens
        if (_audioTrigger.Dequeue(&audioTigger, Ticks::MsToTicks(5000)) == false)
			continue;
        Logger::LogInfo(Logger::LogSource::AI, "Trigger Received");
    }
}

void AIService::GetAudioSnapshot()
{
    AudioSnapshot temp;
    bool result = AudioCircularBuffer::Instance().GetSnapshot(&temp);
    DebugAssertMessage(result, "AudioCircularBuffer not copied properly!");
    _audioTrigger.Enqueue(&temp);
}


} // namespace Applications
