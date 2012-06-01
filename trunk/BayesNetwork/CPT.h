/*
 * CPT.h - Conditional probability table
 *
 * Created on: May 21, 2012
 *      Author: Denis
 */
#ifndef CPT_H_
#define CPT_H_

#include "../Utilities/LinkedList/LinkedList.h"
#include "../IndexTree/IndexTree.h"
#include <time.h>
#include <fstream>
#include <iostream>

using namespace std;

class CPT {
private:

	IndexTree<double>* cpt;

public:
	CPT(Variable* var);
	CPT();
	~CPT();
	void setProb(int* parent_values, double value);
	double getProb(int* parent_values, int variableValueId);
	int generateValue(int* parent_values, int numValues);
	void readValuesFromFile(char* filePath, Variable* var);
	void generateRandomValues(Variable* var);
	void generateRandomProbabilities(int* parent_values, int numValues);
};


CPT::CPT()
{

}

CPT::CPT(Variable* var)
{
	cpt = new IndexTree<double>(var);
}

/*
 * Parses the file with the following formatting:
 *
 * var1_id
 * {
 * parent1_val_id,...parentN_val_id,var_val1_prob,...,var_valN_prob
 * parent1_val_id,...parentN_val_id,var_val1_prob,...,var_valN_prob
 * ...
 * }
 * var2_id
 * {
 * ...
 * }
 * ...
 *
 * where varN_id is the id of the variable, parentN_val_id comma separated values
 * represent the values of the parents and var_valN_prob comma separated values
 * represent the probabilities for the value of the variable given the values of
 * the parents.
 */
void CPT::readValuesFromFile(char* filePath, Variable* var)
{
	string line;
	ifstream file;
	file.open (filePath);

	if(file.is_open())
	{
		while(!file.eof())
		{
			getline(file, line); // Reads the line with the variable's id
			if(var->id == atoi(line.c_str())) // If it is the current variable's id
			{
				int numParents = var->parents->getSize();
				int* parentValues = new int[numParents];

				int numValues = var->getNumValues();

				getline(file, line); // reads the { symbol

				getline(file, line); // reads the first value with the values
				while(line.compare("}") != 0) // until it reaches the last line
				{
					// Reads the parents's values
					int oldSeparatorIndex = 0;
					int newSeparatorIndex = 0;
					for(int i = 0; i < numParents; i++)
					{
						newSeparatorIndex = line.find(",");
						parentValues[i] = atoi(line.substr(oldSeparatorIndex, newSeparatorIndex - oldSeparatorIndex).c_str());
						oldSeparatorIndex = newSeparatorIndex + 1;
					}

					// Reads the probabilities and stores them in the CPT
					for(int i = 0; i < numValues - 1; i++)
					{
						newSeparatorIndex = line.find(",");
						cpt->getProbabilities(parentValues)[i] = atof(line.substr(oldSeparatorIndex, newSeparatorIndex - oldSeparatorIndex).c_str());
						oldSeparatorIndex = newSeparatorIndex + 1;
					}
					cpt->getProbabilities(parentValues)[numValues - 1] = atof(line.substr(oldSeparatorIndex, line.size() - oldSeparatorIndex).c_str());

					delete [] parentValues; // dealocates the memory of the parents' values

					getline(file, line); // reads the next line
				}

				break;
			}
			else // if it is not the data for the current variable skip the section
			{
				getline(file, line); // reads the { symbol

				// loops until it gets to the end of the section
				getline(file, line);
				while(line.compare("}") != 0)
				{
					getline(file, line);
				}
			}
		}
	}
}

/*
 * Generates the random probabilities for each possible combination
 * of values of the parents.
 */
void CPT::generateRandomValues(Variable* var)
{
	int numParents = var->parents->getSize();
	int* numPrevParentValues = new int[numParents]; // Number of combinations of values of the parents up to (including) the i-th parent
	int* numParentValues = new int[numParents]; // Number of values of the i-th parent

	// Fills the arrays
	numPrevParentValues[0] =  var->parents->start->getContent()->getNumValues();
	numParentValues[0] = var->parents->start->getContent()->getNumValues();
	if(numParents > 1)
	{
		int i = 1;
		for(Node<Variable>* node = var->parents->start->getNext(); node != NULL; node = node->getNext())
		{
			numParentValues[i] = node->getContent()->getNumValues();
			numPrevParentValues[i] = numPrevParentValues[i - 1] * numParentValues[i];
			i++;
		}
	}

	// Number of cases is the product of the number of values of each parent
	int numCases = numPrevParentValues[numParents - 1];

	/*
	 * Generates all the possible value combinations and calls the function to generate the probability for each
	 *	For a variable with 4 parents all of them binary it would generate the following cases (in this order):
	 *	0000
	 *	0001
	 *	0010
	 *	0011
	 *	...
	 */

	int* parentValues;
	for(int k = 0; k < numCases; k++)
	{
		// A combination of the value of the parents
		parentValues = new int[numParents];

		// Maps a case number to a combination of values of the parents
		for(int l = numParents - 1; l > 0; l--)
		{
			// (number of cases / number of possible values of the other variables) % number of values of this variable
			parentValues[l] = (k / numPrevParentValues[l - 1]) % numParentValues[l];
			cout << parentValues[l] << " ";
		}
		parentValues[0] = k % numParentValues[0];

		cout << parentValues[0] << "\n";

		// Assigns the probabilities for the combination of parents
		generateRandomProbabilities(parentValues, var->getNumValues());

		delete [] parentValues;
	}

	delete [] numParentValues;
	delete [] numPrevParentValues;
}

void CPT::setProb(int* parent_values, double value)
{
	*(cpt->getProbabilities(parent_values)) = value;
}

double CPT::getProb(int* parent_values, int variableValueId)
{
	return cpt->getProbabilities(parent_values)[variableValueId];
}

/*
 * The function has to generate the probabilities for all the values of variable
 * give the values of the parents, since the sum of these probabilities has to
 * sum up to 1.
 */
void CPT::generateRandomProbabilities(int* parent_values, int numValues)
{
	const int MAX_RANGE = 100;
	srand((unsigned int) time(0));

	// Variables have to have at least two possible values, otherwise they can be dropped
	if(numValues < 2)
	{
		cout << "There has to be at least 2 values!";
		return;
	}

	/*
	 * The idea is to restrict the probability that a variable value
	 * can be assigned depending on the probabilities of the previous
	 * values.
	 * E.g. if the variable has three possible values (0, 1, 2) one possible
	 * execution is that for 0 the program picks a random value ranging from
	 * 0 to MAX_RANGE (e.g. 100), lets say 28. Then, for 1 it chooses between
	 * 0 and 72, lets say 50 and then the last one has to get 22. So the assigned
	 * probabilities will be 0.28, 0.5, 0.22.
	 */
	int range = MAX_RANGE;
	int currVarValue;
	for(int i = 0; i < numValues - 1; i++)
	{
		currVarValue = rand() % (range + 1); // pick a random value
		cpt->getProbabilities(parent_values)[i] = (double) currVarValue / (double) MAX_RANGE; // assign the probability value/MAX_RANGE
		cout << cpt->getProbabilities(parent_values)[i] << " ";
		range -= currVarValue; // restrict the possible values that can be picked
	}
	 // assign the last probability so that all the probabilities sum up to 1
	cpt->getProbabilities(parent_values)[numValues - 1] = (double) range / (double) MAX_RANGE;
	cout << cpt->getProbabilities(parent_values)[numValues - 1] << "\n";
}

/*
 * Returns a value id with the probability specified in the
 * CPT conditioned on the value of the parents.
 *
 * Generates a number between 0 and 1.
 * Returns the first value id such that the sum of all the
 * probabilities up to (including) that value is greater than
 * the generated number.
 *
 */
int CPT::generateValue(int* parent_values, int numValues)
{
	const int MAX_RANGE = 10000;
	srand((unsigned int) time(0));

	// Get the probabilities for variables value conditioned on the give parents' values
	double* probabilities = cpt->getProbabilities(parent_values);

	// Picks a random value
	double randomNumber = (double) (rand() % (MAX_RANGE + 1)) / (double) MAX_RANGE;
	// Accumulates the probabilities of the variable's values
	double acc = 0;
	for(int i = 0; i < numValues; i++)
	{
		// Sums the probabilites
		acc = acc + probabilities[i];
		// Checks if the sum is greater than the random number
		if(randomNumber <= acc)
		{
			return i;
		}
	}

	// Something went terribly wrong if the line executes :)
	return -1;
}


CPT::~CPT()
{
	delete cpt;
}

#include "Variable.h"

#endif
