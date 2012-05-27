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
	virtual ~LeafNode();

private:
	HashTable<int, T> Nijk;
	int Nij; //aggregated Nijk over k

};

template<class T>
LeafNode<T>::LeafNode() {
	// TODO Auto-generated constructor stub

}



template<class T>
LeafNode<T>::~LeafNode() {
	// TODO Auto-generated destructor stub
}
#endif /* LEAFNODE1_H_ */
