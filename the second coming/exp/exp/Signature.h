#pragma once

#include <string>
#include <regex>
#include <vector>
#include <map>
#include <iostream>

using std::vector;
using std::map;
using std::string;
using std::cout;


enum class TT {
    Const, Var
};

class Token {
public:
    Token(string val, TT type) : val{val}, type{type} {}

    string val{};
    TT type;
};


class Signature {
public:
    friend class Proposition;


    Signature(const vector<Token>& signature) : signature{signature} {
        string reg_str {""};
        for (const auto& s : signature) {
            if (s.type == TT::Const) {
                reg_str += s.val;
                view += "`" + s.val + "` ";
            }
            else if (s.type == TT::Var) {
                reg_str += "(.+)";
                vars_order.push_back(s.val);

                view += "[" + s.val + "] ";
            }
        }
        reg = std::regex(reg_str);
        hash = std::hash<string>{}(view);
    }

    Signature() {}


    bool recognize_signature(const string& text, map<string, string>& vars_names) const {
        std::smatch match;
        if (std::regex_match(text, match, reg))
        {
            if (match.size()-1 != vars_order.size()) return false;

            for (std::size_t i = 1; i < match.size(); ++i)
            {
                std::string var_name = match[i].str();
                string var_name_mask = vars_order[i-1];

                if (vars_names.find(var_name_mask) == vars_names.end()) vars_names[var_name_mask] = var_name;
                else if (vars_names[var_name_mask] != var_name) return false;
            }

            return match.position() == 0 and match.length() == text.length();
        }
        return false;
    }

    bool same(const Signature& s) const {
        const auto& s_sign = s.signature;
        if (s_sign.size() != signature.size()) return false;

        map<string, string> vars_pattern {};

        for (int i=0; i < signature.size(); ++i) {
            if (s_sign[i].type != signature[i].type) return false;

            if (signature[i].type == TT::Const) {
                if (signature[i].val != s_sign[i].val) return false;
            }
            else if (signature[i].type == TT::Var) {
                if (vars_pattern.find(signature[i].val) == vars_pattern.end()) vars_pattern[signature[i].val] = s_sign[i].val;
                else if (vars_pattern[signature[i].val] != s_sign[i].val) return false;
            }
        }

        return true;
    }

    void print() const {
        cout << view << '\n';
    }

    string get_view() const {
        return view;
    }

    bool operator <(const Signature& rhs) const {
        return hash < rhs.hash;
    }

protected:
    vector<Token> signature{};

private:
    std::regex reg{};
    
    vector<string> vars_order{};
    
    string view {};
    size_t hash;
    
};