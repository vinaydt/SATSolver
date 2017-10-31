#include "Clause.h"

Clause::Clause(const set<Variable*> _var)
{
	value = true;
	assigned = false;
	set<Variable*>::iterator it_v;
	for (it_v = _var.begin(); it_v != _var.end(); it_v++) {
		variableSet.insert(*it_v);
	}
	value = true;
	//initialiseWatchLiterals(_var);
}

Clause::~Clause()
{
}

string Clause::get_unit()
{
		set<Variable*>::iterator it_v, it_v2;
		string unit;
		for (it_v = variableSet.begin(); it_v != variableSet.end(); it_v++) {
			if ((*it_v)->get_sign() == false)
				unit += "~";
			unit += (*it_v)->get_name();
			if ((++it_v) != variableSet.end())unit += " ^ ";
			it_v--;
		}
		return unit;
}
