/*
 * Variable.cpp
 *
 *  Created on: May 22, 2012
 *      Author: Denis
 */

int Variable::var_counter = 0;

Variable::Variable(char* name) {
	this.id = varCounter;
	varCounter++;

	this.name = name;
	this.parents = new LinkedList<Variable>();
	this.children = new LinkedList<Variable>();
	this.cpt = new CPT();
}

Variable::~Variable() {
	delete parents;
	delete children;
	delete cpt;
	delete [] name;
	delete id;

}

void Variable::addParent(Variable* parent) {
	this.parents.addToBack(parent);
	parent->addChild(this);
}

void Variable::addChild(Variable* child) {
	this.children.addToBack(child);
}
