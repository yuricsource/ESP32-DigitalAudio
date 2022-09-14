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

using Hal::TimeLimit;
using Utilities::Logger;
using Utilities::CircularBuffer;
using Hal::Hardware;
using Hal::DigitalMicrophone;

class AudioAnalyzer : public cpp_freertos::Thread
{
public:
    AudioAnalyzer();
private:
    static constexpr uint16_t TempBufferSize = 512;
    static constexpr int16_t ThresholdLevel = std::numeric_limits<int16_t>::max() / 512;
    DigitalMicrophone* _mic;

    bool detectTrigger(int16_t* buffer, size_t bufferSize);
    int16_t _tempAudioBuffer[TempBufferSize] = {};
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
static_assert(sizeof(AudioAnalyzer) == 1076, "Wrong Size, compilation problem.");
} // namespace Applications