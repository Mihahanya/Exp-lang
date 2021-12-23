#pragma once
#include <vector>
#include <string>

using namespace std;

struct var {
	int val;
	string name;
};

class Variables
{
public:
	vector<var> var_storage = { {0, "___"} };

	void add(string name) {
		var_storage.push_back({0, name});
	}

	var* get_ind_by_name(string name) {
		for (int i=var_storage.size()-1; i>=0; i--) { // Searching by end
			if (var_storage[i].name == name)
				return &var_storage[i];
		}
		return &var_storage[0];
	}

	bool has_var(string name) {
		for (int i=var_storage.size()-1; i>=0; i--) {
			if (var_storage[i].name == name)
				return true;
		}
		return false;
	}
};

