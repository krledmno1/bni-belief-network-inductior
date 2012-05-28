Readme


Summary of tasks:

1. Classes:
	- DataTable
		data: matrix of values
		access: getters and setters given indexes
		generation: random, from a file, sampling

	- BayesNetwork
		data: list of variables
		access: by var id
		generation: from a file
		
	- Variables
		static: enimeration
		data: name, id, parentlist, childlist, CPT
		access: getters & setters
		generation: name, CPT
	-CPT
		data: intextree<double>
		access: given values of parents returns list of probability values, set values given the values of the parents
		generation: from a file or random
		
	- IndexTree
		data: root, corresponding variable, list of leaves
		access: get values given parent values (for CPTs), gets all leaf values using LList (for Nijk-s)
		generation: given a datatable (for Nijk-s) or see CPTs

	- TreeNode
		data: corresponding variable 
	- BranchNode
		data: list of references to treenodes
	- LeafNode
		data: list of Nijk-s (or probabilities)
	- LookupTable
		data: lookuptable for log(i!), i=1..m+r-1
	- Utilities
		- LinkedList
		- HashTable

2. Methods of input
	- datatable
		- from a file
		- random
		- bayesnetwork + CPTs + sampling
			- bayesnetwork from a file + cpt from a file
			- bayesnetwork from a file + cpt random
3. Algorithm for infering the structure
	- precomputed lookup tables
	- index tree computation
	- main algorithms computation
	- paralelization of some parts

4. Visualization
	- given a bayes network object create the picture
	- given list of variables and cpts print probabilities
	- given datatable, print it

5. Analysis
	- asymtotic analysis + inferring constants
	- benchmarking (function: number of variables -> execution time) 

6. Correctness
	- algorithm is heuristic but we can infer how precise it is by samping the datatable and then creating bayes network
	- using alarm example

7. Additional
	- given that the main verison of the algoritham is working we can try to add other versions of heuristic function

8. Presentation
	- create a powerpoint presentation of the work
		- about the problem
		- about the algorithm
		- analysis of the algoritham
		- benchmarking 
		- correctness
		- demo?


