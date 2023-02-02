#pragma once
#include <chrono>
using namespace std;
using namespace chrono;

class Time
{
	bool isRunning = false;
	static double deltaTime;
#ifdef _WIN32
	time_point<steady_clock> start;
	time_point<steady_clock> stop;
#else //64
	time_point<system_clock> start;
	time_point<system_clock> stop;
#endif
public:
	Time();
	double GetMilisecondsElapsed();
	bool Start();
	bool Stop();
	void Restart();
	void UpdateDeltaTime();
	static double GetDeltaTime();
};

