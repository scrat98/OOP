//
//  WavReader.hpp
//  Wav core
//  Laba3 OOP
//

#ifndef WavReader_hpp
#define WavReader_hpp

#include "WavHeader.h"
#include "WavException.h"
#include <vector>
#include <cstdint>
#include <string>
#include <sstream>
#include <fstream>
#include <istream>
#include <iterator>

#define READ_BYTES(content, arr) content.read(reinterpret_cast<char*>(&arr), sizeof(arr))
#define WRITE_BYTES(content, arr) content.write(reinterpret_cast<char*>(&arr), sizeof(arr))

#define COMPARE_HEADER_STR(headerData, str) strncmp(reinterpret_cast<char*>(&headerData), str, sizeof(headerData))

using byte = int8_t;

class WavReader
{
public:
    // ======================== Constructor ===========================
    /* Default constructor */
    WavReader();

    /* Constructor with file */
    WavReader(const std::string& filePath);

    /* Constructor with content */
    WavReader(std::istream& content);

    /* Destructor */
    ~WavReader();

    // ============ get some parameters ==============================

    /* @Return WavHeader from audio */
    WavHeader getHeader() const;

    /* @Return audio byte data */
    std::vector<int8_t> getData() const;

    /* @Return the number of audio channels */
    uint16_t getNumChannels() const;

    /* @Return the sample rate */
    uint32_t getSampleRate() const;

    /* @Return the number of samples per channel */
    int getNumSamplesPerChannel() const;

    /* @Return the byte rate */
    uint32_t getByteRate() const;

    /* @Return the block align - the number of bytes for one sample for all channels*/
    uint16_t getBlockAlign() const;

    /* @Return the bit depth - the number of bits in one sample */
    uint16_t getBitDepth() const;

    /* @Return the audio format PCM = 1 */
    uint16_t getAudioFormat() const;

    /* @Return the audio data size */
    uint32_t getDataSize() const;

    /* @Return the file size */
    uint32_t getFileSize() const;

    /* @Return the length in seconds of the audio file based on the number of samples and sample rate */
    double getLengthInSeconds() const;

    /* @Return true if the audio file is mono */
    bool isMono() const;

    /* @Return true if the audio file is stereo */
    bool isStereo() const;

    /* @Return a string summary of the audio file */
    std::string printSummary() const;

    // ============= create, update, delete functions with file ============================

    /* Loads an audio file from a given file path */
    void load(const std::string& filePath);

    /* Loads an audio file from a given content */
    void load(std::istream& content);

    /* Save an audio file in current audio file */
    void save();

    /* Save an audio file in a given file path */
    void save(const std::string& filePath);

    /* Clear audio data */
    void clearAudioData();

    /* Reset audio header. Set header default values */
    void resetAudioHeader();

    /* Clear audio data and reset audio header */
    void clear();

    // ============== channels functions =============================
    void setNumChannels(uint16_t numChannels);
    void makeMono();
    void makeStereo();

    // ============= reverbiration functions =============================
    // TODO: makeReverb();
    // TODO: makeSomeEffects();

    // ======================= cut functions =============================
    void cut();
    void cutFromBegin();
    void cutFromEnd();

    // ===================== sound tone functions ========================
    // TODO: changeToneOfVoice();
    // TODO: makeSomeEffects();

    // =================== sound and frequency modulation ================
    // TODO: soundModulation();
    // TODO: frequencyModulation();
    // TODO: makeCleanAudio();

    // =================== set audio settings functions ==========================
    void setAudioBuffer();
    void setNumSamplesPerChannel(uint16_t numSamples);
    void setBitDepth(uint16_t numBitsPerSample);
    void setSampleRate(uint32_t newSampleRate);

    // ==================== Default values for header =============================
    static const uint16_t DEFAULT_NUM_CAHNNELS = 2;
    static const uint32_t DEFAULT_SAMPLE_RATE = 44100;
    static const uint16_t DEFAULT_BIT_DEPTH = 16;
    static const uint16_t DEFAULT_AUDIO_FORMAT = 1;
    static const uint32_t DEFAULT_SUBCHUNK1_SIZE = 16;
    static const uint32_t HEADER_SIZE = sizeof(WavHeader);
    static const uint32_t BYTE_SIZE = 8;

private:
    WavHeader wavHeader;
    std::vector<byte> data;
    std::string fileName = "";

    // ====================== private functions ====================================
    /* Read header from content */
    void readHeader(std::istream& content);
    void writeHeader(std::ostream& content);

    /* Read audio data to vector content */
    void readData(std::istream& content);
    void writeData(std::ostream& content);

    /* Throw exception if header doesn't ok */
    void isHeaderCorrect(int32_t fileSize);

    /* Get content size in bytes */
    int getContentSize(std::istream& content);
};

#endif /* WavReader_hpp */