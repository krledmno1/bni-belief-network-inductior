/*
 * TreeNode.h
 *
 *  Created on: May 20, 2012
 *      Author: krle
 */

#ifndef TREENODE_H_
#define TREENODE_H_

#include <map>
#include "../BayesNetwork/Variable.h"

template<class T>
class TreeNode {
public:
	TreeNode();
	TreeNode(Variable* var);

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


#endif /* TREENODE_H_ */
