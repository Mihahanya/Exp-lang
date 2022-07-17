#pragma once

#include <regex>
#include "conf.h"

using std::wregex;

enum TType
{
	SPACE,
	VARIABLE, 
	SWAP,
	MOVE_VAL,
	NUMBER,
	PLUS, MINUS,
	TOBOOL, MORE, LESS,
	PRINT, PRINT_CHAR,
	INPUT_N, INPUT_CHAR,
	BGN_CYC, FNS_CYC,
	BREAK,
	INCLUDE,
	OTHER,
};

struct TokenType 
{
	TType name;
	wregex regex;
};

struct Token 
{
	wstring val;
	TType type;
};


const vector<TokenType> token_types_list = 
{
	{SPACE,		wregex{LR"(\s+)"}},

	{VARIABLE,	wregex{LR"((\$|VAR\s*)([\w\d_]+))"}},

	{NUMBER,	wregex{LR"(-?\d+)"}},

	{MOVE_VAL,	wregex{LR"(=|IS)"}},

	{SWAP,		wregex{LR"(->)"}},

	{PLUS,		wregex{LR"(\+)"}},
	{MINUS,		wregex{LR"(-)"}},

	{BGN_CYC,	wregex{LR"(\{|BEGIN)"}},
	{FNS_CYC,	wregex{LR"(\}|END)"}},

	{TOBOOL,	wregex{LR"(\?|NEG|IF|ELSE)"}},
	{MORE,		wregex{LR"(>\s*0)"}},
	{LESS,		wregex{LR"(<\s*0)"}},

	{PRINT_CHAR,wregex{LR"(;|PRINTCHAR)"}},
	{PRINT,		wregex{LR"(:|PRINT)"}},

	{INPUT_CHAR,wregex{LR"(,|READCHAR)"}},
	{INPUT_N,	wregex{LR"(\.|READ)"}},

	{BREAK,		wregex{LR"(%|BREAKIF)"}},

	{INCLUDE,	wregex{LR"((INCLUDE\s*)?\"(.+)\")"}},
};
