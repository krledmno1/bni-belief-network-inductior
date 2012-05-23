/*
 * BayesNetwork.cpp
 *
 *  Created on: May 22, 2012
 *      Author: Denis
 */

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
