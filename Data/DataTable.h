/*
 * DataTable.h
 *
 * Created on: May 21, 2012
 *
 */

static class DataTable {
public:
	static int get_size();
	int* get_case(int i);
	int get_num_vars();
private:
	int table[][];
	void populate_table();
};
