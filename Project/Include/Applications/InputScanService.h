#pragma once

#include "Hardware.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "thread.hpp"
#include "TimeLimit.h"
#include "Logger.h"

namespace Applications
{

using Hal::TimeLimit;
using Utilities::Logger;
using Hal::Hardware;

class InputScanService : public cpp_freertos::Thread
{
public:
    InputScanService();
private:

protected:
    void Run() override;

private:
    /// @brief	Hide Copy constructor.
    InputScanService(const InputScanService &) = delete;

    /// @brief	Hide Assignment operator.
    InputScanService &operator=(const InputScanService &) = delete;

    /// @brief	Hide Move constructor.
    InputScanService(InputScanService &&) = delete;

    /// @brief	Hide Move assignment operator.
    InputScanService &operator=(InputScanService &&) = delete;
};

} // namespace Applications