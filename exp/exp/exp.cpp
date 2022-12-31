#include "Parser.h"
#include "Colored.h"
#include <fstream>
#include <sstream>
#include <chrono>


namespace conf {
    const string version = "Exp 4.0, 2021-2023";

    const string init_msg =
	"\n     _________________________\n"
	"     _____ __    __    _____\n"
	"   /  __  /\\  \\/  /  /  __  /\n"
	"  /  ___ /  )    /  /  /_/ /\n"
	" /  /___   /    (  /  ___ /\n"
	"/_____ /  /__/\\__\\/_ /\n"
	"________________________\n"
	"\n"
	"" + version + "\n"
    "\n"
	"https://github.com/Mihahanya/Exp-lang\n";

    const string help;
};


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

int main(int argc, char *argv[]) try
{
    string code {};

    if (argc == 1) {
        cout << conf::init_msg;

        //code = read_file_contents("test.exp");
        //
        //Lexer lex(code);
        //auto lexemes = lex.lex_analysis();
        //
        //Parser parser(lexemes);
        //parser.parse();
        //
        //parser.execute();
    }
    else if (argc == 2) {
        if (std::strcmp(argv[1], "--version") == 0) {
            cout << conf::version << "\n";
        }
        else {
            code = read_file_contents(argv[1]);
        
            Lexer lex(code);
            auto lexemes = lex.lex_analysis();
        
            Parser parser(lexemes);
            parser.parse();
        
            parser.execute();        
        }
    }
    else if (argc == 3) {
        if (std::strcmp(argv[2], "-t") == 0) {
            using namespace std::chrono;

            code = read_file_contents(argv[1]);
        
            Lexer lex(code);
            auto lexemes = lex.lex_analysis();
        
                auto parse_start = std::chrono::high_resolution_clock::now();

            Parser parser(lexemes);
            parser.parse();
        
                auto parse_stop_exec_start = std::chrono::high_resolution_clock::now();

            parser.execute();
        
                auto exec_stop = std::chrono::high_resolution_clock::now();

            duration<float> parse_duration = parse_stop_exec_start - parse_start;
            duration<float> exec_duration = exec_stop - parse_stop_exec_start;

            cout << "\n\nParsing: " << parse_duration << " s\n" 
                    "Execution: " << exec_duration << " s\n";
        }
    }
    else {
        cout << "Unknown arguments: ";
        for (int i=0; i<argc; i++) cout << argv[i] << " ";
    }
    return 0; 
}
catch (std::runtime_error e) {
    //std::cout << RED << e.what() << RESET << '\n';
    std::cout << e.what() << '\n';
}
