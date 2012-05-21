/*
 * ValuesMap.h
 *
 * Created on: May 21, 2012
 *
 */

static class ValuesMap {
public:
	ValuesMap(int num_vars);
	~ValuesMap();
	int get_num_values(int node_id);
	void add_value(int node_id, char* value);
	char* get_value_name(int node_id, int value_id);
private:
	LinkedList<char*> table[];
};
