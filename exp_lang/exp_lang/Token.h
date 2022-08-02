#pragma once

#include <regex>
#include "conf.h"

using std::wregex;

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
	wregex regex;
};

struct Token 
{
	wstring str_val = L"";
	tsv num_val = 0;
	TType type = NONE;
};


const vector<TokType> token_types_list = 
{
	{SPACE,		wregex{LR"(\n|\s+)"}},

	{VARIABLE,	wregex{LR"(\$([\w\d_]+))"}},

	{NUMBER,	wregex{LR"(-?\d+)"}},

	{ASSIGN_VAL,wregex{LR"(=)"}},

	{SWAP,		wregex{LR"(->)"}},

	{PLUS,		wregex{LR"(\+)"}},
	{MINUS,		wregex{LR"(-)"}},

	{BGN_FOV,	wregex{LR"(\{)"}},
	{END_FOV,	wregex{LR"(\})"}},

	{TOBOOL,	wregex{LR"(\?|NOT|IF|ELSE)"}},
	{MORE,		wregex{LR"(>\s*0)"}},
	{LESS,		wregex{LR"(<\s*0)"}},

	{PRINT_CHAR,wregex{LR"(printc)"}},
	{PRINT,		wregex{LR"(print)"}},

	{INPUT_CHAR,wregex{LR"(readc)"}},
	{INPUT_N,	wregex{LR"(read)"}},

	{CYCLE,		wregex{LR"(times|cycle)"}},
	{BREAK,		wregex{LR"(break)"}},

	{INCLUDE,	wregex{LR"((include\s*)?\"(.+)\")"}},
};
