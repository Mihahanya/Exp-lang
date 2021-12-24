#pragma once
#include <vector>
#include <string>

using namespace std;

struct token { 
	string val, type; 
};
typedef vector<token> t_vec;

const string 
USE_VAR = "use_var",
MOVE_VAL = "move_value",
PLUS = "plus",
MINUS = "minus",
TOBOOL = "tobool",
DIV = "div",
MUL = "mul",
PRINT = "print",
PRINT_CHAR = "print_ch",
INPUT_N = "input",
INPUT_CHAR = "input_ch",
BGN_CYC = "bgn_contain",
FNS_CYC = "fns_contain",
BREAK = "break",
OTHER = "OTHER";


class Tkn
{
public:
	Tkn (string code) {
		this->code = code;
	}

	t_vec to_tokens() {
		t_vec res;
		for (char l : code)
			res.push_back(check_to_t(string(1, l)));

		return res;
	}
	
private:
	string code;

	const t_vec token_patterns = {
		{"$", USE_VAR}, {"~", MOVE_VAL}, {"%", BREAK},
		{"+", PLUS}, {"-", MINUS}, {"?", TOBOOL}, {">", DIV}, {"<", MUL},
		{":", PRINT}, {";", PRINT_CHAR}, {".", INPUT_N}, {",", INPUT_CHAR},
		{"{", BGN_CYC}, {"}", FNS_CYC}
	};

	token check_to_t(string l) {
		for (token t : token_patterns) {
			if (l == t.val)
				return t;
		}
		return { l, "OTHER" };
	}
};
