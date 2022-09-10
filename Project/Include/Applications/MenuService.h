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

using Utilities::CircularBuffer;
using Utilities::Logger;
using Hal::Hardware;
using Hal::ST7789Display;
using Hal::TimeLimit;
using Hal::Rng;

class MenuService : public cpp_freertos::Thread
{
public:
    MenuService();
    void DisplayAudio(int16_t* buffer, size_t len);

private:
    static constexpr uint16_t DisplayBufferSize = 1024;
    CircularBuffer<int16_t> _audioDisplayBuffer;

    void prepareBackgroundImage();
    void drawAudioLine();

    ST7789Display* _display;
    Rng* _rng;

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