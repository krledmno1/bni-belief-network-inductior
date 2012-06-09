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
//	this->parents = new LinkedList<Variable>();
//	this->children = new LinkedList<Variable>();
//	this->values = new LinkedList<string>();

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

//	this->parents = new LinkedList<Variable>();
//	this->children = new LinkedList<Variable>();
//	this->values = new LinkedList<string>();

	this->parents2 = new map<int, Variable*>();
	this->children2 = new map<int, Variable*>();
	this->values2 = new map<int, string*>();

//	for(Node<string>* node=var->values->start; node!=NULL;node = node->getNext())
//	{
//		string* s = new string(node->getContent()->c_str());
//		this->values->addToBack(s);
//	}

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
//	for(Node<Variable>* node = children->start; node!=NULL;node = node->getNext())
//	{
//		cout << "\n  -- >";
//		node->getContent()->print();
//	}

	map<int, Variable*>::iterator it;
	for(it = children2->begin(); it != children2->end(); it++)
	{
		cout << "\n  -- >";
		it->second->print();
	}
}

void Variable::printParents()
{
//	for(Node<Variable>* node = parents->start; node!=NULL;node = node->getNext())
//	{
//		cout << "\n  -- >";
//		node->getContent()->print();
//	}

	map<int, Variable*>::iterator it;
	for(it = parents2->begin(); it != parents2->end(); it++)
	{
		cout << "\n  -- > " << it->first << endl; ;
		it->second->print();
	}
}

Variable::~Variable() {
//	delete parents;
//	delete children;
//	delete values;

	delete parents2;
	delete children2;
	delete values2;

	delete cpt;
	delete  name;

}

void Variable::addParent(Variable* parent) {
//	this->parents->addToFront(parent);
	(*parents2)[parent->id] = parent;
}

void Variable::addChild(Variable* child) {
//	this->children->addToFront(child);
	(*children2)[child->id] = child;
}

void Variable::removeParent(Variable* parent)
{
//	if(parents->start->getContent()==parent)
//	{
//		parents->removeFromFront();
//	}
//	else
//	{
//		parents->removeNode(parent);
//	}

	parents2->erase(parent->id);
}
void Variable::removeChild(Variable* child)
{
//	if(children->start->getContent()==child)
//	{
//		children->removeFromFront();
//	}
//	else
//	{
//		children->removeNode(child);
//	}

	children2->erase(child->id);
}

bool Variable::isChild(Variable* parent)
{
//	if(parents->find(parent)==NULL)
//		return false;
//	else
//		return true;

	if((*parents2)[parent->id]->id == -1)
	{
		parents2->erase(parent->id);
		return false;
	}
	else
		return true;
}
bool Variable::isParent(Variable* child)
{
//	if(children->find(child)==NULL)
//			return false;
//		else
//			return true;

	if((*children2)[child->id]->id == -1)
	{
		children2->erase(child->id);
		return false;
	}
	else
		return true;
}

int Variable::getNumValues() {
//	return values->getSize();
	return values2->size();
}

int Variable::addValue(string* value) {

	//for(Node<string>* node = values->start;node!=NULL;node=node->getNext())

//	Node<string>* node = values->start;
//
//	int i = 0;
//	string* nodeContent;
//	while(node != NULL) {
//		nodeContent = node->getContent();
//		if(nodeContent->compare(*value) == 0) {
//			delete value;
//			return i;
//		}
//		i++;
//		node = node->getNext();
//	}
//
//	values->addToBack(value);

	int i = 0;
	map<int, string*>::iterator it;
	for(it = values2->begin(); it != values2->end(); it++)
	{
		if(value->compare(it->second->data()) == 0)
		{
			delete value;
			return i;
		}
		i++;
	}
	(*values2)[values2->size()] = value;

	return i;
}

string* Variable::getValueName(int valueId) {
//	Node<string>* node = values->start;
//
//
//	int i = 0;
//	while(i < valueId && node != NULL) {
//		node = node->getNext();
//		i++;
//	}
//
//	if(node != NULL) {
//		return node->getContent();
//	}

	string* name = (*values2)[valueId];
	if(name->length() > 0)
		return name;

	return NULL;
}

string** Variable::getValueNames() {
//	int numValues = values->getSize();
	int numValues = values2->size();
	string** valueNames = new string* [numValues];

//	Node<string>* node = values->start;
//	int i = 0;
//	while (node != NULL) {
//		valueNames[i] = *(node->getContent());
//		node = node->getNext();
//		i++;
//	}

	int i = 0;
	map<int, string*>::iterator it;
	for(it = values2->begin(); it != values2->end(); it++)
	{
		valueNames[i] = it->second;
		i++;
	}

	return valueNames;
}
