#include "Lexer.h"

#include <algorithm>
#include <iostream>
#include <vector>


const vector<Lexer::LexPatt> Lexer::lex_types_list = {
	{ LexType::Space,		regex(R"(^\s+)",				std::regex::optimize) },
	{ LexType::Number,		regex(R"(^-?\d+)",				std::regex::optimize) },
	{ LexType::String,		regex(R"(^'.+')",				std::regex::optimize) },
	{ LexType::Name,		regex(R"(^\$?[A-z0-9_']+\$?)",	std::regex::optimize) },
	{ LexType::LineComment,	regex(R"(^\/\/.*)",				std::regex::optimize) },
	{ LexType::NoWord,		regex(R"(^[^\s\w\$]+)",			std::regex::optimize) },
};


vector<Lexeme> Lexer::lex_analysis() {
	vector<Lexeme> res_lex{};

	size_t line_n = 1;
	std::istringstream f(code);
	for (string this_line{}; std::getline(f, this_line); ++line_n) 
	{
		for (size_t i=0; i < this_line.length(); ) 
		{
			string sub_code(this_line.begin()+i, this_line.end());
			Lexeme lexeme = recognize_lexeme(sub_code);
			lexeme.line = line_n;
			lexeme.chr_pos = i;
			lexeme.file_name = file_name;

			if (lexeme.type == LexType::None) 
				throw runtime_error(some_error_at("Unknown lexeme", line_n, i, file_name));
			
			// Ignore spaces
			if (lexeme.type != LexType::Space and lexeme.type != LexType::LineComment) {
				res_lex.push_back(lexeme);
			}

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
