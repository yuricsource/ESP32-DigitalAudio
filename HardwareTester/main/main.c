
#include "sdkconfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_log.h>

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
#define SAMPLE_BUFFER_SIZE 1024*2
#define SAMPLE_RATE 16000
// most microphones will probably default to left channel but you may need to tie the L/R pin low
#define I2S_MIC_CHANNEL I2S_CHANNEL_FMT_ONLY_LEFT
// either wire your microphone to the same pins or change these to match your wiring
#define I2S_MIC_SERIAL_CLOCK GPIO_NUM_2
#define I2S_MIC_LEFT_RIGHT_CLOCK GPIO_NUM_13
#define I2S_MIC_SERIAL_DATA GPIO_NUM_26
#define PIXEL_SIZE 4

// don't mess around with this
i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
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


i2s_config_t i2sSpeakerConfig = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 1024};

// i2s pins
i2s_pin_config_t i2sSpeakerPins = {
    .bck_io_num = GPIO_NUM_27,
    .ws_io_num = GPIO_NUM_12,
    .data_out_num = GPIO_NUM_25,
    .data_in_num = -1};

    int16_t raw_samples[SAMPLE_BUFFER_SIZE];


void app_main()
{
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &i2s_mic_pins);
    color_t green = hagl_color(0, 255, 0);
    ESP_LOGI(TAG, "Heap when starting: %d", esp_get_free_heap_size());
    hagl_init();
    ESP_LOGI(TAG, "Heap after HAGL init: %d", esp_get_free_heap_size());

    //install and start i2s driver
    i2s_driver_install(I2S_NUM_1, &i2sSpeakerConfig, 4, NULL);
    // set up the i2s pins
    i2s_set_pin(I2S_NUM_1, &i2sSpeakerPins);
    // clear the DMA buffers
    i2s_zero_dma_buffer(I2S_NUM_1);
    // start a task to write samples to the i2s peripheral


    while(true)
    {
        size_t bytes_read = 0;
        i2s_read(I2S_NUM_0, raw_samples, sizeof(int16_t) * SAMPLE_BUFFER_SIZE, &bytes_read, portMAX_DELAY);
        int samples_read = bytes_read / sizeof(int16_t);
        size_t bytes_read_return = 0;
        
        int16_t pData = DISPLAY_WIDTH / 2;
        uint8_t pY = 0;

        uint32_t total = 0;
        
        for (uint16_t i = 0; i < bytes_read; i ++)
            raw_samples[i] *= 16;

        while (bytes_read - total!= 0)
        {
            i2s_write(I2S_NUM_1, &raw_samples[total], bytes_read - total, &bytes_read_return, portMAX_DELAY);
            total += bytes_read_return;
            // ESP_LOGI(TAG, "total = %d, fps=%d\n", total, (int)fps());
        }
                                    // 240
        for (uint16_t y = 0; y < DISPLAY_HEIGHT; y = y + PIXEL_SIZE) 
        {
            int16_t data = ((raw_samples[y * 2 * 4]) / 10) + DISPLAY_WIDTH / 2;
            hagl_draw_line(pData, pY, data, y, green);
            pData = data;
            pY = y;
        }
        hagl_flush();
        hagl_clear_screen();
    }
}
