#pragma once

#include "parser.h"
#include "variables.h"
#include "utilities.h"

class Execute
{
public:
	Variables vars;
	bool ext=false;

	Execute(wstring code=L"") {
		if (code != L"") {
			Tkn tc(code);
			toks = tc.to_tokens();
		}
		vars.choose(L"df");
		var_ind = vars.get_ind_by_name();
	}

	void load_tokens(t_vec t) {
		toks = t;
	}

	void EXECUTE() {
		for (i=0; i<toks.size(); i++) 
		{
			t = toks[i].type;

			if (is_var_tkn(toks[i])) var_ind = choose_var();
			
			else if (t == MOVE_VAL) {
				i++; /// Move a variable value or a number else it's an error and move the zero
				var_ind->val = is_var_tkn(toks[i]) ? choose_var()->val : (is_number(toks[i].val[0]) ? find_num() : 0);
			}

			else if (t == PLUS)   var_ind->val++;
			else if (t == MINUS)  var_ind->val--;
			else if (t == TOBOOL) var_ind->val = var_ind->val == 0 ? 1 : 0;
			else if (t == MORE)   var_ind->val =  var_ind->val > 0 ? 1 : 0;
			else if (t == LESS)   var_ind->val =  var_ind->val < 0 ? 1 : 0;

			else if (t == PRINT)	  cout << var_ind->val;
			else if (t == PRINT_CHAR) cout << char(var_ind->val);

			else if (t == INPUT_N)		cin >> var_ind->val;
			else if (t == INPUT_CHAR) { cin >> inp; var_ind->val = int(inp); }
			
			else if (t == BGN_CYC) cycle_inside(find_cycle(), var_ind->val);
		
			else if (t == BREAK && var_ind->val == 1) { ext = true; break; }
		}
	}

private:
	t_vec toks;
	var* var_ind;
	
	size_t i;
	TokensType t;
	char inp; 

	inline var* choose_var() {
		vars.choose(find_name());
		return vars.get_ind_by_name(t==CHECH_VAR);
	}

	inline void cycle_inside(t_vec code, int k) {
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

	inline int find_num() {
		wstring num{};
		for (i; is_number(toks[i].val[0]); i++)
			num += toks[i].val;
		i--;
		return stoi(num);
	}

	inline wstring find_name() {
		wstring name{};
		for (++i; toks[i].type == OTHER; i++)
			name += toks[i].val;
		i--;
		return name;
	}

	inline t_vec find_cycle() {
		int bracket_n=1; t_vec o;
		i++;
		while (bracket_n) {
			t = toks[i].type;
			if (t == BGN_CYC) bracket_n++;
			else if (t == FNS_CYC) bracket_n--;
			o.push_back(toks[i++]);
		} i--;
		return o;
	}

	t_vec sgm_of_t(size_t from, size_t to) {
		t_vec o;
		for (size_t i=from; i<to; i++)
			o.push_back(toks[i]);
		return o;
	}
};
