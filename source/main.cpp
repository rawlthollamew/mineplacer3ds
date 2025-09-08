#include <3ds.h>
#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "maps.h"
#include "details.h"

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
	Details details(sheet, maps.mineCount, maps.dimentions);

	while (aptMainLoop())
	{
		hidScanInput();
		u32 kDown = hidKeysDown();
		
		touchPosition touch;
		hidTouchRead(&touch);
		
		if (kDown & KEY_START) break;
		else if (kDown & KEY_TOUCH)
		{
			Vector2i mapPosition = {touch.px / maps.tileSize, touch.py / maps.tileSize};

			if (mapPosition.x >= 0 && mapPosition.x < maps.dimentions.x &&
				mapPosition.y >= 0 && mapPosition.y < maps.dimentions.y)
			{
				maps.placeMine(mapPosition);
			}
		}
		else if (kDown & KEY_A)
		{
			maps.generate();
			details.resetTimer();
		}

		if (maps.mapCompleted()) details.stopTimer();


		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, clrBlack);
			C2D_SceneBegin(top);
			details.draw(maps.minesPlaced);
			
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