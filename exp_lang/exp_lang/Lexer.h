#pragma once

#include "Token.h"
#include <map>

using std::smatch;

class Lexer
{
public:
	bool ok = true;

	Lexer(const string&);

	vector<Token> lex_analysis();

private:
	string code;
	
	std::map<TType, size_t> type_poss{};
	bool is_swap = false;

	void embed_inc_toks(const string&, vector<Token>&);
	void find_end_fov(vector<Token>::iterator);

	void add_token(const TType& tt, const string& value, vector<Token>& res);
	std::pair<TType, smatch> recognize_token(const string&, const size_t&);

	void err_exit(size_t line, size_t i);
};

Lexer::Lexer(const string& code) : code{code} {
	static const regex del_comm_reg{ R"(\|.*\|)" };
	this->code = std::regex_replace(this->code, del_comm_reg, string{""});
};

vector<Token> Lexer::lex_analysis() 
{
	vector<Token> res{};
	size_t line_n = 1;

	for (size_t i=0; i < code.length();) {
		if (code[i] == '\n') line_n++;

		auto [ttype, m] = recognize_token(code, i);

		if (ttype == NONE) { err_exit(line_n, i); return vector<Token>{}; }

		add_token(ttype, *(m.end()-1), res);
		i += m.str().length();
	}
	return res;
}

void Lexer::find_end_fov(vector<Token>::iterator t) {
	const auto end_fov = t;
	
	for (int bracket_n = 1; bracket_n; t--) {
		switch ((*t).type) {
		case BGN_FOV: bracket_n--; break;
		case END_FOV: bracket_n++; break;
		}
	}
	t++;
	t->num_val = end_fov-t+1; // end to `}`
}

void Lexer::embed_inc_toks(const string& t, vector<Token>& ts) {
	fs::path path = fs::current_path().wstring() + std::wstring(t.begin(), t.end()) + L".exp";
	string inc_code = read_txt(path);
	Lexer lx(inc_code);
	auto inc_toks = lx.lex_analysis();

	ts.insert(ts.end(), inc_toks.begin(), inc_toks.end());
}

std::pair<TType, smatch> Lexer::recognize_token(const string& code, const size_t& i) {
	const string sub_code = code.substr(i, 33);

	for (const TokType& tt : token_types_list) {
		if (type_poss[tt.name] > i) continue;

		smatch m;
		if (!std::regex_search(sub_code, m, tt.regex)) continue;

		const auto fnd = sub_code.find(m.str());

		if (fnd == 0) return { tt.name, m };
		else type_poss[tt.name] = fnd + i;
	}
	return { NONE, smatch() };
}

inline void Lexer::add_token(const TType& tt, const string& value, vector<Token>& res) {
	Token crr_token{ value, 0, tt };

	switch (tt) {
	case NUMBER: crr_token.num_val = std::stoi(value); break;
	case END_FOV: find_end_fov(res.end() - 1); break;
	}
	switch (tt) { // preprocessing
	case SPACE: break;
	case INCLUDE: embed_inc_toks(value, res); break;
	case SWAP: is_swap = true; break;

	default:
		res.push_back(crr_token);
		if (is_swap) { std::swap(*(res.end() - 1), *(res.end() - 2)); is_swap = false; }
	}
}

inline void Lexer::err_exit(size_t line, size_t i) {
	string unknown_token{};
	do { unknown_token += code[i]; i++; } 
	while (std::get<0>(recognize_token(code, i)) == NONE);

	printf(FRED("Unknown token (%s) in %i line!\n"), 
		unknown_token.c_str(), line);
	ok = false;
}
