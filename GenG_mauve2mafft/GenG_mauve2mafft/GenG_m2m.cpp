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

using namespace std;

vector<string> split(string str, char delimiter)
{
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;
	while (getline(ss, tok, delimiter))
	{
		internal.push_back(tok);
	}

	return internal;
}

string loadfasta(string path)
{
	string buf;
	ifstream fin(path);

	string head;
	char c;
	getline(fin, head);
	while (!fin.eof())
	{
		c = tolower(fin.get());
		if ((c == 'a') || (c == 'c') || (c == 'g') || (c == 't')) buf += c;
	}
	//cout << head << '\t' << buf.size() << '\t';
	return buf;
}

struct Block
{
	int coord1, coord2;
	char dir;
	void reset()
	{
		coord1 = -1;
		coord2 = -1;
		dir = ' ';
	}
};

void ExtractBlocks(string ref1, string ref2, string mauve_file_path, string out_file_name)
{
	vector<pair<Block, Block>> data;
	ofstream f_out;
	ifstream fin(mauve_file_path);
	string buf;

	int state = 0;

	Block bl1;
	Block bl2;
	bl1.reset();
	bl2.reset();

	do
	{
		getline(fin, buf);
		switch (buf[0])
		{
		case '>':
		{
			vector<string> tmp1 = split(buf, ' ');
			vector<string> tmp2 = split(tmp1[1], ':');
			vector<string> tmp3 = split(tmp2[1], '-');

			if (state == 0)
			{
				bl1.dir = tmp1[2][0];
				bl1.coord1 = stoi(tmp3[0]) - 1;
				bl1.coord2 = stoi(tmp3[1]) - 1;
				state = 1;
			}
			else
			{
				bl2.dir = tmp1[2][0];
				bl2.coord1 = stoi(tmp3[0]) - 1;
				bl2.coord2 = stoi(tmp3[1]) - 1;
			}
			break;
		}
		case '=':
		{
			if (bl1.dir == bl2.dir)
			{
				data.push_back(pair<Block, Block>(bl1, bl2));
			}
			bl1.reset();
			bl2.reset();
			state = 0;
			break;
		}
		case '#':
			break;
		default:
			break;
		}
	} while (!fin.eof());


	for (int i = 0; i < data.size(); i++)
	{
		f_out.open(out_file_name + to_string(i) +".fasta");
		f_out << ">r1" << endl;
		f_out << ref1.substr(data[i].first.coord1, data[i].first.coord2 - data[i].first.coord1) << endl;
		f_out << ">r2" << endl;
		f_out << ref1.substr(data[i].second.coord1, data[i].second.coord2 - data[i].second.coord1) << endl;
		f_out.close();
	}
}


void main()
{
	string ref1 = loadfasta("C:/Users/Sergey/Desktop/mauve_test/Ecoli_A39.fasta");
	string ref2 = loadfasta("C:/Users/Sergey/Desktop/mauve_test/Ecoli_K12.fasta");

	string mauveout = "C:/Users/Sergey/Desktop/mauve_test/1";
	string mafftfiles = "C:/Users/Sergey/Desktop/mauve_test/block_";

	ExtractBlocks(ref1, ref2, mauveout, mafftfiles);
}