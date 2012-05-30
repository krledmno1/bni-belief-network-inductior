/*
 * main.cpp
 *
 *  Created on: May 14, 2012
 *      Author: krle
 */
#include "./BayesNetwork/BayesNetwork.h"
#include "./IndexTree/BranchNode.h"
#include "./IndexTree/LeafNode.h"
#include "./IndexTree/TreeNode.h"
#include "./BayesNetwork/CPT.h"
#include "./Data/DataTable.h"


int main(int argc, char* argv[])
{


	char* bla = strdup("/home/krle/workspace/BNIProject/bni-belief-network-inductior/data.txt");
	DataTable d(bla);




	delete bla;
	return 0;
}

void read(char* filePath) {
	if (strlen(filePath) > 0)
	{
		cout << "Reading file: " << filePath << "\n";

		int numVars = 0;
		int numCases = 0;


		int currSepIndex;
		int oldSepIndex;
		string line;
		ifstream file;
		file.open (filePath);

		if(file.is_open())
		{
			// Reading the first line with the names of the variables
			getline(file, line);

			// Finds the number of variables
			currSepIndex = -1;
			do {
				currSepIndex = line.find(",", currSepIndex + 1);
				numVars++;
			} while(currSepIndex != string::npos);

			cout << "Found " << numVars << " variables (" << line << ")\n";

			// Reads the names and creates the variables (all but the last one)
			oldSepIndex = 0;
			currSepIndex = 0;
			string* varName;
			int varIndex = 0;
			while (currSepIndex != string::npos) {

				currSepIndex = line.find_first_of(",", oldSepIndex);

				varName = new string(line.substr(oldSepIndex, (currSepIndex != string::npos ? currSepIndex : line.size()) - oldSepIndex));

				oldSepIndex = max(oldSepIndex, currSepIndex + 1);

			//	variables[varIndex] = new Variable(varName);
				varIndex++;
				cout << *varName << "\n";
			}

			numCases = 0;
			while(!file.eof()) // For each line
			{
				getline(file, line);
				numCases++;
				cout << "\nCase " << numCases;

			}

			//create table
	//		table = new int*[numCases];
			for(int i = 0; i < numCases; i++) {
	//			table[i] = new int[numVars]

			}

			file.seekg(0,ios_base::beg);	//go to the beggining
			getline(file,line);				//skip a line

			int i=0;
			int j = 0;
			oldSepIndex = 0;
			currSepIndex = 0;
			int valueId;
			while(!file.eof()) // For each line
			{
				cout << "\nCase " << i + 1 << ": ";
				getline(file, line);

				j = 0;
				oldSepIndex = 0;
				currSepIndex = 0;

				while (currSepIndex != string::npos){ // For each value on the line

					// Read the value
					currSepIndex = line.find_first_of(",", oldSepIndex);
					string* valueName = new string(line, oldSepIndex, (currSepIndex != string::npos ? currSepIndex : line.size()) - oldSepIndex);
					oldSepIndex = currSepIndex + 1;

					//valueId = variables[j]->addValue(valueName); // Add it to the matching variable
					//table[i][j] = valueId;
					j++;

					cout << valueId << "-" << *valueName << "  ";
				}


				i++;

				cout << "\n";
			}



		}
		else
		{
			//error
		}
	}
	else
	{
		//error
	}

}


