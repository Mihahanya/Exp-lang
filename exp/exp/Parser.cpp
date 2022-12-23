#include "Parser.h"
#include <set>
#include <iostream>
#include <vector>
#include <string>


void Parser::execute() {
	vars_storage_t vars{};
	vars["_"] = -1;
	int* current_var = &vars["_"];

	// TODO: to iterators
	for (int i=0; i < high_tokens.size(); ++i) {
		if (high_tokens[i].type == HighTokenType::Simple) {
			Token curr_tok = *high_tokens[i].val.simpl;

			switch (curr_tok.type) {
			using enum TokenType;

			case Name:	current_var = &vars[curr_tok.val]; break;

			case NewVariable:
				i++;	
				vars[curr_tok.val] = 0; 
				current_var = &vars[curr_tok.val];
				break;

			case Condition:		if (*current_var == 0) i++; break;
			case Increase:		(*current_var)++; break;
			case Decrease:		(*current_var)--; break;
			case Input:			std::cin >> *current_var; break;
			case Output:		std::cout << *current_var; break;
			}
		}
		else if (high_tokens[i].type == HighTokenType::Simple) {
			std::cout << 'hi';
		}
	}
}


void Parser::parse() {
	tokens.erase(std::remove_if(tokens.begin(), tokens.end(), 
		[](Token t) { return t.type == TokenType::Space; }), tokens.end());

	std::set<string> exists_vars {};

	for (auto tok=tokens.begin(); tok!=tokens.end(); ++tok) 
	{
		HighToken this_token;
		
		bool is_proc = false;
		for (auto& proc : procs) {
			int if_proc_len = proc.signature.if_coincidence_len( vector<Token>(tok, tokens.end()) );
			if (if_proc_len != -1) {
				this_token.type = HighTokenType::Function;
				this_token.val.proc = &proc;

				tok += if_proc_len-1;
				is_proc = true;
				break;
			}
		}

		if (!is_proc) {
			switch (tok->type) {
			using enum TokenType;

			case Name:
				if (check_vars and exists_vars.find(tok->val) == exists_vars.end()) 
					throw std::runtime_error("Name `" + tok->val + "` is not defined at line " + std::to_string(tok->line) + " character " + std::to_string(tok->chr_pos)); 
				break;

			case NewVariable:
				//tok++;	
				exists_vars.insert((tok+1)->val);
				break;
			}

			this_token.type = HighTokenType::Simple;
			this_token.val.simpl = &*tok;
		}
		high_tokens.push_back(this_token);
	}
}
