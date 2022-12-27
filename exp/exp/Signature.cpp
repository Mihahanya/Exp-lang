#include "Signature.h"
#include <iostream>


Signature::Signature(const vector<SignatureUnit>& comps) : components{comps} {
	set_components(comps);
}

void Signature::set_components(const vector<SignatureUnit>& comps) {
	vars_names.clear();
	components = comps;

	for (const auto& t : comps) {
		if (t.type == SignType::Var or t.type == SignType::MultipleVar or t.type == SignType::Number) {
			vars_names.push_back(t.val);
			std::cout << 'c';
		}
	}
}

bool Signature::check_coincidence(
	const vector<Lexeme>& lexems, int& len, func_arguments_t& args) const 
{
	assert(components.begin().type != SignType::MultipleVar and 
			(components.end()-1).type != SignType::MultipleVar);

	auto var = vars_names.begin();
	auto lex = lexems.begin();
	for (auto comp=components.begin(); comp!=components.end(); ++comp) {
		if (lex == lexems.end()) return false;
		
		func_argument_t arg;
		
		switch (comp->type) {
		using enum SignType;

		case Var:
			if (lex->type == LexType::Number) return false;
			//if (args[*var].size() > 0 and args[*var] != func_argument_t{ *lex, }) return false;
			args[*var] = { *lex, };
			var++;
			lex++;
			break;
			
		case MultipleVar:
			assert((comp+1)->type != SignType::MultipleVar);

			arg = {}; 
			while (lex->val != (comp+1)->val and lex != lexems.end()) { 
				arg.push_back(*lex);
				std::cout << 'b';
				lex++; 
			}
			if (lex == lexems.end()) return false;
			//if (args[*var].size() > 0 and args[*var] != arg) return false;

			args[*var] = arg;
			var++;
			break;

		case Name:
			if (comp->val != lex->val) return false;
			lex++;
			break;

		case Number:
			if (lex->type != LexType::Number) return false;
			//if (args[*var].size() > 0 and args[*var] != func_argument_t{ *lex, }) return false;
			args[*var] = { *lex, };
			var++;
			lex++;
			break;
		}
	}

	len = std::distance(lexems.begin(), lex);

	return true;
}


Signature Signature::take_signature(const vector<Lexeme>& lexs) {
	vector<SignatureUnit> signs {};

	for (auto lex=lexs.begin(); lex != lexs.end(); ++lex) {
		SignatureUnit sign;
		sign.val = lex->val;
			
		if (lex->val.find('$') != -1) {
			sign.type = SignType::Var;
		}
		/*else if (lex->val.find('\'') != -1) {
			sign.type = SignType::Number;
		}*/
		else {
			sign.type = SignType::Name;
		}

		signs.push_back(sign);
		std::cout << 'a';
	}

	if (signs.size() >= 3) {
		for (auto r=signs.begin()+1; r != signs.end()-1; ++r) {
			if ( ((r-1)->type == SignType::Var or (r-1)->type == SignType::Name) and 
				 (r+1)->type == SignType::Name)
				r->type = SignType::MultipleVar;
		}
	}

	return Signature(signs);
}
