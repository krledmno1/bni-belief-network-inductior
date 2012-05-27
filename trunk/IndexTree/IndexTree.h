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
	T* getProbability(int* parentValues, Variable* var);
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
T* IndexTree<T>:: getProbability(int* parentValues, Variable* var)
{
	TreeNode<double> n = root;
	for(int i = 0 ; i<var->parents->getSize();i++)
	{
		n = ((BranchNode<double>*)n)->branchingNodes[parentValues[i]];
	}
	return ((LeafNode<double>*)n)->Nijk;
}


template<class T>
void IndexTree<T>::constructTree(DataTable* table, Variable* target)
{

	if(root==NULL&&target->parents->getSize()>0)
	{
		root = new BranchNode<int>(target->parents->start->getContent()->getNumValues());
	}

	int acc;
	BranchNode<int>* n;
	for(int j = 0; j<table->getNumCases(); j++)
	{

		for(Node<Variable>* node = target->parents->start;node!=target->parents->end;node=node->getNext())
		{

			if(node ==target->parents->start)
			{
				root = new BranchNode<int>(node->getContent()->getNumValues());
				acc = table->getCase(j)[node->getContent()->id];
				n = root;
			}
			else
			{
				if(n->branchingNodes[acc]==NULL)
				{
					n->branchingNodes[acc] = new BranchNode<int>(node->getContent()->getNumValues());
				}
				n=n->branchingNodes[acc];
				acc = table->getCase(j)[node->getContent()->id];
			}


		}
		if(n->branchingNodes[acc]==NULL)
		{
			n->branchingNodes[acc] = new LeafNode<int>(target->getNumValues());
			n = n->branchingNodes[acc];
			this->leaves.addToBack(n->branchingNodes[acc]);
		}
		((LeafNode<int>*)n)->Nijk[table->getCase(j)[target->id]]++;
		((LeafNode<int>*)n)->Nij++;
	}
}

template<class T>
IndexTree<T>::~IndexTree() {
	// TODO Auto-generated destructor stub
}

#endif /* INDEXTREE_H_ */
