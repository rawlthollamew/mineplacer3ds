#include "settings.h"


Settings::Settings()
{
	int mainTileSize = 20;
	int doubleTileSize = mainTileSize * 2;

	Difficulty newDifficulty;

	newDifficulty.tileSize = doubleTileSize;
	newDifficulty.mineCount = 20;
	newDifficulty.dimentions = { bottomScreen.x / doubleTileSize, bottomScreen.y / doubleTileSize };
	diffs.push_back(newDifficulty);
	
	newDifficulty.tileSize = mainTileSize;
	newDifficulty.mineCount = 50;
	newDifficulty.dimentions = { bottomScreen.x / mainTileSize, bottomScreen.y / mainTileSize };
	diffs.push_back(newDifficulty);
	
	newDifficulty.tileSize = mainTileSize;
	newDifficulty.mineCount = 75;
	newDifficulty.dimentions = { bottomScreen.x / mainTileSize, bottomScreen.y / mainTileSize };
	diffs.push_back(newDifficulty);
}