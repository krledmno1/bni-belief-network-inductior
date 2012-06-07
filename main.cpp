/*
 * main.cpp
 *
 *  Created on: May 14, 2012
 *      Author: krle
 */

#include "Visualisation/Graph.h"
#include <gtkmm.h>

int main(int argc, char* argv[])
{
	srand((unsigned int) time(0));

	char* bla = strdup("/home/krle/workspace/BNIProject/bni-belief-network-inductior/data1.txt");
	DataTable d(bla);
	BayesNetwork b(&d);
	BayesNetwork* newNet = new BayesNetwork(&b);
	newNet->learnStructure(2);
	newNet->print();


	Gtk::Main kit(argc, argv);
	Gtk::Window win;
	win.set_title("Graph");
	win.set_default_size(500,500);
	Graph c(5,40,newNet);
	win.add(c);
	c.show();
	kit.run(win);

	delete bla;
	delete newNet;
	return 0;
}


