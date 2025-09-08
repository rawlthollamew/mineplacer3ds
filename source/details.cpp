#include "details.h"

Details::Details(C2D_SpriteSheet _sheet, int _mineCount, Vector2i _dimentions)
	: mineCount(_mineCount), dimentions(_dimentions)
{
	startingTime = (int)time(0);
	finalTime = -1;
	timeStopped = false;
	
	timeTextBuffer = C2D_TextBufNew(1 << 12); // 4096 (sick way of writing it)
	minesTextBuffer = C2D_TextBufNew(1 << 12); // 4096 (sick way of writing it)
	C2D_SpriteFromSheet(&flagSprite, _sheet, 3);
	C2D_SpriteFromSheet(&timeSprite, _sheet, 4);
}

Details::~Details()
{
	C2D_TextBufDelete(timeTextBuffer);
}

std::string Details::timeFormat(int _timeValue)
{
	if (_timeValue == 0) return "00";
	else if (_timeValue < 10) return "0" + std::to_string(_timeValue);
	else return std::to_string(_timeValue);
}

std::string Details::intAsTime(int _timeSeconds)
{
	// i made this thing that returns time as a string in the format hh:mm:ss
	// but in the end i never used it cause minesweeper shows the time as a 3 digit number.
	// maybe it could be used for a leaderboard function?
	int hours = 0;
	int minutes = 0;
	int seconds = 0;
	
	hours = (int)(_timeSeconds / 3600);
	minutes = (int)(_timeSeconds / 60) % 60;
	seconds = _timeSeconds % 60;

	return timeFormat(hours) + ":" + timeFormat(minutes) + ":" + timeFormat(seconds);
}



void Details::draw(int _minesPlaced)
{
	C2D_TextBufClear(timeTextBuffer);
	C2D_TextBufClear(minesTextBuffer);
	C2D_Text timeText;
	C2D_Text minesText;

	// C2D_TextParse(&timeText, textBuffer, "000");
	// float width = 0;
	// float height = 0;

	// C2D_TextGetDimensions(&timeText, 1, 1, &width, &height);

	// printf("%f, %f\n", width, height);
	// // size of 3 digits is 51, 30
	// // rounded it to 50, 30

	Vector2i detailsPosition = {0, topScreen.y - (int)(flagSprite.image.subtex->height * 1.5)};
	Vector2i detailsSize = {topScreen.x, (int)(flagSprite.image.subtex->height * 1.5)};
	int detailsPadding = (detailsSize.y - flagSprite.image.subtex->height) / 2.f;
	
	Vector2i digitsSize = { 50, 30 };
	// 2 is the padding size inbetween the text and rectangle, with trials proving this is the best number
	int digitsPadding = 2;
	Vector2i digitsPosition = {(int)(flagSprite.image.subtex->width * 1.5f), detailsPosition.y};

	Vector2i minesPosition = {
		detailsPosition.x + (detailsPadding * 3) + digitsSize.x + (flagSprite.image.subtex->width),
		digitsPosition.y + detailsPadding
	};

	Vector2i minesSize = {( 2 * digitsSize.x ) / 3, digitsSize.y};

	C2D_DrawRectSolid(
		detailsPosition.x,
		detailsPosition.y,
		0,
		detailsSize.x,
		detailsSize.y,
		C2D_Color32f(1.f, 1.f, 1.f, 0.5f)
	);

	// both digits positions
	C2D_DrawRectSolid(
		digitsPosition.x,
		digitsPosition.y,
		0,
		digitsSize.x,
		digitsSize.y,
		C2D_Color32f(1.f, 1.f, 1.f, 0.25f)
	);
	C2D_DrawRectSolid(
		minesPosition.x + detailsPadding + flagSprite.image.subtex->width,
		minesPosition.y - detailsPadding,
		0,
		minesSize.x,
		minesSize.y,
		C2D_Color32f(1.f, 1.f, 1.f, 0.25f)
	);

	C2D_SpriteSetPos(
		&timeSprite,
		detailsPosition.x + detailsPadding,
		detailsPosition.y + detailsPadding
	);

	C2D_SpriteSetPos(
		&flagSprite,
		minesPosition.x,
		minesPosition.y
	);

	C2D_DrawSprite(&flagSprite);
	C2D_DrawSprite(&timeSprite);

	// time text handler
	int timeValue = (timeStopped) ? finalTime : (int)time(0) - startingTime;
	std::string timeOutput = std::to_string(timeValue);

	if (timeValue < 10) timeOutput = "00" + timeOutput;
	else if (timeValue >= 10 && timeValue < 100) timeOutput = "0" + timeOutput;

	C2D_TextParse(&timeText, timeTextBuffer, timeOutput.c_str());
	C2D_TextOptimize(&timeText);

	C2D_DrawText(
		&timeText,
		C2D_WithColor,
		digitsPosition.x + digitsPadding,
		digitsPosition.y,
		0,
		1,
		1,
		C2D_Color32f(1.f,1.f,1.f,1.f)
	);

	// mine text handler
	int mineValue = mineCount - _minesPlaced;
	std::string minesOutput = std::to_string(mineValue);
	
	if (mineValue < 10) minesOutput = "0" + minesOutput;

	C2D_TextParse(&minesText, minesTextBuffer, minesOutput.c_str());
	C2D_TextOptimize(&minesText);

	C2D_DrawText(
		&minesText,
		C2D_WithColor,
		minesPosition.x + (digitsPadding * 2) + flagSprite.image.subtex->width + digitsPadding,
		minesPosition.y - detailsPadding,
		0,
		1,
		1,
		C2D_Color32f(1.f,1.f,1.f,1.f)
	);
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