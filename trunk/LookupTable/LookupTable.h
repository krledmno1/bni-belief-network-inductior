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
	int size;				//size should be m+r
	double* lookuptable;	//the dynamic array of precomputed values log(i!) for i = 0..m+r-1

};



#endif /* LOOKUPTABLE_H_ */
