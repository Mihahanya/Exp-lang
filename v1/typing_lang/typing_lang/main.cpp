#include <iostream>
#include "executor.h"
#include "conv_code.h"

int main()
{
    //string path = "D:/МОИ ПРОЕКТЫ/typing-lang/test.ty"; 
    string path = "D:/ПРОЕКТЫ/typing lang/Typing-the-PL/test.ty"; 
    //cin >> path;

    Code code;
    code.load_file(path);
    code.normalise_code();

    print_vec_t(to_tokens(code.code));

    Execute ex(code.code);
    ex.EXECUTE();
}
