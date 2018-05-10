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
#include <Graph.h>

using namespace std;

void Node::Link(Node *N2)
{
	(this->Next).insert(pair<int, Node*>(N2->ID, N2));
	(N2->Prev).insert(pair<int, Node*>(this->ID, this));
}

void Graph::LoadReference(string ref)
{
	Node startNode(ref, 0);
	startNode.B.insert(pair<int, int>(0, -1));
	Body.insert(pair<int, Node>(0, startNode));
	Node endNode("x", -1);
	endNode.End = true;

	Body.insert(pair<int, Node>(-1, endNode));

	Body[-1].End = true;

	Link(&Body[0], &Body[-1]);
}

pair<int, int> Graph::defNodePos(int ThreadId, int pos)
{

	Node curr_Node = Body[0];
	int curr_len = curr_Node.str.length();
	int curr_pos = 0;

	int f_ID = 0;

	while (pos>curr_len)
	{
	//	curr_Node.Next[ThreadId]->print();
		//	system("pause");
		int tmp = curr_Node.Next[ThreadId]->ID;
		curr_Node = Body[tmp];
		curr_len += curr_Node.str.length();
	}

	return pair<int, int>(curr_Node.ID, curr_Node.str.length() - 1 - (curr_len - pos));
}

pair<int, int> Graph::defNodePos(int ThreadId, int pos, Node curr_Node1, int curr_pos1)
{
	int curr_ID = curr_Node1.ID;
	int curr_len = Body[curr_ID].str.length();
	int curr_pos = curr_pos1;

	do
	{
		if ((curr_pos<pos) && (pos <= curr_pos + curr_len))
		{
			return pair<int, int>(curr_ID, (pos - curr_pos));
		}
		else
		{
			curr_ID = Body[curr_ID].B[ThreadId];
			curr_pos += curr_len;
			curr_len = Body[curr_ID].str.length();
		}

	} while (!Body[curr_ID].End);

	return pair<int, int>(-1, -1);
}

void Graph::LoadVariance(int ThreadID1, int ThreadID2, vector<tuple<int, int, string>> VCF)
{
	//walk down the thread1, incepting variance node from position list.
	//ints in VCF should be sorted

	int curr_ID = 0;
	int curr_len = Body[curr_ID].str.length();
	int curr_pos = 0;

	int vcf_id = 0;

	bool check = false;
	int ID1 = 0;
	int ID2 = 0;
	do
	{
		int pos1 = get<0>(VCF[vcf_id]);
		int pos2 = get<1>(VCF[vcf_id]);
		string str = get<2>(VCF[vcf_id]);


		if ((curr_pos<pos1) && (pos1 <= curr_pos + curr_len))
		{

			if (pos1 == curr_pos + curr_len) //начало вариации со стыка нодов
			{
				ID1 = Body[curr_ID].ID;
			}
			else
			{
				ID1 = Body[curr_ID].ID;
				Node tmpNode1(Body.size());
				Body.insert(pair<int, Node>(tmpNode1.ID, tmpNode1));
				Body[curr_ID].Split(pos1 - curr_pos, &Body[tmpNode1.ID]);
			}


			pair<int, int> tmp_pair1 = defNodePos(ThreadID1, pos2, Body[curr_ID], curr_pos);

			if (Body[tmp_pair1.first].str.length() == tmp_pair1.second) //конец вариации на стык нодов
			{
				Node tmp = Body[tmp_pair1.first];
				ID2 = Body[tmp_pair1.first].B[ThreadID1];
			}
			else
			{
				Node tmpNode1(Body.size());
				ID2 = tmpNode1.ID;
				Body.insert(pair<int, Node>(tmpNode1.ID, tmpNode1));
				Body[tmp_pair1.first].Split(tmp_pair1.second, &Body[tmpNode1.ID]);
			}


			//	RecurcivePrint();
			Node incNode(str, Body.size());
			int incID = incNode.ID;

			Body.insert(pair<int, Node>(incID, incNode));
			Link(&Body[ID1], &Body[incID]);
			Link(&Body[incID], &Body[ID2]);
			Body[incID].B.insert(pair<int, int>(ThreadID2, ID2));
			Body[ID1].B.insert(pair<int, int>(ThreadID2, incID));

			check = false;

			if (vcf_id<VCF.size() - 1)
			{
				vcf_id++;
			}
		}
		else
		{
			if (check)
			{
				Body[curr_ID].B.insert(pair<int, int>(ThreadID2, Body[curr_ID].B[ThreadID1]));
			}
		}


		curr_pos += Body[curr_ID].str.length();
		curr_ID = Body[curr_ID].B[ThreadID1];
		curr_len = Body[curr_ID].str.length();
		check = (curr_ID == ID2);
	} while (Body[curr_ID].End != true);
}



void Graph::Save(string filename)
{
	ofstream f_out;
	f_out.open(filename+".gngg");
	for (auto n : Body)
	{

		f_out << n.second.ID << '.' << n.second.str << '.' << n.second.NaNbp << '.' << n.second.End << '.' << n.second.Next.size();
		for (auto n1 : n.second.Next)
		{
			f_out << '.' << n1.second->ID;
		}

		f_out << '.' << n.second.Prev.size();
		for (auto n1 : n.second.Prev)
		{
			f_out << '.' << n1.second->ID;
		}
		f_out << '.' << n.second.B.size();
		for (auto n1 : n.second.B)
		{
			f_out << '.'<< n1.first << '/' << n1.second;
		}
		f_out << '.' << endl;
	}
	f_out.close();
}


void Graph::Load(string filename)
{
	ifstream f_in;
	f_in.open(filename);
	do
	{
		LoadNodep1(f_in);
	}
	while (!f_in.eof());
	f_in.close();
	f_in.open(filename);
	do
	{
		LoadNodep2(f_in);
	} while (!f_in.eof());
	f_in.close();
}

void Graph::LoadNodep1(ifstream &f_in)
{
	string buf;
	getline(f_in, buf, '.');
	if (buf == "") return;
	int ID = stoi(buf);
	getline(f_in, buf, '.');
	string str = buf;
	getline(f_in, buf, '.');
	bool NaNbp = (buf == "1");
	getline(f_in, buf, '.');
	bool End = (buf == "1");
	
	Node nn(str, ID);
	nn.End = End;
	
	Body[nn.ID] = nn;

	getline(f_in, buf);
	return;
}

void Graph::LoadNodep2(ifstream &f_in)
{
	string buf;
	getline(f_in, buf, '.');
	if (buf == "") return;
	int ID = stoi(buf);
	getline(f_in, buf, '.');
	string str = buf;
	getline(f_in, buf, '.');
	bool NaNbp = (buf == "1");
	getline(f_in, buf, '.');
	bool End = (buf == "1");

	getline(f_in, buf, '.');
	int sz = stoi(buf);  //NEXT
	for (int i = 0; i < sz; i++)
	{
		getline(f_in, buf, '.');
		int lnk = stoi(buf);
		Body[ID].Next[lnk] = &(Body[lnk]);
	}
	getline(f_in, buf, '.');
	sz = stoi(buf); //PREV
	for (int i = 0; i < sz; i++)
	{
		getline(f_in, buf, '.');
		int lnk = stoi(buf);
		Body[ID].Prev[lnk] = &(Body[lnk]);
	}
	getline(f_in, buf, '.');
	sz = stoi(buf); //B
	for (int i = 0; i < sz; i++)
	{
		getline(f_in, buf, '/');
		int t1 = stoi(buf);
		getline(f_in, buf, '.');
		int n1 = stoi(buf);
		Body[ID].B.insert(pair<int, int>(t1, n1));
	}
	getline(f_in, buf);
}

void Graph::BuildIndex(int base, int len)
{
	GH.Init(base, len);
	for (auto t1 : Body)
	{
		if ((t1.second.ID != (-1)) && (!t1.second.NaNbp))
		{
			GenPrimeTable(t1.second, base, len);
		}
		else
		{
			cout << "try to hash -1 or NaNbp Node\n";
		}
	}
}


void Graph::GenPrimeTable(Node n1, int base, int len)
{

	GH.ReInit(n1.ID, n1.str);
	int pos = 0;
	unsigned long long res = 0;

	for (int i = 0; i<n1.str.length(); i++)
	{
		if (GH.Next(n1.str[i]) == 0)
		{
			AddHash(n1.ID, GH.pos, n1.ID, i, GH.currhash);
		}
		else
		{
			//	cout<<"oooops"<<endl;
		}
	}


	for (auto node : n1.Next)
	{
		CallNode(*(node.second), GH);
	}
}

void Graph::CallNode(Node n1, GenerHash GH2)
{
	if (n1.ID == -1) return;
	if (n1.NaNbp) return;
	unsigned long long res = 0;
	for (int i = 0; i<n1.str.length(); i++)
	{
		res = GH2.Next(n1.str[i]);
		if (res == -2)//pointer to position at the end of node. return;
		{
			return;
		}
		else
		{
			AddHash(GH2.id, GH2.pos, n1.ID, i, GH2.currhash);
		}
	}

	for (auto p1 : n1.Next)
	{
		CallNode(*(p1.second), GH2);
	}
}

void Graph::AddHash(int ID, int pos, int fID, int fpos, unsigned long long hash)
{
	WArray tmp;
	tmp.value[0] = ID;
	tmp.value[1] = pos;
	tmp.value[2] = fID;
	tmp.value[3] = fpos;

	try
	{
		hashtable[hash].push_back(tmp);
	}
	catch (...)
	{
		cout << "oooooops, exeption" << endl;
	}
}


void Graph::SaveIndex(string filename)
{
	ofstream f_out;
	f_out.open(filename);
	for (auto t : hashtable)
	{
		f_out << t.first << '.' << t.second.size();
		for (int i = 0; i < t.second.size(); i++)
		{
			for (int j = 0; j < 4; j++)
			{
				f_out << '.' << t.second[i].value[j];
			}
		}
		f_out << '.' << endl;
	}
	f_out.close();
}


void Graph::LoadIndex(string filename)
{
	hashtable.clear();
	ifstream f_in;
	f_in.open(filename);
	string buf;
	do
	{
		getline(f_in, buf, '.');
		if (buf == "") return;
		unsigned long long h1 = stoull(buf);
		getline(f_in, buf, '.');
		int sz = stoi(buf);
		WArray tmp1;
		vector<WArray> wr_tmp;
		hashtable[h1] = wr_tmp;
		for (int i = 0; i < sz; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				getline(f_in, buf, '.');
				tmp1.value[j] = stoi(buf);
			}
			hashtable[h1].push_back(tmp1);
		}
		getline(f_in, buf);
	} while (!f_in.eof());
}


void Graph::Clear()
{
	hashtable.clear();
	Body.clear();
}