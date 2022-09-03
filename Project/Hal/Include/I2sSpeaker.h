#ifndef HAL_I2S_SPEAKER_H_
#define HAL_I2S_SPEAKER_H_

#include "driver/i2s.h" 

namespace Hal
{
    class I2sSpeaker
    {
        public:
            I2sSpeaker(gpio_num_t bkPin, gpio_num_t wsPin, gpio_num_t dataPin,
                        uint32_t sampleRate = DefaultSampleRate, i2s_port_t i2sPort = I2S_NUM_1);
            ~I2sSpeaker();
            bool Start();
            void Stop();
            size_t Play(uint16_t* buffer, size_t bufferSize, TickType_t timeout = portMAX_DELAY);
        private:
            static constexpr uint32_t DefaultSampleRate = 16000; 
            i2s_config_t _config = {};
            i2s_port_t _i2sPort = I2S_NUM_1;
            i2s_pin_config_t _i2sPins = {};
            bool _initialized = false;

        private:
            I2sSpeaker& operator=(const I2sSpeaker&) = delete; 
    };
}
#endif /* HAL_I2S_SPEAKER_H_ */