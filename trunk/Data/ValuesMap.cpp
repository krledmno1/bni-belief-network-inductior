/*
 * ValuesMap.cpp
 *
 *  Created on: May 22, 2012
 *      Author: Denis
 */

ValuesMap::setNumVars(int numVars) {
	ValuesMap::numVars = numVars;
}

ValuesMap* ValuesMap::get() {

}

ValuesMap::ValuesMap() {
	this.table = new LinkedList<char*>[numVars];

	for(int i = 0; i < numVars; i++) {
		table[i] = new LinkedList<char*>();
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
