#pragma once
#include <3ds.h>
#include <citro2d.h>
#include <string.h>
#include <optional>

#include "utils.h"
#include "maps.h"
#include "details.h"
#include "buttons.h"
#include "timer.h"
#include "replay_manager.h"
#include "settings.h"

class Game
{
public:
	Game();
	~Game();
	void run();
private:
	void init();
	void handleInput();
	void update();
	void render();

	void onGameCompleted();
	void onReplaySelected();
	void onDifficultyChanged();

	C3D_RenderTarget* top;
	C3D_RenderTarget* bottom;
	u32 clrBlack;
	C2D_SpriteSheet sheet;

	int difficultyIndex;
	Difficulty activeDifficulty;
	bool submittedTime;

	std::optional<Maps> map;
	std::optional<Details> details;
	std::optional<ButtonHandler> buttons;
	std::optional<ReplayManager> replays;
	std::optional<Timer> timer;
};