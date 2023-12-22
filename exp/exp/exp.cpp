#include "Parser.h"
#include "Colored.h"
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


int main(int argc, char *argv[]) try
{
    string code {};

    if (argc == 1) {
        //cout << conf::init_msg;
        const char* path = "D:\\PROJECTS\\Exp-lang\\examples\\ex1.exp";
        code = read_file_contents(path);
        
        Lexer lex(code, path);
        auto lexemes = lex.lex_analysis();
        
        Parser parser(lexemes);
        parser.parse();
        
        parser.execute();
    }
    else if (argc == 2) {
        if (std::strcmp(argv[1], "--version") == 0) {
            cout << conf::version << "\n";
        }
        else {
            code = read_file_contents(argv[1]);
        
            Lexer lex(code, argv[1]);
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
        
            Lexer lex(code, argv[1]);
            auto lexemes = lex.lex_analysis();
        
                auto parse_start = std::chrono::high_resolution_clock::now();

            Parser parser(lexemes);
            parser.parse();
        
                auto parse_stop_exec_start = std::chrono::high_resolution_clock::now();

            parser.execute();
        
                auto exec_stop = std::chrono::high_resolution_clock::now();

            duration<float> parse_duration = parse_stop_exec_start - parse_start;
            duration<float> exec_duration = exec_stop - parse_stop_exec_start;

            csl::set_console_color(CYEL);
            cout << "\n\nParsing: " << parse_duration << " s\n" 
                    "Execution: " << exec_duration << " s\n";
            csl::reset_console_color();
        }
    }
    else {
        cout << "Unknown arguments: ";
        for (int i=0; i<argc; i++) cout << argv[i] << " ";
    }
    return 0; 
}
catch (runtime_error e) {
    //std::cout << RED << e.what() << RESET << '\n';
    csl::set_console_color(CRED);
    std::cout << e.what() << '\n';
    csl::reset_console_color();
}
catch (std::exception e) {
    std::cout << "very bad news :( ->" << e.what() << '\n';
}
