/*
 * BayesNetwork.h
 *
 * Created on: May 21, 2012
 *
 */

#include "LinkedList.h"
#include "Variable.h"

class BayesNetwork {
public:
	BayesNetwork(int num_vars);
	~BayesNetwork();
	double learn_structure();
	void create_prob_tables();
	DataTable generate_data();
private:
	Variable vars[];
};
