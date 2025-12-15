#include "wave_file.h"

#include <cstdio>
#include <iostream>

bool WavFile::load(const char* file_name) {
    FILE* file = fopen(file_name, "rb");

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 44, SEEK_SET);

    std::cout << "Loading: " << file_name << " (" << file_size << " bytes)\n";

    int16_t sample;
    while (fread(&sample, sizeof(int16_t), 1, file) == 1) {
        samples.push_back(sample);
    }
    
    fclose(file);
    std::cout << "Loaded " << samples.size() << " samples\n";
    return true;
}

bool WavFile::save(const char* file_name) {
    FILE* out = fopen(file_name, "wb");
    if (!out) return false;

    unsigned char header[44] = {
        //Riff chunk 12
        'R', 'I', 'F', 'F', //Riff индефикатор - порядок байтов(little endian) 4
        0, 0, 0, 0,         // размер файла - 8 байт file_size 
        'W', 'A', 'V', 'E', 

        //format chunk 24
        'f', 'm', 't', ' ', //4 byte
        16, 0, 0, 0,        // размер fmt-chunk 4
        1, 0,               // аудиоформат (PCM) 2
        1, 0,               // колво каналов 2
        0x44, 0xAC, 0, 0,   // частота (44100) 0x0000AC44 4
        0x88, 0x58, 0x01, 0,// байт в секунду 0x00015888  4
        2, 0,               // байт на сэмпл 2
        16, 0,              // бит на сэмпл 2 
        
        //data chunk 8
        'd', 'a', 't', 'a', //data чанк
        0, 0, 0, 0          // размер данных data_size
    };



    //real_size= 44(header)+data_size
    uint32_t data_size = samples.size() * sizeof(int16_t);
    uint32_t file_size = data_size + 36; // 44+data_size-8(RIFF+chunk_size)


    //Каждый сдвиг двигает нужный байт в младшую позицию
    //file_size разбиваем на little endian и вытаскиваем конкретные байты из 32-битного числа
    header[4] = file_size & 0xFF;
    header[5] = (file_size >> 8) & 0xFF;
    header[6] = (file_size >> 16) & 0xFF;
    header[7] = (file_size >> 24) & 0xFF;

    //data_size
    header[40] = data_size & 0xFF;
    header[41] = (data_size >> 8) & 0xFF;
    header[42] = (data_size >> 16) & 0xFF;
    header[43] = (data_size >> 24) & 0xFF;

    fwrite(header, 1, 44, out);
    fwrite(samples.data(), sizeof(int16_t), samples.size(), out);
    fclose(out);

    std::cout << "Saved: " << file_name << " (" << samples.size() << " samples)\n";
    return true;
}


struct WAVHeader {
    char riff[4];           // "RIFF"
    uint32_t file_size;     // размер файла - 8
    char wave[4];           // "WAVE"
    char fmt[4];            // "fmt "
    uint32_t fmt_size;      // 4 байта
    uint16_t audio_format;  // 1 = PCM
    uint16_t num_channels;  // 1
    uint32_t sample_rate;   // 44100
    uint32_t byte_rate;     // 88200
    uint16_t block_align;   // 2 байта
    uint16_t bits_per_sample; // 16
    char data[4];           // "data"
    uint32_t data_size;     // размер данных
};

//12[RIFF header] 24[fmt chunk] 8[data chunk] = 44[header]