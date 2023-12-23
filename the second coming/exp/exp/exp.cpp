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
                view += "`" + s.val + "` ";
            }
            else if (s.type == TT::Val) {
                reg_str += "(.+)";
                vars_order.push_back(s.val);

                view += "[" + s.val + "] ";
            }
        }
        reg = std::regex(reg_str);
    }

    Signature() {}


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

            std::cout << text << match.position() << match.length() << "\n";
            return match.position() == 0 and match.length() == text.length();
        }
        return false;
    }

    void print() const {
        std::cout << view << '\n';
    }

    string get_view() const {
        return view;
    }

    bool operator <(const Signature& rhs) const {
        return view < rhs.view;
    }

    bool operator() (const Signature& lhs, const Signature& rhs) const {
        return lhs.view < rhs.view;
    }

private:
    vector<Lexeme> signature{};
    std::regex reg{};
    
    vector<string> vars_order{};
    
    string view {};
    
};


class Program {
public:
    // TODO: signatures organisation
    std::map<Signature, Signature> user_signatures {};


    Program() {}

    void eval(const string& code) {
        std::istringstream iss(code);
        for (std::string line; std::getline(iss, line); )
        {
            if (std::regex_replace(line, std::regex(R"(\s+)"), "").size() == 0) continue;

            bool ok = false;
            std::map<string, string> sign_vars{};

            if (new_var_sign.recognize_signature(line, sign_vars)) {
                string name = sign_vars["a"];
                user_signatures[Signature({Lexeme(name, TT::Sign)})] = Signature({Lexeme(name, TT::Sign)});

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
            else {
                for (const auto& sign : user_signatures) {
                    if (sign.first.recognize_signature(line, sign_vars)) {
                        std::cout << sign.first.get_view() << " " << line << "bin\n";
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
    const Signature new_var_sign  {{ Lexeme("let be ", TT::Sign), Lexeme("a", TT::Val) }};
    const Signature let_sign      {{ Lexeme("let ", TT::Sign), Lexeme("a", TT::Val), Lexeme(" mean ", TT::Sign), Lexeme("b", TT::Val) }};
    const Signature change_sign   {{ Lexeme("change ", TT::Sign), Lexeme("a", TT::Val), Lexeme(" to ", TT::Sign), Lexeme("b", TT::Val) }};


    vector<Lexeme> split_signature(const string& code) {
        vector<Lexeme> res {};
        //std::regex var_reg("[A-Z0-9]+");
        std::regex var_reg(R"([A-Z]+(\s[A-Z]+)*)");
        int prev_var_end = 0;

        for (std::sregex_iterator i=std::sregex_iterator(code.begin(), code.end(), var_reg); i != std::sregex_iterator(); ++i)
        {
            std::smatch m = *i;
            if (m.position() != 0) {
                string val(code.begin()+prev_var_end, code.begin()+m.position());
                res.push_back(Lexeme(val, TT::Sign));
            }
            res.push_back(Lexeme(m.str(), TT::Val));

            prev_var_end = m.position() + m.length();
        }
        return res;
    }
};


int main()
{
    string code = read_file_contents("D:/PROJECTS/Exp-lang/the second coming/test_code/abo.exp"); 
    code = std::regex_replace(code, std::regex(R"(\\\s+)"), "");

    Program prog;

    try {
        prog.eval(code);
    }
    catch (std::exception e) {
        std::cout << "some err:\n" << e.what();
    }

    std::cout << "signatures:\n";
    for (const auto& s : prog.user_signatures) s.first.print();


}


