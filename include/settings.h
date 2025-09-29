#pragma once

#include <vector>
#include "utils.h"

// hardcoded diffs.

class Settings
{
public:
	int current = 0;
	int dificultyCount = 3;
	Settings();
	std::vector<Difficulty> diffs;
};