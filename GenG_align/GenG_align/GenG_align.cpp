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
#include <Graph.h>
#include <VarReader.h>
#include <Fasta.h>

using namespace std;

Graph main_graph;

void main(int argc, const char* argv[])
{
	string dir = "G:/Main/VS17/Data/Ecoli_graph/";
	string argv1 = dir + "Ecoli_A39.fasta";
	string argv2 = dir + "Ecoli_K12.fasta";
	string argv3 = dir + "out";
	string argv4 = dir + "mafft/";
	string argv5 = dir + "out_graph";

	string ref1 = loadfasta(argv1);
	string ref2 = loadfasta(argv2);

//	main_graph.LoadReference(ref1);
//	main_graph.BuildIndex(5, 10);

	main_graph.Load(argv5 + ".gngg");
	main_graph.LoadIndex(argv5 + ".gngidx");


	main_graph.NotSimpleFinderSNAP(ref1.substr(50, 2000));

	main_graph.SFinder(ref2.substr(50, 2000));
	system("pause");
	

/*
	Graph Graph1;

	Node n1("aaaaaa", 1);
	Node n2("cccccc", 2);    Node n4("tttttt", 4);
	Node n3("gggggg", 3); 

	Node n_lst("x", -1);

	Graph1.Body[1] = n1;
	Graph1.Body[2] = n2;
	Graph1.Body[3] = n3;
	Graph1.Body[4] = n4;
	Graph1.Body[-1] = n_lst;

	Graph1.Link(&Graph1.Body[1], &Graph1.Body[2]);
	Graph1.Link(&Graph1.Body[1], &Graph1.Body[4]);

	Graph1.Link(&Graph1.Body[2], &Graph1.Body[3]);
	Graph1.Link(&Graph1.Body[4], &Graph1.Body[3]);

	Graph1.Link(&Graph1.Body[3], &Graph1.Body[-1]);

	Graph1.BuildIndex(5, 4);


	Graph1.SFinder("aaaaacctcccgggggg");
	


/*	map<int, string> m1;
	m1[0] = "a";
	m1[1] = "b";
	cout << m1[0] << endl;
	cout << m1[1] << endl;
	if (m1.find(31) == m1.end()) {
		cout << "not found";
	}
	else {
		// found
	}*/
	system("pause");


	
}
