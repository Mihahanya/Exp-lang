#pragma once
#include <string>

#include "utilities.h"

using namespace std;

class Code
{
public:
	string rec_code, code;

	void load_file(string path) {
		rec_code = read_txt(path);
	}
	
	void load_rec_code(string code) {
		rec_code = code;
	}

	void normalise_code() {
		code = rec_code;

		for (auto t : " \t\n")
			code.erase(remove(code.begin(), code.end(), t), code.end());

		string buff_c = code; code = "";
		int bracket_n = 0;

		for (size_t i=0; i < buff_c.length(); i++) {
			auto t = buff_c[i];

			if (t == '|') { bracket_n++; }
			else if (bracket_n % 2 == 0) code += t;
		}
	}
};
