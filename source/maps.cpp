#include "maps.h"

Maps::Maps(C2D_SpriteSheet& _sheet, int _tileSize, int _mineCount, Vector2i _dimentions)
{
	
	textBuffer = C2D_TextBufNew(4096);
	tileSize = _tileSize;
	mineCount = _mineCount;
	dimentions = _dimentions;
	
	clearMaps();

	C2D_SpriteFromSheet(&tileSprite, _sheet, tilePng);
	C2D_SpriteFromSheet(&revealedSprite, _sheet, revealedPng);
	C2D_SpriteFromSheet(&errorSprite, _sheet, errorPng);
	C2D_SpriteFromSheet(&flagSprite, _sheet, flagPng);
}

void Maps::clearMaps()
{
	mineMap.assign(dimentions.y, std::vector<bool>(dimentions.x, false));
	playerMap.assign(dimentions.y, std::vector<bool>(dimentions.x, false));
	generatedMap.assign(dimentions.y, std::vector<int>(dimentions.x, 0));
}

int Maps::surroundingMines(Vector2i _position)
{
	int totalMines = 0;

	std::vector<Vector2i> moves = {
		{0,0}, {1,0}, {-1,0}, {0,1}, {0,-1},
		{-1,1}, {1,1}, {1,-1}, {-1,-1}
	};

	for (Vector2i currentMove : moves)
	{
		Vector2i currentPosition = {_position.x + currentMove.x, _position.y + currentMove.y};
		
		if (currentPosition.x >= 0 && currentPosition.x < dimentions.x &&
			currentPosition.y >= 0 && currentPosition.y < dimentions.y)
		{
			if (mineMap[currentPosition.y][currentPosition.x]) totalMines += 1;
		}
	}

	return totalMines;
}

void Maps::initGeneratedMap()
{
	for (int x = 0; x < dimentions.x; x++)
	{
		for (int y = 0; y < dimentions.y; y++)
		{
			generatedMap[y][x] = surroundingMines({x, y});
		}
	}
}

void Maps::generate()
{
	srand(time(0));
	minesPlaced = 0;
	clearMaps();

	while (minesPlaced < mineCount)
	{
		int x = rand() % dimentions.x;
		int y = rand() % dimentions.y;

		if (!mineMap[y][x])
		{
			mineMap[y][x] = true;
			minesPlaced += 1;
		}
	}

	initGeneratedMap();
}

void Maps::draw()
{
	// Kept this at 8 but snprintf gives a warning saying that the output byte count for any one characteris 2 to 11 (thank you sprintf)
	char buf[16];
	C2D_Text dynamicText;
	
	for (int x = 0; x < dimentions.x; x++)
	{
		for (int y = 0; y < dimentions.y; y++)
		{
			C2D_TextBufClear(textBuffer);
			
			u32 currentTileColor = getColor(static_cast<TileColors>(generatedMap[y][x]));
			
			// C2D_DrawRectSolid(x * tileSize, y * tileSize, 0, tileSize, tileSize, C2D_Color32f(1.f,1.f,1.f,0.5f));

			C2D_Sprite currentSprite;
			
			if (generatedMap[y][x] > 0) currentSprite = revealedSprite;
			else if (generatedMap[y][x] <= 0) currentSprite = tileSprite;
			
			C2D_SpriteSetCenter(&currentSprite, 0.f, 0.f);
			C2D_SpriteSetPos(&currentSprite, x * tileSize, y * tileSize);
			C2D_SpriteSetScale(&currentSprite, 1.f, 1.f);
			C2D_DrawSprite(&currentSprite);
			
			if (playerMap[y][x])
			{
				C2D_SpriteSetCenter(&flagSprite, 0.f, 0.f);
				C2D_SpriteSetPos(&flagSprite, x * tileSize, y * tileSize);
				C2D_SpriteSetScale(&flagSprite, 1.f, 1.f);
				C2D_DrawSprite(&flagSprite);
			}
			
			if (generatedMap[y][x] < 0)
			{
				C2D_SpriteSetCenter(&errorSprite, 0.f, 0.f);
				C2D_SpriteSetPos(&errorSprite, x * tileSize, y * tileSize);
				C2D_SpriteSetScale(&errorSprite, 1.f, 1.f);
				C2D_DrawSprite(&errorSprite);

			}
			else if (generatedMap[y][x] > 0)
			{	
				if (playerMap[y][x])
				{
					Vector2i textPosition = { (int)(x * tileSize + (tileSize * 0.6f)), (int)(y * tileSize + (tileSize * 0.4f)) };

					snprintf(buf, sizeof(buf), "%i", generatedMap[y][x]);
					C2D_TextParse(&dynamicText, textBuffer, buf);
					C2D_TextOptimize(&dynamicText);
					C2D_DrawText(&dynamicText, C2D_WithColor, textPosition.x, textPosition.y, 0.f, 0.5f, 0.5f, currentTileColor);
				}
				else if (!playerMap[y][x])
				{
					Vector2i textPosition = { (int)(x * tileSize + (tileSize * 0.25f)), (int)(y * tileSize + (tileSize * 0.1f)) };
					
					snprintf(buf, sizeof(buf), "%i", generatedMap[y][x]);
					C2D_TextParse(&dynamicText, textBuffer, buf);
					C2D_TextOptimize(&dynamicText);
					C2D_DrawText(&dynamicText, C2D_WithColor, textPosition.x, textPosition.y, 0.f, 0.75f, 0.75f, currentTileColor);
				}
			}
			
			// if (mineMap[y][x]) C2D_DrawRectSolid(x * tileSize, y * tileSize, 0, tileSize, tileSize, C2D_Color32f(0.f,1.f,1.f,0.5f));
		}
	}
}

void Maps::placeMine(Vector2i _position)
{
	if (_position.x >= 0 && _position.x < dimentions.x &&
		_position.y >= 0 && _position.y < dimentions.y)
	{
		playerMap[_position.y][_position.x] = !playerMap[_position.y][_position.x];
	
		std::vector<Vector2i> moves = {
			{0,0}, {1,0}, {-1,0}, {0,1}, {0,-1},
			{-1,1}, {1,1}, {1,-1}, {-1,-1}
		};
	
		for (Vector2i currentMove : moves)
		{
			Vector2i currentPosition = {_position.x + currentMove.x, _position.y + currentMove.y};
	
			if (currentPosition.x >= 0 && currentPosition.x < dimentions.x &&
				currentPosition.y >= 0 && currentPosition.y < dimentions.y)
			{
				if (playerMap[_position.y][_position.x]) generatedMap[currentPosition.y][currentPosition.x] -= 1;
				else if (!playerMap[_position.y][_position.x]) generatedMap[currentPosition.y][currentPosition.x] += 1;
			}
		}
	}

}

bool Maps::mapCompleted()
{
	int count = 0;
	int minesPlacedCount = 0;

	for (int x = 0; x < dimentions.x; x++)
	{
		for (int y = 0; y < dimentions.y; y++)
		{
			// sometimes negative cancels out count to be 0.
			count += abs(generatedMap[y][x]);
			minesPlacedCount += playerMap[y][x];
		}
	}

	minesPlaced = minesPlacedCount;

	if (count == 0 && minesPlacedCount == mineCount) return true;
	else return false;
}

u32 Maps::getColor(TileColors _tileValue)
{
	// Jeepers
	if (_tileValue == Blue) 			return C2D_Color32f(0.f,0.f,1.f,1.f);
	else if (_tileValue == Green) 		return C2D_Color32f(0.f,1.f,0.f,1.f);
	else if (_tileValue == Red) 		return C2D_Color32f(1.f,0.f,0.f,1.f);
	else if (_tileValue == Navy) 		return C2D_Color32f(0.f,0.f,0.5f,1.f);
	else if (_tileValue == DarkRed) 	return C2D_Color32f(0.4f,0.2f,0.2f,1.f);
	else if (_tileValue == Cyan) 		return C2D_Color32f(0.1f,0.4f,0.5f,1.f);
	else if (_tileValue == Black) 		return C2D_Color32f(0.f,0.f,0.f,1.f);
	else if (_tileValue == Grey) 		return C2D_Color32f(0.f,0.f,0.f,0.5f);
	else return C2D_Color32f(1.f,1.f,1.f,1.f);
}