#include "replay_player.h"

ReplayPlayer::ReplayPlayer(C2D_SpriteSheet _sheet)
	: replayMap(_sheet)
{
	score = Score{"", 0, {}, {}};
	replayMap.initMaps();
	
	finished = false;
	playing = false;

	startTime = std::chrono::high_resolution_clock::now();
	elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
	movesDone = 0;
}

void ReplayPlayer::start(Score _score)
{
	score = _score;
	startTime = std::chrono::high_resolution_clock::now();
	elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
	
	replayMap.initMaps();
	replayMap.mineMap = _score.mineMap;
	replayMap.initGeneratedMap();
	movesDone = 0;
	
	finished = false;
	playing = true;
}

void ReplayPlayer::draw()
{
	if (!finished)
	{
		elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
		
		if (movesDone < (int)score.moves.size())
		{
			int ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count();

			if (ms >= score.moves[movesDone].ms)
			{
				replayMap.placeMine(score.moves[movesDone].position);
				movesDone += 1;
			}
		}
		else finished = true;
	}
	
	replayMap.draw();

	// draw a tint on top of the replay
	C2D_DrawRectSolid(
		0.f,
		0.f,
		0.f,
		bottomScreen.x,
		bottomScreen.y,
		C2D_Color32f(0.f, 0.f, 1.f, 0.1f)
	);
}

int ReplayPlayer::getTime()
{
	if (!finished) return (int)(elapsedTime.count() / 1000);
	else return score.time;
}

int ReplayPlayer::getMineCount()
{
	if (!finished)
	{
		int minesPlaced = 0;

		for (int y = 0; y < replayMap.dimentions.y; y++)
		{
			for (int x = 0; x < replayMap.dimentions.x; x++)
			{
				if (replayMap.playerMap[y][x]) minesPlaced += 1;
			}
		}
		
		return replayMap.mineCount - minesPlaced;
	}
	else return 0;
}