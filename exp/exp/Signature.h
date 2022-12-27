#pragma once

#include "Lexer.h"
#include <cassert>
#include <map>

using func_argument_t = vector<Lexeme>;
using func_arguments_t = std::map<string, func_argument_t>;

enum class SignType {
	Var,
	MultipleVar,
	Number,
	Name,
	OpenBrace,
	ClosBrace,
	None,
};
	
struct SignatureUnit {
	SignType type = SignType::None;
	string val;
};

class Signature
{
public:
	vector<string> vars_names {};

	Signature() {}
	Signature(const vector<SignatureUnit>&);

	void set_components(const vector<SignatureUnit>&);

	bool check_coincidence(const vector<Lexeme>&, int&, func_arguments_t&) const;
	static Signature take_signature(const vector<Lexeme>&);

private:
	vector<SignatureUnit> components {};
};
