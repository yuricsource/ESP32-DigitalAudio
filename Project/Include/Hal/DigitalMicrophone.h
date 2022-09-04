#ifndef HAL_DIGITAL_MIC_H_
#define HAL_DIGITAL_MIC_H_

#include "driver/i2s.h" 
#include "HalCommon.h"

namespace Hal
{
    class DigitalMicrophone
    {
        public:
            DigitalMicrophone(gpio_num_t bkPin, gpio_num_t wsPin, gpio_num_t dataPin, uint32_t sampleRate = DefaultSampleRate, i2s_port_t i2sPort = I2S_NUM_0);
            ~DigitalMicrophone();
            size_t GetAudioBuffer(int16_t* buffer, size_t bufferSize, TickType_t timeout = portMAX_DELAY);
            bool Start();
            void Stop();
        private:
            static constexpr uint32_t DefaultSampleRate = 16000; 
            i2s_config_t _config = {};
            i2s_port_t _i2sPort = I2S_NUM_0;
            i2s_pin_config_t _i2sPins = {};
            bool _initialized = false;

        private:
            /// @brief	Hide Copy constructor.
            DigitalMicrophone(const DigitalMicrophone &) = delete;

            /// @brief	Hide Assignment operator.
            DigitalMicrophone &operator=(const DigitalMicrophone &) = delete;

            /// @brief	Hide Move constructor.
            DigitalMicrophone(DigitalMicrophone &&) = delete;

            /// @brief	Hide Move assignment operator.
            DigitalMicrophone &operator=(DigitalMicrophone &&) = delete;
    };
}

#endif /* HAL_DIGITAL_MIC_H_ */