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
        // Initialize regular expression and vars order to recognize fact by structure,
        // schematic representation of the fact structure (scheme_view) and hash by scheme_view
        string reg_str {""};
        for (const auto& s : structure) {
            if (s.type == TT::Const) {
                reg_str += s.val;
                scheme_view += "{" + s.val + "} ";
            }
            else if (s.type == TT::Var) {
                reg_str += "(.+)";
                vars_order.push_back(s.val);

                scheme_view += "[" + s.val + "] ";
            }
        }
        reg = std::regex(reg_str);
        hash = std::hash<string>{}(scheme_view);
    }

    bool recognize_fact(const string& text, map<string, string>& ret_vars_vals) const {
        ret_vars_vals = {};

        std::smatch match;
        if (std::regex_match(text, match, reg))
        {
            if (match.size()-1 != vars_order.size() or 
                match.position() != 0 or 
                match.length() != text.length()) return false;


            for (std::size_t i = 1; i < match.size(); ++i)
            {
                std::string find_val = match[i].str();
                string mask_var_name = vars_order[i-1];

                // for cases when vars sequences like ABA 
                // we check whether the next value of an already found var matches
                if (!ret_vars_vals.contains(mask_var_name)) ret_vars_vals[mask_var_name] = find_val;
                else if (ret_vars_vals[mask_var_name] != find_val) return false;
            }

            return true;
        }
        return false;
    }

    string get_scheme_view() const {
        return scheme_view;
    }

    bool operator <(const FactStruct& rhs) const {
        return hash < rhs.hash;
    }

protected:
    // sequence of tokens like: 
    // { Token("let ", TT::Const), Token("A", TT::Var), Token(" mean ", TT::Const), Token("B", TT::Var) }
    vector<Token> structure{};  

private:
    std::regex reg{};   // like: let (.+) mean (.+)
    
    vector<string> vars_order{};    // like: AB
    
    string scheme_view {};     // like: {let } [A] { mean } [B]
    size_t hash;
    
};