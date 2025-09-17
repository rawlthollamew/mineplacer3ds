#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "utils.h"

// return function done in Maps::getColors(TileColors _tileValue)
enum TileColors
{
	Blue = 1,
	Green,
	Red,
	Navy,
	DarkRed,
	Cyan,
	Black,
	Grey
};

class Maps
{
public:
	Maps(C2D_SpriteSheet& _sheet);

	int tileSize = 20;
	int mineCount = 50;
	int minesPlaced = 0;
	Vector2i dimentions = { bottomScreen.x / tileSize, bottomScreen.y / tileSize };

	void generate();
	void draw();
	void placeMine(Vector2i _position);

	bool mapCompleted();
private:
	std::vector<std::vector<int>> generatedMap;
	std::vector<std::vector<bool>> mineMap;
	std::vector<std::vector<bool>> playerMap;

	C2D_TextBuf textBuffer;
	
	// it crashes all the time on actual hardware, need to initalize stuff better.
	C2D_Sprite tileSprite = {0};
	C2D_Sprite revealedSprite = {0};
	C2D_Sprite errorSprite = {0};
	C2D_Sprite flagSprite = {0};

	int surroundingMines(Vector2i _position);
	u32 getColor(TileColors _tileValue);
};