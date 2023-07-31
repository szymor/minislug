
// Mini Slug Project.
// Code: Clement '17o2!!' CORDE
// Contact: Clement CORDE, c1702@yahoo.com

// Credits for the material used here:
// Rips by:
// Gussprint, The Mad Soldier, Random Rebel Soldier, C2000MC/Futaokuu,
// Random Talking Bush, Kevin Huff, Grey Wolf Leader, Goemar, Magma Dragoon,
// AzRaezel/Dan, The_Protagonist, Tk big_mickey_k, Grim, @$$h0l3 (what a nickname is that!?),
// Crazy Slug, Xezan... and some 'unsigned' rips. Hope I'm not forgetting anyone.


#include "includes.h"
#include "sprites_inc.h"

//#define DEBUG_KEYS	1		// A commenter.

// Variables générales.
struct SGene gVar;

//=====================================

#define	RENDER_BPP	1

enum
{
	e_RenderMode_Normal = 0,
	e_RenderMode_Scale2x,
	e_RenderMode_TV2x,
	//
	e_RenderMode_MAX
};

// Paramètres de rendu.
struct SRender
{
	// Note : Je laisse le pointeur pScreen dans gVar.
	// On fait dans tous les cas le rendu "normal" dans pScreen. Si on a un post-effect à faire, on redirige pScreen dans un buffer secondaire, qu'on recopiera avec l'effet voulu dans l'écran réel une fois le tracé du jeu fini.

	SDL_Surface *pScreen2x;		// En modes 2x, ptr sur la surface écran réelle.
	SDL_Surface *pScreenBuf2;	// Buffer de rendu pour le jeu en modes 2x (à la place de la surface écran réelle).

	u8	nRenderMode;			// Mode en cours : normal / 2x / TV2x.
	u8	nFullscreenMode;		// Fullscreen ou pas.

#ifdef	RENDER_BPP
	u8	nRenderBPP;		// 0 = 16 bits / 1 = 32 bits.
#endif

};
struct SRender	gRender;

// Scaling 2x.
void Render_Scale2x(SDL_Surface *pSDL_Src, SDL_Surface *pSDL_Dst)
{
	SDL_LockSurface(pSDL_Src);
	SDL_LockSurface(pSDL_Dst);

	u32	y, x;
	u8	*pSrc = pSDL_Src->pixels;
	u8	*pDst = pSDL_Dst->pixels;
	u32	nClr;

	u16	*pSrc2;
	u32	*pDst2a, *pDst2b;

	for (y = 0; y < SCR_Height; y++)
	{
		pSrc2 = (u16 *)pSrc;
		pDst2a = (u32 *)pDst;
		pDst2b = (u32 *)(pDst + pSDL_Dst->pitch);
		for (x = 0; x < SCR_Width; x++)
		{
			nClr = *pSrc2++;
			nClr |= (nClr << 16);
			*pDst2a++ = nClr;
			*pDst2b++ = nClr;
		}
		pSrc += pSDL_Src->pitch;
		pDst += pSDL_Dst->pitch * 2;
	}

	SDL_UnlockSurface(pSDL_Src);
	SDL_UnlockSurface(pSDL_Dst);
}

u16	gpTV2xCLUT[65536];

// Calcul de la grosse CLUT pour le mode TV2x.
#define	TV2x_FACTOR	200		// * factor, / 256
void TV2x_CalculateCLUT(SDL_Surface *pSDL_Dst)
{
	u32	i;
	u8	r, g, b;

	for (i = 0; i < 65536; i++)
	{
		SDL_GetRGB(i, pSDL_Dst->format, &r, &g, &b);
		r = ((u32)r * TV2x_FACTOR) / 256;
		g = ((u32)g * TV2x_FACTOR) / 256;
		b = ((u32)b * TV2x_FACTOR) / 256;
		gpTV2xCLUT[i] = SDL_MapRGB(pSDL_Dst->format, r, g, b);
	}
}

// TV2x.
void Render_TV2x(SDL_Surface *pSDL_Src, SDL_Surface *pSDL_Dst)
{
	SDL_LockSurface(pSDL_Src);
	SDL_LockSurface(pSDL_Dst);

	u32	y, x;
	u8	*pSrc = pSDL_Src->pixels;
	u8	*pDst = pSDL_Dst->pixels;
	u32	nClra, nClrb;

	u16	*pSrc2;
	u32	*pDst2a, *pDst2b;

	for (y = 0; y < SCR_Height; y++)
	{
		pSrc2 = (u16 *)pSrc;
		pDst2a = (u32 *)pDst;
		pDst2b = (u32 *)(pDst + pSDL_Dst->pitch);
		for (x = 0; x < SCR_Width; x++)
		{
			nClra = *pSrc2++;
			nClrb = gpTV2xCLUT[nClra];
			nClra |= (nClra << 16);
			nClrb |= (nClrb << 16);
			*pDst2a++ = nClra;
			*pDst2b++ = nClrb;
		}
		pSrc += pSDL_Src->pitch;
		pDst += pSDL_Dst->pitch * 2;
	}

	SDL_UnlockSurface(pSDL_Src);
	SDL_UnlockSurface(pSDL_Dst);
}


#ifdef	RENDER_BPP
u32	gpR2x_32_CLUT[65536];
u32	gpTV2x_32_CLUT[65536];

// Calcul des CLUT 32 bits.
void R2x_32_CalculateCLUT(void)
{
	u32	i;
	u8	r, g, b;

	SDL_Surface *pSrc;
	SDL_Surface *pDst;
/*
//> *** BPP Solution 1. > Sur Mac, pose problème à cause des masques des surfaces et écran qui sont différents.
	pSrc = SDL_CreateRGBSurface(SDL_SWSURFACE, SCR_Width, SCR_Height, 16, 0, 0, 0, 0);
	pDst = SDL_CreateRGBSurface(SDL_SWSURFACE, SCR_Width, SCR_Height, 32, 0, 0, 0, 0);
//<
*/

/*
//> *** BPP Solution 2.
//> Modif Pouet pour Mac. > Donne du bleu sur PC.
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	int mask16r = 0x001F;
	int mask16g = 0x07E0;
	int mask16b = 0xF800;
	int mask16a = 0x0000;
	int mask32r = 0xFF000000;
	int mask32g = 0x00FF0000;
	int mask32b = 0x0000FF00;
	int mask32a = 0x000000FF;
#else
	int mask16r = 0xF800;
	int mask16g = 0x07E0;
	int mask16b = 0x001F;
	int mask16a = 0x0000;
	int mask32r = 0x000000FF;
	int mask32g = 0x0000FF00;
	int mask32b = 0x00FF0000;
	int mask32a = 0xFF000000;
#endif
	pSrc = SDL_CreateRGBSurface(SDL_SWSURFACE, SCR_Width, SCR_Height, 16, mask16r, mask16g, mask16b, mask16a);
	pDst = SDL_CreateRGBSurface(SDL_SWSURFACE, SCR_Width, SCR_Height, 32, mask32r, mask32g, mask32b, mask32a);
//< Modif Pouet pour Mac.
//<
*/

//> *** BPP Solution 3. Ok partout, mais la manip dans InitVideo n'est pas très jolie...
	pSrc = SDL_CreateRGBSurface(SDL_SWSURFACE, SCR_Width, SCR_Height, 16, gRender.pScreenBuf2->format->Rmask, gRender.pScreenBuf2->format->Gmask, gRender.pScreenBuf2->format->Bmask, 0);
	pDst = SDL_CreateRGBSurface(SDL_SWSURFACE, SCR_Width, SCR_Height, 32, gVar.pScreen->format->Rmask, gVar.pScreen->format->Gmask, gVar.pScreen->format->Bmask, 0);
//<
	if (pSrc == NULL || pDst == NULL)
	{
		fprintf(stderr, "R2x_32_CalculateCLUT(): Unable to allocate SDL surfaces.\n");
		exit(1);
	}

	for (i = 0; i < 65536; i++)
	{
		SDL_GetRGB(i, pSrc->format, &r, &g, &b);
		gpR2x_32_CLUT[i] = SDL_MapRGB(pDst->format, r, g, b);
		r = ((u32)r * TV2x_FACTOR) / 256;
		g = ((u32)g * TV2x_FACTOR) / 256;
		b = ((u32)b * TV2x_FACTOR) / 256;
		gpTV2x_32_CLUT[i] = SDL_MapRGB(pDst->format, r, g, b);
	}

	SDL_FreeSurface(pSrc);
	SDL_FreeSurface(pDst);

}

// Scaling 2x - 16 > 32.
void Render_Scale2x_16to32(SDL_Surface *pSDL_Src, SDL_Surface *pSDL_Dst)
{
	SDL_LockSurface(pSDL_Src);
	SDL_LockSurface(pSDL_Dst);

	u32	y, x;
	u8	*pSrc = pSDL_Src->pixels;
	u8	*pDst = pSDL_Dst->pixels;
	u32	nClr;

	u16	*pSrc2;
	u32	*pDst2a, *pDst2b;

	for (y = 0; y < SCR_Height; y++)
	{
		pSrc2 = (u16 *)pSrc;
		pDst2a = (u32 *)pDst;
		pDst2b = (u32 *)(pDst + pSDL_Dst->pitch);
		for (x = 0; x < SCR_Width; x++)
		{
			nClr = gpR2x_32_CLUT[*pSrc2++];
			*pDst2a++ = nClr;
			*pDst2a++ = nClr;
			*pDst2b++ = nClr;
			*pDst2b++ = nClr;
		}
		pSrc += pSDL_Src->pitch;
		pDst += pSDL_Dst->pitch * 2;
	}

	SDL_UnlockSurface(pSDL_Src);
	SDL_UnlockSurface(pSDL_Dst);
}

// TV2x - 16 > 32.
void Render_TV2x_16to32(SDL_Surface *pSDL_Src, SDL_Surface *pSDL_Dst)
{
	SDL_LockSurface(pSDL_Src);
	SDL_LockSurface(pSDL_Dst);

	u32	y, x;
	u8	*pSrc = pSDL_Src->pixels;
	u8	*pDst = pSDL_Dst->pixels;
	u32	nClra, nClrb;

	u16	*pSrc2;
	u32	*pDst2a, *pDst2b;

	for (y = 0; y < SCR_Height; y++)
	{
		pSrc2 = (u16 *)pSrc;
		pDst2a = (u32 *)pDst;
		pDst2b = (u32 *)(pDst + pSDL_Dst->pitch);
		for (x = 0; x < SCR_Width; x++)
		{
			nClra = gpR2x_32_CLUT[*pSrc2];
			nClrb = gpTV2x_32_CLUT[*pSrc2++];
			*pDst2a++ = nClra;
			*pDst2a++ = nClra;
			*pDst2b++ = nClrb;
			*pDst2b++ = nClrb;
		}
		pSrc += pSDL_Src->pitch;
		pDst += pSDL_Dst->pitch * 2;
	}

	SDL_UnlockSurface(pSDL_Src);
	SDL_UnlockSurface(pSDL_Dst);
}

#endif


extern	u8	gnFrameMissed;
typedef void (*pRenderFct)(SDL_Surface *pSDL_Src, SDL_Surface *pSDL_Dst);
// Rendu + Flip.
void RenderFlip(u32 nSync)
{
#ifdef	RENDER_BPP
	static pRenderFct	pFctTb[2][e_RenderMode_MAX] =
	{
		{ NULL, Render_Scale2x, Render_TV2x },	// Screen 16 bits.
		{ NULL, Render_Scale2x_16to32, Render_TV2x_16to32 },	// Screen 32 bits.
	};
#else
	static pRenderFct	pFctTb[e_RenderMode_MAX] = { NULL, Render_Scale2x, Render_TV2x };
#endif

	// Frames loupées ? => Pas de Rendu/Flip.
	if (nSync && gnFrameMissed)
	{
		FrameWait();
		return;
	}

#ifdef	RENDER_BPP
	if (pFctTb[gRender.nRenderBPP][gRender.nRenderMode] != NULL) pFctTb[gRender.nRenderBPP][gRender.nRenderMode](gVar.pScreen, gRender.pScreen2x);
#else
	if (pFctTb[gRender.nRenderMode] != NULL) pFctTb[gRender.nRenderMode](gVar.pScreen, gRender.pScreen2x);
#endif
	if (nSync) FrameWait();
	SDL_Flip(gRender.nRenderMode == e_RenderMode_Normal ? gVar.pScreen : gRender.pScreen2x);

}

// Set video mode.
#ifdef	RENDER_BPP
SDL_Surface * VideoModeSet(u32 nScrWidth, u32 nScrHeight, u32 nBPP, u32 nSDL_Flags)
#else
SDL_Surface * VideoModeSet(u32 nScrWidth, u32 nScrHeight, u32 nSDL_Flags)
#endif
{
	SDL_Surface *pSurf;

#ifdef	RENDER_BPP
	pSurf = SDL_SetVideoMode(nScrWidth, nScrHeight, nBPP, SDL_SWSURFACE | nSDL_Flags);
#else
	pSurf = SDL_SetVideoMode(nScrWidth, nScrHeight, 16, SDL_SWSURFACE | nSDL_Flags);
#endif
	if (pSurf == NULL)
	{
		fprintf(stderr, "VideoModeSet(): Couldn't set video mode: %sn",SDL_GetError());
		//exit(1);
	}
	return (pSurf);
}

// Met le mode video qui va bien.
void Render_SetVideoMode(void)
{
	switch (gRender.nRenderMode)
	{
	case e_RenderMode_Scale2x:
	case e_RenderMode_TV2x:
#ifdef	RENDER_BPP
		gRender.pScreen2x = VideoModeSet(SCR_Width * 2, SCR_Height * 2, 16 << gRender.nRenderBPP, (gRender.nFullscreenMode ? SDL_FULLSCREEN : 0));
#else
		gRender.pScreen2x = VideoModeSet(SCR_Width * 2, SCR_Height * 2, (gRender.nFullscreenMode ? SDL_FULLSCREEN : 0));
#endif
		gVar.pScreen = gRender.pScreenBuf2;
		if (gRender.pScreen2x != NULL) return;		// Ok.
		// Erreur => On repasse en mode Normal et Windowed.
		gRender.nRenderMode = e_RenderMode_Normal;
		gRender.nFullscreenMode = 0;
		// ... et pas de break.
	case e_RenderMode_Normal:
	default:
#ifdef	RENDER_BPP
		gVar.pScreen = VideoModeSet(SCR_Width, SCR_Height, 16, (gRender.nFullscreenMode ? SDL_FULLSCREEN : 0));
#else
		gVar.pScreen = VideoModeSet(SCR_Width, SCR_Height, (gRender.nFullscreenMode ? SDL_FULLSCREEN : 0));
#endif
		gRender.pScreen2x = NULL;
		if (gVar.pScreen == NULL) exit(1);	// Message d'erreur dans VideoModeSet.
		break;
	}

}

// Init de la vidéo.
void Render_InitVideo(void)
{
	gRender.nRenderMode = e_RenderMode_Normal;
	gRender.nFullscreenMode = 0;

	gRender.pScreen2x = NULL;		// En mode 2x, ptr sur la surface écran.
	gRender.pScreenBuf2 = NULL;		// Buffer de rendu pour le jeu en mode 2x (à la place de la surface écran réelle).

	// On initialise d'abord un écran en mode e_RenderMode_Normal. Important, car on fait un CreateRGBSurface à partir de cette surface.
#ifdef	RENDER_BPP
	gVar.pScreen = VideoModeSet(SCR_Width, SCR_Height, 16, gRender.nFullscreenMode ? SDL_FULLSCREEN : 0);
#else
	gVar.pScreen = VideoModeSet(SCR_Width, SCR_Height, gRender.nFullscreenMode ? SDL_FULLSCREEN : 0);
#endif
	if (gVar.pScreen == NULL) exit(1);
	// On créé un buffer de la taille de l'écran.
	// => En mode 2x, on switche le ptr pScreen sur cette surface, les rendus du jeu se font donc dedans. Puis on fait le scale/filtre du buffer vers la vraie surface écran.
	gRender.pScreenBuf2 = SDL_CreateRGBSurface(SDL_SWSURFACE, SCR_Width, SCR_Height, 16, gVar.pScreen->format->Rmask, gVar.pScreen->format->Gmask, gVar.pScreen->format->Bmask, 0);
	if (gRender.pScreenBuf2 == NULL)
	{
		fprintf(stderr, "Render_InitVideo(): Unable to allocate SDL surface: %s\n", SDL_GetError());
		exit(1);
	}

	// Calcul de la CLUT pour le mode TV2x.
	TV2x_CalculateCLUT(gVar.pScreen);
#ifdef	RENDER_BPP
	// Calcul des CLUTs pour les mode 32 bits.
	if (gRender.nRenderBPP)
	{
/*
//> *** BPP Solution 1.
		R2x_32_CalculateCLUT();
//<
*/
//> *** BPP Solution 3.
		gVar.pScreen = VideoModeSet(SCR_Width, SCR_Height, 32, gRender.nFullscreenMode ? SDL_FULLSCREEN : 0);
		R2x_32_CalculateCLUT();
		gVar.pScreen = VideoModeSet(SCR_Width, SCR_Height, 16, gRender.nFullscreenMode ? SDL_FULLSCREEN : 0);
//<
	}
#endif

}

// Libère les ressources du rendu. (1 fois !).
void RenderRelease(void)
{
	SDL_FreeSurface(gRender.pScreenBuf2);
}

//=====================================

struct SCheatCodes	gCCodes;

// Cheat codes - Reset de la saisie.
void CheatCodes_Reset(void)
{
	gCCodes.nIdx = 0;
	gCCodes.nCnt = 0;
	gCCodes.nEnter = 0;
}

// Cheat codes - Compteur.
void CheatCodes_Counter(void)
{
	if (gCCodes.nCnt == 0) return;
	if (--gCCodes.nCnt == 0) CheatCodes_Reset();
}

// Les codes en version non lisible avec un éditeur hexa. (Chr majuscules : 0x40 > 0x5F).
u8 gpCodes[CHEATCODES_NB][CHEATCODES_TBSZMAX + 1] =
{
	{ 'S' ^ 0x1F, 'E' ^ 0x2E, 'L' ^ 0x3D, 'E' ^ 0x0C, 'C' ^ 0x1B, 'T' ^ 0x2A, 0, 0, 0, 0, 0 },				// Weapon select.
	{ 'U' ^ 0x1F, 'N' ^ 0x2E, 'L' ^ 0x3D, 'I' ^ 0x0C, 'M' ^ 0x1B, 'I' ^ 0x2A, 'T' ^ 0x39, 'E' ^ 0x08, 'D' ^ 0x17, 0, 0 },		// Weapon unlimited.
	{ 'I' ^ 0x1F, 'N' ^ 0x2E, 'F' ^ 0x3D, 'I' ^ 0x0C, 'N' ^ 0x1B, 'I' ^ 0x2A, 'T' ^ 0x39, 'E' ^ 0x08, 0, 0, 0 },		// Vies infinies.
	{ 'M' ^ 0x1F, 'O' ^ 0x2E, 'R' ^ 0x3D, 'D' ^ 0x0C, 'E' ^ 0x1B, 'N' ^ 0x2A, 0, 0, 0, 0, 0 },				// Invulnérabilité.
	{ 'L' ^ 0x1F, 'E' ^ 0x2E, 'V' ^ 0x3D, 'E' ^ 0x0C, 'L' ^ 0x1B, 'S' ^ 0x2A, 0, 0, 0, 0, 0 },				// Level select.
	{ 'N' ^ 0x1F, 'X' ^ 0x2E, 'T' ^ 0x3D, 'L' ^ 0x0C, 'V' ^ 0x1B, 'L' ^ 0x2A, 0, 0, 0, 0, 0 },				// Next level.
	{ 'K' ^ 0x1F, 'U' ^ 0x2E, 'R' ^ 0x3D, 'E' ^ 0x0C, 'M' ^ 0x1B, 'O' ^ 0x2A, 'N' ^ 0x39, 0, 0, 0, 0 },		// God mode = Tous les cheats.
};
u8 gpCodesOR[CHEATCODES_NB] =
{ e_Cheat_WeaponSelect, e_Cheat_WeaponUnlimited, e_Cheat_InfiniteLives, e_Cheat_Invulnerability, e_Cheat_LevelSelect, e_Cheat_LevelNext, e_Cheat_GodMode };

// Cheat codes - Touche pressée.
void CheatCodes_KeyPressed(u32 nSdlKey)
{

	// On est en mode entrée de cheat ?
	if (gCCodes.nEnter == 0)
	{	// Non. On y passe ?
		if (nSdlKey == SDLK_F5) gCCodes.nEnter = 1;
		return;
	}
	gVar.pKeys[nSdlKey] = 0;	// RAZ touche.

	// Appui.
	if (gCCodes.nIdx >= CHEATCODES_TBSZMAX - 1) return;
	if (nSdlKey < SDLK_a || nSdlKey > SDLK_z) return;
	nSdlKey -= SDLK_a;
	nSdlKey += 'A';
	gCCodes.nTb[gCCodes.nIdx++] = (char)nSdlKey;
	gCCodes.nTb[gCCodes.nIdx] = 0;		// Arrête la chaîne au suivant.
	gCCodes.nCnt = CHEATCODES_TIMER;	// Timer.

	// Test du code.
	u32	i, t, k;
	char	pToTest[CHEATCODES_TBSZMAX + 1];
	for (i = 0; i < CHEATCODES_NB; i++)
	{
		// Recopie et décodage du code.
		k = 0x1F;
		t = 0;
		while (gpCodes[i][t])
		{
			pToTest[t] = gpCodes[i][t] ^ k;
			k += 0x10;
			k -= 1;
			k &= 0x3F;
			t++;
		}
		pToTest[t] = 0;
		// Le code entré correspond ?
		if (t == gCCodes.nIdx)	// (taille des chaînes)
		if (strcmp(gCCodes.nTb, pToTest) == 0)
		{
			gCCodes.nCheat |= gpCodesOR[i];
			CheatCodes_Reset();
			Sfx_PlaySfx(e_Sfx_Fx_GunReload, e_SfxPrio_0 + 5);	// Sfx.
			return;
		}
	}

}

// Affichage des cheat codes (sur la page principale).
void CheatCodes_Display(void)
{
	char	*ppTxt[] = { "Sel", "Unl", "Inf", "Inv", "Lvl", "Nxt", "God" };
	char	pStr[(CHEATCODES_NB * 4) + 1];
	u32	nIdx = 0;
	u32	i;

	pStr[nIdx] = 0;
	for (i = 0; i < CHEATCODES_NB - 1; i++)	// -1 => On n'affiche pas god.
	{
		if (gCCodes.nCheat & (1 << i))
		{
			if (nIdx) pStr[nIdx++] = '-';
			pStr[nIdx] = *(ppTxt[i]);
			pStr[nIdx+1] = *(ppTxt[i] + 1);
			pStr[nIdx+2] = *(ppTxt[i] + 2);
			nIdx += 3;
			pStr[nIdx] = 0;
		}
	}
	if (nIdx) Font_Print(10, SCR_Height - 4, pStr, 0);

}

//=====================================

void MenuTimer_Reset(void);

// Gestionnaire d'évènements.
int EventHandler(u32 nInGame)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			gVar.pKeys[event.key.keysym.sym] = 1;

			// Pas en cours de jeu ?
			if (nInGame == 0)
			{
				CheatCodes_KeyPressed(event.key.keysym.sym);	// Test des cheat codes.
				MenuTimer_Reset();		// RAZ du timer de time out pour les menus.
			}

// Sortir après la boucle ? => pas sûr.
			// Toggle fullscreen/windowed.
			if (gVar.pKeys[SDLK_F10])
			{
				gRender.nFullscreenMode ^= 1;
				Render_SetVideoMode();
			}
			// Toggle render mode.
			if (gVar.pKeys[SDLK_F9])
			{
				if (++gRender.nRenderMode >= e_RenderMode_MAX) gRender.nRenderMode = 0;
				Render_SetVideoMode();
			}

#ifdef	DEBUG_KEYS
			if (gVar.pKeys[SDLK_ESCAPE]) return (1);	// Emergency exit.
#endif
			break;

		case SDL_KEYUP:
			gVar.pKeys[event.key.keysym.sym] = 0;
			break;

		case SDL_QUIT:		// Fermeture de la fenêtre.
			exit(0);
			break;
		}
	}

	// Gestion du joystick.
	if (gVar.pJoystick != NULL)
	{
//bug?		static u16	pHatMsk[e_CfgKey_MAX] = { SDL_HAT_UP, SDL_HAT_DOWN, SDL_HAT_LEFT, SDL_HAT_RIGHT, 0x100, 0x200, 0x400 };
//bug?		static u8	pKeyVal[e_CfgKey_MAX] = { e_CfgKey_Up, e_CfgKey_Down, e_CfgKey_Left, e_CfgKey_Right, e_CfgKey_ButtonA, e_CfgKey_ButtonB, e_CfgKey_ButtonC };
		static u16	pHatMsk[e_CfgKey_LAST] = { SDL_HAT_UP, SDL_HAT_DOWN, SDL_HAT_LEFT, SDL_HAT_RIGHT, 0x100, 0x200, 0x400 };
		static u8	pKeyVal[e_CfgKey_LAST] = { e_CfgKey_Up, e_CfgKey_Down, e_CfgKey_Left, e_CfgKey_Right, e_CfgKey_ButtonA, e_CfgKey_ButtonB, e_CfgKey_ButtonC };

		u32	i;
		s16	nAxis;
		u16	nHat, nVal;
		nHat = 0;
		// Croix.
		if (gVar.nJoystickNoHat == 0)
			nHat = SDL_JoystickGetHat(gVar.pJoystick, 0);
		// Stick.
		if (gVar.nJoystickNoAxes == 0)
		{
			nAxis = SDL_JoystickGetAxis(gVar.pJoystick, 0);	// X
			if (ABS(nAxis) > 3200) nHat |= (nAxis > 0 ? SDL_HAT_RIGHT : SDL_HAT_LEFT);
			nAxis = SDL_JoystickGetAxis(gVar.pJoystick, 1);	// Y
			if (ABS(nAxis) > 3200) nHat |= (nAxis > 0 ? SDL_HAT_DOWN : SDL_HAT_UP);
		}
		// Boutons.
		if (SDL_JoystickGetButton(gVar.pJoystick, gMSCfg.pKeys[e_CfgKey_JoyButtonA])) nHat |= 0x100;
		if (SDL_JoystickGetButton(gVar.pJoystick, gMSCfg.pKeys[e_CfgKey_JoyButtonB])) nHat |= 0x200;
		if (SDL_JoystickGetButton(gVar.pJoystick, gMSCfg.pKeys[e_CfgKey_JoyButtonC])) nHat |= 0x400;

		nVal = gVar.nJoystickState ^ nHat;
		gVar.nJoystickState = nHat;		// Sauvegarde pour le prochain tour.

//bug?		for (i = 0; i < e_CfgKey_MAX; i++)
		for (i = 0; i < e_CfgKey_LAST; i++)
		if (nVal & pHatMsk[i])
		{
			gVar.pKeys[gMSCfg.pKeys[pKeyVal[i]]] = (nHat & pHatMsk[i] ? 1 : 0);
			gVar.pKeysSDL[gMSCfg.pKeys[pKeyVal[i]]] = (nHat & pHatMsk[i] ? 1 : 0);
		}

		// Pas en cours de jeu ?
		if (nInGame == 0 && nVal) MenuTimer_Reset();		// RAZ du timer de time out pour les menus.

	}

	return (0);
}


// Le Menu (générique).
u32 Menu(void (*pFctInit)(void), u32 (*pFctMain)(void))
{
	u32	nMenuVal = MENU_Null;

	gVar.pBackground = gVar.pBkg[0];		// Décor par défaut.
//	gVar.pBkgRect = NULL;					// Par défaut, NULL (toute la surface).

	Transit2D_Reset();
	CheatCodes_Reset();

	// Fonction d'init.
	(*pFctInit)();

	// Main loop.
	FrameInit();
	#if CACHE_ON == 1
	CacheClear();		// RAZ cache.
	#endif
	while (nMenuVal == MENU_Null)
	{
		// Gestion des évenements.
#ifdef	DEBUG_KEYS
		if (EventHandler(0) != 0) { nMenuVal = MENU_Quit; break; }
#else
		EventHandler(0);
#endif
		CheatCodes_Counter();

		// Menu Main.
		nMenuVal = (*pFctMain)();

		SprDisplayAll();

#ifdef	DEBUG_KEYS
//>> test pour ralentir l'affichage.
if (gVar.pKeys[SDLK_y]) { FrameWait(); FrameWait(); FrameWait(); FrameWait(); FrameWait(); FrameWait(); }
#endif

		// Affichage de la transition.
		Transit2D_Manage();
//		// Wait for frame, Flip.
//		FrameWait();
//		SDL_Flip(gVar.pScreen);
		RenderFlip(1);
	}

	return (nMenuVal);
}

#define	PAUSE_MENU	(1)		// Comment to disable.

#if defined (PAUSE_MENU)

#define	PAUSE_TXT_NB	(3)
#define	PAUSE_KEY	(SDLK_F12)
#define	PAUSE_TXT_LNHT	(12)
#define	PAUSE_TXT_LNPOSY(nLn)	( (SCR_Height / 2) - (((PAUSE_TXT_NB + 1) * PAUSE_TXT_LNHT) / 2) + 10 + ((nLn) * PAUSE_TXT_LNHT) )
#define	PAUSE_CLOSEFX_LG	(8)		// !!! Divisible par 4 !!!

// Affichage du texte de la pause.
void Pause_sub_TxtDisplay(char **pTxt, s32 *pnTxtSz, u32 nChoice)
{
	u32	i;

	// Text.
	CursorMove();
	Font_Print((SCR_Width / 2) - (pnTxtSz[0] / 2), PAUSE_TXT_LNPOSY(0), pTxt[0], 0);
	u32 nFontFlag;
	for (i = 1; i < PAUSE_TXT_NB; i++)
	{
		nFontFlag = 0;

		// Selecteur.
		if (i - 1 == nChoice)
		{
			nFontFlag = FONT_Highlight;
			Font_Print((SCR_Width / 2) - (pnTxtSz[i] / 2) - 18+4 - (CursorGetPos() >> 8), PAUSE_TXT_LNPOSY(i + 1), "\x81", nFontFlag);
			Font_Print((SCR_Width / 2) + (pnTxtSz[i] / 2) + 10-4 + (CursorGetPos() >> 8), PAUSE_TXT_LNPOSY(i + 1), "\x80", nFontFlag);
		}
		Font_Print((SCR_Width / 2) - (pnTxtSz[i] / 2), PAUSE_TXT_LNPOSY(i + 1), pTxt[i], nFontFlag);
	}
}

// Pause (ingame).
// Out: 0=Resume / 1=Quit.
u32 Pause(void)
{
	SDL_Surface	*pBkg;

	// Surface to store currently displayed picture.
	pBkg = SDL_CreateRGBSurface(SDL_SWSURFACE, gVar.pScreen->w, gVar.pScreen->h, gVar.pScreen->format->BitsPerPixel,
		gVar.pScreen->format->Rmask, gVar.pScreen->format->Gmask, gVar.pScreen->format->Bmask, gVar.pScreen->format->Amask);
	if (pBkg == NULL) return (0);	// Failed.

	// Saves current gnFrame.
	u32	nFrame_sav = gnFrame;

	// Current screen => background.
	SDL_BlitSurface(gVar.pScreen, NULL, pBkg, NULL);

	Sfx_ChannelsSave();		// Save data + clear currently playing sounds.

	// Shade.
	{	// (Block added to reduce stack size).
	SDL_LockSurface(pBkg);

	u32	y, x;
	u8	*pSrc = pBkg->pixels;
	u32	nClr;
	u16	*pSrc2;

	for (y = 0; y < SCR_Height; y++)
	{
		pSrc2 = (u16 *)pSrc;
		for (x = 0; x < SCR_Width; x++)
		{
/*
			// v1 : Shade only.
			nClr = gpTV2xCLUT[*pSrc2];
			nClr = gpTV2xCLUT[nClr];
			nClr = gpTV2xCLUT[nClr];
			*pSrc2++ = nClr;
*/

			// v2 : Grayscale + shade.
			// RGB to grayscale : x = 0.299r + 0.587g + 0.114b.
			u8	r, g, b;

			SDL_GetRGB(*pSrc2, gVar.pScreen->format, &r, &g, &b);
			nClr = (r * 0.299) + (g * 0.587) + (b * 0.114);
			nClr = SDL_MapRGB(gVar.pScreen->format, nClr, nClr, nClr);
			nClr = gpTV2xCLUT[nClr];
			nClr = gpTV2xCLUT[nClr];
			*pSrc2++ = nClr;

		}
		pSrc += pBkg->pitch;
	}

	SDL_UnlockSurface(pBkg);
	}

	// Calculate text size.
	char *pTxt[PAUSE_TXT_NB] = { "- PAUSED -", "RESUME", "QUIT" };
	s32	pnTxtSz[PAUSE_TXT_NB];

	u32	i;
	for (i = 0; i < PAUSE_TXT_NB; i++)
		pnTxtSz[i] = Font_Print(0, 0, pTxt[i], FONT_NoDisp);

	// Loop.
	u32	nChoice = 0;			// Cur line.
	gVar.pKeys[PAUSE_KEY] = 0;
	gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Up]] = gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]] = 0;
	CursorInit();
	RenderFlip(1);
	while (1)
	{
#ifdef	DEBUG_KEYS
		if (EventHandler(1) != 0) { nChoice = 0; break; }
#else
		EventHandler(1);
#endif

		// Up/Down.
		u32	nLastChoice = nChoice;
		if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Up]])
		{
			if (--nChoice > PAUSE_TXT_NB-2) nChoice = PAUSE_TXT_NB-2;
			gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Up]] = 0;
		}
		if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]])
		{
			if (++nChoice > PAUSE_TXT_NB-2) nChoice = 0;
			gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]] = 0;
		}
		nLastChoice ^= nChoice;
		if (nLastChoice)
		{
			CursorInit();								// Slt parce que c'est plus joli.
			Sfx_PlaySfx(FX_Menu_Move, e_SfxPrio_10);	// Sfx.
		}

		// Button A.
		if (gVar.pKeys[SDLK_RETURN] || gVar.pKeys[SDLK_SPACE] || gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]])
		{
//			Sfx_PlaySfx(FX_Menu_Clic, e_SfxPrio_10);    // Sfx.
			gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] = 0;
			break;
		}
		// Pause (resume).
		if (gVar.pKeys[PAUSE_KEY])
		{
//			Sfx_PlaySfx(FX_Menu_Clic, e_SfxPrio_10);    // Sfx.
			nChoice = 0;
			gVar.pKeys[PAUSE_KEY] = 0;
			break;
		}

		// Bkg.
		SDL_BlitSurface(pBkg, NULL, gVar.pScreen, NULL);
		// Txt.
		Pause_sub_TxtDisplay(pTxt, pnTxtSz, nChoice);

		SprDisplayAll();
		// Wait for frame, Flip.
		RenderFlip(1);
	}

	// End.
	if (nChoice == 0)
		Sfx_ChannelsRestore();		// Resume => Restore audio data.
	else
	{
		// Quit. Closing transition, the lame way.
//		SDL_BlitSurface(gVar.pScreen, NULL, pBkg, NULL);	// Frame buffer to bkg (so we have the pause text's sprites too).
		Transit2D_Reset();
		Transit2D_InitClosing(e_Transit_Menu);
		while (Transit2D_CheckEnd() == 0)
		{
			SDL_BlitSurface(pBkg, NULL, gVar.pScreen, NULL);	// Bkg.
			Pause_sub_TxtDisplay(pTxt, pnTxtSz, nChoice);		// Txt.
			SprDisplayAll();
//			SDL_BlitSurface(pBkg, NULL, gVar.pScreen, NULL);
			// Affichage de la transition.
			Transit2D_Manage();
			// Wait for frame, Flip.
			RenderFlip(1);
		}
	}

	SDL_FreeSurface(pBkg);

	// Restores gnFrame.
	gnFrame = nFrame_sav;

	return (nChoice);
}
#endif

// Game loop. Externalisé pour jeu, how to play, credits...
void GameLoop(u32 nMissionTbOffset)
{
	ExgPlatformerInit(gVar.nCreditsToUse, nMissionTbOffset);

	Transit2D_Reset();

	// Main loop.
	FrameInit();
	#if CACHE_ON == 1
	CacheClear();		// RAZ cache.
	#endif
	while (gGameVar.nExitCode == 0)
	{
		// Gestion des évenements.
#ifdef	DEBUG_KEYS
		if (EventHandler(1) != 0) { LevelRelease(); gGameVar.nExitCode = e_Game_Aborted; break; }
#else
		EventHandler(1);
#endif
		// Gestion du jeu.
		PlatformerGame();

#ifdef	DEBUG_KEYS
//>> test pour ralentir l'affichage.
if (gVar.pKeys[SDLK_y]) { FrameWait(); FrameWait(); FrameWait(); FrameWait(); FrameWait(); FrameWait(); }
//if (gVar.pKeys[SDLK_y]) { FrameWait(); FrameWait(); FrameWait(); FrameWait(); FrameWait(); FrameWait(); }
//if (gVar.pKeys[SDLK_y]) { FrameWait(); FrameWait(); FrameWait(); FrameWait(); FrameWait(); FrameWait(); }
#endif

		// Affichage de la transition.
		Transit2D_Manage();
		// Wait for frame, Flip.
//		RenderFlip(1);
#ifdef	DEBUG_KEYS
		RenderFlip(gVar.pKeys[SDLK_u] ? 0 : 1);
#else
		RenderFlip(1);
#endif

#if defined (PAUSE_MENU)
		// Pause.
		if (gVar.pKeys[PAUSE_KEY] && gGameVar.nGenLevel >= MISSIONOFFS_LEVELS)	// Pause seulement dans les niveaux de jeu.
		{
			u32	nRetPause;
			nRetPause = Pause();
			if (nRetPause != 0) { LevelRelease(); gGameVar.nExitCode = e_Game_Aborted; } //break; }
			gVar.pKeys[PAUSE_KEY] = 0;
		}
#endif

	}
	Music_Start(e_YmMusic_NoMusic, 1);
}

// La boucle de jeu.
void Game(void)
{
	// Jeu.
	GameLoop(MISSIONOFFS_LEVELS + ((gCCodes.nCheat & e_Cheat_LevelSelect) ? gCCodes.nLevel : 0));

	// Si jeu abandonné, pas de game over, pas de high score, pas de crédits.
	if (gGameVar.nExitCode == e_Game_Aborted) return;

	// Credits si jeu terminé.
	if (gGameVar.nExitCode == e_Game_AllClear)
	{
		u32	nPlayerScore_sav = gGameVar.nBestScore;
		GameLoop(MISSIONOFFS_CREDITS);
		gGameVar.nBestScore = nPlayerScore_sav;
	}

	// Game Over.
	Music_Start(e_YmMusic_GameOver, 1);
	Menu(MenuGameOver_Init, MenuGameOver_Main);

	// High score ?
	if (gCCodes.nCheat == 0)	// Seulement si pas de cheat code !
	if (gGameVar.nExitCode == e_Game_GameOver || gGameVar.nExitCode == e_Game_AllClear)
	{
		if (Scr_CheckHighSc(gGameVar.nBestScore) >= 0)
		{
			Music_Start(e_YmMusic_HighScore, 1);
			// Saisie du nom.
			Menu(MenuGetName_Init, MenuGetName_Main);
			// Affichage de la table des high scores.
			Menu(MenuHighScores_Init, MenuHighScores_Main);
		}
	}
	Music_Start(e_YmMusic_NoMusic, 1);
}

// Lecture de tous les sprites.
#define	SLBAR_X	10
#define	SLBAR_Y	206
#define	SLBAR_W	300
#define	SLBAR_H	12
void SpritesLoad(void)
{
#if SPR_SAVE == 1

//#include <direct.h>		// _chdir()	// Windows
//#include <unistd.h>		// chdir()	// Linux

	char *pSprFiles[] =
	{
	"gfx/font_small1.psd", "gfx/font_mission0.psd",
	"gfx/hero_legs.psd", "gfx/hero_gun.psd", "gfx/hero_shotgun.psd", "gfx/hero_machgun.psd",
	"gfx/hero_sub_legs.psd", "gfx/hero_sub_gun.psd", "gfx/hero_sub_shotgun.psd",
	"gfx/hero_inslug.psd",
	"gfx/hero_shots.psd",
	"gfx/steps_dust.psd",
	"gfx/platforms.psd",
	"gfx/lev2_spr.psd",
	"gfx/lev3_spr.psd",
	"gfx/lev5_spr.psd",
	"gfx/lev6_spr.psd",
	"gfx/lev7_spr.psd",
	"gfx/lev8_spr.psd",
	"gfx/lev9_spr.psd",
	"gfx/lev11_spr.psd",
	"gfx/lev12_spr.psd",
	"gfx/lev13_spr.psd",
	"gfx/lev14_spr.psd",
	"gfx/lev16_spr.psd",
	"gfx/items1.psd",
	"gfx/debris0.psd",
	"gfx/jellyfish_small0.psd", "gfx/squid0.psd", "gfx/whale0.psd", "gfx/jellyfish_big0.psd", "gfx/piranha0.psd",
	"gfx/sensormine0.psd",
	"gfx/rebsoldier_death0.psd",
	"gfx/rebsoldier_std0.psd",
	"gfx/rebsoldier_rifle0.psd",
	"gfx/rebsoldier_mortar0.psd",
	"gfx/rebsoldier_lrac0.psd",
	"gfx/rebsoldier_shield0.psd",
	"gfx/rebsoldier_sunbath0.psd",
	"gfx/rebsoldier_bike0.psd",
	"gfx/rocketdiver0.psd",
	"gfx/girida0.psd",
	"gfx/brainbot0.psd",
	"gfx/marspeople0.psd", "gfx/marsufo0.psd", "gfx/miniufo0.psd",
	"gfx/asteroids0.psd", "gfx/bigasteroid0.psd", "gfx/mothership0.psd",
	"gfx/hostages0.psd", "gfx/aikawarumi0.psd",
	"gfx/rshobu0.psd", "gfx/masknell0.psd", "gfx/flyingtara0.psd", "gfx/hairbusterribert0.psd",
	"gfx/zombie_brain0.psd", "gfx/zombie_fat0.psd", "gfx/zombie_bits0.psd",
	"gfx/vulcan0.psd",
	"gfx/slug_mariner.psd", "gfx/slug_tank.psd", "gfx/slug_copter.psd", "gfx/slug_rocket.psd",
	"gfx/truck0.psd",
	"gfx/suspsub0.psd",
	"gfx/boss1.psd", "gfx/boss2.psd", "gfx/core0.psd", "gfx/monoeye0.psd",
	"gfx/hud.psd", "gfx/mse0.psd",
	"gfx/testrect.psd",		//"gfx/testrect.bmp",
	"gfx/howtoplay_ctrl0.psd",
	"gfx/logo32.psd",
	};
	u32	i, nNbFiles;
	SDL_Rect	rRct;
	SDL_Surface	*pBkg;

	// Blitte l'image de la disquette à l'écran.
	if ((pBkg = SDL_LoadBMP("gfx/bkg_disk.bmp")) == NULL) {
		fprintf(stderr, "Couldn't load picture 'bkg_disk.bmp': %s\n", SDL_GetError());
		exit(1);
	}
	SDL_BlitSurface(pBkg, NULL, gVar.pScreen, NULL);
	SDL_FreeSurface(pBkg);
	// Préparation des prm fixes de l'indicateur.
	rRct.x = SLBAR_X;
	rRct.y = SLBAR_Y;
	rRct.h = SLBAR_H;

	// Capture.
	nNbFiles = sizeof(pSprFiles)/sizeof(pSprFiles[0]);
	SprInitEngine();
	// Loop.
	for (i = 0; i < nNbFiles; i++)
	{
		// Indicateur de chargement.
		rRct.w = (SLBAR_W * (i + 1)) / nNbFiles;
		SDL_FillRect(gVar.pScreen, &rRct, SDL_MapRGB(gVar.pScreen->format, 255, 0, 0));
		//SDL_Flip(gVar.pScreen);
		RenderFlip(0);

		// Lecture planche de sprites.
		SprLoadGfx(pSprFiles[i]);
	}
	SprBinariesSave();
#else
	SprInitEngine();		// Pour initialiser gnSprSto.
	SprBinariesLoad();
#endif
	SprEndCapture();

}


//NDEBUG	// Définir pour supprimer les asserts !
//gcc -DNDEBUG

#ifndef NDEBUG
void Mst00CheckStructSizes(void);
void Mst10CheckStructSizes(void);
void Mst20CheckStructSizes(void);
void Mst30CheckStructSizes(void);
void Mst40CheckStructSizes(void);
void Mst50CheckStructSizes(void);
void BossesCheckStructSizes(void);
#endif
s32 Level_RealNumber(u32 nLevelNo);
u32 ChecksumVerify(u32 nLevNo);
u32 ChecksumCalc(u8 *pBuf, u32 nSz);

//#define	EXE_CHECKSUM	1		// Commenter la ligne pour virer le test.

#ifdef EXE_CHECKSUM
// Recherche d'une chaîne dans un buffer binaire.
u8 * StrFind(u8 *pToFind, u32 nToFindSz, u8 *pToSearch, u32 nToSearchSz)
{
	if (nToFindSz > nToSearchSz) return (NULL);

	u32	i;
	for (i = 0; i <= nToSearchSz - nToFindSz; i++)
	{
		// On ne compare que le premier char pour commencer.
		if (*pToSearch == *pToFind)
			if (strncmp((char *)pToSearch, (char *)pToFind, nToFindSz) == 0) return (pToSearch);
		pToSearch++;
	}
	return (NULL);
}
// Teste le checksum de l'exe.
void ExeChecksumTst(char *pFilename)
{
	FILE	*fPt = NULL;
	u8	*pBuf = NULL;

	// Ouverture du fichier.
	if ((fPt = fopen(pFilename, "rb")) == NULL)
	{
		fprintf(stderr, "Error opening file '%s'.\n", pFilename);
		goto _err_exit1;
	}
	// Récupération de la taille du fichier.
	s32	nFileSzToLoad, nSz;
	fseek(fPt, 0L, SEEK_END);
	nFileSzToLoad = ftell(fPt);
	fseek(fPt, 0L, SEEK_SET);
//printf("fsize = %d\n", nFileSzToLoad);
	// Malloc du buffer de lecture.
	if ((pBuf = (u8 *)malloc(nFileSzToLoad)) == NULL)
	{
		fprintf(stderr, "malloc failed (pBuf).\n");
		goto _err_exit1;
	}
	// Lecture.
	nSz = fread(pBuf, 1, nFileSzToLoad, fPt);
	if (nSz != nFileSzToLoad)
	{
		fprintf(stderr, "fread error.\n");
		goto _err_exit1;
	}
	fclose(fPt); fPt = NULL;

	// Recherche de la chaîne qui va bien.
	static char	*pStrToFind = "CC\x17\x02****";		// !!! Attention à bien laisser les 4 étoiles pour poke externe du checksum !!!
	u8	*pPtr;

	pPtr = StrFind((u8 *)pStrToFind, 4, pBuf, nFileSzToLoad);
	if (pPtr == NULL)
	{
		fprintf(stderr, "File '%s' has been altered. Aborted.\n", pFilename);
		goto _err_exit1;
	}
//printf("Substring found at offset %X\n", (int)(pPtr - pBuf));
	// Checksum du fichier.
	u32	nSum0 = *(u32 *)(pPtr + 4);
	// On efface les 4 cases du checksum.
	memset(pPtr + 4, 0, 4);
	// Recalcul du Checksum.
	u32	nSum1 = ChecksumCalc(pBuf, nFileSzToLoad);
//printf("Exe checksum = %X\n", nSum1);
	free(pBuf); pBuf = NULL;

	// Ok ?
	if (nSum0 != nSum1)
	{
		fprintf(stderr, "File '%s' has been altered. Aborted.\n", pFilename);
		exit(1);
	}
	return;

_err_exit1:
	if (fPt != NULL) fclose(fPt);
	if (pBuf != NULL) free(pBuf);
	exit (1);

}
#endif


// Point d'entrée.
int main(int argc, char *argv[])
{
	u32	nLoop;
	u32	nMenuVal;
	u32	i;

#ifndef NDEBUG
	// Debug : Vérifie la taille des structures spécifiques des monstres.
	Mst00CheckStructSizes();
	Mst10CheckStructSizes();
	Mst20CheckStructSizes();
	Mst30CheckStructSizes();
	Mst40CheckStructSizes();
	Mst50CheckStructSizes();
	BossesCheckStructSizes();
#endif

#ifdef EXE_CHECKSUM
	// Test d'intégrité sur l'exe.
	ExeChecksumTst(argv[0]);
#endif
	// Test d'intégrité sur les fichiers EDT.
	ChecksumVerify(Level_RealNumber(MISSIONOFFS_HOWTOPLAY));
	ChecksumVerify(Level_RealNumber(MISSIONOFFS_CREDITS));
	i = 0;
	while (Level_RealNumber(MISSIONOFFS_LEVELS + i) > 0)
	{
		ChecksumVerify(Level_RealNumber(MISSIONOFFS_LEVELS + i));
		i++;
	}


	// SDL Init.
//	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	// atexit : Quand on quittera (exit, return...), SDL_Quit() sera appelée.
	atexit(SDL_Quit);

#ifdef	RENDER_BPP
	const SDL_VideoInfo *pVideoInfo = SDL_GetVideoInfo();
	gRender.nRenderBPP = (pVideoInfo->vfmt->BitsPerPixel > 16 ? 1 : 0);
//printf("bits pp = %d / v=%d\n", pVideoInfo->vfmt->BitsPerPixel, gRender.nRenderBPP);
//printf("bytes pp = %d\n", pVideoInfo->vfmt->BytesPerPixel);
#endif

	// Video mode init.
	Render_InitVideo();
	SDL_WM_SetCaption("Mini Slug Project by 17o2!!", NULL);	// Nom de la fenêtre.

	// Lecture du fichier de conf.
	CfgLoad();
	// Passe dans le mode video de la conf.
	if (gMSCfg.nVideoMode != gRender.nRenderMode)
	{
		gRender.nRenderMode = gMSCfg.nVideoMode;
		Render_SetVideoMode();
	}


	// Init joystick.
	gVar.pJoystick = NULL;
	gVar.nJoystickState = 0;
	if (SDL_NumJoysticks() > 0)
	{
		SDL_JoystickEventState(SDL_ENABLE);
		gVar.pJoystick = SDL_JoystickOpen(0);		// Si pas NULL, joystick !
		if (gVar.pJoystick != NULL)
		{
			u32	nJoystickNumButtons = SDL_JoystickNumButtons(gVar.pJoystick);
			gVar.nJoystickNoHat = (SDL_JoystickNumHats(gVar.pJoystick) < 1 ? 1 : 0);
			gVar.nJoystickNoAxes = (SDL_JoystickNumAxes(gVar.pJoystick) < 2 ? 1 : 0);

			// Config boutons ok ?
			if (gMSCfg.pKeys[e_CfgKey_JoyButtonA] >= nJoystickNumButtons ||
				gMSCfg.pKeys[e_CfgKey_JoyButtonB] >= nJoystickNumButtons ||
				gMSCfg.pKeys[e_CfgKey_JoyButtonC] >= nJoystickNumButtons)
			{
				fprintf(stderr, "Joystick configuration can't be use with this joystick.\nPlease run the configuration program.\n");
				goto _JoyOff;
			}
			// Hardware ok ?
			if (nJoystickNumButtons < 3 || (gVar.nJoystickNoHat && gVar.nJoystickNoAxes))
			{
				fprintf(stderr, "Unable to handle joystick.\n");
_JoyOff:
				fprintf(stderr, "Joystick disabled.\n");
				// Pas assez de boutons ? => Disable.
				SDL_JoystickClose(gVar.pJoystick);
				gVar.pJoystick = NULL;
			}
		}
	}


	gnFrame = 0;	// RAZ compteur de frames.
	// Init du clavier.
	gVar.pKeysSDL = SDL_GetKeyState(NULL);
	memset(gVar.pKeys, 0, SDLK_LAST);
	// Allocation des buffers de scroll.
	ScrollAllocate();

	// Preca Sinus et Cosinus.
	PrecaSinCos();

	SDL_ShowCursor(SDL_DISABLE);	// Cache le pointeur de la souris.

	// Load sprites.
	SpritesLoad();

	// Load menus backgound pictures.
//	char	*pBkgMenFilenames[MENU_NbBkg] = { "gfx/bkg1.bmp", "gfx/gameover320.bmp" };
	char	*pBkgMenFilenames[MENU_NbBkg] = { "gfx/bkg1.psd", "gfx/gameover320.psd" };
	for (i = 0; i < MENU_NbBkg; i++)
	{
//		if ((gVar.pBkg[i] = SDL_LoadBMP(pBkgMenFilenames[i])) == NULL) {
		if ((gVar.pBkg[i] = PSDLoadToSDLSurf(pBkgMenFilenames[i])) == NULL) {
			fprintf(stderr, "Couldn't load picture '%s': %s\n", pBkgMenFilenames[i], SDL_GetError());
			exit(1);
		}
	}
	// Conversion en 16 bits de bkg1.
	SDL_Surface	*pConv1;
	pConv1 = SDL_ConvertSurface(gVar.pBkg[0], gVar.pScreen->format, SDL_SWSURFACE);
	if (pConv1 == NULL)
	{
		fprintf(stderr, "bkg: 16 bits conversion failed.\n");
		exit(1);
	}
	SDL_FreeSurface(gVar.pBkg[0]);	// Libère le gfx 8 bits.
	gVar.pBkg[0] = pConv1;
	//
	gVar.pBackground = gVar.pBkg[0];
	// Lecture du GIF animé de présentation.
	if ((gVar.pGif = GIF_Load("gfx/ms0.gif")) == NULL)
	{
		fprintf(stderr, "main(): FATAL: GIF_Load() returned NULL.\n");
		exit(1);
	}


	// Init sound.
	Sfx_SoundInit();
	Sfx_LoadWavFiles();
	Sfx_LoadYMFiles();
	Sfx_SoundOn();	// Starts playback.

	Transit2D_Reset();
	gCCodes.nCheat = 0;		// RAZ cheat codes.
	gCCodes.nLevel = 0;

	//
	MenuInit();
	Scr_Load();				// Lecture de la table des high-scores.
	Credits_NextSel();		// Init du nb de crédits à utiliser dans une partie.

//	SDL_ShowCursor(SDL_DISABLE);	// Cache le pointeur de la souris.


/*
*/
//SDL_PumpEvents();
//SDL_Event	ev;
//while (SDL_PollEvent(&ev));
//if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1))
//if (gVar.pKeysSDL[SDLK_RETURN])
//	printf("hidden part!!!\n");


	// Boucle infinie.
	nMenuVal = MENU_Main;//MENU_Game;
	nLoop = 1;
	while (nLoop)
	{
		switch (nMenuVal)
		{
		case MENU_Main:		// Main menu.
			nMenuVal = Menu(MenuMain_Init, MenuMain_Main);
			break;

		case MENU_Game:		// Jeu.
			Game();
			nMenuVal = MENU_Main;
			break;

		case MENU_HallOfFame:	// High scores.
			Menu(MenuHighScores_Init, MenuHighScores_Main);
			nMenuVal = MENU_Main;
			break;

		case MENU_HowToPlay:	// How to play.
			GameLoop(MISSIONOFFS_HOWTOPLAY);
			for (i = 0; i < e_CfgKey_LAST; i++) gVar.pKeys[gMSCfg.pKeys[i]] = 0;	// RAZ Kb (utile slt avec Esc).
			nMenuVal = MENU_Main;
			break;

		case MENU_Quit:		// Sortie.
			nLoop = 0;
			break;
		}

	}


	SDL_ShowCursor(SDL_ENABLE);		// Réautorise l'affichage du curseur de la souris.

	if (gVar.pJoystick != NULL) SDL_JoystickClose(gVar.pJoystick);

	Sfx_SoundOff();	// Stops playback.
	Sfx_FreeWavFiles();	// Libère les ressources des fx.
	Sfx_FreeYMFiles();	// Libère les ressources des YMs.

	// Libère les ressources des sprites.
	SprRelease();
	// Libère les buffers de scroll.
	ScrollRelease();
	// Free the allocated surfaces.
	for (i = 0; i < MENU_NbBkg; i++)
	{
		SDL_FreeSurface(gVar.pBkg[i]);
	}
	// Libère les ressources de rendu.
	RenderRelease();
	// Libère le Gif.
	GIF_Free(gVar.pGif);

	return (0);
}
