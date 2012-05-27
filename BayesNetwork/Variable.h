/*
 * Variable.h
 *
 * Created on: May 21, 2012
 *
 */
#ifndef VARIABLE_H_
#define VARIABLE_H_

#include "../Utilities/LinkedList/LinkedList.h"
//#include "Utilities\LinkedList\LinkedList.h"
//#include "CPT.h"
#include <string>
#include <iostream>


using namespace std;


class Variable {
private:
	static int varCounter;
	LinkedList<Variable>* parents;
	LinkedList<Variable>* children;
	LinkedList<string>* values;
	//CPT* cpt;
	//int sampled_value;

public:
	int id;
	string* name;
	Variable(string* name);

	~Variable();
	void addParent(Variable* parent);
	void addChild(Variable* child);
	int getNumValues();
	int addValue(string* value);
	string* getValueName(int valueId);
	string* getValueNames();
	const static int MAX_VAR_NAME_LENGTH = 10;
	const static int MAX_VAR_VALUE_LENGTH = 10;
};

int Variable::varCounter = 0;


Variable::Variable(string* name) {
	id = varCounter;
	varCounter++;

	this->name = name;
	this->parents = new LinkedList<Variable>();
	this->children = new LinkedList<Variable>();
	this->values = new LinkedList<string>();
}

Variable::~Variable() {
	delete parents;
	delete children;
	delete this->values;
	/*delete cpt;*/
	delete  name;

}

void Variable::addParent(Variable* parent) {
	this->parents->addToBack(parent);
	parent->children->addToBack(this);
}

void Variable::addChild(Variable* child) {
	this->children->addToBack(child);
	child->parents->addToBack(this);
}

int Variable::getNumValues() {
	return values->getSize();
}

int Variable::addValue(string* value) {

	//for(Node<string>* node = values->start;node!=NULL;node=node->getNext())

	Node<string>* node = values->start;

	int i = 0;
	string* nodeContent;
	while(node != NULL) {
		nodeContent = node->getContent();
		if(nodeContent->compare(*value) == 0) {
			delete value;
			return i;
		}
		i++;
		node = node->getNext();
	}

	values->addToBack(value);
	return i;
}

string* Variable::getValueName(int valueId) {
	Node<string>* node = values->start;


	int i = 0;
	while(i < valueId && node != NULL) {
		node = node->getNext();
	}

	if(node != NULL) {
		return node->getContent();
	}

	return NULL;

}

string* Variable::getValueNames() {
	int numValues = values->getSize();
	string* valueNames = new string [numValues];

	Node<string>* node = values->start;
	int i = 0;
	while (node != NULL) {
		valueNames[i] = *(node->getContent());
		node = node->getNext();
		i++;
	}
	//destroy valueNames
	return valueNames;
}
#endif

