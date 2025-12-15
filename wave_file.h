#ifndef WAV_FILE_H
#define WAV_FILE_H

#include <vector>
#include <cstdint>
	
class WavFile
{
public:
	std::vector<int16_t> samples;
	bool load(const char* file_name);
	bool save(const char* file_name);
};

#endif