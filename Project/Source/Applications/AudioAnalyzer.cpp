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
_inputAudioBuffer(DigitalMicrophone::SampleRate * sizeof(int16_t)) // 1 Seconds of sample
{
    _mic = &Hardware::Instance()->GetDigitalMic();
}

void AudioAnalyzer::Run()
{
    Logger::LogInfo(Logger::LogSource::Audio, "CORE %d | Audio Analyzer Service Started", GetCore());
    
    for(;;)
    {
        Delay(Ticks::MsToTicks(10));
        uint16_t read = _mic->GetAudioBuffer(_tempAudioBuffer, TempBufferSize);
        Applications::ApplicationMgr::Instance()->GetMenu().DisplayAudio(
            _tempAudioBuffer, read);
    }
}

} // namespace Applications
