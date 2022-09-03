#ifndef INCLUDE_HAL_HALCOMMON_H_
#define INCLUDE_HAL_HALCOMMON_H_

#include <array>
#include "esp_system.h"
#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include <stdio.h>
#include "esp_err.h"
#include <esp_log.h>

namespace Hal
{
using std::array;

static constexpr uint8_t MacAddressMaxLength = 6;
using MacAddress = array<uint8_t, MacAddressMaxLength>;
static_assert(sizeof(MacAddress) == 6, "Array has invalid size.");

} // namespace Hal
#endif /* INCLUDE_HAL_HALCOMMON_H_ */