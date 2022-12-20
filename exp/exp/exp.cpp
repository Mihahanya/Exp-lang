#include "Execute.h"
#include "Lexer.h"
#include "Colored.h"
#include <fstream>
#include <stdexcept>
#include <sstream>

using std::cout;


string read_file_contents(const char* path) {
    string line, res;
    std::ifstream myfile(path);
    if (myfile.is_open()) {
        while (getline(myfile, line)) res += line + '\n';
        myfile.close();
    }
    else throw std::runtime_error("Unable to open file " + string(path));

    return res;
}

// TODO: flags for time of execution and compiling, var existing chekcing
int main(int argc, char *argv[]) try 
{
    if (argc == 1) {
        string code = read_file_contents("test.exp");
        
        Lexer lex(code);
        auto tokens = lex.lex_analysis();

        /*for (const auto& t : tokens) {
            std::cout << t.val << ' ' << t.line << '\n';
        }*/
        
        exec(tokens);
    }
    else if (argc == 2) {
        
    }

    return 0; 
}
//catch (std::runtime_error e) {
catch (std::exception e) {
    std::cout << RED << e.what() << RESET << '\n';
}
