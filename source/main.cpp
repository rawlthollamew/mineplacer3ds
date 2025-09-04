#include <3ds.h>
#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "maps.h"
#include "details.h"
#include "player.h"

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

	Maps maps;
	Details details;
	Player player;

	while (aptMainLoop())
	{
		hidScanInput();

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) break;

		// player.update(maps.dimentions);
		// if (player.tilePlaced) maps.placeMine(player.position);
		player.update(maps.tileSize);
		if (player.tilePlaced) maps.placeMine(player.position);

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, clrBlack);
			C2D_SceneBegin(top);
			details.draw();
			
			C2D_TargetClear(bottom, clrBlack);
			C2D_SceneBegin(bottom);
			maps.draw();
			// player.draw(maps.tileSize);
		C3D_FrameEnd(0);
	}
	
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	return 0;
}