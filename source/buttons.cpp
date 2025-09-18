#include "buttons.h"

Button::Button()
{
	position = {0,0};
	color = 0;
}

Button::Button(Vector2i _position, C2D_Sprite _sprite, u32 _color, std::string _text, float _textSize)
{
	position = _position;
	sprite = _sprite;
	color = _color;
	textSize = _textSize;

	initText(_text);
}

void Button::initText(std::string _text)
{
	Vector2f textDimentions = { 0, 0 };
	
	C2D_TextBuf buf = C2D_TextBufNew(4096);
	C2D_TextParse(&text, buf, _text.c_str());
	
	C2D_TextGetDimensions(&text, textSize, textSize, &textDimentions.x, &textDimentions.y);
	
	size = {
		(float)sprite.image.subtex->width + textDimentions.x,
		(float)sprite.image.subtex->height
	};

	textHeight = textDimentions.y;
}

void Button::draw()
{
	C2D_DrawRectSolid(
		position.x,
		position.y,
		0,
		size.x,
		size.y,
		color
	);

	C2D_SpriteSetPos(&sprite, position.x, position.y);
	C2D_DrawSprite(&sprite);

	C2D_DrawText(
		&text,
		C2D_WithColor,
		position.x + sprite.image.subtex->width,
		position.y + ((size.y - textHeight) / 2.f),
		0,
		textSize,
		textSize,
		C2D_Color32f(1.f,1.f,1.f,1.f)
	);
}

ButtonHandler::ButtonHandler(C2D_SpriteSheet _sheet, Vector2i _drawPosition, int _padding, float _textSize)
{
	drawPosition = _drawPosition;
	padding = _padding;

	buttonCount = 2;
	selection = 0;
	
	u32 backgroundColor = C2D_Color32f(1.f,1.f,1.f,0.5f);

	C2D_SpriteFromSheet(&selectionLeft, _sheet, selectionLeftPng);
	C2D_SpriteFromSheet(&selectionRight, _sheet, selectionRightPng);

	// adding buttons
	C2D_Sprite currentSprite;

	C2D_SpriteFromSheet(&currentSprite, _sheet, newGamePng);
	newGameButton.textSize = _textSize;
	newGameButton.sprite = currentSprite;
	newGameButton.color = backgroundColor;
	newGameButton.initText("New");
	newGameButton.position = {
		drawPosition.x + currentSprite.image.subtex->width + padding,
		drawPosition.y + padding
	};
	
	C2D_SpriteFromSheet(&currentSprite, _sheet, helpPng);
	helpButton.textSize = _textSize;
	helpButton.sprite = currentSprite;
	helpButton.color = backgroundColor;
	helpButton.initText("Help");
	helpButton.position = {
		drawPosition.x + currentSprite.image.subtex->width + padding,
		drawPosition.y + padding
	};
	
	C2D_SpriteFromSheet(&currentSprite, _sheet, lbPng);
	lbButton.textSize = _textSize;
	lbButton.sprite = currentSprite;
	lbButton.color = backgroundColor;
	lbButton.initText("Scores");
	lbButton.position = {
		drawPosition.x + currentSprite.image.subtex->width + padding,
		drawPosition.y + padding
	};
}


void ButtonHandler::draw()
{
	int totalWidth = 0;
	for (int i = 0; i < (int)activeButtons.size(); i++)
	{
		totalWidth += activeButtons[i].size.x + padding;
	}
	
	int startX = topScreen.x - totalWidth;
	int currentOffset = startX;

	for (int i = 0; i < (int)activeButtons.size(); i++)
	{
		activeButtons[i].position.x = currentOffset;
		activeButtons[i].draw();
		
		if (i == selection)
		{
			C2D_SpriteSetPos(&selectionLeft, activeButtons[i].position.x, activeButtons[i].position.y);
			C2D_DrawSprite(&selectionLeft);

			C2D_SpriteSetPos(&selectionRight, activeButtons[i].position.x + activeButtons[i].size.x - selectionRight.image.subtex->width, activeButtons[i].position.y);
			C2D_DrawSprite(&selectionRight);
		}

		currentOffset += activeButtons[i].size.x + padding;
	}
}

void ButtonHandler::setVector(bool _helpText)
{
	activeButtons = {
		newGameButton,
		_helpText ? lbButton : helpButton
	};
}