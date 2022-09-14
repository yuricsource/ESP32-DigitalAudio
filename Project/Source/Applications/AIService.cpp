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
    size_t audioPlayedSize = 0;
    AudioProcessor audioProcessor(SampleRate, WindowSize, StepSize, PoolingSize);
    for(;;)
    {
        // Waits until a trigger happens
        if (_audioTrigger.Dequeue(&snapshot, Ticks::MsToTicks(5000)) == false)
			continue;
        // Logger::LogInfo(Logger::LogSource::AI, "Trigger Received");
        
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
        if (output > 0.3)
            Logger::LogInfo(Logger::LogSource::AI, "[%d] ->>> That is my name! \\o/", (int)(output * 100));
        else
        {
            // Adding a gain and testing again
            for (uint16_t i = 0 ; i < LocalBufferSize; i ++)
            {
                _audioTempBuffer[i] *= 10;
            }
            // Testing
            float *input_buffer = _neural.getInputBuffer();
            audioProcessor.get_spectrogram((int16_t*)_audioTempBuffer, input_buffer);
                
            float output = 0;
            output = _neural.predict();
            if (output > 0.3)
                Logger::LogInfo(Logger::LogSource::AI, "2[%d] ->>> That is my name! \\o/", (int)(output * 100));
            else
                Logger::LogInfo(Logger::LogSource::AI, "[%d] That is not me! :(", (int)(output * 100));
        }    

#ifdef SPEAKER_BUFFER_TEST
        int16_t length = 16000;
        static constexpr uint16_t bufferSize = 128;
        static int16_t bufferTemp[bufferSize] = {};
        Hardware::Instance()->GetI2sSpeaker().Start();
        while(length > 0)
        {
            int read = snapshot.Read(bufferTemp, bufferSize);
            if (read == 0)
                break;
            length = length - read; 
            Hardware::Instance()->GetI2sSpeaker().Play(bufferTemp, bufferSize * sizeof(int16_t), &audioPlayedSize);
        }
        Hardware::Instance()->GetI2sSpeaker().Stop();
#endif
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
