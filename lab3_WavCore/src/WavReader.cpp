#include "WavReader.hpp"

WavReader::WavReader() 
{
    clear();
}

WavReader::WavReader(const std::string& filePath)
{
    load(filePath);
}

WavReader::WavReader(std::istream& content)
{
    load(content);
}

WavReader::~WavReader() {}

// ============ get some parameters ==============================

WavHeader WavReader::getHeader() const
{
    return wavHeader;
}

std::vector<int8_t> WavReader::getData() const
{
    return data;
}

uint16_t WavReader::getNumChannels() const
{
   return wavHeader.numChannels;
}

uint32_t WavReader::getSampleRate() const
{
    return wavHeader.sampleRate;
}

int WavReader::getNumSamplesPerChannel() const
{
    return getDataSize() / getNumChannels();
}

uint32_t WavReader::getByteRate() const
{
    return wavHeader.byteRate;
}

uint16_t WavReader::getBlockAlign() const
{
    return wavHeader.blockAlign;
}

uint16_t WavReader::getBitDepth() const
{
    return wavHeader.bitsPerSample;
}

uint16_t WavReader::getAudioFormat() const
{
    return wavHeader.audioFormat;
}

uint32_t WavReader::getDataSize() const
{
    return wavHeader.subchunk2Size;
}

uint32_t WavReader::getFileSize() const
{
    return wavHeader.chunkSize + sizeof(wavHeader.chunkSize) + sizeof(wavHeader.chunkId);
}

double WavReader::getLengthInSeconds() const
{
    return (double)getNumSamplesPerChannel() / (double)getSampleRate();
}

bool WavReader::isMono() const
{
    return getNumChannels() == 1;
}

bool WavReader::isStereo() const
{
    return getNumChannels() == 2;
}

std::string WavReader::printSummary() const
{
    std::stringstream out;

    out << "-----------------------------" << std::endl;
    out << "audioFormat       " << getAudioFormat() << std::endl;
    out << "numChannels       " << getNumChannels() << std::endl;
    out << "sampleRate        " << getSampleRate() << std::endl;
    out << "bitsPerSample     " << getBitDepth() << std::endl;
    out << "byteRate          " << getByteRate() << std::endl;
    out << "LengthInSeconds   " << getLengthInSeconds() << std::endl;
    out << "SamplesPerChannel " << getNumSamplesPerChannel() << std::endl;
    out << "-----------------------------" << std::endl;

    return out.str();
}

// ============= create and save file ============================
void WavReader::load(const std::string& filePath)
{
    std::ifstream file(filePath, std::ios::binary);

    if (!file.good())
        throw FileError("File can't be opened");

    fileName = filePath;
    load(file);
}

void WavReader::load(std::istream& content)
{
    content.flags(std::ios::binary);

    if (!content)
        throw FileError("Content can't be opened");

    readHeader(content);
    isHeaderCorrect(getContentSize(content)); // throw exception if header doesn't ok
    readData(content);
}

void WavReader::save()
{
    save(fileName);
}

void WavReader::save(const std::string& filePath)
{
    int32_t fileSize = data.size() * sizeof(byte) + WavReader::HEADER_SIZE;
    isHeaderCorrect(fileSize); // throw exception if header doesn't ok

    std::ofstream outputFile(filePath, std::ios::binary);    
    writeHeader(outputFile);
    writeData(outputFile);
}

void WavReader::clearAudioData()
{
    data.clear();
}

void WavReader::resetAudioHeader()
{
    // set settings
    setNumChannels(WavReader::DEFAULT_NUM_CAHNNELS);
    setSampleRate(WavReader::DEFAULT_SAMPLE_RATE);
    setBitDepth(WavReader::DEFAULT_BIT_DEPTH);

    // Reset chunk
    std::memcpy(wavHeader.chunkId, "RIFF", sizeof(wavHeader.chunkId));
    wavHeader.chunkSize = data.size() * WavReader::BYTE_SIZE + 36; // more information in WavHeader.h

    // Reset format to "WAVE"
    std::memcpy(wavHeader.format, "WAVE", sizeof(wavHeader.format));

    // Reset subchunk1
    std::memcpy(wavHeader.subchunk1Id, "fmt ", sizeof(wavHeader.subchunk1Id));
    wavHeader.subchunk1Size = WavReader::DEFAULT_SUBCHUNK1_SIZE;
    wavHeader.audioFormat = WavReader::DEFAULT_AUDIO_FORMAT;
    wavHeader.byteRate = WavReader::DEFAULT_BIT_DEPTH * WavReader::DEFAULT_NUM_CAHNNELS * WavReader::DEFAULT_SAMPLE_RATE / WavReader::BYTE_SIZE;
    wavHeader.blockAlign = WavReader::DEFAULT_BIT_DEPTH * WavReader::DEFAULT_NUM_CAHNNELS / WavReader::BYTE_SIZE;

    // Reset subchunk2
    std::memcpy(wavHeader.subchunk2Id, "data", sizeof(wavHeader.subchunk2Id));
    wavHeader.subchunk2Size = data.size() * WavReader::BYTE_SIZE;
}

void WavReader::clear()
{
    clearAudioData();
    resetAudioHeader();
}

// ============== channels functions =============================
void WavReader::setNumChannels(uint16_t numChannels)
{
    
}

void WavReader::makeMono()
{
    setNumChannels(1);
}

void WavReader::makeStereo()
{
    setNumChannels(2);
}

// ============= reverbiration functions =============================
// TODO

// ======================= cut functions =============================
void WavReader::cut()
{

}

void WavReader::cutFromBegin()
{

}

void WavReader::cutFromEnd()
{

}

// ===================== sound tone functions ========================
// TODO

// =================== sound and frequency modulation ================
// TODO

// =================== set audio settings functions ==========================
void WavReader::setAudioBuffer()
{

}

void WavReader::setNumSamplesPerChannel(uint16_t numSamples)
{

}

void WavReader::setBitDepth(uint16_t numBitsPerSample)
{

}

void WavReader::setSampleRate(uint32_t newSampleRate)
{

}

// ====================== Private functions ========================
void WavReader::readHeader(std::istream& content)
{
    content.flags(std::ios::binary);

    READ_BYTES(content, wavHeader.chunkId);
    READ_BYTES(content, wavHeader.chunkSize);
    READ_BYTES(content, wavHeader.format);
    READ_BYTES(content, wavHeader.subchunk1Id);
    READ_BYTES(content, wavHeader.subchunk1Size);
    READ_BYTES(content, wavHeader.audioFormat);
    READ_BYTES(content, wavHeader.numChannels);
    READ_BYTES(content, wavHeader.sampleRate);
    READ_BYTES(content, wavHeader.byteRate);
    READ_BYTES(content, wavHeader.blockAlign);
    READ_BYTES(content, wavHeader.bitsPerSample);
    READ_BYTES(content, wavHeader.subchunk2Id);
    READ_BYTES(content, wavHeader.subchunk2Size);
}

void WavReader::writeHeader(std::ostream& content)
{
    WRITE_BYTES(content, wavHeader);
}

void WavReader::readData(std::istream& content)
{
    content.flags(std::ios::binary);

    content.seekg(WavReader::HEADER_SIZE, std::ios::beg);
    data.insert(data.begin(),
        std::istreambuf_iterator<char>(content),
        std::istreambuf_iterator<char>());
}

void WavReader::writeData(std::ostream& content)
{
    content.write(reinterpret_cast<char*>(&data[0]), data.size() * sizeof(byte));
}

void WavReader::isHeaderCorrect(int32_t fileSize)
{
    // Go to WavHeader.h for details
    uint32_t dataSize = fileSize - WavReader::HEADER_SIZE;

    if (COMPARE_HEADER_STR(wavHeader.chunkId, "RIFF") != 0)
        throw HeaderError("HEADER_RIFF_ERROR");

    if (wavHeader.chunkSize != fileSize - sizeof(wavHeader.chunkId) - sizeof(wavHeader.chunkSize))
        throw DataSizeError("HEADER_FILE_SIZE_ERROR");

    if (COMPARE_HEADER_STR(wavHeader.format, "WAVE") != 0)
        throw HeaderError("HEADER_WAVE_ERROR");

    if (COMPARE_HEADER_STR(wavHeader.subchunk1Id, "fmt ") != 0)
        throw HeaderError("HEADER_FMT_ERROR");

    if (wavHeader.audioFormat != 1)
        throw UnsupportedFormat("HEADER_NOT_PCM");

    if (wavHeader.subchunk1Size != 16)
        throw UnsupportedFormat("HEADER_SUBCHUNK1_ERROR");

    if (wavHeader.byteRate != wavHeader.sampleRate * wavHeader.numChannels * wavHeader.bitsPerSample / WavReader::BYTE_SIZE)
        throw HeaderError("HEADER_BYTES_RATE_ERROR");

    if (wavHeader.blockAlign != wavHeader.numChannels * wavHeader.bitsPerSample / WavReader::BYTE_SIZE)
        throw HeaderError("HEADER_BLOCK_ALIGN_ERROR");

    if (COMPARE_HEADER_STR(wavHeader.subchunk2Id, "data") != 0)
        throw HeaderError("HEADER_DATA_ERROR");

    if (wavHeader.subchunk2Size != dataSize)
        throw HeaderError("HEADER_SUBCHUNK2_SIZE_ERROR");

    if (dataSize % wavHeader.numChannels != 0)
        throw DataSizeError("Channels don't have the same number of samples");
}

int WavReader::getContentSize(std::istream& content)
{
    // save current position
    int currentPos = content.tellg();

    // get its size
    content.seekg(0, std::ios::end);
    int fileSize = content.tellg();

    // return current position
    content.seekg(currentPos, std::ios::beg);

    return fileSize;
}