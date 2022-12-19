#include "Lexer.h"

#include <algorithm>
#include <iostream>

#include <iostream>
using std::cout, std::to_string;


// TODO: comments and line by line
vector<Token> Lexer::lex_analysis() {
	to_clear_code();
	
	vector<Token> res_tokens{};

	for (int i=0, line=1; i < clear_code.length(); ) {
		string sub_code(clear_code.begin()+i, clear_code.begin()+i+32);
		Token token = recognize_token(sub_code);

		if (token.type == TokenType::None) 
			throw std::runtime_error("Unknown token at line " + to_string(line));
		
		if (token.type == TokenType::Space and token.val == ";") line++;

		token.line = line;
		res_tokens.push_back(token);
		i += token.val.length();
	}

	return res_tokens;
}

Token Lexer::recognize_token(const string& str) {
	Token token;
	token.type = TokenType::None;

	for (const auto& checked_token : token_types_list) {
		std::smatch m{};

		if (std::regex_search(str, m, checked_token.regex)) {
			token.type = checked_token.type;
			token.val = m.str();	
			//token.line = line;	// assign in place
				
			break;
		}
	}

	return token;
}

void Lexer::to_clear_code() {
	// Delete comments
	/*const static regex del_reg{ R"(\/\*.*\*\/|\/\/.*\n)" };
	clear_code = std::regex_replace(code, del_reg, "\n");*/
	clear_code = code;
	std::replace(clear_code.begin(), clear_code.end(), '\n', ';');
}
