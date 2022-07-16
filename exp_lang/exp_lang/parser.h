#pragma once

#include "Lexer.h"

class Parser
{
public:
	vector<Token> tokens{};
	std::map<wstring, int> vars{};
	bool exit = false;

	Parser();
	Parser(const wstring&);

	void run();

private:
	void cycle_inside(const vector<Token>&, int);
	vector<Token> find_cycle(vector<Token>::iterator&);
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

		case BGN_CYC: cycle_inside(find_cycle(++t), *var_ptr); break;

		case BREAK:
			if (*var_ptr == 1) {
				exit = true;
				break;
			}
			break;
		}
	}
}

void Parser::cycle_inside(const vector<Token>& code, int iter_var) {
	Parser cycle_ex;
	cycle_ex.tokens = code;

	for (const auto& t : code) if (t.type == VARIABLE) cycle_ex.vars[t.val] = vars[t.val];

	iter_var++;
	while (--iter_var) {
		cycle_ex.run();
		if (cycle_ex.exit) break;
	}
	for (const auto& v : cycle_ex.vars) vars[v.first] = v.second;
}

vector<Token> Parser::find_cycle(vector<Token>::iterator& t) {
	vector<Token> res;
	for (int bracket_n = 1; bracket_n; t++) {
		switch ((*t).type) {
		case BGN_CYC:
			bracket_n++;
			break;
		case FNS_CYC:
			bracket_n--;
			break;
		}
		res.push_back(*t);
	}
	t--;
	return res;
}
