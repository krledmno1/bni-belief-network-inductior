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
	BranchNode(Variable* correspondant);
	void print();

	virtual ~BranchNode();

	TreeNode<T>* branchingNodes;


};

template<class T>
void BranchNode<T>::print()
{
	//prints node for debugging purposes
	cout << "Branch node for variable " << this->var->name;
	cout << "\n     Children:";
	for(int i = 0; i<this->var->getNumValues();i++)
	{
		cout << "\n               value:" << this->var->getValueName(i) << "  child: " << branchingNodes[i]->print();
	}


}


template<class T>
BranchNode<T>::BranchNode(Variable* corr)
{
	//initializes the var reference and array of pointers
	this->TreeNode(corr);
	branchingNodes = new TreeNode<T>[corr->getNumValues()];
}

template<class T>
BranchNode<T>::BranchNode() {
//unused constructor
}
template<class T>
BranchNode<T>::~BranchNode() {

	//destroys the node, array of pointers and calls destructors of all child nodes
	if(branchingNodes!=NULL)
	{
		for(int i= 0;i<this->var->getNumValues(); i++)
		{
			if(branchingNodes[i]!=NULL)
				delete branchingNodes[i];
		}
		delete [] branchingNodes;
	}
}



#endif /* BRANCHNODE_H_ */
