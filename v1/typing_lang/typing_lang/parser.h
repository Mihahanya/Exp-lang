#pragma once
#include <vector>
#include <string>

using namespace std;

// Token struct
struct token { 
	string val, type; 

	string print() {
		return "[TOKEN] Val: `"+ val + "`, Type: `" + type + "`";
	}
};

// The possible token types
const vector<token> token_patterns = {
	{"_", "use_var"}, {"~", "move_value"},
	{"+", "plus"}, {"-", "minus"}, 
	{":", "print"}, {";", "print_ch"}, {".", "input"}, {",", "input_ch"}, 
	{"{", "bgn_contain"}, {"}", "fns_contain"}, 
	{"", "OTHER"}
};

token check_to_t(string l) {
	// Convert a litter to the token
	for (token t : token_patterns) {
		if (l == t.val)
			return t;
	}
	return { l, token_patterns.back().type };
}

vector<token> to_tokens(string code) {
	// Convert a code to the vector of the tokens
	vector<token> res;
	for (char l : code) {
		string a = ""; a += l; // kostil
		res.push_back(check_to_t(a));
	}
	return res;
}

vector<token> sgm_of_t(vector<token> a, int from, int to) {
	vector<token> o;
	for (int i=from; i<to; i++)
		o.push_back(a[i]);
	return o;
}

void print_vec_t(vector<token> a) {
	// Printing the vector of the tokens
	for (token t : a) 
		cout << t.print() << '\n';
}
