#pragma once
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

string init_msg = 
"     _________________________\n"
"     _____ __    __    _____\n"
"   /  __  /\\  \\/  /  /  __  /\n"
"  /  ___ /  )    /  /  /_/ /\n"
" /  /___   /    (  /  ___ /\n"
"/_____ /  /__/\\__\\/_ /\n"
"________________________\n"
"\n"
"(c) Exp 1.0, 2021-2022\n"
"https://github.com/Mihahanya \n"
"\n"
"------------------------\n"
"",

pos_cmd = "Possible commands for the terminal:\n"
"!run (run the script)\n"
"!code (show the code)\n"
"!move- (move to the next directory, for example `!move-some_folder\\` or `!move-my_script.exp`)\n"
"!path- (change the current directory to another one, for example `!path-D:\\project_loader\\script.exp`)\n"
"!path (show a path to the current directory)\n"
"!back (move to the back directory)\n"
"!clear (clear console)\n"
"!exit (close interpreter)";


inline bool is_number(char a) {
	return isdigit(a) || a == '-';
}

string read_txt(fs::path path) {
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
u8string sgm(u8string a, size_t f, size_t t=-1) {
	if (t == -1) t = a.length();
	u8string o=u8"";
	for (size_t i=f; i<t; i++)
		o += a[i];
	return o;
}

inline void read(string& a) {
    cout << "\x1b[32m\n-> \x1b[0m"; // Ggreen arrow
    cin >> a;
}

void back_path(fs::path &path) {
	u8string b = path.u8string();
    if (b.back() == '\\') 
        path = sgm(b, 0, b.length()-2);

    path = sgm(b, 0, b.rfind('\\')) + u8"\\";
}
