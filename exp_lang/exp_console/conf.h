#pragma once

#include <cstdlib>
#include <windows.h>
#include <string>
#include <time.h>
#include <iostream>
#include <fstream>
#include <filesystem>

using std::cerr;
using std::cin;
using std::cout;
using std::string;

namespace fs = std::filesystem;


#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define RST  "\x1B[0m"

#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST


namespace csl 
{
	const string init_msg =
	"     _________________________\n"
	"     _____ __    __    _____\n"
	"   /  __  /\\  \\/  /  /  __  /\n"
	"  /  ___ /  )    /  /  /_/ /\n"
	" /  /___   /    (  /  ___ /\n"
	"/_____ /  /__/\\__\\/_ /\n"
	"________________________\n"
	"\n"
	"(c) Exp 3.0, 2021-2022\n"
	"https://github.com/Mihahanya\n"
	"\n"
	"------------------------\n";

	const string pos_cmd =
	"`help`	show the description of the commands for the console\n"
	"`run`	execute the script\n"
	"`dir`	change the current directory\n"
	"`scr`	change the current script\n"
	"`code` show the code\n"
	"`clear`clear the console\n"
	"`exit` close the console\n";

	const string read_arrow = FGRN("\n~> ");
}

string read_txt(const fs::path& path) {
	string line, res{};
	std::ifstream in(path);
	while (getline(in, line)) res += line + '\n';
	in.close();
	return res;
}
