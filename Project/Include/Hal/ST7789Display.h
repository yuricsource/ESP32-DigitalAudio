#ifndef HAL_DISPLAY_ST7789_H_
#define HAL_DISPLAY_ST7789_H_

#include "driver/i2s.h" 
#include "HalCommon.h"

namespace Hal
{
#ifndef CONFIG_MIPI_DCS_PIXEL_FORMAT_24BIT_SELECTED
typedef uint32_t color_t;
#endif

    class ST7789Display
    {
        public:
            ST7789Display(gpio_num_t mosiPin, gpio_num_t clkPin, gpio_num_t csPin, gpio_num_t dataCommandPin, 
                    gpio_num_t resetPin, gpio_num_t backLightPin);
            ~ST7789Display();
            void Clear();
            size_t Flush();
            void DrawCircle(int16_t xc, int16_t yc, int16_t r, color_t color, bool filled = false);
            void PutPixel(int16_t x0, int16_t y0, color_t color);
            void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t color);
            void DrawEllipse(int16_t x0, int16_t y0, int16_t a, int16_t b, color_t color, bool filled = false);
            void DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, color_t color, bool filled = false);
            void DrawRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t color, bool filled = false);
            void DrawRoundedRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, color_t color, bool filled = false);
            uint16_t PrintText(const char *str, int16_t x0, int16_t y0, color_t color);
            size_t GetDisplayWidth();
            size_t GetDisplayHeight();
        private:
           
        private:
            /// @brief	Hide Copy constructor.
            ST7789Display(const ST7789Display &) = delete;

            /// @brief	Hide Assignment operator.
            ST7789Display &operator=(const ST7789Display &) = delete;

            /// @brief	Hide Move constructor.
            ST7789Display(ST7789Display &&) = delete;

            /// @brief	Hide Move assignment operator.
            ST7789Display &operator=(ST7789Display &&) = delete;
    };
}

#endif /* HAL_DISPLAY_ST7789_H_ */