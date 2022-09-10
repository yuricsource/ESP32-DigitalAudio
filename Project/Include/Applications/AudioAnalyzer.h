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

class AudioAnalyzer : public cpp_freertos::Thread
{
public:
    AudioAnalyzer();
private:

protected:
    void Run() override;

private:
    /// @brief	Hide Copy constructor.
    AudioAnalyzer(const AudioAnalyzer &) = delete;

    /// @brief	Hide Assignment operator.
    AudioAnalyzer &operator=(const AudioAnalyzer &) = delete;

    /// @brief	Hide Move constructor.
    AudioAnalyzer(AudioAnalyzer &&) = delete;

    /// @brief	Hide Move assignment operator.
    AudioAnalyzer &operator=(AudioAnalyzer &&) = delete;
};

} // namespace Applications