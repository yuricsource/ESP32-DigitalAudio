#include "DigitalMicrophone.h"

namespace Hal
{

    DigitalMicrophone::DigitalMicrophone(uint32_t sampleRate, i2s_port_t i2sPort):
    _i2sPort(i2sPort)
    {
        _config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX);
        _config.sample_rate = sampleRate;
        _config.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
        _config.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;
        _config.communication_format = I2S_COMM_FORMAT_I2S;
        _config.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
        _config.dma_buf_count = 4;
        _config.dma_buf_len = 1024;
        _config.use_apll = false;
        _config.tx_desc_auto_clear = false;
        _config.fixed_mclk = 0;
    }

    DigitalMicrophone::~DigitalMicrophone()
    {

    }

    bool DigitalMicrophone::Start()
    {
        if (_initialized)
            return true;

        _initialized = (ESP_OK == i2s_driver_install(I2S_NUM_0, &_config, 0, NULL));

        return _initialized;
    }

    void DigitalMicrophone::Stop()
    {
        if (!_initialized)
            return;

        _initialized = false;
        i2s_driver_install(I2S_NUM_0, &_config, 0, NULL);
    }

}