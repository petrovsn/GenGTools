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


	main_graph.Load(argv5 + ".gngg");
	main_graph.LoadIndex(argv5 + ".gngidx");


	main_graph.NotSimpleFinderSNAP(ref1.substr(100, 1000));
	system("pause");
}
