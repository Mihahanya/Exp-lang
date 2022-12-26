#pragma once

#include "Signature.h"
#include "Lexer.h"
#include <map>


enum class BuiltinFunc {
	New,
	Increase,
	Decrease,
	Input,
	Output,
	OutputCh,
	AssignNum,
	If,
	NotBuiltin,
};

struct Function {
	Signature signature;
	vector<string> into_vars_names { "a" };
	vector<Lexeme> tokens{}; 
	BuiltinFunc type = BuiltinFunc::NotBuiltin;
};

// TODO: map<name, argument> 
struct Token {
	Function* func = nullptr;
	arguments arguments {};
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

	vars_storage_t vars {};
	func_storage_t funcs {};

	void init_builtin_funcs();
};
