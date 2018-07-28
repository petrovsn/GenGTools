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
#include <thread>
#include <future>

using namespace std;

class Worker
{
public:
	bool enabled;
	future<int> result;
	string curr_read;

	Worker()
	{
		enabled = true;
	};

	bool Check()
	{
		if (enabled) return true;
		future_status res_stat = result.wait_for(chrono::microseconds(1));
		bool res = (res_stat == future_status::ready);
		return (res);
	}

	int GetValue()
	{
		if (enabled) return -1;
		else return result.get();
	}

	int Finalize()
	{
		if (enabled) return -1;
		future_status res_stat = result.wait_for(chrono::microseconds(1));
		if (res_stat == future_status::ready)
			return result.get();
		if (res_stat == future_status::timeout)
		{
			result.wait();
			return result.get();
		}
		if (res_stat == future_status::deferred)
			return -1;
	}

	void Run(Graph &VG, string read)
	{
		curr_read = read;
		enabled = false;
		result = async(launch::async, &Worker::OneReadAlign, this, ref(VG), read);
	}

	int OneReadAlign(Graph &VG, string read)
	{
		string res_read = reverse(read);
		int tmp_res = VG.SFinder(read);
		if (tmp_res != 0)
		{
			tmp_res = VG.SFinder(res_read);
		}
		return tmp_res;
	}
};

class ThreadPool
{

public:

	int aligned_reads;
	int FreeCount;
	vector<Worker> threads;

	ThreadPool(int numthreads)
	{
		for (int i = 0; i < numthreads; i++)
		{
			threads.push_back(Worker());
		}
	}

	int getFreeThread()
	{
		int res = -1;
		for (int i = 0; i < threads.size(); i++)
		{
			if (threads[i].Check())
			{
				res = i;
				if (threads[i].GetValue() == 0)
				{
					aligned_reads++;
				}
				return res;
			}
		}
		return res;
	}

	void RunAlignment(int ThreadId, Graph &VG, string read)
	{
		threads[ThreadId].Run(VG, read);
	}


	void FinishAll()
	{
		for (int i = 0; i < threads.size(); i++)
		{
			int tmp = threads[i].Finalize();
			if (tmp == 0) aligned_reads++;
		}
	}
};








int SRAalign(Graph &VG, string path, string output, int numthreads)
{
	map<string, bool> checked;
	int count = 0;
	int total = 0;
	int failed_r = 0;
	int failed_s = 0;

	string buf;
	string res_buf;
	ifstream fin(path);
	ThreadPool TPool = ThreadPool(numthreads);

	while (!fin.eof())
	{

		getline(fin, buf);
		getline(fin, buf);
		res_buf = reverse(buf);



		if ((checked.find(buf) == checked.end()) &&
			(checked.find(res_buf) == checked.end()))
		{
			checked[buf] = true;
			checked[res_buf] = true;
			total++;

			int r = -1;
			do
			{
				r= TPool.getFreeThread();
			} while (r == -1);

			TPool.RunAlignment(r, VG, buf);
		}
	}
	TPool.FinishAll();
	return 0;
}

int SRAalign(Graph &VG, string path, string output)
{
	map<string, bool> checked;
	int count = 0;
	int total = 0;
	int failed_r = 0;
	int failed_s = 0;

	string buf;
	string res_buf;
	ifstream fin(path);
	while (!fin.eof())
	{

		getline(fin, buf);
		getline(fin, buf);
		res_buf = reverse(buf);



		if ((checked.find(buf) == checked.end()) &&
			(checked.find(res_buf) == checked.end()))
		{
			checked[buf] = true;
			checked[res_buf] = true;
			total++;
			int res = VG.SFinder(buf);
			if (res != 0)
			{

				res = VG.SFinder(res_buf);
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
	}
	fin.close();

	ofstream f_out;

	f_out.open(output);
	f_out << "Total:" << total << '\n';
	f_out << "Aligned:" << count << '\n';
	f_out << "Proportion:" << (float)(count / total) << '\n';
	f_out.close();

	return 0;

}