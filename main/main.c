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
static EventGroupHandle_t event;
static float fb_fps;
static float fb_bps;
static uint8_t effect = 0;

static const uint8_t RENDER_FINISHED = (1 << 0);
static const uint8_t FLUSH_STARTED= (1 << 1);

static char demo[4][32] = {
    "3 METABALLS   ",
    "PALETTE PLASMA",
    "ROTOZOOM      ",
    "PLANE DEFORM     ",
};

/*
 * Flushes the backbuffer to the display. Needed when using
 * double or triple buffering.
 */
void flush_task(void *params)
{
    while (1) {
        size_t bytes = 0;

        EventBits_t bits = xEventGroupWaitBits(
            event,
            RENDER_FINISHED,
            pdTRUE,
            pdFALSE,
            0
        );

        /* Flush only when RENDER_FINISHED is set. */
        if ((bits & RENDER_FINISHED) != 0 ) {
            xEventGroupSetBits(event, FLUSH_STARTED);
            bytes = hagl_flush();
            fb_bps = aps(bytes);
            fb_fps = fps();
        }
    }

    vTaskDelete(NULL);
}

/*
 * Software vsync. Waits for flush to start. Needed to avoid
 * tearing when using double buffering, NOP otherwise. This
 * could be handler with IRQ's if the display supports it.
 */
static void wait_for_vsync()
{
#ifdef CONFIG_HAGL_HAL_USE_DOUBLE_BUFFERING
    xEventGroupWaitBits(
        event,
        FLUSH_STARTED,
        pdTRUE,
        pdFALSE,
        10000 / portTICK_RATE_MS
    );
    ets_delay_us(18000);
#endif /* CONFIG_HAGL_HAL_USE_DOUBLE_BUFFERING */
}

/*
 * Changes the effect every 10 seconds.
 */
void switch_task(void *params)
{
    while (1) {
        /* Print the message in the console. */
        ESP_LOGI(TAG, "%s %.*f FPS", demo[effect], 1, fb_fps);

        hagl_clear_screen();
        hagl_flush();

        switch(effect) {
        case 0:
            //metaballs_close();
            break;
        case 1:
            plasma_close();
            break;
        case 2:
            //rotozoom_close();
            break;
        case 3:
            deform_close();
            break;
        }

        //effect = (effect + 1) % 4;
        effect = 2;
        
        switch(effect) {
        case 0:
            metaballs_init();
            ESP_LOGI(TAG, "Heap after metaballs init: %d", esp_get_free_heap_size());
            break;
        case 1:
            plasma_init();
            ESP_LOGI(TAG, "Heap after plasma init: %d", esp_get_free_heap_size());
            break;
        case 2:
            rotozoom_init();
            ESP_LOGI(TAG, "Heap after rotozoom init: %d", esp_get_free_heap_size());
            break;
        case 3:
            deform_init();
            ESP_LOGI(TAG, "Heap after deform init: %d", esp_get_free_heap_size());
            break;
        }

        aps(APS_RESET);

        vTaskDelay(10000 / portTICK_RATE_MS);
    }

    vTaskDelete(NULL);
}

/*
 * Runs the actual demo effect.
 */
void demo_task(void *params)
{
    color_t green = hagl_color(0, 255, 0);
    wchar_t message[128];

    /* Avoid waiting when running for the first time. */
    xEventGroupSetBits(event, RENDER_FINISHED);

    while (1) {
        switch(effect) {
        case 0:
            metaballs_animate();
            wait_for_vsync();
            metaballs_render();
            break;
        case 1:
            plasma_animate();
            wait_for_vsync();
            plasma_render();
            break;
        case 2:
            rotozoom_animate();
            wait_for_vsync();
            rotozoom_render();
            break;
        case 3:
            deform_animate();
            wait_for_vsync();
            deform_render();
            break;
        }
        /* Notify flush task that rendering has finished. */
        xEventGroupSetBits(event, RENDER_FINISHED);

        /* Print the message on top left corner. */
        swprintf(message, sizeof(message), u"%s    ", demo[effect]);
        hagl_set_clip_window(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1);
        hagl_put_text(message, 4, 4, green, font6x9);

        /* Print the message on lower left corner. */
        swprintf(message, sizeof(message), u"%.*f FPS  ", 0, fb_fps);
        hagl_put_text(message, 4, DISPLAY_HEIGHT - 14, green, font6x9);

        /* Print the message on lower right corner. */
        swprintf(message, sizeof(message), u"%.*f KBPS  ", 0, fb_bps / 1000);
        hagl_put_text(message, DISPLAY_WIDTH - 60, DISPLAY_HEIGHT - 14, green, font6x9);

        hagl_set_clip_window(0, 20, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 21);
    }

    vTaskDelete(NULL);
}

void app_main()
{
    vTaskDelay(2000 / portTICK_RATE_MS);


    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &i2s_mic_pins);
    int32_t raw_samples[SAMPLE_BUFFER_SIZE];
    color_t green = hagl_color(0, 255, 0);


    ESP_LOGI(TAG, "SDK version: %s", esp_get_idf_version());
    ESP_LOGI(TAG, "Heap when starting: %d", esp_get_free_heap_size());

    event = xEventGroupCreate();

    hagl_init();
    /* Reserve 20 pixels in top and bottom for debug texts. */
    hagl_set_clip_window(0, 20, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 21);

    ESP_LOGI(TAG, "Heap after HAGL init: %d", esp_get_free_heap_size());

    while(true)
    {
        size_t bytes_read = 0;
        i2s_read(I2S_NUM_0, raw_samples, sizeof(int32_t) * SAMPLE_BUFFER_SIZE, &bytes_read, portMAX_DELAY);
        int samples_read = bytes_read / sizeof(int32_t);
        // dump the samples out to the serial channel.
        //for (int i = 0; i < samples_read; i++)
        {
            //printf("%d\n", raw_samples[i]);
        }                           // 135
        //for (uint16_t x = 0; x < DISPLAY_WIDTH; x = x + 1) 
        {
            uint16_t x = 0;
            #define PIXEL_SIZE 2
                                     // 240
            for (uint16_t y = 0; y < DISPLAY_HEIGHT; y = y + PIXEL_SIZE) 
            {
                int16_t data = ((raw_samples[y * 2]) / 1000000) + DISPLAY_WIDTH / 2;
                printf("%d\n", data);
                //hagl_put_pixel(data, y, green);
                hagl_fill_rectangle(data, y, data + PIXEL_SIZE - 1, y + PIXEL_SIZE - 1, green);
                x = x + PIXEL_SIZE;
            }
        }
        hagl_flush();
        hagl_clear_screen();
    }

#ifdef HAGL_HAL_USE_BUFFERING
    xTaskCreatePinnedToCore(flush_task, "Flush", 4096, NULL, 1, NULL, 0);
#endif
    xTaskCreatePinnedToCore(demo_task, "Demo", 8092, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(switch_task, "Switch", 2048, NULL, 2, NULL, 1);
}
