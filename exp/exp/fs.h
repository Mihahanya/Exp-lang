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

inline string some_error_at(
	const string& text, const size_t& line, const size_t& chr, const char* file_name) 
{
	return text + " at file " + string(file_name) + 
				  " line " + to_string(line) + 
				  " character " + to_string(chr);
}

inline string some_error_at_lex(const string& text, const auto& lex) {
	return some_error_at(text, lex.line, lex.chr_pos, lex.file_name);
}
