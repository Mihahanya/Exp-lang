#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <Windows.h>

#include "parser.h"

using std::cout;
using std::cin;
using std::wcout;
using std::wcin;
using std::string;
using std::wstring;
using std::vector;

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
"https://github.com/Mihahanya\n"
"\n"
"------------------------\n";

string pos_cmd = 
"`help`	show the description of the commands for the console\n"
"`run`	execute the script\n"
"`dir`	change the current directory\n"
"`scr`	change the current script\n"
"`code` show the code\n"
"`clr`	clear the console\n"
"`exit` close the console\n";

string read_arrow = "\x1b[32m\n~> \x1b[0m";


wstring read_txt(fs::path path) {
	wstring line, res{};
	std::wifstream in(path); 
	while (getline(in, line)) res += line + L'\n';
	in.close();
	return res;
}


inline bool is_number(char a) {
	return isdigit(a) || a == '-';
}


wstring sgm(wstring a, size_t f, size_t t=-1) {
	if (t == -1) t = a.length();
	wstring o{};
	for (size_t i=f; i<t; i++)
		o += a[i];
	return o;
}
