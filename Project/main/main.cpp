#include "Hardware.h"
#include "ApplicationMgr.h"

extern "C"
void app_main()
{
    Hal::Hardware hardware;
    Applications::ApplicationMgr appMgr;
    appMgr.Initialize();
    appMgr.Instance()->GetMenu().Start();
    appMgr.Instance()->GetAI().Start();

    for(;;)
    {
        vTaskDelay(1000);
    }
}
