#include "Signature.h"
#include <iostream>


Signature::Signature(const signature_t& comps) : components{comps} {
	set_components(comps);
}

void Signature::set_components(const signature_t& comps) {
	vars_names_line.clear();
	components = comps;

	for (const auto& t : comps) {
		if (t.type == SignType::Var or t.type == SignType::MultipleVar) {
			vars_names_line.push_back(t.val);
		}
	}
}

bool Signature::check_coincidence(
	const vector<Lexeme>& lexems, size_t& len, func_arguments_t& args) const 
{
	assert(components.begin()->type != SignType::MultipleVar);
	assert((components.end()-1)->type != SignType::MultipleVar);

	auto var = vars_names_line.begin();
	auto lex = lexems.begin();
	for (auto comp=components.begin(); comp != components.end(); ++comp, ++lex) {
		if (lex == lexems.end()) return false;
		
		func_argument_t arg {};

		if (comp->type == SignType::Name) {
			if (comp->val != lex->val) return false;
		}
		else if (comp->type == SignType::Var) {
			arg = { *lex, };
		}
		else if (comp->type == SignType::MultipleVar) {
			assert((comp+1)->type != SignType::MultipleVar);

			Signature end_signature;
			end_signature.set_components(signature_t(comp+1, components.end()));
			size_t next_len = 0;
			func_arguments_t new_args = args;

			while (!end_signature.check_coincidence(vector<Lexeme>(lex, lexems.end()), next_len, new_args)) 
			{ 
				if (lex == lexems.end()) return false;
				arg.push_back(*lex);
				lex++; 
			}

			args = new_args;
			comp = components.end()-1;
			lex += next_len-1;
		}
		
		if (arg.size() > 0) {	// if not name but var
			if (args[*var].size() > 0 and args[*var] != arg) return false;

			args[*var] = arg;
			var++;
		}
	}

	len += std::distance(lexems.begin(), lex);

	return true;
}


Signature Signature::take_signature(const vector<Lexeme>& lexs) {
	signature_t signs {};

	for (auto lex=lexs.begin(); lex != lexs.end(); ++lex) {
		SignatureUnit sign;
		sign.val = lex->val;
			
		if (lex->val.find('$') != -1) 
			sign.type = SignType::Var;
		else 
			sign.type = SignType::Name;
	
		signs.push_back(sign);
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
