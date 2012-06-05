/*
 * main.cpp
 *
 *  Created on: May 14, 2012
 *      Author: krle
 */

#include "BayesNetwork/BayesNetwork.h"

int main(int argc, char* argv[])
{
	srand((unsigned int) time(0));

	char* bla = strdup("/home/krle/workspace/BNIProject/bni-belief-network-inductior/data.txt");
	DataTable d(bla);
	BayesNetwork b(&d);

	BayesNetwork* newNet = new BayesNetwork(&b);

	newNet->learnStructure(2);

	newNet->print();

	delete bla;
	delete newNet;
	return 0;
}


