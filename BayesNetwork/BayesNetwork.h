/*
 * BayesNetwork.h
 *
 * Created on: May 21, 2012
 *      Author: Denis
 */
#ifndef BAYESNETWORK_H_
#define BAYESNETWORK_H_


#include "../Utilities/LinkedList/LinkedList.h"
#include "../Data/DataTable.h"
#include "../BayesNetwork/Variable.h"

class DataTable;

class BayesNetwork {
private:
	Variable** vars;
	DataTable* data; 
	int numVars;


public:
	BayesNetwork(DataTable* data);

	~BayesNetwork();
	double learnStructure();
	void createProbTables();
	DataTable generateData();

};

BayesNetwork::BayesNetwork(DataTable* data) {
	this->data=data;
	vars = data->getVariables();
	numVars = data->getNumVars();
}



#include "../Data/DataTable.h"


#endif
