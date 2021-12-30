#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <conio.h>
#include <time.h>

#include "executor.h"
#include "conv_code.h"
#include "utilities.h"

inline void exct(fs::path path);

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "ru");
    system("title Exp interpreter");

    string command;
    fs::path path;

    if (argc == 1) {
        cout << init_msg;
        path = sgm(argv[0], 0, ((string)argv[0]).length()-12); cout << path << endl;
        read(command); 
    }
    else {
        path = argv[1];
        command = "!run";
    }

    while (true) {
        try {
            if (sgm(command, 0, 4) == "!run")           exct(path);
            else if (sgm(command, 0, 5) == "!code")     cout << endl << read_txt(path) << endl;
            else if (sgm(command, 0, 6) == "!move-")    { path += sgm(command, 6); cout << path << endl; }
            else if (sgm(command, 0, 6) == "!path-")    path = command;
            else if (sgm(command, 0, 5) == "!path")     cout << path << endl;
            else if (sgm(command, 0, 5) == "!back")     { back_path(path); cout << path << endl; }
            else if (sgm(command, 0, 6) == "!clear")    system("cls");
            else if (sgm(command, 0, 5) == "!exit")     exit(0);
            else cout << "\x1b[31m[UNKNOWN COMMAND]\x1b[0m\n" << pos_cmd;

            read(command);

        } catch (exception e) {
            cout << "\x1b[31m[SOME ERR]\x1b[0m\n";
        }
    }

    _getch();
    return 0;
}

inline void exct(fs::path path) {
    cout << "\x1b[32m\nCompilation...\x1b[0m"; 
                
    Code code; code.load_file(path); 
    code.normalise_code();
    Execute ex(code.code); 

    system("cls");
    clock_t start_t = clock();

    ex.EXECUTE();
    cout << "\n\n\x1b[33mExecution time: " << float(clock() - start_t)/1000 << "s\x1b[0m";
}
