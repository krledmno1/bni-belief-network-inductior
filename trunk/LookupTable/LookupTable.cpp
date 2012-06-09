/*
 * LookupTable.cpp
 *
 *  Created on: May 20, 2012
 *      Author: krle
 */

#include "LookupTable.h"
#include "math.h"
#include <iostream>

using namespace std;

LookupTable::LookupTable() {

}

LookupTable::LookupTable(int size) {
	//size should be m+r, since the table starts from 0 and goes to m+r-1

	//create the array
	this->lookuptable = new double[size];

	//compute values
	lookuptable[0] = 0;
	for(int i = 1; i<size; i++)
	{
		this->lookuptable[i] = this->lookuptable[i-1]+ log(i);
	}

	this->size = size;
}

double LookupTable::lookup(int index)
{
	//queries the table. if out of bounds returns 0;
	if(index<size)
		return this->lookuptable[index];
	else
		return 0;
}


LookupTable::~LookupTable() {

	//deletes the array
	if(lookuptable!=NULL)
		delete [] lookuptable;
	lookuptable = NULL;
}


