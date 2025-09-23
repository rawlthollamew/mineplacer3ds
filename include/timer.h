#pragma once
#include <ctime>
#include <string>

class Timer
{
public:
	Timer();
	void stop();
	void reset();
	
	int getTime()
	{
		if (stopped) return finalTime;
		else return time(0) - startTime;
	}

	std::string intAsTime(int _timeValue);
	
private:
	std::string timeFormat(int _timeValue);
	int startTime;
	int finalTime;
	bool stopped;
};