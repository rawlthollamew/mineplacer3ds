#include "player.h"

Player::Player()
{
	// init position
	position = { 0, 0 };
	lastPosition = { -1, -1 };
	tilePlaced = false;
}

void Player::update(int tileSize)
{
	// u32 kDown = hidKeysDown();

	// if (kDown & KEY_A)			tilePlaced = true;
	// if (!(kDown & KEY_A))		tilePlaced = false;
	// if (kDown & KEY_LEFT)		position.x -= 1;
	// if (kDown & KEY_RIGHT)		position.x += 1;
	// if (kDown & KEY_UP)			position.y -= 1;
	// if (kDown & KEY_DOWN)		position.y += 1;

	// position.x = C2D_Clamp(position.x, 0, _mapSize.x - 1);
	// position.y = C2D_Clamp(position.y, 0, _mapSize.y - 1);

	touchPosition touch;
	hidTouchRead(&touch);

	if (touch.px == 0.f && touch.py == 0.f)
	{
		tilePlaced = false;
		lastPosition = { -1, -1 };
		return;
	}

	if (lastPosition.x == -1 && lastPosition.y == -1)
	{
		lastPosition.x = touch.px;
		lastPosition.x = touch.py;
	}
	
	if (touch.px != lastPosition.x || touch.py != lastPosition.y)
	{
		Vector2i mapPosition = { touch.px / tileSize, touch.py / tileSize };

		lastPosition.x = touch.px;
		lastPosition.y = touch.py;
		position = mapPosition;
		tilePlaced = true;
	}
	else
	{
		tilePlaced = false;
	}

	printf("Touch: {%i, %i, Position: {%i, %i}, lastPosition: {%i, %i} \n", touch.px, touch.py, position.x, position.y, lastPosition.x, lastPosition.y);
}

// void Player::draw(int _tileSize)
// {
// 	C2D_DrawRectSolid(position.x * _tileSize, position.y * _tileSize, 0, _tileSize, _tileSize, C2D_Color32f(1.f,1.f,1.f,0.5f));
// }