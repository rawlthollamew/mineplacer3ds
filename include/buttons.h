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
	Button(Vector2i _position, C2D_Sprite _foreground, C2D_Sprite _background);
	
	Vector2i position;
	C2D_Sprite background;
	C2D_Sprite foreground;
	
	void draw();
};


class ButtonHandler
{
public:
	int buttonCount;
	int selection;
	C2D_Sprite selectionSprite;

	std::vector<Button> buttons;
	ButtonHandler(C2D_SpriteSheet _sheet, Vector2i _drawPosition);
	void draw();
private:
	Vector2i drawPosition;
};