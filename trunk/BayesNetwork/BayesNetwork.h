/*
 * BayesNetwork.h
 *
 * Created on: May 21, 2012
 *      Author: Denis
 */

#include "DataTable.h"
#include "Variable.h"

class BayesNetwork {
public:
	BayesNetwork(int numVars);
	BayesNetwork(int numVars, char** varNames);
	~BayesNetwork();
	double learnStructure();
	void createProbTables();
	DataTable generateData();
private:
	Variable vars[];
};
