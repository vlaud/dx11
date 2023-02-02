#pragma once
#include <windows.h>
#include <string>
using namespace std;
class Debug
{
public:
	static void Log(wstring msg);
	static void Log(HRESULT hr, wstring msg);
};

