#include "Signature.h"


Signature::Signature(const signature_t& comps) : components{comps} {
	set_components(comps);
}

void Signature::set_components(const signature_t& comps) {
	vars_names_line.clear();
	components = comps;

	for (const auto& t : comps) {
		if (t.type == SignType::Var or t.type == SignType::MultiVar) {
			vars_names_line.push_back(t.val);
		}
	}
}

bool Signature::check_match(
	const vector<Lexeme>& lexems, size_t& len, func_arguments_t& args) const 
{
	assert(components.cbegin()->type != SignType::MultiVar);
	assert((components.cend()-1)->type != SignType::MultiVar);

	auto var = vars_names_line.cbegin();
	auto lex = lexems.cbegin();
	for (auto comp=components.cbegin(); comp != components.cend(); ++comp, ++lex) {
		if (lex == lexems.cend()) return false;
		
		func_argument_t arg {};

		if (comp->type == SignType::Name) {
			// Just check name
			if (comp->val != lex->val) return false;
		}
		else if (comp->type == SignType::Var) {
			// Just one current var
			arg = { *lex, };
		}
		else if (comp->type == SignType::MultiVar) {
			assert((comp+1)->type != SignType::MultiVar);

			// Find until next token sequence not match with next signs

			Signature end_signature;
			end_signature.set_components(signature_t(comp+1, components.cend()));
			size_t next_len = 0;
			func_arguments_t new_args = args;

			while (!end_signature.check_match(vector<Lexeme>(lex, lexems.cend()), next_len, new_args)) 
			{ 
				if (lex == lexems.cend()) return false;
				arg.push_back(*lex);
				++lex; 
			}

			// Copy args and breake all
			args = new_args;
			comp = components.cend()-1;
			lex += next_len-1;
		}
		
		// if not name but var
		if (arg.size() > 0) { 
			// If we have the variable with not empty value and the 
			// same name like current then check to matching values
			if (args[*var].size() > 0 and args[*var] != arg) return false;

			args[*var] = arg;
			++var;
		}
	}

	len += std::distance(lexems.cbegin(), lex);

	return true;
}


Signature Signature::take_signature(const vector<Lexeme>& lexs) {
	signature_t signs {};

	// This is a variable if there is a `$` sign in the name, else part of the name
	for (auto lex=lexs.cbegin(); lex != lexs.cend(); ++lex) {
		SignatureUnit sign;
		sign.val = lex->val;
			
		if (lex->val.find('$') != -1) 
			sign.type = SignType::Var;
		else 
			sign.type = SignType::Name;
	
		signs.push_back(sign);
	}

	// Var can be multiple if it not at the bounds and name on the right
	if (signs.size() >= 3) {
		for (auto r=signs.begin()+1; r != signs.end()-1; ++r) {
			if ((r+1)->type == SignType::Name) 
				r->type = SignType::MultiVar;
		}
	}

	return Signature(signs);
}
