#pragma once
#include "parser.h"
#include "variables.h"

class Execute
{
public:
	Variables vars;

	Execute(string code="", vector<token> vt={}) {
		if (code != "") toks = to_tokens(code);
		else toks = vt;

		var_ind = vars.get_ind_by_name("__");
	}

	void EXECUTE() {
		for (int i=0; i<toks.size(); i++) 
		{
			token t = toks[i];

			if (t.type == "use_var") {
				string name = toks[i+1].val + toks[i+2].val; i += 2;

				if (!vars.has_var(name))
					vars.add(name);

				var_ind = vars.get_ind_by_name(name);
			}
			else if (t.type == "plus") var_ind->val++;
			else if (t.type == "minus") var_ind->val--;

			else if (t.type == "print") cout << var_ind->val;
			else if (t.type == "print_ch") cout << char(var_ind->val);

			else if (t.type == "input") cin >> var_ind->val;
			else if (t.type == "input_ch") {
				char a; cin >> a;
				var_ind->val = int(a);
			}

			else if (t.type == "bgn_contain") {
				bracket_n++; 
				i++;
				cycle_begin = i;

				while (bracket_n != 0) {
					token t = toks[i];

					if (t.type == "bgn_contain") 
						bracket_n++;

					else if (t.type == "fns_contain") {
						bracket_n--;

						if (bracket_n == 0) {
							vector<token> cntn_sgm = sgm_of_t(toks, cycle_begin, i); i++;
							//print_vec_t(cntn_sgm); cout << endl;
							
							int n = 1;
							if (toks[i].type == "do_in") {
								string name = toks[i+1].val + toks[i+2].val; i += 1;
								n = vars.get_ind_by_name(name)->val;
							}

							cycle_inside(cntn_sgm, n);
						}
					}
					i++;
				}
			}
		}
	}

private:
	vector<token> toks;
	var* var_ind;
	int cycle_begin=-1, bracket_n=0;

	void cycle_inside(vector<token> code, int k) {
		Execute cycle_ex("", code);
		cycle_ex.vars = vars;
		//print_vec_v(cycle_ex.vars.var_storage); cout << endl;

		for (int n=k; n!=0; n--)
			cycle_ex.EXECUTE();

		vars = cycle_ex.vars;
	}
};
