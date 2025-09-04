#include "details.h"

Details::Details()
{
	textBuffer = C2D_TextBufNew(4096);
	startingTime = time(0);
}

Details::~Details()
{
	C2D_TextBufDelete(textBuffer);
}

std::string Details::timeFormat(int _timeValue)
{
	if (_timeValue == 0) return "00";
	else if (_timeValue < 10) return "0" + std::to_string(_timeValue);
	else return std::to_string(_timeValue);
}

std::string Details::intAsTime(int _timeSeconds)
{
	// hh:mm:ss
	int hours = 0;
	int minutes = 0;
	int seconds = 0;
	
	hours = (int)(_timeSeconds / 3600);
	minutes = hours / 60;
	seconds = _timeSeconds % 60;

	return timeFormat(hours) + ":" + timeFormat(minutes) + ":" + timeFormat(seconds);
}

void Details::draw()
{
	C2D_TextBufClear(textBuffer);
	C2D_Text detailsText;

	std::string currentTime = intAsTime((int)(time(0) - startingTime));

	C2D_TextParse(&detailsText, textBuffer, currentTime.c_str());
	C2D_TextOptimize(&detailsText);
	C2D_DrawText(&detailsText, C2D_WithColor, 0, 0, 0, 1, 1, C2D_Color32f(1.f,1.f,1.f,1.f));
}