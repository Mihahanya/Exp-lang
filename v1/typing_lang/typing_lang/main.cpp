#include <iostream>
#include "conv_code.h"

int main()
{
    string path = "D:/МОИ ПРОЕКТЫ/typing-lang/simple_code.ty"; //cin >> path;

    Code code;
    code.load_file(path);
    code.normalise_code();

    cout << code.code;
}
