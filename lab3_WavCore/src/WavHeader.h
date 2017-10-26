//
//  WavHeader.h
//  WAV header structure
//  Laba3 OOP
//

#ifndef WavHeader_h
#define WavHeader_h

#include <cstdint>

struct WavHeader
{
    // Contains the letters "RIFF" in ASCII form
    // (0x52494646 big - endian form).
    int8_t chunkId[4];

    // 36 + SubChunk2Size, or more precisely :
    // 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
    // This is the size of the rest of the chunk
    // following this number.This is the size of the
    // entire file in bytes minus 8 bytes for the
    // two fields not included in this count :
    // ChunkID and ChunkSize.
    uint32_t chunkSize;

    // Contains the letters "WAVE"
    // (0x57415645 big - endian form).
    int8_t format[4];

    // The "WAVE" format consists of two subchunks : "fmt " and "data" :
    // The "fmt " subchunk describes the sound data's format:

    // Contains the letters "fmt "
    // (0x666d7420 big - endian form).
    int8_t subchunk1Id[4];

    // 16 for PCM.This is the size of the
    // rest of the Subchunk which follows this number.
    uint32_t subchunk1Size;

    // PCM = 1 (i.e.Linear quantization)
    // Values other than 1 indicate some
    // form of compression.
    uint16_t audioFormat;

    // Mono = 1, Stereo = 2, etc.
    uint16_t numChannels;

    // 8000, 44100, etc.
    uint32_t sampleRate;

    // == sampleRate * numChannels * bitsPerSample / 8
    uint32_t byteRate;

    // == NumChannels * BitsPerSample / 8
    // The number of bytes for one sample including
    // all channels.I wonder what happens when
    // this number isn't an integer?
    uint16_t blockAlign;

    // 8 bits = 8, 16 bits = 16, etc.
    uint16_t bitsPerSample;

    // The "data" subchunk contains the size of the data and the actual sound:

    // Contains the letters "data"
    // (0x64617461 big - endian form).
    int8_t subchunk2Id[4];

    // == NumSamples * NumChannels * BitsPerSample / 8
    // This is the number of bytes in the data.
    // You can also think of this as the size
    // of the read of the subchunk following this
    // number.
    uint32_t subchunk2Size;

    // The actual sound data.
};

#endif /* WavHeader_h */