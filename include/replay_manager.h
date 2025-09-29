#pragma once

#include <string>
#include <vector>
#include <dirent.h>
#include <jansson.h>
#include <algorithm>

#include "replay_player.h"
#include "replay_recorder.h"
#include "timer.h"
#include "maps.h"

class ReplayManager
{
public:
	ReplayManager(C2D_SpriteSheet& _sheet, Difficulty _difficulty, std::string _directory);
	ReplayPlayer player;
	ReplayRecorder recorder;

	std::vector<Score> scores;

	void saveReplay(std::vector<Move> _moves, std::vector<std::vector<bool>> _map, int _finalTime);
	void getScores();
private:
	std::string directory;
	
	std::string playerInput(int _finalTime);
	std::string epochToDate(time_t _epoch);
};