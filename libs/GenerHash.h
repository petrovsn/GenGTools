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

using namespace std;
class GenerHash
{
public:
	vector<unsigned long long> p;
	int base;
	int len;

	unsigned long long currhash;
	int curr_len;

	string str;
	int id;
	int pos;
	int m_pos;

	bool formed;

	GenerHash()
	{

	}

	GenerHash(int base, int len)
	{
		this->base = base;
		this->len = len;
		for (int i = len - 1; i >= 0; i--)
		{
			p.push_back(pow(base, i));
		}
	}

	int ord(char c)
	{
		switch (c)
		{
		case 'a': return 1;
			break;
		case 'c': return 2;
			break;
		case 'g': return 3;
			break;
		case 't': return 4;
			break;
		case 'A': return 1;
			break;
		case 'C': return 2;
			break;
		case 'G': return 3;
			break;
		case 'T': return 4;
			break;
		default:
			cout << "cashe error. unknown simbol: " << c << endl;
			break;
		}

	}

	void Init(int base, int len)
	{
		this->str = "";
		this->curr_len = 0;
		this->currhash = 0;

		p.clear();

		this->base = base;
		this->len = len;
		for (int i = len - 1; i >= 0; i--)
		{
			p.push_back(pow(base, i));
		}
	}

	void ReInit(int Id, string str)
	{
		this->str = str;
		this->m_pos = str.length() - 1;

		this->curr_len = 0;
		this->currhash = 0;

		this->pos = 0;
		this->id = Id;
		this->formed = false;
	}

	int Inc(char c)
	{
		currhash += ord(c)*p[curr_len];
		curr_len++;
		if (curr_len == len)
		{
			formed = true;
			return 0;
		}
		return -1;
	}

	int Next(char c1)
	{
		unsigned long long res;
		if (!formed)
		{
			res = Inc(c1);
		}
		else
		{
			if (pos<m_pos)
			{
				currhash = (currhash - ord(str[pos])*p[0])*base + ord(c1);
				pos++;
				res = 0;
			}
			else
			{
				res = -2;
			}
		}
		return res;
	}

	unsigned long long Hash(string str)
	{
		unsigned long long res = 0;
		for (int i = 0; i<len; i++)
		{
			res += ord(str[i])*p[i];
		}
		return res;
	}

	int left()
	{
		return len - curr_len;
	}
};


class aux_SVHash
{
	

public:
	unsigned long long value;
	string curr_string;
	aux_SVHash()
	{
		curr_string = "";
		value = 0;
	}

	aux_SVHash(string newstr, unsigned long long newval) 
	{
		curr_string = newstr;
		value = newval;
	}

	int ord(char c)
	{
		switch (c)
		{
		case 'a': return 1;
			break;
		case 'c': return 2;
			break;
		case 'g': return 3;
			break;
		case 't': return 4;
			break;
		case 'A': return 1;
			break;
		case 'C': return 2;
			break;
		case 'G': return 3;
			break;
		case 'T': return 4;
			break;
		default:
			cout << "cashe error. unknown simbol: " << c << endl;
			break;
		}

	}

	vector<aux_SVHash> Inc(vector<char> c, vector<unsigned long long> &p)
	{
		vector<aux_SVHash> tmp;
		int curr_len = curr_string.size();
		for (int i = 0; i < c.size(); i++)
		{
			unsigned long long tmp_val = value + ord(c[i])*p[curr_len];
			string tmp_str = curr_string + c[i];
			tmp.push_back(aux_SVHash(tmp_str, tmp_val));
		}
		return tmp;
	}

	vector<aux_SVHash> Next(vector<char> c, vector<unsigned long long> &p, int base)
	{
		vector<aux_SVHash> tmp;
		int curr_len = curr_string.size();
		for (int i = 0; i < c.size(); i++)
		{
			unsigned long long tmp_val = (value - ord(curr_string[0])*p[0])*base + ord(c[i]);
			string tmp_str = curr_string.substr(1, curr_string.size() - 1)+c[i];
			tmp.push_back(aux_SVHash(tmp_str, tmp_val));
		}
		return tmp;
	}

	bool operator == (aux_SVHash &b)
	{
		return this->value == b.value;
	}

	bool operator > (aux_SVHash &b)
	{
		return this->value > b.value;
	}

	bool operator < (aux_SVHash &b)
	{
		return this->value < b.value;
	}
};

class StringVectorHash
{
public:
	vector<unsigned long long> p;
	int base;
	int len;

	vector<aux_SVHash> auxHashes;


	int curr_len;

	string str;
	int id;
	int pos;
	int m_pos;

	bool formed;

	StringVectorHash(int base, int len)
	{
		this->base = base;
		this->len = len;
		for (int i = len - 1; i >= 0; i--)
		{
			p.push_back(pow(base, i));
		}
	}

	
	vector<char> transform(char c)
	{
		if ((c == 'N') || (c == 'n')) return vector<char>{'a', 'c', 'g', 't'};
		else return vector<char>{c};
	}
	
	vector<unsigned long long> extract(vector<aux_SVHash> hashes)
	{
		vector<unsigned long long> tmp;
		for (int i = 0; i < hashes.size(); i++)
		{
			tmp.push_back(hashes[i].value);
		}
		return tmp;
	}


	vector<vector<unsigned long long>> GetHash(string read)
	{
		vector<vector<unsigned long long>> res;
		if (read.size() < len) return res;

		auxHashes.push_back(aux_SVHash("", 0));
		for (int i = 0; i < len; i++)
		{
			vector<aux_SVHash> tmp;
			for (int j = 0; j < auxHashes.size(); j++)
			{
				vector<aux_SVHash> tmp2 = auxHashes[j].Inc(transform(read[i]), p);
				tmp.insert(tmp.end(), tmp2.begin(), tmp2.end());
			}
			sort(tmp.begin(), tmp.end());
			tmp.resize(abs(distance(unique(tmp.begin(), tmp.end()), tmp.begin())));
			auxHashes = tmp;
		}

		res.push_back(extract(auxHashes));

		for (int i = len; i < read.length(); i++)
		{
			vector<aux_SVHash> tmp;
			for (int j = 0; j < auxHashes.size(); j++)
			{
				vector<aux_SVHash> tmp2 = auxHashes[j].Next(transform(read[i]), p, base);
				tmp.insert(tmp.end(), tmp2.begin(), tmp2.end());
			}
			sort(tmp.begin(), tmp.end());
			tmp.resize(abs(distance(unique(tmp.begin(), tmp.end()), tmp.begin())));
			auxHashes = tmp;
			res.push_back(extract(auxHashes));
		}

		return res;
	}

};


struct PoAl
{
	int ID;
	int start;
	int offset;
}; 
