#pragma once

#include "Hardware.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "thread.hpp"
#include "TimeLimit.h"
#include "Logger.h"
#include "CircularBuffer.h"

namespace Applications
{
using Utilities::Logger;

class AudioPlayer : public cpp_freertos::Thread
{
public:
    AudioPlayer();
private:

protected:
    void Run() override;

private:
    /// @brief	Hide Copy constructor.
    AudioPlayer(const AudioPlayer &) = delete;

    /// @brief	Hide Assignment operator.
    AudioPlayer &operator=(const AudioPlayer &) = delete;

    /// @brief	Hide Move constructor.
    AudioPlayer(AudioPlayer &&) = delete;

    /// @brief	Hide Move assignment operator.
    AudioPlayer &operator=(AudioPlayer &&) = delete;
};
static_assert(sizeof(AudioPlayer) == 48, "Wrong Size, compilation problem.");
} // namespace Applications