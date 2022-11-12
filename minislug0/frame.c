
#include "includes.h"

#define FPS_Default (1000 / 70)
u32 gnTimer1;
u32 gnFrame;	// Compteur général.
#define	FPS_MissMax	(2)
u8	gnFrameMissed;

// Init timers.
void FrameInit(void)
{
	gnTimer1 = SDL_GetTicks();
	gnFrameMissed = 0;
}

// Attente de la frame.
void FrameWait(void)
{
	u32	nTimer2;

/*
*/
	// On a loupé des frames ?
	if (gnFrameMissed == 0)
	{
		nTimer2 = SDL_GetTicks() - gnTimer1;
		if (nTimer2 >= FPS_Default) gnFrameMissed = nTimer2 / FPS_Default;
//if (gnFrameMissed) printf("Frames missed: %d / %d\n", gnFrame, gnFrameMissed);
		if (gnFrameMissed > FPS_MissMax) gnFrameMissed = FPS_MissMax;	// On ne saute pas trop de frames quand même.
		if (gnFrameMissed) goto _FrameEnd;
	}
	else
	{
		gnFrameMissed--;
		goto _FrameEnd;
	}

//nTimer2 = SDL_GetTicks() - gnTimer1;
//if (nTimer2 >= FPS_Default) printf("missed: %d\n", nTimer2 / FPS_Default);

	// On s'assure qu'on ne va pas trop vite...
	while (1)
	{
		nTimer2 = SDL_GetTicks() - gnTimer1;
		if (nTimer2 >= FPS_Default) break;
		SDL_Delay(3);
	}
_FrameEnd:
	gnTimer1 = SDL_GetTicks();

	gnFrame++;

}

