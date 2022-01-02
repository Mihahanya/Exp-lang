#pragma once
#include <vector>
#include <string>

using namespace std;

struct token { 
	string val, type; 
};
typedef vector<token> t_vec;

const string 
USE_VAR		= "use_var",
CHECH_VAR		= "get_val",
MOVE_VAL	= "move_value",
PLUS		= "plus",
MINUS		= "minus",
TOBOOL		= "tobool",
MORE		= "div",
LESS		= "mul",
PRINT		= "print",
PRINT_CHAR	= "print_ch",
INPUT_N		= "input",
INPUT_CHAR	= "input_ch",
BGN_CYC		= "bgn_contain",
FNS_CYC		= "fns_contain",
BREAK		= "break",
INCLUDE		= "include",
OTHER		= "OTHER";

const t_vec token_patterns = {
	{"$", USE_VAR}, {"&", CHECH_VAR}, {"~", MOVE_VAL}, {"%", BREAK},
	{"+", PLUS}, {"-", MINUS}, {"?", TOBOOL}, {">", MORE}, {"<", LESS},
	{":", PRINT}, {";", PRINT_CHAR}, {".", INPUT_N}, {",", INPUT_CHAR},
	{"{", BGN_CYC}, {"}", FNS_CYC}, {"\"", INCLUDE}
};

class Tkn
{
public:
	Tkn (string code) {
		this->code = code;
	}

	t_vec to_simple_tokens() {
		t_vec res;
		for (char l : code)
			res.push_back(check_to_t(string(1, l)));

		return res;
	}

	t_vec to_tokens() {
		t_vec st = to_simple_tokens(), nt;

		for (size_t i=0; i<st.size(); i++) {
			if (st[i].type == OTHER) {
				string ctn = "";
				while (st[i].type == OTHER) {
					ctn += st[i].val;
					i++;
				} i--;
				nt.push_back({ ctn, OTHER });
			}
			else nt.push_back(st[i]);
		}
		return nt;
	}
	
private:
	string code;

	token check_to_t(string l) {
		for (token t : token_patterns) {
			if (l == t.val)
				return t;
		}
		return { l, OTHER };
	}
};
