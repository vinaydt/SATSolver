#include "CNF.h"



CNF::CNF(set<Variable*> variableSet, set<Clause*> ClauseSet, string sentenceCNF)
{
	set<Variable*>::iterator it_v;
	set<Clause*>::iterator it_c;
	for (it_v = variableSet.begin(); it_v != variableSet.end(); it_v++) {
		var.insert(*it_v);
	}
	for (it_c = ClauseSet.begin(); it_c != ClauseSet.end(); it_c++) {
		clauses.insert(*it_c);
	}
	sentence = sentenceCNF;
}

CNF::CNF(CNF & thisCnf) {
	set<Variable*>::iterator it_v;
	set<Clause*>::iterator it_c;
	for (it_v = thisCnf.get_var()->begin(); it_v != thisCnf.get_var()->end(); it_v++) {	
		this->var.insert(*it_v);
	}
	for (it_c = thisCnf.get_clauses()->begin(); it_c != thisCnf.get_clauses()->end(); it_c++) {
		this->clauses.insert(*it_c);
	}
	this->sentence = thisCnf.get_sentence();
}

CNF::~CNF()
{
}
