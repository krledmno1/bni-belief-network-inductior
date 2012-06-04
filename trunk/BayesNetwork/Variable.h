/*
 * Variable.h
 *
 * Created on: May 21, 2012
 *
 */
#ifndef VARIABLE_H_
#define VARIABLE_H_



#include "../Utilities/LinkedList/LinkedList.h"
#include <string>
#include <iostream>
using namespace std;


class CPT;

class Variable {
public:
	static int varCounter;
	LinkedList<Variable>* parents;
	LinkedList<Variable>* children;
	LinkedList<string>* values;
	CPT* cpt;
	int sampled_value;


	int id;
	string* name;
	Variable(string* name);

	~Variable();
	void addParent(Variable* parent);
	void addChild(Variable* child);
	int getNumValues();
	int addValue(string* value);
	void print();
	void printChildren();
	string* getValueName(int valueId);
	string* getValueNames();
	const static int MAX_VAR_NAME_LENGTH = 10;
	const static int MAX_VAR_VALUE_LENGTH = 10;
};

#include "CPT.h"


#endif

