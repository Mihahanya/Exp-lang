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

	void embed_inc_toks(vector<Token>&);
};

Lexer::Lexer(const wstring& code) : code(code) {
	static const wregex del_reg{ LR"(\s|\|.*\|)" };
	this->code = std::regex_replace(this->code, del_reg, wstring{L""});
};

vector<Token> Lexer::to_tokens() {
	vector<Token> res{};
	std::map<TType, size_t> type_poses{};

	for (size_t i=0; i < code.length(); i++) {
		const wstring sub_code = code.substr(i, 33);

		for (const TokenType& tt : token_types_list) {
			if (type_poses[tt.name] > i) continue;

			std::wsmatch m;
			std::regex_search(sub_code, m, tt.regex);

			if (m.empty()) continue;

			const wstring match_str = m.str();
			const auto fnd = sub_code.find(match_str);

			if (fnd == 0) {
				res.push_back(Token{ .val=*(m.end()-1), .type=tt.name });
				i += match_str.length()-1;
				break;
			}
			else {
				type_poses[tt.name] = fnd+i;
			}
		}
		if ((*(res.end()-1)).type == INCLUDE) embed_inc_toks(res);
	}
	return res;
}

void Lexer::embed_inc_toks(vector<Token>& ts) {
	auto last = ts.end() - 1;

	fs::path path = fs::current_path().wstring() + (*last).val + L".exp";
	wstring inc_code = read_txt(path);
	Lexer lx(inc_code);
	auto inc_toks = lx.to_tokens();

	ts.erase(last);
	ts.insert(ts.end(), inc_toks.begin(), inc_toks.end());
}
