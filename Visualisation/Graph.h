#ifndef GTKMM_EXAMPLE_CLOCK_H
#define GTKMM_EXAMPLE_CLOCK_H

#include <gtkmm/drawingarea.h>
#include "../BayesNetwork/BayesNetwork.h"


class Graph : public Gtk::DrawingArea
{
public:
	Graph(int numVars,int delta,BayesNetwork* net);
	virtual ~Graph();

protected:

	//Override default signal handler
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	bool on_timeout();

	int* getLongestPath(int* length);
	int exploreNode(Variable* var, int* len,Variable** from);


	double m_line_width;
	int delta;
	int num;
	BayesNetwork* network;

};

#endif // GTKMM_EXAMPLE_CLOCK_H
