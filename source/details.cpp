#include "details.h"

Details::Details(C2D_SpriteSheet _sheet, int _mineCount, Vector2i _dimentions, float _textSize)
	: mineCount(_mineCount), dimentions(_dimentions), textSize(_textSize)
{
	startingTime = (int)time(0);
	finalTime = -1;
	timeStopped = false;
	helpText = true;
	
	// staticly set (means the text buffer will never change)
	setTopText();
	
	// dynamic, meaning that the text buffer constantly updates.
	timeTextBuffer = C2D_TextBufNew(1 << 12); // 4096 (sick way of writing it)
	minesTextBuffer = C2D_TextBufNew(1 << 12); // 4096 (sick way of writing it)
	
	C2D_SpriteFromSheet(&flagSprite, _sheet, flagPng);
	C2D_SpriteFromSheet(&timeSprite, _sheet, clockPng);

	
	// constants for drawing the informations
	int infoHeight = (int)(flagSprite.image.subtex->height * 1.5f);
	
	infoPosition = { 0,topScreen.y - infoHeight };
	infoSize = { topScreen.x, infoHeight};
	infoPadding = (infoHeight - (int)(flagSprite.image.subtex->height)) / 2.f;
	
	// set the size of the digit
	C2D_Text tempText;
	C2D_TextBuf tempBuf = C2D_TextBufNew(4096);
	C2D_TextParse(&tempText, tempBuf, "0");

	C2D_TextGetDimensions(&tempText, textSize, textSize, &digitSize.x, &digitSize.y);


	timeDigits = 3;
	mineDigits = 2;

	// sent to the buttons class to position the buttons towards the right side of the infobox
	buttonsPosition = {
		topScreen.x - (2 * (flagSprite.image.subtex->width + infoPadding)) - infoPadding,
		infoPosition.y + infoPadding
	};
}

Details::~Details()
{
	C2D_TextBufDelete(timeTextBuffer);
	C2D_TextBufDelete(minesTextBuffer);
	C2D_TextBufDelete(topTextBuffer);
}

// i made these 2 functions that returns time as a string in the format hh:mm:ss
// but in the end i never used it cause i forgot that minesweeper shows the time as a 3 digit number.
// maybe it could be used for a leaderboard function?
std::string Details::timeFormat(int _timeValue)
{
	if (_timeValue == 0) return "00";
	else if (_timeValue < 10) return "0" + std::to_string(_timeValue);
	else return std::to_string(_timeValue);
}
std::string Details::intAsTime(int _timeSeconds)
{
	int hours = 0;
	int minutes = 0;
	int seconds = 0;
	
	hours = (int)(_timeSeconds / 3600);
	minutes = (int)(_timeSeconds / 60) % 60;
	seconds = _timeSeconds % 60;

	return timeFormat(hours) + ":" + timeFormat(minutes) + ":" + timeFormat(seconds);
}

void Details::drawBackground(Vector2i& _timeDigitsPosition, Vector2i& _mineDigitsPosition)
{
	Vector2i timeSpritePosition = {
		infoPosition.x + infoPadding,
		infoPosition.y + infoPadding
	};
	_timeDigitsPosition = {
		timeSpritePosition.x + infoPadding + timeSprite.image.subtex->width,
		infoPosition.y
	};
	
	// digitsSize.x * timeDigits is the size of the timebox.
	Vector2i mineSpritePosition = {
		(int)(_timeDigitsPosition.x + (digitSize.x * timeDigits) + infoPadding),
		infoPosition.y + infoPadding
	};
	_mineDigitsPosition = {
		mineSpritePosition.x + infoPadding + flagSprite.image.subtex->width,
		infoPosition.y
	};

	// infobox.
	C2D_DrawRectSolid(
		infoPosition.x,
		infoPosition.y,
		0,
		infoSize.x,
		infoSize.y,
		C2D_Color32f(1.f, 1.f, 1.f, 0.5f)
	);
	
	// time and mine icons
	C2D_SpriteSetPos(&timeSprite, timeSpritePosition.x, timeSpritePosition.y);
	C2D_SpriteSetPos(&flagSprite, mineSpritePosition.x, mineSpritePosition.y);
	
	C2D_DrawSprite(&flagSprite);
	C2D_DrawSprite(&timeSprite);
	
	// both digits positions
	// the timer has 3 digits, and mine counter have 2 digits.
	C2D_DrawRectSolid(
		_timeDigitsPosition.x,
		_timeDigitsPosition.y,
		0,
		digitSize.x * 3.f,
		infoSize.y,
		C2D_Color32f(1.f, 1.f, 1.f, 0.25f)
	);
	C2D_DrawRectSolid(
		_mineDigitsPosition.x,
		_mineDigitsPosition.y,
		0,
		digitSize.x * 2.f,
		infoSize.y,
		C2D_Color32f(1.f, 1.f, 1.f, 0.25f)
	);	
}

void Details::update(int _minesPlaced)
{
	int timeValue = (timeStopped) ? finalTime : (int)time(0) - startingTime;
	int mineValue = mineCount - _minesPlaced;

	timeString = std::to_string(timeValue);
	minesString = std::to_string(mineValue);
	
	while ((int)timeString.length() < timeDigits) timeString = "0" + timeString;
	while ((int)minesString.length() < mineDigits) minesString = "0" + minesString;
}

void Details::drawText(Vector2i _timeDigitsPosition, Vector2i _mineDigitsPosition)
{
	// static text buffer (meaning it does not change)
	// help/lb text

	float helpTextSize = 0.5f;

	C2D_DrawText(
		&topText,
		C2D_WithColor,
		30.f,
		30.f,
		0.f,
		helpTextSize,
		helpTextSize,
		C2D_Color32f(1.f,1.f,1.f,1.f)
	);

	// dynamic text buffer (meaning the text can change every frame by switching out the buffer)
	C2D_Text timeText;
	C2D_Text minesText;
	C2D_TextBufClear(timeTextBuffer);
	C2D_TextBufClear(minesTextBuffer);

	C2D_TextParse(&minesText, minesTextBuffer, minesString.c_str());
	C2D_TextOptimize(&minesText);
	
	C2D_TextParse(&timeText, timeTextBuffer, timeString.c_str());
	C2D_TextOptimize(&timeText);

	Vector2i textOffset = {
		0,
		(digitSize.y < infoSize.y) 
			? (int)((infoSize.y - digitSize.y ) / 2)
			: 0
	};
	
	C2D_DrawText(
		&timeText,
		C2D_WithColor,
		_timeDigitsPosition.x + textOffset.x,
		_timeDigitsPosition.y + textOffset.y,
		0.f,
		textSize,
		textSize,
		C2D_Color32f(1.f,1.f,1.f,1.f)
	);

	C2D_DrawText(
		&minesText,
		C2D_WithColor,
		_mineDigitsPosition.x + textOffset.x,
		_mineDigitsPosition.y + textOffset.y,
		0.f,
		textSize,
		textSize,
		C2D_Color32f(1.f,1.f,1.f,1.f)
	);

	
}

void Details::draw()
{
	Vector2i timePosition = {0,0};
	Vector2i mineCounterPosition = {0,0};

	// both vectors get edited before going into the drawtext function.
	drawBackground(timePosition, mineCounterPosition);
	drawText(timePosition, mineCounterPosition);
}

void Details::setTopText()
{
	std::string newText = "";

	if (helpText)
	{
		newText = 
			"How to play: \n\n"
			"    Place down mines using the touch controls.\n"
			"    Each mine will affect the area around it.\n"
			"    Fill the grid so that the tiles are perfectly even.\n"
			"    Hint: start from the outside and work your way in.\n\n"
			"Controls: \n\n"
			"    : Select current button (defaulted to new game)\n"
			"    : Change button selection (bottom right of top screen)";
	}
	else if (!helpText)
	{
		newText = "leaderboard:";
	}
	
	C2D_TextBufClear(topTextBuffer);
	topTextBuffer = C2D_TextBufNew(4096);

	C2D_TextParse(&topText, topTextBuffer, newText.c_str());
}

void Details::stopTimer()
{
	if (!timeStopped)
	{
		finalTime = (int)time(0) - startingTime;
		timeStopped = true;
	}
}

void Details::resetTimer()
{
	timeStopped = false;
	finalTime = -1;
	startingTime = (int)time(0);
}