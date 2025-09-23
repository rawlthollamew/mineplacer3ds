#include "timer.h"

Timer::Timer()
{
	reset();
}

void Timer::stop()
{
	if (!stopped)
	{
		finalTime = time(0) - startTime;
		stopped = true;
	}
}

void Timer::reset()
{
	stopped = false;
	finalTime = -1;
	startTime = time(0);
}

std::string Timer::timeFormat(int _timeValue)
{
	if (_timeValue == 0) return "00";
	else if (_timeValue < 10) return "0" + std::to_string(_timeValue);
	else return std::to_string(_timeValue);
}

std::string Timer::intAsTime(int _timeSeconds)
{
	int hours = 0;
	int minutes = 0;
	int seconds = 0;
	
	hours = (int)(_timeSeconds / 3600);
	minutes = (int)(_timeSeconds / 60) % 60;
	seconds = _timeSeconds % 60;

	return timeFormat(hours) + ":" + timeFormat(minutes) + ":" + timeFormat(seconds);
}