#pragma once
#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <stdlib.h>
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
	
	void update(int _minesPlaced, int _currentTime);
	void draw();

	TextPanel textPanel;
	Vector2i getInfoPosition() { return infoPosition; }
	int getInfoPadding() { return infoPadding; }
private:
	CounterDisplay mines;
	C2D_Sprite mineSprite;

	CounterDisplay times;
	C2D_Sprite timeSprite;

	Vector2i infoPosition;
	Vector2i infoSize;
	
	int infoPadding;
	int mineCount;
	static Vector2f getDigitSize(float _textSize);
};