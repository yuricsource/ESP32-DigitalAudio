#include "I2sSpeaker.h"
#include "DebugAssert.h"

// #define SPEAKER_DEBUG

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

        Initialize();

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
                Play(&raw_samples[total], bytes_read - total, &bytes_read_return);
                total += bytes_read_return;
            }
        }
#endif /* SPEAKER_DEBUG */
    }

    I2sSpeaker::~I2sSpeaker()
    {

    }

    void I2sSpeaker::Play(int16_t *src, size_t size, size_t *bytes_written, TickType_t timeout)
    {
        i2s_write(_i2sPort, src, size, bytes_written, timeout);
        return;
    }

    bool I2sSpeaker::Initialize()
    {
        if (_initialized)
            return true;

        // Initializing the I2S driver
        esp_err_t error = i2s_driver_install(_i2sPort, &_config, 0, NULL);

        DebugAssertMessage(ESP_OK == error, "I2S speaker not initialized!");

        _initialized = (ESP_OK == error);
        i2s_set_pin(_i2sPort, &_i2sPins);

        return _initialized;
    }

    void I2sSpeaker::Deinitialize()
    {
        if (!_initialized)
            return;

        _initialized = false;
        i2s_driver_install(_i2sPort, &_config, 0, NULL);
    }

    void I2sSpeaker::Start()
    {
        i2s_start(_i2sPort);
    }

    void I2sSpeaker::Stop()
    {
        i2s_stop(_i2sPort);
    }

}