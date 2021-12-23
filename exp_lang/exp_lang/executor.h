#pragma once
#include "parser.h"
#include "variables.h"
#include "utilities.h"

#define tok toks[i]

const int var_size = 3;

class Execute
{
public:
	Variables vars;

	Execute(string code="") {
		toks = to_tokens(code);

		var_ind = vars.get_ind_by_name("___");
	}

	void load_tokens(t_vec t) {
		toks = t;
	}

	void EXECUTE() {
		for (size_t i=0; i<toks.size(); i++) 
		{
			string t = tok.type;

			if (t == "use_var") {
				string name = sgm(toks, i+1, i+1+var_size); i += var_size;

				if (!vars.has_var(name)) vars.add(name);
				var_ind = vars.get_ind_by_name(name);
			}
			else if (t == "move_value") {
				i++;
				if (is_number(tok.val[0])) {
					string num = "";
					while (is_number(tok.val[0])) {
						num += tok.val[0];
						i++;
					} i--;
					var_ind->val = stoi(num);
				}
				else {
					string name = sgm(toks, i, i+var_size); i += var_size-1;
					var_ind->val = vars.get_ind_by_name(name)->val;
				}
			}

			else if (t == "plus") var_ind->val++;
			else if (t == "minus") var_ind->val--;
			else if (t == "tobool") var_ind->val = var_ind->val == 0 ? 1 : 0;
			else if (t == "mul") var_ind->val <<= 1;
			else if (t == "div") var_ind->val >>= 1;

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

				while (bracket_n) {
					string t = tok.type;
					if (t == "bgn_contain") bracket_n++;
					else if (t == "fns_contain") bracket_n--;
					i++;
				} i--;
				t_vec cntn_sgm = sgm_of_t(toks, cycle_begin, i);
				cycle_inside(cntn_sgm, var_ind->val);
			}
		}
	}

private:
	t_vec toks;
	var* var_ind;
	int cycle_begin=-1, bracket_n=0;

	void cycle_inside(t_vec code, size_t k) {
		Execute cycle_ex;
		cycle_ex.load_tokens(code);
		cycle_ex.vars = vars;

		k++;
		while (--k)
			cycle_ex.EXECUTE();

		vars = cycle_ex.vars;
	}
};
