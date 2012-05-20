/*
 * IndexTree.h
 *
 *  Created on: May 20, 2012
 *      Author: krle
 */

#ifndef INDEXTREE_H_
#define INDEXTREE_H_

#include "LinkedList.h"
#include "TreeNode.h"
#include "LeafNode.h"


class IndexTree {
public:
	IndexTree();
	//TO DO: constructor with the database reference as a parameter.
	virtual ~IndexTree();

private:
	TreeNode* root;
	LinkedList<LeafNode> leaves;


};

#endif /* INDEXTREE_H_ */
