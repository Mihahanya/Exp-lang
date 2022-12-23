#pragma once

#include "TokenTypes.h"
#include <cassert>


enum class SignatureType {
	Var,
	MultipleVar,
	Name,
	None,
};
	
struct SignatureUnit {
	SignatureType type;
	string val;
};

class Signature
{
public:
	string name;
	vector<SignatureUnit> components {};

	int if_coincidence_len(const vector<Token>& tokens) const;
};
