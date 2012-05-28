/*
 * BranchNode.h
 *
 *  Created on: May 20, 2012
 *      Author: krle
 */

#ifndef BRANCHNODE_H_
#define BRANCHNODE_H_

#include "TreeNode.h"
#include "../Utilities/HashTable/HashTable.h"



template<class T>
class BranchNode: public TreeNode<T> {
public:
	BranchNode();
	BranchNode(int size);
	BranchNode(Variable* correspondant);

	virtual ~BranchNode();

	TreeNode<T>* branchingNodes;


};

template<class T>
BranchNode<T>::BranchNode(int size)
{
	branchingNodes = new TreeNode<T>[size];
}

template<class T>
BranchNode<T>::BranchNode(Variable* corr)
{
	this->TreeNode(corr);
	branchingNodes = new TreeNode<T>[corr->getNumValues()];
}

template<class T>
BranchNode<T>::BranchNode() {
		// TODO Auto-generated constructor stub

}
template<class T>
BranchNode<T>::~BranchNode() {
		// TODO Auto-generated destructor stub
	if(branchingNodes!=NULL)
	{
		for(int i= 0;i<this->var->getNumValues(); i++)
		{
			delete branchingNodes[i];
		}
		delete [] branchingNodes;
	}
}



#endif /* BRANCHNODE_H_ */
