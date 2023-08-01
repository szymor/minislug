
#include "includes.h"

#define FPS_Default (1000 / 70)
u32 gnTimer1;
u32 gnFrame;	// General counter.
#define	FPS_MissMax	(2)
u8	gnFrameMissed;

// Init timers.
void FrameInit(void)
{
	gnTimer1 = SDL_GetTicks();
	gnFrameMissed = 0;
}

// Waiting for the frame.
void FrameWait(void)
{
	u32	nTimer2;

/*
*/
	// Have we missed any frames?
#ifndef NO_FRAME_SKIPPING
	if (gnFrameMissed == 0)
	{
		nTimer2 = SDL_GetTicks() - gnTimer1;
		if (nTimer2 >= FPS_Default) gnFrameMissed = nTimer2 / FPS_Default;
		if (gnFrameMissed > FPS_MissMax) gnFrameMissed = FPS_MissMax;	// Don't skip too many frames, though.
		if (gnFrameMissed) goto _FrameEnd;
	}
	else
	{
		gnFrameMissed--;
		goto _FrameEnd;
	}
#endif

//nTimer2 = SDL_GetTicks() - gnTimer1;
//if (nTimer2 >= FPS_Default) printf("missed: %d\n", nTimer2 / FPS_Default);

	// Making sure we don't go too fast...
#ifndef FLIP_VSYNC
	while (1)
	{
		nTimer2 = SDL_GetTicks() - gnTimer1;
		if (nTimer2 >= FPS_Default) break;
		SDL_Delay(3);
	}
#endif
_FrameEnd:
	gnTimer1 = SDL_GetTicks();

	gnFrame++;

}
