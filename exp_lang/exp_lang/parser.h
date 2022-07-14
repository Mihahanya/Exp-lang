#pragma once

#include "Lexer.h"

class Parser
{
public:
	std::map<wstring, int> vars{};
	bool exit = false;

	Parser();
	Parser(const wstring&);

	void load_tokens(const vector<Token> &) noexcept;

	void run() {
		for (i=0; i<tokens.size(); i++) 
		{
			t = tokens[i].type;

			if (t == VARIABLE) var_ptr = &vars[tokens[i].val];
			
			else if (t == MOVE_VAL) {
				i++; 
				if (tokens[i].type == VARIABLE) *var_ptr = vars[tokens[i].val];
				else if (tokens[i].type == NUMBER) *var_ptr = stoi(tokens[i].val);
			}

			else if (t == PLUS)   (*var_ptr)++;
			else if (t == MINUS)  (*var_ptr)--;
			else if (t == TOBOOL) *var_ptr = *var_ptr == 0 ? 1 : 0;
			else if (t == MORE)   *var_ptr = *var_ptr > 0 ? 1 : 0;
			else if (t == LESS)   *var_ptr = *var_ptr < 0 ? 1 : 0;

			else if (t == PRINT)	  wcout << *var_ptr;
			else if (t == PRINT_CHAR) wcout << char(*var_ptr);

			else if (t == INPUT_N)		cin >> *var_ptr;
			else if (t == INPUT_CHAR) { char a; cin >> a >> *var_ptr; }
			
			else if (t == BGN_CYC) cycle_inside(find_cycle(), *var_ptr);
					
			else if (t == BREAK && *var_ptr == 1) { exit = true; break; }
		}
	}

private:
	vector<Token> tokens;
	int* var_ptr = &vars[L"_"];
	
	size_t i = 0;
	TType t = OTHER;

	inline void cycle_inside(const vector<Token>& code, int k) {
		Parser cycle_ex;
		cycle_ex.load_tokens(code);
		cycle_ex.vars = vars;
		
		k++;
		while (--k) {
			cycle_ex.run();
			if (cycle_ex.exit) break;
		}
		vars = cycle_ex.vars;		
	}

	inline vector<Token> find_cycle() {
		int bracket_n=1; vector<Token> res;
		i++;
		while (bracket_n) {
			t = tokens[i].type;
			if (t == BGN_CYC) bracket_n++;
			else if (t == FNS_CYC) bracket_n--;
			res.push_back(tokens[i++]);
		} i--;
		return res;
	}
};


Parser::Parser() {};

Parser::Parser(const wstring& code) {
	Lexer tc(code);
	tokens = tc.to_tokens();
}

void Parser::load_tokens(const vector<Token>& t) noexcept { tokens = t; }


