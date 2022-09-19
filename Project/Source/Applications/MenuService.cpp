#include "MenuService.h"
#include "FreeRtosTaskConfig.h"
#include "Hardware.h"
#include "DebugAssert.h"
#include "thread.hpp"
#include "ticks.hpp"
#include "StatusMgr.h"

namespace Applications
{
using Hal::Hardware;
using cpp_freertos::Ticks;
using Status::StatusMgr;

MenuService::MenuService() : cpp_freertos::Thread("MENUSVC", configMENUSVC_STACK_DEPTH, 3),
_audioDisplayBuffer(DisplayBufferSize)
{
    _display = &Hardware::Instance()->GetDisplay();
    _rng = &Hardware::Instance()->GetRng();
}

void MenuService::Run()
{
    Logger::LogInfo(Logger::LogSource::Menu, "CORE %d | Menu Service Started. [%d]", GetCore(), sizeof(MenuService));
    for(;;) 
    {
        Delay(Ticks::MsToTicks(10));
        if (_audioDisplayBuffer.Used() > 0)
        {
            prepareBackgroundImage();
            drawAudioLine();
            _display->Flush();
        }
    }
}

void MenuService::drawAudioLine()
{
    int16_t raw_samples[1024];
    size_t bytes_read = 0;
    Hal::color_t colour = 0xffffff;
    bytes_read = _audioDisplayBuffer.Read(raw_samples, 1024);
    
    int16_t pData = _display->GetDisplayWidth() / 2;
    uint8_t pY = 0;
    uint32_t unitStep = bytes_read / _display->GetDisplayHeight();

    for (uint16_t y = 0; y < _display->GetDisplayHeight(); y += 1) 
    {
        int16_t data = (raw_samples[y * unitStep]) + _display->GetDisplayWidth() / 2;
        _display->DrawLine(pData, pY, data, y, colour);
        pData = data;
        pY = y;
    }   
}

void MenuService::DisplayAudio(int16_t* buffer, size_t len)
{
    if (buffer == nullptr || len == 0)
        return;    

    if (_audioDisplayBuffer.Free() < len)
        _audioDisplayBuffer.Skip(len - _audioDisplayBuffer.Free());
    
    _audioDisplayBuffer.Write(buffer, len);
}

void MenuService::prepareBackgroundImage()
{
    _display->Clear();
    for (uint16_t i = 1; i < 2000; i++) 
    {
        int16_t x0 = _rng->GetNumber() % _display->GetDisplayWidth();
        int16_t y0 = _rng->GetNumber() % _display->GetDisplayHeight();
        Hal::color_t color = _rng->GetNumber() % 0xffff;

        _display->PutPixel(x0, y0, color);
    }
}

} // namespace Applications
