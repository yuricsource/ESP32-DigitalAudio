#include "AIService.h"
#include "FreeRtosTaskConfig.h"
#include "Hardware.h"
#include "DebugAssert.h"
#include "thread.hpp"
#include "ticks.hpp"
#include "AudioSnapshot.h"
#include "AudioCircularBuffer.h"
#include "I2sSpeaker.h"
#include "AudioProcessor.h"
#include "ApplicationMgr.h"

// #define SPEAKER_BUFFER_TEST

namespace Applications
{
using Hal::Hardware;
using Hal::I2sSpeaker;
using cpp_freertos::Ticks;
using AudioInterfaces::AudioSnapshot;
using AudioInterfaces::AudioCircularBuffer;

AIService::AIService() : cpp_freertos::Thread("AISVC", configAISVC_STACK_DEPTH, 3),
_audioTrigger(QueueLength, sizeof(AudioSnapshot)), _neural(converted_model_tflite)
{
}

void AIService::Run()
{
    Logger::LogInfo(Logger::LogSource::AI, "CORE %d | AI Service Started. [%d]", GetCore(), sizeof(AIService));
    DebugAssertMessage(true, "This is a example of assert message");
    AudioSnapshot snapshot;
    AudioProcessor audioProcessor(SampleRate, WindowSize, StepSize, PoolingSize);

#ifdef SPEAKER_BUFFER_TEST
    size_t audioPlayedSize = 0;
#endif

    for(;;)
    {
        // Waits until a trigger happens
        if (_audioTrigger.Dequeue(&snapshot, Ticks::MsToTicks(5000)) == false)
			continue;
        
        // Wait at least one second to copy one second buffer
        while (!snapshot.IsBufferReady(1000))
        {
            vTaskDelay(10);
        }
        Logger::LogInfo(Logger::LogSource::AI, "Trigger Received");
        snapshot.Read(_audioTempBuffer, LocalBufferSize);

        // Testing
        float *input_buffer = _neural.getInputBuffer();
	    audioProcessor.get_spectrogram((int16_t*)_audioTempBuffer, input_buffer);
			
        float output = 0;
        output = _neural.predict();
        if (output > 0.8)
        {
            Logger::LogInfo(Logger::LogSource::AI, "[%d] Key word detected.", (int)(output * 100));
            ApplicationMgr::Instance()->GetAudioPlayer().PlayFile("/spiffs/ready.wav", 30);
        }      
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
