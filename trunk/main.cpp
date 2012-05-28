/*
 * main.cpp
 *
 *  Created on: May 14, 2012
 *      Author: krle
 */
#include "BayesNetwork/BayesNetwork.h"
#include "IndexTree/BranchNode.h"
#include "IndexTree/LeafNode.h"
#include "IndexTree/TreeNode.h"
#include "BayesNetwork/CPT.h"
#include "Data/DataTable.h"


int main(int argc, char* argv[])
{
	char* bla=NULL;
	DataTable d(bla);
	CPT a;
	BayesNetwork b(&d);
	return 0;
}


