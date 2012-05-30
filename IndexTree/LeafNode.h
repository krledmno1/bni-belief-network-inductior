/*
 * LeafNode1.h
 *
 *  Created on: May 23, 2012
 *      Author: krle
 */

#ifndef LEAFNODE1_H_
#define LEAFNODE1_H_

#include "TreeNode.h"


template<class T>
class LeafNode : public TreeNode<T> {
public:
	LeafNode();
	LeafNode(Variable* var);
	void print();

	virtual ~LeafNode();

	T* Nijk;	//counters in case of index tree and probabilities in case of CPTs
	int Nij;	//aggregated Nijk over k

};

template<class T>
void LeafNode<T>::print()
{
	//prints the node for debugging purposes
	cout << "Leaf node for variable " << *(this->var->name);
	cout << "\n     Counts: ";
	for(int i = 0; i<this->var->getNumValues();i++)
	{
		cout << "\n               value:" << *(this->var->getValueName(i));
		cout << "  count: " << Nijk[i];
	}
	cout << "\n     Total: " << Nij;
}





template<class T>
LeafNode<T>::LeafNode(Variable* var)
:TreeNode<T>(var)
{
	//initializes reference to corresponding var and the array of values
	Nijk = new T[var->getNumValues()];
	for(int i = 0; i<var->getNumValues(); i++)
	{
		Nijk[i] = 0;
	}
	Nij = 0;
}

template<class T>
LeafNode<T>::LeafNode() {
	//unused constructor
}



template<class T>
LeafNode<T>::~LeafNode() {
	if(Nijk!=NULL)
		delete [] Nijk;
}
#endif /* LEAFNODE1_H_ */
