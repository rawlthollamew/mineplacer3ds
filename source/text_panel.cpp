#include "text_panel.h"

TextPanel::TextPanel()
{
	;
}

TextPanel::TextPanel(float _textSize, C2D_SpriteSheet _sheet, Vector2f _halfDigitSize, Vector2f _digitSize)
{
	textSize = _textSize;
	halfDigitSize = _halfDigitSize;
	digitSize = _digitSize;
	currentScreen = helpScreen;
	mainBuf = C2D_TextBufNew(4096);
	mainText = {0};

	helpString =
		"How to play: \n"
		"        Place down mines using the touch controls.\n"
		"        Each mine will affect the area around it.\n"
		"        Fill the grid so that the tiles are perfectly even.\n"
		"        Hint: start from the outside and work your way in.\n\n"
		"Controls: \n"
		"        L / R: Change main selection (bottom right of top screen)\n"
		"        : Change sub selection (center of top screen)\n"
		"        : Select main current selection (defaulted to new game)\n"
		"        : Select sub current selection (defaulted to easy)\n"
		"        Press START anytime to quit the game.";

	lbString =
		"Leaderboard: \n"
		"        Press up/down to go through scores\n"
		"        Press left/right to go through pages\n"
		"        Press  to watch replay\n\n";
	
	easyDiffBuf = C2D_TextBufNew(256);
	mediumDiffBuf = C2D_TextBufNew(256);
	hardDiffBuf = C2D_TextBufNew(256);

	C2D_TextParse(&easyDiffText, easyDiffBuf, "Easy");
	C2D_TextParse(&mediumDiffText, mediumDiffBuf, "Medium");
	C2D_TextParse(&hardDiffText, hardDiffBuf, "Hard");
	C2D_TextOptimize(&easyDiffText);
	C2D_TextOptimize(&mediumDiffText);
	C2D_TextOptimize(&hardDiffText);

	currentString = helpString;
	setText();

	selection = { 0, 0 };
	C2D_SpriteFromSheet(&selectionTopLeft, _sheet, selectionTopLeftPng);
	C2D_SpriteFromSheet(&selectionTopRight, _sheet, selectionTopRightPng);
	C2D_SpriteFromSheet(&selectionBottomLeft, _sheet, selectionBottomLeftPng);
	C2D_SpriteFromSheet(&selectionBottomRight, _sheet, selectionBottomRightPng);
}

void TextPanel::updatePage(std::vector<Score> _scores, int _change)
{
	int newValue = replayPage + _change;
	
	if (newValue < 0) return;
	if (newValue > (int)((_scores.size() - 1) / 8)) return;

	replayPage = newValue;
	loadLeaderboardText(_scores);
}

void TextPanel::loadLeaderboardText(std::vector<Score> _scores)
{
	lbString =
		"Leaderboard: \n"
		"        Press up/down to go through scores\n"
		"        Press left/right to go through pages\n"
		"        Press  to watch replay\n\n";

	// max of 8 items on the lb.
	// get the minimum between 8 and number of scores there are.
	if (_scores.size() > 0)
	{
		for (int i = replayPage * 8; i < (int)std::min((size_t)(replayPage * 8) + 8, _scores.size()); i++)
		{
			lbString = lbString + std::to_string(i + 1) + ": " + _scores[i].username + "(" + std::to_string(_scores[i].time) + ")\n";
		}
	}
	else if (_scores.size() == 0)
	{
		lbString += "No scores here. Try setting some!";
	}

	setText();
}

void TextPanel::setText()
{
	if (currentScreen == helpScreen) currentString = helpString;
	else if (currentScreen == lbScreen) currentString = lbString;

	C2D_TextBufClear(mainBuf);
	C2D_TextParse(&mainText, mainBuf, currentString.c_str());
	C2D_TextOptimize(&mainText);
}

void TextPanel::updateSelection(std::vector<Score> _scores, Vector2i _change)
{
	Vector2i finalChange = {
		selection.x + _change.x,
		selection.y + _change.y
	};

	if (finalChange.x < 0) finalChange.x = 0;
	if (finalChange.y < 0) finalChange.y = 0;

	if (currentScreen == lbScreen)
	{
		updatePage(_scores, _change.x);

		finalChange.x = 0;
		if (finalChange.x > 1) return;
		if (finalChange.y > 7) return;
		// means that out of bounds not allowed.
		if (replayPage == (int)((_scores.size() - 1) / 8) && finalChange.y > ((int)_scores.size() - 1) - (replayPage * 8)) finalChange.y = ((int)_scores.size() - 1) - (replayPage * 8);
	}
	
	if (currentScreen == helpScreen && finalChange.x > 2) return;
	if (currentScreen == helpScreen && finalChange.y > 0) return;

	selection = finalChange;
}

void TextPanel::draw(Vector2i _position, int _difficultyIndex)
{
	C2D_DrawText(
		&mainText,
		C2D_WithColor,
		_position.x,
		_position.y,
		0,
		textSize,
		textSize,
		C2D_Color32f(1.f, 1.f, 1.f, 1.f)
	);

	if (currentScreen == lbScreen)
	{
		Vector2f selectionBoxSize = {
			(float)(topScreen.x),
			halfDigitSize.y
		};
		Vector2f selectionBoxPosition = {
			selection.x * (selectionBoxSize.x / 2.f),
			halfDigitSize.y * (selection.y + 5) + _position.y
		};

		C2D_SpriteSetPos(
			&selectionTopLeft,
			selectionBoxPosition.x,
			selectionBoxPosition.y
		);

		C2D_SpriteSetPos(
			&selectionBottomLeft,
			selectionBoxPosition.x,
			selectionBoxPosition.y + selectionBoxSize.y
		);

		C2D_SpriteSetPos(
			&selectionTopRight,
			selectionBoxPosition.x + selectionBoxSize.x,
			selectionBoxPosition.y
		);

		C2D_SpriteSetPos(
			&selectionBottomRight,
			selectionBoxPosition.x + selectionBoxSize.x,
			selectionBoxPosition.y + selectionBoxSize.y
		);

		C2D_DrawSprite(&selectionTopLeft);
		C2D_DrawSprite(&selectionBottomLeft);
		C2D_DrawSprite(&selectionTopRight);
		C2D_DrawSprite(&selectionBottomRight);
	}
	else if (currentScreen == helpScreen)
	{
		Vector2f selectionBoxSize = {
			(float)(topScreen.x / 3.f),
			halfDigitSize.y
		};
		Vector2f selectionBoxPosition = {
			selectionBoxSize.x * selection.x,
			topScreen.y - (digitSize.y * 2.f)
		};
		
		C2D_SpriteSetPos(
			&selectionTopLeft,
			selectionBoxPosition.x,
			selectionBoxPosition.y
		);
		
		C2D_SpriteSetPos(
			&selectionBottomLeft,
			selectionBoxPosition.x,
			selectionBoxPosition.y + selectionBoxSize.y
		);
		
		C2D_SpriteSetPos(
			&selectionTopRight,
			selectionBoxPosition.x + selectionBoxSize.x - selectionTopRight.image.subtex->width,
			selectionBoxPosition.y
		);
		
		C2D_SpriteSetPos(
			&selectionBottomRight,
			selectionBoxPosition.x + selectionBoxSize.x - selectionBottomRight.image.subtex->width,
			selectionBoxPosition.y + selectionBoxSize.y
		);
		C2D_DrawRectSolid(
			selectionBoxSize.x * _difficultyIndex,
			selectionBoxPosition.y,
			0,
			selectionBoxSize.x,
			selectionBoxSize.y + selectionTopLeft.image.subtex->width,
			C2D_Color32f(0.f,1.f,1.f,0.5f)
		);
		
		C2D_DrawSprite(&selectionTopLeft);
		C2D_DrawSprite(&selectionBottomLeft);
		C2D_DrawSprite(&selectionTopRight);
		C2D_DrawSprite(&selectionBottomRight);
		
		C2D_DrawText(
			&easyDiffText,
			C2D_WithColor | C2D_AlignCenter,
			topScreen.x / 6.f,
			topScreen.y - (digitSize.y * 2),
			0,
			0.5f,
			0.5f,
			C2D_Color32f(1.f,1.f,1.f,1.f)
		);
		C2D_DrawText(
			&mediumDiffText,
			C2D_WithColor | C2D_AlignCenter,
			topScreen.x / 2.f,
			topScreen.y - (digitSize.y * 2),
			0,
			0.5f,
			0.5f,
			C2D_Color32f(1.f,1.f,1.f,1.f)
		);
		C2D_DrawText(
			&hardDiffText,
			C2D_WithColor | C2D_AlignCenter,
			topScreen.x - (topScreen.x / 6.f),
			topScreen.y - (digitSize.y * 2),
			0,
			0.5f,
			0.5f,
			C2D_Color32f(1.f,1.f,1.f,1.f)
		);
	}
}