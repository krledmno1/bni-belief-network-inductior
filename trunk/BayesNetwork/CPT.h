/*
 * CPT.h - Conditional probability table
 *
 * Created on: May 21, 2012
 *
 */

#import "LinkedList.h";

class CPT {
public:
	CPT(LinkedList  parents);
	~CPT();
	void set_prob(int* parent_values, float value);
	float get_prob(int* parent_values);
	int generate_value(int* parent_values);
private:
	void* table;
	int num_parents;
};
