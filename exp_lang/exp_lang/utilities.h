#pragma once
#include <fstream>

bool is_number(char a) {
	return isdigit(a) || a == '-';
}

string read_txt(string path) {
	ifstream in(path); 
	string line, o="";
	while (getline(in, line)) 
		o += line + '\n';
	in.close();
	return o;
}

string sgm(string a, size_t f, size_t t=-1) {
	if (t == -1) t = a.length();
	string o="";
	for (size_t i=f; i<t; i++)
		o += a[i];
	return o;
}

void read(string& a) {
    cout << "\x1b[32m\n-> \x1b[0m"; // Ggreen arrow
    cin >> a;
}

void back_path(string &path) {
    if (path.back() == '\\') 
        path = sgm(path, 0, path.length()-2);

    path = sgm(path, 0, path.rfind('\\')) + "\\";
}
