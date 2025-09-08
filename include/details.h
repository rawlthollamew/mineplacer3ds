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
	Details(C2D_SpriteSheet _sheet, int _mineCount, Vector2i _dimentions);
	~Details();
	void draw(int _minesPlaced);

	bool timeStopped;
	void stopTimer();
	void resetTimer();

	int mineCount;
	Vector2i dimentions;
private:
	int startingTime;
	int finalTime;

	std::string intAsTime(int _timeSeconds);
	std::string timeFormat(int _timeValue);
	
	C2D_TextBuf timeTextBuffer;
	C2D_TextBuf minesTextBuffer;
	C2D_Sprite timeSprite;
	C2D_Sprite flagSprite;
};