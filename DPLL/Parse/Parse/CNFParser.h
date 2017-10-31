#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <set>
#include <sys/stat.h>
#include <list>
#include "Variable.h"
#include "Clause.h"
#include "CNF.h"

using namespace std;

class CNFParser
{
private:
	set<Variable*> var;
	set<Clause*> clauses;
	set<CNF*> cnfs;
	fstream *input;
public:
	CNFParser(char* filename);
	void printCNF();
	~CNFParser();
	set<CNF*> get_CNFS() {
		return cnfs;
	}
	int parsing();
	void readSpace(fstream*);
	void readSpace(istringstream*);
	//void readCNFSentences(fstream*, string &, bool &, bool &, istringstream *, set<Variable*> &);
	string take_format();
	bool isPure(Variable *v);
};

