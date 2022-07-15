#pragma once

#include <regex>
#include "conf.h"

using std::wregex;

enum TType
{
	SPACE,
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
	{wregex{LR"(\s+)"}, SPACE},	
	
	{wregex{LR"((\$|&|VAR)([\w\d_]+))"}, VARIABLE},	
	
	{wregex{LR"(-?\d+)"}, NUMBER},
	
	{wregex{LR"(~|=|IS)"}, MOVE_VAL},
	
	{wregex{LR"(\+|PLUS)"}, PLUS}, 
	{wregex{LR"(-|MINUS)"}, MINUS}, 
	
	{wregex{LR"(\{|BEGIN)"}, BGN_CYC},	
	{wregex{LR"(\}|END)"}, FNS_CYC}, 

	{wregex{LR"(\?|NEG|IF|ELSE)"}, TOBOOL},	
	{wregex{LR"(>|ISGREATERZERO)"}, MORE}, 
	{wregex{LR"(<|ISLESSZERO)"}, LESS},
	
	{wregex{LR"(;|PRINTCHAR)"}, PRINT_CHAR}, 
	{wregex{LR"(:|PRINT)"}, PRINT}, 
	
	{wregex{LR"(,|READCHAR)"}, INPUT_CHAR},
	{wregex{LR"(\.|READ)"}, INPUT_N}, 
	
	{wregex{LR"(%|BREAKIF)"}, BREAK},

	{wregex{LR"((INCLUDE)?\"(.+)\")"}, INCLUDE},
};
