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

class AIService : public cpp_freertos::Thread
{
public:
    AIService();
private:

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

} // namespace Applications