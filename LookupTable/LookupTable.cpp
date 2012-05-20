/*
 * LookupTable.cpp
 *
 *  Created on: May 20, 2012
 *      Author: krle
 */

#include "LookupTable.h"
#include "math.h"

LookupTable::LookupTable() {

}

LookupTable::LookupTable(int size) {
	//size should be m+r, since the table starts from 0 and goes to m+r-1

	this->lookuptable = new double[size];
	double temp = 1;
	for(int i = 0; i<size; i++)
	{
		this->lookuptable[i] = log(temp);
		temp = temp*(i+1);
	}

}

double LookupTable::lookup(int index)
{
	if(index<size)
		return this->lookuptable[index];
	else
		return 0;
}


LookupTable::~LookupTable() {

	delete [] lookuptable;
}


