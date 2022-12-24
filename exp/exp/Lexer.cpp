#include "Lexer.h"

#include <algorithm>
#include <sstream>
#include <iostream>
#include <vector>


const vector<Lexer::LexPatt> Lexer::lex_types_list = {
	{ LexType::Space,		regex(R"(^\s+)") }, //, std::regex::optimize) },
	{ LexType::Name,		regex(R"(^\$?[A-z0-9_']+\$?)") }, //, std::regex::optimize) },
	{ LexType::Number,		regex(R"(^\d+)") }, //, std::regex::optimize) },
	{ LexType::LineComment,	regex(R"(^\/\/.*)") }, //, std::regex::optimize) },
	{ LexType::NoWord,		regex(R"(^\W+)") }, //, std::regex::optimize) },
};


vector<Lexeme> Lexer::lex_analysis() {
	vector<Lexeme> res_lex{};

	int line_n = 1;
	std::istringstream f(code);
	for (string this_line{}; std::getline(f, this_line); ++line_n) 
	{
		for (int i=0; i < this_line.length(); ) 
		{
			string sub_code(this_line.begin()+i, this_line.end());
			Lexeme lexeme = recognize_lexeme(sub_code);

			if (lexeme.type == LexType::None) 
				throw std::runtime_error("Unknown lexeme at line " + to_string(line_n) + " character " + to_string(i));
		
			lexeme.line = line_n;
			lexeme.chr_pos = i;
			res_lex.push_back(lexeme);
			i += lexeme.val.length();
		}
	}

	return res_lex;
}

Lexeme Lexer::recognize_lexeme(const string& str) {
	Lexeme lexeme;
	lexeme.type = LexType::None;

	for (const auto& checked_lexeme : lex_types_list) {
		std::smatch m;

		if (std::regex_search(str, m, checked_lexeme.regex)) {
			lexeme.type = checked_lexeme.type;
			lexeme.val = m.str();	
			// line and char numbers are assigns in place
				
			break;
		}
	}

	return lexeme;
}
