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

	Maps maps(sheet);
	maps.generate();
	Details details(sheet, maps.mineCount, maps.dimentions, 1);
	ReplayManager replays(sheet, "sdmc:/mineplacer/");
	ButtonHandler buttonHandler(sheet, details.getInfoPosition(), details.getInfoPadding(), 0.75f);
	details.textPanel.loadLeaderboardText(replays.scores);
	Timer timer;

	buttonHandler.setVector(details.textPanel.helpText);
	bool submittedTime = false;
	bool replay = false;
	
	while (aptMainLoop())
	{
		if (maps.mapCompleted())
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
		else if (kDown & KEY_TOUCH)
		{
			Vector2i mapPosition = {
				(int)(touch.px / maps.tileSize),
				(int)(touch.py / maps.tileSize)
			};

			maps.placeMine(mapPosition);
			replays.recorder.update(mapPosition);
		}
		else if (kDown & KEY_LEFT)
		{
			buttonHandler.selection -= 1;
			if (buttonHandler.selection < 0) buttonHandler.selection = 0;
		}
		else if (kDown & KEY_RIGHT)
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
				submittedTime = false;
			}
			else if (buttonHandler.selection == 1)
			{
				details.textPanel.helpText = !details.textPanel.helpText;
				details.textPanel.setText();
				buttonHandler.setVector(details.textPanel.helpText);
			}
		}
		else if (kDown & KEY_B)
		{
			replay = !replay;
		}

		details.update(
			maps.mineCount - maps.minesPlaced,
			timer.getTime()
		);

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, clrBlack);
			C2D_SceneBegin(top);
			details.draw();
			buttonHandler.draw();
			
			C2D_TargetClear(bottom, clrBlack);
			C2D_SceneBegin(bottom);
			if (!replay) maps.draw();
			else if (replay) replays.player.draw(replays.scores[0]);
		C3D_FrameEnd(0);
	}
	
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	return 0;
}