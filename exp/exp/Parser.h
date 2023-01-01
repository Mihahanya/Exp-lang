#pragma once

#include "Signature.h"

// TODO: replaces (macro)
enum class BuiltinFunc {
	Increase, Decrease, New,
	Add, Sub, Mul, Div, Pow, Root, Mod,
	Neg, Or, And, Xor,
	Input, Output, OutputCh,
	AssignVar, AssignPointer,
	If, IfElse,
	DefineFunc,	
	Replace,
	Include,
	Pass,
	NotBuiltin,
};

struct Function {
	Signature signature;
	vector<Lexeme> tokens; 
	BuiltinFunc type = BuiltinFunc::NotBuiltin;
};

struct Token {
	const Function* func;
	func_arguments_t arguments;
	Lexeme info;
};

// TODO: redo to another stl containers vector<Lexeme> to deque vector<function> to list maybe...
class Parser
{
	using VarTy = int;

	using vars_storage_t = std::map<string, VarTy*>;
	using func_storage_t = std::vector<Function>;

public:
	Parser(const vector<Lexeme>& ls);
	~Parser();

	void parse();
	void execute();

private:
	vector<Lexeme> lexems {};
	vector<Token> tokens {};

	vars_storage_t vars;
	func_storage_t* funcs;

	void init_builtin_funcs();
	void exec_fov(const vector<Lexeme>&);
	inline void include_script(const string& file_path, const string& this_path);
};
