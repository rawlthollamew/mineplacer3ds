#include "replay_player.h"

ReplayPlayer::ReplayPlayer(C2D_SpriteSheet _sheet)
	: replayMap(_sheet)
{
	for (int i = 0; i < 4; i++)
	{
		imageTint.corners[i].blend = 0.5f;
		imageTint.corners[i].color = C2D_Color32f(1.f,0.f,0.f,1.f);
	}
	
	replayMap.initMaps();

	init();
}

void ReplayPlayer::init()
{
	started = false;
	startTime = std::chrono::high_resolution_clock::now();
	elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
	movesDone = 0;
}

void ReplayPlayer::draw(Score _score)
{
	if (!started)
	{
		startTime = std::chrono::high_resolution_clock::now();
		elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
		
		replayMap.mineMap = _score.mineMap;
		replayMap.initGeneratedMap();
		movesDone = 0;
		
		started = true;
	}

	elapsedTime = std::chrono::high_resolution_clock::now() - startTime;

	for (int i = 0; i < (int)_score.moves.size(); i++)
	{
		if (elapsedTime.count() > _score.moves[i].ms && movesDone <= i)
		{
			movesDone += 1;
			replayMap.placeMine(_score.moves[i].position);
		}
	}
	
	replayMap.draw();

	// for (int y = 0; y < replayMap.dimentions.y; y++)
	// {
	// 	for (int x = 0; x < replayMap.dimentions.x; x++)
	// 	{
	// 		if (replayMap.playerMap[y][x])
	// 		{
	// 			C2D_DrawRectSolid(
	// 				replayMap.tileSize * x,
	// 				replayMap.tileSize * y,
	// 				0,
	// 				replayMap.tileSize,
	// 				replayMap.tileSize,
	// 				C2D_Color32f(1.f,1.f,1.f,1.f)
	// 			);
	// 		}
	// 	}
	// }
	

	// draw a tint on top of the replay
	C2D_DrawRectSolid(
		0.f,
		0.f,
		0.f,
		bottomScreen.x,
		bottomScreen.y,
		C2D_Color32f(1.f, 0.f, 0.f, 0.2f)
	);
}