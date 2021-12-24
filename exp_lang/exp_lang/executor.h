#pragma once
#include <tuple>

#include "parser.h"
#include "variables.h"
#include "utilities.h"

#define tok toks[i]

class Execute
{
public:
	Variables vars;
	bool ext=false;

	Execute(string code="") {
		Tkn* tc = new Tkn(code);
		toks = (*tc).to_tokens();
		delete tc;

		var_ind = vars.get_ind_by_name("___");
	}

	void load_tokens(t_vec t) {
		toks = t;
	}

	void EXECUTE() {
		for (size_t i=0; i<toks.size(); i++) 
		{
			t = tok.type;

			if (t == USE_VAR) {
				tie(name, i) = find_name(i+1);

				if (!vars.has_var(name)) vars.add(name);
				var_ind = vars.get_ind_by_name(name);
			}
			else if (t == "move_value") {
				i++;
				if (is_number(tok.val[0])) 
					tie(var_ind->val, i) = find_num(i);
				else {
					tie(name, i) = find_name(i);
					var_ind->val = vars.get_ind_by_name(name)->val;
				}
			}

			else if (t == PLUS)   var_ind->val++;
			else if (t == MINUS)  var_ind->val--;
			else if (t == TOBOOL) var_ind->val = var_ind->val == 0 ? 1 : 0;
			else if (t == MUL)    var_ind->val <<= 1;
			else if (t == DIV)    var_ind->val >>= 1;

			else if (t == PRINT)	  cout << var_ind->val;
			else if (t == PRINT_CHAR) cout << char(var_ind->val);

			else if (t == INPUT_N)		cin >> var_ind->val;
			else if (t == INPUT_CHAR) { cin >> inp; var_ind->val = int(inp); }
			
			else if (t == BREAK && var_ind->val == 1) { ext = true; break; }

			else if (t == BGN_CYC) {
				tie(cycle_ctn, i) = find_cycle(i+1);
				cycle_inside(cycle_ctn, var_ind->val);
			}
		}
	}

private:
	t_vec toks;
	var* var_ind;
	
	char inp; 
	string name, t;
	int c_bgn, c_end;
	t_vec cycle_ctn;

	void cycle_inside(t_vec code, int k) {
		Execute cycle_ex;
		cycle_ex.load_tokens(code);
		cycle_ex.vars = vars;
		
		k++;
		while (--k) {
			cycle_ex.EXECUTE();
			if (cycle_ex.ext) break;
		}
		vars = cycle_ex.vars;		
	}

	tuple<int, int> find_num(int start) {
		int i = start; string num = "";
		while (is_number(tok.val[0])) {
			num += tok.val;
			i++;
		}
		return { stoi(num), i-1 };
	}

	tuple<string, int> find_name(int start) {
		int i = start; string name = "";
		while (tok.type == OTHER) {
			name += tok.val;
			i++;
		}
		return { name, i-1 };
	}

	tuple<t_vec, int> find_cycle(int start) {
		int i=start, bracket_n=1; t_vec o;
		while (bracket_n) {
			string t = tok.type;
			if		(t == BGN_CYC) bracket_n++;
			else if (t == FNS_CYC) bracket_n--;
			o.push_back(toks[i]);
			i++;
		}
		return { o, i-1 };
	}

	t_vec sgm_of_t(size_t from, size_t to) {
		t_vec o;
		for (size_t i=from; i<to; i++)
			o.push_back(tok);
		return o;
	}
};
