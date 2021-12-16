﻿#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <conio.h>
#include <time.h>

#include "executor.h"
#include "conv_code.h"
#include "utilities.h"


int main(int argc, char **argv)
{
    setlocale(LC_ALL, "ru");
    system("title Exp interpreter");

    string command, path;

    if (argc == 1) {
        cout << read_txt("init_msg.txt");
        path = sgm(argv[0], 0, ((string)argv[0]).length()-12); cout << path << endl;
        read(command); 
    }
    else {
        path = argv[1];
        command = "!run";
    }

    while (true) {
        try {
            if (sgm(command, 0, 4) == "!run") {
                system("cls");
                
                Code code;
                code.load_file(path);
                code.normalise_code();

                Execute ex(code.code);
                clock_t start_t = clock();
                ex.EXECUTE();

                cout << "\n\n\x1b[33mExecution time: " << float(clock() - start_t)/1000 << "s\x1b[0m";
            }
            else if (sgm(command, 0, 5) == "!code") cout << endl << read_txt(path) << endl;
            else if (sgm(command, 0, 6) == "!move-") { path += sgm(command, 6); cout << path << endl; }
            else if (sgm(command, 0, 6) == "!path-") path = command;
            else if (sgm(command, 0, 5) == "!path") cout << path << endl;
            else if (sgm(command, 0, 5) == "!back") { back_path(path); cout << path << endl; }
            else if (sgm(command, 0, 6) == "!clear") system("cls");
            else if (sgm(command, 0, 5) == "!exit") exit(0);
            else cout << "\x1b[31m[UNKNOWN COMMAND]\x1b[0m\n" << read_txt("terminal commands.txt");;

            read(command);

        } catch (exception e) {
            cout << "\x1b[31m[SOME ERR]\x1b[0m\n";
        }
    }

    _getch();
    return 0;
}
