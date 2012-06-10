/*
 * main.cpp
 *
 *  Created on: May 14, 2012
 *      Author: krle
 *
 *  ====To enable openMP====
 *  1. Go to Project->Properties->C/C++ Build->Settings
 *  2. In the Tool Settings tab select the GCC C++ Compiler/Miscellaneous item
 *  3. Add "-fopenmp" (without quotes)
 *  4. In the Tool Settings tab select the GCC C++ Linker/Miscellaneous item
 *  5. Add "-fopenmp" (without quotes)
 *  ========================
 */

#include "Visualisation/Graph.h"
#include <gtkmm.h>

int numVars;
BayesNetwork* network;

/*
 * Returns the list (ids) of potential roots and throught the argument "numRoots"
 * returns the number of potential roots
 */
int* getRoots(int* numRoots)
{
	*numRoots = 0;
	for(int i = 0; i < numVars; i++)
	{
//		if(network->getVariables()[i]->parents->getSize() == 0)
		if(network->getVariables()[i]->parents2->size() == 0)
			(*numRoots)++;
	}

	int* rootIds = new int[*numRoots];
	int rootCounter = 0;
	for(int i = 0; i < numVars; i++)
	{
//		if(network->getVariables()[i]->parents->getSize() == 0)
		if(network->getVariables()[i]->parents2->size() == 0)
		{
			rootIds[rootCounter] = i;
			rootCounter++;

			if(rootCounter == *numRoots)
				break;
		}
	}

	return rootIds;
}

/*
 * Recursive function that returns the max height of the DAG rooted at the
 * node with id "rootId".
 *
 * "depth" is used internally in the function in order to know the depth of the current node.
 * (invoke with an address pointing to a memory location containt value 0)
 *
 * Through the "path" argument an array is returned whose elements are the the nodes along the longest path.
 */
int getSubgraphDepth(int rootId, int* depth, int** path)
{
	// Set the current depth and variable
	Variable* currNode = network->getVariables()[rootId];
	(*depth)++;

	// Is it a leaf node?
	int numChildren = currNode->children2->size();
	if(numChildren == 0)
	{
		// If so, create an array of size "depth" in order to be able to store all the nodes along the path
		*path = new int[*depth];
		// Decrease the "depth" before going to the parent
		(*depth)--;
		// Put the current node's id in the last position in the array
		(*path)[(*depth)] = currNode->id;

		// For getting the height
		return 1;
	}
	else
	{
		// If not..
		// "bestPath" is used to store the current longest path of the children
		int* bestPath = NULL;
		// "maxDepth" is used to store the length of the longest path
		int maxDepth = 0;

		// For each child...
		map<int, Variable*>::iterator it;
		for(it = currNode->children2->begin(); it != currNode->children2->end(); it++)
		{
			// Temporary variable used to get the child's path and compare it to the best one
			int* childPath;
			// Recursively invoke the method on the child
			int subgraphDepth = getSubgraphDepth(it->second->id, depth, &childPath);
			// If the child has a longer path than the current one update the "maxDepth" and "bestPath" values
			if(maxDepth < subgraphDepth)
			{
				maxDepth = subgraphDepth;

				if(bestPath != NULL)
					delete [] bestPath;
				bestPath = childPath;
			}
			else
			{
				delete [] childPath;
			}
		}
		// Decrease the value before returning to the parent
		(*depth)--;
		// Set the nodes id in the best path array
		bestPath[*depth] = currNode->id;
		// The argument that the parent will use to update it's best path
		*path = bestPath;

		// Counts the height
		return maxDepth + 1;
	}
}

/*
 * Depth-first search of the reachable nodes.
 */
void reachableNodes(Variable* var, LinkedList<int>* nodes)
{
	int* id = new int(var->id);
	nodes->addToBack(id);
	map<int, Variable*>::iterator it;
	for(it = var->children2->begin(); it != var->children2->end(); it++)
	{
		reachableNodes(it->second, nodes);
	}
}

/*
 * For each potential root node generates a reachability list and returns an
 * array of the lists.
 */
LinkedList<int>** getReachabilityLists(int* roots, int numRoots)
{
	LinkedList<int>** reachabilityLists = new LinkedList<int>* [numRoots];
	for(int i = 0; i < numRoots; i++)
	{
		cout << "DAG num: " << (i + 1) << "\n";
		reachabilityLists[i] = new LinkedList<int>();
		reachableNodes(network->getVariables()[roots[i]], reachabilityLists[i]);

		for(Node<int>* node = reachabilityLists[i]->start; node != NULL; node = node->getNext())
			cout << *(node->getContent()) << " ";
		cout << "\n";
	}

	return reachabilityLists;
}

/*
 * Checks if some of the potential roots belong to the same graph
 * and removes one if that's the case.
 */
LinkedList<int>** removeEquivalentGraphs(int* numRoots, LinkedList<int>** reachabilityLists)
{
	// Keeps track of the number of deleted nodes
	int numDeleted = 0;
	// For each list of reachable nodes
	for(int i = 0; i < *numRoots - 1; i++)
	{
		// Has a common node been found?
		bool found = false;
		// If it hasn't been deleted
		if(reachabilityLists[i] != NULL)
		{
			// For all the subsequent reachability lists
			for(int j = i + 1; j < *numRoots; j++)
			{
				// If they haven't been deleted
				if(reachabilityLists[j] != NULL)
				{
					// Compare all the values from both lists
					for(Node<int>* node1 = reachabilityLists[i]->start; node1 != NULL && !found; node1 = node1->getNext())
					{
						for(Node<int>* node2 = reachabilityLists[j]->start; node2 != NULL && !found; node2 = node2->getNext())
						{
							if(*(node2->getContent()) == *(node1->getContent()))
								found = true; // There was a match
						}
					}
					if(found) // If two common nodes exist
					{
						numDeleted++;
						found = false;

						// Delete the shorter list
						if(reachabilityLists[i]->getSize() >= reachabilityLists[j]->getSize())
						{
							cout << i << " and " << j << " are the same. Deleting " << j << "\n" << endl;
							delete reachabilityLists[j];
							reachabilityLists[j] = NULL;
						}
						else
						{
							cout << i << " and " << j << " are the same. Deleting " << i << "\n" << endl;
							delete reachabilityLists[i];
							reachabilityLists[i] = NULL;
							break; // Goes to the next item in the first list
						}
					}

				}
			}
		}
	}

	// Creates an array of true roots and their reachability lists
	int finalSize = *numRoots - numDeleted;
	LinkedList<int>** graphList = new LinkedList<int>* [finalSize];
	int j = 0;
	for(int i = 0; i < *numRoots && j < finalSize; i++)
	{
		if(reachabilityLists[i] != NULL)
		{
			graphList[j] = reachabilityLists[i];
			j++;
		}
	}
	*numRoots = finalSize;
	return graphList;
}

/*
 * Arguments:
 * argv[0] - program name
 * argv[1] - number of threads to use
 */
int main(int argc, char* argv[])
{
	srand((unsigned int) time(0));

	int numThreads = 1; // number of threads to use
	int u = 5; // max number of parents a node can have

	/*
	 * Checks if some arguments were passed.
	 * The arguments have to be of the following format:
	 * 		<name> <value>
	 */
	if(argc > 1 && argc % 2 == 1)
	{
		// Goes through the argument list
		for (int i = 1; i < argc; i += 2)
		{
			// Reads each argument and performs a specific task for each argument
			if(strcmp(argv[i], "-n_threads") == 0) // argument that specifies the number of threads
			{
				try
				{
					numThreads = atoi(argv[i + 1]);
				}
				catch (int e)
				{
					cout << "Expected the number of threads to use but read '" << argv[i+1] << "'!" << endl;
					return -1;
				}
			}
			else if(strcmp(argv[i], "-max_num_par") == 0) // argument that specifies the maximum number of parents for a node
			{
				try
				{
					u = atoi(argv[i + 1]);
				}
				catch (int e)
				{
					cout << "Expected the max number of parents but read '" << argv[i + 1] << "'!" << endl;
				}
			}
		}
	}


//	char* bla = strdup("/home/denis/Documents/PoliMi/AA/project/BNI2/data2.txt");
//	DataTable d(bla);
//	BayesNetwork b(&d);
//	BayesNetwork* newNet = new BayesNetwork(&b);
//	newNet->learnStructure(2, numThreads);
//	newNet->print();

	char* bla = strdup("/home/denis/Documents/PoliMi/AA/project/BNI2/structure2.txt");
	BayesNetwork* newNet = new BayesNetwork(bla,5);
//	newNet->learnStructure(2, numThreads);
	newNet->print();
	network = newNet;
	numVars = network->getNumVars();

	int numPotentialRoots = 0;
	int* potentialRoots = getRoots(&numPotentialRoots);
	LinkedList<int>** reachabilityLists = getReachabilityLists(potentialRoots, numPotentialRoots);
	LinkedList<int>** graphList = removeEquivalentGraphs(&numPotentialRoots, reachabilityLists);


	cout << "Drawing graphs\n" << endl;
	Gtk::Main kit(argc, argv);
	Gtk::Window** wins = new Gtk::Window* [numPotentialRoots];
	Graph** graphs = new Graph* [numPotentialRoots];
	for(int i = 0; i < numPotentialRoots; i++)
	{
		cout << "Graph for root #" << (i + 1) << "\n";
		int* longestPath = new int[graphList[i]->getSize()];
		int j = 0;
		for(Node<int>* node = graphList[i]->start; node != NULL; node = node->getNext())
		{
			longestPath[j] = *(node->getContent());
			cout << longestPath[j] << " ";
			j++;
		}
		cout << "\nCopied list into array\n" << endl;

		wins[i] = new Gtk::Window();
		wins[i]->set_title("Graph");
		wins[i]->set_default_size(500,500);
		graphs[i] = new Graph(40,newNet, longestPath, graphList[i]->getSize());

		cout << "Created graph\n" << endl;

		wins[i]->add(*(graphs[i]));
		(*(graphs[i])).show();
		kit.run(*(wins[i]));

		cout << "Created window\n" << endl;
	}

	delete bla;
	delete newNet;
	return 0;
}


