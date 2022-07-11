#pragma once

#include "utilities.h"

class CodePreprocessor
{
public:
	wstring code;

	void load_file(fs::path path) {
		rec_code = read_txt(path);
	}
	
	void load_rec_code(wstring code) {
		rec_code = code;
	}

	void convert_code() {
		code = rec_code;

		wstring buff_c = code; 
		code = L"";
		
		int bracket_n=0;

		for (size_t i=0; i < buff_c.length(); i++) {
			char t = buff_c[i];
			
			/// Including file...
			/// if a double quote occurs, we get the contents until another one occurs.
			/// And instead of including, we substitute the content according to the received path.
			if (t == '"') {
				wstring path = L"";
				i++; for (i; buff_c[i] != '"'; i++) path += buff_c[i];

				CodePreprocessor c; 
				c.load_file(fs::current_path().wstring() + path + L".exp"); 
				c.convert_code();
				
				code += c.code;
			}

			/// Deleting comments
			if (t == '|') bracket_n++; 
			else if (bracket_n % 2 == 0) 
				if (t != ' ' && t != '\t' && t != '\n' && t != '"') code += t;
		}
	}
private:
	wstring rec_code;
};
