#pragma once

#include <3ds.h>
#include <citro2d.h>
#include <vector>

#include "utils.h"

enum Screens
{
	helpScreen,
	lbScreen
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
	void draw(Vector2i _position, int _difficultyIndex);
	void updateSelection(std::vector<Score> _scores, Vector2i _change);
private:
	C2D_TextBuf mainBuf;
	C2D_Text mainText;

	C2D_TextBuf easyDiffBuf;
	C2D_Text easyDiffText;
	C2D_TextBuf mediumDiffBuf;
	C2D_Text mediumDiffText;
	C2D_TextBuf hardDiffBuf;
	C2D_Text hardDiffText;

	Vector2f halfDigitSize;
	Vector2f digitSize;
	
	C2D_Sprite selectionTopLeft;
	C2D_Sprite selectionTopRight;
	C2D_Sprite selectionBottomLeft;
	C2D_Sprite selectionBottomRight;

	std::string helpString;
	std::string lbString;
	std::string currentString;
	float textSize;
};