#ifndef HAL_I2S_SPEAKER_H_
#define HAL_I2S_SPEAKER_H_

#include "driver/i2s.h" 

namespace Hal
{
    class I2sSpeaker
    {
        public:
            I2sSpeaker(gpio_num_t bkPin, gpio_num_t wsPin, gpio_num_t dataPin,
                        uint32_t sampleRate = SampleRate, i2s_port_t i2sPort = I2S_NUM_1);
            ~I2sSpeaker();
            bool Initialize();
            void Deinitialize();
            void Start();
            void Stop();
            void Play(int16_t *src, size_t size, size_t *bytes_written, TickType_t timeout = portMAX_DELAY);
        private:
            static constexpr uint32_t SampleRate = 16000; 
            i2s_config_t _config = {};
            i2s_port_t _i2sPort = I2S_NUM_1;
            i2s_pin_config_t _i2sPins = {};
            bool _initialized = false;

        private:
            /// @brief	Hide Copy constructor.
            I2sSpeaker(const I2sSpeaker &) = delete;

            /// @brief	Hide Assignment operator.
            I2sSpeaker &operator=(const I2sSpeaker &) = delete;

            /// @brief	Hide Move constructor.
            I2sSpeaker(I2sSpeaker &&) = delete;

            /// @brief	Hide Move assignment operator.
            I2sSpeaker &operator=(I2sSpeaker &&) = delete;
    };
}
#endif /* HAL_I2S_SPEAKER_H_ */