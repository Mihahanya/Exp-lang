#pragma once
#include <string>

#include "utilities.h"

using namespace std;

class Code
{
public:
	string rec_code, code;

	void load_file(string path) {
		/* Loading the file with code (a file extention is .exp) */
		rec_code = read_txt(path);
	}
	
	void load_rec_code(string code) {
		/* Just loading code with string type */
		rec_code = code;
	}

	void normalise_code() {
		code = rec_code;

		// Delate space symbols and tabs
		for (auto t : " \t\n")
			code.erase(remove(code.begin(), code.end(), t), code.end());

		// Delate coments
		string buff_c = code; code = "";
		int bracket_n = 0;

		for (int i=0; i < buff_c.length(); i++) {
			auto t = buff_c[i];

			if (t == '/') { bracket_n++; }
			else if (bracket_n % 2 == 0) code += t;
		}
	}
};
