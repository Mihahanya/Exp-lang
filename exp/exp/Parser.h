#pragma once

#include "Signature.h"
#include "Lexer.h"
#include <map>


enum class BuiltinFunc {
	Increase,
	Decrease,
	Input,
	Output,
	OutputCh,
	AssignNum,
	AssignVar,
	If,
	DefineFunc,	// definding of a new function
	Replace,	// soon
	NotBuiltin,
};

struct Function {
	Signature signature;
	vector<Lexeme> tokens{}; 
	BuiltinFunc type = BuiltinFunc::NotBuiltin;
};

struct Token {
	Function* func = nullptr;
	func_arguments_t arguments {};
};


class Parser
{
	friend class Parser;

	using vars_storage_t = std::map<string, int>;
	using func_storage_t = std::vector<Function>;

public:
	Parser(vector<Lexeme> ls, bool check_vars=true) : lexems{ls}, check_vars{check_vars} {}

	void parse();
	void execute();

private:
	bool check_vars=true;
	vector<Lexeme> lexems {};
	vector<Token> tokens {};

	vars_storage_t vars;
	func_storage_t funcs;

	void init_builtin_funcs();
};
