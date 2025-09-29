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
	ReplayPlayer(C2D_SpriteSheet& _sheet, Difficulty _difficulty);
	void start(Score _score);
	void draw();
	int getTime();
	int getMinesPlaced();
	bool finished;
	bool playing;
private:
	Score score;
	Maps replayMap;

	int movesDone;
	int minesPlaced;
	int minesCount;

	Vector2i dimentions;

	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::duration<double, std::milli> elapsedTime;
};