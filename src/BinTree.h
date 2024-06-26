#ifndef BINTREE_H
#define BINTREE_H
#include<stdio.h>
#include<vector>
#include<iostream>
#include <unordered_map>
using namespace std;

class BinTree
{
	public:
	
	BinTree(int n):numNodes(0){Create(n);}
	//~BinTree();
	//return childrens of a node with some ID
	std::vector<int> GetChildren(int ID);

	std::vector<int> GetTreeDesc();
	std::vector<vector<int>> hsslevel();
	int GetNumNodes();

	vector<int> tr; 
	//stores children of all the parent
	std::unordered_map<int, vector<int>> ch;
	//stores all the leaf IDs. The IDs start from 1 and NOT 0.
	std::vector<int> leaves;
	int numNodes; //number of nodes in the full binary tree.
	int numLevels; //number of levels in full binary tree
	std::vector<vector<int>>nodeAtLvl;
	//helper functions to create the tree
	void Create(int n);
};

#endif
