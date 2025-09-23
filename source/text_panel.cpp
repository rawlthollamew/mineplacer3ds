#include "text_panel.h"

TextPanel::TextPanel(float _textSize)
	: textSize(_textSize)
{
	buf = C2D_TextBufNew(4096);
	text = {0};

	helpString = 
		"How to play: \n\n"
		"    Place down mines using the touch controls.\n"
		"    Each mine will affect the area around it.\n"
		"    Fill the grid so that the tiles are perfectly even.\n"
		"    Hint: start from the outside and work your way in.\n\n"
		"Controls: \n\n"
		"    : Select current button (defaulted to new game)\n"
		"    : Change button selection (bottom right of top screen)\n"
		"    Press START anytime to quit the game.";
	
	lbString = "Leaderboard: (press up/down to go through scores)\n\n";
	
	helpText = true;
	currentString = helpString;
	setText();
}

void TextPanel::loadLeaderboardText(std::vector<Score> _scores)
{
	lbString = "Leaderboard: (press up/down to go through scores)\n\n";

	// max of 10 items on the lb.
	// get the minimum between 10 and number of scores there are.
	for (int i = 0; i < (int)std::min((size_t)10, _scores.size()); i++)
	{
		lbString = lbString + std::to_string(i + 1) + ": " + _scores[i].username + "(" + std::to_string(_scores[i].time) + ")\n";
	}

	setText();
}

void TextPanel::setText()
{
	if (helpText) currentString = helpString;
	else if (!helpText) currentString = lbString;

	C2D_TextBufClear(buf);
	C2D_TextParse(&text, buf, currentString.c_str());
	C2D_TextOptimize(&text);
}

void TextPanel::draw(Vector2i _position)
{
	C2D_DrawText(
		&text,
		C2D_WithColor,
		_position.x,
		_position.y,
		0,
		textSize,
		textSize,
		C2D_Color32f(1.f,1.f,1.f,1.f)
	);
}