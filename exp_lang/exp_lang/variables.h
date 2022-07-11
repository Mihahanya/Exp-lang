#pragma once

#include "utilities.h"

struct var {
	size_t id;
	int val;
	wstring name;
};

class Variables
{
public:
	vector<var> v_strg = { {0, 0, L""}, {25184, 0, L"df"}, {SIZE_MAX, 0, L""}};

	void choose(wstring name) {
		if (name == this->name) return;
		this->name = name;
		cid = std::hash<wstring>{}(name);;
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
	size_t cid, pos;
	wstring name;

	inline bool has_var() { 
		return v_strg[pos].name == name; 
	}

	inline void add() {
		v_strg.insert(v_strg.begin()+1+pos, {cid, 0, name});
	}

	size_t pos_id(size_t id) { 
		/// The binary search a variavle by id
		size_t l=0, r=v_strg.size(), mid;
		while (l < r) {
			mid = (l+r)>>1;
			if (v_strg[mid].id > id) r = mid;
			else l = mid+1;
		} 
		return r-1;
	}
};
