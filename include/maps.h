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
	Maps();

	int tileSize = 20;
	int mineCount = 50;
	Vector2i dimentions = { bottomScreen.x / tileSize, bottomScreen.y / tileSize };

	void generate();
	void draw();
	void placeMine(Vector2i _position);
private:
	std::vector<std::vector<int>> generatedMap;
	std::vector<std::vector<bool>> mineMap;
	std::vector<std::vector<bool>> playerMap;

	C2D_TextBuf textBuffer;
	
	C2D_SpriteSheet sheet;
	C2D_Sprite tileSprite;
	C2D_Sprite revealedSprite;
	C2D_Sprite errorSprite;
	C2D_Sprite flagSprite;

	int surroundingMines(Vector2i _position);
	u32 getColor(TileColors _tileValue);
};