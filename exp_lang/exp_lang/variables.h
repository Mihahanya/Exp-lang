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
	vector<var> v_strg = { {0, "NULL"} };

	void add(string name) {
		v_strg.push_back({0, name});
	}

	var* get_ind_by_name(string name) {
		for (int i=v_strg.size()-1; i>=0; i--) { // Searching by end
			if (v_strg[i].name == name)
				return &v_strg[i];
		}
		return &v_strg[0];
	}

	bool has_var(string name) {
		for (int i=v_strg.size()-1; i>=0; i--) {
			if (v_strg[i].name == name)
				return true;
		}
		return false;
	}
};

