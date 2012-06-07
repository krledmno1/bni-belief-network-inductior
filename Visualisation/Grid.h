/*
 * Grid.h
 *
 *  Created on: Jun 6, 2012
 *      Author: krle
 */

#ifndef GRID_H_
#define GRID_H_
#include <gtkmm.h>
#include <cairomm/context.h>
#include <glibmm/main.h>
#include <gtkmm/drawingarea.h>
#include <math.h>
#include <iostream>
#include <string.h>
#include <sstream>



typedef struct point{
	int x;
	int y;
} point;

class Grid {
public:
	int delta;
	int width;
	int height;
	int centerX;

	int nodeIntentLevel;
	bool nodeDirection;

	int arcIntentLevel;
	bool arcDirection;


	Grid(int delta, int width,int height,Cairo::RefPtr<Cairo::Context>& cr);
	virtual ~Grid();
	point sqCenter(int x, int y);
	point circlePoint(int x1, int y1, int x2, int y2);

	void drawNode(int ID, int x, int y,Cairo::RefPtr<Cairo::Context>& cr);
	void drawLine(int x1, int y1, int x2, int y2, bool start, bool fin,Cairo::RefPtr<Cairo::Context>& cr);

	void drawArc(int x1, int y1, int x2, int y2,Cairo::RefPtr<Cairo::Context>& cr);
	void drawAngledArc(int x1, int y1, int xIndent, int x3, int y3,Cairo::RefPtr<Cairo::Context>& cr);
	void drawArrow(int x1,int y1, int x2, int y2,Cairo::RefPtr<Cairo::Context>& cr);

	void updateNodeIndentLevel();
	void updateArcIndentLevel();

	bool validPoints(int x, int y);
	int distance(point p1,point p2);
};

#endif /* GRID_H_ */
