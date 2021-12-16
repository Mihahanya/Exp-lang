#pragma once
#include "parser.h"
#include "variables.h"
#include "utilities.h"

const int var_size = 3;

class Execute
{
public:
	Variables vars;

	Execute(string code="") {
		toks = to_tokens(code);

		var_ind = vars.get_ind_by_name("___");
	}

	void load_tokens(vector<token> t) {
		toks = t;
	}

	void EXECUTE() {
		for (int i=0; i<toks.size(); i++) 
		{
			string t = toks[i].type;

			if (t == "use_var") {
				string name = sgm(toks, i+1, i+1+var_size); i += var_size;

				if (!vars.has_var(name)) vars.add(name);
				var_ind = vars.get_ind_by_name(name);
			}
			else if (t == "move_value") {
				i++;
				if (isdigit(toks[i].val[0])) {
					string num = "";
					while (isdigit(toks[i].val[0]) || toks[i].val[0] == '-') {
						num += toks[i].val[0];
						i++;
					} 
					i--;
					var_ind->val = stoi(num);
				}
				else {
					string name = sgm(toks, i, i+var_size); i += var_size-1;
					var_ind->val = vars.get_ind_by_name(name)->val;
				}
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
				//cout << "a ";
				bracket_n++; 
				i++;
				cycle_begin = i;

				while (bracket_n != 0) {
					string t = toks[i].type;

					if (t == "bgn_contain") bracket_n++;
					else if (t == "fns_contain") bracket_n--;
						
					i++;
				}
				i--;
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
