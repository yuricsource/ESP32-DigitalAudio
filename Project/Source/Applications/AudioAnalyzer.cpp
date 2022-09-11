#include "AudioAnalyzer.h"
#include "FreeRtosTaskConfig.h"
#include "Hardware.h"
#include "DebugAssert.h"
#include "thread.hpp"
#include "ticks.hpp"
#include "ApplicationMgr.h"

namespace Applications
{
using cpp_freertos::Ticks;

AudioAnalyzer::AudioAnalyzer() : cpp_freertos::Thread("AUDASVC", configAISVC_STACK_DEPTH, 3),
_inputAudioBuffer(DigitalMicrophone::SampleRate * 2) // 2 Seconds of sample
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
    Logger::LogInfo(Logger::LogSource::Audio, "CORE %d | Audio Analyzer Service Started", GetCore());
    Logger::LogInfo(Logger::LogSource::Audio, "Max int16_t=%d and Thredshold level:%d", std::numeric_limits<int16_t>::max(), ThresholdLevel);

    for(;;)
    {
        Delay(Ticks::MsToTicks(10));
        uint16_t read = _mic->GetAudioBuffer(_tempAudioBuffer, TempBufferSize);
        displayTask->DisplayAudio(_tempAudioBuffer, read);

        if (detectTrigger(_tempAudioBuffer, read))
        {
            Logger::LogInfo(Logger::LogSource::Audio, "Trigger Detected");
        }
        

        if (_inputAudioBuffer.Free() < read)
            _inputAudioBuffer.Skip(read - _inputAudioBuffer.Free());
        
        _inputAudioBuffer.Write(_tempAudioBuffer, read);
    }
}

} // namespace Applications
