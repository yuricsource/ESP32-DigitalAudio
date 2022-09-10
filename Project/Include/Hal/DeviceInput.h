
#ifndef INCLUDE_HAL_DEVICEINPUT_H_
#define INCLUDE_HAL_DEVICEINPUT_H_

#include "HalCommon.h"
#include "Gpio.h"
#include "Analog.h"

namespace Hal
{

class DeviceInput
{

public:
	uint16_t GetAnalogInput(AnalogInputIndex input, uint8_t averageSamples = 1);
	DeviceInput(Gpio* IoPins, Adc* adc);
	~DeviceInput();
	bool GetDigitalInput(DigitalInputIndex input);

private:
	Gpio* _gpio;
	Adc* _adc;
	Gpio::GpioIndex digitalInputIndex[MaxDigitalInputs] =
	{
		Gpio::GpioIndex::Gpio0,
		Gpio::GpioIndex::Gpio35
	};
	
	Gpio::GpioIndex analogIndex[MaxAnalogInputs] =
	{
		Gpio::GpioIndex::Gpio34
	};
	
	Adc::AdcIndex analogAdcIndex[MaxAnalogInputs] = {};
};
} // namespace Hal

#endif /* INCLUDE_HAL_DEVICEINPUT_H_ */
