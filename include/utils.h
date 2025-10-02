#pragma once

#include <3ds.h>
#include <citro2d.h>
#include <string>
#include <vector>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

enum PngOrder
{
	tilePng,
	revealedPng,
	errorPng,
	flagPng,
	clockPng,
	newGamePng,
	helpPng,
	lbPng,
	selectionPng,
	configPng
};

struct Vector2f
{
	float x;
	float y;
};

struct Vector2i
{
	int x;
	int y;
};

inline bool operator==(Vector2i& _a, Vector2i& _b)
{
	return _a.x == _b.x && _a.y == _b.y;
}
inline bool operator!=(Vector2i& _a, Vector2i& _b)
{
	return _a.x != _b.x || _a.y != _b.y;
}

const Vector2i topScreen = {400, 240};
const Vector2i bottomScreen = {320, 240};

struct Move
{
	int ms;
	Vector2i position;
};
struct Difficulty
{
	float textSize;
	int tileSize;
	int mineCount;
	Vector2i dimentions;
	float scaling;
};

struct Score
{
	std::string username;
	int time;

	std::vector<Move> moves;
	Difficulty difficulty;
	std::vector<std::vector<bool>> mineMap;
};

inline void DrawSelection(C2D_Sprite _selection, Vector2f _position, Vector2f _size)
{
	C2D_SpriteSetRotationDegrees(&_selection, 0.f);
	C2D_SpriteSetPos(&_selection, _position.x, _position.y);
	C2D_DrawSprite(&_selection);
	C2D_SpriteSetRotationDegrees(&_selection, 90.f);
	C2D_SpriteSetPos(&_selection, _position.x + _size.x, _position.y);
	C2D_DrawSprite(&_selection);
	C2D_SpriteSetRotationDegrees(&_selection, 180.f);
	C2D_SpriteSetPos(&_selection, _position.x + _size.x, _position.y + _size.y);
	C2D_DrawSprite(&_selection);
	C2D_SpriteSetRotationDegrees(&_selection, 270.f);
	C2D_SpriteSetPos(&_selection, _position.x, _position.y + _size.y);
	C2D_DrawSprite(&_selection);
}