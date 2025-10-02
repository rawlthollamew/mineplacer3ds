#pragma once
#include <3ds.h>
#include <citro2d.h>
#include <string>
#include <vector>

#include "utils.h"
#include "counter_display.h"
#include "text_panel.h"
#include "timer.h"

// buttons
// new map with options (mine count, width, height, walls)

class Details
{
public:
	Details(C2D_SpriteSheet _sheet, int _mineCount, Vector2i _dimentions, float _textSize);
	~Details();
	
	bool replayMode;

	void update(u32 _color, int _minesPlaced, int _currentTime);
	void draw(int _difficultyIndex);

	TextPanel textPanel;
	Vector2f getInfoPosition() { return infoPosition; }
	int getInfoPadding() { return infoPadding; }
	void initReplayText(int _finalTime, std::string _name);
private:
	u32 infoColor;

	CounterDisplay mines;
	C2D_Sprite mineSprite;

	CounterDisplay times;
	C2D_Sprite timeSprite;

	Vector2f infoPosition;
	Vector2i infoSize;

	C2D_Text replayText;
	C2D_TextBuf replayBuf;
	
	int infoPadding;
	int mineCount;
	Vector2f halfDigitSize;
	Vector2f digitSize;
};