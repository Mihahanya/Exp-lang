#pragma once
#include "parser.h"
#include "variables.h"

class Execute
{
public:
	string code;
	Variables vars;

	Execute(string code) {
		this->code = code;
	}

	void EXECUTE() {
		vector<token> toks = to_tokens(code);
		var* var_ind = vars.get_indicator_by_name("NAN");

		for (int i=0; i<toks.size(); i++) {
			token t = toks[i];

			if (t.type == "new_var") {
				string name = toks[i+1].val + toks[i+2].val; i += 2;

				vars.add(name);
				var_ind = vars.get_indicator_by_name(name);
			}
			else if (t.type == "plus") {
				var_ind->val++;
			}
			else if (t.type == "minus") {
				var_ind->val--;
			}
		}
		cout << vars.var_storage[2].val << " " << vars.var_storage[2].name;
	}
};
