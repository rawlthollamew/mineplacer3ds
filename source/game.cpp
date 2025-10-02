#include "game.h"

Game::Game()
{
	run();
}

Game::~Game()
{
	;
}

void Game::init()
{
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	romfsInit();
	C2D_Prepare();

	top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	clrBlack = C2D_Color32f(0.f, 0.f, 0.f, 0.f);

	sheet = C2D_SpriteSheetLoad("romfs:/gfx/textures.t3x");

	difficultyIndex = 0;
	activeDifficulty = Settings::diffs[difficultyIndex];

	map.emplace(sheet, activeDifficulty);
	map->generate();

	details.emplace(sheet, activeDifficulty.mineCount, activeDifficulty.dimentions, 1.f);
	buttons.emplace(sheet, details->getInfoPosition(), details->getInfoPadding(), 0.75f);

	replays.emplace(sheet, activeDifficulty, "sdmc:/mineplacer/");
	details->textPanel.loadLeaderboardText(replays->scores);

	timer.emplace();

	buttons->setVector();
	submittedTime = false;
}

void Game::onGameCompleted()
{
	timer->stop();

	if (!submittedTime)
	{
		replays->saveReplay(replays->recorder.getMoves(), map->mineMap, timer->getTime(), activeDifficulty);

		replays->getScores(activeDifficulty);
		details->textPanel.loadLeaderboardText(replays->scores);
		submittedTime = true;
	}
}

void Game::handleInput()
{
	u32 kDown = hidKeysDown();

	touchPosition touch;
	hidTouchRead(&touch);

	if (kDown & KEY_START) return;
	else if ((kDown & KEY_TOUCH) && !map->mapCompleted())
	{
		Vector2i mapPosition = {
			(int)(touch.px / activeDifficulty.tileSize),
			(int)(touch.py / activeDifficulty.tileSize)
		};

		map->placeMine(mapPosition);
		replays->recorder.update(mapPosition);
	}
	else if (kDown & KEY_DOWN) details->textPanel.updateSelection(replays->scores, {0, 1});
	else if (kDown & KEY_LEFT) details->textPanel.updateSelection(replays->scores, {-1, 0});
	else if (kDown & KEY_RIGHT) details->textPanel.updateSelection(replays->scores, {1, 0});
	else if (kDown & KEY_L)
	{
		buttons->selection -= 1;
		if (buttons->selection < 0) buttons->selection = 0;
	}
	else if (kDown & KEY_R)
	{
		buttons->selection += 1;
		if (buttons->selection >= buttons->buttonCount) buttons->selection = buttons->buttonCount - 1;
	}
	else if (kDown & KEY_A)
	{	
		if (buttons->selection == 0)
		{
			difficultyIndex = details->textPanel.selection.x;
			activeDifficulty = Settings::diffs[difficultyIndex];
			map = Maps(sheet, activeDifficulty);
			map->generate();
			timer->reset();

			replays->recorder.clear();
			replays->player.playing = false;
			submittedTime = false;
		}
		else if (buttons->selection == 1)
		{
			if (details->textPanel.currentScreen == helpScreen)
			{
				buttons->helpText = false;
				details->textPanel.currentScreen = lbScreen;
				replays->getScores(activeDifficulty);
			}
			else
			{
				buttons->helpText = true;
				details->textPanel.currentScreen = helpScreen;
				details->textPanel.selection.x = difficultyIndex;
			}
			buttons->setVector();
			details->textPanel.updateSelection(replays->scores, {0,0});
			details->textPanel.setText();
		}
	}
	else if (kDown & KEY_X)
	{
		if (details->textPanel.currentScreen == lbScreen && replays->scores.size() > 0)
		{
			int replayIndex = details->textPanel.selection.y + (details->textPanel.replayPage * 8);
			replays->player.start(replays->scores[replayIndex]);
			replays->player.playing = !replays->player.playing;
			replays->player.finished = false;
			if (replays->player.playing) details->initReplayText(replays->scores[replayIndex].time, replays->scores[replayIndex].username);
		}
	}
}

void Game::render()
{
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, clrBlack);
	C2D_SceneBegin(top);
	details->draw(difficultyIndex);
	buttons->draw();
	
	C2D_TargetClear(bottom, clrBlack);
	C2D_SceneBegin(bottom);
	if (!replays->player.playing) map->draw();
	else if (replays->player.playing) replays->player.draw();
	C3D_FrameEnd(0);
}

void Game::run()
{
	// libctru inits.
	init();

	while (aptMainLoop())
	{
		if (map->mapCompleted()) onGameCompleted();

		hidScanInput();
		handleInput();

		details->replayMode = replays->player.playing;	

		if (details->replayMode)
		{
			details->update(
				C2D_Color32f(0.f,0.f,1.f,0.5),
				activeDifficulty.mineCount - replays->player.getMinesPlaced(),
				replays->player.getTime()
			);
		}
		else
		{
			details->update(
				C2D_Color32f(1.f,1.f,1.f,0.5),
				activeDifficulty.mineCount - map->minesPlaced,
				timer->getTime()
			);
		}

		render();
	}


}