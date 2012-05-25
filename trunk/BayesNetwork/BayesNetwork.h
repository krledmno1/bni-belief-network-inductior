/*
 * BayesNetwork.h
 *
 * Created on: May 21, 2012
 *      Author: Denis
 */

#include "DataTable.h"
#include "Variable.h"

class BayesNetwork {
private:
	Variable vars[];

public:
	BayesNetwork(int numVars);
	BayesNetwork(int numVars, char** varNames);
	~BayesNetwork();
	double learnStructure();
	void createProbTables();
	DataTable generateData();

};

BayesNetwork::BayesNetwork(int numVars) {
	vars = new Variable[numVars];
	char* name;
	for(int i = 0; i < numVars; i++) {
		sprintf(name, "%d", i);
		vars[i] = new Variable(name);
	}

	delete[] name;
}

BayesNetwork::BayesNetwork(int numVars, char** varNames) {
	vars = new Variable[numVars];
	for(int i = 0; i < numVars; i++) {
		vars[i] = new Variable(varNames[i]);
	}
}
