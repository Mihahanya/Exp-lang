#include "fs.h"
#include "Program.h"


using std::vector;


int main()
{
    string code = read_file_contents("D:/PROJECTS/Exp-lang/the second coming/test_code/little test.exp"); 
    code = std::regex_replace(code, std::regex(R"(\\\s+)"), "");
    code = std::regex_replace(code, std::regex(R"(\.[ \t]*)"), "\n");

    Program prog;

    try {
        prog.eval(code);
    }
    catch (std::exception e) {
        cout << "some err:\n" << e.what();
    }

    cout << "-------\nstructures:\n";
    for (const auto& s : prog.fact_tree_nodes) {
        cout << s.first.get_scheme_view() << s.second->consequences.size() << " " << ":->\n";
        for (const auto& m : s.second->consequences)
            cout << "    " << m->fact.get_scheme_view() << '\n';
        cout << '\n';
    }

    cout << "-------\npropositions:\n";
    for (const auto& s : prog.propositions) cout << s.meaning() << "\n";

}


