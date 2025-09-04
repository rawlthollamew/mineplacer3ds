#pragma once
#include <3ds.h>
#include <citro2d.h>
#include "utils.h"

class Player
{
public:
	Player();
	// void update(Vector2i _mapSize);
	bool update(int _tileSize);
	// void draw(int _tileSize);
	bool tilePlaced;
	bool touchDown;
	Vector2i position;
};