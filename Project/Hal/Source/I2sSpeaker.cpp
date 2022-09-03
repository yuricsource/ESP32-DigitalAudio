#include "I2sSpeaker.h"

namespace Hal
{
    I2sSpeaker::I2sSpeaker(gpio_num_t bkPin, gpio_num_t wsPin, gpio_num_t dataPin,
                            uint32_t sampleRate, i2s_port_t i2sPort):
    _i2sPort(i2sPort)
    {
        // Configuring I2S driver
        _config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
        _config.sample_rate = sampleRate;
        _config.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
        _config.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;
        _config.communication_format = I2S_COMM_FORMAT_STAND_I2S;
        _config.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
        _config.dma_buf_count = 4;
        _config.dma_buf_len = 1024;

        // Configuring I2S Pins
        _i2sPins.bck_io_num = bkPin;
        _i2sPins.ws_io_num = wsPin;
        _i2sPins.data_out_num = dataPin;
        _i2sPins.data_in_num = -1;

#ifdef SPEAKER_DEBUG
        static int16_t raw_samples[1024];
        for(;;)
        {
            size_t bytes_read = 0;
            i2s_read(I2S_NUM_0, raw_samples, sizeof(int16_t) * 1024, &bytes_read, portMAX_DELAY);
            size_t bytes_read_return = 0;
            uint32_t total = 0;

            while (bytes_read - total!= 0)
            {
                i2s_write(I2S_NUM_1, &raw_samples[total], bytes_read - total, &bytes_read_return, portMAX_DELAY);
                total += bytes_read_return;
            }
        }
#endif /* SPEAKER_DEBUG */
    }

    I2sSpeaker::~I2sSpeaker()
    {

    }

    size_t I2sSpeaker::Play(uint16_t* buffer, size_t bufferSize, TickType_t timeout)
    {
        size_t bytes_read = 0;
        i2s_read(_i2sPort, buffer, bufferSize, &bytes_read, timeout);
        return bytes_read / sizeof(int16_t);
    }

    bool I2sSpeaker::Start()
    {
        if (_initialized)
            return true;

        // Initializing the I2S driver
        _initialized = (ESP_OK == i2s_driver_install(_i2sPort, &_config, 0, NULL));
        i2s_set_pin(_i2sPort, &_i2sPins);

        return _initialized;
    }

    void I2sSpeaker::Stop()
    {
        if (!_initialized)
            return;

        _initialized = false;
        i2s_driver_install(_i2sPort, &_config, 0, NULL);
    }

}