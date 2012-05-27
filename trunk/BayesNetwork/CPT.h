<<<<<<< .mine
/*
 * CPT.h - Conditional probability table
 *
 * Created on: May 21, 2012
 *      Author: Denis
 */

#include "Utilities\LinkedList\LinkedList.h"
//#include "Variable.h"

class CPT {
private:
	void* table; // Index
	int numParents;

public:
	//CPT(LinkedList<Variable>  parents);
	~CPT();
	void setProb(int* parent_values, float value);
	float getProb(int* parent_values);
	int generateValue(int* parent_values);

};

//CPT::CPT(LinkedList<Variable> parents) {
//
//}
=======
/*
 * CPT.h - Conditional probability table
 *
 * Created on: May 21, 2012
 *      Author: Denis
 */
#ifndef CPT_H_
#define CPT_H_

#include "../Utilities/LinkedList/LinkedList.h"


class Variable;

class CPT {
private:
	void* table; // Index
	int numParents;

public:
	CPT(LinkedList<Variable>  parents);
	CPT();

	~CPT();
	void setProb(int* parent_values, float value);
	float getProb(int* parent_values);
	int generateValue(int* parent_values);

};

CPT::CPT()
{

}

CPT::CPT(LinkedList<Variable> parents) {

}

CPT::~CPT()
{

}

#include "Variable.h"

#endif
>>>>>>> .r13
