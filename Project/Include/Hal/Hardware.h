#ifndef HAL_SYSTEM_H_
#define HAL_SYSTEM_H_

#include <stdlib.h>
#include <stdio.h>
#include <cstdint>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "HalCommon.h"
#include "Rng.h"
#include "soc/rtc.h"
#include "soc/rtc_periph.h"
#include "DigitalMicrophone.h"
#include "I2sSpeaker.h"
#include "ST7789Display.h"

namespace Hal
{

class Hardware
{
public:
    static inline Hardware *Instance()
	{
		if (_pHardware == nullptr)
		{
			_pHardware = new Hardware();
		}
		return _pHardware;
	}

    Hardware();
	uint32_t Milliseconds();
	DigitalMicrophone &GetDigitalMic() { return _digitalMic; }
	I2sSpeaker &GetI2sSpeaker() { return _i2sSpk; }
	ST7789Display& GetDisplay() { return _display; }
	Rng &GetRng() { return _rng; }
	
	uint32_t GetSystemClockBase()
	{
		return ets_get_cpu_frequency();
	}

private:
    static Hardware *_pHardware;
	esp_chip_info_t _mcuInfo;
	MacAddress _macAdrress;
	Rng _rng;
	DigitalMicrophone _digitalMic;
	I2sSpeaker _i2sSpk;
	ST7789Display _display;


private:
	/// @brief	Hide Copy constructor.
	Hardware(const Hardware &) = delete;

	/// @brief	Hide Assignment operator.
	Hardware &operator=(const Hardware &) = delete;

	/// @brief	Hide Move constructor.
	Hardware(Hardware &&) = delete;

	/// @brief	Hide Move assignment operator.
	Hardware &operator=(Hardware &&) = delete;
};
} // namespace Hal
#endif /* HAL_SYSTEM_H_ */