#include "AIService.h"
#include "FreeRtosTaskConfig.h"
#include "Hardware.h"
#include "DebugAssert.h"
#include "thread.hpp"
#include "ticks.hpp"

namespace Applications
{
using Hal::Hardware;
using cpp_freertos::Ticks;

AIService::AIService() : cpp_freertos::Thread("AISVC", configAISVC_STACK_DEPTH, 3)
{
}

void AIService::Run()
{
    Logger::LogInfo(Logger::LogSource::AI, "CORE %d | AI Service Started", GetCore());
    DebugAssertMessage(true, "This is a example of assert message");
    for(;;)
    {
        Delay(Ticks::MsToTicks(100));
    }
}

} // namespace Applications
