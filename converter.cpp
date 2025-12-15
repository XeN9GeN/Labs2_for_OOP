#include "converter.h"
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>

class MuteConverter : public Converter {
	int target_file;
	double start_sec, end_sec;
public:
	MuteConverter(int target, double s, double e) : target_file(target), start_sec(s), end_sec(e) {}

	void apply(std::vector<WavFile>& files) override {
		if (target_file >= files.size()) return;

		int start_sample = SecondsToSamples(start_sec);
		int end_sample = SecondsToSamples(end_sec);

		for (int i = start_sample; i < end_sample && i < files[target_file].samples.size(); i++) {
			files[target_file].samples[i] = 0;
		}

	}
	int getTargetFile() const override {
		return target_file;
	}
};

class MixFiles : public Converter {
	int target_file;
	int source_file; 
	double start_sec;
	double end_sec;

public:
	MixFiles(int target, int source, double start, double end)
		: target_file(target), source_file(source), start_sec(start), end_sec(end) {
	}

	void apply(std::vector<WavFile>& files) override {
		int start_sample = SecondsToSamples(start_sec);
		int end_sample = SecondsToSamples(end_sec);

		auto& target = files[target_file];
		auto& source = files[source_file];

		int samples_to_mix = end_sample - start_sample;

		for (int i = 0; i < samples_to_mix; i++) {
			int target_index = start_sample + i;
			int source_index = i;

			if (target_index >= target.samples.size()) break;
			if (source_index >= source.samples.size()) break;

			int mixed_sample = static_cast<int>(target.samples[target_index]) + static_cast<int>(source.samples[source_index]);

			if (mixed_sample > 32767) mixed_sample = 32767;
			if (mixed_sample < -32768) mixed_sample = -32768;

			target.samples[target_index] = static_cast<int16_t>(mixed_sample);
		}

	}
	int getTargetFile() const override {
		return target_file;
	}
};


class ReverseFiles : public Converter {
	int target_file;
public:
	ReverseFiles(int target) : target_file(target) {}

	void apply(std::vector<WavFile>& files) override {
		if (target_file >= files.size()) return;

		std::reverse(files[target_file].samples.begin(), files[target_file].samples.end());
	}

	int getTargetFile() const override {
		return target_file;
	}
};

class SetVolumeFiles : public Converter {
	int target_file;
	float volume;
public:
	SetVolumeFiles(int target, float v) : target_file(target), volume(v) {}

	void apply(std::vector<WavFile>& files) override {
		if (target_file >= files.size()) return;

		auto& file = files[target_file];
		for (auto& s : file.samples) {
			int a = static_cast<int>(s * volume);
			if (a > 32767) a = 32767;
			if (a < -32768) a = -32768;
			s = static_cast<int16_t>(a);
		}
	}

	int getTargetFile() const override {
		return target_file;
	}
};

std::unique_ptr<Converter> ConverterFactory::create(const char* name, std::vector<const char*> args) {
	if (strcmp(name, "mute") == 0 && args.size() >= 3) {
		int target = atoi(args[0]);
		double s = atof(args[1]), e = atof(args[2]);
		return std::make_unique<MuteConverter>(target, s, e);
	}

	if (strcmp(name, "mix") == 0 && args.size() >= 4) {
		int target = atoi(args[0]);
		int source = atoi(args[1]);
		double start = atof(args[2]);
		double end = atof(args[3]);
		return std::make_unique<MixFiles>(target, source, start, end);
	}

	else if (strcmp(name, "reverse") == 0 && args.size() >= 1) {
		int target = atoi(args[0]);
		return std::make_unique<ReverseFiles>(target);
	}

	else if (strcmp(name, "set_volume") == 0 && args.size() >= 2) {
		int target = atoi(args[0]);
		float v = static_cast<float>(atof(args[1]));
		return std::make_unique<SetVolumeFiles>(target, v);
	}

	return nullptr;
}