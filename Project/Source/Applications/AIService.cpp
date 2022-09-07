#include "AIService.h"
#include "FreeRtosTaskConfig.h"
#include "Hardware.h"
#include "DebugAssert.h"
#include "thread.hpp"

namespace Applications
{
using Hal::Hardware;

AIService::AIService() : cpp_freertos::Thread("AISVC", configMENUSVC_STACK_DEPTH, 3)
{
}

void AIService::Run()
{
    Logger::LogInfo(Logger::LogSource::AI, "AI Service Started");
    DebugAssertMessage(true, "This is a example of assert message");
    for(;;)
    {
        Delay(100 / portTICK_PERIOD_MS);
    }
}

} // namespace Applications
