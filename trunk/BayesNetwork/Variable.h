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
#include <map>
using namespace std;


class CPT;

class Variable {
public:
	static int varCounter;
//	LinkedList<Variable>* parents;
//	LinkedList<Variable>* children;
//	LinkedList<string>* values;

	map<int, Variable*>* parents2;
	map<int, Variable*>* children2;
	map<int, string*>* values2;

	CPT* cpt;
	int sampled_value;
	double parenthoodPotential;
	bool drawn;
	int xBox;
	int yBox;


	int id;
	string* name;
	Variable();
	Variable(string* name);
	Variable(Variable* var);

	~Variable();
	void addParent(Variable* parent);
	void addChild(Variable* child);
	void removeParent(Variable* parent);
	void removeChild(Variable* child);
	bool isChild(Variable* parent);
	bool isParent(Variable* child);


	int getNumValues();
	int addValue(string* value);
	void print();
	void printChildren();
	void printParents();
	string* getValueName(int valueId);
	string** getValueNames();
	const static int MAX_VAR_NAME_LENGTH = 10;
	const static int MAX_VAR_VALUE_LENGTH = 10;
};

#include "CPT.h"


#endif

