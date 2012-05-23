/*
 * IndexTree.h
 *
 *  Created on: May 20, 2012
 *      Author: krle
 */

#ifndef INDEXTREE_H_
#define INDEXTREE_H_

#include "LinkedList.h"
#include "BranchNode.h"
#include "LeafNode1.h"




template<class T>
class IndexTree {
public:
	IndexTree();
	//TO DO: constructor with the database reference as a parameter.
	virtual ~IndexTree();

private:
	TreeNode* root;
	LinkedList<LeafNode<T>> leaves;

};

template<class T>
IndexTree<T>::IndexTree() {
	// TODO Auto-generated constructor stub

}

template<class T>
IndexTree<T>::~IndexTree() {
	// TODO Auto-generated destructor stub
}

#endif /* INDEXTREE_H_ */
