/*
 * ValuesMap.h
 *
 * Created on: May 21, 2012
 *      Author: Denis
 */

class ValuesMap {
private:
	static LinkedList<char*> table[];
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
