#pragma once

#include "include.h"

#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define RST  "\x1B[0m"

#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FYEL(x) KYEL x RST

typedef int tsv; // type stored in the variable


string read_txt(const fs::path& path) {
	string line, res{};
	std::ifstream in(path); 
	while (getline(in, line)) res += line + '\n';
	in.close();
	return res;
}
