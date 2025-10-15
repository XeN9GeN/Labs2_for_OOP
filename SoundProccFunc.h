#include <cstdio>
#include <cstdint>

class SoundFunc {
public:

	void mute(const std::string& input_file, const std::string& output_file, long start_second, long end_second);
	void mix(const std::string& input_file1, const std::string& input_file2, const std::string& output_file, long start_byte, long end_byte);
};
