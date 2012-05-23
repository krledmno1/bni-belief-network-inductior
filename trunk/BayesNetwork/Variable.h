/*
 * Variable.h
 *
 * Created on: May 21, 2012
 *
 */

#include "CPT.h"

class Variable {
public:
	int id;
	char* name;
	Variable(char* name);
	~Variable();
	void addParent(Variable* parent);
	void addChild(Variable* child);
private:
	static int var_counter;
	LinkedList<Variable> parents;
	LinkedList<Variable> children;
	CPT* cpt;
	//int sampled_value;
};
