/*
 * Variable.cpp
 *
 *  Created on: Jun 3, 2012
 *      Author: krle
 */

#include "Variable.h"

int Variable::varCounter = 0;

Variable::Variable()
{
	id = -1;
}

Variable::Variable(string* name) {
	id = varCounter;
	varCounter++;
	sampled_value=0;
	parenthoodPotential=0;
	drawn = false;
	xBox=0;
	yBox=0;

	this->name = name;

	this->parents2 = new map<int, Variable*>();
	this->children2 = new map<int, Variable*>();
	this->values2 = new map<int, string*>();

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

	this->parents2 = new map<int, Variable*>();
	this->children2 = new map<int, Variable*>();
	this->values2 = new map<int, string*>();

	map<int, string*>::iterator it;
	for(it = var->values2->begin(); it != var->values2->end(); it++)
	{
		string* s = new string(*(it->second));
		(*values2)[it->first] = s;
	}
}


void Variable::print()
{
	cout << "Variable "  << id << " ( ";
	map<int, string*>::iterator it;
	for(it = values2->begin(); it != values2->end(); it++)
		cout << *(it->second) << " ";
	cout << ")\n";
}

void Variable::printChildren()
{
	map<int, Variable*>::iterator it;
	for(it = children2->begin(); it != children2->end(); it++)
	{
		cout << "\n  -- >";
		it->second->print();
	}
}

void Variable::printParents()
{
	map<int, Variable*>::iterator it;
	for(it = parents2->begin(); it != parents2->end(); it++)
	{
		cout << "\n  -- > " << it->first << endl; ;
		it->second->print();
	}
}

Variable::~Variable() {
	delete parents2;
	delete children2;
	delete values2;

	delete cpt;
	delete  name;

}

void Variable::addParent(Variable* parent) {
	(*parents2)[parent->id] = parent;
}

void Variable::addChild(Variable* child) {
	(*children2)[child->id] = child;
}

void Variable::removeParent(Variable* parent)
{
	parents2->erase(parent->id);
}
void Variable::removeChild(Variable* child)
{
	children2->erase(child->id);
}

bool Variable::isChild(Variable* parent)
{
	if(parents2->count(parent->id) == 0)
		return false;
	else
		return true;
}
bool Variable::isParent(Variable* child)
{
	if(children2->count(child->id) == 0)
		return false;
	else
		return true;
}

int Variable::getNumValues() {
//	return values->getSize();
	return values2->size();
}

int Variable::addValue(string* value) {
	int i = 0;
	map<int, string*>::iterator it;
	for(it = values2->begin(); it != values2->end(); it++)
	{
		if(value->compare(it->second->data()) == 0)
		{
//			cout << *value << " value already exists\n";
			delete value;
			return i;
		}
		i++;
	}
	(*values2)[values2->size()] = value;
//	cout << "added value " << (*values2)[values2->size()-1]->data() << "\n";

	return i;
}

string* Variable::getValueName(int valueId) {

	string* name = (*values2)[valueId];
	if(name->length() > 0)
		return name;

	return NULL;
}

string** Variable::getValueNames() {
//	int numValues = values->getSize();
	int numValues = values2->size();
	string** valueNames = new string* [numValues];

	int i = 0;
	map<int, string*>::iterator it;
	for(it = values2->begin(); it != values2->end(); it++)
	{
		valueNames[i] = it->second;
		i++;
	}

	return valueNames;
}
