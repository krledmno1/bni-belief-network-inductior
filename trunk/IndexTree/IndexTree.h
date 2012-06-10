/*
 * IndexTree.h
 *
 *  Created on: May 20, 2012
 *      Author: krle
 */

#ifndef INDEXTREE_H_
#define INDEXTREE_H_

#include "../IndexTree/BranchNode.h"
#include "LeafNode.h"
#include <omp.h>



template<class T>
class IndexTree {
public:

	IndexTree();										//not used
	IndexTree(Variable* target);						//this should be used by CPT and T=double
	IndexTree(DataTable* table, Variable* target);		//this should be used for conputing Nijk with T=int
	virtual ~IndexTree();
	T* getProbabilities(int* parentValues);				//returns probabilities for corrVar from CPT
	LinkedList<LeafNode<T> >* getNijks();				//returns linkedlist of Nijk-s
	void print();
	Variable* target;									//corresponding variable
private:
	TreeNode<T>* root;							//root of the tree
	LinkedList<LeafNode<T> > leaves;			//linked list of the leaves of the tree
	void constructTree(DataTable* table);		//used by 3rd constructor
	void constructTree();						//used by 2nd constructor
	void constructNode(BranchNode<T>* n,map<int, Variable*>::iterator it, int value);		//used by constructTree()


};



template<class T>
IndexTree<T>::IndexTree() {


}

template<class T>
IndexTree<T>::IndexTree(Variable* target) {
	root=NULL;
	this->target = target;
	constructTree();

}

template<class T>
IndexTree<T>::IndexTree(DataTable* table, Variable* target)
{
	root=NULL;
	this->target = target;
	constructTree(table);
}

template<class T>
void IndexTree<T>::print()
{
	cout << "\nPrinting index tree for " << *(target->name) << ":\n";
	if(root!=NULL)
		root->print();
}
template<class T>
LinkedList<LeafNode<T> >* IndexTree<T>::getNijks()
{
	return &(this->leaves);
}


template<class T>
T* IndexTree<T>:: getProbabilities(int* parentValues)
{
	//we start from the root
	//set n (current node in traversal) to root
	TreeNode<T>* n = root;

	//we consider all the parents of the node
	for(int i = 0; i < this->target->parents2->size();i++)
	{
		//we traverse from one branch node to the next by taking appropritate paths
		//depending on the values supplied in the parentValues
		n = ((BranchNode<T>*)n)->branchingNodes[parentValues[i]];
	}

	//when we exploit all the parents (the depth of the tree should be the same as the number of parents)
	//we reach the leaf node, and return the whole array of probabilities that the variable can take for each of its value
	return ((LeafNode<T>*)n)->Nijk;
}

template<class T>
void IndexTree<T>::constructTree()
{
	//see whether there are parents
	map<int, Variable*>::iterator it = target->parents2->begin();
	if(it != target->parents2->end())
	{
		//if there is at least one - create branching node
		root = new BranchNode<T>(it->second);

		//now for each value of that parent create a node (either branchin' if node->getNext != NULL or leaf otherwise)

		Variable* currentParent = it->second;
		it++;
		for(int i = 0; i < currentParent->getNumValues();i++)
		{
			constructNode((BranchNode<T>*)root,it,i);
		}
	}
	else
	{
		//there are no parents of target node, create leaf node immediately
		root = new LeafNode<T>(target);
	}
}

template<class T>
void IndexTree<T>::constructNode(BranchNode<T>* node,map<int, Variable*>::iterator it, int value)
{


	if(it != target->parents2->end())
	{
		//for the value "value" of the already constructed node create a new node
		node->branchingNodes[value] = new BranchNode<T>(it->second);

		//if there are more parents then construct branchin' node
		Variable* currentParent = it->second;
		it++;
		for(int i = 0; i < currentParent->getNumValues();i++)
				constructNode((BranchNode<T>*)node->branchingNodes[value],it,i);
	}
	else
	{
		//...if not create a leaf node - finally using the parameter "target"
		node->branchingNodes[value] = new LeafNode<T>(target);
	}

}

template<class T>
void IndexTree<T>::constructTree(DataTable* table)
{

	// this is the construction of the Index tree - implemented iteratively
	// first declare two aux variables - an accumulator that is not accumulating anything
	// but just keeps the value of the current parent that is considered and
	// the pointer to the newly created (or not) branching node that corresponds to that parent
	int acc;
	BranchNode<T>* n=NULL;
	omp_lock_t writelock;
	omp_init_lock(&writelock);

	#pragma omp parallel private(n, acc) num_threads(2)
	{
		n = NULL;
		#pragma omp for schedule(dynamic, 100)
		for(int j = 0; j < table->getNumCases(); j++)//now, for each case do:
		{

			//for each parent do (notice that if there are no parents this loop is skipped):
			map<int, Variable*>::iterator it;
			for(it = target->parents2->begin(); it != target->parents2->end(); it++)
			{

				//if we are considering the first parent
				if(it == target->parents2->begin())
				{
					//if the root is not constructed yet (in the first case, acctually)
					omp_set_lock(&writelock);
					if(root==NULL)
						root = new BranchNode<T>(it->second);
					omp_unset_lock(&writelock);

					//we take the value for first parent from the table and store it in acc
					//throughout this loop acc will correspond to the value of the previous parent wrt to "node"
					acc = table->getCase(j)[it->second->id];

					//we set the current branchin' node to the root (which represents the first parent)
					//throughout this loop n will correspond to the branching node of the previous parent wrt to "node"
					n = (BranchNode<T>*)root;
				}
				else
				{
					//if we are considering other parents
					omp_set_lock(&writelock);
					if(n->branchingNodes[acc]==NULL)
					{
						//if the branching node of the previous parent had never been expanded for its current value
						//we create new brancing node that should correspond to the current parent referenced by "node"
						n->branchingNodes[acc] = new BranchNode<T>(it->second);
					}
					omp_unset_lock(&writelock);
					//if it already exists just traverse it
					n=(BranchNode<T>*)n->branchingNodes[acc];

					//and query the table for the value of the current parent
					acc = table->getCase(j)[it->second->id];
				}
			}

			if(n==NULL)
			{
				//this means that there are no parents of the target node
				//we create only one leafnode at root
				omp_set_lock(&writelock);
				if(root==NULL)
				{
					root = new LeafNode<T>(target);
					this->leaves.addToBack((LeafNode<T>*)root);
				}
				omp_unset_lock(&writelock);

				//we increment the Nijk and Nij
				LeafNode<T>* l = (LeafNode<T>*)root;
				int caseVal = table->getCase(j)[target->id];

				omp_set_lock(&writelock);
				l->Nijk[caseVal]=l->Nijk[caseVal]+1;
				l->Nij=l->Nij+1;
				omp_unset_lock(&writelock);
			}
			else
			{
				//now in n we have the bottommost branching node
				//in acc we have the value of the parent corresponding to the branching node in n
				//if there is no leafnode in its branch n->branchingNodes[acc]
				omp_set_lock(&writelock);
				LeafNode<T>* l = (LeafNode<T>*)n->branchingNodes[acc];
				if(l==NULL)
				{
					//we create new leafnode
					l = new LeafNode<T>(target);
					n->branchingNodes[acc] = l;
					//and we link it to the the rest of the leaves
					this->leaves.addToBack(l);
				}
				omp_unset_lock(&writelock);

				//we increment the Nijk and Nij
				int caseVal = table->getCase(j)[target->id];

				omp_set_lock(&writelock);
				l->Nijk[caseVal]=l->Nijk[caseVal]+1;
				l->Nij=l->Nij+1;
				omp_unset_lock(&writelock);
			}
		}
	}
}

template<class T>
IndexTree<T>::~IndexTree() {

	//calls destructior of the branch or leaf node (TreeNode constructor is virtual)
	delete root;
}



#endif /* INDEXTREE_H_ */
