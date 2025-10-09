#pragma once

class SoundFunc {
	void mute(FILE*file, long start_byte, long end_byte, long bytes_per_sample);
	void mix();
};
