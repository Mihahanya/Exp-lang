#pragma once

#include <string>
#include <regex>

using std::string, std::regex, std::vector, std::to_string;

enum class LexType {
	Space,
	Name,
	NoWord,
	Number,
	LineComment,
	None,
};


struct Lexeme {
	size_t line, chr_pos;
	LexType type;
	string val;
};

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
