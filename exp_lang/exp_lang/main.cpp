#include <time.h>
#include <windows.h>

#include "Parser.h"


int main(int argc, char **argv) 
{
    if (argc == 1) exit(0);
    else system("");

    {
        const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

        COORD coord = { 64, 32 };
        SMALL_RECT rect = { 0, 0, coord.X-1, coord.Y-1 };
        //SetConsoleScreenBufferSize(handle, coord);
        SetConsoleWindowInfo(handle, TRUE, &rect);

        LPCWSTR title{ L"Exp interpreter" };
        SetConsoleTitle(title);
    }

    try {
        const fs::path path {argv[1]};
        //const fs::path path = "D:\\PROJECTS\\Exp-lang\\examples\\fibonacci.exp";

        printf(FYEL("Compilation start\n"));
        printf("Script: %s\n", path.filename().string().c_str());

        clock_t start_comp = clock();

        const string code = read_txt(path);
        Parser ex(code);

        if (ex.ok) system("cls");

        clock_t start_ex = clock();

        ex.run();

        if (ex.vars["_DEBUG_MODE"]) {
            printf(FGRN("\nCompilation status: %.3fs\nExecution time: %.3fs\n"),
                float(start_ex-start_comp)/1000, float(clock()-start_ex)/1000);
        }
        if (ex.vars["_NOT_CLOSE_MODE"]) {
            cin.ignore(1);
            cin.get();
        }
    }
    catch (...) {
        cerr << FRED("[Some error]\n");
    }

    return 0;
}
