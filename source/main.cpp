#include <3ds.h>
#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "maps.h"
#include "details.h"
#include "buttons.h"
#include "scores.h"
#include "timer.h"

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
	Details details(sheet, maps.mineCount, maps.dimentions, 1);
	ButtonHandler buttonHandler(sheet, details.getInfoPosition(), details.getInfoPadding());
	Scores scores("sdmc://scores.txt");
	details.textPanel.loadLeaderboardText(scores.lb);
	Timer timer;

	buttonHandler.setVector(details.textPanel.helpText);
	bool submittedTime = false;
	
	while (aptMainLoop())
	{
		if (maps.mapCompleted())
		{
			timer.stop();

			if (!submittedTime)
			{
				scores.insertScore(timer.getTime());
				details.textPanel.loadLeaderboardText(scores.lb);
				submittedTime = true;
			}
		}
		
		hidScanInput();
		u32 kDown = hidKeysDown();
		
		touchPosition touch;
		hidTouchRead(&touch);
		
		if (kDown & KEY_START) break;
		else if (kDown & KEY_TOUCH) maps.placeMine({touch.px, touch.py});
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
				submittedTime = false;
			}
			else if (buttonHandler.selection == 1)
			{
				details.textPanel.helpText = !details.textPanel.helpText;
				details.textPanel.setText();
				buttonHandler.setVector(details.textPanel.helpText);
			}
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
			maps.draw();
		C3D_FrameEnd(0);
	}
	
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	return 0;
}