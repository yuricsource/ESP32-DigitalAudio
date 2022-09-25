#include "Hardware.h"
#include "DebugAssert.h"
#include "sdkconfig.h"
#include "tensorflow/lite/version.h"
#include "FreeRtosTaskConfig.h"

namespace Hal
{
Hardware *Hardware::_pHardware;

Hardware::Hardware():   _gpio(),
						_adc(&_gpio),
						_deviceInput(&_gpio, &_adc),
						_rng(),
                        _digitalMic(GPIO_NUM_2, GPIO_NUM_13, GPIO_NUM_26),
                        _i2sSpk(GPIO_NUM_27, GPIO_NUM_12, GPIO_NUM_25),
						_display(GPIO_NUM_19, GPIO_NUM_18, GPIO_NUM_5, GPIO_NUM_16, GPIO_NUM_23, GPIO_NUM_4),
						_sdCard(&_gpio, GPIO_NUM_17, GPIO_NUM_22, GPIO_NUM_21, GPIO_NUM_15)
{
    esp_chip_info(&_mcuInfo);
	esp_base_mac_addr_get(_macAdrress.data());
	printf("\n\n");
	printf("SDK Version         		: %s\n", (char *)esp_get_idf_version());
	printf("CPU Cores           		: %d\n", _mcuInfo.cores);
	printf("APB Clock           		: %d MHz\n", GetSystemClockBase());
	printf("CPU Revision        		: %d\n", _mcuInfo.revision);
	printf("Embedded Flash      		: %s\n", (_mcuInfo.features & CHIP_FEATURE_EMB_FLASH) ? "YES" : "NO");
	printf("Wi-Fi Modle         		: %s\n", (_mcuInfo.features & CHIP_FEATURE_WIFI_BGN) ? "YES" : "NO");
	printf("Bluetooth Classic   		: %s\n", (_mcuInfo.features & CHIP_FEATURE_BT) ? "YES" : "NO");
	printf("Bluetooth LE        		: %s\n", (_mcuInfo.features & CHIP_FEATURE_BLE) ? "YES" : "NO");
	printf("MAC Address         		: %02X:%02X:%02X:%02X\n",
		_macAdrress[0], _macAdrress[1], _macAdrress[2], _macAdrress[3]);
	printf("MCU Free Heap       		: %d\n", GetFreeHeap());
	printf("FreeRtos Task Memory  		: %d\n", configTOTAL_PROJECT_HEAP_SIZE_ALLOCATED);
	printf("TensorFlow lite         	: v%d.%d.%d\n", TF_MAJOR_VERSION, TF_MINOR_VERSION, TF_PATCH_VERSION);
	printf("\n");
	
	if (_pHardware == nullptr)
		_pHardware = this;
	else
		DebugAssertFail("!!! Error: Only one instance of Hardware can be created !!!\n");

	// Try mounting the SD card
	// _sdCard.Mount()
}

uint32_t Hardware::Milliseconds()
{
	return xTaskGetTickCount() * (1000 / configTICK_RATE_HZ);
}

}