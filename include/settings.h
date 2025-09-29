#pragma once

#include <vector>
#include "utils.h"

// hardcoded diffs.

class Settings
{
public:
	static std::vector<Difficulty> diffs;
	static Difficulty easyDiff;
	static Difficulty mediumDiff;
	static Difficulty hardDiff;
};