#pragma once

#include "Signature.h"
#include <sstream>


class Proposition {
public:
    Proposition() {}

    Proposition(const Signature& sign, map<string, string> vars) : signature{sign}, vars{vars} {}

    string meaning() const {
        string res = "";

        for (const auto& s : signature.signature) {
            if (s.type == TT::Const) res += s.val;
            else if (s.type == TT::Var) res += vars.at(s.val);
        }

        return res;
    }

private:
    // TODO: pointer sign
    Signature signature {};
    map<string, string> vars {};

};

class Program {
public:
    // TODO: signatures organisation
    map<Signature, Signature> user_signatures {};
    vector<Proposition> propositions {};


    Program() {
        user_signatures[Signature{{ Token("ignore ", TT::Const), Token("a", TT::Var) }}] = Signature();
    }

    void eval(const string& code) {
        std::istringstream iss(code);
        for (std::string line; std::getline(iss, line); )
        {
            if (std::regex_replace(line, std::regex(R"(\s+)"), "").size() == 0) continue;

            bool ok = false;
            map<string, string> sign_vars{};


            if (new_var_sign.recognize_signature(line, sign_vars)) {
                string name = sign_vars["a"];
                user_signatures[Signature({Token(name, TT::Const)})] = Signature({Token(name, TT::Const)});

                ok = true;
            }
            else if (change_sign.recognize_signature(line, sign_vars)) {
                Signature change_sign(split_signature(sign_vars["a"]));
                Signature to_sign(split_signature(sign_vars["b"]));

                if (user_signatures.find(change_sign) == user_signatures.end()) {
                    string err = "[!nothing to change at:\n\t```" + line + "```]\n";
                    throw std::exception(err.c_str());
                }

                user_signatures[change_sign] = to_sign;

                ok = true;
            }
            else if (let_sign.recognize_signature(line, sign_vars)) {
                Signature let_sign(split_signature(sign_vars["a"]));
                Signature to_sign(split_signature(sign_vars["b"]));

                if (user_signatures.find(let_sign) != user_signatures.end()) {
                    string err = "[!reassignment at:\n\t```" + line + "```]\n";
                    throw std::exception(err.c_str());
                }

                user_signatures[let_sign] = to_sign;

                ok = true;
            }
            else {
                for (const auto& sign : user_signatures) {
                    if (sign.first.recognize_signature(line, sign_vars)) {
                        propositions.push_back(Proposition{sign.first, sign_vars});
                        propositions.push_back(Proposition{sign.second, sign_vars});
                        ok = true;
                        break;
                    }
                }
            }

            if (!ok) {
                string err = "[!unrecognized token at:\n\t```" + line + "```]\n";
                throw std::exception(err.c_str());
            }
        }
    }

private:
    const Signature new_var_sign  {{ Token("let be ", TT::Const), Token("a", TT::Var) }}; // TODO: embedded
    const Signature let_sign      {{ Token("let ", TT::Const), Token("a", TT::Var), Token(" mean ", TT::Const), Token("b", TT::Var) }};
    const Signature change_sign   {{ Token("let now ", TT::Const), Token("a", TT::Var), Token(" mean ", TT::Const), Token("b", TT::Var) }};


    vector<Token> split_signature(const string& code) {
        vector<Token> res {};
        //std::regex var_reg("[A-Z0-9]+");
        std::regex var_reg(R"([A-Z]+(\s[A-Z]+)*)");
        int prev_var_end = 0;

        for (std::sregex_iterator i=std::sregex_iterator(code.begin(), code.end(), var_reg); i != std::sregex_iterator(); ++i)
        {
            std::smatch m = *i;
            if (m.position() != 0) {
                string val(code.begin()+prev_var_end, code.begin()+m.position());
                res.push_back(Token(val, TT::Const));
            }
            res.push_back(Token(m.str(), TT::Var));

            prev_var_end = m.position() + m.length();
        }

        if (prev_var_end < code.size()) {
            string val(code.begin()+prev_var_end, code.end());
            res.push_back(Token(val, TT::Const));
        }

        return res;
    }
};
