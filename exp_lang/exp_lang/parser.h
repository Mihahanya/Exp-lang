#pragma once
#include <vector>
#include <string>

using namespace std;

struct token { 
	string val, type; 
};
typedef vector<token> t_vec;

const t_vec token_patterns = {
	{"_", "use_var"}, {"~", "move_value"},
	{"+", "plus"}, {"-", "minus"}, {"?", "tobool"}, {">", "div"}, {"<", "mul"}, 
	{":", "print"}, {";", "print_ch"}, {".", "input"}, {",", "input_ch"}, 
	{"{", "bgn_contain"}, {"}", "fns_contain"}
}; // number and name

token check_to_t(string l) {
	for (token t : token_patterns) {
		if (l == t.val)
			return t;
	}
	return { l, "OTHER" };
}

t_vec to_tokens(string code) {
	t_vec res;
	for (char l : code) 
		res.push_back(check_to_t(string(1, l)));

	return res;
}

t_vec sgm_of_t(t_vec a, size_t from, size_t to) {
	t_vec o;
	for (size_t i=from; i<to; i++)
		o.push_back(a[i]);
	return o;
}
