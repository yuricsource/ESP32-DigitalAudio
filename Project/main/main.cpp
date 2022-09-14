#include "Hardware.h"
#include "ApplicationMgr.h"
#include "ticks.hpp"
#include "StatusMgr.h"
#include "AudioCircularBuffer.h"
#include "esp_spiffs.h"
#include "esp_log.h"

using Status::StatusMgr;
using Hal::TimeLimit;
using Hal::Hardware;
using Utilities::Logger;
// static const char *TAG = "example";
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
    appMgr.Instance()->GetAI().Start(1);
    appMgr.Instance()->GetMenu().Start();
    appMgr.Instance()->GetInputScan().Start();
    appMgr.Instance()->GetAudioAnalyzer().Start();
    TimeLimit timerDisplayStatus = {};
/*
    esp_vfs_spiffs_conf_t conf = {};
    conf.base_path = "/spiffs";
    conf.partition_label = NULL;
    conf.max_files = 5;
    conf.format_if_mount_failed = true;

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

      // there is a way to fix it
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }
*/
    
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
