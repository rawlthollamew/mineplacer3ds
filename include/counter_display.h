#pragma once

#include <3ds.h>
#include <citro2d.h>
#include "utils.h"

class CounterDisplay
{
public:
	CounterDisplay();
	CounterDisplay(C2D_Sprite _icon, int _padding, float _textSize, Vector2f _digitSize, int _digits, Vector2i _position);
	void draw();
	void update(int _value);
	Vector2f size;
private:
	int defaultDigits;
	std::string valueString;

	float textSize;
	int padding;
	Vector2f digitSize;
	C2D_TextBuf counterBuffer;

	void drawText();
	void drawBackground();

	C2D_Sprite icon;
	Vector2i position;
};