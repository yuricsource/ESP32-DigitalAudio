#include "AudioAnalyzer.h"
#include "FreeRtosTaskConfig.h"
#include "Hardware.h"
#include "DebugAssert.h"
#include "thread.hpp"
#include "ticks.hpp"

namespace Applications
{
using Hal::Hardware;
using cpp_freertos::Ticks;

AudioAnalyzer::AudioAnalyzer() : cpp_freertos::Thread("AUDASVC", configAISVC_STACK_DEPTH, 3)
{
}

void AudioAnalyzer::Run()
{
    Logger::LogInfo(Logger::LogSource::Audio, "CORE %d |Audio Analyzer Service Started", GetCore());

    for(;;)
    {
        Delay(Ticks::MsToTicks(100));
    }
}

} // namespace Applications
