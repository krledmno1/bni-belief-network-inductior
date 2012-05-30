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
	void print();

};

BayesNetwork::BayesNetwork(DataTable* data) {
	this->data=data;
	vars = data->getVariables();
	numVars = data->getNumVars();
}

void BayesNetwork::print()
{
	cout << "\nPrinting Bayes network:";
	for(int i = 0; i<numVars;i++)
	{
		cout << "\n";
		vars[i]->print();
		vars[i]->printChildren();
	}
}

BayesNetwork::~BayesNetwork() {

	//if we need to destroy datatable
	//if(data!=NULL)
	//	delete data;

	for(int i =0;i<numVars;i++)
		delete vars[i];
	delete [] vars;

}

#include "../Data/DataTable.h"


#endif
