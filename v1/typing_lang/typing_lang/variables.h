#pragma once
#include <vector>
#include <string>

using namespace std;

struct var
{
	int val;
	string name;
};

class Variables
{
public:
	vector<var> var_storage = { {-1, "NAN"} };

	void add(string name) {
		// Create new variable
		var_storage.push_back({0, name});
	}

	var* get_ind_by_name(string name) {
		// Transmit the variable indicator by the name
		for (int i=1; i<var_storage.size(); i++) {
			if (var_storage[i].name == name)
				return &var_storage[i];
		}
		return &var_storage[0];
	}

	bool has_var(string name) {
		// Checks if there is a variable
		for (int i=1; i<var_storage.size(); i++) {
			if (var_storage[i].name == name)
				return true;
		}
		return false;
	}
};