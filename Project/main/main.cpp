#include "Hardware.h"
#include "ApplicationMgr.h"
#include "ticks.hpp"
#include "StatusMgr.h"
#include "AudioCircularBuffer.h"

using Status::StatusMgr;
using Hal::TimeLimit;
using Hal::Hardware;
using Utilities::Logger;

extern "C"
void app_main()
{
    // Initialize the hardware
    Hardware hardware;

    // Initialize Application Manager
    Applications::ApplicationMgr appMgr;

    // Initialize Status Manager
	StatusMgr statusMgr;

	// Start Applications
    appMgr.Instance()->GetAI().Start();
    appMgr.Instance()->GetMenu().Start();
    appMgr.Instance()->GetInputScan().Start();
    appMgr.Instance()->GetAudioAnalyzer().Start();
    TimeLimit timerDisplayStatus = {};
    AudioInterfaces::AudioCircularBuffer::Instance();
    
    for(;;)
    {
        vTaskDelay(cpp_freertos::Ticks::MsToTicks(1000));
        if (timerDisplayStatus.IsTimeUp(30 * 1000))
        {
            timerDisplayStatus.Reset();
            Logger::LogInfo(Logger::LogSource::System, 
                "Free Heap Memory: %d", Hardware::Instance()->GetFreeHeap());
        }
    }
}
