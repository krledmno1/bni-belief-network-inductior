#include <ctime>
#include <cmath>
#include <cairomm/context.h>
#include <glibmm/main.h>
#include "Graph.h"
#include "Grid.h"

Graph::Graph(int numVars,int delta,BayesNetwork* net)
: m_line_width(1)
{

	network = net;
	num =network->getNumVars();

  signal_draw().connect(sigc::mem_fun(*this, &Graph::on_draw), false);



}

Graph::~Graph()
{
}

bool Graph::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	//height in tiles (one tile = delta*delta)
	int height = 2*num+2;

	//width in tiles (one tile = delta*delta)
	int width = (num-2)*(num-1)+4;

	//Allocate actual space
	Gtk::Allocation allocation = get_allocation();
	allocation.set_height(height*delta);
	allocation.set_width(width*delta);

	//set up the universal line width
	cr->set_line_width(m_line_width);

	//create white background
	cr->save();
	cr->set_source_rgba(1, 1, 1, 0.9);
	cr->paint();
	cr->restore();

	//create a grid
	Grid grid(delta,width,height,(Cairo::RefPtr<Cairo::Context>&)cr);

	//get longest path (returns list of var ids that belong
	//to the longest path, and by reference returns length of the path
	int length;
	int* nodes = getLongestPath(&length);		//todo!

  //draws longest path nodes and arcs between them
  for(int i=0;i<length;i++)
  {
	  int y = 1+i*2;
	  grid.drawNode(network->getVariables()[nodes[i]]->id, grid.centerX,y,(Cairo::RefPtr<Cairo::Context>&)cr);
	  network->getVariables()[nodes[i]]->yBox = y;
	  network->getVariables()[nodes[i]]->xBox = grid.centerX;
	  network->getVariables()[nodes[i]]->drawn = true;
	  if(i<length-1)
		  grid.drawArc(grid.centerX,y,grid.centerX,y+2,(Cairo::RefPtr<Cairo::Context>&)cr);

  }

  //draws other arcs between the longest path nodes
  for(int i=0;i<length-1;i++)
  {
	  for(Node<Variable>* node = network->getVariables()[nodes[i]]->children->start; node!=NULL; node = node->getNext())
	  {
		  if(node->getContent()!=network->getVariables()[nodes[i+1]])
		  {
			  grid.drawAngledArc(grid.centerX,1+i*2,grid.centerX+grid.arcIntentLevel,grid.centerX,1+i*2+2,(Cairo::RefPtr<Cairo::Context>&)cr);
			  grid.updateArcIndentLevel();
		  }
	  }
  }

  //draw the rest of the nodes
  for(int i = 0;i<num;i++)
  {
	  if(!network->getVariables()[i]->drawn)
	  {
		  if(!network->getVariables()[i]->parents->isEmpty())
		  {
			  int higestYBox=0;
			  for(Node<Variable>* node = network->getVariables()[i]->parents->start; node!=NULL; node = node->getNext())
			  {
				  if(node->getContent()->yBox>higestYBox)
				  {
					  higestYBox=node->getContent()->yBox;
				  }
			  }
			  network->getVariables()[i]->yBox=higestYBox+2;
			  network->getVariables()[i]->xBox=grid.centerX+grid.nodeIntentLevel;

			  grid.drawNode(network->getVariables()[i]->id, grid.centerX+grid.nodeIntentLevel,higestYBox+2,(Cairo::RefPtr<Cairo::Context>&)cr);
			  grid.updateNodeIndentLevel();

			  //all parent arcs
			  for(Node<Variable>* node = network->getVariables()[i]->parents->start; node!=NULL; node = node->getNext())
			  {
				  grid.drawArc(node->getContent()->xBox,node->getContent()->yBox,network->getVariables()[i]->xBox,network->getVariables()[i]->yBox,(Cairo::RefPtr<Cairo::Context>&)cr);
			  }
			  //children acts of drawn nodes
			  for(Node<Variable>* node = network->getVariables()[i]->children->start; node!=NULL; node = node->getNext())
			  {
				  if(node->getContent()->drawn)
					  grid.drawArc(network->getVariables()[i]->xBox,network->getVariables()[i]->yBox,node->getContent()->xBox,node->getContent()->yBox,(Cairo::RefPtr<Cairo::Context>&)cr);
			  }
		  }
		  else
		  {
			  /*if(!network->getVariables()[i]->children->isEmpty())
			  {
				  for(Node<Variable>* node = network->getVariables()[i]->children->start; node!=NULL; node = node->getNext())
				  {
					  if(node->getContent()->drawn)
					  {

					  }
					 else
					 {
					 	 :(((
					 }
				  }
			  }*/
		  }
	  }
  }

  grid.drawAngledArc(grid.centerX,1,grid.centerX+grid.arcIntentLevel,grid.centerX,5,(Cairo::RefPtr<Cairo::Context>&)cr);
  grid.updateArcIndentLevel();
  grid.drawAngledArc(grid.centerX,1,grid.centerX+grid.arcIntentLevel,grid.centerX,7,(Cairo::RefPtr<Cairo::Context>&)cr);

  grid.drawNode(6, grid.centerX+grid.nodeIntentLevel,3,(Cairo::RefPtr<Cairo::Context>&)cr);
  grid.drawArc(grid.centerX,1,grid.centerX+grid.nodeIntentLevel,3,(Cairo::RefPtr<Cairo::Context>&)cr);
  grid.updateNodeIndentLevel();
  grid.drawNode(7, grid.centerX+grid.nodeIntentLevel,3,(Cairo::RefPtr<Cairo::Context>&)cr);

  grid.drawArc(grid.centerX,1,grid.centerX+grid.nodeIntentLevel,3,(Cairo::RefPtr<Cairo::Context>&)cr);

  return true;
}

int* Graph::getLongestPath(int* length)
{
	int* len = new int[num];
	for(int i = 0;i<num;i++)
	{
		len[i] = -1;
	}

	Variable** from = new Variable*[num];
	for(int i = 0;i<num;i++)
	{
		from[i] = NULL;
	}

	Variable** bestFrom = new Variable*[num];
	for(int i = 0;i<num;i++)
	{
		bestFrom[i] = NULL;
	}

	*(length) = 0;
	int startNode =0;
	for(int i = 0;i<num;i++)
	{
		int someLength = exploreNode(network->getVariables()[i],len,from);
		if(someLength>*(length))
		{
			startNode = i;
			*(length)=someLength;
			for(int j = 0;j<num;j++)
			{
				bestFrom[j]=from[j];
			}
		}
	}



	int* result = new int[*(length)];
	int i = 1;
	result[0]=startNode;
	Variable* nextNode = bestFrom[startNode];
	while(nextNode!=NULL)
	{
		result[i++] = nextNode->id;
		nextNode = bestFrom[nextNode->id];
	}

	delete [] len;
	delete [] from;
	delete [] bestFrom;

	return result;
}


int Graph::exploreNode(Variable* var, int* len,Variable** from)
{
	if(len[var->id]==-1)
	{
		for(Node<Variable>* node = var->children->start; node!=NULL; node = node->getNext())
		{
			int p = exploreNode(node->getContent(),len,from);
			if(p>len[var->id])
			{
				len[var->id]=p;
				from[var->id] = node->getContent();
			}
		}
		return len[var->id]+1;
	}
	else
	{
		return len[var->id]+1;
	}
}

bool Graph::on_timeout()
{

    return true;
}
