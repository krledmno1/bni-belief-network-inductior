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
	readStructure(filePath);
	generateDataRandomCDT(numCases);
	int r = maxR();
	this->lookupTable = new LookupTable(this->data->getNumCases()+r);
}

BayesNetwork::BayesNetwork(char* filePath,int numCases,char* cpt_file[])
{
	numVars = 0;
	data = NULL;
	vars = NULL;
	readStructure(filePath);
	generateDataFileCPT(numCases,cpt_file);
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
DataTable* BayesNetwork::generateDataFileCPT(int numCases, char* cpt_file[])
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


BayesNetwork* BayesNetwork::learnStructure(int maxNumParent)
{
	if(maxNumParent>=numVars)
		maxNumParent=numVars-1;
	//copy constr. calls copy consr. of vars (without copying parent-child rels)

	for(int i =0;i<numVars;i++)
	{
		Variable* currentVariable = vars[i];

		double contributionOld = g(currentVariable,NULL);
		bool canProceed = true;
		while(canProceed && maxNumParent>currentVariable->parents->getSize())
		{
			Variable* z = getBestParent(currentVariable);
			if(z==NULL)
				break;
			double contributionNew = z->parenthoodPotential;
			if(contributionNew>contributionOld)
			{
				currentVariable->addParent(z);
				contributionOld=contributionNew;
			}
			else
				canProceed=false;
		}
		currentVariable->print();
	}
	return NULL;
}

double BayesNetwork::g(Variable* current, Variable* potential)
{
	if(potential!=NULL)
	{
		current->addParent(potential);
	}
	IndexTree<int> tree(data,current);

	int ri = current->getNumValues()-1;
	double result=0;
	Node<LeafNode<int> >* node = tree.getNijks()->start;
	for(int j = 0;j<tree.getNijks()->getSize();j++)
	{
		result+=lookupTable->lookup(ri);
		result-=lookupTable->lookup(node->getContent()->Nij+ri);

		for(int k = 0;k<ri+1;k++)
		{
			result+=lookupTable->lookup(node->getContent()->Nijk[k]);
		}
		node = node->getNext();
	}

	if(potential!=NULL)
	{
		current->removeParent(potential);
	}
	return result;
}

Variable* BayesNetwork::getBestParent(Variable* currentVariable)
{
	Variable* bestParent = NULL;
	double bestpotential = 0;
	Variable* potentialParent = vars[0];
	int i = 0;
	while(potentialParent!=currentVariable)
	{
		if(!currentVariable->isChild(potentialParent))
		{
			double potential = g(currentVariable,potentialParent);
			if(potential>bestpotential)
			{
				bestParent = potentialParent;
				bestpotential = potential;
			}
		}
		i++;
		potentialParent=vars[i];
	}
	if(bestParent!=NULL)
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

	if(data!=NULL)
	{
		data->print();
	}
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
