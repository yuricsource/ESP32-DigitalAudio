/*

MIT No Attribution

Copyright (c) 2020 Mika Tuupola

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-cut-

SPDX-License-Identifier: MIT-0

*/

#include "sdkconfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_log.h>

#ifdef CONFIG_DEVICE_HAS_AXP192
#include <i2c_helper.h>
#include <axp192.h>
#endif /* CONFIG_DEVICE_HAS_AXP192 */
#ifdef CONFIG_DEVICE_HAS_AXP202
#include <i2c_helper.h>
#include <axp202.h>
#endif /* CONFIG_DEVICE_HAS_AXP202 */

#include <font6x9.h>
#include <aps.h>
#include <fps.h>
#include <hagl_hal.h>
#include <hagl.h>

#include "metaballs.h"
#include "plasma.h"
#include "rotozoom.h"
#include "deform.h"

#include "driver/i2s.h" 

// you shouldn't need to change these settings
#define SAMPLE_BUFFER_SIZE 512
#define SAMPLE_RATE 8000
// most microphones will probably default to left channel but you may need to tie the L/R pin low
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_LEFT
// either wire your microphone to the same pins or change these to match your wiring
#define I2S_MIC_SERIAL_CLOCK GPIO_NUM_2
#define I2S_MIC_LEFT_RIGHT_CLOCK GPIO_NUM_13
#define I2S_MIC_SERIAL_DATA GPIO_NUM_26
#define PIXEL_SIZE 2

// don't mess around with this
i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0};

// and don't mess around with this
i2s_pin_config_t i2s_mic_pins = {
    .bck_io_num = I2S_MIC_SERIAL_CLOCK,
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_MIC_SERIAL_DATA};

static const char *TAG = "main";

void app_main()
{
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &i2s_mic_pins);
    int32_t raw_samples[SAMPLE_BUFFER_SIZE];
    color_t green = hagl_color(0, 255, 0);
    ESP_LOGI(TAG, "Heap when starting: %d", esp_get_free_heap_size());
    hagl_init();
    ESP_LOGI(TAG, "Heap after HAGL init: %d", esp_get_free_heap_size());

    while(true)
    {
        size_t bytes_read = 0;
        i2s_read(I2S_NUM_0, raw_samples, sizeof(int32_t) * SAMPLE_BUFFER_SIZE, &bytes_read, portMAX_DELAY);
        int samples_read = bytes_read / sizeof(int32_t);

        int16_t pData = DISPLAY_WIDTH / 2;
        uint8_t pY = 0;
                                    // 240
        for (uint16_t y = 0; y < DISPLAY_HEIGHT; y = y + PIXEL_SIZE) 
        {
            int16_t data = ((raw_samples[y * 2]) / 500000) + DISPLAY_WIDTH / 2;
            hagl_draw_line(pData, pY, data, y, green);
            pData = data;
            pY = y;
        }
        hagl_flush();
        hagl_clear_screen();
    }
}
