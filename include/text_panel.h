#pragma once

#include <3ds.h>
#include <citro2d.h>
#include <vector>

#include "utils.h"

enum Screens
{
	helpScreen,
	lbScreen,
	settingsScreen
};

class TextPanel
{
public:
	TextPanel();
	TextPanel(float _textSize, C2D_SpriteSheet _sheet, Vector2f _halfDigitSize, Vector2f _digitSize);
	Screens currentScreen;
	Vector2i selection;
	void setText();
	int replayPage = 0;
	void updatePage(std::vector<Score> _scores, int _change);
	void loadLeaderboardText(std::vector<Score> _scores);
	void draw(Vector2i _position);
	void updateSelection(std::vector<Score> _scores, Vector2i _change);
private:
	C2D_TextBuf mainBuf;
	C2D_Text mainText;

	Vector2f halfDigitSize;
	Vector2f digitSize;
	
	C2D_Sprite selectionTopLeft;
	C2D_Sprite selectionTopRight;
	C2D_Sprite selectionBottomLeft;
	C2D_Sprite selectionBottomRight;

	std::string helpString;
	std::string lbString;
	std::string settingsString;
	std::string currentString;
	float textSize;
};