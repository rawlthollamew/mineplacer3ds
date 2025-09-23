#pragma once

#include <3ds.h>
#include <citro2d.h>
#include <vector>
#include <chrono>

#include "maps.h"
#include "utils.h"

class ReplayPlayer
{
public:
	ReplayPlayer(C2D_SpriteSheet _sheet);
	void init();
	void draw(Score _score);
private:
	Maps replayMap;

	bool started;
	int movesDone;
	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::duration<double, std::milli> elapsedTime;
	
	C2D_ImageTint imageTint;
};