/*
 * ValuesMap.h
 *
 * Created on: May 21, 2012
 *      Author: Denis
 */
#include <stdlib.h>

class ValuesMap {
private:
	static LinkedList<char> table[];
	static int numVars;
	static ValuesMap instance;
	ValuesMap(int numVars);
	ValuesMap();

public:
	static ValuesMap* get();
	static void setNumVars(int numVars);
	int getNumValues(int varId);
	void addValue(int varId, char* value);
	char* getValueName(int varId, int valueId);

};

void ValuesMap::setNumVars(int numVars) {
	ValuesMap::numVars = numVars;
}

ValuesMap* ValuesMap::get() {
	return NULL;
}

ValuesMap::ValuesMap() {
	this->table = new LinkedList<char>[numVars];

	for(int i = 0; i < numVars; i++) {
		table[i] = new LinkedList<char>();
	}
}

int ValuesMap::getNumValues(int varId) {
	return table[varId].getSize();
}

void ValuesMap::addValue(int varId, char* value) {
	table[varId].addToBack(value);
}

char* ValuesMap::getValueName(int varId, int valueId) {
	Node<char*>* node = table[varId].start;

	int i = 0;
	while(i < valueId, node != NULL) {
		node = node->getNext();
	}

	if(node != NULL) {
		return *(node->getContent());
	}

	return NULL;
}
