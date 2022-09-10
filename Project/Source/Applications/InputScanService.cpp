#include "InputScanService.h"
#include "FreeRtosTaskConfig.h"
#include "Hardware.h"
#include "DebugAssert.h"
#include "thread.hpp"
#include "ticks.hpp"
#include "InputStatusList.h"
#include "StatusMgr.h"

// #define DEBUG_INPUT_SCAN

namespace Applications
{
using Hal::Hardware;
using Hal::DeviceInput;
using cpp_freertos::Ticks;
using Status::InputStatusList;
using Status::StatusMgr;
using Hal::DigitalInputIndex;
using Hal::AnalogInputIndex;

InputScanService::InputScanService() : cpp_freertos::Thread("INPUTSVC",
                                        configINPUTSVC_STACK_DEPTH, 2)
{
}

void InputScanService::Run()
{
    // Gathering low laywer references
	DeviceInput& deviceInput = Hal::Hardware::Instance()->GetDeviceInput();

	// Getting input and controller Status
	InputStatusList& inputList = StatusMgr::Instance()->GetInputStatusList();

    Logger::LogInfo(Logger::LogSource::Input, "CORE %d |Input Scan Service Started", GetCore());
    for(;;)
    {
        for(uint8_t loops = 0; loops < 10; loops++)
        {
            Delay(Ticks::MsToTicks(1));
            // Read physical input and save in the status list
		    inputList.GetInput(Configuration::InputIndex::ButtonBack).
                SetDigitalLevel(deviceInput.GetDigitalInput(DigitalInputIndex::UserButtonReturn));

            inputList.GetInput(Configuration::InputIndex::ButtonOk).
                SetDigitalLevel(deviceInput.GetDigitalInput(DigitalInputIndex::UserButtonEnter));	

            inputList.GetInput(Configuration::InputIndex::BatteryVoltage).
                SetAnalogLevel(deviceInput.GetAnalogInput(AnalogInputIndex::BatteryVoltage));
        }

#ifdef DEBUG_INPUT_SCAN
        Delay(Ticks::MsToTicks(500));
        for (uint8_t i = 0; i < InputStatusList::Count(); i++)
            Logger::LogInfo(Logger::LogSource::Input, "Input [%d] - Level = %d", i+1, 
            (inputList.GetInput((Configuration::InputIndex)i).GetType() == Hal::InputType::Digital)?
                inputList.GetInput((Configuration::InputIndex)i).GetDigitalLevelDebounce() :
                inputList.GetInput((Configuration::InputIndex)i).GetAnalogLevel());
#else
        Delay(Ticks::MsToTicks(100));
#endif
    }
}

} // namespace Applications
