/*
 * BayesNetwork.h
 *
 * Created on: May 21, 2012
 *      Author: Denis
 */
#ifndef BAYESNETWORK_H_
#define BAYESNETWORK_H_


#include "../Data/DataTable.h"
#include "../LookupTable/LookupTable.h"
#include <ctime>
#include <float.h>
#include <omp.h>

class BayesNetwork {
private:
	Variable** vars;		//maybe unnessesary
	DataTable* data; 
	int numVars;			//maybe unnessesary
	double confidence;
	LookupTable* lookupTable;

public:
	Variable** getVariables();
	int getNumVars();
	void clearDraw();


	//to be used with already imported datatable
	BayesNetwork(DataTable* data);

	//to be used to init network from a file
	//and use random cpts
	BayesNetwork(char* filePath,int numCases);

	//to be used to init network from a file
	//and to init cpts from files
	BayesNetwork(char* filePath,int numCases,char* cpt_file, char* outputDataFile);



	~BayesNetwork();

	BayesNetwork* learnStructure(int maxNumParent,int numThreads);
	void createProbTables();

	BayesNetwork(BayesNetwork* net);

	void print();
	int getVarId(string name);
private:

	//these two are used for sampling
	DataTable* generateDataFileCPT(int numCases, char* cpt_file);
	DataTable* generateDataRandomCDT(int numCases);
	DataTable* generateData(int numCases);
	void readStructure(char* filePath);
	double g(Variable* current, Variable* potential);
	int maxR();
	Variable* getBestParent(Variable* currentVariable);
};




#endif
