#include "AIService.h"
#include "FreeRtosTaskConfig.h"
#include "Hardware.h"
#include "DebugAssert.h"
#include "thread.hpp"
#include "ticks.hpp"
#include "AudioSnapshot.h"
#include "AudioCircularBuffer.h"
#include "I2sSpeaker.h"

namespace Applications
{
using Hal::Hardware;
using Hal::I2sSpeaker;
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
    AudioSnapshot snapshot;
    size_t audioPlayedSize = 0;
    for(;;)
    {
        // Waits until a trigger happens
        if (_audioTrigger.Dequeue(&snapshot, Ticks::MsToTicks(5000)) == false)
			continue;
        Logger::LogInfo(Logger::LogSource::AI, "Trigger Received");
        int16_t length = 16000;
        static constexpr uint16_t bufferSize = 128;
        static int16_t bufferTemp[bufferSize] = {};
        
        // Wait at least one second to copy one second buffer
        while (!snapshot.IsBufferReady(1000))
        {
            vTaskDelay(10);
        }
          

        Hardware::Instance()->GetI2sSpeaker().Start();
        while(length > 0)
        {
            int read = snapshot.Read(bufferTemp, bufferSize);
            if (read == 0)
                break;
            length = length - read; 
            // Logger::LogInfo(Logger::LogSource::AI, "length: %d, read:%d",length, read);
            Hardware::Instance()->GetI2sSpeaker().Play(bufferTemp, bufferSize * sizeof(int16_t), &audioPlayedSize);
        }
        Hardware::Instance()->GetI2sSpeaker().Stop();
    }
}

void AIService::GetAudioSnapshot()
{
    if (_audioTrigger.IsFull())
    {
        Logger::LogError(Logger::LogSource::AI, "Unable to enqueue more audio snapshots.");
        return;
    }

    AudioSnapshot temp;
    bool result = AudioCircularBuffer::Instance().GetSnapshot(&temp);
    DebugAssertMessage(result, "AudioCircularBuffer not copied properly!");

    _audioTrigger.Enqueue(&temp);
}


} // namespace Applications
