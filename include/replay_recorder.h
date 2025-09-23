#pragma once

#include <3ds.h>
#include <citro2d.h>
#include <vector>
#include <chrono>
#include "utils.h"

class ReplayRecorder
{
public:
	ReplayRecorder()
	{
		startTime = std::chrono::high_resolution_clock::now();
	}
	void update(Vector2i _position)
	{
		std::chrono::high_resolution_clock::time_point current = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> timeSpan = current - startTime;

		moves.push_back({
			(int)timeSpan.count(),
			_position
		});
	}
	void clear()
	{
		startTime = std::chrono::high_resolution_clock::now();
		moves.clear();
	}
	std::vector<Move> getMoves()
	{
		return moves;
	}
private:
	std::chrono::high_resolution_clock::time_point startTime;
	std::vector<Move> moves;
};