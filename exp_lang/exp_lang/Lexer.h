#pragma once

#include "Token.h"
#include <map>

class Lexer
{
public:
	Lexer(const wstring&);

	vector<Token> to_tokens();

private:
	wstring code;

	void embed_inc_toks(const wstring&, vector<Token>&);
};

Lexer::Lexer(const wstring& code) : code(code) {
	static const wregex del_comm_reg{ LR"(\|.*\|)" };
	this->code = std::regex_replace(this->code, del_comm_reg, wstring{L""});
};

vector<Token> Lexer::to_tokens() {
	vector<Token> res{};
	std::map<TType, size_t> type_poses{};

	for (size_t i=0; i < code.length(); i++) {
		const wstring& sub_code = code.substr(i, 33);

		for (const TokenType& tt : token_types_list) {
			const TType current_type = tt.name;
			
			if (type_poses[current_type] > i) continue;

			std::wsmatch m;
			std::regex_search(sub_code, m, tt.regex);

			if (m.empty()) continue;

			const wstring match_str = m.str();
			const auto fnd = sub_code.find(match_str);

			if (fnd == 0) {
				if (current_type == INCLUDE) embed_inc_toks(*(m.end()-1), res);
				else if (current_type != SPACE) {
					res.push_back(Token{ .val=*(m.end()-1), .type=current_type });
				}
				i += match_str.length()-1;
				break;
			}
			else {
				type_poses[current_type] = fnd+i;
			}
		}
	}
	return res;
}

void Lexer::embed_inc_toks(const wstring& t, vector<Token>& ts) {
	fs::path path = fs::current_path().wstring() + t + L".exp";
	wstring inc_code = read_txt(path);
	Lexer lx(inc_code);
	auto inc_toks = lx.to_tokens();

	ts.insert(ts.end(), inc_toks.begin(), inc_toks.end());
}
