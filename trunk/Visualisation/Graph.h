#ifndef GTKMM_EXAMPLE_CLOCK_H
#define GTKMM_EXAMPLE_CLOCK_H

#include <gtkmm/drawingarea.h>
#include "../BayesNetwork/BayesNetwork.h"
#include <map>

class Graph : public Gtk::DrawingArea
{
public:
	Graph(int delta,BayesNetwork* net, int* nodes, int numNodes);
	virtual ~Graph();

protected:

	//Override default signal handler
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	bool on_timeout();

	int* getLongestPath(int* length);
	int exploreNode(Variable* var, int* len,Variable** from);


	double m_line_width;
	int delta;
	int numVars;
	int* nodes;
	int numNodes;
	BayesNetwork* network;

};

#endif // GTKMM_EXAMPLE_CLOCK_H
