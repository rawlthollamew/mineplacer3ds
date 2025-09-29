#include <3ds.h>
#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "maps.h"
#include "details.h"
#include "buttons.h"
#include "timer.h"
#include "replay_manager.h"
#include "settings.h"

int main(int argc, char* argv[])
{
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	romfsInit();
	C2D_Prepare();
	
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	
	u32 clrBlack = C2D_Color32f(0.f, 0.f, 0.f, 0.f);
	C2D_SpriteSheet sheet = C2D_SpriteSheetLoad("romfs:/gfx/textures.t3x");
	
	Difficulty currentDifficulty = Settings::diffs[0];

	Maps maps(sheet, currentDifficulty);
	maps.generate();

	Details details(sheet, currentDifficulty.mineCount, currentDifficulty.dimentions, 1.f);
	ButtonHandler buttonHandler(sheet, details.getInfoPosition(), details.getInfoPadding(), 0.75f);
	
	ReplayManager replays(sheet, currentDifficulty, "sdmc:/mineplacer/");
	details.textPanel.loadLeaderboardText(replays.scores);

	Timer timer;

	buttonHandler.setVector();
	bool submittedTime = false;
	bool completed = maps.mapCompleted();
	
	while (aptMainLoop())
	{
		completed = maps.mapCompleted();

		if (completed)
		{
			timer.stop();

			if (!submittedTime)
			{
				replays.saveReplay(replays.recorder.getMoves(), maps.mineMap, timer.getTime(), currentDifficulty);

				replays.getScores(currentDifficulty);
				details.textPanel.loadLeaderboardText(replays.scores);				
				submittedTime = true;
			}
		}
		
		hidScanInput();
		u32 kDown = hidKeysDown();
		
		touchPosition touch;
		hidTouchRead(&touch);
		
		if (kDown & KEY_START) break;
		else if ((kDown & KEY_TOUCH) && !completed)
		{
			Vector2i mapPosition = {
				(int)(touch.px / currentDifficulty.tileSize),
				(int)(touch.py / currentDifficulty.tileSize)
			};

			maps.placeMine(mapPosition);
			replays.recorder.update(mapPosition);
		}
		else if (kDown & KEY_UP) details.textPanel.updateSelection(replays.scores, {0, -1});
		else if (kDown & KEY_DOWN) details.textPanel.updateSelection(replays.scores, {0, 1});
		else if (kDown & KEY_LEFT) details.textPanel.updateSelection(replays.scores, {-1, 0});
		else if (kDown & KEY_RIGHT) details.textPanel.updateSelection(replays.scores, {1, 0});
		else if (kDown & KEY_L)
		{
			buttonHandler.selection -= 1;
			if (buttonHandler.selection < 0) buttonHandler.selection = 0;
		}
		else if (kDown & KEY_R)
		{
			buttonHandler.selection += 1;
			if (buttonHandler.selection >= buttonHandler.buttonCount) buttonHandler.selection = buttonHandler.buttonCount - 1;
		}
		else if (kDown & KEY_A)
		{
			if (buttonHandler.selection == 0)
			{
				maps.generate();
				timer.reset();
				replays.recorder.clear();
				replays.player.playing = false;
				submittedTime = false;
			}
			else if (buttonHandler.selection == 1)
			{
				buttonHandler.helpText = !buttonHandler.helpText;
				buttonHandler.setVector();
				if (buttonHandler.helpText) details.textPanel.currentScreen = lbScreen;
				else if (!buttonHandler.helpText) details.textPanel.currentScreen = helpScreen;
				details.textPanel.setText();
			}
		}
		else if (kDown & KEY_X)
		{
			if (details.textPanel.currentScreen == lbScreen && replays.scores.size() > 0)
			{
				int replayIndex = details.textPanel.selection.y + (details.textPanel.replayPage * 8);
				replays.player.start(replays.scores[replayIndex]);
				replays.player.playing = !replays.player.playing;
				replays.player.finished = false;
				if (replays.player.playing) details.initReplayText(replays.scores[replayIndex].time, replays.scores[replayIndex].username);
			}
			else if (details.textPanel.currentScreen == helpScreen)
			{
				currentDifficulty = Settings::diffs[details.textPanel.selection.x];
				maps = Maps(sheet, currentDifficulty);
				maps.generate();
				replays.getScores(currentDifficulty);
			}
		}
		
		details.replayMode = replays.player.playing;

		details.update(
			(replays.player.playing) ? C2D_Color32f(0.f, 0.f, 1.f, 0.5f) : C2D_Color32f(1.f, 1.f, 1.f, 0.5f),
			(replays.player.playing) ? currentDifficulty.mineCount - replays.player.getMinesPlaced() : currentDifficulty.mineCount - maps.minesPlaced,
			(replays.player.playing) ? replays.player.getTime() : timer.getTime()
		);

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, clrBlack);
			C2D_SceneBegin(top);
			details.draw();
			buttonHandler.draw();
			
			C2D_TargetClear(bottom, clrBlack);
			C2D_SceneBegin(bottom);
			if (!replays.player.playing) maps.draw();
			else if (replays.player.playing) replays.player.draw();
		C3D_FrameEnd(0);
	}
	
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	return 0;
}