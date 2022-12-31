#include "Parser.h"
#include <set>
#include <iostream>


void Parser::execute() 
{
	for (auto& tok : tokens) 
	{
		switch (tok.func.type) {
		using enum BuiltinFunc;

		case AssignVar:	vars[tok.arguments["a"][0].val] = vars[tok.arguments["b"][0].val]; break;
		
		case Increase:	vars[tok.arguments["a"][0].val]++; break;
		case Decrease:	vars[tok.arguments["a"][0].val]--; break;
		
		case Input:		std::cin >> vars[tok.arguments["a"][0].val]; break;
		case Output:	std::cout << vars[tok.arguments["a"][0].val]; break;
		case OutputCh:	std::cout << (char)vars[tok.arguments["a"][0].val]; break;

		case If: 
			if (vars[tok.arguments["a"][0].val] != 0) {
				Parser if_field_parser(tok.arguments["b"]);
				if_field_parser.funcs = funcs;
				if_field_parser.vars = vars;

				if_field_parser.parse();
				if_field_parser.execute();

				funcs = if_field_parser.funcs;
				vars = if_field_parser.vars;
			}
			break;

		case NotBuiltin:
			auto func_toks = tok.func.tokens;
			for (size_t i=0; i<func_toks.size(); ++i) 
			{
				for (const auto& n : tok.func.signature.vars_names_line) {
					if (func_toks[i].val == n) {
						func_toks.erase(func_toks.begin()+i);
						func_toks.insert(func_toks.begin()+i, 
							tok.arguments[n].begin(), tok.arguments[n].end());

						i += tok.arguments[n].size()-1;
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

			break;
		}
	}
}

void Parser::parse() {
	init_builtin_funcs();

	// Ignore spaces and comments
	lexems.erase(std::remove_if(lexems.begin(), lexems.end(), 
		[](Lexeme l) { return l.type == LexType::Space or l.type == LexType::LineComment; }), lexems.end());

	// Define numbers variables
	for (auto& l : lexems) {
		if (l.type == LexType::Number) {
			l.type = LexType::Name;
			vars[l.val] = stoi(l.val);
		}
	}

	for (auto lex=lexems.begin(); lex!=lexems.end(); ++lex) {
		bool find_func = false;

		// Reverse find of function from new to old
		for (auto func = funcs.begin(); func != funcs.end(); ++func) 
		{
			size_t if_func_len = 0;
			func_arguments_t args {};

			// If find function signautre
			if (func->signature.check_coincidence(vector<Lexeme>(lex, lexems.end()), if_func_len, args)) 
			{
				if (func->type == BuiltinFunc::DefineFunc) {
					// Define function
					Function f;
					f.type = BuiltinFunc::NotBuiltin;
					f.signature = Signature::take_signature(args["a"]);
					f.tokens = args["b"];
					
					funcs.push_back(f);
				}
				else {
					// Add defined function token
					Token this_token;
					this_token.arguments = args;
					this_token.func = *func;

					tokens.push_back(this_token);
				}

				lex += if_func_len-1;
				find_func = true;

				break;
			}
		}
		
		// If not find throw error
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

