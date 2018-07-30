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
#include <Interface.h>
#include <thread>
#include <future>

using namespace std;

Graph main_graph;

int func(int i, int j)
{
	cout << i << endl;
	return (i + j);
}

class testclass
{
public:
	testclass() {};
	int func(int i, int j)
	{
		//cout << i << endl;
		return (i + j);
	}
};
void main(int argc, const char* argv[])
{
/*	string argv5 = argv[1];

	main_graph.Load(argv5 + ".gngg");
	main_graph.LoadIndex(argv5 + ".gngidx");
	main_graph.BubbleIndexBuildTest();

	string testfile = argv[2];
	string logfile = argv[3];

	SRAalign(main_graph, testfile, logfile);
	*/

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
	main_graph.BubbleIndexBuildTest();

//	SRAalign(main_graph, argv5 + ".fasta", argv5 + ".logfasta", 5);
	main_graph.SFinder("NNNNNNNNNNNNNNNNNNNNNNNNNNN");

//	main_graph.NotSimpleFinderSNAP(ref1.substr(50, 2000));

/*	string read = "AAGTTTGTGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG";
	main_graph.SFinder(read);
	string read2 = reverse(read);
	main_graph.SFinder(read2);
	system("pause");
	
*/
	/*Graph Graph1;

	Node n1("aaaaaaaaaaaaaaaaaa", 0);
	Node n2("cccccccccccccccccc", 2);    Node n4("ttttttttttttttt", 4);
	Node n3("ggggggggggggggg", 3); 

	Node n_lst("x", -1);

	Graph1.Body[0] = n1;
	Graph1.Body[2] = n2;
	Graph1.Body[3] = n3;
	Graph1.Body[4] = n4;
	Graph1.Body[-1] = n_lst;

	Graph1.Link(&Graph1.Body[0], &Graph1.Body[2]);
	Graph1.Link(&Graph1.Body[0], &Graph1.Body[4]);

	Graph1.Link(&Graph1.Body[2], &Graph1.Body[3]);
	Graph1.Link(&Graph1.Body[4], &Graph1.Body[3]);

	Graph1.Link(&Graph1.Body[3], &Graph1.Body[-1]);

	Graph1.BuildIndex(5, 4);
	Graph1.BubbleIndexBuild();
	string read1 = "aaaaaaccccccccccccccccccggggggggggg";
	string read2 = "aaaaaacccccccccNccccccccggggggggggg";
	Graph1.SFinder(read2);



	
//	SRAalign(main_graph, argv5 + ".fasta", argv5 + ".logfasta", 5);
	

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

}
