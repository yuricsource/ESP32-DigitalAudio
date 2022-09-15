#include <cstdint>
#include "TimeLimit.h"
#include "FatFS.h"
#include "esp_spiffs.h"
#include "esp_log.h"
#include "DebugAssert.h"
#include "Logger.h"

namespace FileSystem
{

using Utilities::Logger;

bool FatFS::Initialize(PartitionTypes type)
{
    switch(type)
    {
        case PartitionTypes::Sdcard:
            DebugAssertFail("Sd Card not implemented.");
        break;
        case PartitionTypes::Spiffs:
            return StartSpiffs();
        break;
    }
    return false;
}

size_t FatFS::GetFreeSpace(PartitionTypes type)
{
    switch(type)
    {
        case PartitionTypes::Sdcard:
            DebugAssertFail("Sd Card not implemented.");
        break;
        case PartitionTypes::Spiffs:
            size_t total = 0, used = 0;
            esp_spiffs_info(nullptr, &total, &used);
            return total - used;
        break;
    }
    return 0;
}

size_t FatFS::GetTotalSpace(PartitionTypes type)
{
    switch(type)
    {
        case PartitionTypes::Sdcard:
            DebugAssertFail("Sd Card not implemented.");
        break;
        case PartitionTypes::Spiffs:
            size_t total = 0, used = 0;
            esp_spiffs_info(nullptr, &total, &used);
            return total;
        break;
    }
    return 0;
}

size_t FatFS::GetUsedSpace(PartitionTypes type)
{
    switch(type)
    {
        case PartitionTypes::Sdcard:
            DebugAssertFail("Sd Card not implemented.");
        break;
        case PartitionTypes::Spiffs:
            size_t total = 0, used = 0;
            esp_spiffs_info(nullptr, &total, &used);
            return used;
        break;
    }
    return 0;
}

bool FatFS::StartSpiffs()
{
    esp_vfs_spiffs_conf_t conf = {};
    conf.base_path = "/spiffs";
    conf.partition_label = nullptr;
    conf.max_files = 2;
    conf.format_if_mount_failed = true;

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            Logger::LogError(Logger::LogSource::FileSystem, 
                "Spiffs | Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            Logger::LogError(Logger::LogSource::FileSystem, "Failed to find SPIFFS partition");
        } else {
            Logger::LogError(Logger::LogSource::FileSystem, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return false;
    }   
    return true;
}
} //FileSystem