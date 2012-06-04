/*
 * BayesNetwork.cpp
 *
 *  Created on: Jun 3, 2012
 *      Author: krle
 */
#include "BayesNetwork.h"



BayesNetwork::BayesNetwork(DataTable* data) {
	this->data=data;
	vars = data->getVariables();
	numVars = data->getNumVars();
}
BayesNetwork::BayesNetwork(char* filePath,int numCases)
{
	readStructure(filePath);
	generateDataRandomCDT(numCases);
}

BayesNetwork::BayesNetwork(char* filePath,int numCases,char* cpt_file[])
{
	readStructure(filePath);
	generateDataFileCDT(numCases,cpt_file);
}


//sampling wrapper for CPTs from files
DataTable* BayesNetwork::generateDataFileCDT(int numCases, char* cpt_file[])
{
	for(int i = 0; i<numVars;i++)
	{
		vars[i]->cpt->readValuesFromFile(cpt_file[i]);
	}
	return generateData(numCases);
}

//sampling wrapper for random CPTs
DataTable* BayesNetwork::generateDataRandomCDT(int numCases)
{
	for(int i = 0; i<numVars;i++)
	{
		vars[i]->cpt = new CPT(vars[i]);
		vars[i]->cpt->generateRandomValues();
	}
	return generateData(numCases);

}

//actual sampling function
//BIG ASSUMTION: there is no parent-child relation that
//violates the order of the variables in the array.
//That's why sampling function can be invoked in linear order
DataTable* BayesNetwork::generateData(int numCases)
{
	//create the table
	data = new DataTable(vars,numCases);

	//foreach case and var do a sample
	for(int i=0;i<numCases;i++)
	{
	 	for(int j=0;j<numVars;j++)
		{
			data->getCase(i)[j] = vars[j]->cpt->sample();
		}
	}
	return data;
}


//reads the structure of the bayes network (the parent-child relations)
//from a file along with the cardinality of the variable values
//sample file:
/*
x1,x2,x3   <-defines variables
4,2,3	   <-defines caridinality of values for each variable
x2		   <-children of x1
x3 		   <-children of x2 ... and so on
 */

void BayesNetwork::readStructure(char* filePath)
{
	if (strlen(filePath) > 0)
	{
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
					do {
						currSepIndex = line.find(",", currSepIndex + 1);
						numVars++;
					} while(currSepIndex != string::npos);

					cout << "Found " << numVars << " variables (" << line << ")\n";

					//create variables
					this->vars = new Variable*[numVars];

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

						vars[varIndex] = new Variable(varName);	//destroyed by Bayes network or Data Table...
						varIndex++;
						cout << *varName << "\n";
					}

					//read number of values
					oldSepIndex = 0;
					currSepIndex = 0;
					varName = NULL;
					varIndex = 0;
					getline(file, line);

					while (currSepIndex != string::npos) {

						currSepIndex = line.find_first_of(",", oldSepIndex);

						//crate a string name for the variable (destroyed by Variable)
						varName = new string(line.substr(oldSepIndex, (currSepIndex != string::npos ? currSepIndex : line.size()) - oldSepIndex));

						oldSepIndex = max(oldSepIndex, currSepIndex + 1);

						int numValues = atoi(varName->c_str());
						for(int i = 0;i<numValues;i++)
						{
							string* s;
							std::stringstream out;
							out << i;
							s = new string(out.str());
							vars[varIndex]->addValue(s);
						}
						varIndex++;
						delete varName;
					}



					//we create dependencies
					varIndex = 0;

					getline(file, line);
					while(!file.eof())
					{
						oldSepIndex = 0;
						currSepIndex = 0;
						varName = NULL;
						while (currSepIndex != string::npos)
						{

							currSepIndex = line.find_first_of(",", oldSepIndex);

							varName = new string(line.substr(oldSepIndex, (currSepIndex != string::npos ? currSepIndex : line.size()) - oldSepIndex));
							oldSepIndex = max(oldSepIndex, currSepIndex + 1);

							int childId = getVarId(*varName);
							vars[varIndex]->addChild(vars[childId]);

							delete varName;


						}

						varIndex++;
						getline(file, line);

					}
					file.close();

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

int BayesNetwork::getVarId(string name)
{
	if(vars!=NULL)
	{
		for(int i = 0; i<this->numVars;i++)
		{
			if(name.compare(*(vars[i]->name))==0)
			{
				return i;
			}
		}
	}
	return -1;
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
