#pragma once

#include "Lexer.h"
#include <cassert>
#include <map>

using func_argument_t = vector<Lexeme>;
using func_arguments_t = std::map<string, func_argument_t>;

enum class SignType {
	Var,
	MultiVar,
	Name,
	None,
};
	
struct SignatureUnit {
	SignType type = SignType::None;
	string val;
};


inline bool operator==(const SignatureUnit& l, const SignatureUnit& r) {
	return l.type == r.type and l.val == r.val;
}

inline bool operator!=(const SignatureUnit& l, const SignatureUnit& r) {
	return !(l == r);
}

using signature_t = vector<SignatureUnit>;


class Signature
{
public:
	vector<string> vars_names_line {};

	Signature() {}
	Signature(const signature_t&);

	void set_components(const signature_t&);

	bool check_match(const vector<Lexeme>&, size_t&, func_arguments_t&) const;
	static Signature take_signature(const vector<Lexeme>&);

private:
	signature_t components {};

};
