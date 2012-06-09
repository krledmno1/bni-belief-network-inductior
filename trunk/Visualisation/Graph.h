#ifndef GTKMM_EXAMPLE_CLOCK_H
#define GTKMM_EXAMPLE_CLOCK_H

#include <gtkmm/drawingarea.h>
#include "../BayesNetwork/BayesNetwork.h"
#include <map>

class Graph : public Gtk::DrawingArea
{
public:
	Graph(int delta,BayesNetwork* net);
	virtual ~Graph();

protected:

	//Override default signal handler
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	bool on_timeout();

	int* getLongestPath(int* length);
	int getSubgraphDepth(int rootId, int* depth, int* path);
	int* getRoots(int* numRoots);
	int exploreNode(Variable* var, int* len,Variable** from);


	double m_line_width;
	int delta;
	int numVars;
	BayesNetwork* network;

};

#endif // GTKMM_EXAMPLE_CLOCK_H
