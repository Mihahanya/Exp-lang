#pragma once

#include <string>
#include <regex>
#include <iostream>

using std::string, std::regex, std::vector, std::to_string, std::cout;

enum class LexType {
	Space,
	Name,
	NoWord,
	Number,
	LineComment,
	None,
};


struct Lexeme {
	size_t line=-1, chr_pos=-1;
	LexType type=LexType::None;
	string val {};
};

inline bool operator==(const Lexeme& l, const Lexeme& r) {
	return l.type == r.type and l.val == r.val;
}

inline bool operator!=(const Lexeme& l, const Lexeme& r) {
	return !(l == r);
}


class Lexer {
public:
	Lexer(string code) : code{code} {}

	vector<Lexeme> lex_analysis();

private:
	string code{};

	struct LexPatt { LexType type; regex regex; };
	const static vector<LexPatt> lex_types_list;

	Lexeme recognize_lexeme(const string&);
};
