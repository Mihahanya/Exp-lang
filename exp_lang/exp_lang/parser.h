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
		for (size_t i=0; i<tokens.size(); i++) 
		{
			switch (tokens[i].type) 
			{
			case VARIABLE: var_ptr = &vars[tokens[i].val]; break;
			
			case MOVE_VAL: 
				i++; 
				if (tokens[i].type == VARIABLE) *var_ptr = vars[tokens[i].val];
				else if (tokens[i].type == NUMBER) *var_ptr = stoi(tokens[i].val);
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
			
			case BGN_CYC: cycle_inside(find_cycle(++i), *var_ptr); break;
					
			case BREAK: 
				if (*var_ptr == 1) { 
					exit = true; 
					break; 
				} 
				break;
			}
		}
	}

private:
	vector<Token> tokens;
	int* var_ptr = &vars[L"_"];
	
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

	vector<Token> find_cycle(size_t& i);
};


Parser::Parser() {};

Parser::Parser(const wstring& code) {
	Lexer tc(code);
	tokens = tc.to_tokens();
}

void Parser::load_tokens(const vector<Token>& t) noexcept { tokens = t; }

vector<Token> Parser::find_cycle(size_t& i) {
	vector<Token> res;
	for (int bracket_n = 1; bracket_n; i++) {
		switch (tokens[i].type) {
		case BGN_CYC:
			bracket_n++;
			break;
		case FNS_CYC:
			bracket_n--;
			break;
		}
		res.push_back(tokens[i]);
	}
	i--;
	return res;
}
