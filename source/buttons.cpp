#include "buttons.h"

Button::Button(Vector2i _position, C2D_Sprite _foreground, C2D_Sprite _background)
	: position(_position)
{
	foreground = _foreground;
	background = _background;
}

void Button::draw()
{
	C2D_SpriteSetPos(&background, position.x, position.y);
	C2D_SpriteSetPos(&foreground, position.x, position.y);

	C2D_DrawSprite(&background);
	C2D_DrawSprite(&foreground);
}

ButtonHandler::ButtonHandler(C2D_SpriteSheet _sheet, Vector2i _drawPosition)
	: drawPosition(_drawPosition)
{
	buttonCount = 2;
	selection = 0;

	C2D_SpriteFromSheet(&selectionSprite, _sheet, selectionPng);

	// add buttons to vector
	C2D_Sprite currentForeground;
	C2D_Sprite currentBackground;

	C2D_SpriteFromSheet(&currentBackground, _sheet, tilePng);
	C2D_SpriteFromSheet(&currentForeground, _sheet, newGamePng);
	
	Button newButton(drawPosition, currentForeground, currentBackground);
	buttons.push_back(newButton);
	
	C2D_SpriteFromSheet(&currentForeground, _sheet, helpPng);
	
	newButton.foreground = currentForeground;
	newButton.position = {drawPosition.x + (int)(currentForeground.image.subtex->width * 1.5f), drawPosition.y};
	buttons.push_back(newButton);
}


void ButtonHandler::draw()
{
	for (int i = 0; i < (int)(buttons.size()); i++)
	{
		buttons[i].draw();
		
		if (i == selection)
		{
			C2D_SpriteSetPos(&selectionSprite, buttons[i].position.x, buttons[i].position.y);
			C2D_DrawSprite(&selectionSprite);
		}
	}
}