#pragma once

#include "Token.h"
#include <map>

class Lexer
{
public:
	bool ok = true;

	Lexer(const wstring&);

	vector<Token> lex_analysis();

private:
	wstring code;

	void embed_inc_toks(const wstring&, vector<Token>&);
	void find_end_fov(vector<Token>::iterator);

	void add_token(const TokenType& tt, vector<Token>& res, const wstring& value);
};

Lexer::Lexer(const wstring& code) : code(code) {
	static const wregex del_comm_reg{ LR"(\|.*\|)" };
	this->code = std::regex_replace(this->code, del_comm_reg, wstring{L""});
};

vector<Token> Lexer::lex_analysis() 
{
	vector<Token> res{};
	std::map<TType, size_t> type_poses{};
	size_t line_n = 1;

	for (size_t i=0; i < code.length();) {
		const wstring sub_code = code.substr(i, 33);
		bool is_find_token = false;
		if (sub_code[0] == '\n') line_n++;

		for (const TokenType& tt : token_types_list) {
			if (type_poses[tt.name] > i) continue;

			std::wsmatch m;
			if (!std::regex_search(sub_code, m, tt.regex)) continue;

			const auto fnd = sub_code.find(m.str());
			if (fnd == 0) {
				add_token(tt, res, *(m.end()-1));

				i += m.str().length();
				is_find_token = true;
				break;
			}
			else {
				type_poses[tt.name] = fnd+i;
			}
		}
		if (!is_find_token) {
			cerr << FRED("Unknown token in " + std::to_string(line_n) + " line!\n");
			ok = false;
			return vector<Token>{};
		}
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

void Lexer::embed_inc_toks(const wstring& t, vector<Token>& ts) {
	fs::path path = fs::current_path().wstring() + t + L".exp";
	wstring inc_code = read_txt(path);
	Lexer lx(inc_code);
	auto inc_toks = lx.lex_analysis();

	ts.insert(ts.end(), inc_toks.begin(), inc_toks.end());
}

inline void Lexer::add_token(const TokenType& tt, vector<Token>& res, const wstring& value) {
	static bool is_swap = false;

	Token crr_token{ value, 0, tt.name };

	switch (tt.name) {
	case NUMBER: crr_token.num_val = std::stoi(crr_token.str_val); break;
	case END_FOV: find_end_fov(res.end() - 1); break;
	}
	switch (tt.name) { // preprocessing
	case SPACE: break;
	case INCLUDE: embed_inc_toks(crr_token.str_val, res); break;
	case SWAP: is_swap = true; break;

	default:
		res.push_back(crr_token);
		if (is_swap) { std::swap(*(res.end() - 1), *(res.end() - 2)); is_swap = false; }
	}
}
