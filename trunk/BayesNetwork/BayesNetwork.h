/*
 * BayesNetwork.h
 *
 * Created on: May 21, 2012
 *      Author: Denis
 */
#ifndef BAYESNETWORK_H_
#define BAYESNETWORK_H_


#include "../Data/DataTable.h"


class BayesNetwork {
private:
	Variable** vars;		//maybe unnessesary
	DataTable* data; 
	int numVars;			//maybe unnessesary


public:
	//to be used with already imported datatable
	BayesNetwork(DataTable* data);

	//to be used to init network from a file
	//and use random cpts
	BayesNetwork(char* filePath,int numCases);

	//to be used to init network from a file
	//and to init cpts from files
	BayesNetwork(char* filePath,int numCases,char* cpt_file[]);



	~BayesNetwork();
	double learnStructure();
	void createProbTables();



	void print();
	int getVarId(string name);
private:
	//these two are used for sampling
	DataTable* generateDataFileCDT(int numCases, char* cpt_file[]);
	DataTable* generateDataRandomCDT(int numCases);
	DataTable* generateData(int numCases);
	void readStructure(char* filePath);
};




#endif
