#include "details.h"

Details::Details(C2D_SpriteSheet _sheet, int _mineCount, Vector2i _dimentions, float _textSize)
	: textPanel(_textSize / 2.f), mineCount(_mineCount)
{
	C2D_SpriteFromSheet(&mineSprite, _sheet, flagPng);
	C2D_SpriteFromSheet(&timeSprite, _sheet, clockPng);
	
	infoPadding = mineSprite.image.subtex->width / 4;
	infoPosition = {0, topScreen.y - mineSprite.image.subtex->height - (infoPadding * 2)};
	infoSize = {topScreen.x, mineSprite.image.subtex->height + (infoPadding * 2)};

	Vector2f digitSize = getDigitSize(_textSize);
	
	mines = CounterDisplay(mineSprite, infoPadding, _textSize, digitSize, 2, {0, infoPosition.y});
	times = CounterDisplay(timeSprite, infoPadding, _textSize, digitSize, 2, {mines.size.x, infoPosition.y});
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

void Details::draw()
{
	C2D_DrawRectSolid(
		infoPosition.x,
		infoPosition.y,
		0,
		infoSize.x,
		infoSize.y,
		C2D_Color32f(1.f,1.f,1.f,0.5f)
	);

	textPanel.draw({infoPadding, infoPadding});
	mines.draw();
	times.draw();
}

void Details::update(int _minesRemaining, int _currentTime)
{
	mines.update(_minesRemaining);
	times.update(_currentTime);
}