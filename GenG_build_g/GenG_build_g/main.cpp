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
Graph supp_graph;

void main(int argc, const char* argv[])
{
	string dir = "G:/Main/VS17/Data/Ecoli_graph/";
	string argv1 = dir + "Ecoli_A39.fasta";
	string argv2 = dir + "Ecoli_K12.fasta";
	string argv3 = dir + "out";
	string argv4 = dir + "mafft/";
	string argv5 = dir + "out_graph";


	
	VariationData VD1;
	string ref1 = loadfasta(argv1);
	string ref2 = loadfasta(argv2);
	VD1.load(argv3);
	vector<tuple<int, int, string>> VCF = VD1.getVCF(ref1, ref2, argv4);

	main_graph.LoadReference(ref1);
	main_graph.LoadVariance(0, 1, VCF);

	main_graph.Save(argv5 + ".gngg");


	
	main_graph.BuildIndex(5, 10);
	main_graph.SaveIndex(argv5 + ".gngidx");

	//main_graph.Clear();

	
	//supp_graph.Load(argv5 + ".gngg");
	//supp_graph.LoadIndex(argv5 + ".gngidx");
}