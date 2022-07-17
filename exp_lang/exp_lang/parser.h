#pragma once

#include "Lexer.h"

typedef std::map<wstring, int> VarStrg;

class Parser
{
public:
	vector<Token> tokens{};
	VarStrg vars{};
	short exit = 0;

	Parser();
	Parser(const wstring&);

	void run();

private:
	void cycle_inside(vector<Token>::iterator&, int);
	std::pair<vector<Token>, VarStrg> field_view(vector<Token>::iterator&);
};


Parser::Parser() {};

Parser::Parser(const wstring& code) {
	Lexer tc(code);
	tokens = tc.to_tokens();
}


void Parser::run() {
	int* var_ptr = &vars[L"_"];

	for (auto t = tokens.begin(); t < tokens.end(); t++)
	{
		switch ((*t).type)
		{
		case VARIABLE: var_ptr = &vars[(*t).val]; break;

		case MOVE_VAL:
			t++;
			if ((*t).type == VARIABLE) *var_ptr = vars[(*t).val];
			else if ((*t).type == NUMBER) *var_ptr = stoi((*t).val);
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
			*var_ptr = int(a);
			break;

		case BGN_CYC: cycle_inside(++t, *var_ptr); break;

		case BREAK:
			if (*var_ptr == 1) {
				exit++;
				break;
			}
			break;
		}
	}
}

void Parser::cycle_inside(vector<Token>::iterator& t, int iter_var) {
	auto [tks, vrs] = field_view(t);

	Parser cycle_pars;
	cycle_pars.tokens = tks; 
	cycle_pars.vars = vrs;

	iter_var++;
	while (--iter_var) {
		cycle_pars.run();
		if (cycle_pars.exit > 0) break;
	}
	exit = std::max(cycle_pars.exit-1, 0);
	for (const auto& v : cycle_pars.vars) vars[v.first] = v.second;
}

inline std::pair<vector<Token>, VarStrg> Parser::field_view(vector<Token>::iterator& t) {
	auto from = t;
	VarStrg vrs{};

	for (int bracket_n = 1; bracket_n; t++) {
		switch ((*t).type) {
		case BGN_CYC:
			bracket_n++;
			break;
		case FNS_CYC:
			bracket_n--;
			break;
		case VARIABLE:
			const wstring name = (*t).val;
			if (!vrs.count(name)) vrs[name] = vars[name];
			break;
		}
	}
	t--; // end of cycle `}`
	return { vector<Token>(from, t), vrs };
}

