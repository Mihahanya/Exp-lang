#pragma once
#include "parser.h"
#include "variables.h"

class Execute
{
public:
	Variables vars;

	Execute(string code="") {
		toks = to_tokens(code);

		var_ind = vars.get_ind_by_name("__");
	}

	void load_tokens(vector<token> t) {
		toks = t;
	}

	void EXECUTE() {
		for (int i=0; i<toks.size(); i++) 
		{
			string t = toks[i].type;

			if (t == "use_var") {
				string name = toks[i+1].val + toks[i+2].val + toks[i+3].val; i += 3;

				if (!vars.has_var(name))
					vars.add(name);

				var_ind = vars.get_ind_by_name(name);
			}
			else if (t == "plus") var_ind->val++;
			else if (t == "minus") var_ind->val--;

			else if (t == "print") cout << var_ind->val;
			else if (t == "print_ch") cout << char(var_ind->val);

			else if (t == "input") cin >> var_ind->val;
			else if (t == "input_ch") {
				char a; cin >> a;
				var_ind->val = int(a);
			}

			else if (t == "bgn_contain") {
				bracket_n++; 
				i++;
				cycle_begin = i;

				while (bracket_n != 0) {
					string t = toks[i].type;

					if (t == "bgn_contain") bracket_n++;
					else if (t == "fns_contain") bracket_n--;
						
					i++;
				}
				i -= 1;
				vector<token> cntn_sgm = sgm_of_t(toks, cycle_begin, i);
				cycle_inside(cntn_sgm, var_ind->val);
			}
		}
	}

private:
	vector<token> toks;
	var* var_ind;
	int cycle_begin=-1, bracket_n=0;

	void cycle_inside(vector<token> code, int k) {
		Execute cycle_ex("");
		cycle_ex.load_tokens(code);
		cycle_ex.vars = vars;

		for (int n=k; n!=0; n--)
			cycle_ex.EXECUTE();

		vars = cycle_ex.vars;
	}
};
