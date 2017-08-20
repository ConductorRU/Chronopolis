#pragma once
namespace DEN
{
	string trim(const string &line, const string &keys = " ");
	vector<string> explode(const string &s, char delim, bool notEmpty = true);
};