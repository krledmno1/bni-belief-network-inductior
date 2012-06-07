/*
 * Grid.cpp
 *
 *  Created on: Jun 6, 2012
 *      Author: krle
 */


#include "Grid.h"

Grid::Grid(int delta, int width,int height,Cairo::RefPtr<Cairo::Context>& cr)
{
	if(delta>0 && width>0 && height>0)
	{
		this->delta = delta;
		this->width = width;
		this->height = height;
		centerX = width/2;

		nodeIntentLevel = 2;
		nodeDirection = true; //right

		arcIntentLevel = 1;
		arcDirection = false; //left

		cr->rectangle(0,0,width*delta,height*delta);
		cr->save();
		cr->set_source_rgba(0.8, 0.8, 0.8, 1);
		cr->fill_preserve();
		cr->restore();
		cr->stroke_preserve();
		cr->clip();
	}
}

point Grid::sqCenter(int x, int y)
{
	point p;
	p.x = 0;
	p.y = 0;
	if(validPoints(x,y))
	{
		p.x = x*delta + delta/2;
		p.y = y*delta + delta/2;
	}
	return p;

}

bool Grid::validPoints(int x, int y)
{
	if(x>=0 && y>=0 && x<width && y<height)
		return true;
	else return false;
}


point Grid::circlePoint(int x1, int y1, int x2, int y2)
{
	point returnPoint;
	returnPoint.x = 0;
	returnPoint.y = 0;

	if(validPoints(x1,y1) && validPoints(x2,y2))
	{
		point start = sqCenter(x1,y1);
		point end = sqCenter(x2,y2);

		int D = distance(start,end);
		returnPoint.x = start.x - ((start.x-end.x)*delta)/(2*D);
		returnPoint.y = start.y - ((start.y-end.y)*delta)/(2*D);


	}
	return returnPoint;

}

int Grid::distance(point p1,point p2)
{
	return round(sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)));

}

void Grid::drawNode(int ID, int x, int y,Cairo::RefPtr<Cairo::Context>& cr)
{
	if(validPoints(x,y))
	{
	point center = sqCenter(x,y);
	int radius = delta/2;

	cr->arc(center.x, center.y, radius, 0, 2 * M_PI);

	Pango::FontDescription font;
	font.set_family("Monospace");
	font.set_weight(Pango::WEIGHT_BOLD);
	Gtk::DrawingArea a;


	std::string Result;
	std::stringstream convert;
	convert << ID;
	Result = convert.str();

	Glib::RefPtr<Pango::Layout> layout = a.create_pango_layout(Result);
	layout->set_font_description(font);
	int text_width;
	int text_height;
	layout->get_pixel_size(text_width, text_height);
	cr->move_to(center.x+delta/2, center.y-delta/2);
	layout->show_in_cairo_context(cr);


	cr->save();
	cr->set_source_rgba(0.4, 0.4, 0.4, 1);
	cr->fill();

	cr->restore();

	}
}
void Grid::drawLine(int x1, int y1, int x2, int y2, bool start, bool fin,Cairo::RefPtr<Cairo::Context>& cr)
{
	if(validPoints(x1,y1)&&validPoints(x2,y2))
	{
		point first;
		point second;

		if(start)
		{
			first = circlePoint(x1,y1,x2,y2);
		}
		else
		{
			first = sqCenter(x1,y1);
		}
		if(fin)
		{
			second = circlePoint(x2,y2,x1,y1);
		}
		else
		{
			second = sqCenter(x2,y2);
		}




		cr->save();
		cr->set_source_rgba(0, 0, 0, 0.5);
		cr->set_line_cap(Cairo::LINE_CAP_SQUARE);
		cr->move_to(first.x,first.y);
		cr->line_to(second.x,second.y);
		cr->restore();
		cr->stroke();



	}
}

void Grid::drawArc(int x1, int y1, int x2, int y2,Cairo::RefPtr<Cairo::Context>& cr)
{
	drawLine(x1,y1,x2,y2,true,true,cr);
	drawArrow(x2,y2,x1,y1,cr);
}
void Grid::drawAngledArc(int x1, int y1, int xIndent, int x2, int y2,Cairo::RefPtr<Cairo::Context>& cr)
{
	drawLine(x1,y1,xIndent,y1+1,true,false,cr);
	drawLine(xIndent,y1+1,xIndent,y2-1, false, false,cr);
	drawLine(xIndent,y2-1,x2,y2,false,true,cr);
	drawArrow(x2,y2,xIndent,y2-1,cr);
}

void Grid::drawArrow(int x1,int y1, int x2, int y2,Cairo::RefPtr<Cairo::Context>& cr)
{
	point head = circlePoint(x1,y1,x2,y2);
	cr->arc(head.x, head.y, delta/10, 0, 2 * M_PI);
	cr->save();
	cr->set_source_rgba(0.1, 0.1, 0.1, 1);
	cr->fill();
	cr->restore();
	//cr->stroke_preserve();
	//cr->clip();
}

void Grid::updateNodeIndentLevel()
{
	if(nodeDirection)
	{
		nodeDirection=false;
		nodeIntentLevel=-nodeIntentLevel;
	}
	else
	{
		nodeDirection = true;
		nodeIntentLevel-=2;
		nodeIntentLevel=-nodeIntentLevel;
	}
}
void Grid::updateArcIndentLevel()
{

	if(arcDirection)
	{
			arcDirection=false;
			arcIntentLevel-=2;
			arcIntentLevel=-arcIntentLevel;
	}
	else
	{
			arcDirection = true;
			arcIntentLevel=-arcIntentLevel;
	}
}


Grid::~Grid() {
	// TODO Auto-generated destructor stub
}

