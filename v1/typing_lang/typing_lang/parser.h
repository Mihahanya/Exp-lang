#pragma once
#include <vector>
#include <string>

using namespace std;

// Token struct
struct token { 
	string val, type; 
	string print() {
		return "[TOKEN] Val: `"+ val + "`, Type `" + type + "`";
	}
};

// The possible token types
const vector<token> token_patterns = {
	{"_", "use_var"}, {"+", "plus"}, {"-", "minus"}, 
	{":", "print"}, {";", "print_char"}, {".", "input"}, {",", "input_char"}, 
	{"{", "begin_contain"}, {"}", "finish_contain"}, {"*", "do_in"},
	{">", "next_action"}, {"\n", "next_action"},
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

string sgm_t_to_str(vector<token> a, int from, int to) {
	string o = "";
	for (int i=from; i<to; i++)
		o += a[i].val;
	return o;
}

void print_vec_t(vector<token> a) {
	// Printing the vector of the tokens
	for (token t : a) 
		cout << t.print() << '\n';
}