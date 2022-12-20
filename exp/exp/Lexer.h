#pragma once

#include "TokenTypes.h"
#include <vector>


class Lexer {
public:
	Lexer(string code) : code{code} {}

	std::vector<Token> lex_analysis();

private:
	string code{}, clear_code{};

	Token recognize_token(const string&);
};
