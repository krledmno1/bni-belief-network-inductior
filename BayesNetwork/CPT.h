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

class Variable;

class CPT {
private:


	IndexTree<double> cpt;

public:
	CPT(Variable* var);
	CPT();
	~CPT();
	void setProb(int* parent_values, float value);
	double* getProb(int* parent_values);
	int generateValue(int* parent_values);

};


CPT::CPT()
{

}

CPT::CPT(Variable* var)
{
	cpt.var = var;
	cpt.generateTree();
}

void CPT::setProb(int* parent_values, double value)
{
	//TODO
}
double* CPT::getProb(int* parent_values)
{
	return cpt.getProbabilities(parent_values);
}

int CPT::generateValue(int* parent_values)
{
	//TODO
}



CPT::~CPT()
{

}

#include "Variable.h"

#endif
