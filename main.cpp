#include "converter.h"
#include "wave_file.h"
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>

using namespace std;

struct Operations {
    std::string name;
    std::vector<std::string>args;
    std::string output_name;
};

std::vector<Operations> Script(const std::string& file_name) {
    std::vector<Operations> operations;
    ifstream file(file_name);
    string line;
    int line_num = 0;

    while (getline(file, line)) {
        line_num++;
        if (line.empty() || line[0] == '#') continue;

        istringstream iss(line);
        Operations op;
        string token;

        iss >> op.name;

        while (iss >> token) {
            op.args.push_back(token);
        }

        if (!op.args.empty()) {
            op.output_name = op.args.back();
            op.args.pop_back();
        }

        operations.push_back(op);
    }

    return operations;
}
    
int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <in1> <in2> <script>\n";
        return 1;
    }

    const string file_in1 = argv[1];
    const string file_in2 = argv[2];
    const string script_file = argv[3];

    vector<WavFile> files(2);

    files[0].load(file_in1.c_str());
    files[1].load(file_in2.c_str());

    auto operations = Script(script_file);

    for (const auto& op : operations) {

        std::vector<const char*> args_cstr;
        for (const auto& arg : op.args) {
            args_cstr.push_back(arg.c_str());
        }

        auto converter = ConverterFactory::create(op.name.c_str(), args_cstr);
        std::vector<WavFile> working_files = files;

        converter->apply(working_files);

        int target_file = converter->getTargetFile();
        if (target_file >= 0 && target_file < working_files.size()) {
            std::cout << "Saving to: " << op.output_name << std::endl;
            working_files[target_file].save(op.output_name.c_str());
        }

        else { std::cerr << "Error: Invalid target file index: " << target_file << std::endl; }
    }
    return 0;
}