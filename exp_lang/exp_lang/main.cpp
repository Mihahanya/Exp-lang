#include <cstdlib>
#include <conio.h>
#include <time.h>

#include "Parser.h"

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
            
            else if (command == "clear") system("cls");
            
            else if (command == "exit") exit(0);
            
            else if (!command.empty()) wcout << FRED("[Unknown command]\n") << "Type `help` for more information\n";
        }
        catch (...) {
            cerr << FRED("[Some error]\n");
        }
    }

    _getch();
    return 0;
}

inline void execute(fs::path path) {
    wcout << FYEL(L"Compilation start\n") << L"Script: " << path.filename() << '\n'; 
    
    clock_t start_comp = clock();

    wstring code = read_txt(path);
    Parser ex(code); 

    if (ex.ok) system("cls");
    
    clock_t start_ex = clock();

    ex.run();

    printf(FGRN("\n\nCompilation status: %.3fs\nExecution time: %.3fs\n"), 
        float(start_ex-start_comp)/1000, float(clock()-start_ex)/1000);
}
