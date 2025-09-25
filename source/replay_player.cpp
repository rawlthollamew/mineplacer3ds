#include "replay_player.h"

ReplayPlayer::ReplayPlayer(C2D_SpriteSheet& _sheet, int _tileSize, int _mineCount, Vector2i _dimentions)
	: replayMap(_sheet, _tileSize, _mineCount, _dimentions)
{
	score = Score{"", 0, {}, {}};
	dimentions = _dimentions;
	replayMap.clearMaps();
	
	finished = false;
	playing = false;

	startTime = std::chrono::high_resolution_clock::now();
	elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
	movesDone = 0;
	minesPlaced = 0;
	minesCount = _mineCount;
}

void ReplayPlayer::start(Score _score)
{
	score = _score;
	startTime = std::chrono::high_resolution_clock::now();
	elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
	
	replayMap.clearMaps();
	replayMap.mineMap = _score.mineMap;
	replayMap.initGeneratedMap();
	movesDone = 0;
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
				minesPlaced = 0;

				for (int y = 0; y < dimentions.y; y++)
				{
					for (int x = 0; x < dimentions.x; x++)
					{
						if (replayMap.playerMap[y][x]) minesPlaced += 1;
					}
				}

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

int ReplayPlayer::getMinesPlaced()
{
	if (!finished) return minesPlaced;
	else return minesCount;
}