#include "DigitalMicrophone.h"

#ifdef MIC_DEBUG    
#include <hagl_hal.h>
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
#include <hagl.h>
#include "sdkconfig.h"

#define TAG "MIC"
#define SAMPLE_BUFFER_SIZE 1024
#define PIXEL_SIZE 4
#endif /* MIC_DEBUG */

namespace Hal
{
    DigitalMicrophone::DigitalMicrophone(gpio_num_t bkPin, gpio_num_t wsPin, gpio_num_t dataPin,
                                            uint32_t sampleRate, i2s_port_t i2sPort):
    _i2sPort(i2sPort)
    {
        // Configuring the I2S driver
        _config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX);
        _config.sample_rate = sampleRate;
        _config.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
        _config.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;
        _config.communication_format = I2S_COMM_FORMAT_STAND_I2S;
        _config.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
        _config.dma_buf_count = 4;
        _config.dma_buf_len = 1024;
        _config.use_apll = false;
        _config.tx_desc_auto_clear = false;
        _config.fixed_mclk = 0;

        // Configuring the pins
        _i2sPins.bck_io_num = bkPin;
        _i2sPins.ws_io_num = wsPin;
        _i2sPins.data_out_num = I2S_PIN_NO_CHANGE;
        _i2sPins.data_in_num = dataPin;

        Start();

#ifdef MIC_DEBUG
        color_t green = hagl_color(0, 255, 0);
        ESP_LOGI(TAG, "Heap when starting: %d", esp_get_free_heap_size());
        hagl_init();
        ESP_LOGI(TAG, "Heap after HAGL init: %d", esp_get_free_heap_size());

        static int16_t raw_samples[SAMPLE_BUFFER_SIZE];

        while(true)
        {
            size_t bytes_read = 0;
            bytes_read = GetAudioBuffer(raw_samples, sizeof(int16_t) * SAMPLE_BUFFER_SIZE);
            int samples_read = bytes_read / sizeof(int16_t);
            
            int16_t pData = DISPLAY_WIDTH / 2;
            uint8_t pY = 0;
            uint32_t unitStep = SAMPLE_BUFFER_SIZE / DISPLAY_HEIGHT;

            for (uint16_t i = 0; i < bytes_read; i ++)
            {
                raw_samples[i] = raw_samples[i] / 8;
            }
                                        // 240
            for (uint16_t y = 0; y < DISPLAY_HEIGHT; y += 1) 
            {
                int16_t data = (raw_samples[y * unitStep]) + DISPLAY_WIDTH / 2;
                hagl_draw_line(pData, pY, data, y, green);
                pData = data;
                pY = y;
            }
            hagl_flush();
            hagl_clear_screen();
            vTaskDelay(1);
        }
#endif /* MIC_DEBUG */
    }

    DigitalMicrophone::~DigitalMicrophone()
    {

    }

    size_t DigitalMicrophone::GetAudioBuffer(int16_t* buffer, size_t bufferSize, TickType_t timeout)
    {
        size_t bytes_read = 0;
        i2s_read(_i2sPort, buffer, bufferSize, &bytes_read, timeout);
        return bytes_read / sizeof(int16_t);
    }

    bool DigitalMicrophone::Start()
    {
        if (_initialized)
            return true;

        // Initializing the I2S driver
        _initialized = (ESP_OK == i2s_driver_install(_i2sPort, &_config, 0, NULL));
        i2s_set_pin(_i2sPort, &_i2sPins);

        return _initialized;
    }

    void DigitalMicrophone::Stop()
    {
        if (!_initialized)
            return;

        _initialized = false;
        i2s_driver_install(_i2sPort, &_config, 0, NULL);
    }

}