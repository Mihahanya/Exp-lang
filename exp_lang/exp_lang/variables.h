#pragma once
#include <vector>
#include <string>

using namespace std;

struct var
{
	int val;
	string name;

	string print() {
		return "[VARIABLE] Val: `"+ to_string(val) + "`, Name: `" + name + "`";
	}
};

class Variables
{
public:
	vector<var> var_storage = { {0, "___"} };

	void add(string name) {
		var_storage.push_back({0, name});
	}

	var* get_ind_by_name(string name) {
		for (int i=1; i<var_storage.size(); i++) {
			if (var_storage[i].name == name)
				return &var_storage[i];
		}
		return &var_storage[0];
	}

	bool has_var(string name) {
		for (int i=1; i<var_storage.size(); i++) {
			if (var_storage[i].name == name)
				return true;
		}
		return false;
	}
};

void print_vec_v(vector<var> a) {
	for (var t : a) 
		cout << t.print() << '\n';
}
