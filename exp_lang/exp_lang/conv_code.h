#pragma once
#include <string>
#include <filesystem>

#include "utilities.h"

using namespace std;

class Code
{
public:
	string code;

	void load_file(fs::path path) {
		rec_code = read_txt(path);
	}
	
	void load_rec_code(string code) {
		rec_code = code;
	}

	void normalise_code() {
		code = rec_code;

		string buff_c = code; code = "";
		int bracket_n=0;

		for (size_t i=0; i < buff_c.length(); i++) {
			char t = buff_c[i];
			
			// Including file
			if (t == '"') {
				u8string path = u8"";
				i++;
				for (i; buff_c[i] != '"'; i++) 
					path += buff_c[i];

				Code c; c.load_file(path); c.normalise_code();
				code += c.code;
			}

			if (t == '|') bracket_n++; 
			else if (bracket_n % 2 == 0) 
				if (t != ' ' && t != '\t' && t != '\n' && t != '"') code += t;
		}
	}
private:
	string rec_code;
};
