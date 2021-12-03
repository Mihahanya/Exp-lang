#include <iostream>
#include "conv_code.h"
#include "token_manager.h"

int main()
{
    string path = "D:/МОИ ПРОЕКТЫ/typing-lang/test.ty"; //cin >> path;

    Code code;
    code.load_file(path);
    code.normalise_code();

    //cout << code.code;

    print_vec_t(to_tokens(code.code));
}
