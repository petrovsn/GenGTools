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
#include <Graph.h>
#include <VarReader.h>
#include <Fasta.h>

using namespace std;

int SRAalign(Graph &VG, string path, string output)
{
	int count = 0;
	int total = 0;
	int failed_r = 0;
	int failed_s = 0;

	string buf;
	ifstream fin(path);
	while (!fin.eof())
	{

		getline(fin, buf);
		getline(fin, buf);
		total++;
		int res = VG.SFinder(buf);
		if (res != 0)
		{
			buf = reverse(buf);
			res = VG.SFinder(buf);
			if (res != 0)
			{
				count++;
			}
		}
		else
		{
			count++;
		}

	}
	fin.close();

	ofstream f_out;

	f_out.open(output);
	f_out << "Total:" << total << '\n';
	f_out << "Aligned:" << count << '\n';
	f_out << "Proportion:" << (float)(count / total) << '\n';
	f_out.close();

}