#include "SoundProccFunc.h"
#include <iostream>
#include "SoundInfo.h"

void mute(FILE* file, long start_byte, long end_byte, long bytes_per_sample) {
	long byte_count = end_byte - start_byte;
	fseek(file, start_byte, SEEK_SET);
	uint8_t zero_byte = 0x00;
	for(long i=0;i<byte_count; i++) {
		fwrite(&zero_byte, 1, 1, file);
	}
}