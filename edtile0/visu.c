
#include "includes.h"


#define	VISU_SCR_B16	(1)		// Force les visu dans des multiples de 16.

#ifdef VISU_SCR_B16
	#define	VISU_FRAME_SZ	(16)	// Largeur du cadre.
#else
	#define	VISU_FRAME_SZ	(32)
#endif

#define	VISU_SZ_W_MINI	(10 * 16)
#define	VISU_SZ_H_MINI	(8 * 16)


// Prototypes des fonctions privées.
void Visu_InitPlanesValues(struct SVisu *psVisu);
void Visu_ScrollDifferentiel(struct SVisu *psVisu);


// Constructeur.
struct SVisu * Visu_New(u32 nVisuLg, u32 nVisuHt)
{
	struct SVisu *psVisu;

	if ((psVisu = (struct SVisu *)malloc(sizeof(struct SVisu))) == NULL)
	{
		fprintf(stderr, "Visu_New(): malloc failed (psVisu).\n");
		exit(1);
	}

	struct SMap	*psMap = gMisc.psMap;
	u32	i;

	psVisu->nInit = 0;		// 0: Pas initialisé / 1: Initialisé.
#ifdef VISU_SCR_B16
	psVisu->nDisplayWidth = nVisuLg & ~0xF;
	psVisu->nDisplayHeight = nVisuHt & ~0xF;
#else
	psVisu->nDisplayWidth = nVisuLg;
	psVisu->nDisplayHeight = nVisuHt;
#endif
	// Pas plus grand que l'écran...
	if (psVisu->nDisplayWidth > (gMisc.nScrW & ~0xF) - (2 * VISU_FRAME_SZ)) psVisu->nDisplayWidth = (gMisc.nScrW & ~0xF) - (2 * VISU_FRAME_SZ);
	if (psVisu->nDisplayHeight > (gMisc.nScrH & ~0xF) - (2 * VISU_FRAME_SZ)) psVisu->nDisplayHeight = (gMisc.nScrH & ~0xF) - (2 * VISU_FRAME_SZ);
	// ... et pas trop petit.
	if (psVisu->nDisplayWidth < VISU_SZ_W_MINI) psVisu->nDisplayWidth = VISU_SZ_W_MINI;
	if (psVisu->nDisplayHeight < VISU_SZ_H_MINI) psVisu->nDisplayHeight = VISU_SZ_H_MINI;

//-	_pScroll = new SScrollVar[gMap._nPlanesNb];
//-	if (_pScroll == NULL) return;
	if ((psVisu->pScroll = (struct SScrollVar *)malloc(sizeof(struct SScrollVar) * psMap->nPlanesNb)) == NULL)
	{
		fprintf(stderr, "Visu_New(): malloc failed (pScroll).\n");
		exit(1);
	}

	for (i = 0; i < psMap->nPlanesNb; i++)
	{
		(*(psVisu->pScroll + i)).nValidPlane = 0;
	}
	psVisu->nRefPlane = 0;
	psVisu->nPosX = 0;			// Position générale de la visualisation.
	psVisu->nPosY = 0;

	psVisu->nWndPosX = -1;		// Négatif, InitPlanes() fera l'init.

	return (psVisu);
}

// Destructeur.
void Visu_Delete(struct SVisu *psVisu)
{
	if (psVisu->pScroll != NULL)
	{
		free(psVisu->pScroll);
		psVisu->pScroll = NULL;
	}

	free(psVisu);
}

// Toggle.
void Visu_VisuToggle(struct SVisu *psVisu, u32 nOnOff)
{
	if (nOnOff)
	{
		// On.
		Visu_InitPlanesValues(psVisu);
		Visu_WindowLocation(psVisu, psVisu->nWndPosX, psVisu->nWndPosY);	// Après InitPlanesValues(), qui initialise _nWndPosX et Y au premier passage.
	}
	else
	{
		// Off.
		psVisu->nInit = 0;
	}
}

// Initialisation des valeurs des plans pour les scrolls.
void Visu_InitPlanesValues(struct SVisu *psVisu)
{
	struct SMap	*psMap = gMisc.psMap;
	u32	nPlane;
	s32	*pBlocks;
	u32	i;

	// Affichage des plans.
	for (nPlane = 0; nPlane < psMap->nPlanesNb; nPlane++)
	{
		if (Map_PlaneHasCmd(psMap, nPlane))
		{
			// C'est un plan de monstres. On prend les lg et ht du plan précédent + il devient le plan de ref.
			if (nPlane && psVisu->pScroll[nPlane - 1].nValidPlane)
			{
				psVisu->pScroll[nPlane].nValidPlane = 1;
				psVisu->pScroll[nPlane].nPlaneLg = psVisu->pScroll[nPlane - 1].nPlaneLg;
				psVisu->pScroll[nPlane].nPlaneHt = psVisu->pScroll[nPlane - 1].nPlaneHt;
				psVisu->nRefPlane = nPlane;
//printf("mst plane #%d valid : lg=%d / ht=%d\n", nPlane, psVisu->pScroll[nPlane].nPlaneLg, psVisu->pScroll[nPlane].nPlaneHt);
			}
		}
		else
		{
			// C'est un plan normal, on cherche les lg et ht.

			// On récupère un pointeur sur les blocs.
			pBlocks = (*(psMap->ppsPlanes + nPlane))->pPlane;
			// Lg.
			for (i = 0; i < psMap->nMapLg; i++) if (*(pBlocks + i) == -1) break;
			if (i == 0) continue;
			psVisu->pScroll[nPlane].nPlaneLg = i;
			// Ht.
			for (i = 0; i < psMap->nMapHt; i++) if (*(pBlocks + (i * psMap->nMapLg)) == -1) break;
			if (i == 0) continue;
			psVisu->pScroll[nPlane].nPlaneHt = i;
			// Ok.
			psVisu->pScroll[nPlane].nValidPlane = 1;
//printf("plane #%d valid : lg=%d / ht=%d\n", nPlane, _pScroll[nPlane].nPlaneLg, _pScroll[nPlane].nPlaneHt);
		}
	}
//printf("ref plane = %d\n", _nRefPlane);

	// Position de la fenêtre d'affichage. (-1 => Pas encore fait).
	if (psVisu->nWndPosX == -1)
	{
		// Init de base, au milieu de l'écran.
		psVisu->nWndPosX = (gMisc.nScrW - psVisu->nDisplayWidth) / 2;
		psVisu->nWndPosY = (gMisc.nScrH - psVisu->nDisplayHeight) / 2;
	}

	// MAJ rectangle de la visu.
	Mouse_RectVisu(gMisc.psMouse, psVisu->nWndPosX - VISU_FRAME_SZ, psVisu->nWndPosY - VISU_FRAME_SZ,
								psVisu->nWndPosX + psVisu->nDisplayWidth + VISU_FRAME_SZ, psVisu->nWndPosY + psVisu->nDisplayHeight + VISU_FRAME_SZ);

	psVisu->nInit = 1;		// 0: Pas initialisé / 1: Initialisé.

}

// Limite le déplacement.
void Visu_ScrollPosition(struct SVisu *psVisu, u8 *pKeys)
{
	s32	nInc;

	if (psVisu->nInit == 0) return;

	// ALT : Déplacement en bout de map.
	if (pKeys[SDLK_LALT] || pKeys[SDLK_RALT])
	{
		if (pKeys[SDLK_UP]) psVisu->nPosY = 0;
		if (pKeys[SDLK_DOWN]) psVisu->nPosY = ((psVisu->pScroll[psVisu->nRefPlane].nPlaneHt * 16) - (s32)psVisu->nDisplayHeight) << 8;
		if (pKeys[SDLK_LEFT]) psVisu->nPosX = 0;
		if (pKeys[SDLK_RIGHT]) psVisu->nPosX = ((psVisu->pScroll[psVisu->nRefPlane].nPlaneLg * 16) - (s32)psVisu->nDisplayWidth) << 8;
		return;
	}

	// Flèches de direction (Shift = speed up, ctrl = spd *= 4).
	nInc = ((pKeys[SDLK_LSHIFT] || pKeys[SDLK_RSHIFT]) ? 0x800 : 0x100);
	if (pKeys[SDLK_LCTRL] || pKeys[SDLK_RCTRL]) nInc *= 4;
	if (pKeys[SDLK_UP]) psVisu->nPosY -= nInc;
	if (pKeys[SDLK_DOWN]) psVisu->nPosY += nInc;
	if (pKeys[SDLK_LEFT]) psVisu->nPosX -= nInc;
	if (pKeys[SDLK_RIGHT]) psVisu->nPosX += nInc;

}

// Calcule la position de chaque plan par rapport au plan de ref.
void Visu_ScrollDifferentiel(struct SVisu *psVisu)
{
	struct SMap	*psMap = gMisc.psMap;
	u32	i;

	if (psVisu->nInit == 0) return;

	// Limites de la map.
#ifdef VISU_SCR_B16
	if ((psVisu->nPosX >> 12) + ((s32)psVisu->nDisplayWidth / 16) >= psVisu->pScroll[psVisu->nRefPlane].nPlaneLg)
		psVisu->nPosX = ((psVisu->pScroll[psVisu->nRefPlane].nPlaneLg - (psVisu->nDisplayWidth / 16)) << 12);// - 0x100;
	if ((psVisu->nPosY >> 12) + ((s32)psVisu->nDisplayHeight / 16) >= psVisu->pScroll[psVisu->nRefPlane].nPlaneHt)
		psVisu->nPosY = ((psVisu->pScroll[psVisu->nRefPlane].nPlaneHt - (psVisu->nDisplayHeight / 16)) << 12);// - 0x100;
#else
	if ((psVisu->nPosX >> 8) + (s32)psVisu->nDisplayWidth >= psVisu->pScroll[psVisu->nRefPlane].nPlaneLg * 16)
		psVisu->nPosX = (((psVisu->pScroll[psVisu->nRefPlane].nPlaneLg * 16) - psVisu->nDisplayWidth) << 8);// - 0x100;
	if ((psVisu->nPosY >> 8) + (s32)psVisu->nDisplayHeight >= psVisu->pScroll[psVisu->nRefPlane].nPlaneHt * 16)
		psVisu->nPosY = (((psVisu->pScroll[psVisu->nRefPlane].nPlaneHt * 16) - psVisu->nDisplayHeight) << 8);// - 0x100;
#endif
	if (psVisu->nPosX < 0) psVisu->nPosX = 0;		// Après le test précédent, qui peut renvoyer du négatif (taille plan < taille visu).
	if (psVisu->nPosY < 0) psVisu->nPosY = 0;

	// Calcul du différentiel.
	for (i = 0; i < psMap->nPlanesNb; i++)
	{
		if (psVisu->pScroll[i].nValidPlane == 0) continue;

		if (i == psVisu->nRefPlane)
		{
			psVisu->pScroll[i].nPlaneNewPosX = psVisu->nPosX;	// Nouvelles positions de chaque plan.
			psVisu->pScroll[i].nPlaneNewPosY = psVisu->nPosY;
//printf("Diff plan #%d (ref) x=%d y=%d\n", (int)i, (int)_pScroll[i].nPlaneNewPosX >> 8, (int)_pScroll[i].nPlaneNewPosY >> 8);
		}
		else
		{
			s32	nDiffX, nDiffY;
#ifdef VISU_SCR_B16
			nDiffX = (psVisu->pScroll[psVisu->nRefPlane].nPlaneLg - (psVisu->nDisplayWidth / 16) == 0 ? 0 :
				(psVisu->nPosX * (psVisu->pScroll[i].nPlaneLg - (psVisu->nDisplayWidth / 16))) / (psVisu->pScroll[psVisu->nRefPlane].nPlaneLg - (psVisu->nDisplayWidth / 16)) );
			nDiffY = (psVisu->pScroll[psVisu->nRefPlane].nPlaneHt - (psVisu->nDisplayHeight / 16) == 0 ? 0 :
				(psVisu->nPosY * (psVisu->pScroll[i].nPlaneHt - (psVisu->nDisplayHeight / 16))) / (psVisu->pScroll[psVisu->nRefPlane].nPlaneHt - (psVisu->nDisplayHeight / 16)) );
#else
			nDiffX = ((psVisu->pScroll[psVisu->nRefPlane].nPlaneLg * 16) - psVisu->nDisplayWidth == 0 ? 0 :
				(psVisu->nPosX * ((psVisu->pScroll[i].nPlaneLg * 16) - psVisu->nDisplayWidth)) / ((psVisu->pScroll[psVisu->nRefPlane].nPlaneLg * 16) - psVisu->nDisplayWidth) );
			nDiffY = ((psVisu->pScroll[psVisu->nRefPlane].nPlaneHt * 16) - psVisu->nDisplayHeight == 0 ? 0 :
				(psVisu->nPosY * ((psVisu->pScroll[i].nPlaneHt * 16) - psVisu->nDisplayHeight)) / ((psVisu->pScroll[psVisu->nRefPlane].nPlaneHt * 16) - psVisu->nDisplayHeight) );
#endif
			psVisu->pScroll[i].nPlaneNewPosX = nDiffX;	// Nouvelles positions de chaque plan.
			psVisu->pScroll[i].nPlaneNewPosY = nDiffY;
//printf("Diff plan #%d --- x=%d y=%d\n", (int)i, (int)psVisu->pScroll[i].nPlaneNewPosX >> 8, (int)psVisu->pScroll[i].nPlaneNewPosY >> 8);
		}
	}

}

// Dessine la visu.
void Visu_DrawVisu(struct SVisu *psVisu)
{
	struct SMap	*psMap = gMisc.psMap;
	u32	nPlane;
	s32	*pBlocks;
	s32	nBlock;
	u32	i, j;

	SDL_Rect	sRect;

	if (psVisu->nInit == 0) return;

	// Calcul du différentiel.
	Visu_ScrollDifferentiel(psVisu);

	// Clear screen.
	sRect.x = psVisu->nWndPosX;
	sRect.y = psVisu->nWndPosY;
	sRect.w = psVisu->nDisplayWidth;
	sRect.h = psVisu->nDisplayHeight;
	SDL_FillRect(gMisc.pScreen, &sRect, 0);

	// Affichage des plans.
	SDL_LockSurface(gMisc.pScreen);
	// On boucle sur tous les plans.
	for (nPlane = 0; nPlane < psMap->nPlanesNb; nPlane++)
	{
		if (psVisu->pScroll[nPlane].nValidPlane == 0) continue;
		if (Plane_IsHidden(*(psMap->ppsPlanes + nPlane))) continue;

		// On récupère un pointeur sur les blocs.
		pBlocks = (*(psMap->ppsPlanes + nPlane))->pPlane;

#ifdef VISU_SCR_B16
		for (j = 0; j < (psVisu->nDisplayHeight / 16) + 1 && j + (psVisu->pScroll[nPlane].nPlaneNewPosY >> 12) < (u32)psVisu->pScroll[nPlane].nPlaneHt; j++)
#else
		for (j = 0; j < (psVisu->nDisplayHeight / 16) + ((psVisu->nDisplayHeight & 0xF) ? 2 : 1) && j + (psVisu->pScroll[nPlane].nPlaneNewPosY >> 12) < (u32)psVisu->pScroll[nPlane].nPlaneHt; j++)
#endif
		{
#ifdef VISU_SCR_B16
			for (i = 0; i < (psVisu->nDisplayWidth / 16) + 1 && i + (psVisu->pScroll[nPlane].nPlaneNewPosX >> 12) < (u32)psVisu->pScroll[nPlane].nPlaneLg; i++)
#else
			for (i = 0; i < (psVisu->nDisplayWidth / 16) + ((psVisu->nDisplayWidth & 0xF) ? 2 : 1) && i + (psVisu->pScroll[nPlane].nPlaneNewPosX >> 12) < (u32)psVisu->pScroll[nPlane].nPlaneLg; i++)
#endif
			{
				nBlock = *(pBlocks + ((j + (psVisu->pScroll[nPlane].nPlaneNewPosY >> 12)) * psMap->nMapLg) + i + (psVisu->pScroll[nPlane].nPlaneNewPosX >> 12));
				if (nBlock != -1)
				{
					Plane_BlockDrawPix(*(psMap->ppsPlanes + nPlane), nBlock,
						(i * 16) + psVisu->nWndPosX - ((psVisu->pScroll[nPlane].nPlaneNewPosX >> 8) & 0x0F),
						(j * 16) + psVisu->nWndPosY - ((psVisu->pScroll[nPlane].nPlaneNewPosY >> 8) & 0x0F), false);
				}
			}
		}

	}
	SDL_UnlockSurface(gMisc.pScreen);

	// On dessine un cadre qui cachera les bords.
	u32	nClr = SDL_MapRGB(gMisc.pScreen->format, 66, 65, 132);
	// Haut.
	sRect.x = psVisu->nWndPosX;
	sRect.y = psVisu->nWndPosY - VISU_FRAME_SZ;
	sRect.w = psVisu->nDisplayWidth;
	sRect.h = VISU_FRAME_SZ;
	SDL_FillRect(gMisc.pScreen, &sRect, nClr);
	// Bas.
	sRect.x = psVisu->nWndPosX;
	sRect.y = psVisu->nWndPosY + psVisu->nDisplayHeight;
	sRect.w = psVisu->nDisplayWidth;
	sRect.h = VISU_FRAME_SZ;
	SDL_FillRect(gMisc.pScreen, &sRect, nClr);
	// Gauche.
	sRect.x = psVisu->nWndPosX - VISU_FRAME_SZ;
	sRect.y = psVisu->nWndPosY - VISU_FRAME_SZ;
	sRect.w = VISU_FRAME_SZ;
	sRect.h = psVisu->nDisplayHeight + (VISU_FRAME_SZ * 2);
	SDL_FillRect(gMisc.pScreen, &sRect, nClr);
	// Droite.
	sRect.x = psVisu->nWndPosX + psVisu->nDisplayWidth;
	sRect.y = psVisu->nWndPosY - VISU_FRAME_SZ;
	sRect.w = VISU_FRAME_SZ;
	sRect.h = psVisu->nDisplayHeight + (VISU_FRAME_SZ * 2);
	SDL_FillRect(gMisc.pScreen, &sRect, nClr);

	// Titre de la fenêtre.
//	Font_Printf(psVisu->nWndPosX, psVisu->nWndPosY - 16 + 4, "MAP VISUAL", 0xFFFF);
	Font_StrReset(&gFont);
	Font_StrAdd(&gFont, "MAP VISUAL (");
	Font_StrItoa(&gFont, psVisu->nDisplayWidth);
	Font_StrAdd(&gFont, ",");
	Font_StrItoa(&gFont, psVisu->nDisplayHeight);
	Font_StrAdd(&gFont, ")");
	Font_Printf(psVisu->nWndPosX, psVisu->nWndPosY - 16 + 4, gFont.pString, 0xFFFF);

}

// Déplacement de la fenêtre de visualisation.
void Visu_WindowLocation(struct SVisu *psVisu, s32 nNewPosX, s32 nNewPosY)
{
	// Contrôle de sortie d'écran.
	if (nNewPosX < VISU_FRAME_SZ) nNewPosX = VISU_FRAME_SZ;
	if (nNewPosX + psVisu->nDisplayWidth + VISU_FRAME_SZ >= gMisc.nScrW) nNewPosX = gMisc.nScrW - psVisu->nDisplayWidth - VISU_FRAME_SZ;
	if (nNewPosY < VISU_FRAME_SZ) nNewPosY = VISU_FRAME_SZ;
	if (nNewPosY + psVisu->nDisplayHeight + VISU_FRAME_SZ >= (gMisc.nScrH & ~0xF)) nNewPosY = (gMisc.nScrH & ~0xF) - psVisu->nDisplayHeight - VISU_FRAME_SZ;

	// Nouvelle position.
	psVisu->nWndPosX = nNewPosX;
	psVisu->nWndPosY = nNewPosY;

	// MAJ rectangle de la visu.
	Mouse_RectVisu(gMisc.psMouse, psVisu->nWndPosX - VISU_FRAME_SZ, psVisu->nWndPosY - VISU_FRAME_SZ,
								psVisu->nWndPosX + psVisu->nDisplayWidth + VISU_FRAME_SZ, psVisu->nWndPosY + psVisu->nDisplayHeight + VISU_FRAME_SZ);
	Mouse_LocationGet(gMisc.psMouse);

}

