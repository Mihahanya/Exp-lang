#pragma once

#include "Lexer.h"
#include <map>


using f_argument_t = vector<Lexeme>;
using f_arguments_t = vector<f_argument_t>;


class Parser
{
	friend class Parser;

	struct Function;
	struct Token;

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

	void init_buildin_funcs();
};
