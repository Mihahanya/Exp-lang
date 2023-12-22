#include <iostream>
#include "fs.h"
#include <vector>
#include <map>
#include <string>
#include <regex>
#include <sstream>

using std::vector;


enum class TT {
    Sign, Val
};

class Lexeme {
public:
    Lexeme(string val, TT type) : val{val}, type{type} {}

    string val{};
    TT type;
};

class Signature {
public:
    Signature(const vector<Lexeme> signature) : signature{signature} {
        string reg_str {""};
        for (const auto& s : signature) {
            if (s.type == TT::Sign) {
                reg_str += s.val;
            }
            else if (s.type == TT::Val) {
                reg_str += "(.+)";
                vars_order.push_back(s.val);
            }
        }
        reg = std::regex(reg_str);
    }

    bool recognize_signature(const string& text, std::map<string, string>& vars_names) const {
        std::smatch match;
        if (std::regex_match(text, match, reg))
        {
            if (match.size()-1 != vars_order.size()) return false;

            for (std::size_t i = 1; i < match.size(); ++i)
            {
                std::string var_name = match[i].str();
                string var_name_mask = vars_order[i-1];

                if (vars_names.find(var_name_mask) == vars_names.end()) vars_names[var_name_mask] = var_name;
                if (vars_names[var_name_mask] != var_name) return false;
            }

            return true;
        }
        return false;
    }

    void print() const {
        string p = "";
        for (const auto& s : signature) {
            if (s.type == TT::Sign) p += "`" + s.val + "` ";
            else if (s.type == TT::Val) p += "[" + s.val + "] ";
        }
        std::cout << p << '\n';
    }

private:
    vector<Lexeme> signature{};
    std::regex reg{};
    
    vector<string> vars_order{};
    
};


int main()
{
    string code = read_file_contents("D:/PROJECTS/Exp-lang/the second coming/test_code/abo.exp"); 
    code = std::regex_replace(code, std::regex(R"(\\\s+)"), "");

    Signature new_var_sign  ({ Lexeme("let be ", TT::Sign), Lexeme("a", TT::Val) });
    Signature let_sign      ({ Lexeme("let ", TT::Sign), Lexeme("a", TT::Val), Lexeme(" mean ", TT::Sign), Lexeme("b", TT::Val) });
    Signature change_sign   ({ Lexeme("change ", TT::Sign), Lexeme("a", TT::Val), Lexeme(" to ", TT::Sign), Lexeme("b", TT::Val) });

    // TODO: signatures organisation
    vector<Signature> user_signatures {};

    
    std::istringstream iss(code);
    for (std::string line; std::getline(iss, line); )
    {
        if (std::regex_replace(line, std::regex(R"(\s+)"), "").size() == 0) continue;

        bool ok = false;
        std::map<string, string> sign_vars{};

        if (new_var_sign.recognize_signature(line, sign_vars)) {
            string name = sign_vars["a"];
            user_signatures.push_back(Signature({Lexeme(name, TT::Sign)}));

            ok = true;
        }
        else {
            for (const auto& sign : user_signatures) {
                if (sign.recognize_signature(line, sign_vars)) {
                    std::cout << "bin\n";
                    ok = true;
                }
            }
        }

        if (!ok) std::cout << "[!unrecognized token at:\n\t```" << line << "```]\n";
    }

    std::cout << "signatures:\n";
    for (const auto& s : user_signatures) s.print();


}


