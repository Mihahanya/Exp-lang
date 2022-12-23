#include "Lexer.h"

#include <algorithm>
#include <sstream>
#include <iostream>

#include <iostream>
using std::cout, std::to_string;


vector<Token> Lexer::lex_analysis() {
	vector<Token> res_tokens{};

	int line_n = 1;
	std::istringstream f(code);
	for (string this_line{}; std::getline(f, this_line); ++line_n) 
	{
		for (int i=0; i < this_line.length(); ) 
		{
			string sub_code(this_line.begin()+i, this_line.end());
			Token token = recognize_token(sub_code);

			if (token.type == TokenType::None) 
				throw std::runtime_error("Unknown token at line " + to_string(line_n) + " character " + to_string(i));
		
			token.line = line_n;
			token.chr_pos = i;
			res_tokens.push_back(token);
			i += token.val.length();
		}
	}

	return res_tokens;
}

Token Lexer::recognize_token(const string& str) {
	Token token;
	token.type = TokenType::None;

	for (const auto& checked_token : token_types_list) {
		std::smatch m;

		if (std::regex_search(str, m, checked_token.regex)) {
			token.type = checked_token.type;
			token.val = m.str();	
			// line and char numbers are assigns in place
				
			break;
		}
	}

	return token;
}
