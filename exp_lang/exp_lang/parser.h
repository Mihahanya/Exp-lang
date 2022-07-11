#pragma once

#include "utilities.h"

enum TokensType
{ 
	USE_VAR, CHECH_VAR,
	MOVE_VAL,
	PLUS, MINUS,
	TOBOOL,MORE,LESS,
	PRINT,PRINT_CHAR,
	INPUT_N,INPUT_CHAR,
	BGN_CYC,FNS_CYC,
	BREAK,
	INCLUDE,
	OTHER,
};

struct token {
	wstring val;
	TokensType type;
};

typedef vector<token> t_vec;

inline bool is_var_tkn(token t) {
	return t.type == USE_VAR || t.type == CHECH_VAR;
}

const t_vec token_patterns = {
	{L"$", USE_VAR}, {L"&", CHECH_VAR}, {L"~", MOVE_VAL}, 
	{L"+", PLUS}, {L"-", MINUS}, {L"?", TOBOOL}, {L">", MORE}, {L"<", LESS},
	{L":", PRINT}, {L";", PRINT_CHAR}, {L".", INPUT_N}, {L",", INPUT_CHAR},
	{L"{", BGN_CYC}, {L"}", FNS_CYC}, {L"%", BREAK}, {L"\"", INCLUDE}
};

class Tkn
{
public:
	Tkn (wstring code) {
		this->code = code;
	}

	t_vec to_simple_tokens() {
		t_vec res;
		for (char l : code)
			res.push_back(check_to_t(wstring(1, l)));

		return res;
	}

	t_vec to_tokens() {
		/// Connect a set of consecutive tokens with the OTHER type into one
		t_vec st = to_simple_tokens(), o;
		wstring ctn;
		for (size_t i=0; i<st.size(); i++) {
			if (st[i].type == OTHER) {
				ctn = L"";
				for (i; st[i].type == OTHER; ctn += st[i].val, i++); i--;
				o.push_back({ ctn, OTHER });
			}
			else o.push_back(st[i]);
		}
		return o;
	}
	
private:
	wstring code;

	token check_to_t(wstring l) {
		for (token t : token_patterns) {
			if (l == t.val)
				return t;
		}
		return { l, OTHER };
	}
};
