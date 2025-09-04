#pragma once
#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "utils.h"

// time for this map
// mines left
// buttons
// new map with options (mine count, width, height)

class Details
{
public:
	Details();
	~Details();
	void draw();
private:
	int startingTime;
	std::string intAsTime(int _timeSeconds);
	std::string timeFormat(int _timeValue);
	C2D_TextBuf textBuffer;
};