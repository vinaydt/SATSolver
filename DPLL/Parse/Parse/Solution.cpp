#include "Solution.h"


int main() {
	CNFParser *parser;
	set<CNF*>::iterator it_CNF;
	try {
		parser = new CNFParser("Debug\\Schur_160_5_d22.cnf");
	}
	catch (exception &e) {
		cerr << "exception: " << e.what() << endl;
		return EXIT_FAILURE;
	}
	cout << "Reading Data ...";
	fflush(stdout);
	if (parser->parsing() == EXIT_FAILURE) {
		cout << " Reading failure";
	}
	else {
		cout << "OK!" << endl;
		set<CNF*> cnfs = parser->get_CNFS();
		set<CNF*>::iterator it_cnf;
		for (it_cnf = cnfs.begin(); it_cnf != cnfs.end(); it_cnf++)
		{
			int returnValue = DPLL(*it_cnf).performSearch();
			if (returnValue == SAT)
				cout << "Given CNF is satisfiable!";
			else
				cout << "Given CNF is not satisfiable!";
		}

	}

	cin.get();
}
