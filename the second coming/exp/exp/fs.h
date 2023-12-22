#pragma once

#include <fstream>
#include <sstream>
#include <exception>

using std::string, std::to_string;
using std::runtime_error;


inline string read_file_contents(const char* path) {
    string line, res;
    std::ifstream myfile(path);
    if (myfile.is_open()) {
        while (getline(myfile, line)) res += line + '\n';
        myfile.close();
    }
    else throw runtime_error("Unable to open file " + string(path));

    return res;
}
