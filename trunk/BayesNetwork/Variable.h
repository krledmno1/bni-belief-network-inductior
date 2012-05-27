/*
 * Variable.h
 *
 * Created on: May 21, 2012
 *
 */
#ifndef VARIABLE_H_
#define VARIABLE_H_

#include "../Utilities/LinkedList/LinkedList.h"
#include <stdlib.h>
#include "CPT.h"



class Variable {
private:
	static int var_counter;
	LinkedList<Variable> parents;
	LinkedList<Variable> children;
	LinkedList<char*> values;
	CPT* cpt;
	//int sampled_value;

public:
	int id;
	char* name;
	Variable();
	Variable(char* name);
	~Variable();
	void addParent(Variable* parent);
	void addChild(Variable* child);
	int getNumValues();
	void addValue(char* value);
	char* getValueName(int valueId);

};

int Variable::var_counter = 0;

Variable::Variable()
{

}

Variable::Variable(char* name) {
	this->id = var_counter;
	var_counter++;

	this->name = name;
	this->cpt = new CPT();

}

Variable::~Variable() {
	parents.removeAll();
	children.removeAll();
	delete cpt;
	delete [] name;


}

void Variable::addParent(Variable* parent) {
	this->parents.addToBack(parent);
	parent->addChild(this);
}

void Variable::addChild(Variable* child) {
	this->children.addToBack(child);
}

int Variable::getNumValues() {
	return values.getSize();
}

void Variable::addValue(char* value) {
	values.addToBack(&value);
}

char* Variable::getValueName(int valueId) {
	Node<char*>* node = values.start;

		int i = 0;
		while(i < valueId && node != NULL) {
			node = node->getNext();
		}

		if(node != NULL) {
			return *(node->getContent());
		}

		return NULL;

}



#endif
