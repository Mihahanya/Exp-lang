#include "Parser.h"
#include <set>
#include <vector>
#include <string>
#include <iostream>


void Parser::execute() 
{
	for (auto tok=tokens.begin(); tok!=tokens.end(); ++tok) 
	{
		string first_var_name = tok->arguments[0][0].val;

		switch (tok->func->type) {
		using enum BuiltinFunc;

		case New:		vars[first_var_name] = 0; break;
		
		case Increase:	vars[first_var_name]++; break;
		case Decrease:	vars[first_var_name]--; break;
		
		case Input:		std::cin >> vars[first_var_name]; break;
		case Output:	std::cout << vars[first_var_name]; break;
		case OutputCh:	std::cout << (char)vars[first_var_name]; break;

		case AssignNum:	vars[first_var_name] = stoi(tok->arguments[1][0].val); break;
		
		case If: 
			if (vars[first_var_name] != 0) {
				Parser if_field_parser(tok->arguments[1]);
				if_field_parser.funcs = funcs;
				if_field_parser.vars = vars;

				if_field_parser.parse();
				if_field_parser.execute();

				funcs = if_field_parser.funcs;
				vars = if_field_parser.vars;
			}
			break;

		default:
			std::cout << "hi";
		}
	}
}


void Parser::parse() {
	init_builtin_funcs();

	lexems.erase(std::remove_if(lexems.begin(), lexems.end(), 
		[](Lexeme l) { return l.type == LexType::Space or l.type == LexType::LineComment; }), lexems.end());

	std::set<string> exists_vars {};

	// TODO: make var existing checking
	for (auto lex=lexems.begin(); lex!=lexems.end(); ++lex) {
		Token this_token;
		
		bool find_func = false;
		for (auto func = funcs.rbegin(); func != funcs.rend(); ++func) 
		{
			int if_func_len;
			arguments args;

			if (func->signature.check_coincidence(vector<Lexeme>(lex, lexems.end()), if_func_len, args)) {
				this_token.arguments = args;
				this_token.func = &*func;

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


void Parser::init_builtin_funcs() {
	Function new_f;
	new_f.type = BuiltinFunc::New;
	new_f.signature = Signature({ 
		SignatureUnit(SignatureType::Name, "new"), 
		SignatureUnit(SignatureType::Var, "a"), 
	});
	funcs.push_back(new_f);

	Function inc;
	inc.type = BuiltinFunc::Increase;
	inc.signature = Signature({ 
		SignatureUnit(SignatureType::Var, "a"), 
		SignatureUnit(SignatureType::Name, "inc"), 
	});
	funcs.push_back(inc);
	
	Function dec;
	dec.type = BuiltinFunc::Decrease;
	dec.signature = Signature({ 
		SignatureUnit(SignatureType::Var, "a"), 
		SignatureUnit(SignatureType::Name, "dec"), 
	});
	funcs.push_back(dec);
	
	Function input;
	input.type = BuiltinFunc::Input;
	input.signature = Signature({ 
		SignatureUnit(SignatureType::Var, "a"), 
		SignatureUnit(SignatureType::Name, "input"), 
	});
	funcs.push_back(input);

	Function output;
	output.type = BuiltinFunc::Output;
	output.signature = Signature({ 
		SignatureUnit(SignatureType::Var, "a"), 
		SignatureUnit(SignatureType::Name, "print"), 
	});
	funcs.push_back(output);
	
	Function output_ch;
	output_ch.type = BuiltinFunc::OutputCh;
	output_ch.signature = Signature({ 
		SignatureUnit(SignatureType::Var, "a"), 
		SignatureUnit(SignatureType::Name, "putch"), 
	});
	funcs.push_back(output_ch);

	Function if_f;
	if_f.type = BuiltinFunc::If;
	if_f.signature = Signature({ 
		SignatureUnit(SignatureType::Name, "if"),
		SignatureUnit(SignatureType::Var, "a"),
		SignatureUnit(SignatureType::Name, "then"), 
		SignatureUnit(SignatureType::MultipleVar, "b"),
		SignatureUnit(SignatureType::Name, "end"),
	});
	if_f.into_vars_names = { "a", "b" };
	funcs.push_back(if_f);

	Function assign_num;
	assign_num.type = BuiltinFunc::AssignNum;
	assign_num.signature = Signature({ 
		SignatureUnit(SignatureType::Var, "a"), 
		SignatureUnit(SignatureType::Number, "b"), 
	});
	funcs.push_back(assign_num);
}

