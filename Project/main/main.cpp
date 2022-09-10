#include "Hardware.h"
#include "ApplicationMgr.h"
#include "ticks.hpp"

extern "C"
void app_main()
{
    Hal::Hardware hardware;
    Applications::ApplicationMgr appMgr;
    appMgr.Initialize();
    appMgr.Instance()->GetMenu().Start();
    appMgr.Instance()->GetAI().Start();
    appMgr.Instance()->GetInputScan().Start();

    for(;;)
    {
        vTaskDelay(cpp_freertos::Ticks::MsToTicks(1000));
    }
}
