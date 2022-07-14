#pragma once

#include <regex>
#include "conf.h"

using std::wregex;

enum TType
{
	//SPACE,
	VARIABLE,
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
	wregex regex;
	TType name;
};

struct Token 
{
	wstring val;
	TType type;
};


const vector<TokenType> token_types_list = 
{
	//{wregex{LR"(\s)"}, SPACE},	
	
	{wregex{LR"((\$|&)([\w\d_]+))"}, VARIABLE},	
	
	{wregex{LR"(-?\d+)"}, NUMBER},
	
	{wregex{LR"(~)"}, MOVE_VAL},
	
	{wregex{LR"(\+)"}, PLUS}, 
	{wregex{LR"(-)"}, MINUS}, 
	
	{wregex{LR"(\{)"}, BGN_CYC},	
	{wregex{LR"(\})"}, FNS_CYC}, 

	{wregex{LR"(\?)"}, TOBOOL},	
	{wregex{LR"(>)"}, MORE}, 
	{wregex{LR"(<)"}, LESS},
	
	{wregex{LR"(:)"}, PRINT}, 
	{wregex{LR"(;)"}, PRINT_CHAR}, 
	
	{wregex{LR"(\.)"}, INPUT_N}, 
	{wregex{LR"(,)"}, INPUT_CHAR},
	
	{wregex{LR"(%)"}, BREAK},

	{wregex{LR"(\"(.+)\")"}, INCLUDE},
};
