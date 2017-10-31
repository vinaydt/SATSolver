#include "DPLL.h"



DPLL::DPLL(CNF* _cnf)
{
	cnf = _cnf;
	CONFLICT = false;
	//performSearch(cnf);
}


DPLL::~DPLL()
{
}

int DPLL::performSearch()
{
	CNF* cnf = getCNF();
	pureLiteralElimination(cnf);
	//processSingletonClauses(cnf); // To be removed!
	addActiveClauses(cnf); //To be completed!
	while (true) {
		performUnitPropagate(cnf);
		if (!CONFLICT) {
			if (allAssigned(sigma) == true) {
				return SAT;
			}
			else {
				Variable *var = makeDecision(cnf);
				if (var == NULL)
					return UNSAT;
				var->set_assigned(true);
				var->set_value(true);
				fix_clauses(var, cnf->get_clauses(), true, false);
				delta = addToStack(delta, var); //Ading to decision stack
				sigma = addToMap(sigma, var, true); //Adding to sigma map
				tau = addToStack(tau, var, cnf);
			}		
		}
		else if (delta.empty()==true) {
			return UNSAT;
		}
		else {
			set_conflict(false);
			Variable* var = removeFromStack(delta);
			if (var == NULL)
				return UNSAT;
			delta.pop();
			Backtrack(var);
			//Check
			var->set_assigned(true);
			var->set_value(false);
			//
			sigma = addToMap(sigma, var, false);
			tau = addToStack(tau,var, cnf);
		}

	}
	return UNSAT;
}

void DPLL::pureLiteralElimination(CNF * cnf)
{
	Variable *var;
	fix_pures(cnf->get_var(), cnf->get_clauses());
	var = FIND_PURE_SYMBOL(cnf->get_var());
	if (var != NULL) {
		fix_clauses(var, cnf->get_clauses(), true, true);
		fix_variables(var, cnf->get_var(), true);
		var->set_value(var->get_sign());
	}
}

void DPLL::performUnitPropagate(CNF * cnf)
{
	while (true) {
		Clause * c = pickAClause();
		if (c == NULL ) {
			//set_sigma(sigma);
			return;
		}
		processConflict(c);
		if (CONFLICT == true) {
			vector<Clause*> empty;
			swap(tau, empty);
			return;
		}
		processUnitClauseProcessing(cnf);
	}

}
Clause * DPLL::pickAClause() 
{
	
		Clause *clause = removeFromStack(tau);
		if (clause == NULL)
			return NULL;
		tau.pop_back();
		return clause;
	
}

void DPLL::processConflict(Clause * c)
{
	if (c == NULL)
		return;
	set<Variable*>::iterator it_v, it_v2;
	set<Clause*>::iterator it_c;
	set<Clause*> *clauses = getCNF()->get_clauses();
	/*
	set<Variable*>ptr = c->get_var_set();
	set<Variable*>::iterator iter;
	for (iter = ptr.begin(); iter != ptr.end(); ++iter) {
		;
	}
	*/
	for (it_v = c->get_var()->begin(); it_v != c->get_var()->end(); it_v++) {
	
		for (it_c = clauses->begin(); it_c != clauses->end(); it_c++) {
			if ((*it_c) == c)
				continue;
			for (it_v2 = (*it_c)->get_var()->begin(); it_v2 != (*it_c)->get_var()->end(); it_v2++) {
				if ((*it_v2)->get_name() == (*it_v)->get_name()) {
					//Same variable in diff clause
					if (((*it_v2)->get_assigned() == (*it_v)->get_assigned())) {  //&& (*it_v)->get_assigned()==true) {
						//Same variable - assigned
						if ((*it_v2)->get_sign() == (*it_v)->get_sign()) {
							// Same variable - assigned - same sign
							if ((*it_v2)->get_value() != (*it_v)->get_value()) {
								// Same variable - assigned - same sign - diff value - CONFLICT!
								set_conflict(true);
								return;
							}
						}
						else {
							// Same variable - assigned - diff sign 
							if ((*it_v2)->get_value() == (*it_v)->get_value()) {
								// Same variable - assigned - diff sign - same value - CONFLICT!
								set_conflict(true);
								return;
							}
						}
					}
				}
			}
		}
		
	}
}

bool DPLL::allAssigned(map<Variable*, int> sigma)
{
	if (sigma.empty())
		return false;
	//Check if all memebers in sigma are assigned!
	map<Variable*, int>::iterator it_sigma;
	for (it_sigma = sigma.begin(); it_sigma != sigma.end(); it_sigma++) {
		if (it_sigma->first->get_assigned() == false)
			return false;
	}
	//Additionally!
	set<Clause*> *clauses = getCNF()->get_clauses();
	set<Clause*>::iterator it_c;
	set<Variable*> *variables = getCNF()->get_var();
	set<Variable*>::iterator it_v;
	for (it_c = clauses->begin(); it_c != clauses->end(); it_c++) {
		if ((*it_c)->get_assigned() == false)
			return false;
	}
	for (it_v = variables->begin(); it_v != variables->end(); it_v++) {
		if ((*it_v)->get_assigned() == false)
			return false;
	}
	return true;
}

map<Variable*, int> DPLL::addToMap(map<Variable*, int> map, Variable *var, bool flag)
{
	if (flag == true) {
		if (map.insert(make_pair(var, var->get_value())).second == false) //Check Logic!
		{
			throw "Insertion failed. Key was present";
		}

	}
	if (flag == false) {
		if (map.insert(make_pair(var, !var->get_value())).second == false) //Check Logic!
		{
			throw "Insertion failed. Key was present";
		}
	}
	return map;
}

vector<Clause*> DPLL::addToStack(vector<Clause*> tau, Variable* variable, CNF *cnf)
{
	if (cnf == NULL)
		throw "CNF is empty! Verify";
	if (variable == NULL)
		throw "Variable is Empty! Verify";
	set<Clause*> *clauses = cnf->get_clauses();
	set<Clause*>::iterator it_c;
	set<Variable*>::iterator it_v;
	for (it_c = clauses->begin(); it_c != clauses->end(); it_c++) {
		for (it_v = (*it_c)->get_var()->begin(); it_v != (*it_c)->get_var()->end(); it_v++) {
			if ((*it_v)->get_name() == variable->get_name()) {
				//Check if Tau already has the element
				if (std::find(tau.begin(), tau.end(), *it_c) != tau.end()) {
					continue;
				}
				else
					tau.push_back((*it_c));
			}
		}
	}
	return tau;
}

stack<Variable*> DPLL::addToStack(stack<Variable*> delta, Variable * var)
{
	delta.push(var);
	return delta;
}

Variable * DPLL::removeFromStack(stack<Variable*> delta)
{
	if (delta.empty())
		return NULL;
	return delta.top();
}

Clause * DPLL::removeFromStack(vector<Clause*> tau) {
	if (tau.empty())
		return NULL;
	return tau.back();
}

map<Variable*, int> DPLL::removeFromMap(map<Variable*, int> sigma, Variable *var)
{
	map<Variable*, int>::iterator iter_sigma = sigma.find(var);
	if (iter_sigma != sigma.end())
		sigma.erase(iter_sigma);
	else
		throw("Something wrong! Sigma doesn't hold the variable. Please check!");
	return sigma;
}


void DPLL::Backtrack(Variable* var)
{
	sigma = removeFromMap(sigma, var);
	restore_variables(var, getCNF()->get_var());
	restore_clauses(var, getCNF()->get_clauses());
	restore_symbol(var);
}


void DPLL::fix_clauses(Variable * var, set<Clause*>* clauses, bool lvalue, bool flag, bool unit)
{
	set<Clause*>::iterator it_c;
	set<Variable*>::iterator it_v;
	for (it_c = clauses->begin(); it_c != clauses->end(); it_c++) {
		for (it_v = (*it_c)->get_var()->begin(); it_v != (*it_c)->get_var()->end(); it_v++) {
			if ((*it_v)->get_name() == var->get_name()) {
				(*it_v)->set_assigned(true);
		
				if (flag == true) {
					(*it_v)->set_value(true);
					(*it_v)->set_real_value((*it_v)->get_sign());
				}
				else {
					if ((*it_v)->get_sign() == true) 
						(*it_v)->set_value(lvalue);
					else				
						(*it_v)->set_value(!lvalue);
				}
			}
		}
		if ((*it_c)->get_assigned() == true)
			continue;				
		if (ALL_VARIABLES_ARE_FALSE(*it_c) == true) {	
			(*it_c)->set_assigned(true);
			(*it_c)->set_value(false);
			continue;
		}
		if (ONE_VARIABLE_IS_TRUE(*it_c) == true) {	
			(*it_c)->set_assigned(true);					
			(*it_c)->set_value(true);					
			continue;
		}
	}
}

void DPLL::fix_variables(Variable * v, set<Variable*>* var, bool u)
{
	set<Variable*>::iterator it_v;
	for (it_v = var->begin(); it_v != var->end(); it_v++) {
		if ((*it_v)->get_name() == v->get_name()) {	
			if (u == true) {
				(*it_v)->set_value(v->get_sign());			
				(*it_v)->set_sign(v->get_sign());
			}
			(*it_v)->set_assigned(true);
			return;
		}
	}
	return;
}

bool DPLL::ALL_VARIABLES_ARE_FALSE(Clause * cl)
{
	set<Variable*>::iterator it_v;
	for (it_v = cl->get_var()->begin(); it_v != cl->get_var()->end(); it_v++) {
		if ((*it_v)->get_value() == true || (*it_v)->get_assigned() == false) 
			return false;
	}
	return true;
}

bool DPLL::ONE_VARIABLE_IS_TRUE(Clause * cl)
{
	set<Variable*>::iterator it_v;
	for (it_v = cl->get_var()->begin(); it_v != cl->get_var()->end(); it_v++) {
		if ((*it_v)->get_assigned() == true && (*it_v)->get_value() == true) return true;
	}
	return false;
}

Variable * DPLL::FIND_PURE_SYMBOL(set<Variable*>* var)
{
	set<Variable*>::iterator it_v;
	for (it_v = var->begin(); it_v != var->end(); it_v++) {
		if ((*it_v)->get_pure() == true && (*it_v)->get_assigned() == false) {
			(*it_v)->set_assigned(true);
			(*it_v)->set_value(true);
			(*it_v)->set_real_value((*it_v)->get_sign());
			return *it_v;
		}
	}
	return NULL;
}

void DPLL::processUnitClauseProcessing(CNF * cnf)
{
	Variable *var = FIND_UNIT_CLAUSE(cnf->get_clauses(), cnf->get_var());
	if (var != NULL) {
		fix_clauses(var, cnf->get_clauses(), var->get_sign(), false);
		fix_variables(var, cnf->get_var(), true);

		sigma = addToMap(sigma, var, true);
		tau = addToStack(tau, var, cnf);
	}
}

Variable * DPLL::FIND_UNIT_CLAUSE(set<Clause*>* clauses, set<Variable*>* vars)
{
	set<Clause*>::iterator it_c;
	set<Variable*>::iterator it_v;
	Variable *var = NULL;
	int size, v_assigned;
	for (it_c = clauses->begin(); it_c != clauses->end(); it_c++) {
		if ((*it_c)->get_assigned() == true)
			continue;
		size = (*it_c)->get_var()->size();	
		v_assigned = 0;
		for (it_v = (*it_c)->get_var()->begin(); it_v != (*it_c)->get_var()->end(); it_v++) {
			if ((*it_v)->get_assigned() == true) {
				v_assigned++;
			}
			else {
				var = *it_v;
			}
		}
		if (v_assigned == size - 1) {
			(*it_c)->set_value(true);
			(*it_c)->set_assigned(true);
			var->set_assigned(true);
			var->set_value(true);
			return var;
		}
	}
	return NULL;
}

Variable * DPLL::makeDecision(CNF * cnf)
{
	set<Variable*> *vars = cnf->get_var();
	set<Variable*>::iterator it_v;
	for (it_v = vars->begin(); it_v != vars->end(); it_v++)
		if ((*it_v)->get_assigned() == false)
			return *it_v;
	return NULL;
}

void DPLL::addActiveClauses(CNF * cnf)
{
	set<Clause*>::iterator it_c;
	for (it_c = cnf->get_clauses()->begin(); it_c != cnf->get_clauses()->end(); it_c++) {
		if ((*it_c)->get_assigned() == true)  //Check this logic! Should I continue?
			continue;
		int size = (*it_c)->get_var()->size();
		if (size == 1) {
			//Check if Tau already has the element
			if (std::find(tau.begin(), tau.end(), *it_c) != tau.end()) {
				continue;
			}
			else {
				tau.push_back((*it_c));
				//Check
				(*it_c)->set_assigned(true);
				(*it_c)->set_value(true);
				(*(*it_c)->get_var()->begin())->set_assigned(true);
				(*(*it_c)->get_var()->begin())->set_value(true);
			}
		}
	}
}

void DPLL::restore_symbol(Variable *v) {
	v->set_assigned(false);
}
/*
bool DPLL::moveWatch(Clause * clause, int i)
{
	if (clause == NULL)
		return false;
	set<Variable*>::iterator it_v;
	for (it_v = clause->get_var()->begin(); it_v != clause->get_var()->end(); it_v++) {
		if ((*it_v)->get_watched() == false) {
			if (sigmaCheck(*it_v) == true) {
				clause->set
			}
		}
	}
	return false;
}
*/
void DPLL::restore_variables(Variable *v, set<Variable*>* var) {
	set<Variable*>::iterator it_v;
	for (it_v = var->begin(); it_v != var->end(); it_v++) {				
		if ((*it_v)->get_name() == v->get_name()) {				
			(*it_v)->set_assigned(false);
			return;
		}
	}
}


void DPLL::restore_clauses(Variable * var, set<Clause*>* clauses)
{
	set<Clause*>::iterator it_c;
	set<Variable*>::iterator it_v;
	for (it_c = clauses->begin(); it_c != clauses->end(); it_c++) {	
		for (it_v = (*it_c)->get_var()->begin(); it_v != (*it_c)->get_var()->end(); it_v++) { 
			if ((*it_v)->get_name() == var->get_name()) {
				(*it_v)->set_assigned(false);				
			}
		}
		if ((*it_c)->get_assigned() == false) 
			continue;				
		if (ALL_VARIABLES_ARE_FALSE(*it_c) == true) {
			(*it_c)->set_assigned(true);
			(*it_c)->set_value(false);
			continue;
		}
		if (ONE_VARIABLE_IS_TRUE(*it_c) == true) {	
			(*it_c)->set_assigned(true);					
			(*it_c)->set_value(true);					
			continue;
		}
		(*it_c)->set_assigned(false);						
	}
}

void DPLL::fix_pures(set<Variable*> *vars, set<Clause*> *clauses) {
	set<Variable*>::iterator it_v, it_cv;
	set<Clause*>::iterator it_c;
	Variable *var;
	int i;
	bool sign;
	for (it_v = vars->begin(); it_v != vars->end(); it_v++) {			
		if ((*it_v)->get_assigned() == true) 
			continue;	
		else {
			var = *it_v;
			i = 0;
			for (it_c = clauses->begin(); it_c != clauses->end(); it_c++) { 
				if ((*it_c)->get_assigned() == true) 
					continue;
				else {
					for (it_cv = (*it_c)->get_var()->begin(); it_cv != (*it_c)->get_var()->end(); it_cv++) {
						if (i == 2)
							break;
						if ((*it_cv)->get_assigned() == true)
							continue;
						else {
							if ((*it_cv)->get_name() == var->get_name()) {
								if (i == 0) {
									sign = (*it_cv)->get_sign();
									i = 1;
								}
								else {
									if ((*it_cv)->get_sign() != sign) { 
										i = 2;
										break;
									}
								}
							}
						}
					}
					if (i == 2)break;
				}
				if (i == 2)break;
			}
			if (i == 2) continue;
			if (i == 1 || i == 0) {	
				var->set_pure(true);
				var->set_value(sign);
				var->set_sign(sign);
			}
		}
	}
}
