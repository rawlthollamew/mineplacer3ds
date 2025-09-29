#include "settings.h"


Settings::Settings()
{
	int mainTileSize = 20;
	int doubleTileSize = mainTileSize * 2;

	Difficulty newDifficulty;

	newDifficulty.tileSize = doubleTileSize;
	newDifficulty.mineCount = 10;
	newDifficulty.dimentions = { bottomScreen.x / doubleTileSize, bottomScreen.y / doubleTileSize };
	newDifficulty.textSize = 2.f;
	newDifficulty.scaling = 2.f;
	diffs.push_back(newDifficulty);
	
	newDifficulty.tileSize = mainTileSize;
	newDifficulty.mineCount = 50;
	newDifficulty.dimentions = { bottomScreen.x / mainTileSize, bottomScreen.y / mainTileSize };
	newDifficulty.textSize = 1.f;
	newDifficulty.scaling = 1.f;
	diffs.push_back(newDifficulty);
	
	newDifficulty.tileSize = mainTileSize;
	newDifficulty.mineCount = 75;
	newDifficulty.dimentions = { bottomScreen.x / mainTileSize, bottomScreen.y / mainTileSize };
	newDifficulty.textSize = 1.f;
	newDifficulty.scaling = 1.f;
	diffs.push_back(newDifficulty);
}