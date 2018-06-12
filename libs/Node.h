#pragma once
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <cstdio>
#include <time.h>
#include <map>
#include <set>
#include <math.h>
#include <unordered_map>
#include <tuple>
#include <math.h>
#include <sstream>
#include <algorithm>
#include <GenerHash.h>

class Node
{
public:
	int ID;    //id == -1 - final node of thread
	string  str;
	bool NaNbp;
	bool End;
	map<int, Node*> Prev;
	map<int, Node*> Next;
	unordered_map<int, int> B; //threads 

	Node();

	Node(int id);

	Node(string str, int id);

	const Node operator = (Node& n);

	Node Split(int pos, int id);

	void Split(int pos, Node* n_pr);

	void Link(Node *node);
}; 
