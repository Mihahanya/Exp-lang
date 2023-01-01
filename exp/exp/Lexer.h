#pragma once

#include <regex>
#include <iostream>
#include "fs.h"

using std::regex, std::vector, std::cout;

enum class LexType {
	Space,
	Name, 
	NoWord,
	Number,
	String,
	LineComment,
	None, 
};


struct Lexeme {
	size_t line=-1, chr_pos=-1;
	LexType type=LexType::None;
	string val {};
	const char* file_name;
};

inline bool operator==(const Lexeme& l, const Lexeme& r) {
	return l.type == r.type and l.val == r.val;
}

inline bool operator!=(const Lexeme& l, const Lexeme& r) {
	return !(l == r);
}


class Lexer {
public:
	Lexer(const string& code, const char* file_name) : code{code}, file_name{file_name} {}

	vector<Lexeme> lex_analysis();

private:
	string code{};
	const char* file_name{};

	struct LexPatt { LexType type; regex regex; };
	const static vector<LexPatt> lex_types_list;

	Lexeme recognize_lexeme(const string&);
};
