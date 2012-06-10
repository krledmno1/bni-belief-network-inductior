#include <ctime>
#include <cmath>
#include <cairomm/context.h>
#include <glibmm/main.h>
#include "Graph.h"
#include "Grid.h"

Graph::Graph(int delta,BayesNetwork* net, int* nodes, int numNodes)
: m_line_width(1)
{
	this->delta = delta;
	network = net;
	numVars =network->getNumVars();
	this->nodes = nodes;
	this->numNodes = numNodes;

	signal_draw().connect(sigc::mem_fun(*this, &Graph::on_draw), false);
}

Graph::~Graph()
{
}

bool Graph::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	//height in tiles (one tile = delta*delta)
	int height = 2*numVars+2;

	//width in tiles (one tile = delta*delta)
	int width = (numVars-2)*(numVars-1)+4;

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
	int length = numNodes;
//	int* nodes = getLongestPath(&length);		//todo!


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
	  std::map<int, Variable*>::iterator it;
	  for(it = network->getVariables()[nodes[i]]->children2->begin(); it != network->getVariables()[nodes[i]]->children2->end(); it++)
	  {
		  if(it->second->drawn == true && it->second != network->getVariables()[nodes[i+1]])
		  {
			  grid.drawAngledArc(grid.centerX,1+i*2,grid.centerX+grid.arcIntentLevel,grid.centerX,it->second->yBox,(Cairo::RefPtr<Cairo::Context>&)cr);
			  grid.updateArcIndentLevel();
		  }
	  }
  }

  //draw the rest of the nodes
  if(length<numVars)
  {

	  //first those who have drawn parents
	  for(int i = 0;i<numVars;i++)
	  {
		  if(!network->getVariables()[i]->drawn && !network->getVariables()[i]->parents2->empty())
		  {
			  int higestYBox=-1;
			  std::map<int, Variable*>::iterator it;
			  for(it = network->getVariables()[i]->parents2->begin(); it != network->getVariables()[i]->parents2->end(); it++)
			  {
				  if(it->second->yBox > higestYBox && it->second->drawn)
				  {
					  higestYBox=it->second->yBox;
				  }
			  }

			  if(higestYBox>-1)
			  {
				  //draw the node at the found level
				  network->getVariables()[i]->yBox=higestYBox+2;
				  network->getVariables()[i]->xBox=grid.centerX+grid.nodeIntentLevel;
				  network->getVariables()[i]->drawn = true;
				  grid.drawNode(network->getVariables()[i]->id, grid.centerX+grid.nodeIntentLevel,higestYBox+2,(Cairo::RefPtr<Cairo::Context>&)cr);
				  grid.updateNodeIndentLevel();

				  //draw the arcs from the already drawn parents
				  for(it = network->getVariables()[i]->parents2->begin(); it != network->getVariables()[i]->parents2->end(); it++)
				  {
					  if(it->second->drawn)
						  grid.drawArc(it->second->xBox,it->second->yBox,network->getVariables()[i]->xBox,network->getVariables()[i]->yBox,(Cairo::RefPtr<Cairo::Context>&)cr);
				  }

				  //draw the arcs to the already drawn children
				  for(it = network->getVariables()[i]->children2->begin(); it != network->getVariables()[i]->children2->end(); it++)
				  {
					  if(it->second->drawn)
						  grid.drawArc(network->getVariables()[i]->xBox,network->getVariables()[i]->yBox,it->second->xBox,it->second->yBox,(Cairo::RefPtr<Cairo::Context>&)cr);
				  }
			  }
		  }
	  }

	  //then those who have drawn children
	  for(int i = numVars-1;i>=0;i--)
	  {
//		  if(!network->getVariables()[i]->drawn && !network->getVariables()[i]->children->isEmpty())
		  if(!network->getVariables()[i]->drawn && !network->getVariables()[i]->children2->empty())
		  {
			  int lowestBox = 1+numVars*2;
			  std::map<int, Variable*>::iterator it;
			  for(it = network->getVariables()[i]->children2->begin(); it != network->getVariables()[i]->children2->end(); it++)
			  {
				  if(it->second->yBox < lowestBox && it->second->drawn)
				  {
					  lowestBox = it->second->yBox;
				  }
			  }
			  if(lowestBox<1+2*numVars)
			  {
				  //draw the node at the found level
				  network->getVariables()[i]->yBox=lowestBox-2;
				  network->getVariables()[i]->xBox=grid.centerX+grid.nodeIntentLevel;
				  network->getVariables()[i]->drawn = true;
				  grid.drawNode(network->getVariables()[i]->id, grid.centerX+grid.nodeIntentLevel,lowestBox-2,(Cairo::RefPtr<Cairo::Context>&)cr);
				  grid.updateNodeIndentLevel();


				  //draw the arcs from the already drawn parents
				  for(it = network->getVariables()[i]->parents2->begin(); it != network->getVariables()[i]->parents2->end(); it++)
				  {
					  if(it->second->drawn)
						  grid.drawArc(it->second->xBox,it->second->yBox,network->getVariables()[i]->xBox,network->getVariables()[i]->yBox,(Cairo::RefPtr<Cairo::Context>&)cr);
				  }

				  //draw the arcs to the already drawn children
				  for(it = network->getVariables()[i]->children2->begin(); it != network->getVariables()[i]->children2->end(); it++)
				  {
					  if(it->second->drawn)
						  grid.drawArc(network->getVariables()[i]->xBox,network->getVariables()[i]->yBox,it->second->xBox,it->second->yBox,(Cairo::RefPtr<Cairo::Context>&)cr);
				  }
			  }

		  }
	  }
  }

//  delete [] nodes;
  return true;
}

int* Graph::getLongestPath(int* length)
{
	int* len = new int[numVars];
	for(int i = 0;i<numVars;i++)
	{
		len[i] = -1;
	}

	Variable** from = new Variable*[numVars];
	for(int i = 0;i<numVars;i++)
	{
		from[i] = NULL;
	}

	Variable** bestFrom = new Variable*[numVars];
	for(int i = 0;i<numVars;i++)
	{
		bestFrom[i] = NULL;
	}

	*(length) = 0;
	int startNode =0;
	for(int i = 0;i<numVars;i++)
	{
		int someLength = exploreNode(network->getVariables()[i],len,from);
		someLength++;
		if(someLength>*(length))
		{
			startNode = i;
			*(length)=someLength;
			for(int j = 0;j<numVars;j++)
			{
				bestFrom[j]=from[j];
			}
		}

		for(int i = 0;i<numVars;i++)
		{
			len[i] = -1;
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
//		for(Node<Variable>* node = var->children->start; node!=NULL; node = node->getNext())
//		{
//			int p = exploreNode(node->getContent(),len,from);
//			if(p>len[var->id])
//			{
//				len[var->id]=p;
//				from[var->id] = node->getContent();
//			}
//		}
		std::map<int, Variable*>::iterator it;
		for(it = var->children2->begin(); it != var->children2->end(); it++)
		{
			int p = exploreNode(it->second,len,from);
			if(p>len[var->id])
			{
				len[var->id]=p;
				from[var->id] = it->second;
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

/*

	  //first those who have parents
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

		  }
	  }

	  //then those that don't, but have children
	  for(int i = num-1;i>=0;i--)
	  {
		  if(!network->getVariables()[i]->drawn)
		  {
			  if(!network->getVariables()[i]->children->isEmpty())
			  {
				  for(Node<Variable>* node = network->getVariables()[i]->children->start; node!=NULL; node = node->getNext())
				  {

				  }
			  }
		  }
	  }


 */
