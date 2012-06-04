/*
 * TreeNode.h
 *
 *  Created on: May 20, 2012
 *      Author: krle
 */

#ifndef TREENODE_H_
#define TREENODE_H_

#include <map>
#include <iostream>

#include "../Data/DataTable.h"
#include "../BayesNetwork/Variable.h"

using namespace std;

template<class T>
class TreeNode {
public:
	TreeNode();
	TreeNode(Variable* var);
	virtual void print();

	virtual ~TreeNode();

	Variable* var;
private:
	//reference to variable class
};


template<class T>
TreeNode<T>::TreeNode() {
	// TODO Auto-generated constructor stub

}
template<class T>
TreeNode<T>::TreeNode(Variable* var) {
	// TODO Auto-generated constructor stub
this->var = var;
}
template<class T>
TreeNode<T>::~TreeNode() {
	// TODO Auto-generated destructor stub
}


template<class T>
void TreeNode<T>::print()
{

}


#endif /* TREENODE_H_ */
