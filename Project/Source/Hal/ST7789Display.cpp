#include "ST7789Display.h"
#include <hagl_hal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include <font6x9.h>
#include <aps.h>
#include <fps.h>
#include <hagl.h>
#include "../build/config/sdkconfig.h"

#ifdef DEBUG_DISPLAY
        #define TAG "DISPLAY"
#endif

namespace Hal
{
    ST7789Display::ST7789Display(gpio_num_t mosiPin, gpio_num_t clkPin, gpio_num_t csPin, gpio_num_t dataCommandPin, 
                    gpio_num_t resetPin, gpio_num_t backLightPin)
    {
        
        assert(mosiPin == (gpio_num_t)CONFIG_MIPI_DISPLAY_PIN_MOSI);
        assert(clkPin == (gpio_num_t)CONFIG_MIPI_DISPLAY_PIN_CLK);
        assert(csPin == (gpio_num_t)CONFIG_MIPI_DISPLAY_PIN_CS);
        assert(dataCommandPin == (gpio_num_t)CONFIG_MIPI_DISPLAY_PIN_DC);
        assert(resetPin == (gpio_num_t)CONFIG_MIPI_DISPLAY_PIN_RST);
        assert(backLightPin == (gpio_num_t)CONFIG_MIPI_DISPLAY_PIN_BL);
#ifdef DEBUG_DISPLAY
        ESP_LOGI(TAG, "Heap when starting: %d", esp_get_free_heap_size());
#endif
        hagl_init();

#ifdef DEBUG_DISPLAY
        ESP_LOGI(TAG, "Heap after HAGL init: %d", esp_get_free_heap_size());

        for (uint16_t i = 1; i < 500; i++) 
        {
            hagl_clear_screen();
            for (uint16_t i = 1; i < 2000; i++) 
            {
                int16_t x0 = rand() % DISPLAY_WIDTH;
                int16_t y0 = rand() % DISPLAY_HEIGHT;
                color_t color = rand() % 0xffff;

                hagl_put_pixel(x0, y0, color);
                
            }
            hagl_flush();
            vTaskDelay(1);
        }

        for (uint16_t i = 1; i < 10; i++) 
        {
            hagl_clear_screen();
            int16_t x0 = rand() % DISPLAY_WIDTH;
            int16_t y0 = rand() % DISPLAY_HEIGHT;
            color_t color = rand() % 0xffff;

            hagl_put_text("YO! MTV raps.", x0, y0, color, font6x9);
            hagl_flush();
            vTaskDelay(100);
        }
#endif
    }

    size_t ST7789Display::GetDisplayWidth()
    {
        return DISPLAY_WIDTH;
    }

    size_t ST7789Display::GetDisplayHeight()
    {
        return DISPLAY_HEIGHT;
    }

    ST7789Display::~ST7789Display()
    {
    }

    size_t ST7789Display::Flush()
    {
        return hagl_flush();
    }

    void ST7789Display::Clear()
    {
        hagl_clear_screen();
    }

    void ST7789Display::PutPixel(int16_t x0, int16_t y0, color_t color)
    {
        hagl_put_pixel(x0, y0, color);
    }

    void ST7789Display::DrawEllipse(int16_t x0, int16_t y0, int16_t a, int16_t b, color_t color, bool filled)
    {
        if (filled)
            hagl_fill_ellipse(x0, y0, a, b, color);
        else
            hagl_draw_ellipse(x0, y0, a, b, color);
    }

    void ST7789Display::DrawCircle(int16_t xc, int16_t yc, int16_t r, color_t color, bool filled)
    {
        if (filled)
            hagl_fill_circle(xc, yc, r, color);
        else
            hagl_draw_circle(xc, yc, r, color);
    }


    void ST7789Display::DrawTriangle(int16_t x0, int16_t y0,
                                    int16_t x1, int16_t y1, 
                                    int16_t x2, int16_t y2, color_t color, 
                                    bool filled)
    {
        if (filled)
            hagl_fill_triangle(x0, y0, x1, y1, x2, y2, color);
        else
            hagl_draw_triangle(x0, y0, x1, y1, x2, y2, color);
    }

    void ST7789Display::DrawRectangle(int16_t x0, int16_t y0, 
                                        int16_t x1, int16_t y1, 
                                        color_t color, bool filled)
    {
        if (filled)
             hagl_fill_rectangle(x0, y0, x1, y1, color);
        else
             hagl_draw_rectangle(x0, y0, x1, y1, color);
    }
    
    void ST7789Display::DrawRoundedRectangle(int16_t x0, int16_t y0,
                                            int16_t x1, int16_t y1, 
                                            int16_t r, color_t color,
                                             bool filled)
    {
        if (filled)
            hagl_fill_rounded_rectangle(x0, y0, x1, y1, r, color);
        else
            hagl_draw_rounded_rectangle(x0, y0, x1, y1, r, color);
    }

    uint16_t ST7789Display::PrintText(const char *str, int16_t x0, int16_t y0, color_t color)
    {
        if (str == nullptr)
            return 0;

        return hagl_put_text(str, x0, y0, color, font6x9);
    }

    void ST7789Display::DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t color)
    {
        hagl_draw_line(x0, y0, x1, y1, color);
    }
}