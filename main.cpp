#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "SoundProccFunc.h"
int main(int argc, char* argv[]) {
	
	
	SoundFunc soundFunc;

	int bytes_per_sample = (16 / 8) * 1;
	int sample_rate = 44100;


	int start_ind = (sample_rate * sound.start_second * bytes_per_sample);
	int end_ind = (sample_rate * sound.end_second * bytes_per_sample);

	if (argc == 4) {
		FILE* file = fopen(argv[1], "r+b");
		sound.start_second = atoi(argv[2]);
		sound.end_second = atoi(argv[3]);
		
		int start_ind = (sample_rate * sound.start_second * bytes_per_sample);
		int end_ind = (sample_rate * sound.end_second * bytes_per_sample);
		
		std::cout << "Muting from " << sound.start_second << "s to " << sound.end_second << "s" << std::endl;
		std::cout << "Byte range: " << end_ind << " - " << start_ind << std::endl;
		soundFunc.mute(file, start_ind, end_ind);

		fclose(file);
	}
	
	if (argc==5){
		FILE* file1 = fopen(argv[1], "r+b");
		FILE* file2 = fopen(argv[2], "r+b");
		FILE* output = fopen("test.wav", "wb");
		sound.start_second = atoi(argv[3]);
		sound.end_second = atoi(argv[4]);

		long start_ind = (sample_rate * sound.start_second * bytes_per_sample);
		long end_ind = (sample_rate * sound.end_second * bytes_per_sample);
		
		std::cout << "Mix file " << argv[1] << "with " << argv[2] << "from " << atoi(argv[3]) << "to " << atoi(argv[4]);
		soundFunc.mix(file1, file2, output, start_ind, end_ind);
		fclose(file1);
		fclose(file2);
		fclose(output);
	}
	
	return 0;
}