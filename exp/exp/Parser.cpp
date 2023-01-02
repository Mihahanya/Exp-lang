#include "Parser.h"


Parser::Parser(const vector<Lexeme>& ls) 
	: lexems{ls} 
{
	funcs = new func_storage_t;
}

Parser::~Parser() {
	delete funcs;

	for (auto v : vars) delete v.second;
	vars.clear();
}


inline void Parser::exec_fov(const vector<Lexeme>& lexs) {
	Parser* field_parser = new Parser(lexs);
	field_parser->funcs = funcs;
	field_parser->vars = vars;

	field_parser->parse();
	field_parser->execute();

	// To not delete actoal data through pointers
	field_parser->funcs = nullptr;
	field_parser->vars.clear();

	delete field_parser;
}

#define left_arg vars[tok.arguments["a"][0].val] 
#define right_arg vars[tok.arguments["b"][0].val] 

void Parser::execute() 
{
	for (auto& tok : tokens) 
	{
		if (tok.func->type != BuiltinFunc::New and 
			tok.func->type != BuiltinFunc::NotBuiltin and
			tok.func->type != BuiltinFunc::DefineFunc) {
			for (const auto& args : tok.arguments) {
				// Watch initialization of unary variables only
				if (args.second.size() == 1 and vars[args.second[0].val] == nullptr) {
					throw runtime_error(
						some_error_at_lex("Uninitialized variable `" + args.second[0].val + "`", tok.info));
				}
			}
		}

		switch (tok.func->type) {
		using enum BuiltinFunc;

		case New: 
			for (const auto& name : tok.arguments["a"]) {
				//if (vars[name.val] == nullptr)
					vars[name.val] = new VarTy(0); 
				//else
				//	*vars[name.val] = 0;
			}
			break;

		case AssignVar:			*left_arg = *right_arg; break;
		case AssignPointer:		left_arg = right_arg; break;
		
		case Add:	*left_arg += *right_arg; break;
		case Sub:	*left_arg -= *right_arg; break;
		case Mul:	*left_arg *= *right_arg; break;
		case Div:	*left_arg /= *right_arg; break;
		case Mod:	*left_arg %= *right_arg; break;
		case Pow:	*left_arg = std::pow(*left_arg, *right_arg); break;
		case Root:	*left_arg = std::sqrt(*left_arg); break;
		case Neg:	*left_arg = *left_arg == 0; break;
		case Bool:	*left_arg = *left_arg != 0; break;
		case Or:	*left_arg |= *right_arg; break;
		case And:	*left_arg &= *right_arg; break;
		case Xor:	*left_arg ^= *right_arg; break;
		
		case Increase:	(*left_arg)++; break;
		case Decrease:	(*left_arg)--; break;
		
		case Input:		std::cin >> *left_arg; break;
		case Output:	std::cout << *left_arg; break;
		case OutputCh:	std::cout << (char)*left_arg; break;

		case If: 
			if (*left_arg != 0)
				exec_fov(tok.arguments["b"]);
			break;

		case IfElse: {
			func_argument_t block_tok;

			if (*left_arg != 0) 
				block_tok = tok.arguments["b"];
			else 
				block_tok = tok.arguments["c"];

			exec_fov(block_tok);
			break;
		}

		case NotBuiltin: {
			auto func_toks = tok.func->tokens;
			for (size_t i=0; i<func_toks.size(); ++i) {
				for (const auto& n : tok.func->signature.vars_names_line) {
					if (func_toks[i].val == n) {
						func_toks.erase(func_toks.begin()+i);
						func_toks.insert(func_toks.begin()+i, 
							tok.arguments[n].begin(), tok.arguments[n].end());

						i += tok.arguments[n].size()-1;
						break;
					}
				}
			}

			exec_fov(func_toks);
			break;
		}

		case Pass: break;

		}
	}
}

#undef left_arg 
#undef right_arg 

void Parser::parse() {
	init_builtin_funcs();

	// Define numbers and strings variables
	for (auto& l : lexems) {
		if (l.type == LexType::Number) {
			l.type = LexType::Name;
			vars[l.val] = new VarTy(stoi(l.val));
		}
		else if (l.type == LexType::String) {
			l.type = LexType::Name;
			vars[l.val] = new VarTy(0);

			for (size_t i=1; i < l.val.length()-1; ++i) {
				*vars[l.val] += (VarTy)l.val[i] * std::pow(256, i-1);
			}
		}
	}

	for (auto lex=lexems.begin(); lex!=lexems.end(); ++lex) {
		bool find_func = false;

		// Reverse find of function from new to old
		for (auto func = funcs->crbegin(); func != funcs->crend(); ++func) 
		{
			size_t if_func_len = 0;
			func_arguments_t args {};

			// If find function signautre
			if (func->signature.check_match(vector<Lexeme>(lex, lexems.end()), if_func_len, args)) 
			{
				// TODO: where add function by prefixes
				if (func->type == BuiltinFunc::DefineFunc) {
					// Define function
					Function f;
					f.type = BuiltinFunc::NotBuiltin;
					f.signature = Signature::take_signature(args["a"]);
					f.tokens = args["b"];
					
					funcs->push_back(f);
				}
				else if (func->type == BuiltinFunc::Include) {
					// Without first and end `'`
					string file_path = args["a"][0].val.substr(1, args["a"][0].val.length()-2); 
					include_script(file_path, lex->file_name);
				}
				else {
					// Add defined function token
					Token this_token;
					this_token.arguments = args;
					this_token.func = &*func;
					this_token.info = *lex; this_token.info.val.clear();

					tokens.push_back(this_token);
				}

				lex += if_func_len-1;
				find_func = true;

				break;
			}
		}
		
		// If not find throw error
		if (!find_func) {
			string this_line {};
			for (const auto& l : lexems) {
				if (l.line == lex->line)
					this_line += l.val + ' ';
			}
			throw runtime_error(
				some_error_at("Not defined function", lex->line, lex->chr_pos, lex->file_name) + 
							  "\n\t" + this_line);
		}
	}
}

void Parser::include_script(const string& file_path, const string& this_path)
{
	string incl_path = "";
	if (this_path.find_first_of("\\/") != -1) {
		incl_path = this_path.substr(0, this_path.find_last_of("\\/")) + "/";
	}
	
	incl_path += file_path + ".exp";
	const char* file_name = incl_path.c_str();

	string code = read_file_contents(file_name);
	Lexer lex(code, file_name);
					
	Parser* include_parser = new Parser(lex.lex_analysis());
	include_parser->funcs = funcs;
	include_parser->vars = vars;
	include_parser->parse();

	auto inc_toks = include_parser->tokens;
	tokens.insert(tokens.end(), inc_toks.begin(), inc_toks.end());

	vars = include_parser->vars;
	// To not delete actoal data through pointers
	include_parser->funcs = nullptr;
	include_parser->vars = {};
	delete include_parser;
}

void Parser::init_builtin_funcs() {
	if (funcs->size() > 0) return;

	Function include;
	include.type = BuiltinFunc::Include;
	include.signature = Signature({ 
		SignatureUnit(SignType::Name, "include"), 
		SignatureUnit(SignType::Var, "a"), 
	});
	funcs->push_back(include);

	Function pass;
	pass.type = BuiltinFunc::Pass;
	pass.signature = Signature({ 
		SignatureUnit(SignType::Name, "pass"), 
	});
	funcs->push_back(pass);

	Function replace;
	replace.type = BuiltinFunc::Replace;
	replace.signature = Signature({ 
		SignatureUnit(SignType::Name, "replace"), 
		SignatureUnit(SignType::MultiVar, "a"), 
		SignatureUnit(SignType::Name, "with"), 
		SignatureUnit(SignType::MultiVar, "b"), 
		SignatureUnit(SignType::Name, "end"), 
	});
	funcs->push_back(replace);

	Function func;
	func.type = BuiltinFunc::DefineFunc;
	func.signature = Signature({ 
		SignatureUnit(SignType::Name, "def"), 
		SignatureUnit(SignType::MultiVar, "a"), 
		SignatureUnit(SignType::Name, "beg"), 
		SignatureUnit(SignType::MultiVar, "b"), 
		SignatureUnit(SignType::Name, "end"), 
		SignatureUnit(SignType::Name, "def"), 
	});
	funcs->push_back(func);
	
	Function new_mul;
	new_mul.type = BuiltinFunc::New;
	new_mul.signature = Signature({ 
		SignatureUnit(SignType::Name, "new"), 
		SignatureUnit(SignType::MultiVar, "a"), 
		SignatureUnit(SignType::Name, "."), 
	});
	funcs->push_back(new_mul);
	
	Function inc;
	inc.type = BuiltinFunc::Increase;
	inc.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "inc"), 
	});
	funcs->push_back(inc);
	
	Function dec;
	dec.type = BuiltinFunc::Decrease;
	dec.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "dec"), 
	});
	funcs->push_back(dec);
	
	Function input;
	input.type = BuiltinFunc::Input;
	input.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "input"), 
	});
	funcs->push_back(input);

	Function output;
	output.type = BuiltinFunc::Output;
	output.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "print"), 
	});
	funcs->push_back(output);
	
	Function output_ch;
	output_ch.type = BuiltinFunc::OutputCh;
	output_ch.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "putch"), 
	});
	funcs->push_back(output_ch);
	
	//

	Function add;
	add.type = BuiltinFunc::Add;
	add.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "<+"), 
		SignatureUnit(SignType::Var, "b"), 
	});
	funcs->push_back(add);

	Function sub;
	sub.type = BuiltinFunc::Sub;
	sub.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "<-"), 
		SignatureUnit(SignType::Var, "b"), 
	});
	funcs->push_back(sub);

	Function mul;
	mul.type = BuiltinFunc::Mul;
	mul.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "<*"), 
		SignatureUnit(SignType::Var, "b"), 
	});
	funcs->push_back(mul);

	Function div;
	div.type = BuiltinFunc::Div;
	div.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "</"), 
		SignatureUnit(SignType::Var, "b"), 
	});
	funcs->push_back(div);

	Function pow;
	pow.type = BuiltinFunc::Pow;
	pow.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "<**"), 
		SignatureUnit(SignType::Var, "b"), 
	});
	funcs->push_back(pow);

	Function mod;
	mod.type = BuiltinFunc::Mod;
	mod.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "<%"), 
		SignatureUnit(SignType::Var, "b"), 
	});
	funcs->push_back(mod);

	Function root;
	root.type = BuiltinFunc::Root;
	root.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "root"), 
	});
	funcs->push_back(root);

	Function neg;
	neg.type = BuiltinFunc::Neg;
	neg.signature = Signature({ 
		SignatureUnit(SignType::Name, "not"), 
		SignatureUnit(SignType::Var, "a"), 
	});
	funcs->push_back(neg);
	
	Function boool;
	boool.type = BuiltinFunc::Bool;
	boool.signature = Signature({ 
		SignatureUnit(SignType::Name, "bool"), 
		SignatureUnit(SignType::Var, "a"), 
	});
	funcs->push_back(boool);

	Function orr;
	orr.type = BuiltinFunc::Or;
	orr.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "<|"), 
		SignatureUnit(SignType::Var, "b"), 
	});
	funcs->push_back(orr);

	Function andd;
	andd.type = BuiltinFunc::And;
	andd.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "<&"), 
		SignatureUnit(SignType::Var, "b"), 
	});
	funcs->push_back(andd);

	Function xorr;
	xorr.type = BuiltinFunc::Xor;
	xorr.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "<^"), 
		SignatureUnit(SignType::Var, "b"), 
	});
	funcs->push_back(xorr);

	//

	Function if_f;
	if_f.type = BuiltinFunc::If;
	if_f.signature = Signature({ 
		SignatureUnit(SignType::Name, "if"),
		SignatureUnit(SignType::Var, "a"),
		SignatureUnit(SignType::Name, "then"), 
		SignatureUnit(SignType::MultiVar, "b"),
		SignatureUnit(SignType::Name, "end"),
		SignatureUnit(SignType::Name, "if"),
	});
	funcs->push_back(if_f);

	Function elif_f;
	elif_f.type = BuiltinFunc::IfElse;
	elif_f.signature = Signature({ 
		SignatureUnit(SignType::Name, "if"),
		SignatureUnit(SignType::Var, "a"),
		SignatureUnit(SignType::Name, "then"), 
		SignatureUnit(SignType::MultiVar, "b"),
		SignatureUnit(SignType::Name, "else"),
		SignatureUnit(SignType::MultiVar, "c"),
		SignatureUnit(SignType::Name, "end"),
		SignatureUnit(SignType::Name, "if"),
	});
	funcs->push_back(elif_f);

	Function assign_var;
	assign_var.type = BuiltinFunc::AssignVar;
	assign_var.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "<<"),
		SignatureUnit(SignType::Var, "b"), 
	});
	funcs->push_back(assign_var);
	
	Function assign_pointer;
	assign_pointer.type = BuiltinFunc::AssignPointer;
	assign_pointer.signature = Signature({ 
		SignatureUnit(SignType::Var, "a"), 
		SignatureUnit(SignType::Name, "->"),
		SignatureUnit(SignType::Var, "b"), 
	});
	funcs->push_back(assign_pointer);
}

