#pragma once
#include <fstream>
#include <string>

using namespace std;

string read_txt(string path) {
	ifstream in(path); 
	string line, o="";
	while (getline(in, line)) 
		o += line + '\n';
	in.close();
	return o;
}

string sgm(string a, int f, int t=-1) {
	if (t == -1) t = a.length();
	string o="";
	for (int i=f; i<t; i++)
		o += a[i];
	return o;
}

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
