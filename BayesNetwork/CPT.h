/*
 * CPT.h - Conditional probability table
 *
 * Created on: May 21, 2012
 *      Author: Denis
 */

#include "LinkedList.h"

class CPT {
private:
	void* table; // Index
	int numParents;

public:
	CPT(LinkedList<Variable>  parents);
	~CPT();
	void setProb(int* parent_values, float value);
	float getProb(int* parent_values);
	int generateValue(int* parent_values);

};

CPT::CPT(LinkedList<Variable> parents) {

}
