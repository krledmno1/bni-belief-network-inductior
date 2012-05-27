/*
 * DataTable.h
 *
 * Created on: May 21, 2012
 *      Author: Denis
 */
#ifndef DATATABLE__H_
#define DATATABLE__H_


#include "../Utilities/LinkedList/LinkedList.h"
#include "../BayesNetwork/Variable.h"
#include <time.h>
#include <iostream>
using namespace std;

class BayesNetwork;

class DataTable {
private:
	int** table;
	int numCases;
	int numVars;
	DataTable();

public:
	static DataTable* getRandomTable(int numCases, int numVars, int maxNumValues, int minNumValues);
	static DataTable* getRandomTable(int numCases, LinkedList<Variable> variables);
	static DataTable* getTableFromFile(char* filePath);
	static DataTable* generateTableFromBN(BayesNetwork* network);
	int getNumCases();
	int* getCase(int index);
	int getNumVars();
};

DataTable::DataTable()
{

}

int DataTable::getNumCases() {
	return numCases;
}

int DataTable::getNumVars() {
	return numVars;
}

int* DataTable::getCase(int index) {
	return table[index];
}

DataTable* DataTable::getRandomTable(int numCases, int numVars, int maxNumValues, int minNumValues = 2) {

	if(minNumValues < 2 || minNumValues > maxNumValues || numCases <= 0 || numVars <= 0) {
		cout << "Incorrect parameters!";
		return NULL;
	}

	DataTable* dataTable = new DataTable();
	dataTable->table = new int* [numCases];
	for(int i = 0; i < numCases; i++) {
		dataTable->table[i] = new int [numVars];
	}

	Variable** vars = new Variable*[numVars];

	srand((unsigned int) time(0));
	Variable* var;
	char* varName;
	int numValues;
	for(int i = 0; i < numVars; i++) {
		sprintf(varName, "%d", i);
		var = new Variable(varName);

		numValues = (rand() % (maxNumValues - minNumValues + 1)) + minNumValues;
		char* valueName;
		for(int j = 0; j < numValues; j++) {
			sprintf(valueName, "%d", j);
			var->addValue(valueName);
		}

		vars[i] = var;
	}

	for(int i = 0; i < numCases; i++) {
		for(int j = 0; j < numVars; j++)
		{
			numValues = vars[j]->getNumValues();
			dataTable->table[i][j] = rand() % numValues;

			cout << dataTable->table[i][j] << " ";
		}

		cout << "\n";
	}

	dataTable->numCases = numCases;
	dataTable->numVars = numVars;

	return dataTable;
}


#include "../BayesNetwork/BayesNetwork.h"


#endif
