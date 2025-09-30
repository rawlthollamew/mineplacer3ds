#include "details.h"

Details::Details(C2D_SpriteSheet _sheet, int _mineCount, Vector2i _dimentions, float _textSize)
	: mineCount(_mineCount)
{
	C2D_SpriteFromSheet(&mineSprite, _sheet, flagPng);
	C2D_SpriteFromSheet(&timeSprite, _sheet, clockPng);

	replayBuf = C2D_TextBufNew(256);
	
	infoPadding = mineSprite.image.subtex->width / 4;
	infoPosition = {0, topScreen.y - mineSprite.image.subtex->height - (infoPadding * 2)};
	infoSize = {topScreen.x, mineSprite.image.subtex->height + (infoPadding * 2)};
	
	C2D_Text digitText;
	C2D_TextBuf digitBuf = C2D_TextBufNew(64);
	
	digitSize = {0, 0};
	halfDigitSize = {0, 0};
	
	C2D_TextParse(&digitText, digitBuf, "0");
	C2D_TextGetDimensions(&digitText, 1.f, 1.f, &digitSize.x, &digitSize.y);
	C2D_TextGetDimensions(&digitText, 0.5f, 0.5f, &halfDigitSize.x, &halfDigitSize.y);

	C2D_TextBufDelete(digitBuf);
	
	mines = CounterDisplay(mineSprite, infoPadding, _textSize, digitSize, 2, {0, infoPosition.y});
	times = CounterDisplay(timeSprite, infoPadding, _textSize, digitSize, 2, {(int)mines.size.x, infoPosition.y});
	infoColor = C2D_Color32f(1.f,1.f,1.f,0.5f);
	replayMode = false;

	textPanel = TextPanel(_textSize / 2.f, _sheet, halfDigitSize, digitSize);

	initReplayText(0, "");
}

Details::~Details()
{
	C2D_TextBufDelete(replayBuf);
}

void Details::initReplayText(int _finalTime, std::string _name)
{
	std::string replayString =
		"You are currently in replay mode. Press  to exit replay mode. \n"
		"    Time: " + std::to_string(_finalTime) + "    Player: " + _name;

	C2D_TextParse(&replayText, replayBuf, replayString.c_str());
	C2D_TextOptimize(&replayText);
}

void Details::draw(int _difficultyIndex)
{
	C2D_DrawRectSolid(
		infoPosition.x,
		infoPosition.y,
		0,
		infoSize.x,
		infoSize.y,
		infoColor
	);

	textPanel.draw({infoPadding, infoPadding}, _difficultyIndex);
	mines.draw();
	times.draw();

	if (replayMode)
	{
		Vector2i replaySize = {
			topScreen.x,
			(int)(halfDigitSize.y * 2)
		};

		Vector2i replayPosition = {
			infoPosition.x,
			infoPosition.y - replaySize.y
		};

		C2D_DrawRectSolid(
			replayPosition.x,
			replayPosition.y,
			0,
			replaySize.x,
			replaySize.y,
			C2D_Color32f(0.f,0.f,0.2f,1.f)
		);

		C2D_DrawText(
			&replayText,
			C2D_WithColor,
			replayPosition.x,
			replayPosition.y,
			0,
			0.5f,
			0.5f,
			C2D_Color32f(1.f,1.f,1.f,1.f)
		);

		C2D_TextBufClear(replayBuf);
	}
}

void Details::update(u32 _color, int _minesRemaining, int _currentTime)
{
	infoColor = _color;

	mines.update(_minesRemaining);
	times.update(_currentTime);
}