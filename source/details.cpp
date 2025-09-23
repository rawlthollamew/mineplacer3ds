#include "details.h"

Details::Details(C2D_SpriteSheet _sheet, int _mineCount, Vector2i _dimentions, float _textSize)
	: textPanel(_textSize / 2.f), mineCount(_mineCount)
{
	C2D_SpriteFromSheet(&mineSprite, _sheet, flagPng);
	C2D_SpriteFromSheet(&timeSprite, _sheet, clockPng);

	C2D_SpriteFromSheet(&selectionTopLeft, _sheet, selectionTopLeftPng);
	C2D_SpriteFromSheet(&selectionBottomLeft, _sheet, selectionBottomLeftPng);
	C2D_SpriteFromSheet(&selectionTopRight, _sheet, selectionTopRightPng);
	C2D_SpriteFromSheet(&selectionBottomRight, _sheet, selectionBottomRightPng);

	replayBuf = C2D_TextBufNew(256);
	
	infoPadding = mineSprite.image.subtex->width / 4;
	infoPosition = {0, topScreen.y - mineSprite.image.subtex->height - (infoPadding * 2)};
	infoSize = {topScreen.x, mineSprite.image.subtex->height + (infoPadding * 2)};

	Vector2f digitSize = getDigitSize(_textSize);
	
	mines = CounterDisplay(mineSprite, infoPadding, _textSize, digitSize, 2, {0, infoPosition.y});
	times = CounterDisplay(timeSprite, infoPadding, _textSize, digitSize, 2, {(int)mines.size.x, infoPosition.y});
	infoColor = C2D_Color32f(1.f,1.f,1.f,0.5f);
	replayMode = false;

	initReplayText(0, "");
}

Details::~Details()
{
	;
}

Vector2f Details::getDigitSize(float _textSize)
{
	C2D_Text text;
	C2D_TextBuf buf = C2D_TextBufNew(256);
	
	Vector2f digitSize = { 0, 0 };
	
	C2D_TextParse(&text, buf, "0");
	C2D_TextGetDimensions(&text, _textSize, _textSize, &digitSize.x, &digitSize.y);

	return digitSize;
}

void Details::initReplayText(int _finalTime, std::string _name)
{
	std::string replayString =
		"You are currently in replay mode. Press B to exit replay mode. \n"
		"    Time: " + std::to_string(_finalTime) + "    Player: " + _name;

	C2D_TextParse(&replayText, replayBuf, replayString.c_str());
}

void Details::draw()
{
	C2D_DrawRectSolid(
		infoPosition.x,
		infoPosition.y,
		0,
		infoSize.x,
		infoSize.y,
		infoColor
	);

	if (replayMode)
	{
		float replayTextSize = 0.5f;
		Vector2i replaySize = {
			topScreen.x,
			getDigitSize(replayTextSize).y * 2
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
			C2D_Color32f(0.f,0.f,1.f,0.2f)
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

	textPanel.draw({infoPadding, infoPadding});
	mines.draw();
	times.draw();
}

void Details::drawSelection(int _selection)
{
	// textpanel textsize
	Vector2f selectionDigitSize = getDigitSize(0.5f);

	Vector2f selectionBoxSize = {
		topScreen.x - 60,
		selectionDigitSize.y * 2
	};
	Vector2f selectionBoxPosition = {
		0,
		selectionDigitSize.y * (_selection + 2)
	};

	C2D_SpriteSetPos(
		&selectionTopLeft,
		selectionBoxPosition.x,
		selectionBoxPosition.y
	);

	C2D_SpriteSetPos(
		&selectionBottomLeft,
		selectionBoxPosition.x,
		selectionBoxPosition.y + selectionBoxSize.y - selectionDigitSize.y
	);

	C2D_SpriteSetPos(
		&selectionTopRight,
		selectionBoxPosition.x + selectionBoxSize.x - selectionDigitSize.x,
		selectionBoxPosition.y
	);

	C2D_SpriteSetPos(
		&selectionBottomRight,
		selectionBoxPosition.x + selectionBoxSize.x - selectionDigitSize.x,
		selectionBoxPosition.y + selectionBoxSize.y - selectionDigitSize.y
	);

	C2D_DrawSprite(&selectionTopLeft);
	C2D_DrawSprite(&selectionBottomLeft);
	C2D_DrawSprite(&selectionTopRight);
	C2D_DrawSprite(&selectionBottomRight);
}

void Details::update(u32 _color, int _minesRemaining, int _currentTime)
{
	infoColor = _color;

	mines.update(_minesRemaining);
	times.update(_currentTime);
}