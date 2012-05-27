/*
 * BayesNetwork.h
 *
 * Created on: May 21, 2012
 *      Author: Denis
 */
#ifndef BAYESNETWORK_H_
#define BAYESNETWORK_H_


#include "Utilities/LinkedList/LinkedList.h"
#include "Data\DataTable.h"
#include "../BayesNetwork/Variable.h"

class DataTable;

class BayesNetwork {
private:
	Variable** vars;
	DataTable* data; 


public:
	BayesNetwork(int numVars);
	BayesNetwork(int numVars, char** varNames);
	~BayesNetwork();
	double learnStructure();
	void createProbTables();
	DataTable generateData();

};

BayesNetwork::BayesNetwork(int numVars) {
	vars = new Variable* [numVars];
	char* name = "";

	for(int i = 0; i < numVars; i++) {
		sprintf(name, "%d", i);
		vars[i] = new Variable(name);
	}

	delete[] name;
}

BayesNetwork::BayesNetwork(int numVars, char** varNames) {
	vars = new Variable*[numVars];
	for(int i = 0; i < numVars; i++) {
		vars[i] = new Variable(varNames[i]);
	}
}

#include "../Data/DataTable.h"


#endif
