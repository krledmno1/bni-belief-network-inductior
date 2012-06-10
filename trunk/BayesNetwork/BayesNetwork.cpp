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
	int r = maxR();
	this->lookupTable = new LookupTable(this->data->getNumCases()+r);
}

BayesNetwork::BayesNetwork(BayesNetwork* net)
{
	numVars = net->numVars;
	data = net->data;
	vars = new Variable*[numVars];
	for(int i=0;i<numVars;i++)
	{
		vars[i] = new Variable(net->vars[i]);
	}
	lookupTable = net->lookupTable;

}

BayesNetwork::BayesNetwork(char* filePath,int numCases)
{
	numVars = 0;
	data = NULL;
	vars = NULL;
	cout << "Reading Bayes structure\n";
	readStructure(filePath);
	cout << "Bayes structure read\n";
	generateDataRandomCDT(numCases);
	int r = maxR();
	this->lookupTable = new LookupTable(this->data->getNumCases()+r);
}

BayesNetwork::BayesNetwork(char* filePath,int numCases,char* cpt_file, char* outputDataFile)
{
	numVars = 0;
	data = NULL;
	vars = NULL;
	readStructure(filePath);
	data = generateDataFileCPT(numCases,cpt_file);
	if(outputDataFile != NULL)
		data->saveTableToFile(outputDataFile);

	int r = maxR();
	this->lookupTable = new LookupTable(this->data->getNumCases()+r);
}
int BayesNetwork::maxR()
{
	int max = vars[0]->getNumValues();
	for(int i = 0; i<numVars;i++)
	{
		if(max<vars[i]->getNumValues())
		{
			max = vars[i]->getNumValues();
		}
	}
	return max;
}

//sampling wrapper for CPTs from files
DataTable* BayesNetwork::generateDataFileCPT(int numCases, char* cpt_file)
{
	cout << "Reading CPTs...\n";
	for(int i = 0; i<numVars;i++)
	{
		vars[i]->cpt = new CPT(vars[i]);
		vars[i]->cpt->readValuesFromFile(cpt_file);
	}
	cout << "CPTs read\n";
	return generateData(numCases);
}

//sampling wrapper for random CPTs
DataTable* BayesNetwork::generateDataRandomCDT(int numCases)
{
	cout << "Generating CPTs...\n";
	for(int i = 0; i<numVars;i++)
	{
		cout << "CPT for variable " << i << " \n";
		vars[i]->cpt = new CPT(vars[i]);
		vars[i]->cpt->generateRandomValues();
	}
	cout << "CPTs generated\n";

	cout << "Creating data...\n";
	return generateData(numCases);
	cout << "Data created\n";

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

Variable** BayesNetwork::getVariables()
{
	return vars;
}
int BayesNetwork::getNumVars()
{
	return numVars;
}

void BayesNetwork::clearDraw()
{
	for(int i =0;i<numVars;i++)
	{
		vars[i]->drawn = false;
	}
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
						cout << *varName << " ";
					}
					cout << "\n";

					cout << "Reading number of values...\n";
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
					cout << "Read number of values\n";

					//we create dependencies
					varIndex = 0;

					cout << "Creating dependencies...\n";
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
							if(childId != -1)
							{
								vars[varIndex]->addChild(vars[childId]);
								vars[childId]->addParent(vars[varIndex]);
							}

							delete varName;


						}

						varIndex++;
						getline(file, line);

					}
					file.close();
					cout << "Dependencies created\n";
					cout << "Structure created\n";
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


BayesNetwork* BayesNetwork::learnStructure(int maxNumParent, int numThreads)
{
	cout << "Learning the structure of the Bayes network..." << endl;
	cout << "Number of variables: " << numVars << endl;
	cout << "Max number of parents is " << maxNumParent << endl;
	cout << "Using " << numThreads << " threads." << endl;

	confidence = 0;
	if(maxNumParent>=numVars)
		maxNumParent=numVars-1;

	time_t started = time(NULL);

	#pragma omp parallel num_threads(numThreads)
	{
		#pragma omp for schedule(static, 1)
		for(int i = 0; i < numVars; i++)
		{

			//std::cout << "Thread: " << omp_get_thread_num() << std::endl;
			Variable* currentVariable = vars[i];

			double contributionOld = g(currentVariable,NULL);
//			cout << currentVariable->name->data() << " value without parents " << contributionOld << "\n" << endl;
			bool canProceed = true;
			while(canProceed && maxNumParent > currentVariable->parents2->size())
			{
				Variable* z = getBestParent(currentVariable);
				if(z == NULL)
					break;

				double contributionNew = z->parenthoodPotential;
				//cout << "Variable " << currentVariable->id << " value with " << currentVariable->parents2->size() << " parents " << contributionNew << "\n" << endl;
				if(contributionNew > contributionOld)
				{
					currentVariable->addParent(z);
					z->addChild(currentVariable);
					contributionOld = contributionNew;
					//cout << "Variable " << currentVariable->id << " added parent " << z->name << "\n" << endl;
				}
				else
					canProceed = false;
			}
//			currentVariable->print();
			confidence += contributionOld;
		}
	}

	confidence = -confidence;
	time_t ended = time(NULL);
	cout << "Found structure with confidence: " << confidence << endl;

	cout << "Execution time: " << difftime(ended, started) << "s" << endl;

	return NULL;
}

double BayesNetwork::g(Variable* current, Variable* potential)
{
	if(potential != NULL)
	{
		current->addParent(potential);
	}
	IndexTree<int> tree(data,current);

	int ri = current->getNumValues() - 1; // Since most of the formulae use ri - 1
	double result=0;
	Node<LeafNode<int> >* node = tree.getNijks()->start;
	for(int j = 0; j < tree.getNijks()->getSize(); j++)
	{
		result += lookupTable->lookup(ri);
		result -= lookupTable->lookup(node->getContent()->Nij + ri);

		for(int k = 0; k < ri + 1; k++)
		{
			result += lookupTable->lookup(node->getContent()->Nijk[k]);
		}

		node = node->getNext();
	}

	if(potential != NULL)
	{
		current->removeParent(potential);
	}
	return result;
}

Variable* BayesNetwork::getBestParent(Variable* currentVariable)
{
	Variable* bestParent = NULL;
	double bestpotential = -DBL_MAX;
	Variable* potentialParent = vars[0];
	int i = 0;
	while(potentialParent != currentVariable)
	{
		if(!currentVariable->isChild(potentialParent))
		{
			double potential = g(currentVariable,potentialParent);
			if(potential > bestpotential)
			{
				bestParent = potentialParent;
				bestpotential = potential;
			}
		}

		i++;
		potentialParent = vars[i];
	}

	if(bestParent != NULL)
		bestParent->parenthoodPotential = bestpotential;

	return bestParent;
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

//	if(data!=NULL)
//	{
//		data->print();
//	}
}

BayesNetwork::~BayesNetwork() {

	//if we need to destroy datatable
	//if(data!=NULL)
	//	delete data;
	if(vars!=NULL)
	{
		for(int i =0;i<numVars;i++)
		{
			//delete vars[i];
			vars[i] = NULL;
		}
	delete [] vars;
	vars=NULL;
	}
	if(lookupTable!=NULL)
	{
		delete lookupTable;
		lookupTable=NULL;
	}

}
