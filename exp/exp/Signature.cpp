#include "Signature.h"


int Signature::if_coincidence_len(const vector<Token>& tokens) const {
	assert(components.begin().type != SignatureType::MultipleVar and 
			(components.end()-1).type != SignatureType::MultipleVar);

	auto token = tokens.begin();
	for (const auto& comp : components) {
		if (token == tokens.end()) return -1;

		switch (comp.type) {
		using enum SignatureType;

		case Var:
			if (token->val.find('$') == -1) return -1;
			token++;
			break;
			
		case MultipleVar:
			if (token->val.find('$') == -1) return -1;
			while (token->val.find('$') != -1) token++;
			break;

		case Name:
			if (comp.val != token->val) return -1;
			token++;
			break;
		}
	}

	return std::distance(tokens.begin(), token);
}
