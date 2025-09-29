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
	Maps(C2D_SpriteSheet& _sheet, Difficulty _difficulty);

	int minesPlaced = 0;

	void clearMaps();
	void generate();
	void initGeneratedMap();
	void draw();
	void placeMine(Vector2i _position);

	bool mapCompleted();

	std::vector<std::vector<int>> generatedMap;
	std::vector<std::vector<bool>> mineMap;
	std::vector<std::vector<bool>> playerMap;
private:
	int mineCount;
	int tileSize;
	float scaling;
	float textSize;
	Vector2i dimentions;

	C2D_TextBuf textBuffer;
	
	C2D_Sprite tileSprite;
	C2D_Sprite revealedSprite;
	C2D_Sprite errorSprite;
	C2D_Sprite flagSprite;

	int surroundingMines(Vector2i _position);
	u32 getColor(TileColors _tileValue);
};