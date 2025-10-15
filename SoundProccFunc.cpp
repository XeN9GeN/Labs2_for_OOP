#include "SoundProccFunc.h"
#include <iostream>

void mute(const std::string& input_file, const std::string& output_file, long start_second, long end_second) {
	FILE* input = fopen(input_file.c_str(), "rb");
    FILE* output = fopen(output_file.c_str(), "wb");

	uint8_t header[44];
    fread(header, sizeof(uint8_t), 44, input);
    fwrite(header, sizeof(uint8_t), 44, output);

    int num_channel = 1;
	int bytes_per_sample = (16 / 8) * num_channel;
	int sample_rate = 44100;
	int bytes_per_second = sample_rate * bytes_per_sample;

	long start_byte = sample_rate * start_second * bytes_per_sample;
	long end_byte = sample_rate * end_second * bytes_per_sample;
    uint8_t buffer[1024];

    int cur_mute_byte = 0;
    
    while (size_t byte_read = fread(buffer, 1, 1024, input)) {
        for (size_t pos = 0; pos < byte_read; pos++) {
            if (cur_mute_byte >= start_byte && cur_mute_byte <= end_byte) {
				buffer[pos] = 0;
            }
			cur_mute_byte++;
		}
        fwrite(buffer, 1, byte_read, output);
    }
    
    fclose(input);
    fclose(output);

    std::cout << "Muted from " << start_second << " to " << end_second << " seconds" << std::endl;

}


long get_file_size(FILE* file) {
    long current_pos = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, current_pos, SEEK_SET);
    return size;
}

void mix(const std::string& input_file1, const std::string& input_file2, const std::string& output_file, long start_byte, long end_byte){
	FILE* input1 = fopen(input_file1.c_str(), "rb");
    FILE* input2 = fopen(input_file2.c_str(), "rb");
	FILE* output = fopen(output_file.c_str(), "wb");
    
    uint8_t header1[44];
	uint8_t header2[44];
    

}