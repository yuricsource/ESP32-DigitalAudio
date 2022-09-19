#include "Hardware.h"
#include "ApplicationMgr.h"
#include "ticks.hpp"
#include "StatusMgr.h"
#include "AudioCircularBuffer.h"
#include "FatFS.h"

using Status::StatusMgr;
using Hal::TimeLimit;
using Hal::Hardware;
using Utilities::Logger;
using FileSystem::FatFS;
using Common::PartitionTypes;

extern "C"
void app_main()
{
    // Initialize the hardware
    Hardware hardware;

    // Initialize Application Manager
    Applications::ApplicationMgr appMgr;

    // Initialize Status Manager
	StatusMgr statusMgr;

    // Initialize SPIFFS
    FatFS::Initialize(PartitionTypes::Spiffs); 
    Logger::LogInfo(Logger::LogSource::System, 
        "Partition SPIFFS Initialized Size: %d, Free:%d", 
        FatFS::GetTotalSpace(PartitionTypes::Spiffs),
        FatFS::GetFreeSpace(PartitionTypes::Spiffs));
        
	// Start Applications
    appMgr.Instance()->GetAI().Start(1);
    appMgr.Instance()->GetMenu().Start();
    appMgr.Instance()->GetInputScan().Start();
    appMgr.Instance()->GetAudioAnalyzer().Start();
    appMgr.Instance()->GetAudioPlayer().Start();
    
    TimeLimit timerDisplayStatus = {};
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
