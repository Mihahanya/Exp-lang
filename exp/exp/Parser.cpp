#include "Parser.h"
#include <set>
#include <vector>
#include <string>
#include <iostream>


void Parser::execute() 
{
	for (auto tok=tokens.begin(); tok!=tokens.end(); ++tok) 
	{
		switch (tok->func->type) {
		using enum BuiltinFunc;

		case AssignNum:	vars[tok->arguments["a"][0].val] = stoi(tok->arguments["b"][0].val); break;
		case AssignVar:	vars[tok->arguments["a"][0].val] = vars[tok->arguments["b"][0].val]; break;
		
		case Increase:	vars[tok->arguments["a"][0].val]++; break;
		case Decrease:	vars[tok->arguments["a"][0].val]--; break;
		
		case Input:		std::cin >> vars[tok->arguments["a"][0].val]; break;
		case Output:	std::cout << vars[tok->arguments["a"][0].val]; break;
		case OutputCh:	std::cout << (char)vars[tok->arguments["a"][0].val]; break;

		case If: 
			if (vars[tok->arguments["a"][0].val] != 0) {
				Parser if_field_parser(tok->arguments["b"]);
				if_field_parser.funcs = funcs;
				if_field_parser.vars = vars;

				if_field_parser.parse();
				if_field_parser.execute();

				funcs = if_field_parser.funcs;
				vars = if_field_parser.vars;
			}
			break;

		default:
			auto func_toks = tok->func->tokens;
			for (int i=0; i<func_toks.size(); ++i) 
			{
				for (const auto& n : tok->func->signature.vars_names) {
					if (func_toks[i].val == n) {
						func_toks.erase(func_toks.begin()+i);
						func_toks.insert(func_toks.begin()+i, 
							tok->arguments[n].begin(), tok->arguments[n].end());

						i += tok->arguments[n].size()-1;
						std::cout << 'z';
						break;
					}
				}
			}

			Parser procedure(func_toks);

			procedure.funcs = funcs;
			procedure.vars = vars;

			procedure.parse(); 
			procedure.execute();

			funcs = procedure.funcs;
			vars = procedure.vars;
			//for (auto v=vars.begin(); v!=vars.end(); ++v) vars[v->first] = procedure.vars[v->first];
		}
	}
}


void Parser::parse() {
	init_builtin_funcs();

	lexems.erase(std::remove_if(lexems.begin(), lexems.end(), 
		[](Lexeme l) { return l.type == LexType::Space or l.type == LexType::LineComment; }), lexems.end());

	for (auto lex=lexems.begin(); lex!=lexems.end(); ++lex) {
		bool find_func = false;
		for (auto func = funcs.rbegin(); func != funcs.rend(); ++func) 
		{
			int if_func_len;
			func_arguments_t args;

			if (func->signature.check_coincidence(vector<Lexeme>(lex, lexems.end()), if_func_len, args)) {
				assert(func->signature.vars_names.size() == args.size());

				if (func->type == BuiltinFunc::DefineFunc) {
					Function f;
					f.type = BuiltinFunc::NotBuiltin;
					f.signature = Signature::take_signature(args["a"]);
					f.tokens = args["b"];
					
					funcs.push_back(f);
					std::cout << 'f';
				}
				else {
					Token this_token;
					this_token.arguments = args;
					this_token.func = &*func;

					tokens.push_back(this_token);
					std::cout << 'l';
				}

				lex += if_func_len-1;
				find_func = true;

				break;
			}
		}
		
		if (!find_func) {
			string line {};
			for (const auto& l : lexems) {
				if (l.line == lex->line)
					line += l.val + ' ';
			}
			throw std::runtime_error("Not defined function at line " + std::to_string(lex->line) + " character " + std::to_string(lex->chr_pos) + "\n\t" + line);
		}
	}
}


void Parser::init_builtin_funcs() {
	if (funcs.size() > 0) return;

	Function inc;
	inc.type = BuiltinFunc::Increase;
	inc.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "inc"), 
	});
	funcs.push_back(inc);
	
	Function dec;
	dec.type = BuiltinFunc::Decrease;
	dec.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "dec"), 
	});
	funcs.push_back(dec);
	
	Function input;
	input.type = BuiltinFunc::Input;
	input.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "input"), 
	});
	funcs.push_back(input);

	Function output;
	output.type = BuiltinFunc::Output;
	output.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "print"), 
	});
	funcs.push_back(output);
	
	Function output_ch;
	output_ch.type = BuiltinFunc::OutputCh;
	output_ch.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "putch"), 
	});
	funcs.push_back(output_ch);

	Function if_f;
	if_f.type = BuiltinFunc::If;
	if_f.signature = Signature({ 
		SignatureUnit(SignType::Name, "if"),
		SignatureUnit(SignType::Var, "a"),
		SignatureUnit(SignType::Name, "then"), 
		SignatureUnit(SignType::MultipleVar, "b"),
		SignatureUnit(SignType::Name, "endif"),
	});
	funcs.push_back(if_f);

	Function assign_num;
	assign_num.type = BuiltinFunc::AssignNum;
	assign_num.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "<<"),
		SignatureUnit(SignType::Number, "b"), 
	});
	funcs.push_back(assign_num);

	Function assign_var;
	assign_var.type = BuiltinFunc::AssignVar;
	assign_var.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "<<"),
		SignatureUnit(SignType::Var, "b"), 
	});
	funcs.push_back(assign_var);

	Function func;
	func.type = BuiltinFunc::DefineFunc;
	func.signature = Signature({ 
		SignatureUnit(SignType::Name, "def"), 
		SignatureUnit(SignType::MultipleVar, "a"), 
		SignatureUnit(SignType::Name, "beg"), 
		SignatureUnit(SignType::MultipleVar, "b"), 
		SignatureUnit(SignType::Name, "endef"), 
	});
	funcs.push_back(func);

	Function replace;
	replace.type = BuiltinFunc::Replace;
	replace.signature = Signature({ 
		SignatureUnit(SignType::Name, "replace"), 
		SignatureUnit(SignType::MultipleVar, "a"), 
		SignatureUnit(SignType::Name, "with"), 
		SignatureUnit(SignType::MultipleVar, "b"), 
		SignatureUnit(SignType::Name, "end"), 
	});
	funcs.push_back(replace);
}

