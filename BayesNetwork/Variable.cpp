/*
 * Variable.cpp
 *
 *  Created on: Jun 3, 2012
 *      Author: krle
 */

#include "Variable.h"

int Variable::varCounter = 0;

Variable::Variable(string* name) {
	id = varCounter;
	varCounter++;
	sampled_value=0;

	this->name = name;
	this->parents = new LinkedList<Variable>();
	this->children = new LinkedList<Variable>();
	this->values = new LinkedList<string>();

}



void Variable::print()
{
	cout << "Variable "  << id << " (";
	for(Node<string>* node = values->start;node!=NULL;node=node->getNext())
		cout << *(node->getContent()) << " ";
	cout << ")";
}
void Variable::printChildren()
{
	for(Node<Variable>* node = children->start; node!=NULL;node = node->getNext())
	{
		cout << "\n  -- >";
		node->getContent()->print();
	}
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
		i++;
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
