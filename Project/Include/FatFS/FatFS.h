#pragma once

#include <cstdint>
#include "CommonTypes.h"
#include "TimeLimit.h"

namespace FileSystem
{
using Common::PartitionTypes;

class FatFS
{
public:
    static bool Initialize(PartitionTypes type);
    static size_t GetFreeSpace(PartitionTypes type);
    static size_t GetTotalSpace(PartitionTypes type);
    static size_t GetUsedSpace(PartitionTypes type);
private:
    static bool StartSpiffs();
};
} //FileSystem