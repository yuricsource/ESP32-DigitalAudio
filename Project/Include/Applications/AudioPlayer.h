#pragma once

#include "Hardware.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "thread.hpp"
#include "TimeLimit.h"
#include "Logger.h"
#include "CircularBuffer.h"
#include "AudioRequest.h"
#include "queue.hpp"

namespace Applications
{
using Utilities::Logger;
using AudioInterfaces::AudioRequest;
using cpp_freertos::Queue;

class AudioPlayer : public cpp_freertos::Thread
{
public:
    AudioPlayer();
    void PlayFile(const char* fileName, uint8_t volume = 100);
private:

    class AudioFileRequest
    {
        public:
        Common::Name Name;
        uint8_t Volume;
    };

    static constexpr uint16_t BufferSize = 128;
    bool playAudioFile(Common::Name& audioRequest, uint8_t volume = 100);
    static constexpr uint8_t QueueLength = 6;
    Queue _audioList;
protected:
    void Run() override;

private:
    /// @brief	Hide Copy constructor.
    AudioPlayer(const AudioPlayer &) = delete;

    /// @brief	Hide Assignment operator.
    AudioPlayer &operator=(const AudioPlayer &) = delete;

    /// @brief	Hide Move constructor.
    AudioPlayer(AudioPlayer &&) = delete;

    /// @brief	Hide Move assignment operator.
    AudioPlayer &operator=(AudioPlayer &&) = delete;
};
static_assert(sizeof(AudioPlayer) == 56, "Wrong Size, compilation problem.");
} // namespace Applications