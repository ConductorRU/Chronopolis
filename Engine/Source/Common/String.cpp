#include "stdafx.h"
#include "String.h"
namespace DEN
{
	string trim(const string &line, const string &keys)
	{
		string res;
		const size_t size = line.size();
		const size_t cn = keys.size();
		if(size == 0 || cn == 0)
			return res;
		size_t start = 0;
		size_t end = 0;
		for(size_t i = 0; i < size; ++i)
		{
			bool is = false;
			for(size_t k = 0; k != cn; ++k)
				if(line[i] == keys[cn])
					is = true;
			if(!is)
			{
				start = i;
				break;
			}
		}
		for(size_t i = size - 1; i >= 0; --i)
		{
			bool is = false;
			for(size_t k = 0; k != cn; ++k)
				if(line[i] == keys[cn])
					is = true;
			if(!is)
			{
				end = i;
				break;
			}
		}
		for(size_t i = start; i <= end; ++i)
			res.push_back(line[i]);
		return res;
	}
	vector<string> explode(const string &s, char delim, bool notEmpty)
	{
		vector<string> result;
		string w;
		for(char c: s)
			if(c == delim)
			{
				if(w.size() || !notEmpty)
					result.push_back(w);
				w = "";
			}
			else
				w += c;
		if(w.size() || !notEmpty)
			result.push_back(w);
		return result;
	}
};