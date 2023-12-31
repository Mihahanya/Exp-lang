#pragma once

#include "FactStruct.h"
#include <sstream>
#include <set>


class Proposition {
public:
    Proposition() {}

    Proposition(const FactStruct& fact, map<string, string> vars) : fact{fact}, vars{vars} {}

    string meaning() const {
        string res = "";

        for (const auto& s : fact.structure) {
            if (s.type == TT::Const) res += s.val;
            else if (s.type == TT::Var) res += vars.at(s.val);
        }

        return res;
    }

private:
    // TODO: pointer struct
    FactStruct fact {};
    map<string, string> vars {};

};


class ReasoningNode {
public:
    FactStruct fact;
    vector<ReasoningNode*> consequences{};
    //vector<ReasoningNode*> premises{};

    ReasoningNode(const FactStruct& fact) : fact{fact} {}

    ~ReasoningNode() {
        for (auto p : consequences) delete p; 
        consequences.clear();

        //for (auto p : premises) delete p; 
        //premises.clear();
    }
};


class Program {
public:
    // TODO: fact organisation to differentiate:
    // `let a mean b` and `let a, b mean c`
    map<FactStruct, ReasoningNode*> fact_tree_nodes {};
    vector<Proposition> propositions {};


    Program() {
        // built-in
        fact_tree_nodes[let_sign] = new ReasoningNode(FactStruct{{Token("built-in let-mean", TT::Const)}});
        fact_tree_nodes[comment_sign] = new ReasoningNode(FactStruct{{Token("built-in ignore comments", TT::Const)}});
    }

    void eval(const string& code) 
    {
        vector<string> proposition_lines{};

        // Find let construction and collect facts 
        std::istringstream iss(code);
        for (std::string line; std::getline(iss, line); )
        {
            if (line.find_first_not_of(" \t") == std::string::npos) continue;

            map<string, string> fact_vars{};
            if (let_sign.recognize_fact(line, fact_vars)) {
                FactStruct let_fact(split_structure(fact_vars["A"]));
                FactStruct mean_fact(split_structure(fact_vars["B"]));

                // TODO: exceptions if empty

                if (!fact_tree_nodes.contains(let_fact))
                    fact_tree_nodes[let_fact] = new ReasoningNode(let_fact);

                if (!fact_tree_nodes.contains(mean_fact))
                    fact_tree_nodes[mean_fact] = new ReasoningNode(mean_fact);

                fact_tree_nodes[let_fact]->consequences.push_back(fact_tree_nodes[mean_fact]);
            }
            else {
                proposition_lines.push_back(line);
            }
        }

        // Deploying the tree
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
    const FactStruct let_sign {{ Token("let ", TT::Const), Token("A", TT::Var), Token(" mean ", TT::Const), Token("B", TT::Var) }};
    const FactStruct comment_sign {{ Token("ignore ", TT::Const), Token("A", TT::Var) }};

    vector<Proposition> to_reason(const string& statement) {
        for (const auto& fact_and_node : fact_tree_nodes) {
            map<string, string> fact_vars{};
            if (fact_and_node.first.recognize_fact(statement, fact_vars)) {
                std::set<ReasoningNode*> cons_nodes{};
                dfs(fact_and_node.second, cons_nodes);

                vector<Proposition> cons_props {};
                for (const auto& cons_node : cons_nodes) {
                    Proposition p{cons_node->fact, fact_vars};
                    cons_props.push_back(p);
                }
                return cons_props;
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

        for (std::sregex_iterator i = std::sregex_iterator(code.begin(), code.end(), var_reg); 
            i != std::sregex_iterator(); ++i) 
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
