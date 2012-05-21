/*
 * Variable.h
 *
 * Created on: May 21, 2012
 *
 */

#import "LinkedList.h"
#import "CPT.h"

class Variable {
public:
	int id;
	char* name;
	Variable(char* name);
	~Variable();
	void add_parent(Variable* parent);
	void add_child(Variable* child);
private:
	int variable_counter;
	LinkedList<Variable> parents;
	LinkedList<Variable> children;
};
