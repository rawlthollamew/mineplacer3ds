#pragma once
#include <3ds.h>
#include <citro2d.h>

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "utils.h"

class Button
{
public:
	Button();
	Button(Vector2i _position, C2D_Sprite _sprite, u32 _color, std::string _text, float _textSize);
	
	u32 color;
	Vector2i position;
	Vector2f size;
	float textSize;
	float textHeight;

	// null as default.
	C2D_Sprite sprite;
	C2D_Text text;
	C2D_TextBuf buf;
	
	void initText(std::string _text);
	void draw();
};


class ButtonHandler
{
public:
	int buttonCount = 2;
	int selection = 0;
	bool helpText = true;

	C2D_Sprite selectionTopLeft;
	C2D_Sprite selectionBottomLeft;
	C2D_Sprite selectionTopRight;
	C2D_Sprite selectionBottomRight;

	ButtonHandler(C2D_SpriteSheet _sheet, Vector2i _drawPosition, int _padding, float _textSize);
	void draw();
	void setVector();
private:
	int padding;
	Vector2i drawPosition;

	Button newGameButton;
	Button helpButton;
	Button lbButton;

	std::vector<Button> activeButtons;
};