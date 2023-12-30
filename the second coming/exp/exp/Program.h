#pragma once

#include "FactStruct.h"
#include <sstream>
#include <set>


class Proposition {
public:
    Proposition() {}

    Proposition(const FactStruct& sign, map<string, string> vars) : structure{sign}, vars{vars} {}

    string meaning() const {
        string res = "";

        for (const auto& s : structure.structure) {
            if (s.type == TT::Const) res += s.val;
            else if (s.type == TT::Var) res += vars.at(s.val);
        }

        return res;
    }

private:
    // TODO: pointer sign
    FactStruct structure {};
    map<string, string> vars {};

};


class ReasoningNode {
public:
    FactStruct sign;
    vector<ReasoningNode*> consequences{};
    //vector<ReasoningNode*> premises{};

    ReasoningNode(const FactStruct& sign) : sign{sign} {}

    ~ReasoningNode() {
        for (auto p : consequences) delete p; 
        consequences.clear();

        //for (auto p : premises) delete p; 
        //premises.clear();
    }
};


class Program {
public:
    // TODO: structures organisation
    map<FactStruct, ReasoningNode*> statements_tree_nodes {};
    vector<Proposition> propositions {};


    Program() {
        // built-in
        statements_tree_nodes[let_sign] = new ReasoningNode(FactStruct{{Token("built-in let-mean", TT::Const)}});
        statements_tree_nodes[comment_sign] = new ReasoningNode(FactStruct{{Token("built-in ignore comments", TT::Const)}});
    }

    void eval(const string& code) 
    {
        vector<string> proposition_lines{};

        std::istringstream iss(code);
        for (std::string line; std::getline(iss, line); )
        {
            if (std::regex_replace(line, std::regex(R"(\s+)"), "").size() == 0) continue;

            map<string, string> sign_vars{};
            if (let_sign.recognize_structure(line, sign_vars)) {
                FactStruct let_sign(split_structure(sign_vars["a"]));
                FactStruct mean_sign(split_structure(sign_vars["b"]));

                if (!statements_tree_nodes.contains(let_sign))
                    statements_tree_nodes[let_sign] = new ReasoningNode(let_sign);

                if (!statements_tree_nodes.contains(mean_sign))
                    statements_tree_nodes[mean_sign] = new ReasoningNode(mean_sign);

                statements_tree_nodes[let_sign]->consequences.push_back(statements_tree_nodes[mean_sign]);
            }
            else {
                proposition_lines.push_back(line);
            }
        }

        for (const auto& line : proposition_lines) {
            auto props = to_reason(line);

            if (props.empty()) {
                string err = "[!unrecognized token at:\n\t```" + line + "```]\n";
                throw std::exception(err.c_str());
            }
            else {
                propositions.insert(propositions.end(), props.begin(), props.end());
            }
        }
    }

private:
    const FactStruct let_sign {{ Token("let ", TT::Const), Token("a", TT::Var), Token(" mean ", TT::Const), Token("b", TT::Var) }};
    const FactStruct comment_sign {{ Token("ignore ", TT::Const), Token("a", TT::Var) }};

    vector<Proposition> to_reason(const string& statement) {
        for (const auto& sign : statements_tree_nodes) {
            map<string, string> sign_vars{};
            if (sign.first.recognize_structure(statement, sign_vars)) {
                std::set<ReasoningNode*> cons_statements{};
                dfs(sign.second, cons_statements);

                vector<Proposition> res {};
                for (const auto& st : cons_statements) {
                    Proposition p{st->sign, sign_vars};
                    res.push_back(p);
                }
                return res;
            }
        }
    }

    void dfs(ReasoningNode* node, std::set<ReasoningNode*>& result) {
        result.insert(node);

        for (ReasoningNode* consequence : node->consequences) {
            if (!result.contains(consequence)) {
                dfs(consequence, result);
            }
        }
    }

    vector<Token> split_structure(const string& code) {
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
