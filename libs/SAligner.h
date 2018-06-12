#pragma once
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <cstdio>
#include <map>
#include <math.h>
#include <WArray.h>

class CoordMatcher
{
public:
	map<pair<int, int>, pair<int, int>> m;

	CoordMatcher(vector<WArray> warray)
	{
		for (int i = 0; i < warray.size(); i++)
		{
			AddTrack(warray[i]);
		}
	}

	void AddTrack(WArray w1)
	{
		pair<int, int> p1 = pair<int, int>(w1.value[0], w1.value[1]);
		pair<int, int> p2 = pair<int, int>(w1.value[2], w1.value[3]);
		m[p1] = p2;
	}

	pair<int, int> Next(pair<int, int> c_begin)
	{
		if (m.find(c_begin) == m.end()) {
			return pair<int, int>(-2, 0);
		}
		else { 
			// found
			return m[c_begin];
		}
		return pair<int, int>(-2, 0);
	}
};

class STrack
{

public:
	int loc_first, loc_last;
	pair<int, int> pos_first, pos_last;
	bool failed_elong;
	int length;
	int hash_length;

	STrack() {};
	STrack(int hash, int loc, pair<int, int> p_first, pair<int, int> p_last)
	{
		loc_first = loc;
		loc_last = loc + hash;
		pos_last = p_last;
		pos_first = p_first;
		failed_elong = false;
		hash_length = hash;
		length = hash;
	}

	void Step(int loc, CoordMatcher cm)
	{
		pair<int, int> pos = cm.Next(pos_last);
		if (pos.first != -2)
		{
			pos_last = pos;
			loc_last = loc;
			length += hash_length;
		}
		else
		{
			failed_elong = true;
		}
	}

	void Increase(int loc, pair<int, int> p1, pair<int, int>p2)
	{
		loc_last = loc;
		pos_last = p2;
		length += hash_length-1;
		failed_elong = false;
	}
};


class STrackPool
{
public:
	vector<STrack> stracks;
	int failed_elong;
	
	void Increase(int loc, pair<int, int> p1, pair<int, int>p2)
	{

	}


};

class STrackList
{
public:
	int hash_length;
	vector<STrack> stored_path;
	//vector<STrack> current_path;
	map<pair<int, int>, STrack> curr_path;


	STrackList() {};
	STrackList(int hash, int loc, CoordMatcher cm)
	{
		hash_length = hash;
		for (auto t : cm.m)
		{
			STrack tmp(hash, loc, t.first, t.second);
			curr_path[t.second] = tmp;
		}
	}

	void AddPoint(int loc, pair<pair<int, int>, pair<int, int>> point)
	{
		STrack tmp(hash_length, loc, point.first, point.second);
		
		if (curr_path.find(point.second) == curr_path.end())
		{
			curr_path[point.second] = tmp;
		}
	}

	void AddPoint(int loc, pair<int, int> p1, pair<int, int> p2)
	{
		STrack tmp(hash_length, loc, p1, p2);
		if (curr_path.find(p2) == curr_path.end())
		{
			curr_path[p2] = tmp;
		}
	}


	void Restart(int loc, CoordMatcher cm)
	{
		for (auto t : cm.m)
		{
			STrack tmp(hash_length, loc, t.first, t.second);
		}
	}

	void Reflag()
	{
		for (auto t : curr_path)
		{
			curr_path[t.first].failed_elong = true;
			
		}
	}

	void Clear()
	{
		vector<pair<int, int>> todelete;
		for (auto t : curr_path)
		{
			if (t.second.failed_elong)
			{
				todelete.push_back(t.first);
				if (t.second.length > hash_length * 2)
				{
					stored_path.push_back(t.second);
				}
			}
		}
		for (int i = 0; i < todelete.size(); i++)
		{
			curr_path.erase(todelete[i]);
		}
	}


	void Step(int loc, CoordMatcher cm)
	{
		for (auto p : cm.m)
		{
			if (curr_path.find(p.first) == curr_path.end())
			{
				AddPoint(loc, p.first, p.second);
			}
			else
			{
				curr_path[p.first].Increase(loc, p.first, p.second);
				curr_path[p.second] = curr_path[p.first];
				curr_path.erase(p.first);
			}
		}

		Clear();
		Reflag();

	}

	

};


class SAligner
{
public:
	int hash_length;
	int start_loc;
	string read;
	STrackList STracks;
	vector<CoordMatcher> r_matches;
	

	SAligner(int hash, vector<vector<WArray>> warray)
	{
		hash_length = hash;
		for (int i = 0; i < warray.size(); i++)
		{
			CoordMatcher tmp(warray[i]);
			r_matches.push_back(tmp);
		}

		int i = 0; 
		while (r_matches[i].m.size() == 0) i++;
		for (auto t : r_matches[i].m)
		{
			start_loc = i;
			STracks = STrackList(hash_length, start_loc, r_matches[i]);
		}
	}

	int Run()
	{
		int curr_loc= start_loc+hash_length - 1;
		int max_pos = r_matches.size();
		do
		{
			STracks.Step(curr_loc, r_matches[curr_loc]);
			curr_loc += hash_length-1;
		} while (curr_loc <= max_pos);
		return 0;
	}

};