#include "CNFParser.h"



CNFParser::CNFParser(char* filename)
{
	struct stat info;
	/*
	if (stat(filename, &info))
		throw "File Not Found Exception";
	*/
	input = new fstream(filename, fstream::in);	
	/*
	if (input->fail()) {
		throw "File Not Found Exception";
	}
	*/
}

CNFParser::~CNFParser()
{
}

void CNFParser::readSpace(fstream *input)
{
	*input >> ws;
}

void CNFParser::readSpace(istringstream *input)
{
	*input >> ws;
}

string CNFParser::take_format()
{
	
		set<Clause*>::iterator it_c;
		string cnf;
		cnf = "CNF: ";
		for (it_c = clauses.begin(); it_c != clauses.end(); it_c++) {
			cnf += "( ";
			cnf += (*it_c)->get_unit();
			cnf += " ) ";
			if ((++it_c) != clauses.end())cnf += " v ";
			it_c--;
		}
		return cnf;
}

bool CNFParser::isPure(Variable * v)
{
		set<Variable *>::iterator it_v;
		bool flag = true;
		for (it_v = var.begin(); it_v != var.end(); it_v++) {
			if ((*it_v)->get_name() == v->get_name()) {
				if (v->get_sign() != (*it_v)->get_sign()) (*it_v)->set_pure(false);
				flag = false;
				break;
			}
		}
		return flag;
}

/*
void CNFParser::readCNFSentences(fstream *input, string &line, bool &commentFlag, bool &pLetterFlag, istringstream *ins)
{
	

}
*/

int CNFParser::parsing()
{
	string line,p;
	istringstream *ins;
	set<Variable*> variable;
	int noOfVariables, noOfClauses;
	bool pLetterFlag = false, commentFlag = false, sign;
	Clause *clause;
	string VAR;
	string name;
	Variable *v, *vtmp;
	CNF *cnf;
	getline(*input, line);
	int i = 0;
	while (!input->eof()) {
		switch (line[0]) {
		case 'c':
			readSpace(input);
			break;
		case 'p':
			ins = new istringstream(line);
			*ins >> ws >> p >> ws >> noOfVariables >> ws >> noOfClauses >> ws;
			delete ins;
			readSpace(input);
			//readCNFSentences(input, &line, &commentFlag, &pLetterFlag, ins, &variable);
			while (getline(*input, line)) {
				/*
				if (i % 5 == 0) {
					cout << i << endl;
				}
				i++;
				*/
				if (line[0] == 'c') {
					commentFlag = true;
					break;
				}
				else if (line[0] == 'p') {
					pLetterFlag = true;
					break;
				}
				else {
					ins = new istringstream(line);
					readSpace(ins);
					while (!ins->eof()) {
						readSpace(ins);
						*ins >> VAR;
						if (VAR == "0") 
							break;
						else if (VAR[0] == '-') {
							sign = false;
							name = VAR.substr(1);
						}
						else {
							sign = true;
							name = VAR;	
						}
						v = new  Variable(sign, true, true, name);
						if (isPure(v) == true) {
							vtmp = new Variable(*v);
							var.insert(vtmp);
							vtmp = NULL;
						}
						variable.insert(v);	
					}
					clause = new Clause(variable);
					variable.clear();
					clauses.insert(clause);	
					clause = NULL;
					*input >> ws;
					line.clear();
					delete ins;
				}
			}
			cnf = new CNF(var, clauses, take_format());
			cnfs.insert(cnf);
			clauses.clear();
			var.clear();
			cnf = NULL;

			break;
		default:
			return EXIT_FAILURE;
		}
		if (pLetterFlag == true)
			pLetterFlag = false;
		else if (commentFlag == true) {
			commentFlag = false;
			line.clear();
			readSpace(input);
			getline(*input, line);
		}
		else {
			line.clear();
			readSpace(input);
			getline(*input, line);
		}
	}
	return 0;
}
