#pragma once

#include "TokenTypes.h"
#include "Signature.h"
#include <map>


struct HighToken;
struct Procedure;

enum class HighTokenType {
	Simple,
	Function,
	Macros,
};

// TODO: redo and add this shit to tormal token
struct HighToken {
	HighTokenType type;

	union val {
		val() {}
		~val() {}

		Token* simpl {};
		Procedure* proc;
	} val;
};

struct Procedure {
	Signature signature{};
	vector<string> vars_names;
	vector<HighToken> *tokens = nullptr; 
};


using vars_storage_t = std::map<string, int>;
using proc_storage_t = std::vector<Procedure>;


class Parser
{
	friend class Parser;

public:
	Parser(vector<Token> ts, bool check_vars=true) : tokens{ts}, check_vars{check_vars} {}

	void parse();
	void execute();

private:
	bool check_vars=true;
	vector<Token> tokens {};
	vector<HighToken> high_tokens {};

	vars_storage_t vars {};
	proc_storage_t procs {};
};
