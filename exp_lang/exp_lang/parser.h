#pragma once

#include "Lexer.h"

typedef std::map<wstring, tsv> VarStrg;

class Parser
{
public:
	vector<Token> tokens{};
	VarStrg vars{};
	short exit = 0;
	bool ok = true;

	Parser();
	Parser(const wstring&);

	void run();

private:
	void cycle_inside(vector<Token>::iterator&, tsv);
	void field_view(vector<Token>::iterator&, vector<Token>&, VarStrg&);
};


Parser::Parser() {};

Parser::Parser(const wstring& code) {
	Lexer tc(code);
	tokens = tc.lex_analysis();
	ok = tc.ok;
}


void Parser::run() {
	if (not ok) return;

	tsv* var_ptr = &vars[L"_"];

	for (auto t = tokens.begin(); t < tokens.end(); t++)
	{
		switch ((*t).type)
		{
		case VARIABLE: var_ptr = &vars[(*t).str_val]; break;

		case MOVE_VAL:
			t++;
			if ((*t).type == VARIABLE) *var_ptr = vars[(*t).str_val];
			else if ((*t).type == NUMBER) *var_ptr = (*t).num_val;
			break;

		case PLUS:	 (*var_ptr)++; break;
		case MINUS:  (*var_ptr)--; break;
		case TOBOOL: *var_ptr = *var_ptr == 0 ? 1 : 0; break;
		case MORE:   *var_ptr = *var_ptr > 0 ? 1 : 0; break;
		case LESS:   *var_ptr = *var_ptr < 0 ? 1 : 0; break;

		case PRINT:		 wcout << *var_ptr; break;
		case PRINT_CHAR: wcout << char(*var_ptr); break;

		case INPUT_N: cin >> *var_ptr; break;
		case INPUT_CHAR:
			char a; cin >> a;
			*var_ptr = tsv(a);
			break;

		case BGN_FOV: cycle_inside(t, *var_ptr); break;

		case BREAK:
			exit += 2;
			break;
		}
		if (exit > 0) t = tokens.end();
	}
}

void Parser::cycle_inside(vector<Token>::iterator& t, tsv iter_var) {
	vector<Token> tks{}; VarStrg vrs{};
	field_view(t, tks, vrs);

	Parser cycle_pars;
	cycle_pars.tokens = tks; 
	cycle_pars.vars = vrs;

	iter_var++;
	while (--iter_var) {
		cycle_pars.run();
		if (cycle_pars.exit > 0) break;
	}
	exit = std::max(cycle_pars.exit-1, 0);
	for (const auto& v : vrs) vars[v.first] = cycle_pars.vars[v.first];
}

inline void Parser::field_view(vector<Token>::iterator& t, vector<Token>& tokens_in, VarStrg& vars_in) {
	auto to = t+t->num_val;
	tokens_in = vector<Token>(t+1, to);
	t = to; 
	
	for (const auto& t : tokens_in) {
		if (t.type == VARIABLE) {
			const wstring name = t.str_val;
			if (!vars_in.count(name) and vars.count(name)) vars_in[name] = vars[name];
		}
	}
}

