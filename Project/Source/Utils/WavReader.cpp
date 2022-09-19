#include "WavReader.h"
#include "Logger.h"

namespace Utilities
{

typedef struct
{
    // RIFF Header
    char riff_header[4]; // Contains "RIFF"
    int wav_size;        // Size of the wav portion of the file, which follows the first 8 bytes. File size - 8
    char wave_header[4]; // Contains "WAVE"

    // Format Header
    char fmt_header[4]; // Contains "fmt " (includes trailing space)
    int fmt_chunk_size; // Should be 16 for PCM
    short audio_format; // Should be 1 for PCM. 3 for IEEE Float
    short num_channels;
    int sample_rate;
    int byte_rate;          // Number of bytes per second. sample_rate * num_channels * Bytes Per Sample
    short sample_alignment; // num_channels * Bytes Per Sample
    short bit_depth;        // Number of bits per sample

    // Data
    char data_header[4]; // Contains "data"
    int data_bytes;      // Number of bytes in data. Number of samples * num_channels * sample byte size
    // uint8_t bytes[]; // Remainder of wave file is bytes
} WavHeader;

WavReader::WavReader(const char *fileName)
{
    bool validFile = true;
    _file = fopen(fileName, "r");
    
    if (!_file)
    {
        Logger::LogError(Logger::LogSource::System, "File not found %s", fileName);
        return;
    }
    
    // Read the WAV header
    WavHeader wav_header;
    fread((uint8_t *)&wav_header, sizeof(WavHeader), 1, _file);

    // Check the bit depth
    if (wav_header.bit_depth != 16)
    {
        Logger::LogError(Logger::LogSource::System, "Bit depth %d is not supported please use 16 bit signed integer\n", wav_header.bit_depth);
        validFile = false;
    }

    if (wav_header.sample_rate != 16000)
    {
        Logger::LogError(Logger::LogSource::System, "Bit depth %d is not supported please us 16KHz\n", wav_header.sample_rate);
        validFile = false;
    }

    if (!validFile && _file != nullptr)
        fclose(_file);

}

WavReader::~WavReader()
{
    if (_file != nullptr)
        fclose(_file);
}

void WavReader::Reset()
{
    // Seek to the start of the wav data
    fseek(_file, 44, SEEK_SET);
}

bool WavReader::Available()
{
    return _file != nullptr;
}

size_t WavReader::Read(int16_t* buffer, size_t length)
{
    if (_file == nullptr)
    {
        Logger::LogError(Logger::LogSource::System, "Invalid File");
        return 0;
    }
    size_t read = fread((uint8_t*)buffer, 1, length * sizeof(int16_t), _file);
    return read;
}

}