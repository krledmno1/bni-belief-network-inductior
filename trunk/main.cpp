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

	char* bla = strdup("/home/krle/workspace/BNIProject/bni-belief-network-inductior/structure.txt");


	BayesNetwork b(bla,500);

	b.print();

	delete bla;
	return 0;
}


