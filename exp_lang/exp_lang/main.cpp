#include <cstdlib>
#include <conio.h>
#include <time.h>

#include "executor.h"
#include "conv_code.h"
#include "utilities.h"

inline void execute(fs::path);

int main(int argc, char **argv)
{
    system("title Exp interpreter");

    string command;
    fs::path crr_path;

    if (argc == 1) {
        cout << init_msg;
        
        crr_path = fs::current_path(); 
        cout << crr_path << '\n';
    }
    else {
        crr_path = argv[1];
        command = "run";

        goto not_read;
    }

    while (true) 
    {
        cout << read_arrow;
        std::getline(cin, command);

        not_read:
        try {
            if (command == "run") execute(crr_path);

            else if (command == "help") cout << '\n' << pos_cmd;
            
            else if (command.substr(0, 3) == "dir") crr_path = command.substr(4);

            else if (command.substr(0, 3) == "scr") crr_path.replace_filename(command.substr(4)); 
            
            else if (command == "code") wcout << '\n' << read_txt(crr_path).c_str() << '\n';
            
            else if (command == "clr") system("cls");
            
            else if (command == "exit") exit(0);
            
            else if (!command.empty()) cout << " \x1b[31m[UNKNOWN COMMAND]\x1b[0m\nType `help` for more information\n";
        } 
        catch (std::exception e) {
            cout << "\x1b[31m[SOME ERROR]\x1b[0m\n";
        }
    }

    _getch();
    return 0;
}

inline void execute(fs::path path) {
    cout << "\x1b[32m\nCompilation...\x1b[0m"; 
                
    CodePreprocessor code; 
    code.load_file(path);
    code.convert_code();
    Execute ex(code.code); 

    system("cls"); 

    clock_t start_t = clock();

    ex.EXECUTE();
    cout << "\n\n\x1b[33mExecution time: " << float(clock() - start_t)/1000 << "s\x1b[0m";
}
