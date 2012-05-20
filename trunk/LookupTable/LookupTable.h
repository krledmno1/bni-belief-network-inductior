/*
 * LookupTable.h
 *
 *  Created on: May 20, 2012
 *      Author: krle
 */

#ifndef LOOKUPTABLE_H_
#define LOOKUPTABLE_H_

class LookupTable {
public:
	LookupTable();
	LookupTable(int size);
	double lookup(int index);
	virtual ~LookupTable();

private:
	int size;
	double* lookuptable;

};



#endif /* LOOKUPTABLE_H_ */
