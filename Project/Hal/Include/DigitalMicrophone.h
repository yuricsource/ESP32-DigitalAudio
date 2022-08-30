#ifndef HAL_DIGITAL_MIC_H_
#define HAL_DIGITAL_MIC_H_

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

// // don't mess around with this
// i2s_config_t i2s_config = {
//     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
//     .sample_rate = SAMPLE_RATE,
//     .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
//     .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
//     .communication_format = I2S_COMM_FORMAT_I2S,
//     .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
//     .dma_buf_count = 4,
//     .dma_buf_len = 1024,
//     .use_apll = false,
//     .tx_desc_auto_clear = false,
//     .fixed_mclk = 0};

// // and don't mess around with this
// i2s_pin_config_t i2s_mic_pins = {
//     .bck_io_num = I2S_MIC_SERIAL_CLOCK,
//     .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,
//     .data_out_num = I2S_PIN_NO_CHANGE,
//     .data_in_num = I2S_MIC_SERIAL_DATA};

// static const char *TAG = "main";


// i2s_config_t i2sSpeakerConfig = {
//     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
//     .sample_rate = SAMPLE_RATE,
//     .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
//     .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
//     .communication_format = I2S_COMM_FORMAT_I2S,
//     .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
//     .dma_buf_count = 4,
//     .dma_buf_len = 1024};

// // i2s pins
// i2s_pin_config_t i2sSpeakerPins = {
//     .bck_io_num = GPIO_NUM_27,
//     .ws_io_num = GPIO_NUM_12,
//     .data_out_num = GPIO_NUM_25,
//     .data_in_num = -1};

//     int16_t raw_samples[SAMPLE_BUFFER_SIZE];

namespace Hal
{
    class DigitalMicrophone
    {
        public:
            DigitalMicrophone(uint32_t sampleRate = DefaultSampleRate, i2s_port_t i2sPort = I2S_NUM_0);
            ~DigitalMicrophone();
            bool Start();
            void Stop();
        private:
            static constexpr uint32_t DefaultSampleRate = 16000; 
            i2s_config_t _config = {};
            i2s_port_t _i2sPort = I2S_NUM_0;
            bool _initialized = false;
    };
}

#endif /* HAL_DIGITAL_MIC_H_ */