#pragma once
#include <fstream>

string read_txt(string path) {
	ifstream in(path); 
	string line, o="";
	while (getline(in, line)) 
		o += line + '\n';
	in.close();
	return o;
}

string sgm(string a, int f, int t=-1) {
	if (t == -1) t = a.length();
	string o="";
	for (int i=f; i<t; i++)
		o += a[i];
	return o;
}

string sgm(vector<token> a, int f, int t=-1) {
	if (t == -1) t = a.size();
	string o="";
	for (int i=f; i<t; i++)
		o += a[i].val;
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
