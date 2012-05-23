/*
 * DataTable.h
 *
 * Created on: May 21, 2012
 *      Author: Denis
 */

class DataTable {
private:
	int table[][];
	int numCases;
	int numVars;

public:
	static void populateTable();
	static int getSize();
	static int* getCase(int i);
	static int getNumVars();

};
