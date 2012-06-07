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
	parenthoodPotential=0;
	drawn = false;
	xBox=0;
	yBox=0;

	this->name = name;
	this->parents = new LinkedList<Variable>();
	this->children = new LinkedList<Variable>();
	this->values = new LinkedList<string>();

}
Variable::Variable(Variable* var)
{
		this->id = var->id;
		this->name = var->name;
		sampled_value=0;
		parenthoodPotential=0;
		drawn = false;
		xBox=0;
		yBox=0;

		this->parents = new LinkedList<Variable>();
		this->children = new LinkedList<Variable>();
		this->values = new LinkedList<string>();
		for(Node<string>* node=var->values->start; node!=NULL;node = node->getNext())
		{
			string* s = new string(node->getContent()->c_str());
			this->values->addToBack(s);
		}
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
	this->parents->addToFront(parent);
	parent->children->addToFront(this);
}

void Variable::addChild(Variable* child) {
	this->children->addToFront(child);
	child->parents->addToFront(this);
}

void Variable::removeParent(Variable* parent)
{
	if(parents->start->getContent()==parent)
	{
		parents->removeFromFront();
	}
	else
	{
		parents->removeNode(parent);
	}
}
void Variable::removeChild(Variable* child)
{
	if(children->start->getContent()==child)
	{
		children->removeFromFront();
	}
	else
	{
		children->removeNode(child);
	}

}

bool Variable::isChild(Variable* parent)
{
	if(parents->find(parent)==NULL)
		return false;
	else
		return true;
}
bool Variable::isParent(Variable* child)
{
	if(children->find(child)==NULL)
			return false;
		else
			return true;
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
