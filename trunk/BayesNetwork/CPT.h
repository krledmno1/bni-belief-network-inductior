/*
 * CPT.h - Conditional probability table
 *
 * Created on: May 21, 2012
 *      Author: Denis
 */
#ifndef CPT_H_
#define CPT_H_

#include <time.h>
#include <fstream>
#include <iostream>

using namespace std;

#include "../IndexTree/IndexTree.h"


class CPT {
private:

	IndexTree<double>* cpt;

public:
	CPT(Variable* var);
	~CPT();
	void setProb(int* parent_values, double value);
	int sample();
	double getProb(int* parent_values, int variableValueId);
	int generateValue(int* parent_values, int numValues);
	void readValuesFromFile(char* filePath);
	void generateRandomValues();
	void generateRandomProbabilities(int* parent_values, int numValues);
	void print();
};

#include "../IndexTree/IndexTree.h"


#endif
