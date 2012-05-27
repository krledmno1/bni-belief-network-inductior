/*
 * IndexTree.h
 *
 *  Created on: May 20, 2012
 *      Author: krle
 */

#ifndef INDEXTREE_H_
#define INDEXTREE_H_

#include "../Utilities/LinkedList/LinkedList.h"
#include "BranchNode.h"
#include "LeafNode.h"
#include "../Data/DataTable.h"



template<class T>
class IndexTree {
public:
	IndexTree();
	IndexTree(DataTable* table, Variable* target);
	virtual ~IndexTree();
	T getProbability(/* list of parent values */);
	LinkedList<LeafNode<T> >* getNijks();

private:
	TreeNode<T>* root;
	LinkedList<LeafNode<T> > leaves;

	void constructTree(DataTable* table, Variable* target);

};

template<class T>
IndexTree<T>::IndexTree() {
	// TODO Auto-generated constructor stub

}

template<class T>
IndexTree<T>::IndexTree(DataTable* table, Variable* target)
{
	constructTree(table, target);
}


template<class T>
void IndexTree<T>::constructTree(DataTable* table, Variable* target)
{

}

template<class T>
IndexTree<T>::~IndexTree() {
	// TODO Auto-generated destructor stub
}

#endif /* INDEXTREE_H_ */
