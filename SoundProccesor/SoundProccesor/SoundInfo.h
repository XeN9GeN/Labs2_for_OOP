#pragma once
class SoundInfo
{
	float duration; // длительность в секундах
	float file_size; // размер файла в байтах
	int start_second; // начальная секунда
	int end_second; // конечная секунда
	int segment_length=start_second-end_second; // длина сегмента в секундах
	SoundInfo() : duration(0), file_size(0),start_second(),end_second(),segment_length() {}

public:
	int sample_rate = 44100;
	int bit_depth = 16;
	int channels = 1;
	int audio_format = 1; // PCM = 1
	int bytes_per_sample = 2;

	
	int samples_per_sec = sample_rate;
	int bytes_per_sample_all_channels = (bytes_per_sample/8) * channels;
	
	long start_sample = start_second * samples_per_sec;
	long end_sample = end_second * samples_per_sec;
	long start_byte = start_sample * bytes_per_sample_all_channels;
	long end_byte = end_sample * bytes_per_sample_all_channels;

	
	long data_poz = 44;
	long byte_poz_start = data_poz + (start_sample * bytes_per_sample_all_channels);
	long byte_poz_end = data_poz + (end_sample * bytes_per_sample_all_channels);
	
	long data_size = (end_sample - start_sample) * bytes_per_sample_all_channels;
	long total_samples = data_size / bytes_per_sample_all_channels;
	
};

