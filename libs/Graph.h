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
using namespace std;


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

	Node()
	{
		this->ID = -1;
		this->str = "";
		this->End = false;
		this->NaNbp = false;
	}

	Node(int id)
	{
		this->ID = id;
		this->str = "";
		this->End = false;
		this->NaNbp = false;
	}

	Node(string str, int id)
	{
		this->str = str;
		this->ID = id;
		this->End = false;
		if (str[0] == 'n') this->NaNbp = true;
		else this->NaNbp = false;
	}

	const Node operator = (Node& n)
	{
		this->ID = n.ID;
		this->str = n.str;

		this->B = n.B;
		this->Next = n.Next;
		this->Prev = n.Prev;

		return *this;
	}

	Node Split(int pos, int id)
	{
		string tmp = this->str.substr(pos, str.length() - pos);
		this->str = this->str.substr(0, pos);

		Node n_pr(tmp, id);

		(n_pr.Prev).insert(pair<int, Node*>(this->ID, this));



		for (auto t1 : this->Next)
		{
			t1.second->Prev.erase(this->ID);
			t1.second->Prev.insert(pair<int, Node*>(id, &n_pr));
			n_pr.Next.insert(t1);
		}



		Next.clear();
		Next.insert(pair<int, Node*>(id, &n_pr));

		return n_pr;
	}

	void Split(int pos, Node* n_pr)
	{
		string tmp = this->str.substr(pos, str.length() - pos);
		this->str = this->str.substr(0, pos);

		n_pr->str = tmp;
		n_pr->NaNbp = this->NaNbp;

		(n_pr->Prev).insert(pair<int, Node*>(this->ID, this));

		for (auto t1 : this->Next)
		{
			t1.second->Prev.erase(this->ID);
			t1.second->Prev.insert(pair<int, Node*>(n_pr->ID, n_pr));
			n_pr->Next.insert(t1);
		}

		Next.clear();
		Next.insert(pair<int, Node*>(n_pr->ID, n_pr));



		for (auto t2 : this->B)
		{
			n_pr->B.insert(t2);
			B[t2.first] = n_pr->ID;
		}

	}

	void Link(Node *node);
};

struct WArray
{
	unsigned long long int value[4];
	bool operator <(WArray &b)
	{
		return tie(this->value[0], this->value[1], this->value[2], this->value[3]) < tie(b.value[0], b.value[1], b.value[2], b.value[3]);
	}

	bool operator == (WArray &b)
	{
		return (((this->value[0]) == (b.value[0])) && ((this->value[1]) == (b.value[1])) && ((this->value[2]) == (b.value[2])) && ((this->value[3]) == (b.value[3])));
	}
	bool operator <(const WArray &b)
	{
		return tie(this->value[0], this->value[1], this->value[2], this->value[3]) < tie(b.value[0], b.value[1], b.value[2], b.value[3]);
	}

	bool operator == (const WArray &b)
	{
		return (((this->value[0]) == (b.value[0])) && ((this->value[1]) == (b.value[1])) && ((this->value[2]) == (b.value[2])) && ((this->value[3]) == (b.value[3])));
	}
};

class Graph
{

	map<unsigned long long, Node> Body;
	unordered_map<unsigned long long, vector<WArray>> hashtable;
	bool IdxBuilded;

	void Link(Node* N1, Node* N2)
	{
		(N1->Next).insert(pair<int, Node*>(N2->ID, N2));
		(N2->Prev).insert(pair<int, Node*>(N1->ID, N1));
	}
	pair<int, int> defNodePos(int ThreadId, int pos);
	pair<int, int> defNodePos(int ThreadId, int pos, Node curr_Node1, int curr_pos1);
	void LoadNodep1(ifstream &f_in);
	void LoadNodep2(ifstream &f_in);


	GenerHash GH;
	void CallNode(Node n1, GenerHash GH2);
	void GenPrimeTable(Node n1, int base, int len);
	void AddHash(int ID, int pos, int fID, int fpos, unsigned long long hash);

		

public:

	Graph() {};
	void LoadReference(string ref);

	
	void LoadVariance(int ThreadID1, int ThreadID2, vector<tuple<int, int, string>> VCF);
	
	void BuildIndex(int base, int len);

	void Save(string filename);
	void Load(string filename);

	void SaveIndex(string filename);
	void LoadIndex(string filename);

	void Clear();

};