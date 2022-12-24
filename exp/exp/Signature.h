#pragma once

#include "Parser.h"
#include <cassert>


enum class SignatureType {
	Var,
	MultipleVar,
	Number,
	Name,
};
	
struct SignatureUnit {
	SignatureType type;
	string val;
};

class Signature
{
public:
	vector<SignatureUnit> components {};

	Signature() {}
	Signature(const vector<SignatureUnit>& comps) : components{comps} {}

	bool check_coincidence(const vector<Lexeme>&, int&, f_arguments_t&) const;

};
