#include "MenuService.h"
#include "FreeRtosTaskConfig.h"
#include "Hardware.h"
#include "DebugAssert.h"
#include "thread.hpp"
#include "ticks.hpp"

namespace Applications
{
using Hal::Hardware;
using cpp_freertos::Ticks;

MenuService::MenuService() : cpp_freertos::Thread("MENUSVC", configMENUSVC_STACK_DEPTH, 3)
{
}

void MenuService::Run()
{
    Logger::LogInfo(Logger::LogSource::Menu, "Menu Service Started");
    DebugAssertMessage(true, "This is a example of assert message");
    for(;;) 
    {
        Hardware* hardware = Hardware::Instance();
        hardware->GetDisplay().Clear();
        Hal::ST7789Display* display = &hardware->GetDisplay();
        Hal::DigitalMicrophone* mic = &hardware->GetDigitalMic();
        for (uint16_t i = 1; i < 2000; i++) 
        {
            int16_t x0 = hardware->GetRng().GetNumber() % display->GetDisplayWidth();
            int16_t y0 = hardware->GetRng().GetNumber() % display->GetDisplayHeight();
            Hal::color_t color = hardware->GetRng().GetNumber() % 0xffff;

            display->PutPixel(x0, y0, color);
        }

        size_t bytes_read = 0;
        Hal::color_t green = 0xffffff;

        static int16_t raw_samples[1024];

        bytes_read = mic->GetAudioBuffer(raw_samples, sizeof(int16_t) * 1024);
        int samples_read = bytes_read / sizeof(int16_t);

        int16_t pData = display->GetDisplayWidth() / 2;
        uint8_t pY = 0;
        uint32_t unitStep = 1024 / display->GetDisplayHeight();

        for (uint16_t i = 0; i < bytes_read; i ++)
        {
            raw_samples[i] = raw_samples[i] / 8;
        }
        
        for (uint16_t y = 0; y < display->GetDisplayHeight(); y += 1) 
        {
            int16_t data = (raw_samples[y * unitStep]) + display->GetDisplayWidth() / 2;
            display->DrawLine(pData, pY, data, y, green);
            pData = data;
            pY = y;
        }
        display->Flush();
        Delay(Ticks::MsToTicks(10));
    }
}

} // namespace Applications
