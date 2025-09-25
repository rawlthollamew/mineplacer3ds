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

const int tileSize = 20;
const int mineCount = 50;
const Vector2i dimentions = { bottomScreen.x / tileSize, bottomScreen.y / tileSize };

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
	
	Maps maps(sheet, tileSize, mineCount, dimentions);
	maps.generate();

	Details details(sheet, mineCount, dimentions, 1);
	ButtonHandler buttonHandler(sheet, details.getInfoPosition(), details.getInfoPadding(), 0.75f);
	
	ReplayManager replays(sheet, tileSize, mineCount, dimentions, "sdmc:/mineplacer/");
	details.textPanel.loadLeaderboardText(replays.scores);

	Timer timer;

	buttonHandler.setVector(details.textPanel.helpText);
	bool submittedTime = false;
	bool completed = maps.mapCompleted();
	int replaySelection = 0;
	
	while (aptMainLoop())
	{
		completed = maps.mapCompleted();

		if (completed)
		{
			timer.stop();

			if (!submittedTime)
			{
				replays.saveReplay(replays.recorder.getMoves(), maps.mineMap, timer.getTime());

				replays.getScores();
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
				(int)(touch.px / tileSize),
				(int)(touch.py / tileSize)
			};

			maps.placeMine(mapPosition);
			replays.recorder.update(mapPosition);
		}
		else if (kDown & KEY_UP)
		{
			replaySelection -= 1;
			if (replaySelection < 0) replaySelection = 0;
		}
		else if (kDown & KEY_DOWN)
		{
			// lb has a max size of 8.
			replaySelection += 1;
			if (replaySelection >= (int)std::min((size_t)8, replays.scores.size())) replaySelection = std::min((size_t)8, replays.scores.size()) - 1;
		}
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
				details.textPanel.helpText = !details.textPanel.helpText;
				details.textPanel.setText();
				buttonHandler.setVector(details.textPanel.helpText);
			}
		}
		else if (kDown & KEY_X)
		{
			replays.player.start(replays.scores[replaySelection]);
			replays.player.playing = !replays.player.playing;
			if (replays.player.playing) details.initReplayText(replays.scores[replaySelection].time, replays.scores[replaySelection].username);
		}
		
		details.replayMode = replays.player.playing;

		details.update(
			(replays.player.playing) ? C2D_Color32f(0.f, 0.f, 1.f, 0.5f) : C2D_Color32f(1.f, 1.f, 1.f, 0.5f),
			(replays.player.playing) ? mineCount - replays.player.getMinesPlaced() : mineCount - maps.minesPlaced,
			(replays.player.playing) ? replays.player.getTime() : timer.getTime()
		);

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, clrBlack);
			C2D_SceneBegin(top);
			if (!details.textPanel.helpText) details.drawSelection(replaySelection);
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