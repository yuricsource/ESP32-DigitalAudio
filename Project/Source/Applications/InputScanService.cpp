#include "InputScanService.h"
#include "FreeRtosTaskConfig.h"
#include "Hardware.h"
#include "DebugAssert.h"
#include "thread.hpp"
#include "ticks.hpp"

namespace Applications
{
using Hal::Hardware;
using cpp_freertos::Ticks;

InputScanService::InputScanService() : cpp_freertos::Thread("INPUTSVC",
                                        configINPUTSVC_STACK_DEPTH, 3)
{
}

void InputScanService::Run()
{
    Logger::LogInfo(Logger::LogSource::Input, "Input Scan Service Service Started");
    for(;;)
    {
        Delay(Ticks::MsToTicks(100));
    }
}

} // namespace Applications
