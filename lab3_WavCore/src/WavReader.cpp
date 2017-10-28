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

std::vector<byte> WavReader::getByteOfData() const
{
    return data;
}

std::vector<std::vector<int64_t>> WavReader::getAudioBuffer() const
{
	std::vector<std::vector<int64_t>> buffer;
	uint16_t numChannels = getNumChannels();
	uint64_t numSamplesPerChannel = getNumSamplesPerChannel();
	buffer.resize(numChannels);

	for (size_t i = 0; i < numSamplesPerChannel; i++) {
		auto sample = getSample(i);
		for (size_t ch = 0; ch < sample.size(); ch++) {
			buffer[ch].push_back(sample[ch]);
		}
	}

	return buffer;
}

std::vector<int64_t> WavReader::getSample(uint64_t i) const
{
	std::vector<int64_t> sample;
	uint16_t sampleSize = getBitDepth() / WavReader::BYTE_SIZE;
	uint32_t blockAlign = getBlockAlign();

	uint64_t start = blockAlign * i;
	uint64_t end = start + blockAlign;

	for (uint64_t pos = start; pos < end; pos += sampleSize) {
		sample.push_back(bytesToSample(pos, sampleSize));
	}

	return sample;
}

uint16_t WavReader::getNumChannels() const
{
   return wavHeader.numChannels;
}

uint32_t WavReader::getSampleRate() const
{
    return wavHeader.sampleRate;
}

uint64_t WavReader::getNumSamplesPerChannel() const
{
    return getDataSize() / getNumChannels() / getBitDepth() * WavReader::BYTE_SIZE;
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
    // TODO: setNumChannels(WavReader::DEFAULT_NUM_CAHNNELS);
	wavHeader.numChannels = WavReader::DEFAULT_NUM_CAHNNELS;

    // TODO: setSampleRate(WavReader::DEFAULT_SAMPLE_RATE);
	wavHeader.sampleRate = WavReader::DEFAULT_SAMPLE_RATE;

    // TODO: setBitDepth(WavReader::DEFAULT_BIT_DEPTH);
	wavHeader.bitsPerSample = WavReader::DEFAULT_BIT_DEPTH;

    // Reset chunk
    std::memcpy(wavHeader.chunkId, "RIFF", sizeof(wavHeader.chunkId));
    wavHeader.chunkSize = data.size() * WavReader::BYTE_SIZE + WavReader::HEADER_SIZE - sizeof(wavHeader.chunkId) - sizeof(wavHeader.chunkSize);

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
// TODO: void WavReader::setNumChannels(uint16_t numChannels) {}

void WavReader::makeMono()
{
	if (getNumChannels() != 2)
		throw BadParams("In order to make mono audio must be stereo");

	if (getNumChannels() == 1)
		return;

	std::vector<std::vector<int64_t>> tmp;
	tmp.resize(1);
	for (uint64_t i = 0; i < getNumSamplesPerChannel(); i++) {
		auto sample = getSample(i);
		tmp[0].push_back((sample[0] + sample[1]) / 2);
	}
	setAudioBuffer(tmp);

	wavHeader.numChannels = 1;
	wavHeader.byteRate = getBitDepth() * getSampleRate() *  getNumChannels() / WavReader::BYTE_SIZE;
	wavHeader.blockAlign = getBitDepth() * getNumChannels() / WavReader::BYTE_SIZE;
	wavHeader.chunkSize = data.size() + WavReader::HEADER_SIZE - sizeof(wavHeader.chunkId) - sizeof(wavHeader.chunkSize);
	wavHeader.subchunk2Size = data.size();
}

void WavReader::makeStereo()
{
	if (getNumChannels() != 1)
		throw BadParams("In order to make stereo audio must be mono");

	if (getNumChannels() == 2)
		return;

	wavHeader.numChannels = 2;
	wavHeader.sampleRate /= 2;
	wavHeader.byteRate = getBitDepth() * getSampleRate() *  getNumChannels() / WavReader::BYTE_SIZE;
	wavHeader.blockAlign = getBitDepth() * getNumChannels() / WavReader::BYTE_SIZE;
	wavHeader.chunkSize = data.size() + WavReader::HEADER_SIZE - sizeof(wavHeader.chunkId) - sizeof(wavHeader.chunkSize);
	wavHeader.subchunk2Size = data.size();
}

// ============= reverbiration functions =============================
// TODO: WavReader::makeReverb() {}
// TODO: WavReader::makeAudioEffects() {}

// ======================= cut functions =============================
void WavReader::cut(double from, double to)
{
	uint16_t sampleSize = getBitDepth() / WavReader::BYTE_SIZE;
	uint64_t start = from * getSampleRate() * sampleSize;
	uint64_t end = to * getSampleRate() * sampleSize;
	
	data.erase(data.begin() + start, data.begin() + end);

	wavHeader.chunkSize = data.size() + WavReader::HEADER_SIZE - sizeof(wavHeader.chunkId) - sizeof(wavHeader.chunkSize);
	wavHeader.subchunk2Size = data.size();
}

void WavReader::cutFromBegin(double time)
{
	uint64_t samples = time * getSampleRate();
	uint16_t sampleSize = getBitDepth() / WavReader::BYTE_SIZE;

	data.erase(data.begin(), data.begin() + samples * sampleSize);

	wavHeader.chunkSize = data.size() + WavReader::HEADER_SIZE - sizeof(wavHeader.chunkId) - sizeof(wavHeader.chunkSize);
	wavHeader.subchunk2Size = data.size();
}

void WavReader::cutFromEnd(double time)
{
	uint64_t samples = time * getSampleRate();
	uint16_t sampleSize = getBitDepth() / WavReader::BYTE_SIZE;

	data.erase(data.end() - samples * sampleSize, data.end());

	wavHeader.chunkSize = data.size() + WavReader::HEADER_SIZE - sizeof(wavHeader.chunkId) - sizeof(wavHeader.chunkSize);
	wavHeader.subchunk2Size = data.size();
}

// ===================== sound tone functions ========================
// TODO: WavReader::changeToneOfVoice() {}
// TODO: WavReader::makeSomeEffects() {}

// =================== sound and frequency modulation ================
// TODO: WavReader::soundModulation() {}
// TODO: WavReader::frequencyModulation() {}
// TODO: WavReader::makeCleanAudio() {}
// TODO: WavReader::speedUp() {}
// TODO: WavReader::slowDown() {}

// =================== set audio settings functions ==========================
void WavReader::setAudioBuffer(std::vector<std::vector<int64_t>> buffer)
{
	if (!buffer.size())
		throw BadParams("Buffer can't be empty");

	uint64_t numSamplesPerChannel = buffer[0].size();
	for (uint64_t ch = 0; ch < buffer.size(); ch++)
		if (buffer[ch].size() != numSamplesPerChannel)
			throw BadParams("All channels in buffer must have the same sizes");

	clearAudioData();
	for (uint64_t sample = 0; sample < numSamplesPerChannel; sample++) {
		for (uint64_t ch = 0; ch < buffer.size(); ch++) {
			addSampleToByteData(buffer[ch][sample]);
		}
	}
}

// TODO: void WavReader::setAudioBuffer() {}
// TODO: void WavReader::setNumSamplesPerChannel(uint16_t numSamples) {}
// TODO: void WavReader::setBitDepth(uint16_t numBitsPerSample) {}
// TODO: void WavReader::setSampleRate(uint32_t newSampleRate) {}

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

void WavReader::isHeaderCorrect(uint32_t fileSize)
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

	if (wavHeader.bitsPerSample != 8 && wavHeader.bitsPerSample != 16 &&
		wavHeader.bitsPerSample != 32 && wavHeader.bitsPerSample != 64)
		throw HeaderError("Bit depth must be 8, 16, 32 or 64 bit");

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

uint32_t WavReader::getContentSize(std::istream& content) const
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

uint64_t WavReader::bytesToSample(uint64_t start, uint16_t sampleSize) const
{
	int64_t sample;

	if (sampleSize == 1) {
		int8_t tmp = data[start];
		sample = tmp;
	}

	if (sampleSize == 2) {
		int16_t tmp = (data[start + 1] << 8 | data[start]);
		sample = tmp;
	}

	if (sampleSize == 3) {
		int32_t tmp = (data[start + 2] << 16 | data[start + 1] << 8 | data[start]);
		sample = tmp;
	}

	if (sampleSize == 4) {
		int64_t tmp = (data[start + 3] << 24 | data[start + 2] << 16 | data[start + 1] << 8 | data[start]);
		sample = tmp;
	}

	return sample;
}

void WavReader::addSampleToByteData(int64_t sample)
{
	uint16_t sampleSize = getBitDepth() / WavReader::BYTE_SIZE;

	for(uint16_t byte = 0; byte < sampleSize; byte++)
		data.push_back(GET_NTH_BYTE_OF_NUMBER(sample, byte));
}