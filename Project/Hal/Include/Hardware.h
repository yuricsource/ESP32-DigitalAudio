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
	DigitalMicrophone &GetDigitalMic() { return _digitalMic; }

	uint32_t GetSystemClockBase()
	{
		return rtc_clk_apb_freq_get();
	}

private:
    static Hardware *_pHardware;
	esp_chip_info_t _mcuInfo;
	MacAddress _macAdrress;
	Rng _rng;
	DigitalMicrophone _digitalMic;
};
} // namespace Hal
#endif /* HAL_SYSTEM_H_ */