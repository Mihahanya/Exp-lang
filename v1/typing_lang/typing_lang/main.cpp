#include <iostream>
#include "executor.h"
#include "conv_code.h"

int main()
{
    system("mode con cols=40 lines=20");

    string path = "D:/МОИ ПРОЕКТЫ/typing-lang/test.ty"; 
    //string path = "D:/ПРОЕКТЫ/typing lang/Typing-the-PL/test.ty"; 
    //cin >> path;

    system(("title " + path).c_str());

    Code code;
    code.load_file(path);
    code.normalise_code();

    Execute ex(code.code);
    ex.EXECUTE();

    cout << ex.OUTPUT;
}
