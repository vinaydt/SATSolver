#pragma once
#include <set>
#include<iostream>
#include "Variable.h"
using namespace std;
class Clause
{
	set<Variable*> variableSet;		
	bool value;	
	bool assigned;
	//Variable *watch[2];
public:
	Clause(const set<Variable*> _var);
	~Clause();
	string get_unit();
	bool get_assigned() const{
		return assigned;
	}
	void set_assigned(bool _val) {
		assigned = _val;
	}
	void set_value(bool _val) {
		value = _val;
	}
	set<Variable*>* get_var() {
		if (variableSet.empty())
			throw("Empty Variable Set");
		return &variableSet;
	}

	set<Variable*> get_var_set() {
		if (variableSet.empty())
			throw("Empty Variable Set");
		cout << variableSet.size();
		return variableSet;
	}
	/*
	void set_watch_variable(Variable * var, int i) {

	}
	void initialiseWatchLiterals(set<Variable*> variable_set) {

	}
	*/
};

