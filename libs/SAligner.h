#pragma once
#include <map>
#include <vector>

#include <iostream>
#include <stdio.h>

#include <string>
#include <cstdio>
#include <math.h>
#include <WArray.h>
#include <Node.h>

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

class SBundle
{
public:
	pair<int, int> last_pos;
	vector<STrack> Tracks;
	bool failed_elong;
	bool cont;

	SBundle() {};
	SBundle(STrack strack)
	{
		Tracks.push_back(strack);
	};

	void Increase(int loc, pair<int, int> p1, pair<int, int>p2)
	{
		last_pos = p2;
		for (int i = 0; i < Tracks.size(); i++)
		{
			Tracks[i].Increase(loc, p1, p2);
		}
		failed_elong = false;
	}

	vector<STrack> Clear(int hash_length)
	{
		vector<STrack> res;
		if (failed_elong == false) return res;
		for(int i =0; i<Tracks.size(); i++)
		{
				if (Tracks[i].length > hash_length * 2)
				{
					res.push_back(Tracks[i]);
				}
				Tracks.erase(Tracks.begin() + i);
				i--;
		}
		return res;
	}
	
};

class FinTrackPool
{
public:
	int seedkey;
	map<int, pair<bool, int>> values;
	FinTrackPool()
	{
	};

	void AddPoint(pair<int, int> p, bool seed = false) // ID/value
	{
		if (seed) seedkey = p.first;
		if (values[p.first].second < p.second)
		{
			values[p.first] = pair<bool, int>(true, p.second);
		}
	}

	void AddVectorPoint(vector<pair<int, int>> p_vec)
	{
		ReFlagVisited();
		for (int i = 0; i < p_vec.size(); i++)
		{
			AddPoint(p_vec[i]);
		}
	}

	void ReFlagVisited()
	{
		for (auto k : values)
		{
			values[k.first].first = false;
		}
	}

	bool NeedToUpdate()
	{
		
		for (auto k : values)
		{
			if (values[k.first].first)
				return true;
		}
		return false;
	}

	int GetMaxWeight()
	{
		int maxlen = values[seedkey].second;
		for (auto k : values)
		{
			if (k.second.second > maxlen) maxlen = k.second.second;
		}
		return maxlen;
	}
};



class STrackList
{
public:
	int hash_length;
	vector<STrack> stored_path;

	vector<STrack> linked_blocks;


	//vector<STrack> current_path;
	map<pair<int, int>, SBundle> curr_path;
	map<pair<int, int>, SBundle> elonged_path;

	vector<vector<int>> sorted_by_end;
	vector<vector<int>> sorted_by_begin;

	vector<vector<int>> relations;

	FinTrackPool FinTracks;
	FinTrackPool FinBackTracks;

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
		SBundle sb(tmp);
		if (elonged_path.find(p2) == elonged_path.end())
		{
			elonged_path[p2] = sb;
		}
		else
		{
			elonged_path[p2].Tracks.push_back(tmp);
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
			curr_path[t.first].cont = false;
		}
	}

	void Clear()
	{
		vector<pair<int, int>> todelete;
		for (auto t : curr_path)
		{
			vector<STrack> res = curr_path[t.first].Clear(hash_length);
			stored_path.insert(stored_path.end(), res.begin(), res.end());
			if (curr_path[t.first].Tracks.size() == 0)
			{
				todelete.push_back(t.first);
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
				curr_path[p.first].cont = true;
				elonged_path[p.second] = curr_path[p.first];
				elonged_path[p.second].Increase(loc, p.first, p.second);
			}
		}
		SaveBreakedBundles();
		curr_path = elonged_path;
		elonged_path.clear();
		Clear();
		Reflag();

	}

	void SaveBreakedBundles()
	{
		for (auto t : curr_path)
		{
			if (curr_path[t.first].cont == false)
			{
				vector<STrack> res = curr_path[t.first].Clear(hash_length);
				stored_path.insert(stored_path.end(), res.begin(), res.end());
			}
		}
	}

	void InitRelations(map<int, Node> &Body)
	{
		int l = stored_path.size();
		relations = vector<vector<int>>(l);
		for (int i = 0; i < l; i++)
		{
			relations[i] = vector<int>(l);
		}


		for (int i = 0; i < l; i++)
		{
			for (int j = 0; j < l; j++)
			{
				relations[i][j] = GetSTracksRelation(i, j, Body);
			}
		}

	}

	int GetNodeRelation(pair<int, int> p1, pair<int, int> p2, map<int, Node> &Body)
	{

		int q = Body[p1.first].GetRelation(Body[p2.first]); //-1 = prev, 0 = parallel, 1 = next;
		if (q == 3)
		{
			if (p1.second < p2.second)
			{
				q = 1;
			}
			else if (p1.second > p2.second)
			{
				q = -1;
			}
			else
			{
				q = 0;
			}
		}

		return q;
	}

	int GetSTracksRelation(int sT1ID, int sT2ID, map<int, Node> &Body)
	{
		
		int c1 = GetNodeRelation(stored_path[sT1ID].pos_last, stored_path[sT2ID].pos_first, Body);
		int c2 = GetNodeRelation(stored_path[sT1ID].pos_first, stored_path[sT2ID].pos_last, Body);
		if (c1 == 1) return 1;
		if (c2 == -1) return -1;
		return 0;
	}

	int GetGapEstimate(int sT1ID, int sT2ID)
	{
		return stored_path[sT1ID].loc_last - stored_path[sT2ID].loc_first;
	}

	int GetInitialSeed()
	{
		int SPl = stored_path.size();
		if (SPl == 0) return -1;
		int len = stored_path[0].length;
		int i_res = 0;
		for (int i = 0; i < SPl; i++)
		{
			if (stored_path[i].length > len)
			{
				i_res = i;
				len = stored_path[i].length;
			}
		}
		return i_res;
	}

	void SortByBeginEnds(int len)
	{
		sorted_by_begin = vector<vector<int>>(len);
		sorted_by_end = vector<vector<int>>(len);
		for (int i = 0; i < stored_path.size(); i++)
		{
			sorted_by_begin[stored_path[i].loc_first].push_back(i);
			sorted_by_end[stored_path[i].loc_last].push_back(i);
		}
	}



	vector<pair<int,int>> GetListofPrecs(int seedId, int weight)
	{
		vector<pair<int, int>> res;
		for (int i = 0; i < stored_path.size(); i++)
		{
			int loc_dist = (stored_path[i].loc_first - stored_path[seedId].loc_last);
			if ((relations[seedId][i] == 1) &&
				(loc_dist > 0))
			{
				res.push_back(pair<int,int>(i, weight + stored_path[i].length - loc_dist+1));
			}
		}
		return res;
	}


	vector<pair<int, int>> GetListofAns(int seedId, int weight)
	{
		vector<pair<int, int>> res;
		for (int i = 0; i < stored_path.size(); i++)
		{
			int loc_dist = (stored_path[seedId].loc_first - stored_path[i].loc_last);
			if ((relations[seedId][i] == -1) &&
				(loc_dist > 0))
			{
				res.push_back(pair<int, int>(i, weight + stored_path[i].length - loc_dist + 1));
			}
		}
		return res;
	}

	float LigationBurn(int readlen)
	{
		int seedID = GetInitialSeed();
		FinTracks.AddPoint(pair<int, int>(seedID, stored_path[seedID].length), true);
		bool needtoupdate = false;
		vector<pair<int, int>> nexts;
		do
		{
			for (auto k : FinTracks.values)
			{
				if (k.second.first)
				{
					vector<pair<int, int>> tmp = GetListofPrecs(k.first, k.second.second);
					nexts.insert(nexts.end(), tmp.begin(), tmp.end());
				}
			}
			FinTracks.AddVectorPoint(nexts);
			nexts.clear();
			needtoupdate = FinTracks.NeedToUpdate();
		} 
		while (needtoupdate);

		
		FinBackTracks.AddPoint(pair<int, int>(seedID, stored_path[seedID].length), true);
		needtoupdate = false;
		do
		{
			for (auto k : FinBackTracks.values)
			{
				if (k.second.first)
				{
					vector<pair<int, int>> tmp = GetListofAns(k.first, k.second.second);
					nexts.insert(nexts.end(), tmp.begin(), tmp.end());
				}
			}
			FinBackTracks.AddVectorPoint(nexts);
			nexts.clear();
			needtoupdate = FinBackTracks.NeedToUpdate();
		} while (needtoupdate);
		

		int TotalWeight = FinBackTracks.GetMaxWeight() + FinTracks.GetMaxWeight() - stored_path[seedID].length;

		float coverage = (float)TotalWeight / (float)readlen;

		return coverage;

	}

};


class SAligner
{
public:
	bool failed = false;


	int hash_length;
	int start_loc;
	string read;
	STrackList STracks;
	vector<CoordMatcher> r_matches;
	

	SAligner(int hash, string input_read, vector<vector<WArray>> warray)
	{
		read = input_read;
		hash_length = hash;
		for (int i = 0; i < warray.size(); i++)
		{
			CoordMatcher tmp(warray[i]);
			r_matches.push_back(tmp);
		}

		int i = 0; 
		while (r_matches[i].m.size() == 0)
		{
			i++;
			if (i == r_matches.size())
			{
				failed = true;
				return;
			}
		}
		for (auto t : r_matches[i].m)
		{
			start_loc = i;
			STracks = STrackList(hash_length, start_loc, r_matches[i]);
		}
	}

	int Run()
	{
		int curr_loc= start_loc+hash_length - 1;
		int max_pos = r_matches.size()-1;
		if (curr_loc > max_pos)
		{
			STracks.Clear();
			return 0;
		}
		do
		{
			STracks.Step(curr_loc, r_matches[curr_loc]);
			curr_loc += hash_length-1;
		} while (curr_loc <= max_pos);
		STracks.Clear();
		return 0;
	}

	int Ligation(int readlen, map<int, Node> &Body)
	{
		if (STracks.stored_path.size() == 0) return -1;
		STracks.InitRelations(Body);
		
		float coverage = STracks.LigationBurn(readlen);
		if (coverage >= 0.75) return 0;

		return -1;
	}

};