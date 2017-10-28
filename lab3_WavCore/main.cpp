#include <iostream>
#include "WavReader.hpp"

int main()
{
	try {
		WavReader wav("../media/molly.wav");
		std::cout << wav.printSummary() << std::endl;
		wav.makeMono();
		wav.save("../media/molly_out.wav");
		auto buffer = wav.getAudioBuffer();

		WavReader wav2("../media/molly_out.wav");
		std::cout << wav2.printSummary() << std::endl;
		int sampleRate = wav2.getSampleRate();
		int chanCount = wav2.getNumChannels();
		bool isStereo = wav2.isStereo();
		// Cut first 10 seconds and last 20 seconds
		wav2.cutFromBegin(10.0);
		wav2.cutFromEnd(20.0);
		wav2.makeStereo();
		wav2.save("../media/molly_out_out.wav");
		std::cout << wav2.printSummary() << std::endl;
	}
	catch (WavReaderException& we) {
		std::cerr << we.what();
		system("pause");
	}

	system("pause");
}