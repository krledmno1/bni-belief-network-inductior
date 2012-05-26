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
	virtual ~BranchNode();

private:
	HashTable<int,TreeNode<T> > branchingNodes;

};


template<class T>
BranchNode<T>::BranchNode() {
		// TODO Auto-generated constructor stub

}
template<class T>
BranchNode<T>::~BranchNode() {
		// TODO Auto-generated destructor stub
}



#endif /* BRANCHNODE_H_ */
