#include "AudioPlayer.h"
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

AudioPlayer::AudioPlayer() : cpp_freertos::Thread("PLYASVC", configPLAYERSVC_STACK_DEPTH, 3)
{  
}

void AudioPlayer::Run()
{
    Logger::LogInfo(Logger::LogSource::Player, "CORE %d | Audio Player Service Started. [%d]",
                    GetCore(), sizeof(AudioPlayer));
 
    for(;;)
    {
        Delay(Ticks::MsToTicks(100));
    }
}

} // namespace Applications
