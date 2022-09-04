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

class MenuService : public cpp_freertos::Thread
{
public:
    MenuService();
private:

protected:
    void Run() override;

private:
    /// @brief	Hide Copy constructor.
    MenuService(const MenuService &) = delete;

    /// @brief	Hide Assignment operator.
    MenuService &operator=(const MenuService &) = delete;

    /// @brief	Hide Move constructor.
    MenuService(MenuService &&) = delete;

    /// @brief	Hide Move assignment operator.
    MenuService &operator=(MenuService &&) = delete;
};

} // namespace Applications