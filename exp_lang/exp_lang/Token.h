#pragma once

#include <regex>
#include "conf.h"

using std::wregex;

enum TType
{
	VARIABLE, ASSIGN_VAL, NUMBER, PLUS, MINUS, TOBOOL, MORE, LESS,
	PRINT, PRINT_CHAR, INPUT_N, INPUT_CHAR,
	BGN_FOV, END_FOV,
	CYCLE, BREAK,
	INCLUDE, SWAP,
	SPACE, NONE,
};

struct TokenType 
{
	TType name;
	wregex regex;
};

struct Token 
{
	wstring str_val = L"";
	tsv num_val = 0;
	TType type = NONE;
};


const vector<TokenType> token_types_list = 
{
	{SPACE,		wregex{LR"(\n|\s+)"}},

	{VARIABLE,	wregex{LR"((\$|VAR\s*)([\w\d_]+))"}},

	{NUMBER,	wregex{LR"(-?\d+)"}},

	{ASSIGN_VAL,wregex{LR"(=|IS)"}},

	{SWAP,		wregex{LR"(->)"}},

	{PLUS,		wregex{LR"(\+)"}},
	{MINUS,		wregex{LR"(-)"}},

	{BGN_FOV,	wregex{LR"(\{|BEGIN)"}},
	{END_FOV,	wregex{LR"(\}|END)"}},

	{TOBOOL,	wregex{LR"(\?|NOT|IF|ELSE)"}},
	{MORE,		wregex{LR"(>\s*0)"}},
	{LESS,		wregex{LR"(<\s*0)"}},

	{PRINT_CHAR,wregex{LR"(;|PRINTCHAR)"}},
	{PRINT,		wregex{LR"(:|PRINT)"}},

	{INPUT_CHAR,wregex{LR"(,|READCHAR)"}},
	{INPUT_N,	wregex{LR"(\.|READ)"}},

	{CYCLE,		wregex{LR"(TIMES)"}},
	{BREAK,		wregex{LR"(%|BREAK)"}},

	{INCLUDE,	wregex{LR"((INCLUDE\s*)?\"(.+)\")"}},
};
