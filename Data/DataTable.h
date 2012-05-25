/*
 * DataTable.h
 *
 * Created on: May 21, 2012
 *      Author: Denis
 */

#include "LinkedList.h"
#include "Variable.h"
#include <time.h>

class DataTable {
private:
	int table[][];
	int numCases;
	int numVars;
	DataTable();

public:
	static DataTable getRandomTable(int numCases, int numVars, int minNumValues = 2, int maxNumValues);
	static DataTable getRandomTable(int numCases, LinkedList<Variable>);
	static DataTable getTableFromFile(char* filePath);
	static DataTable generateTableFromBN(BayesNewtork* network);
	int getNumCases();
	int* getCase(int index);
	int getNumVars();
};

int DataTable::getNumCases() {
	return numCases;
}

int DataTable::getNumVars() {
	return numVars;
}

int* DataTable::getCase(int index) {
	return table[index];
}

static DataTable DataTable::getRandomTable(int numCases, int numVars, int minNumValues = 2, int maxNumValues) {

	if(minNumValues < 2 || minNumValues > maxNumValues || numCases <= 0 || numVars <= 0) {
		cout << "Incorrect parameters!";
		return NULL;
	}

	DataTable dataTable = new DataTable();
	dataTable.table = new table[numCases][numVars];
	Variable* vars[] = new Variable* [numVars];

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
			dataTable.table[i][j] = rand() % numValues;

			cout << dataTable.table[i][j] << " ";
		}

		cout << "\n";
	}

	dataTable.numCases = numCases;
	dataTable.numVars = numVars;

	return dataTable;
}
