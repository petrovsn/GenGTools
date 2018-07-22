#pragma once
#include <tuple>;
using namespace std;

struct WArray
{
	int value[4];
	bool operator <(WArray &b)
	{
		return tie(this->value[0], this->value[1], this->value[2], this->value[3]) < tie(b.value[0], b.value[1], b.value[2], b.value[3]);
	}

	bool operator == (WArray &b)
	{
		return (((this->value[0]) == (b.value[0])) && ((this->value[1]) == (b.value[1])) && ((this->value[2]) == (b.value[2])) && ((this->value[3]) == (b.value[3])));
	}
	bool operator <(const WArray &b)
	{
		return tie(this->value[0], this->value[1], this->value[2], this->value[3]) < tie(b.value[0], b.value[1], b.value[2], b.value[3]);
	}

	bool operator == (const WArray &b)
	{
		return (((this->value[0]) == (b.value[0])) && ((this->value[1]) == (b.value[1])) && ((this->value[2]) == (b.value[2])) && ((this->value[3]) == (b.value[3])));
	}
}; 
