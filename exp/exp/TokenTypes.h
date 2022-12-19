#pragma once

#include <string>
#include <regex>

using std::string, std::regex, std::vector;


enum class TokenType {
	ChooseVariable,
	NewVariable,
	Increase,
	Decrease,
	Input,
	Output,

	DeclPattern,
	Equivalents,
	Number,
	Space,
	Condition,
	UsePattern,		//
	OtherPattern,	//
	None,
};

struct Token {
	TokenType type;
	size_t line;
	string val;
};

struct TokenPatt {
	TokenType type;
	regex regex;
};

// TODO: `<` to operators to do faster
const vector<TokenPatt> token_types_list {
	{ TokenType::Space,				regex(R"(^(;|\s+))") }, //, std::regex::optimize) },	// `\n` <=> `;`
	{ TokenType::Increase,			regex(R"(^inc)") }, //, std::regex::optimize) },
	{ TokenType::Decrease,			regex(R"(^dec)") }, //, std::regex::optimize) },
	{ TokenType::Condition,			regex(R"(^if)") }, //, std::regex::optimize) },
	{ TokenType::Output,			regex(R"(^print)") }, //, std::regex::optimize) },
	{ TokenType::Input,				regex(R"(^input)") }, //, std::regex::optimize) },
	{ TokenType::NewVariable,		regex(R"(^new)") }, //, std::regex::optimize) },
	{ TokenType::Number,			regex(R"(^\d+)") }, //, std::regex::optimize) },
	{ TokenType::ChooseVariable,	regex(R"(^[A-z0-9_']+)") }, //, std::regex::optimize) },
	{ TokenType::DeclPattern,		regex(R"(^$)") }, //, std::regex::optimize) },
	{ TokenType::Equivalents,		regex(R"(^<=>)") }, //, std::regex::optimize) },
};

