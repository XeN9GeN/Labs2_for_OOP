#ifndef CONVERTER_H
#define CONVERTER_H
#include "wave_file.h"
#include <vector>
#include<memory>

class Converter
{
public:
	virtual void apply(std::vector <WavFile>& files) = 0;
	virtual int getTargetFile() const = 0;
	virtual ~Converter() = default;

protected:
	int SecondsToSamples(double seconds) const {
		return static_cast<int>(seconds * 44100);
	}
};




class ConverterFactory {
public:
	static std::unique_ptr<Converter> create(const char* name, std::vector<const char*> args);
};

#endif