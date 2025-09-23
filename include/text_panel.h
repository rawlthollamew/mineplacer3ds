#pragma once

#include <3ds.h>
#include <citro2d.h>
#include <vector>

#include "utils.h"

class TextPanel
{
public:
	TextPanel(float _textSize);
	void setText();
	void draw(Vector2i _position);
	void loadLeaderboardText(std::vector<Score> _scores);
	bool helpText;
private:
	C2D_TextBuf buf;
	C2D_Text text;
	std::string helpString;
	std::string lbString;
	std::string currentString;
	float textSize;
};