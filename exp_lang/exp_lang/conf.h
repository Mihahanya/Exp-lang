#pragma once

#include "include.h"

#define CLR_CONSOLE false

const string init_msg = 
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

const string pos_cmd = 
"`help`	show the description of the commands for the console\n"
"`run`	execute the script\n"
"`dir`	change the current directory\n"
"`scr`	change the current script\n"
"`code` show the code\n"
"`clr`	clear the console\n"
"`exit` close the console\n";

const string read_arrow = "\x1b[32m\n~> \x1b[0m";


wstring read_txt(const fs::path& path) {
	wstring line, res{};
	std::wifstream in(path); 
	while (getline(in, line)) res += line + L'\n';
	in.close();
	return res;
}
