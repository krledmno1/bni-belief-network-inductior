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

	Variable* var;
	IndexTree<double> cpt;

public:
	CPT(Variable* var);
	CPT();
	~CPT();
	void setProb(int* parent_values, float value);
	float getProb(int* parent_values);
	int generateValue(int* parent_values);
private:
	void generateTable();

};


CPT::CPT()
{

}

CPT::CPT(Variable* var)
{
	this->var = var;
	generateTable();
}

void CPT::generateTable()
{
	//TODO: iskopirati iz index tree...
}


CPT::~CPT()
{

}

#include "Variable.h"

#endif
