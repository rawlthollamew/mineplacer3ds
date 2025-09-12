#pragma once
#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "utils.h"

// buttons
// new map with options (mine count, width, height, walls)

class Details
{
public:
	Details(C2D_SpriteSheet _sheet, int _mineCount, Vector2i _dimentions, float _textSize);
	~Details();
	void update(int _minesPlaced);
	void draw();

	bool helpText;
	void setTopText();
	
	bool timeStopped;
	void stopTimer();
	void resetTimer();
	
	int mineCount;
	Vector2i dimentions;
	Vector2i buttonsPosition;
	
	Vector2i getInfoPosition()
	{
		return infoPosition;
	};
	int getInfoPadding()
	{
		return infoPadding;
	};
private:
	float textSize;
	int startingTime;
	int finalTime;
	
	// vectors for positioning the informations
	// what a mess
	
	// position and size of the background grey behind everything
	// this uses 1.5x the sprite for height, and width is topScreen.x
	Vector2i infoPosition;
	Vector2i infoSize;
	// padding within the info box taking into account sprite size.
	int infoPadding;
	// digits size. this is the size for one glyph, stored to use later.
	Vector2f digitSize;
	int timeDigits;
	int mineDigits;

	void drawBackground(Vector2i& _timeDigitsPosition, Vector2i& _mineDigitsPosition);

	std::string timeString;
	std::string minesString;
	void drawText(Vector2i _timeDigitsPosition, Vector2i _mineDigitsPosition);
	
	std::string intAsTime(int _timeSeconds);
	std::string timeFormat(int _timeValue);
	
	// dynamic text buffers 
	C2D_TextBuf timeTextBuffer;
	C2D_TextBuf minesTextBuffer;

	// static text buffers
	C2D_TextBuf topTextBuffer;
	C2D_Text topText;

	C2D_Sprite timeSprite;
	C2D_Sprite flagSprite;
};