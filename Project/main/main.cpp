#include "Hardware.h"
#include "ApplicationMgr.h"
#include "ticks.hpp"
#include "StatusMgr.h"

using Status::StatusMgr;

extern "C"
void app_main()
{
    // Initialize the hardware
    Hal::Hardware hardware;

    // Initialize Application Manager
    Applications::ApplicationMgr appMgr;

    // Initialize Status Manager
	StatusMgr statusMgr;

	// Start Applications
    appMgr.Instance()->GetAI().Start(1);
    appMgr.Instance()->GetMenu().Start();
    appMgr.Instance()->GetInputScan().Start();
    appMgr.Instance()->GetAudioAnalyzer().Start();

    for(;;)
    {
        vTaskDelay(cpp_freertos::Ticks::MsToTicks(1000));
    }
}
