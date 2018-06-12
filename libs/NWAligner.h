#pragma once
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

class NWNode
{
public:
	string ref;
	int ID;    //id == -1 - final node of thread
	bool NaNbp;
	bool End;
	map<int, NWNode*> Prev;
	map<int, NWNode*> Next;
	vector<vector<int>> NWmatrix;

	
};

class NWAligner
{
public:
	NWAligner() {};

	map<int, NWNode> Body;

	add



};