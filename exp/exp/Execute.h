#pragma once

#include "TokenTypes.h"
#include <map>
#include <iostream>
#include <vector>
#include <string>


void exec(const std::vector<Token>& tokens, bool check_vars=true) {
	// TODO: put errors throwing other place
	std::map<string, int> vars{};
	vars["_"] = -1;
	int* current_var = &vars["_"];

	for (int i=0; i < tokens.size(); ++i) 
	{
		switch (tokens[i].type) {
		using enum TokenType;

		case ChooseVariable:
			if (check_vars and vars.find(tokens[i].val) == vars.end()) 
				throw std::runtime_error("Accessing to undeclared variable `" + tokens[i].val + "` at line " + std::to_string(tokens[i].line) + "\n"); 
			
			current_var = &vars[tokens[i].val];
			break;

		case NewVariable:
			i += 2;
			vars[tokens[i].val] = 0; 
			current_var = &vars[tokens[i].val];
			break;

		case Increase:		(*current_var)++; break;
		case Decrease:		(*current_var)--; break;
		case Input:			std::cin >> *current_var; break;
		case Output:		std::cout << *current_var; break;
		}
	}
}
