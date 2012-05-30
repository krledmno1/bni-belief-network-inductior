/*
 * DataTable.h
 *
 * Created on: May 21, 2012
 *      Author: Denis
 */
#ifndef DATATABLE__H_
#define DATATABLE__H_


//#include "..\Utilities\LinkedList\LinkedList.h"
#include "../Utilities/LinkedList/LinkedList.h"
//#include "..\BayesNetwork\Variable.h"
#include "../BayesNetwork/Variable.h"

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
	DataTable();


public:
	DataTable(int numCases, int numVars, int maxNumValues, int minNumValues);
	DataTable(int numCases, int numVars, Variable** variables);
	DataTable(char* filePath);
	int getNumCases();
	int* getCase(int index);
	int getNumVars();
	void saveTableToFile(char* filePath);
	Variable** getVariables();
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

DataTable::DataTable(int numCases, int numVars, int maxNumValues, int minNumValues = 2) {

	// Input parameters check
	if(minNumValues < 2 || minNumValues > maxNumValues || numCases <= 0 || numVars <= 0) {
		cout << "Incorrect parameters!";
		return;
	}

	srand((unsigned int) time(0));

	// Table initialization
	this->numCases = numCases;
	this->numVars = numVars;
	table = new int* [numCases];

	for(int i = 0; i < numCases; i++) {
		table[i] = new int [numVars];
	}


	// Creates numVars new variables
	variables = new Variable*[numVars];

	string* varName;
	int* numValues = new int[numVars];
	for(int i = 0; i < numVars; i++) {
		stringstream ss;
		ss << "Var" << i;
		varName = new string(ss.str()); // The name of the variable is it's number/id
		variables[i] = new Variable(varName);

		numValues[i] = (rand() % (maxNumValues - minNumValues + 1)) + minNumValues; // The number of values of a variable is element of [minNumValues, maxNumValues]
		string* valueName; // The name of the value is its id
		for(int j = 0; j < numValues[i]; j++) { // Adds numValues possible values of the variable
			
			stringstream ss;
			ss << "Value" << j;
			valueName = new string(ss.str());
			variables[i]->addValue(valueName);
		}


	}

	// Fills the table with random values
	for(int i = 0; i < numCases; i++) {
		for(int j = 0; j < numVars; j++)
		{
			table[i][j] = rand() % numValues[j];
			cout << table[i][j] << " ";
		}
		cout << "\n";
	}
}


DataTable::DataTable(int numCases, int numVars, Variable** variables) {
	
	if (!(numCases > 0 && numVars > 0)) {
		cout << "Incorrect parameters!";
		return;
	}

	// Table initialization
	this->numCases = numCases;
	this->numVars = numVars;
	table = new int* [numCases];
	for(int i = 0; i < numCases; i++) {
		table[i] = new int [numVars];
	}

	// Saves the number of values for each variable
	int* numValues = new int[numVars];
	for (int i = 0; i < numVars; i++) {
		numValues[i] = variables[i]->getNumValues();
	}

	// Generates random values for each table element
	for (int i = 0; i < numCases; i++) {
		for (int j = 0; j < numVars; j++) {
			table[i][j] = rand() % numValues[j];	 
		}
	}
}

DataTable::DataTable(char* filePath) {

	if (strlen(filePath) > 0)
		{
			cout << "Reading file: " << filePath << "\n";

			int numVars = 0;
			int numCases = 0;


			int currSepIndex;
			int oldSepIndex;
			string line;
			ifstream file;
			file.open (filePath);

			if(file.is_open())
			{
				// Reading the first line with the names of the variables
				getline(file, line);

				// Finds the number of variables
				currSepIndex = -1;
				do {
					currSepIndex = line.find(",", currSepIndex + 1);
					numVars++;
				} while(currSepIndex != string::npos);

				cout << "Found " << numVars << " variables (" << line << ")\n";

				//create variables
				this->variables = new Variable*[numVars];

				// Reads the names and creates the variables (all but the last one)
				oldSepIndex = 0;
				currSepIndex = 0;
				string* varName;
				int varIndex = 0;
				while (currSepIndex != string::npos) {

					currSepIndex = line.find_first_of(",", oldSepIndex);

					//crate a string name for the variable (destroyed by Variable)
					varName = new string(line.substr(oldSepIndex, (currSepIndex != string::npos ? currSepIndex : line.size()) - oldSepIndex));

					oldSepIndex = max(oldSepIndex, currSepIndex + 1);

					variables[varIndex] = new Variable(varName);	//destroyed by Bayes network or Data Table...
					varIndex++;
					cout << *varName << "\n";
				}

				//count cases
				numCases = 0;
				getline(file, line);
				while(!file.eof()) // For each line
				{
					numCases++;
					cout << "\nCase " << numCases;
					getline(file, line);

				}

				//create table
				table = new int*[numCases];
				for(int i = 0; i < numCases; i++) {
					table[i] = new int[numVars];

				}

				file.close();					//go to the beggining
				file.open(filePath);
				getline(file,line);				//skip a line with vars

				int i=0;
				int j = 0;
				oldSepIndex = 0;
				currSepIndex = 0;
				int valueId;	//id of the value (returned by the Variable object
				getline(file, line);
				while(!file.eof()) // For each line
				{
					cout << "\nCase " << i + 1 << ": ";

					j = 0;
					oldSepIndex = 0;
					currSepIndex = 0;

					while (currSepIndex != string::npos && !line.empty()){ // For each value on the line

						// Read the value
						currSepIndex = line.find_first_of(",", oldSepIndex);
						string* valueName = new string(line, oldSepIndex, (currSepIndex != string::npos ? currSepIndex : line.size()) - oldSepIndex);
						oldSepIndex = currSepIndex + 1;

						valueId = variables[j]->addValue(valueName); // Add it to the matching variable
						table[i][j] = valueId;
						j++;

						cout << valueId <<  "  ";
					}


					i++;

					cout << "\n";
					getline(file, line);

				}



			}
			else
			{
				cout << "\nError reading file!";
			}
		}
		else
		{
			cout << "\nInvalid file path!";

		}


}


/*DataTable::DataTable(char* filePath) {
	if (strlen(filePath) > 0) {
		cout << "Reading file: " << filePath << "\n";

		int currSepIndex;
		int oldSepIndex;
		string line;
		ifstream file;
		file.open (filePath);

		if(file.is_open())
		{
			// Reading the first line with the names of the variables
			getline(file, line);

			// Finds the number of variables
			currSepIndex = -1;
			int numVars = 0;
			do {
				currSepIndex = line.find(",", currSepIndex + 1);
				numVars++;
			} while(currSepIndex != string::npos);

			cout << "Found " << numVars << " variables (" << line << ")\n";

			variables = new Variable*[numVars];

			// Reads the names and creates the variables (all but the last one)
			oldSepIndex = 0;
			currSepIndex = 0;
			string* varName;
			int varIndex = 0;
			while (currSepIndex != string::npos) {

				currSepIndex = line.find_first_of(",", oldSepIndex);

				varName = new string(line.substr(oldSepIndex, (currSepIndex != string::npos ? currSepIndex : line.size()) - oldSepIndex));

				oldSepIndex = max(oldSepIndex, currSepIndex + 1);

				variables[varIndex] = new Variable(varName);
				varIndex++;
				cout << *varName << "\n";
			}

			// Reads the name and creates the last variable
			//line.copy(varName, line.size() - oldSepIndex, oldSepIndex);
			//vars[varIndex] = new Variable(varName);

			cout << "---------";

			// Reads the values of the variables
			LinkedList<int*>* cases = new LinkedList<int*>();
			int** caseValues;
			int valueId;
			int i = 0; // cases index
			int j = 0; // variable index
			while(!file.eof()) // For each line
			{
				cout << "\nCase " << i + 1 << ": ";

				getline(file, line);

				j = 0;
				oldSepIndex = 0;
				currSepIndex = 0;
				caseValues = new int*[1];
				caseValues[0] = new int[numVars];
				while (currSepIndex != string::npos){ // For each value on the line

					// Read the value
					currSepIndex = line.find_first_of(",", oldSepIndex);
					string* valueName = new string(line, oldSepIndex, (currSepIndex != string::npos ? currSepIndex : line.size()) - oldSepIndex);
					oldSepIndex = currSepIndex + 1;

					valueId = variables[j]->addValue(valueName); // Add it to the matching variable
					caseValues[0][j] = valueId; // Sets the value of the case
					j++;

					cout << valueId << "-" << *valueName << "  ";
				}

				// Adds the case values to the list
				cases->addToBack(caseValues);

				i++;

				cout << "\n";
			}

			file.close();

			this->numVars = numVars;
			this->numCases = cases->getSize();
			table = new int*[numCases];
			Node<int*>* node = cases->start;
			for(int i = 0; i < numCases; i++) {
				table[i] = *(node->getContent());
				node = node->getNext();
			}
			delete cases;

			cout << "---------";
			cout << "Total: " << numCases << " cases read";
			cin.get();

		}
		else
		{
			cout << "\nError reading file!";
		}
	}

}
*/
void DataTable::saveTableToFile(char* filePath) {
	if (strlen(filePath) > 0) {
		cout << "\nWriting to file: " << filePath << "\n";


		string** varNames = new string* [numVars];
		string** valueNames = new string* [numVars];
		for(int i = 0; i < numVars; i++) {
			varNames[i] = variables[i]->name;
			valueNames[i] = variables[i]->getValueNames();
		}

		ofstream file;
		file.open (filePath);

		string firstLine = "";
		for(int i = 0; i < numVars; i++) {
			firstLine.append(varNames[i]->data()).append(",");
		}
		firstLine.erase(firstLine.size() - 1, 1);

		file << firstLine << "\n";

		string* line;
		for(int i = 0 ; i < numCases; i++) {
			line = new string();
			for(int j = 0; j < numVars; j++) {
				line->append(valueNames[j][table[i][j]].data()).append(",");
			}
			line->erase(line->size() - 1, 1);

			file << *line << "\n";
		}

		file.close();

		cout << "\nWrite completed";
	}
}

Variable** DataTable::getVariables()
{
	return variables;
}

#endif

