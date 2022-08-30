#ifndef HAL_SYSTEM_H_
#define HAL_SYSTEM_H_

#include <stdlib.h>
#include <stdio.h>
#include <cstdint>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace Hal
{

class Hardware
{
public:
    Hardware();

    static inline Hardware *Instance()
	{
		if (_pHardware == nullptr)
		{
			_pHardware = new Hardware();
		}
		return _pHardware;
	}
private:
    static Hardware *_pHardware;
};
} // namespace Hal
#endif /* HAL_SYSTEM_H_ */