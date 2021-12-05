#pragma once
#include "parser.h"
#include "variables.h"

class Execute
{
public:
	string code, OUTPUT;
	Variables vars;

	Execute(string code) {
		this->code = code;
	}

	void EXECUTE() {
		vector<token> toks = to_tokens(code);
		var* var_ind = vars.get_ind_by_name("NAN");
		int cycle_begin=-1, bracket_n=0;
		bool is_cycle = false;

		for (int i=0; i<toks.size(); i++) {
			token t = toks[i];

			if (!is_cycle) {
				if (t.type == "use_var") {
					string name = toks[i+1].val + toks[i+2].val; i += 2;

					if (!vars.has_var(name))
						vars.add(name);

					var_ind = vars.get_ind_by_name(name);
				}
				else if (t.type == "plus") var_ind->val++;
				else if (t.type == "minus") var_ind->val--;

				else if (t.type == "print") OUTPUT += to_string(var_ind->val);
				else if (t.type == "print_char") OUTPUT += char(var_ind->val);

				else if (t.type == "input") cin >> var_ind->val;
				else if (t.type == "input_char") {
					char a; cin >> a;
					var_ind->val = int(a);
				}

				else if (t.type == "begin_contain") {
					cycle_begin = i+1;
					bracket_n++;
					is_cycle = true;
				}
			}
			else {
				if (t.type == "finish_contain") {
					bracket_n--;

					if (bracket_n == 0) {
						if (toks[i+1].type == "do_in") {
							string cntn_sgm = sgm_t_to_str(toks, cycle_begin, i); i++;
							string name = toks[i+1].val + toks[i+2].val; i += 2;

							Execute cycle_ex(cntn_sgm);
							cycle_ex.vars = vars;

							for (int i=0; i<vars.get_ind_by_name(name)->val; i++)
								cycle_ex.EXECUTE();

							vars = cycle_ex.vars;
							cout << vars.var_storage[2].name;
							OUTPUT += cycle_ex.OUTPUT;
						}
					}
				}
			}
		}
	}
};
