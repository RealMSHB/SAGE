#pragma once

#include <vector>
#include <string>

using namespace std;

class WordDelimitedByCharacter
{
public:
	static vector<string> split(const string& str, const string& delim);
};

