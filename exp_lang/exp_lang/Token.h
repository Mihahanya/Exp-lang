#pragma once

#include <regex>
#include "conf.h"

using std::regex;

enum TType
{
	VARIABLE, ASSIGN_VAL, NUMBER, PLUS, MINUS, TOBOOL, MORE, LESS, 
	PRINT, PRINT_CHAR, INPUT_N, INPUT_CHAR, 
	BGN_FOV, END_FOV, BGN_BRC, END_BRC, 
	IF, ELSE, CYCLE, BREAK, 
	INCLUDE, SWAP, 
	SPACE, NONE, 
};

struct TokType 
{
	TType name;
	regex regex;
};

struct Token 
{
	string str_val = "";
	tsv num_val = 0;
	TType type = NONE;
};


const vector<TokType> token_types_list = 
{
	{SPACE,		regex{R"(\n|\s+)"}},

	{VARIABLE,	regex{R"(\$([\w\d_]+))"}},

	{NUMBER,	regex{R"(-?\d+)"}},

	{ASSIGN_VAL,regex{R"(=)"}},

	{SWAP,		regex{R"(->)"}},

	{PLUS,		regex{R"(\+)"}},
	{MINUS,		regex{R"(-)"}},

	{BGN_FOV,	regex{R"(\{)"}},
	{END_FOV,	regex{R"(\})"}},

	{TOBOOL,	regex{R"(\?|NOT|IF|ELSE)"}},
	{MORE,		regex{R"(>\s*0)"}},
	{LESS,		regex{R"(<\s*0)"}},

	{PRINT_CHAR,regex{R"(printc)"}},
	{PRINT,		regex{R"(print)"}},

	{INPUT_CHAR,regex{R"(readc)"}},
	{INPUT_N,	regex{R"(read)"}},

	{CYCLE,		regex{R"(times|cycle)"}},
	{BREAK,		regex{R"(break)"}},

	{INCLUDE,	regex{R"(include\s*\"(.+)\")"}},
};
