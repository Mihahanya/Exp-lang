#pragma once

void read(string& a) {
    cout << "\x1b[32m\n-> \x1b[0m"; // Ggreen arrow
    cin >> a;
}

void back_path(string &path) {
    if (path.back() == '\\') 
        path = sgm(path, 0, path.length()-2);

    path = sgm(path, 0, path.rfind('\\')) + "\\";
}
