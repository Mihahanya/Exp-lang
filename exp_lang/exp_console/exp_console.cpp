#include "conf.h"

inline void execute(fs::path);

int main(int argc, char** argv)
{
    LPCWSTR title{ L"Exp interpreter" };
    SetConsoleTitle(title);
    system("");

    string command;
    fs::path crr_path;

    if (argc == 1) {
        cout << csl::init_msg;

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
        cout << csl::read_arrow;
        std::getline(cin, command);

        not_read:
        try {
            if (command == "run") execute(crr_path);

            else if (command == "help") cout << '\n' << csl::pos_cmd;

            else if (command.substr(0, 3) == "dir") crr_path = command.substr(4);

            else if (command.substr(0, 3) == "scr") crr_path.replace_filename(command.substr(4));

            else if (command == "code") cout << '\n' << read_txt(crr_path).c_str() << '\n';

            else if (command == "clear") system("cls");

            else if (command == "exit") exit(0);

            else if (!command.empty()) cout << FRED("[Unknown command]\n") << "Type `help` for more information\n";
        }
        catch (...) {
            cerr << FRED("[Some error]\n");
        }
    }

    return 0;
}

inline void execute(fs::path path) {
    string ex = "exp_lang.exe " + path.string();
    system(ex.c_str());
}
