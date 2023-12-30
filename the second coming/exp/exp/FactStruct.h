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


class FactStruct {
    friend class Proposition;

public:
    FactStruct() {}

    FactStruct(const vector<Token>& structure) : structure{structure} {
        string reg_str {""};
        for (const auto& s : structure) {
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

    bool recognize_structure(const string& text, map<string, string>& vars_names) const {
        vars_names = {};

        std::smatch match;
        if (std::regex_match(text, match, reg))
        {
            if (match.size()-1 != vars_order.size()) return false;

            for (std::size_t i = 1; i < match.size(); ++i)
            {
                std::string var_name = match[i].str();
                string var_name_mask = vars_order[i-1];

                if (!vars_names.contains(var_name_mask)) vars_names[var_name_mask] = var_name;
                else if (vars_names[var_name_mask] != var_name) return false;
            }

            return match.position() == 0 and match.length() == text.length();
        }
        return false;
    }

    bool same(const FactStruct& s) const {
        const auto& s_sign = s.structure;
        if (s_sign.size() != structure.size()) return false;

        map<string, string> vars_pattern {};

        for (int i=0; i < structure.size(); ++i) {
            if (s_sign[i].type != structure[i].type) return false;

            if (structure[i].type == TT::Const) {
                if (structure[i].val != s_sign[i].val) return false;
            }
            else if (structure[i].type == TT::Var) {
                if (!vars_pattern.contains(structure[i].val)) vars_pattern[structure[i].val] = s_sign[i].val;
                else if (vars_pattern[structure[i].val] != s_sign[i].val) return false;
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

    bool operator <(const FactStruct& rhs) const {
        return hash < rhs.hash;
    }

protected:
    vector<Token> structure{};

private:
    std::regex reg{};
    
    vector<string> vars_order{};
    
    string view {};
    size_t hash;
    
};