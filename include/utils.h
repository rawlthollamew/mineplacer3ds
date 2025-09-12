#pragma once

#include <stdio.h>
#include <3ds.h>
#include <citro2d.h>
#include <string>

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
	selectionLeftPng,
	selectionRightPng
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

const Vector2i topScreen = {400, 240};
const Vector2i bottomScreen = {320, 240};