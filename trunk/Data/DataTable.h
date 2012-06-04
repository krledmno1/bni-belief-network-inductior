/*
 * DataTable.h
 *
 * Created on: May 21, 2012
 *      Author: Denis
 */
#ifndef DATATABLE__H_
#define DATATABLE__H_

class Variable;

#include <time.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>


using namespace std;

class DataTable {
private:
	int** table;
	int numCases;
	int numVars;
	Variable** variables;



public:
	DataTable(Variable** vars, int numCases);
	DataTable(int numCases, int numVars, int maxNumValues, int minNumValues);
	DataTable(int numCases, int numVars, Variable** variables);
	DataTable(char* filePath);

	int getNumCases();
	int* getCase(int index);
	int getNumVars();
	void saveTableToFile(char* filePath);
	Variable** getVariables();
};

#include "../BayesNetwork/Variable.h"


#endif

