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

    cout << "structures:\n";
    for (const auto& s : prog.statements_tree_nodes) {
        cout << s.first.get_view() << s.second->consequences.size() << " " << ":->\n";
        for (const auto& m : s.second->consequences)
            cout << "    " << m->sign.get_view() << '\n';
    }

    cout << "propositions:\n";
    for (const auto& s : prog.propositions) cout << s.meaning() << "\n";

}


