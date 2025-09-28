#pragma once

#include <vector>
#include "utils.h"

struct Difficulty
{
	int tileSize;
	int mineCount;
	Vector2i dimentions;
};

// hardcoded diffs.

class Settings
{
public:
	int dificultyCount = 3;
	Settings();
	Difficulty getDifficulty(int _diff)
	{
		return diffs[_diff];
	}
private:
	std::vector<Difficulty> diffs;
};