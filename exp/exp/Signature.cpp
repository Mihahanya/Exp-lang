#include "Signature.h"
#include <iostream>


bool Signature::check_coincidence(
	const vector<Lexeme>& lexems, int& len, f_arguments_t& args) const 
{
	assert(components.begin().type != SignatureType::MultipleVar and 
			(components.end()-1).type != SignatureType::MultipleVar);

	auto lex = lexems.begin();
	for (auto comp=components.begin(); comp!=components.end(); ++comp) {
		if (lex == lexems.end()) return false;
		
		f_argument_t arg;
		
		switch (comp->type) {
		using enum SignatureType;

		case Var:
			args.push_back({ *lex, });
			lex++;
			break;
			
		case MultipleVar:
			assert((comp+1)->type != SignatureType::MultipleVar);

			arg = {}; 
			while (lex->val != (comp+1)->val and lex != lexems.end()) { 
				arg.push_back(*lex);
				lex++; 
			}
			if (lex == lexems.end()) return false;

			args.push_back(arg);
			break;

		case Name:
			if (comp->val != lex->val) return false;
			lex++;
			break;

		case Number:
			if (comp->val != lex->val or lex->type != LexType::Number) return false;
			break;
		}
	}

	len = std::distance(lexems.begin(), lex);

	return true;
}
