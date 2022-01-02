#pragma once
#include <vector>
#include <string>

using namespace std;

struct var {
	uint64_t id;
	int val;
	string name;
};

class Variables
{
public:
	vector<var> v_strg = { {0, 0, ""}, {25184, 0, "df"}, {ULLONG_MAX, 0, ""}};

	void choose(string name) {
		if (name == this->name) return;
		this->name = name;
		cid = to_id(name);
		pos = pos_id(cid);
	}
	
	var* get_ind_by_name(bool check=true) {
		if (check && !has_var()) {
			add();
			pos++;
		}
		return &v_strg[pos];
	}

private:
	uint64_t cid;
	size_t pos;
	string name;

	inline bool has_var() { 
		return v_strg[pos].name == name; 
	}

	inline void add() {
		v_strg.insert(v_strg.begin()+1+pos, {cid, 0, name});
	}

	size_t pos_id(uint64_t id) { /// Binary search
		size_t l=0, r=v_strg.size(), mid;
		while (l < r) {
			mid = (l+r)>>1;
			if (v_strg[mid].id > id) r = mid;
			else l = mid+1;
		} 
		return r-1;
	}

	uint64_t to_id(string name) {
		uint64_t id = 0;
		for (int i=0; i<name.length(); i++) {
			//id += name[i] * pow(63, i); /// 26 litters * 2 (A and a) + 10 digits + `_` = 63 symbols
			id ^= name[i];
			id <<= 4;
		}
		return id;
	}
};
