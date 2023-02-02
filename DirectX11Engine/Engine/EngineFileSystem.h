#pragma once
#include <string>
using namespace std;
static string GetDirectoryFromPath(const string& filePath)
{
	size_t off1 = filePath.find_last_of('/');
	size_t off2 = filePath.find_last_of('\\');

	if (off1 == string::npos && off2 == string::npos)
	{
		return "";
	}
	else if (off1 == string::npos)
	{
		return filePath.substr(0, off2);
	}
	else if (off2 == string::npos)
	{
		return filePath.substr(0, off1);
	}
	return filePath.substr(0, off1 > off2 ? off1 : off2);
}