#pragma once

#include "Hardware.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "thread.hpp"
#include "TimeLimit.h"
#include "Logger.h"
#include "queue.hpp"
#include "NeuralNetwork.h"
#include "Model.h"

namespace Applications
{

using Hal::TimeLimit;
using Utilities::Logger;
using Hal::Hardware;
using cpp_freertos::Queue;

class AIService : public cpp_freertos::Thread
{
public:
    AIService();
    void GetAudioSnapshot();
private:
    static constexpr uint16_t WindowSize = 320;
	static constexpr uint8_t StepSize = 160;
	static constexpr uint8_t PoolingSize = 6;
	static constexpr uint16_t LocalBufferSize = 16000;
	static constexpr uint16_t SampleRate = 16000;
    static constexpr uint8_t QueueLength = 6;
    Queue _audioTrigger;
    NeuralNetwork _neural;
    int16_t _audioTempBuffer[LocalBufferSize] = {};
   

protected:
    void Run() override;
private:
    /// @brief	Hide Copy constructor.
    AIService(const AIService &) = delete;

    /// @brief	Hide Assignment operator.
    AIService &operator=(const AIService &) = delete;

    /// @brief	Hide Move constructor.
    AIService(AIService &&) = delete;

    /// @brief	Hide Move assignment operator.
    AIService &operator=(AIService &&) = delete;
};
static_assert(sizeof(AIService) == 32084, "Wrong Size, compilation problem.");
} // namespace Applications