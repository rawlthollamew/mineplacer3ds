#include "counter_display.h"

CounterDisplay::CounterDisplay()
	: CounterDisplay(C2D_Sprite(), 0, 0.f, {0,0}, 0, {0,0})
{
}

CounterDisplay::CounterDisplay(C2D_Sprite _icon, int _padding, float _textSize, Vector2f _digitSize, int _digits, Vector2i _position)
{
	padding = _padding;
	textSize = _textSize;
	digits = _digits;
	icon = _icon;
	position = _position;
	digitSize = _digitSize;

	if (icon.image.subtex)
	{
		size = {
			(float)icon.image.subtex->width + (digitSize.x * digits) + (padding * 2),
			(float)icon.image.subtex->height
		};
	}
	else
	{
		size = { 0.f, 0.f };
	}

	counterBuffer = C2D_TextBufNew(512);
	update(0);
}

void CounterDisplay::update(int _value)
{
	valueString = std::to_string(_value);
	
	if ((int)valueString.length() < digits)
	{
		valueString = std::string(digits - (int)valueString.length(), '0') + valueString;
	}
}

void CounterDisplay::drawBackground()
{
	C2D_SpriteSetPos(&icon, position.x + padding, position.y + padding);
	C2D_DrawSprite(&icon);

	C2D_DrawRectSolid(
		position.x + icon.image.subtex->width + (padding * 2),
		position.y,
		0,
		digitSize.x * digits,
		digitSize.y,
		C2D_Color32f(1.f, 1.f, 1.f, 0.5f)
	);
}

void CounterDisplay::drawText()
{
	C2D_Text counterText;
	C2D_TextBufClear(counterBuffer);
	C2D_TextParse(&counterText, counterBuffer, valueString.c_str());
	C2D_TextOptimize(&counterText);

	// centered text
	Vector2f textOffset = {
		(float)icon.image.subtex->width + (padding * 2),
		(digitSize.y < icon.image.subtex->height) ? ((icon.image.subtex->height - digitSize.y) / 2) : 0
	};
	
	C2D_DrawText(
		&counterText,
		C2D_WithColor,
		position.x + textOffset.x,
		position.y + textOffset.y,
		0,
		textSize,
		textSize,
		C2D_Color32f(1.f, 1.f, 1.f, 1.f)
	);
}

void CounterDisplay::draw()
{
	drawBackground();
	drawText();
}