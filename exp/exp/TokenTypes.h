#pragma once

#include <string>
#include <regex>

using std::string, std::regex, std::vector;


enum class TokenType {
	Name,
	NewVariable,
	Increase,
	Decrease,
	Input,
	Output,

	//DeclPattern,
	//Equivalents,
	Number,
	Space,
	Condition,
	LineComment,
	None,
};

struct Token {
	TokenType type = TokenType::None;
	size_t line = -1, chr_pos = -1;
	string val{};
};

struct TokenPatt {
	TokenType type;
	regex regex;
};

// TODO: maybe `<` to operators to do faster
// TODO: includes
// TODO: putch
const vector<TokenPatt> token_types_list {
	{ TokenType::Space,				regex(R"(^\s+)") }, //, std::regex::optimize) },
	{ TokenType::Increase,			regex(R"(^inc)") }, //, std::regex::optimize) },
	{ TokenType::Decrease,			regex(R"(^dec)") }, //, std::regex::optimize) },
	{ TokenType::Condition,			regex(R"(^if)") }, //, std::regex::optimize) },
	{ TokenType::Output,			regex(R"(^print)") }, //, std::regex::optimize) },
	{ TokenType::Input,				regex(R"(^input)") }, //, std::regex::optimize) },
	{ TokenType::NewVariable,		regex(R"(^new)") }, //, std::regex::optimize) },
	{ TokenType::Name,				regex(R"(^\$?[A-z0-9_']+\$?)") }, //, std::regex::optimize) },
	{ TokenType::Number,			regex(R"(^\d+)") }, //, std::regex::optimize) },
	//{ TokenType::DeclPattern,		regex(R"(^$)") }, //, std::regex::optimize) },
	//{ TokenType::Equivalents,		regex(R"(^<=>)") }, //, std::regex::optimize) },
	{ TokenType::LineComment,		regex(R"(^\/\/.*)") }, //, std::regex::optimize) },
};

