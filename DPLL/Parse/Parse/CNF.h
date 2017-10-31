#pragma once
#include<set>
#include"Variable.h"
#include "Clause.h"
using namespace std;

class CNF
{
private:
	set<Variable*> var;
	set<Clause*> clauses;
	string sentence;
	set<Clause*> singletonClauses;
public:
	CNF(set<Variable*>, set<Clause*>, string);
	CNF(CNF &cnf);
	~CNF();
	string get_sentence() const {
		return sentence;
	}
	set<Clause*>* get_clauses() {
		return &clauses;
	}
	set<Variable*>* get_var() {
		return &var;
	}
};

