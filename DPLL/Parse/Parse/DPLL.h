#pragma once
#include<set>
#include<map>
#include<iostream>
#include<stack>
#include"CNF.h"
#include"Variable.h"
#include<vector>
#define SAT 1
#define UNSAT -1
using namespace std;
class DPLL
{
private:
	CNF* cnf;
	map<Variable*, int> sigma;
	stack<Variable*> delta;
	//stack<Clause*> tau;
	vector<Clause*> tau;
	bool CONFLICT;
public:
	DPLL(CNF*);
	~DPLL();
	int performSearch();
	CNF* getCNF() {
		if (cnf == NULL) {
			throw "CNF is NULL! Please verify";
		}
		return cnf;
	}
	void pureLiteralElimination(CNF* cnf);
	void performUnitPropagate(CNF* cnf);
	map<Variable, int> makeDecision(CNF* cnf, map<Variable, int> sigma);
	bool allAssigned(map<Variable*, int> sigma);
	map<Variable*, int>  addToMap(map<Variable*, int> map, Variable *var, bool flag);
	vector<Clause*> addToStack(vector<Clause*>, Variable*, CNF * cnf);
	stack<Variable*> addToStack(stack<Variable*> delta, Variable *var);
	Variable* removeFromStack(stack <Variable*> delta);
	Clause* removeFromStack(vector <Clause*> tau);
	map<Variable*, int> removeFromMap(map<Variable*, int>, Variable *);
	Clause* relevantClauses(map<Variable, int>);
	void Backtrack(Variable *var);
	void fix_pures(set<Variable*> *vars, set<Clause*> *clauses);
	void fix_clauses(Variable *var, set<Clause*> *clauses, bool lvalue, bool flag = true, bool unit = false);
	void fix_variables(Variable *v, set<Variable*>* var, bool u = false);
	bool ALL_VARIABLES_ARE_FALSE(Clause* cl);
	bool ONE_VARIABLE_IS_TRUE(Clause* cl);
	Variable* FIND_PURE_SYMBOL(set<Variable*> *var);
	void processUnitClauseProcessing(CNF * cnf);
	Variable* FIND_UNIT_CLAUSE(set<Clause*> *clauses, set<Variable*> *vars);
	Variable* makeDecision(CNF *cnf);
	void addActiveClauses(CNF *cnf);
	void restore_clauses(Variable* var, set<Clause*> *clauses);
	void restore_variables(Variable *v, set<Variable*>* var);
	void restore_symbol(Variable *v);
	map<Variable*, int> get_sigma() {
		return sigma;
	}
	void set_sigma(map<Variable*, int> _sigma) {
		sigma = _sigma;
	}
	void set_conflict(bool _flag) {
		CONFLICT = _flag;
	}
	bool get_conflict() const {
		return CONFLICT;
	}
	vector<Clause*> get_tau() const {
		return tau;
	}
	void set_sigma(vector<Clause*> _tau) {
		tau = _tau;
	}
	stack<Variable*> get_delta() const {
		return delta;
	}
	void set_sigma(stack<Variable*> _delta) {
		delta = _delta;
	}
	Clause * pickAClause();
	void processConflict(Clause *c);
};

