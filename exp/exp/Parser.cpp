#include "Parser.h"
#include "Signature.h"
#include <set>
#include <vector>
#include <string>
#include <iostream>


struct Parser::Function {
	Signature signature{};
	vector<string> into_vars_names { "a" };
	vector<Token> tokens{}; 
};

struct Parser::Token {
	Function func;
	f_arguments_t arguments;
};


void Parser::execute() {
	vars_storage_t vars{};
	vars["_"] = -1;
	int* current_var = &vars["_"];

	for (auto lex=tokens.begin(); lex != tokens.end(); ++lex) {
		std::cout << "hi";
	}
}


void Parser::parse() {
	init_buildin_funcs();

	lexems.erase(std::remove_if(lexems.begin(), lexems.end(), 
		[](Lexeme l) { return l.type == LexType::Space; }), lexems.end());

	std::set<string> exists_vars {};

	for (auto lex=lexems.begin(); lex!=lexems.end(); ++lex) {
		Token this_token;
		
		bool find_func = false;
		for (auto& func : funcs) 
		{
			int if_func_len;
			f_arguments_t args;

			if (func.signature.check_coincidence(vector<Lexeme>(lex, lexems.end()), if_func_len, args)) {
				this_token.arguments = args;
				this_token.func = func;

				lex += if_func_len-1;
				find_func = true;
				break;
			}
		}
		
		if (!find_func) 
			throw std::runtime_error("Not defined function at line " + std::to_string(lex->line) + " character " + std::to_string(lex->chr_pos));

		tokens.push_back(this_token);
	}
}


void Parser::init_buildin_funcs() {
	Function new_f;
	new_f.signature = Signature({ 
		SignatureUnit(SignatureType::Name, "new"), 
		SignatureUnit(SignatureType::Var, "a"), 
	});
	funcs.push_back(new_f);

	Function inc;
	inc.signature = Signature({ 
		SignatureUnit(SignatureType::Var, "a"), 
		SignatureUnit(SignatureType::Name, "inc"), 
	});
	funcs.push_back(inc);
	
	Function dec;
	dec.signature = Signature({ 
		SignatureUnit(SignatureType::Var, "a"), 
		SignatureUnit(SignatureType::Name, "dec"), 
	});
	funcs.push_back(dec);
	
	Function input;
	input.signature = Signature({ 
		SignatureUnit(SignatureType::Var, "a"), 
		SignatureUnit(SignatureType::Name, "input"), 
	});
	funcs.push_back(input);

	Function output;
	output.signature = Signature({ 
		SignatureUnit(SignatureType::Var, "a"), 
		SignatureUnit(SignatureType::Name, "print"), 
	});
	funcs.push_back(output);

	Function if_f;
	if_f.signature = Signature({ 
		SignatureUnit(SignatureType::Var, "a"), 
		SignatureUnit(SignatureType::Name, "if"), 
		SignatureUnit(SignatureType::Var, "b"),
	});
	if_f.into_vars_names = { "a", "b" };
	funcs.push_back(if_f);

	Function assign_num;
	assign_num.signature = Signature({ 
		SignatureUnit(SignatureType::Var, "a"), 
		SignatureUnit(SignatureType::Number, "b"), 
	});
	funcs.push_back(assign_num);
}

