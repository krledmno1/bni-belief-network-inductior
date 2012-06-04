/*
 * main.cpp
 *
 *  Created on: May 14, 2012
 *      Author: krle
 */

#include "BayesNetwork/BayesNetwork.h"

int main(int argc, char* argv[])
{


	char* bla = strdup("/home/krle/workspace/BNIProject/bni-belief-network-inductior/structure.txt");


	BayesNetwork b(bla,7);

	b.print();
	delete bla;
	return 0;
}


