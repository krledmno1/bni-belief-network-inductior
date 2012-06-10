/*
 * DataTable.cpp
 *
 *  Created on: Jun 3, 2012
 *      Author: krle
 */

#include "DataTable.h"

DataTable::DataTable(Variable** vars, int numCases)
{

	numVars = Variable::varCounter;
	if (!(numCases > 0 && numVars > 0)) {
		cout << "Incorrect parameters!" << endl;
		return;
	}

	variables=vars;
	this->numCases = numCases;

	table = new int* [numCases];
	for(int i = 0; i < numCases; i++) {
		table[i] = new int [numVars];
	}

}

void DataTable::print()
{
	cout << "\nData table: \n";
	for(int i =0;i<numVars;i++)
	{
		cout << *(variables[i]->name) << "  ";
	}
	cout << "\n";

	for(int j =0;j<numCases;j++)
	{
		for(int i =0;i<numVars;i++)
		{
			cout << table[j][i] << " ";
		}
		cout << "\n";
	}
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

	//srand((unsigned int) time(0));

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
	delete [] numValues;
}

DataTable::DataTable(char* filePath) {

	if (strlen(filePath) > 0)
		{
			cout << "Reading file: " << filePath << "\n";

			this->numVars = 0;
			this->numCases = 0;


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
//					cout << *varName << "\n";
				}

				//count cases
				numCases = 0;
				getline(file, line);
				while(!file.eof()) // For each line
				{
					numCases++;
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
//					cout << "\nCase " << i + 1 << ": ";

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

//						cout << valueId <<  "  ";
					}


					i++;

					getline(file, line);

				}

				cout << "Data has been read\n" << endl;

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


void DataTable::saveTableToFile(char* filePath) {
	if (strlen(filePath) > 0) {
		cout << "\nWriting to file: " << filePath << "\n";


//		string** varNames = new string* [numVars];
//		string*** valueNames = new string** [numVars];
//		for(int i = 0; i < numVars; i++) {
//			varNames[i] = variables[i]->name;
//			valueNames[i] = variables[i]->getValueNames();
//		}

		ofstream file;
		file.open (filePath);

		string firstLine = "";
		for(int i = 0; i < numVars; i++) {
			firstLine.append(variables[i]->name->data()).append(",");
		}
		firstLine.erase(firstLine.size() - 1, 1);

		file << firstLine << "\n";
		string* line;
		map<int, string*>* valueNames;
		for(int i = 0 ; i < numCases; i++) {
			line = new string();
			for(int j = 0; j < numVars; j++) {
				line->append((*(variables[j]->values2))[table[i][j]]->data()).append(",");
//				line->append((*(valueNames[j]))[table[i][j]].data()).append(",");
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

DataTable::~DataTable()
{
	if(table!=NULL)
	{
		for(int i = 0; i< numCases;i++)
		{
			if(table[i]!=NULL)
			{
				delete table[i];
				table[i]=NULL;
			}
		}
		delete [] table;
		table =NULL;
	}

	if(variables!=NULL)
		{
			for(int i = 0; i< numVars;i++)
			{
				if(variables[i]!=NULL)
				{
					delete variables[i];
					variables[i]=NULL;
				}
			}
			delete [] variables;
			variables =NULL;
		}
}



