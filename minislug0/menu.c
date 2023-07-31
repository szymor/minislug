// Menus.

#include "includes.h"

#define	MENU_Bkg_Mvt	1

enum
{
	MENU_State_FadeIn = 0,
	MENU_State_Input,
	MENU_State_FadeOut,
};
#define	FADE_Step	8

#define HISC_Nb	10
#define HISC_NameLg (16+1)
#define HISC_Filename "high.scr"

#pragma pack(1)
struct SScore
{
	char	pName[HISC_NameLg];
	u32	nScore;
};
struct SScore	gpHighScores[HISC_Nb];
#pragma pack()


struct SMenuItm
{
	u32	nMenuVal;
	u32	nLg;		// Largeur en pixels du texte (pour centrage).
	char	*pTxt;
};


#define CURS_Acc 0x200

struct SMenuGen
{
	u32	nChoix;
//	s32	nOldMousePosX, nOldMousePosY;		// Ancienne position de la souris.
	u32	nState;
	s32	nFadeVal;

	u32	nFrmCnt0;

	s16	nCursPos;		// Pour faire bouger le curseur.
	s16	nCursAccel;

	u32	nTimerGen;		// Timer pour changer d'écran quand inactivité.

	// Variables pour saisie du nom.
	u32	nScIdx;				// Pos dans la chaîne. / Pas remis à 0 !
	char	pScName[HISC_NameLg];	// Nom saisi. / Pas remis à 0 !
	u8	nRank;
	u8	nKeyDown;

	u8	nHighScoreBlinkIdx;	// N° du score à faire clignoter.

	u8	nHiSc_ArrivalType;	// Style d'arrivée des scores.
	u8	nMain_Pos;			// Pour sauvegarde du curseur du main menu. Si plusieurs menus a choix, faire une table de Menu_Max cases.
	u8	nMain_SinIdx;		// Pour bouffonerie sur le main.

	s8	nAnm0;	// Anim Marco Win pour hiscore get name.
#ifdef HIGH_NoKeyboard
	u8	nHiCursPos;			// Position du viseur (hi score sans clavier).
	u8	nHiAltLtr;			// Alternate letters.
#endif

};
struct SMenuGen	gMenu;

//=============================================================================

// Inits de trucs généraux, appelé une fois en début de programme.
void MenuInit(void)
{
	gMenu.nScIdx = 0;		// RAZ entrée du nom pour High Score.
	gMenu.nHighScoreBlinkIdx = -1;		// N° du score à faire clignoter. Reset.

	gMenu.nHiSc_ArrivalType = 0;	// Style d'arrivée des scores.
	gMenu.nMain_Pos = 0;			// Pour sauvegarde du curseur du main menu. Si plusieurs menus a choix, faire une table de Menu_Max cases.

}

// Curseur - Init.
void CursorInit(void)
{
	gMenu.nCursPos = 0;
	gMenu.nCursAccel = CURS_Acc;
}

// Curseur - Déplacement.
void CursorMove(void)
{
	gMenu.nCursAccel -= 0x20;		// Gravité.
	gMenu.nCursPos += gMenu.nCursAccel;
	if (gMenu.nCursPos < 0) CursorInit();
}

// Curseur - Renvoie la position (appel externe).
s32 CursorGetPos(void)
{
	return (gMenu.nCursPos);
}

//=============================================================================

#define	MENU_TIMER_GEN	(64*5)

// RAZ du compteur.
void MenuTimer_Reset(void)
{
	if (gMenu.nState == MENU_State_FadeOut) return;		// Pas de reset pendant les fermetures.
	gMenu.nTimerGen = 0;		// Timer pour changer d'écran quand inactivité.
}

// Le compteur avance.
// Out: 1 = Max atteint. 0 sinon.
u32 MenuTimer_Cnt(u32 nMaxCnt)
{
	if (++gMenu.nTimerGen >= nMaxCnt) return (1);
	return (0);
}

// Init du fader. Appelé dans les Init de chaque menu.
void MenuInitFade(void)
{
	// Fader.
	gMenu.nState = MENU_State_FadeIn;
	gMenu.nFadeVal = 0;

	MenuTimer_Reset();

}

//=============================================================================

// Selectionne le nb de crédits (1 / 3 / infini).
void Credits_NextSel(void)
{
	static	u8	nCreditsSel = 0;		// Selecteur.
	static	s8	pTb[] = { 1, 3, -1 };

	if (++nCreditsSel >= sizeof(pTb)/sizeof(pTb[0])) nCreditsSel = 0;
	gVar.nCreditsToUse = pTb[nCreditsSel];		// Nb de crédits à utiliser dans la partie.

}

// Affichage du nombre de credits. !! Prévu pour chiffres de 0 à 9 !!
void Credits_Display(s32 nCreditsNb)
{
	char pStr[] = "CREDIT(S) -   ";

	if (nCreditsNb < 0)
		pStr[strlen(pStr) - 2] = ' ' + 94 + 1;		// Infini.
	else
		pStr[strlen(pStr) - 1] = '0' + nCreditsNb;	// Chiffre.

	Font_Print(SCR_Width - (12 * 10), SCR_Height - 4, pStr, 0);

}

//=============================================================================

extern	u8	gnFrameMissed;

// Affichage d'une image GIF.
void GIF_Display(struct SGIFFile *pGif, u32 nBkgColor, s32 nPosX, s32 nPosY)
{
	u16	pPal[256];
	s32	nXMin, nXMax, nYMin, nYMax;
	s32	nSprXMin, nSprXMax, nSprYMin, nSprYMax;
	s32	diff;

	if (gnFrameMissed) return;
	if (pGif == NULL || pGif->pLogicalScrDesc == NULL) return;

	// Préparation.
	nXMin = nPosX;
	nYMin = nPosY;
	nXMax = nPosX + pGif->pLogicalScrDesc->nLogScrWidth - 1;
	nYMax = nPosY + pGif->pLogicalScrDesc->nLogScrHeight - 1;

	nSprXMin = 0;
	nSprXMax = pGif->pLogicalScrDesc->nLogScrWidth - 1;
	nSprYMin = 0;
	nSprYMax = pGif->pLogicalScrDesc->nLogScrHeight - 1;

	// Clips.
	if (nXMin < 0)
	{
		diff = 0 - nXMin;
		nSprXMin += diff;
	}
	if (nXMax > SCR_Width - 1)
	{
		diff = nXMax - (SCR_Width - 1);
		nSprXMax -= diff;
	}
	// Sprite complètement en dehors ?
	if (nSprXMin - nSprXMax >= 0) return;
	//
	if (nYMin < 0)
	{
		diff = 0 - nYMin;
		nSprYMin += diff;
	}
	if (nYMax > SCR_Height - 1)
	{
		diff = nYMax - (SCR_Height - 1);
		nSprYMax -= diff;
	}
	// Sprite complètement en dehors ?
	if (nSprYMin - nSprYMax >= 0) return;


	SDL_LockSurface(gVar.pScreen);

	u16	*pScr = (u16 *)gVar.pScreen->pixels;
	u8	*pGfx = pGif->pImg;
	u32	nScrLg = gVar.pScreen->pitch / sizeof(u16);
	s32	ix, iy;

	// Conversion de la palette en format 16 bits.
	for (ix = 0; ix < 256; ix++) pPal[ix] = SDL_MapRGB(gVar.pScreen->format, pGif->pPal[ix * 3], pGif->pPal[ix * 3 + 1], pGif->pPal[ix * 3 + 2]);
	// Une couleur de transparence ?
	if (pGif->nTransparentColorIndex < 256) pPal[pGif->nTransparentColorIndex] = nBkgColor;

//l	pScr += ((nYMin + nSprYMin) * SCR_Width) + nXMin;
	pScr += ((nYMin + nSprYMin) * nScrLg) + nXMin;
	pGfx += (nSprYMin * pGif->pLogicalScrDesc->nLogScrWidth);

/*
printf("***\nSprXMin = %d / SprXMax = %d / SprYMin = %d / SprYMax = %d\n\
XMin = %d / XMax = %d / YMin = %d / YMax = %d\n", nSprXMin, nSprXMax, nSprYMin, nSprYMax,
nXMin, nXMax, nYMin, nYMax);
*/

/*
	// Version qui met une couleur à la place de la couleur de transparence. C'est le plus rapide, mais il faut un fond uni.
	for (iy = nSprYMin; iy <= nSprYMax; iy++)
	{
		for (ix = nSprXMin; ix <= nSprXMax; ix++)
			*(pScr + ix) = pPal[*(pGfx + ix)];
//			*(pScr + (iy * SCR_Width) + ix) =	// mais virer les 2 add qui suivent !
//				pPal[*(pGfx + (iy * pGif->pLogicalScrDesc->nLogScrWidth) + ix)];

//l		pScr += SCR_Width;
		pScr += nScrLg;
		pGfx += pGif->pLogicalScrDesc->nLogScrWidth;
	}
*/


	if (pGif->nTransparentColorIndex == 256)
	{
		// Pas de transparence.
		for (iy = nSprYMin; iy <= nSprYMax; iy++)
		{
			for (ix = nSprXMin; ix <= nSprXMax; ix++)
				*(pScr + ix) = pPal[*(pGfx + ix)];
			pScr += nScrLg;
			pGfx += pGif->pLogicalScrDesc->nLogScrWidth;
		}
	}
	else
	{
		// Version qui skippe la couleur transparente.
		for (iy = nSprYMin; iy <= nSprYMax; iy++)
		{
			ix = nSprXMin;
			// Skippe les pixels transparents.
_SkipTrans:
			while (ix <= nSprXMax && *(pGfx + ix) == pGif->nTransparentColorIndex) ix++;
			// Les autres pixels.
			for (; ix <= nSprXMax; ix++)
			{
				if (*(pGfx + ix) == pGif->nTransparentColorIndex) goto _SkipTrans;
				*(pScr + ix) = pPal[*(pGfx + ix)];
			}
			pScr += nScrLg;
			pGfx += pGif->pLogicalScrDesc->nLogScrWidth;
		}
	}

	SDL_UnlockSurface(gVar.pScreen);

}

// Bkg qui scrolle, image 128 x 128.
void Bkg1Scroll(u32 nOffsetX, u32 nOffsetY)
{
	u32 i, j;
	SDL_Rect	sSrc, sDst;

	if (gnFrameMissed) return;

	sSrc.y = nOffsetY & 0x3F;
	sSrc.h = gVar.pBackground->h - sSrc.y;
	sDst.y = 0;
	for (j = 0; sDst.y < SCR_Height; j++)
	{
		sSrc.x = nOffsetX & 0x3F;
		sSrc.w = gVar.pBackground->w - sSrc.x;
		sDst.x = 0;
		for (i = 0; sDst.x < SCR_Width; i++)
		{
			SDL_BlitSurface(gVar.pBackground, &sSrc, gVar.pScreen, &sDst);
			sSrc.x = 0;
			sDst.x += sSrc.w;
			sSrc.w = gVar.pBackground->w;
		}
		sSrc.y = 0;
		sDst.y += sSrc.h;
		sSrc.h = gVar.pBackground->h;
		//if (sDst.y + sSrc.h >= SCR_Height) sSrc.h = SCR_Height - sDst.y;	// Test ok, mais autant laisser SDL clipper. Ou sinon il faut le faire en x aussi et à l'init.
	}
}

//=============================================================================

#define	MENUMAIN_SIN_MIN	0
#define	MENUMAIN_SIN_MAX	64
#define	MENUMAIN_SIN_SPD	1

#define	MENU_Main_StartLn	160

struct SMenuItm gpMenuItems_Main[] =
{
	{ MENU_Game, 0, "START" },
	{ MENU_HallOfFame, 0, "HALL OF FAME" },
	{ MENU_Quit, 0, "QUIT" },
};

// Menu main : Init.
void MenuMain_Init(void)
{
	u32	i;

	MenuInitFade();
	// Décor.
	gVar.pBackground = gVar.pBkg[0];

	Transit2D_InitOpening(e_Transit_Menu);

	// Récupère les longueurs des phrases.
	for (i = 0; i < NBELEM(gpMenuItems_Main); i++)
	{
		gpMenuItems_Main[i].nLg = Font_Print(0, 0, gpMenuItems_Main[i].pTxt, FONT_NoDisp);
	}

//	gMenu.nChoix = 0;
	gMenu.nChoix = gMenu.nMain_Pos;

	gMenu.nFrmCnt0 = 0;		// !!! Pour lecture du GIF image par image.

	CursorInit();
	gMenu.nMain_SinIdx = MENUMAIN_SIN_MIN;

}

#define	MENUMAIN_BkgClr_R	64
#define	MENUMAIN_BkgClr_G	56
#define	MENUMAIN_BkgClr_B	48

void CheatCodes_Display(void);
char * Level_NameGet(u32 nLevelNo);
s32 Level_RealNumber(u32 nLevelNo);

// Menu main : Main.
u32 MenuMain_Main(void)
{
	u32	nRetVal = MENU_Null;
	u32	i;

	// Selon l'état.
	switch (gMenu.nState)
	{
	case MENU_State_FadeIn:
		if (Transit2D_CheckEnd()) gMenu.nState = MENU_State_Input;
		break;

	case MENU_State_FadeOut:
//		if (Transit2D_CheckEnd()) nRetVal = gpMenuItems_Main[gMenu.nChoix].nMenuVal;
		if (Transit2D_CheckEnd())
		{
			if (MenuTimer_Cnt(MENU_TIMER_GEN))
			{
				// Time out.
				static	u8	nTimerChoice = 0;
				static	u8	pnTimerChoices[] = { MENU_HallOfFame, MENU_HowToPlay };
				if (++nTimerChoice >= sizeof(pnTimerChoices)/sizeof(pnTimerChoices[0])) nTimerChoice = 0;
				nRetVal = pnTimerChoices[nTimerChoice];
			}
			else
				nRetVal = gpMenuItems_Main[gMenu.nChoix].nMenuVal;		// Sortie normale, choix en cours.

			gMenu.nMain_Pos = gMenu.nChoix;		// Sauvegarde de la pos en cours du curseur.
		}
		break;

	case MENU_State_Input:
		{
			u32	nLastChoix = gMenu.nChoix | (gCCodes.nLevel << 8);

			// Selecteur de level ?
			if (gCCodes.nCheat & e_Cheat_LevelSelect)
			{
				if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Right]])
				{
					if (Level_RealNumber(MISSIONOFFS_LEVELS + gCCodes.nLevel + 1) > 0) gCCodes.nLevel++;
					gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Right]] = 0;
				}
				if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Left]])
				{
					if (gCCodes.nLevel > 0) gCCodes.nLevel--;
					gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Left]] = 0;
				}
			}

			// Déplacement du curseur.
//			u32	nLastChoix = gMenu.nChoix;
			if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Up]])
			{
				if (gMenu.nChoix > 0) gMenu.nChoix--;
				gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Up]] = 0;
			}
			if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]])
			{
				if (gMenu.nChoix < NBELEM(gpMenuItems_Main) - 1) gMenu.nChoix++;
				gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]] = 0;
			}

//			if (nLastChoix != gMenu.nChoix)
			nLastChoix ^= (gMenu.nChoix | (gCCodes.nLevel << 8));
			if (nLastChoix)
			{
				if (nLastChoix & 0xFF) CursorInit();	// Slt parce que c'est plus joli. Et uniquement sur haut/bas.
				Sfx_PlaySfx(FX_Menu_Move, e_SfxPrio_10);	// Sfx.
			}

			// Validation ?
			if (gVar.pKeys[SDLK_RETURN] || gVar.pKeys[SDLK_SPACE] ||
				gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] || gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonB]] || gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonC]] ||
				MenuTimer_Cnt(MENU_TIMER_GEN))
			{
				gMenu.nState = MENU_State_FadeOut;
				Transit2D_InitClosing(e_Transit_Menu);
				Sfx_PlaySfx(FX_Menu_Clic, e_SfxPrio_10);	// Sfx.
			}

			// Changement du nombre de credits ?
			if (gVar.pKeys[SDLK_F1])
			{
				Credits_NextSel();
				gVar.pKeys[SDLK_F1] = 0;
				Sfx_PlaySfx(FX_Menu_Move, e_SfxPrio_10);		// Sfx.
			}
		}
		break;
	}

	CursorMove();
	if (gMenu.nMain_SinIdx < MENUMAIN_SIN_MAX) gMenu.nMain_SinIdx += MENUMAIN_SIN_SPD;

	//>>> Affichage.

	// Clear screen.
//	SDL_FillRect(gVar.pScreen, 0, SDL_MapRGB(gVar.pScreen->format, MENUMAIN_BkgClr_R, MENUMAIN_BkgClr_G, MENUMAIN_BkgClr_B));

	// Bkg qui scrolle, image 128 x 128.
	Bkg1Scroll(-gnFrame >> 1, -gnFrame >> 1);

	// Image suivante dans le GIF animé.
	if ((gMenu.nFrmCnt0++ & 3) == 0) GIF_GetNextImage(gVar.pGif);
	// Affichage.
//	GIF_Display(gVar.pGif, SDL_MapRGB(gVar.pScreen->format, MENUMAIN_BkgClr_R, MENUMAIN_BkgClr_G, MENUMAIN_BkgClr_B), 26, -33);
	GIF_Display(gVar.pGif, SDL_MapRGB(gVar.pScreen->format, MENUMAIN_BkgClr_R, MENUMAIN_BkgClr_G, MENUMAIN_BkgClr_B), 26 + (255 + gVar.pSin[gMenu.nMain_SinIdx]), -33);

	// Logo MS en haut à gauche.
//	SprDisplayAbsolute(e_Spr_MS_Logo, 10, 10, 200);
	SprDisplayAbsolute(e_Spr_MS_Logo, 10 - (255 + gVar.pSin[gMenu.nMain_SinIdx]), 10, 200);

	// Menu.
	u32	nFontFlag;
	for (i = 0; i < NBELEM(gpMenuItems_Main); i++)
	{
		nFontFlag = 0;
		// Selecteur.
		if (i == gMenu.nChoix)
		{
			nFontFlag = FONT_Highlight;
//			Font_Print((SCR_Width / 2) - (gpMenuItems_Main[i].nLg / 2) - 18+4 - (gMenu.nCursPos >> 8), MENU_Main_StartLn + (i * 12), ">", nFontFlag);
//			Font_Print((SCR_Width / 2) + (gpMenuItems_Main[i].nLg / 2) + 10-4 + (gMenu.nCursPos >> 8), MENU_Main_StartLn + (i * 12), "<", nFontFlag);
			static char *pCursL = "\x81";	// ' ' + 94 + 3
			static char *pCursR = "\x80";	// ' ' + 94 + 2
			Font_Print((SCR_Width / 2) - (gpMenuItems_Main[i].nLg / 2) - 18+4 - (gMenu.nCursPos >> 8), MENU_Main_StartLn + (i * 12), pCursL, nFontFlag);
			Font_Print((SCR_Width / 2) + (gpMenuItems_Main[i].nLg / 2) + 10-4 + (gMenu.nCursPos >> 8), MENU_Main_StartLn + (i * 12), pCursR, nFontFlag);
		}
		Font_Print((SCR_Width / 2) - (gpMenuItems_Main[i].nLg / 2), MENU_Main_StartLn + (i * 12), gpMenuItems_Main[i].pTxt, nFontFlag);
	}

	// Selecteur de level ?
	if (gCCodes.nCheat & e_Cheat_LevelSelect)
	{
		char	*pLvlName = Level_NameGet(MISSIONOFFS_LEVELS + gCCodes.nLevel);
		i = Font_Print(0, 0, pLvlName, FONT_NoDisp);
		Font_Print((SCR_Width / 2) - (i / 2), MENU_Main_StartLn - (1 * 12), pLvlName, 0);
	}

	// Les credits.
	Credits_Display(gVar.nCreditsToUse);
	// Les cheats.
	CheatCodes_Display();

	return (nRetVal);

}

//=============================================================================

// Le pixeliseur.
// In: nPixSz [1;64]
void Pixelizer(SDL_Surface *pSurf8, u32 nPixSz)
{
	u32	ix, iy;
	u32	nPosX, nPosY, nResetPosX;
	u32	nOffsX, nOffsY;
	u32	nZoom;
	u16	*pScr;
	u16	nClr;
	u32	nRemLn;

	u16	pPal[256];

	// La palette.
	for (ix = 0; ix < 256; ix++)
	{
		pPal[ix] = SDL_MapRGB(gVar.pScreen->format,
			(pSurf8->format->palette->colors[ix].r * (256+4 - (nPixSz * 4))) / 256,
			(pSurf8->format->palette->colors[ix].g * (256+4 - (nPixSz * 4))) / 256,
			(pSurf8->format->palette->colors[ix].b * (256+4 - (nPixSz * 4))) / 256);
	}

	// L'effet.
	SDL_LockSurface(gVar.pScreen);
	pScr = (u16 *)gVar.pScreen->pixels;
	nRemLn = (gVar.pScreen->pitch / sizeof(u16)) - SCR_Width;

	nZoom = 0x1000 / nPixSz;

	nResetPosX = (((SCR_Width / 2) << 12) - (nZoom * (SCR_Width / 2))) & 0xFFF;
	nPosY = (((SCR_Height / 2) << 12) - (nZoom * (SCR_Height / 2))) & 0xFFF;
	nOffsY = (nPixSz / 2);
	for (iy = 0; iy < SCR_Height; iy++)
	{
		nPosX = nResetPosX;
		nOffsX = (nPixSz / 2);
		nClr = pPal[*((u8 *)pSurf8->pixels + ((nOffsY) * pSurf8->pitch) + (nOffsX))];
		for (ix = 0; ix < SCR_Width; ix++)
		{
//			*pScr++ = pPal[*((u8 *)pSurf8->pixels + ((nOffsY) * pSurf8->pitch) + (nOffsX))];
			*pScr++ = nClr;
			nPosX += nZoom;
			if (nPosX & ~0xFFF)
			{
				nOffsX = ix + (nPixSz / 2);
				if (nOffsX >= pSurf8->w) nOffsX = pSurf8->w - 1;
				nPosX &= 0xFFF;
				nClr = pPal[*((u8 *)pSurf8->pixels + ((nOffsY) * pSurf8->pitch) + (nOffsX))];
			}
		}
		pScr += nRemLn;		// Au cas où le pitch soit plus grand que l'écran.
		nPosY += nZoom;
		if (nPosY & ~0xFFF)
		{
			nOffsY = iy + (nPixSz / 2);
			if (nOffsY >= pSurf8->h) nOffsY = pSurf8->h - 1;
			nPosY &= 0xFFF;
		}
	}

	SDL_UnlockSurface(gVar.pScreen);

}

#define	GAMEOVER_TIMEOUT	(60)	//(180)

// Game Over : Init.
void MenuGameOver_Init(void)
{
	gMenu.nState = MENU_State_FadeIn;
	gMenu.nFadeVal = 64 + 1;
	gMenu.nFrmCnt0 = GAMEOVER_TIMEOUT;

}

// Game Over : Main.
u32 MenuGameOver_Main(void)
{
	u32	nRetVal = MENU_Null;

	switch (gMenu.nState)
	{
	case MENU_State_FadeIn:
		if (--gMenu.nFadeVal == 1) gMenu.nState = MENU_State_Input;
		break;

	case MENU_State_FadeOut:
		if (++gMenu.nFadeVal >= 64) nRetVal = MENU_Main;
		break;

	case MENU_State_Input:
		if (Music_IsOver()) gMenu.nFrmCnt0--;	// On attend la fin de la musique avant de compter.

//		if (--gMenu.nFrmCnt0 == 0 ||
		if (gMenu.nFrmCnt0 == 0 ||
			gVar.pKeys[SDLK_RETURN] || gVar.pKeys[SDLK_SPACE] ||
//			gVar.pKeys[KB_BUTTONA] || gVar.pKeys[KB_BUTTONB] || gVar.pKeys[KB_BUTTONC]) gMenu.nState = MENU_State_FadeOut;
			gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] || gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonB]] || gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonC]])
				gMenu.nState = MENU_State_FadeOut;
		break;
	}

	//>>> Affichage.
	Pixelizer(gVar.pBkg[1], gMenu.nFadeVal);

	return (nRetVal);

}

//=============================================================================

// Scores - Check si un score entre au Hall of Fame.
// Out : -1, pas dedans / >= 0, rang.
s32 Scr_CheckHighSc(u32 nScorePrm)
{
	s32	i, nRank;

	nRank = -1;
	if (nScorePrm == 0) return (nRank);		// Si score == 0, on ne teste pas.
	for (i = HISC_Nb - 1; i >= 0; i--)
	{
		if (nScorePrm >= gpHighScores[i].nScore)
		{
			nRank = i;
		}
	}
	return (nRank);

}

// Insère un nom dans la table.
s32 Scr_PutNameInTable(char *pName, u32 nScore)
{
	s32	nRank = Scr_CheckHighSc(nScore);
	s32	i;

	if (nRank < 0) return (-1);		// Ne devrait pas arriver.

	// Décalage de la table.
	for (i = HISC_Nb - 2; i >= nRank; i--)
	{
		strcpy(gpHighScores[i + 1].pName, gpHighScores[i].pName);
		gpHighScores[i + 1].nScore = gpHighScores[i].nScore;
	}
	// Le score à insérer.
	strcpy(gpHighScores[nRank].pName, pName);
	gpHighScores[nRank].nScore = nScore;

	return (nRank);

}


// RAZ de la table des high scores.
void Scr_RazTable(void)
{
	static char	pDefStr[HISC_NameLg] = "----------------";
	u32	i;

	for (i = 0; i < HISC_Nb; i++)
	{
		strcpy(gpHighScores[i].pName, pDefStr);
		gpHighScores[i].nScore = 0;
	}

	// Init des premiers scores.
	static struct SScore pDefTb[] =
	{
//c		{ "Clement 17o2!!", 65050 },
//c		{ "Pouet !", 59050 },
		{ "Colonel", 50000 },
		{ "Lt-Colonel", 45000 },
		{ "Major", 40000 },
		{ "Captain", 35000 },
		{ "1st Lieutenant", 30000 },
		{ "2nd Lieutenant", 25000 },
	};
	for (i = 0; i < NBELEM(pDefTb); i++)
	{
		strcpy(gpHighScores[i].pName, pDefTb[i].pName);
		gpHighScores[i].nScore = pDefTb[i].nScore;
	}
//c	Scr_Save();

}

// Calcule le checksum de la table des scores.
u32 Scr_CalcChecksum(void)
{
	u32	i, j;
	u32	nChk = 0;

	for (i = 0; i < HISC_Nb; i++)
	{
		nChk += gpHighScores[i].nScore;
		for (j = 0; j < HISC_NameLg; j++) nChk += ((u32)gpHighScores[i].pName[j]) << (8 * (j & 3));
	}
	return (nChk);
}

// Lecture du fichier des high scores.
void Scr_Load(void)
{
	FILE	*pFile;
	u32	nChk;

	if ((pFile = fopen(HISC_Filename, "rb")) != NULL)
	{
		// Le fichier existe, lecture.
		fread(gpHighScores, sizeof(struct SScore), HISC_Nb, pFile);
		fread(&nChk, sizeof(u32), 1, pFile);
		fclose(pFile);
		// Checksum ok ?
		if (nChk != Scr_CalcChecksum())
		{
			// Wrong checksum, RAZ table.
			fprintf(stderr, "Scr_Load(): Wrong checksum! Resetting table.\n");
			Scr_RazTable();
		}
	}
	else
	{
		// Le fichier n'existe pas, RAZ table.
		Scr_RazTable();
	}

}

// Sauvegarde du fichier des high scores.
void Scr_Save(void)
{
	FILE	*pFile;
	u32	nChk;

	if ((pFile = fopen(HISC_Filename, "wb")) == NULL)
	{
		fprintf(stderr, "Scr_Save(): Unable to save highscores table.\n");
		return;
	}
	// Sauvegarde des enregistrements.
	fwrite(gpHighScores, sizeof(struct SScore), HISC_Nb, pFile);
	// Checksum.
	nChk = Scr_CalcChecksum();
	fwrite(&nChk, sizeof(u32), 1, pFile);
	fclose(pFile);

}

//=============================================================================

#define	HiSc_SIN_MIN	10
#define	HiSc_SIN_MAX	64
#define	HiSc_SIN_SPD	1
struct SBouffonerie3	// Effet de lignes.
{
	u32	nWait;
	s32	nPosYCur;
	u8	nSinIdx;

};
struct SBouffonerie3	gpBouf3[HISC_Nb];

#define	MENU_HiSc_Interligne	19
#define	MENU_HiSc_FigureSpc		9
// Menu des high-scores : Init.
void MenuHighScores_Init(void)
{
	u32	i;

	MenuInitFade();
	AnmInitEngine();	// Pour monstres.

	// Décor.
	gVar.pBackground = gVar.pBkg[0];

	// Init effet des lignes.
	for (i = 0; i < HISC_Nb; i++)
	{
		gpBouf3[i].nWait = i * 3;//4;//8;
		gpBouf3[i].nSinIdx = HiSc_SIN_MIN;
		gpBouf3[i].nPosYCur = SCR_Height + 16;
	}

	// Change le type d'arrivée.
	gMenu.nHiSc_ArrivalType++;
	gMenu.nHiSc_ArrivalType &= 3;

	Transit2D_InitOpening(e_Transit_Menu);

}

// Menu des high-scores : Main.
u32 MenuHighScores_Main(void)
{
	u32	nRetVal = MENU_Null;
	u32	i;
	s32 nPosX, nPosY;

	// Selon l'état.
	switch (gMenu.nState)
	{
	case MENU_State_FadeIn:
		if (Transit2D_CheckEnd()) gMenu.nState = MENU_State_Input;
		break;

	case MENU_State_FadeOut:
		if (Transit2D_CheckEnd())
		{
			nRetVal = MENU_Main;			// Sortie.
			gMenu.nHighScoreBlinkIdx = -1;	// N° du score à faire clignoter. Reset.
		}
		break;

	case MENU_State_Input:
		{
			// Validation ?
			u32 nTimeOut = (gpBouf3[HISC_Nb - 1].nSinIdx == HiSc_SIN_MAX ? MenuTimer_Cnt(MENU_TIMER_GEN) : 0);	// TMP : on ne compte que quand l'effet est terminé.
			if (gVar.pKeys[SDLK_RETURN] || gVar.pKeys[SDLK_SPACE] ||
//				gVar.pKeys[KB_BUTTONA] || gVar.pKeys[KB_BUTTONB] || gVar.pKeys[KB_BUTTONC] ||
				gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] || gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonB]] || gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonC]] ||
				nTimeOut)
			{
				gMenu.nState = MENU_State_FadeOut;
				Transit2D_InitClosing(e_Transit_Menu);
				// Sfx.
				Sfx_PlaySfx(FX_Menu_Clic, e_SfxPrio_10);
			}
		}
		break;
	}


	// Effet des lignes.
	if (gMenu.nState != MENU_State_FadeIn)
	{
		for (i = 0; i < HISC_Nb; i++)
		{
			if (gpBouf3[i].nWait == 0)
			{
				if (gpBouf3[i].nSinIdx < HiSc_SIN_MAX) gpBouf3[i].nSinIdx += HiSc_SIN_SPD;
				gpBouf3[i].nPosYCur = 41 + (i * MENU_HiSc_Interligne) + (256 + gVar.pSin[gpBouf3[i].nSinIdx]);
			}
			else
				gpBouf3[i].nWait--;
		}
	}


	//>>> Affichage.

	// Clear screen.
//	SDL_FillRect(gVar.pScreen, 0, SDL_MapRGB(gVar.pScreen->format, MENUMAIN_BkgClr_R, MENUMAIN_BkgClr_G, MENUMAIN_BkgClr_B));

	// Bkg qui scrolle, image 128 x 128.
	Bkg1Scroll(-gnFrame >> 1, -gnFrame >> 1);

	// Titre.
	char	pTitle[] = "- HALL OF FAME -";
	u32	nLg = Font_Print(0, 8, pTitle, FONT_NoDisp);
	Font_Print((SCR_Width - nLg) / 2, 24-7, pTitle, 0);

	// Affichage des lignes.
	for (i = 0; i < HISC_Nb; i++)
	{
		char	pStr[8+1];// = "00000000";
//		s32	nOfs;
		u32	nFontFlags = ((i == gMenu.nHighScoreBlinkIdx) && (gnFrame & 8) ? FONT_Highlight : 0);	// Pour clignotement du score qui vient d'être entré, si nécessaire.

		// Pos x et y en fct de l'effet.
		switch (gMenu.nHiSc_ArrivalType)
		{
		case 1:		// Arrivée verticale.
			nPosX = 16;
			nPosY = gpBouf3[i].nPosYCur;
			break;

		case 2:		// Arrivée horizontale entrelacée.
			nPosX = 16 + ((((256 + gVar.pSin[gpBouf3[i].nSinIdx]) * (i & 1 ? -1 : 1)) * 7) / 4);
			nPosY = 41 + (i * MENU_HiSc_Interligne);
			break;

		case 3:		// Arrivée verticale + offset x entrelacé.
			nPosX = 16 + (((256 + gVar.pSin[gpBouf3[i].nSinIdx]) * (i & 1 ? -1 : 1)) / 2);
			nPosY = gpBouf3[i].nPosYCur;
			break;

		default:	// Arrivée horizontale.
			nPosX = 16 + (((256 + gVar.pSin[gpBouf3[i].nSinIdx]) * 7) / 4);
			nPosY = 41 + (i * MENU_HiSc_Interligne);
			break;
		}

		// Pos.
		strcpy(pStr, "00");
		MyItoA(i + 1, pStr);
		Font_PrintSpc(nPosX, nPosY, pStr, nFontFlags, MENU_HiSc_FigureSpc);
		// Nom.
		Font_Print(nPosX + (8 * 3) + 4+4, nPosY, gpHighScores[i].pName, nFontFlags);

/*
		// Round.
		nOfs = 0;

//#define	LEVEL_Max_Old	37
		if (gpHighScores[i].nRound + 1 > LEVEL_Max_Old)
		{
			strcpy(pStr, "ALL");
			nOfs = -4;
		}
		else
		{
			strcpy(pStr, "00");
			MyItoA(gpHighScores[i].nRound + 1, pStr);
		}
		Font_Print(nPosX + (8 * 20) + 8+8 + nOfs, nPosY + (i * MENU_HiSc_Interligne), pStr, nFontFlags);
*/

		// Score.
		strcpy(pStr, "00000000");
		MyItoA(gpHighScores[i].nScore, pStr);
		Font_PrintSpc(nPosX + (8 * 23) + 12+12+9, nPosY, pStr, nFontFlags, MENU_HiSc_FigureSpc);

	}

	gVar.nFadeVal = gMenu.nFadeVal;
	return (nRetVal);

}

//=============================================================================
#ifdef HIGH_NoKeyboard

#define	KEY_ALT_LETTERS	(SDLK_LSHIFT)
#define	KEY_ENTER	(SDLK_RETURN)
#define	KEY_ALT_MODE	(0)			// 0 or 1.

#define	HIGN_OFFS1STLN	(32+4)		// Décalage de la première ligne affichée.

#define	HIGN_RANK_LN	(1)
struct SMenuItm gpMenuItems_GetName[] =
{
	{ 0, 0, "Congratulations!" },
	{ 0, 0, "You ranked #0@" },
	{ 0, 0, "Enter your name:" },
};

// Init.
void MenuGetName_Init(void)
{
	u32	i;

	MenuInitFade();
	AnmInitEngine();	// Pour anim spr.

	gMenu.nAnm0 = AnmSet(gAnm_Hero_Victory, -1);	// Anim Marco Win.
	gMenu.nHiCursPos = 0;			// Position du viseur (hi score sans clavier).
	gMenu.nHiAltLtr = 0;			// Alternate letters.

	// Rank atteint.
//s	gMenu.nRank = Scr_CheckHighSc(gShoot.nPlayerScore);
	gMenu.nRank = Scr_CheckHighSc(gGameVar.nBestScore);
	// Calcul de la longueur des chaînes.
	for (i = 0; i < NBELEM(gpMenuItems_GetName); i++)
	{
		gpMenuItems_GetName[i].nLg = Font_Print(0, 8, gpMenuItems_GetName[i].pTxt, FONT_NoDisp);
	}
	//
	gMenu.nKeyDown = 0;

	Transit2D_InitOpening(e_Transit_Menu);

}

// Saisie du nom quand high-score.
u32 MenuGetName_Main(void)
{
	u32	nRet = MENU_Null;
	u32	i, j;

	#define	HI_LTR_LG	(10)
	#define	HI_LTR_HT	(5)
	static	u8 pLettersUC[HI_LTR_LG * HI_LTR_HT] =
	{
		'0','1','2','3','4','5','6','7','8','9',
		'A','B','C','D','E','F','G','H','I','J',
		'K','L','M','N','O','P','Q','R','S','T',
		'U','V','W','X','Y','Z',' ','.',':','!',
		'-','+','*','/','[',']','<','>',' ',' '
	};
	static	u8 pLettersLC[HI_LTR_LG * HI_LTR_HT] =
	{
		'!','@','#','$','%','^','&','*','(',')',
		'a','b','c','d','e','f','g','h','i','j',
		'k','l','m','n','o','p','q','r','s','t',
		'u','v','w','x','y','z',' ',',',';','?',
		'_','=','*','/','{','}','<','>',' ',' '
	};
#if KEY_ALT_MODE == 0
	u8	*pLetters = pLettersUC;
#else
	u8	*pLetters = (gMenu.nHiAltLtr ? pLettersLC : pLettersUC);
#endif

	// Selon l'état.
	switch (gMenu.nState)
	{
	case MENU_State_FadeIn:
		if (Transit2D_CheckEnd()) gMenu.nState = MENU_State_Input;
		break;

	case MENU_State_FadeOut:
		if (Transit2D_CheckEnd())	// Sortie.
		{
			char	*pCurStr = gMenu.pScName;

			// Si pas de nom, mettre John Doe.
			static char	pDefName[HISC_NameLg] = "John DOE";
//			if (gMenu.nScIdx == 0 || MenuTimer_Cnt(MENU_TIMER_GEN)) pCurStr = pDefName;		// Fonctionne, mais bof. // Note on se fiche de l'incrémentation du timer à ce niveau. Soit on est passé avec return et ça passe de 0 à 1, soit on est au delà de la limite et c'est ce qu'on veut tester.
			if (gMenu.nScIdx == 0) pCurStr = pDefName;
			// Rajoute le nom dans les High-scores + note la ligne à faire clignoter pour l'affichage des high-scores qui va suivre.
//s			gMenu.nHighScoreBlinkIdx = Scr_PutNameInTable(pCurStr, gGameVar.nMissionNo, gGameVar.nContinueUsed + 1, gShoot.nPlayerScore);
			gMenu.nHighScoreBlinkIdx = Scr_PutNameInTable(pCurStr, gGameVar.nBestScore);
			Scr_Save();				// Sauvegarde du fichier des scores.

			nRet = MENU_Main;		// Sortie.
		}
		break;

	case MENU_State_Input:
		// Gestion du clavier.
		{
			// On termine la saisie ?
			if (gVar.pKeys[KEY_ENTER] || MenuTimer_Cnt(MENU_TIMER_GEN))
			{
				gVar.pKeys[KEY_ENTER] = 0;
				gMenu.nState = MENU_State_FadeOut;
				Transit2D_InitClosing(e_Transit_Menu);
				// Sfx.
				Sfx_PlaySfx(FX_Menu_Clic, e_SfxPrio_10);
				break;
			}

			// Alternate keys ?
#if KEY_ALT_MODE == 0
			if (gVar.pKeys[KEY_ALT_LETTERS])
				pLetters = pLettersLC;
#else
			if (gVar.pKeys[KEY_ALT_LETTERS])
			{
				gMenu.nHiAltLtr ^= 1;			// Alternate letters.
				pLetters = (gMenu.nHiAltLtr ? pLettersLC : pLettersUC);
				gVar.pKeys[KEY_ALT_LETTERS] = 0;
			}
#endif

			// Position du viseur.
			j = gMenu.nHiCursPos / HI_LTR_LG;
			i = gMenu.nHiCursPos % HI_LTR_LG;

			if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Right]])
			{
				if (++i >= HI_LTR_LG) i = 0;
				gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Right]] = 0;
			}
			if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Left]])
			{
				if (--i >= HI_LTR_LG) i = HI_LTR_LG - 1;
				gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Left]] = 0;
			}
			if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]])
			{
				if (++j >= HI_LTR_HT) j = 0;
				gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]] = 0;
			}
			if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Up]])
			{
				if (--j >= HI_LTR_HT) j = HI_LTR_HT - 1;
				gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Up]] = 0;
			}

			gMenu.nHiCursPos = (j * HI_LTR_LG) + i;

			u32	nChr = 0;

			// Backspace.
			if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonB]])
			{
				nChr = 0xFF;
			}
			// Lettre.
			if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]])
			{
				nChr = pLetters[gMenu.nHiCursPos];
			}
	
			// Pseudo trigger.
			if (gMenu.nKeyDown != nChr && nChr)
			{
				if (nChr == 0xFF)	// BACKSPACE
				{
					if (gMenu.nScIdx) gMenu.pScName[--gMenu.nScIdx] = 0;
				}
				else if (gMenu.nScIdx < HISC_NameLg - 1)
				{
					gMenu.pScName[gMenu.nScIdx++] = nChr;
					gMenu.pScName[gMenu.nScIdx] = 0;
				}
				gMenu.nKeyDown = nChr;
				Sfx_PlaySfx(FX_Menu_Move, e_SfxPrio_10);	// Sfx.
			}
			else if (nChr == 0)
			{
				gMenu.nKeyDown = 0;		// Release.
			}

		}
		break;
	}

	// Affichage.

	// Bkg qui scrolle, image 128 x 128.
	Bkg1Scroll(-gnFrame >> 1, -gnFrame >> 1);

	// On rajoute le rank dans sa ligne.
	char pRank[30];
	strcpy(pRank, gpMenuItems_GetName[HIGN_RANK_LN].pTxt);
	char *pPtr = strchr(pRank, '@');
	if (pPtr != NULL)
	{
		MyItoA(gMenu.nRank + 1, pPtr-1);
	}

	// Lignes.
	for (i = 0; i < NBELEM(gpMenuItems_GetName)-1; i++)
	{
//		Font_Print((SCR_Width - gpMenuItems_GetName[i].nLg) / 2, 80 + (i*32), (i == HIGN_RANK_LN ? pRank : gpMenuItems_GetName[i].pTxt), 0);
		Font_Print((SCR_Width - gpMenuItems_GetName[i].nLg) / 2, HIGN_OFFS1STLN + (i*16), (i == HIGN_RANK_LN ? pRank : gpMenuItems_GetName[i].pTxt), 0);
	}
	Font_Print((SCR_Width - gpMenuItems_GetName[i].nLg) / 2, HIGN_OFFS1STLN +48, gpMenuItems_GetName[i].pTxt, 0);
	// Nom en cours.
	i = Font_Print(0, 0, gMenu.pScName, FONT_NoDisp);
//	Font_Print((SCR_Width - i) / 2, 80+64+16, gMenu.pScName, 0);
	Font_Print((SCR_Width - i) / 2, HIGN_OFFS1STLN +48+16, gMenu.pScName, 0);
	// Curseur au bout du nom.
	if (gnFrame & 8)
	{
//		Font_Print(((SCR_Width - i) / 2) + i, 80+64+16, "_", 0);
		Font_Print(((SCR_Width - i) / 2) + i, HIGN_OFFS1STLN +48+16, "_", 0);
	}

	// Affichage du tableau de lettres.
	#define	HI_LTR_Isp	(16)
	#define	HI_LTR_OrgX	((SCR_Width / 2) - (((10 * HI_LTR_Isp) - 8) / 2))
	#define	HI_LTR_OrgY	(HIGN_OFFS1STLN +48+16+32)
	u32	c;
	for (j = 0; j < HI_LTR_HT; j++)
	for (i = 0; i < HI_LTR_LG; i++)
	{
		c = pLetters[(j * HI_LTR_LG) + i];
		if (c != ' ')
			SprDisplayAbsolute(e_Spr_FontSmall + c - ' ' - 1, HI_LTR_OrgX + (i * HI_LTR_Isp), HI_LTR_OrgY + (j * HI_LTR_Isp), e_Prio_HUD);
	}

	// Affichage du viseur.
	j = gMenu.nHiCursPos / HI_LTR_LG;
	i = gMenu.nHiCursPos % HI_LTR_LG;

	s32	nOffs = (gMenu.nKeyDown != 0 && gMenu.nKeyDown != 0xFF ? 1 : 0);

	SprDisplayAbsolute(e_Spr_Tstrct_CornerUL, HI_LTR_OrgX + (i * HI_LTR_Isp) -3 +nOffs, HI_LTR_OrgY + (j * HI_LTR_Isp) -10 +nOffs, e_Prio_HUD + 1);
	SprDisplayAbsolute(e_Spr_Tstrct_CornerUR, HI_LTR_OrgX + (i * HI_LTR_Isp) +9 -nOffs, HI_LTR_OrgY + (j * HI_LTR_Isp) -10 +nOffs, e_Prio_HUD + 1);
	SprDisplayAbsolute(e_Spr_Tstrct_CornerDL, HI_LTR_OrgX + (i * HI_LTR_Isp) -3 +nOffs, HI_LTR_OrgY + (j * HI_LTR_Isp) +2 -nOffs, e_Prio_HUD + 1);
	SprDisplayAbsolute(e_Spr_Tstrct_CornerDR, HI_LTR_OrgX + (i * HI_LTR_Isp) +9 -nOffs, HI_LTR_OrgY + (j * HI_LTR_Isp) +2 -nOffs, e_Prio_HUD + 1);

	// Sprite de Marco Win.
	i = 0;
	if (gMenu.nAnm0 != -1) i += AnmGetImage(gMenu.nAnm0);
	i += e_Spr_Hero_Victory_Machinegun;
	SprDisplayAbsolute(i, 30, (SCR_Height/2) + 80-8-4, e_Prio_Joueur);
	SprDisplayAbsolute(i ^ SPR_Flip_X, SCR_Width - 30, (SCR_Height/2) + 80-8-4, e_Prio_Joueur);


	gVar.nFadeVal = gMenu.nFadeVal;
	return (nRet);

}

//=============================================================================
#else

#define	HIGN_OFFS1STLN	58		// Décalage de la première ligne affichée.

#define	HIGN_RANK_LN	1
struct SMenuItm gpMenuItems_GetName[] =
{
	{ 0, 0, "Congratulations!" },
	{ 0, 0, "You ranked #0@" },
	{ 0, 0, "Enter your name:" },
};

// Init.
void MenuGetName_Init(void)
{
	u32	i;

	MenuInitFade();
	AnmInitEngine();	// Pour anim spr.

	gMenu.nAnm0 = AnmSet(gAnm_Hero_Victory, -1);	// Anim Marco Win.

	// Rank atteint.
//s	gMenu.nRank = Scr_CheckHighSc(gShoot.nPlayerScore);
	gMenu.nRank = Scr_CheckHighSc(gGameVar.nBestScore);
	// Calcul de la longueur des chaînes.
	for (i = 0; i < NBELEM(gpMenuItems_GetName); i++)
	{
		gpMenuItems_GetName[i].nLg = Font_Print(0, 8, gpMenuItems_GetName[i].pTxt, FONT_NoDisp);
	}
	//
	gMenu.nKeyDown = 0;

	Transit2D_InitOpening(e_Transit_Menu);

}

// Saisie du nom quand high-score.
u32 MenuGetName_Main(void)
{
	u32	nRet = MENU_Null;
	u32	i;

	// Selon l'état.
	switch (gMenu.nState)
	{
	case MENU_State_FadeIn:
		if (Transit2D_CheckEnd()) gMenu.nState = MENU_State_Input;
		break;

	case MENU_State_FadeOut:
		if (Transit2D_CheckEnd())	// Sortie.
		{
			char	*pCurStr = gMenu.pScName;

			// Si pas de nom, mettre John Doe.
			static char	pDefName[HISC_NameLg] = "John DOE";
//			if (gMenu.nScIdx == 0 || MenuTimer_Cnt(MENU_TIMER_GEN)) pCurStr = pDefName;		// Fonctionne, mais bof. // Note on se fiche de l'incrémentation du timer à ce niveau. Soit on est passé avec return et ça passe de 0 à 1, soit on est au delà de la limite et c'est ce qu'on veut tester.
			if (gMenu.nScIdx == 0) pCurStr = pDefName;
			// Rajoute le nom dans les High-scores + note la ligne à faire clignoter pour l'affichage des high-scores qui va suivre.
//s			gMenu.nHighScoreBlinkIdx = Scr_PutNameInTable(pCurStr, gGameVar.nMissionNo, gGameVar.nContinueUsed + 1, gShoot.nPlayerScore);
			gMenu.nHighScoreBlinkIdx = Scr_PutNameInTable(pCurStr, gGameVar.nBestScore);
			Scr_Save();				// Sauvegarde du fichier des scores.

			nRet = MENU_Main;		// Sortie.
		}
		break;

	case MENU_State_Input:
		// Gestion du clavier.
		{
			// On quitte ?
			if (gVar.pKeys[SDLK_RETURN] || MenuTimer_Cnt(MENU_TIMER_GEN))
			{
				gMenu.nState = MENU_State_FadeOut;
				Transit2D_InitClosing(e_Transit_Menu);
				// Sfx.
				Sfx_PlaySfx(FX_Menu_Clic, e_SfxPrio_10);
				break;
			}

			// On regarde quelle touche est enfoncée.
			u32	nChr = 0;
			// a-z.
			for (i = SDLK_a; i <= SDLK_z; i++)
			{
				if (gVar.pKeys[i])
				{
					nChr = i - SDLK_a + (gVar.pKeys[SDLK_RSHIFT] || gVar.pKeys[SDLK_LSHIFT] ? 'A' : 'a');
					break;
				}
			}
			// 0-9.
			for (i = SDLK_0; i <= SDLK_9; i++)
			{
				if (gVar.pKeys[i])
				{
					static char *pKeys09Shift = ")!@#$%^&*(";
					if (gVar.pKeys[SDLK_RSHIFT] || gVar.pKeys[SDLK_LSHIFT])
						nChr = pKeys09Shift[i - SDLK_0];
					else
						nChr = i - SDLK_0 + '0';
					break;
				}
			}
			// Keypad 0-9.
			for (i = SDLK_KP0; i <= SDLK_KP9; i++)
			{
				if (gVar.pKeys[i])
				{
					nChr = i - SDLK_KP0 + '0';
					break;
				}
			}
			// Backspace.
			if (gVar.pKeys[SDLK_BACKSPACE]) nChr = SDLK_BACKSPACE;
			// Touches spéciales.
			static	u16 pKeysSpeToTest[] = { SDLK_SPACE, SDLK_SEMICOLON, SDLK_PERIOD, SDLK_COMMA, SDLK_SLASH, SDLK_RIGHTBRACKET, SDLK_LEFTBRACKET, SDLK_EQUALS, SDLK_MINUS,
				SDLK_KP_DIVIDE, SDLK_KP_MULTIPLY, SDLK_KP_MINUS, SDLK_KP_PLUS, SDLK_KP_PERIOD };
			static	char *pKeysSpeNorm  = " ;.,/][=-/*-+.";
			static	char *pKeysSpeShift = " :><?}{+_/*-+.";
			for (i = 0; i < sizeof(pKeysSpeToTest)/sizeof(pKeysSpeToTest[0]); i++)
			{
				if (gVar.pKeys[pKeysSpeToTest[i]])
				{
					nChr = (gVar.pKeys[SDLK_RSHIFT] || gVar.pKeys[SDLK_LSHIFT] ? pKeysSpeShift[i] : pKeysSpeNorm[i]);
					break;
				}
			}

			// Pseudo trigger.
			if (gMenu.nKeyDown != nChr && nChr)
			{
				if (nChr == SDLK_BACKSPACE)
				{
					if (gMenu.nScIdx) gMenu.pScName[--gMenu.nScIdx] = 0;
				}
				else if (gMenu.nScIdx < HISC_NameLg - 1)
				{
					gMenu.pScName[gMenu.nScIdx++] = nChr;
					gMenu.pScName[gMenu.nScIdx] = 0;
				}
				gMenu.nKeyDown = nChr;
				Sfx_PlaySfx(FX_Menu_Move, e_SfxPrio_10);	// Sfx.
			}
			else if (nChr == 0)
			{
				gMenu.nKeyDown = 0;		// Release.
			}

		}
		break;
	}

	// Affichage.

	// Bkg qui scrolle, image 128 x 128.
	Bkg1Scroll(-gnFrame >> 1, -gnFrame >> 1);

	// On rajoute le rank dans sa ligne.
	char pRank[30];
	strcpy(pRank, gpMenuItems_GetName[HIGN_RANK_LN].pTxt);
	char *pPtr = strchr(pRank, '@');
	if (pPtr != NULL)
	{
		MyItoA(gMenu.nRank + 1, pPtr-1);
	}

	// Lignes.
	for (i = 0; i < NBELEM(gpMenuItems_GetName); i++)
	{
//		Font_Print((SCR_Width - gpMenuItems_GetName[i].nLg) / 2, 80 + (i*32), (i == HIGN_RANK_LN ? pRank : gpMenuItems_GetName[i].pTxt), 0);
		Font_Print((SCR_Width - gpMenuItems_GetName[i].nLg) / 2, HIGN_OFFS1STLN + (i*32), (i == HIGN_RANK_LN ? pRank : gpMenuItems_GetName[i].pTxt), 0);
	}
	// Nom en cours.
	i = Font_Print(0, 0, gMenu.pScName, FONT_NoDisp);
//	Font_Print((SCR_Width - i) / 2, 80+64+16, gMenu.pScName, 0);
	Font_Print((SCR_Width - i) / 2, HIGN_OFFS1STLN +64+16, gMenu.pScName, 0);
	// Curseur au bout du nom.
	if (gnFrame & 8)
	{
//		Font_Print(((SCR_Width - i) / 2) + i, 80+64+16, "_", 0);
		Font_Print(((SCR_Width - i) / 2) + i, HIGN_OFFS1STLN +64+16, "_", 0);
	}

	// Sprite de Marco Win.
	i = 0;
	if (gMenu.nAnm0 != -1) i += AnmGetImage(gMenu.nAnm0);
	i += e_Spr_Hero_Victory_Machinegun;
	SprDisplayAbsolute(i, SCR_Width / 2, SCR_Height - 20, e_Prio_Joueur);


	gVar.nFadeVal = gMenu.nFadeVal;
	return (nRet);

}

#endif
//=============================================================================

#define	CFG_FILENAME	"mslug.cfg"
struct SMSCfg	gMSCfg;

// Calcul d'un petit checksum.
u32	CfgChecksumCalc(void)
{
	u32	i;
	u32	nSum = 0;

	for (i = 0; i < e_CfgKey_MAX; i++) nSum += gMSCfg.pKeys[i];
	nSum += gMSCfg.nVideoMode;

	return (nSum);
}

/*
// Cmp du qsort.
int CfgCmp(const void *a, const void *b)
{
  return (gMSCfg.pKeys[*(u16 *)a] - gMSCfg.pKeys[*(u16 *)b]);
}

// Teste si on a pas deux touches identiques.
u32 CfgCheck(void)
{
	u16	pKeysIdx[e_CfgKey_MAX];
	u32	i;

	// Tri de valeurs des touches.
	for (i = 0; i < e_CfgKey_MAX; i++) pKeysIdx[i] = i;
	qsort(pKeysIdx, e_CfgKey_MAX, sizeof(u16), CfgCmp);

	// Si il y a plus de 1 fois une touche, erreur.
	for (i = 0; i < e_CfgKey_MAX-1; i++)
		if (gMSCfg.pKeys[pKeysIdx[i + 1]] == gMSCfg.pKeys[pKeysIdx[i]]) return (1);

	return (0);
}
*/

// Lecture de la config.
u32 CfgLoad(void)
{
	FILE	*fPt;
	u32	nSz;

	static struct SMSCfg	sCfgDefault = { { SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_v, SDLK_c, SDLK_x, 0, 1, 2 }, 0, 0 };	// Cfg par défaut.

	fPt = fopen(CFG_FILENAME, "rb");
	if (fPt == NULL)
	{
		fprintf(stderr, "CfgLoad(): fopen error.\n");
		goto _CfgLoad_def;
	}
	nSz = fread(&gMSCfg, 1, sizeof(struct SMSCfg), fPt);
	fclose(fPt);
	if (nSz != sizeof(struct SMSCfg))
	{
		fprintf(stderr, "CfgLoad(): fread error.\n");
		goto _CfgLoad_def;
	}
	// Test du checksum.
	nSz = CfgChecksumCalc();
	if (gMSCfg.nChecksum != (u16)nSz)
	{
		fprintf(stderr, "CfgLoad(): Checksum error.\n");
		goto _CfgLoad_def;
	}
	return (0);
_CfgLoad_def:
	fprintf(stderr, "CfgLoad(): Default configuration used.\n");
	memcpy(&gMSCfg, &sCfgDefault, sizeof(struct SMSCfg));
	return (1);
}


