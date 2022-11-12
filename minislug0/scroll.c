// Scroll multidirectionnel.
// Méthode de la mémoire "à rouleaux".
//
// Code: 17o2!! / Contact: Clement CORDE, c1702@yahoo.com

#include "includes.h"


struct SScrollPos	gScrollPos;

struct SScrollMulti
{
	struct SDL_Surface	*ppPlanesScrollBuf[MAP_PLANES_MAX];	// Buffers de scroll des plans.
	s32	pPlanePosX[MAP_PLANES_MAX];	// Positions de chaque plan.
	s32	pPlanePosY[MAP_PLANES_MAX];
	s32	pPlaneNewPosX[MAP_PLANES_MAX];	// Nouvelles positions de chaque plan, utilisées lors du calcul du différentiel.
	s32	pPlaneNewPosY[MAP_PLANES_MAX];

};
struct SScrollMulti	gScrollM;


#define	SCROLLBUF_LG	512		// Taille du buffer à rouleaux (en pixels).
#define	SCROLLBUF_HT	256

// Alloue les buffers de scroll. 1 seule fois !
void ScrollAllocate(void)
{
	u32	i;

	for (i = 0; i < MAP_PLANES_MAX; i++)
	{
		//gScrollM.ppPlanesScrollBuf[i] = SDL_CreateRGBSurface(SDL_HWSURFACE, SCROLLBUF_LG, SCROLLBUF_HT, 16, 0, 0, 0, 0);
//		gScrollM.ppPlanesScrollBuf[i] = SDL_CreateRGBSurface(SDL_SWSURFACE, SCROLLBUF_LG, SCROLLBUF_HT, 16, 0, 0, 0, 0);
		gScrollM.ppPlanesScrollBuf[i] = SDL_CreateRGBSurface(SDL_SWSURFACE, SCROLLBUF_LG, SCROLLBUF_HT, 16, gVar.pScreen->format->Rmask, gVar.pScreen->format->Gmask, gVar.pScreen->format->Bmask, 0);
		if (gScrollM.ppPlanesScrollBuf[i] == NULL)
		{
			fprintf(stderr, "ScrollAllocate: Unable to allocate scroll buffers: %s\n", SDL_GetError());
			exit(1);
		}
	}

}

// Libère les buffers de scroll. 1 seule fois !
void ScrollRelease(void)
{
	u32	i;

	for (i = 0; i < MAP_PLANES_MAX; i++)
	{
		SDL_FreeSurface(gScrollM.ppPlanesScrollBuf[i]);
	}
}

// Copie d'une colonne.
void Scr_sub_NewCol(u32 nPlane, s32 sBlMapX, s32 sBlMapY)
{
	u32	j, k;
	s32	nBlockNo;
	u32	nBlX, nBlY;
	u16	*pSrc, *pDst;

	// Cas extrème, complètement à droite. Il y a un appel sur la 1ere colonne derrière la map lors du scroll vers la droite.
//b	if ((u32)sBlMapX >= gMap.nMapLg) return;
	if ((u32)sBlMapX >= gMap.pPlanesLg[nPlane]) return;

	// Trace la colonne.
	SDL_LockSurface(gScrollM.ppPlanesScrollBuf[nPlane]);
	//for (j = 0; j < (SCR_Height / 16) + 1; j++)
//b	for (j = 0; j < (SCR_Height / 16) + 1 && sBlMapY + j < gMap.nMapHt; j++)
	for (j = 0; j < (SCR_Height / 16) + 1 && sBlMapY + j < gMap.pPlanesHt[nPlane]; j++)
	{
		nBlockNo = *(*(gMap.ppPlanesBlocks + nPlane) + ((sBlMapY + j) * gMap.nMapLg) + sBlMapX);
		// Coordonées x,y du bloc dans son plan.
		nBlY = nBlockNo / (gMap.ppPlanesGfx[nPlane]->w / 16);
		nBlX = nBlockNo - (nBlY * (gMap.ppPlanesGfx[nPlane]->w / 16));
		// Src et Dst.
		pSrc = (u16 *)gMap.ppPlanesGfx[nPlane]->pixels + (nBlY * 16 * gMap.ppPlanesGfx[nPlane]->w) + (nBlX * 16);
		pDst = (u16 *)gScrollM.ppPlanesScrollBuf[nPlane]->pixels +
			((((sBlMapY + j) % (SCROLLBUF_HT / 16)) * 16) * SCROLLBUF_LG) +
			((sBlMapX % (SCROLLBUF_LG / 16)) * 16);
		// Bloc.
		for (k = 0; k < 16; k++)
		{
			*(u32 *)pDst = *(u32 *)pSrc;
			*(u32 *)(pDst + 2) = *(u32 *)(pSrc + 2);
			*(u32 *)(pDst + 4) = *(u32 *)(pSrc + 4);
			*(u32 *)(pDst + 6) = *(u32 *)(pSrc + 6);
			*(u32 *)(pDst + 8) = *(u32 *)(pSrc + 8);
			*(u32 *)(pDst + 10) = *(u32 *)(pSrc + 10);
			*(u32 *)(pDst + 12) = *(u32 *)(pSrc + 12);
			*(u32 *)(pDst + 14) = *(u32 *)(pSrc + 14);
			pSrc += gMap.ppPlanesGfx[nPlane]->w;
			pDst += SCROLLBUF_LG;
		}

	}
	SDL_UnlockSurface(gScrollM.ppPlanesScrollBuf[nPlane]);

	AnmBlkScrollNewCol(nPlane, sBlMapX, sBlMapY);	// Update des blocs animés entrant sur la colonne.

}

// Copie d'une ligne.
void Scr_sub_NewLn(u32 nPlane, s32 sBlMapX, s32 sBlMapY)
{
	u32	i, k;
	s32	nBlockNo;
	u32	nBlX, nBlY;
	u16	*pSrc, *pDst;

	// Cas extrème, complètement en bas. Il y a un appel sur la 1ere ligne sous la map lors du scroll vers le bas.
//b	if ((u32)sBlMapY >= gMap.nMapHt) return;
	if ((u32)sBlMapY >= gMap.pPlanesHt[nPlane]) return;

	// Trace la ligne.
	SDL_LockSurface(gScrollM.ppPlanesScrollBuf[nPlane]);
	//for (i = 0; i < (SCR_Width / 16) + 1; i++)
//b	for (i = 0; i < (SCR_Width / 16) + 1 && sBlMapX + i < gMap.nMapLg; i++)
	for (i = 0; i < (SCR_Width / 16) + 1 && sBlMapX + i < gMap.pPlanesLg[nPlane]; i++)
	{
		nBlockNo = *(*(gMap.ppPlanesBlocks + nPlane) + (sBlMapY * gMap.nMapLg) + sBlMapX + i);
		// Coordonées x,y du bloc dans son plan.
		nBlY = nBlockNo / (gMap.ppPlanesGfx[nPlane]->w / 16);
		nBlX = nBlockNo - (nBlY * (gMap.ppPlanesGfx[nPlane]->w / 16));
		// Src et Dst.
		pSrc = (u16 *)gMap.ppPlanesGfx[nPlane]->pixels + (nBlY * 16 * gMap.ppPlanesGfx[nPlane]->w) + (nBlX * 16);
		pDst = (u16 *)gScrollM.ppPlanesScrollBuf[nPlane]->pixels +
			(((sBlMapY % (SCROLLBUF_HT / 16)) * 16) * SCROLLBUF_LG) +
			(((sBlMapX + i) % (SCROLLBUF_LG / 16)) * 16);
		// Bloc.
		for (k = 0; k < 16; k++)
		{
			*(u32 *)pDst = *(u32 *)pSrc;
			*(u32 *)(pDst + 2) = *(u32 *)(pSrc + 2);
			*(u32 *)(pDst + 4) = *(u32 *)(pSrc + 4);
			*(u32 *)(pDst + 6) = *(u32 *)(pSrc + 6);
			*(u32 *)(pDst + 8) = *(u32 *)(pSrc + 8);
			*(u32 *)(pDst + 10) = *(u32 *)(pSrc + 10);
			*(u32 *)(pDst + 12) = *(u32 *)(pSrc + 12);
			*(u32 *)(pDst + 14) = *(u32 *)(pSrc + 14);
			pSrc += gMap.ppPlanesGfx[nPlane]->w;
			pDst += SCROLLBUF_LG;
		}

	}
	SDL_UnlockSurface(gScrollM.ppPlanesScrollBuf[nPlane]);

	AnmBlkScrollNewLn(nPlane, sBlMapX, sBlMapY);	// Update des blocs animés entrant sur la ligne.

}

#define	SCROLL_SPDX		0x400
#define	SCROLL_SPDY		0x400
#define SCROLL_SPDY_MAX	SPDY_MAX	//0x800
	// = Gravité. (voir si on peut faire un #define	SCROLL_SPD_Y GRAVITY dans les .h)	// A mettre en commun dans le game.h

//>tst
s32	gnScrollLimitXMin;	// Avec 8 bits de virgule fixe.
s32	gnScrollLimitXMax;
//s32	gnScrollLastPosX;	// Pour empêcher le retour en arrière.

s32	gnScrollLimitYMin;	// Avec 8 bits de virgule fixe.
s32	gnScrollLimitYMax;

//<tst
//u32	gnScrollLockOrgX;
//extern u32	gnScrollLockOrgX;
//gnScrollLockOrgX = gScrollPos.nPosX;

//// En macro car ça ne sert que dans ScrollPosition et une seule fois dans InitScreen. => Appel de fonction pas rentable.
//#define	SCROLL_PLYR_POSX	( gShoot.nPlayerPosX - (((gShoot.nPlayerDir ? 3 : 1) * (SCR_Width / 16) / 4) << 12) )
//#define	SCROLL_PLYR_POSY	( gShoot.nPlayerPosY - ((2 * (SCR_Height / 16) / 3) << 12) )

#define	SCROLL_PLYR_POSX	( gShoot.nPlayerPosX - (((gShoot.nPlayerDir ? 2 : 1) * (SCR_Width / 16) / 3) << 12) )
#define	SCROLL_PLYR_POSY	( (gShoot.nPlayerGnd ? gShoot.nPlayerPosY : gShoot.nPlayerLastGndPosY) - ((2 * (SCR_Height / 16) / 3) << 12) )

/*
s32 Scroll_sub_PlayerPosX(void)
{
	switch (gnScrollType)
	{
	case e_ScrollType_RightOnly:
		return ( gShoot.nPlayerPosX - (((SCR_Width / 16) / 2) << 12) );
		break;
	}
	// Par défaut, "free", on excentre le perso à 1/4 d'écran.
	return ( gShoot.nPlayerPosX - (((gShoot.nPlayerDir ? 3 : 1) * (SCR_Width / 16) / 4) << 12) );
}
s32 Scroll_sub_PlayerPosY(void)
{
	return ( (gShoot.nPlayerGnd ? gShoot.nPlayerPosY : gShoot.nPlayerLastGndPosY) - ((2 * (SCR_Height / 16) / 3) << 12) );
}
*/

// Détermine la position du scroll par rapport au héros.
// Out : gScrollM.nPosX et gScrollM.nPosY sont aux nouvelles coordonées.
void ScrollPosition(void)
{
	s32	nPos;
	s32	nDiff, nSpd;

	// Position X.
	nPos = SCROLL_PLYR_POSX;
	nDiff = (nPos - gScrollPos.nPosX) / 2;
	if (ABS(nDiff) > SCROLL_SPDX) nDiff = SGN(nDiff) * SCROLL_SPDX;
	gScrollPos.nPosX += nDiff;

	// Position Y.
	nPos = SCROLL_PLYR_POSY;
	nSpd = SCROLL_SPDY;
	// Trop bas ? (ex: le joueur saute dans un trou, du coup la ligne du dessus ne change pas l'offset).
	if (gShoot.nPlayerPosY > nPos + ((3 * (SCR_Height / 16) / 4) << 12) )
	{
		nPos = gShoot.nPlayerPosY - ((3 * (SCR_Height / 16) / 4) << 12);
		nSpd = SCROLL_SPDY_MAX;		// A ce moment là, on permet de descendre plus vite (à la vitesse Y max du joueur).
	}
	nDiff = (nPos - gScrollPos.nPosY) / 2;
	if (ABS(nDiff) > nSpd) nDiff = SGN(nDiff) * nSpd;
	gScrollPos.nPosY += nDiff;

	// Empèche le retour arrière dans les scrolls e_ScrollType_RightOnly.
	if (gnScrollLimitXMin == -1)
	if (gScrollPos.nScrollType == e_ScrollType_RightOnly)
		if (gScrollPos.nPosX < gScrollPos.nLastPosX - (((SCR_Width / 16) / 3) << 12) ) gScrollPos.nPosX = gScrollPos.nLastPosX - (((SCR_Width / 16) / 3) << 12);

	// Limites du scroll X ?
	if (gnScrollLimitXMax != -1)
		if (gScrollPos.nPosX + (SCR_Width << 8) >= gnScrollLimitXMax) gScrollPos.nPosX = gnScrollLimitXMax - (SCR_Width << 8);
	if (gnScrollLimitXMin != -1)
		if (gScrollPos.nPosX < gnScrollLimitXMin) gScrollPos.nPosX = gnScrollLimitXMin;
	// Limites du scroll Y ?
	if (gnScrollLimitYMax != -1)
		if (gScrollPos.nPosY + (SCR_Height << 8) >= gnScrollLimitYMax) gScrollPos.nPosY = gnScrollLimitYMax - (SCR_Height << 8);
	if (gnScrollLimitYMin != -1)
		if (gScrollPos.nPosY < gnScrollLimitYMin) gScrollPos.nPosY = gnScrollLimitYMin;

	// Limites de la map.
	if ((gScrollPos.nPosX >> 12) + (SCR_Width / 16) >= gMap.pPlanesLg[gMap.nHeroPlane]) gScrollPos.nPosX = ((gMap.pPlanesLg[gMap.nHeroPlane] - (SCR_Width / 16)) << 12);// - 0x100;
	if (gScrollPos.nPosX < 0) gScrollPos.nPosX = 0;
	if ((gScrollPos.nPosY >> 12) + (SCR_Height / 16) >= gMap.pPlanesHt[gMap.nHeroPlane]) gScrollPos.nPosY = ((gMap.pPlanesHt[gMap.nHeroPlane] - (SCR_Height / 16)) << 12);// - 0x100;
	if (gScrollPos.nPosY < 0) gScrollPos.nPosY = 0;

	// Suppression du désagréable effet de scintillement sur le joueur !
	gScrollPos.nPosX &= ~0xFF;
	gScrollPos.nPosY &= ~0xFF;

	// MAJ LastPos si le scroll n'est pas bloqué.
	if (gnScrollLimitXMin == -1)
	{
		switch (gScrollPos.nScrollType)
		{
		case e_ScrollType_RightOnly:
			if (gScrollPos.nPosX > gScrollPos.nLastPosX) gScrollPos.nLastPosX = gScrollPos.nPosX;
			break;
		default:
			gScrollPos.nLastPosX = gScrollPos.nPosX;	// < Au cas ou on passe d'un type de scroll à un autre.
			break;
		}
	}

}

// Calcule la position de chaque plan par rapport au plan de ref.
void ScrollDifferentiel(void)
{
	u32	i;

	for (i = 0; i < gMap.nPlanesNb; i++)
	{
		if (i == gMap.nHeroPlane)
		{
			gScrollM.pPlaneNewPosX[i] = gScrollPos.nPosX;	// Nouvelles positions de chaque plan.
			gScrollM.pPlaneNewPosY[i] = gScrollPos.nPosY;
//printf("Diff plan #%d (ref) x=%d y=%d\n", (int)i, (int)gScrollM.pPlaneNewPosX[i] >> 8, (int)gScrollM.pPlaneNewPosY[i] >> 8);
		}
		else
		{
			s32	nDiffX, nDiffY;
//			nDiffX = (gScrollM.nPosX * (gMap.pPlanesLg[i] - (SCR_Width / 16))) / (gMap.pPlanesLg[gMap.nHeroPlane] - (SCR_Width / 16));
//			nDiffY = (gScrollM.nPosY * (gMap.pPlanesHt[i] - (SCR_Height / 16))) / (gMap.pPlanesHt[gMap.nHeroPlane] - (SCR_Height / 16));
			nDiffX = (gMap.pPlanesLg[i] - (SCR_Width / 16) == 0 ? 0 :
				(gScrollPos.nPosX * (gMap.pPlanesLg[i] - (SCR_Width / 16))) / (gMap.pPlanesLg[gMap.nHeroPlane] - (SCR_Width / 16)) );
			nDiffY = (gMap.pPlanesHt[i] - (SCR_Height / 16) == 0 ? 0 :
				(gScrollPos.nPosY * (gMap.pPlanesHt[i] - (SCR_Height / 16))) / (gMap.pPlanesHt[gMap.nHeroPlane] - (SCR_Height / 16)) );
			gScrollM.pPlaneNewPosX[i] = nDiffX;	// Nouvelles positions de chaque plan.
			gScrollM.pPlaneNewPosY[i] = nDiffY;
//if (i == 0) printf("Diff plan #%d --- x=%d y=%d\n", (int)i, (int)gScrollM.pPlaneNewPosX[i] >> 8, (int)gScrollM.pPlaneNewPosY[i] >> 8);
//printf("Diff plan #%d --- x=%d y=%d\n", (int)i, (int)gScrollM.pPlaneNewPosX[i] >> 8, (int)gScrollM.pPlaneNewPosY[i] >> 8);
		}
	}

}



// Defines spécifiques.
#define	SCROLL_L08_POSX_INIT	0
#define	SCROLL_L11_SPDY_INIT	-0x0040//-0x100//
#define	SCROLL_L02_SPDX_INIT	0x0040

// Initialise l'écran une fois avant le scroll.
void ScrollInitScreen(u32 nScrollType)
{
	s32	i;
	u32	nPlane;

	gScrollPos.nScrollType = nScrollType;		// Stockage du type du scroll.

	gScrollPos.nPosX = SCROLL_PLYR_POSX;	// Init position fenêtre.
	//gScrollPos.nPosX = Scroll_sub_PlayerPosX();	// Init position fenêtre.
gScrollPos.nLastPosX = gScrollPos.nPosX;
	gScrollPos.nPosY = SCROLL_PLYR_POSY;
	//gScrollPos.nPosY = Scroll_sub_PlayerPosY();
//	gScrollPos.nLockX = 0;		// 0 : Normal / 1 : Locké, ne bouge plus.

//tst
//gnScrollLimitXMin = 0;
//gnScrollLimitXMax = (gMap.pPlanesLg[gMap.nHeroPlane] << 12) - 0x100;
gnScrollLimitXMin = gnScrollLimitXMax = -1;
gnScrollLimitYMin = gnScrollLimitYMax = -1;
//tst

	ScrollPosition();	// Appel pour les limites de map.
	ScrollDifferentiel();
//printf("scroll init: posx=%d posy=%d\n", (int)gScrollPos.nPosX, (int)gScrollPos.nPosY);
//printf("scroll init: posx=%d posy=%d\n", (int)gScrollPos.nPosX>>12, (int)gScrollPos.nPosY>>12);

	// Initialise les blocs de l'écran sur tous les plans.
	for (nPlane = 0; nPlane < gMap.nPlanesNb; nPlane++)
	{
		gScrollM.pPlanePosX[nPlane] = gScrollM.pPlaneNewPosX[nPlane];	// A l'init, pour initialiser pPlanePosX et pPlanePosY.
		gScrollM.pPlanePosY[nPlane] = gScrollM.pPlaneNewPosY[nPlane];

		for (i = 0; i < (SCR_Width / 16) + 1; i++)
		{
			Scr_sub_NewCol(nPlane, (gScrollM.pPlanePosX[nPlane] >> 12) + i, gScrollM.pPlanePosY[nPlane] >> 12);
		}
		// Transparence.
		if (nPlane) SDL_SetColorKey(gScrollM.ppPlanesScrollBuf[nPlane], SDL_SRCCOLORKEY, gMap.nTranspColorKey);	// Enable transparence.
	}

	// Initialisation des monstres.
	// On boucle sur les colonnes, init recherche X en même temps.
	gLoadedMst.nMstRechIdxX = 0;
	nPlane = gMap.nHeroPlane;
	for (i = -MST_CLIP_VAL; i <= (SCR_Width / 16) + MST_CLIP_VAL; i++)
	{
		MstCheckNewCol((gScrollM.pPlanePosX[nPlane] >> 12) + i, (gScrollM.pPlanePosY[nPlane] >> 12) - MST_CLIP_VAL, 1);
	}
	// On initialise la recherche Y.
	gLoadedMst.nMstRechIdxY = 0;
	MstCheckNewLine((gScrollM.pPlanePosY[nPlane] >> 12) + (SCR_Height / 16) + MST_CLIP_VAL, (gScrollM.pPlanePosX[nPlane] >> 12) - MST_CLIP_VAL, 1);
//printf("> initscreen end\n");


	// Initialisation d'une variable spéciale pour certains niveaux. (Si téléportation dans un niveau, il faudra changer ça de place).
	static s32	gSpecialVarTb[LEVEL_MAX] =
	{
		0, 0,
		SCROLL_L02_SPDX_INIT,
		0, 0, 0, 0, 0,
		SCROLL_L08_POSX_INIT,
		0, 0,
		SCROLL_L11_SPDY_INIT,
		0, 0, 0, 0, 0, 0,
	};
	gScrollPos.nSpecialVar = gSpecialVarTb[gGameVar.nLevel];

}


// Pour init des monstres du L11.
void Scroll_HeroPlaneScrollPosXY_Get(s32 *pnPosX, s32 *pnPosY)
{
	*pnPosX = gScrollM.pPlaneNewPosX[gMap.nHeroPlane];
	*pnPosY = gScrollM.pPlaneNewPosY[gMap.nHeroPlane];
}

// Le gros patch pour le niveau de l'espace.
void ScrollPatchLev11(void)
{
	// Scroll du plan de background.
	gScrollM.pPlaneNewPosY[0] -= 0x100;
	// Loop ?
	if (gScrollM.pPlaneNewPosY[0] < 0)
	{
		gScrollM.pPlanePosY[0] += 16 << 12;
		gScrollM.pPlaneNewPosY[0] += 16 << 12;
	}

	// Scroll du plan du héros (sauf quand mort).
	if (gShoot.nDeathFlag == 0)
	{
		gScrollM.pPlaneNewPosY[1] += gScrollPos.nL11SpdY;
		if (gScrollM.pPlaneNewPosY[1] < 0) gScrollM.pPlaneNewPosY[1] = 0;
	}

}

// Le gros patch pour le niveau de l'air.
void ScrollPatchLev02(void)
{
	// Scroll des plans de background.
	gScrollM.pPlaneNewPosX[0] += 0x100;
	gScrollM.pPlaneNewPosX[1] += 0x140;//0x180;
	// Loop ?
	u32	i;
	for (i = 0; i < 2; i++)
	if (gScrollM.pPlaneNewPosX[i] + (SCR_Width << 8) >= gMap.pPlanesLg[i] << 12)
	{
		gScrollM.pPlanePosX[i] &= ~(-1 << 8);
		gScrollM.pPlanePosX[i] -= 1 << 8;	// Pour forcer la mise à jour de la première colonne à droite de l'écran lors du loop. Lev2, plan 1, le plan mesure 2 fois la largeur du buffer, si on ne force pas l'affichage lors du loop, une col n'est pas mise à jour.
		gScrollM.pPlaneNewPosX[i] &= ~(-1 << 8);
	}

	// Scroll du plan du héros (sauf quand mort).
	if (gShoot.nDeathFlag == 0)
	{
		gScrollM.pPlaneNewPosX[gMap.nHeroPlane] += gScrollPos.nL02SpdX;
		if (gScrollM.pPlaneNewPosX[gMap.nHeroPlane] + (SCR_Width << 8) >= gMap.pPlanesLg[gMap.nHeroPlane] << 12)
			gScrollM.pPlaneNewPosX[gMap.nHeroPlane] = (gMap.pPlanesLg[gMap.nHeroPlane] << 12) - (SCR_Width << 8);
	}

}

// Le gros patch pour le niveau du train...
void ScrollPatchLev08(void)
{
//		static s32 nPosX = 0;	// Position du background.

	s32	nOldPosX;
	u32	nHeroPlane_sav;
s32	nSPosX_sav, nSPosY_sav;		//*** Avec scroll modifié
	u32	i;

	// Le bruit des roues. (Placement pas génial, mais bref...).
	if ((gnFrame & 63) == 0 || (gnFrame & 63) == 24)
		Sfx_PlaySfx(e_Sfx_Fx_TrainClang, e_SfxPrio_0);

	nHeroPlane_sav = gMap.nHeroPlane;
	ScrollPosition();	// On récupère la position de la fenêtre par rapport au joueur. => Position dans le plan du train.
nSPosX_sav = gScrollPos.nPosX;	//*** Avec scroll modifié
nSPosY_sav = gScrollPos.nPosY;	//*** Avec scroll modifié
//		nOldPosX = nPosX;
//		nPosX += gScrollPos.nPosX - gScrollM.pPlanePosX[gMap.nHeroPlane];	// On ajoute au background le même décalage que le plan du train.
//nPosX += 0x200;//0x080;//0x200;		// + la vitesse du train.
//if (nPosX < 0) nPosX = 0;
	nOldPosX = gScrollPos.nL08PosX;
	gScrollPos.nL08PosX += gScrollPos.nPosX - gScrollM.pPlanePosX[gMap.nHeroPlane];	// On ajoute au background le même décalage que le plan du train.
	gScrollPos.nL08PosX += 0x200;//0x080;//0x200;		// + la vitesse du train.
if (gScrollPos.nL08PosX < 0) gScrollPos.nL08PosX = 0;

	// Loop ?
//		if ((nPosX >> 12) == 208)
	if ((gScrollPos.nL08PosX >> 12) == 208)
	{
		// On calcule les différentes positions des plans à la position de loop (pour éviter de scroller en arrière jusqu'à la position de loop).
		nOldPosX -= (208 - 48) << 12;
		gScrollPos.nPosX = nOldPosX;
		gMap.nHeroPlane = gMap.nHeroPlane - 1;
		ScrollDifferentiel();
		gMap.nHeroPlane = nHeroPlane_sav;
		for (i = 0; i < gMap.nHeroPlane; i++)
		{
			gScrollM.pPlanePosX[i] = gScrollM.pPlaneNewPosX[i];
			gScrollM.pPlanePosY[i] = gScrollM.pPlaneNewPosY[i];
		}

		// Loop.
//			nPosX -= (208 - 48) << 12;
		gScrollPos.nL08PosX -= (208 - 48) << 12;
	}

//		gScrollPos.nPosX = nPosX;
	gScrollPos.nPosX = gScrollPos.nL08PosX;
	gMap.nHeroPlane = gMap.nHeroPlane - 1;
	ScrollDifferentiel();	// On calcule le différentiel en prenant comme référentiel le plan de background. (On se fout de la position qui va être affectée au plan du train, on va la réécraser plus tard).
	gMap.nHeroPlane = nHeroPlane_sav;

gScrollPos.nPosX = nSPosX_sav;	//*** Avec scroll modifié
gScrollPos.nPosY = nSPosY_sav;	//*** Avec scroll modifié
//		ScrollPosition();	// Re-scrollposition pour remettre gScrollPos.nPosX et Y par rapport au héros (important pour l'affichage des sprites).
	gScrollM.pPlaneNewPosX[gMap.nHeroPlane] = gScrollPos.nPosX;		// Et on force la position du plan du train.
	gScrollM.pPlaneNewPosY[gMap.nHeroPlane] = gScrollPos.nPosY;

}

typedef void (*pFctScrollPatch) (void);

// Gestion du scroll.
void ScrollManage(void)
{
//	u32	nOldOffs, nNewOffs;
	s32	nOldOffs, nNewOffs;		// Passé en s32 à cause de patch du lev2.
	u32	nPlane;
	u32	i;

	static	pFctScrollPatch	gpFctPatch[LEVEL_MAX] =
	{
		NULL, NULL,
		ScrollPatchLev02,
		NULL, NULL, NULL, NULL, NULL,
		ScrollPatchLev08,
		NULL, NULL,
		ScrollPatchLev11,
		NULL, NULL, NULL, NULL, NULL, NULL,
	};

	// Gestion de la position du scroll.
	if (gpFctPatch[gGameVar.nLevel] != NULL)
	{
		// Gestion particulière du scroll.
		gpFctPatch[gGameVar.nLevel]();
	}
	else
	{
		// Cas normal.
		ScrollPosition();
		ScrollDifferentiel();
	}

	// Scroll.
	for (i = 0; i < gMap.nPlanesNb; i++)
	{
		nPlane = i;		// quand finalisé, remplacer l'index de boucle i par nPlane.

		// Scroll horizontal ?
		nOldOffs = gScrollM.pPlanePosX[i] >> 12;
		nNewOffs = gScrollM.pPlaneNewPosX[i] >> 12;
		if (nOldOffs != nNewOffs)
		{
			if (nOldOffs < nNewOffs)
			{
				// Scroll vers la droite.
				while (nOldOffs < nNewOffs)
				{
					nOldOffs++;
					Scr_sub_NewCol(nPlane, nOldOffs + (SCR_Width / 16), gScrollM.pPlanePosY[i] >> 12);
					// Monstres. Test colonne de droite.
					if (nPlane == gMap.nHeroPlane)
						MstCheckNewCol(nOldOffs + (SCR_Width / 16) + MST_CLIP_VAL, (gScrollM.pPlanePosY[i] >> 12) - MST_CLIP_VAL, 1);
				}
			}
			else
			{
				// Scroll vers la gauche.
				while (nOldOffs > nNewOffs)
				{
					nOldOffs--;
					Scr_sub_NewCol(nPlane, nOldOffs, gScrollM.pPlanePosY[i] >> 12);
					// Monstres. Test colonne de gauche.
					if (nPlane == gMap.nHeroPlane)
						MstCheckNewCol(nOldOffs - MST_CLIP_VAL, (gScrollM.pPlanePosY[i] >> 12) - MST_CLIP_VAL, -1);
				}
			}
		}
		// New scroll pos.
		gScrollM.pPlanePosX[i] = gScrollM.pPlaneNewPosX[i];

		// Scroll vertical ?
		nOldOffs = gScrollM.pPlanePosY[i] >> 12;
		nNewOffs = gScrollM.pPlaneNewPosY[i] >> 12;
		if (nOldOffs != nNewOffs)
		{
			if (nOldOffs < nNewOffs)
			{
				// Scroll vers le bas.
				while (nOldOffs < nNewOffs)
				{
					nOldOffs++;
					Scr_sub_NewLn(nPlane, gScrollM.pPlanePosX[i] >> 12, nOldOffs + (SCR_Height / 16));
					// Monstres. Test ligne du bas.
					if (nPlane == gMap.nHeroPlane)
						MstCheckNewLine(nOldOffs + (SCR_Height / 16) + MST_CLIP_VAL, (gScrollM.pPlanePosX[i] >> 12) - MST_CLIP_VAL, 1);
				}
			}
			else
			{
				// Scroll vers le haut.
				while (nOldOffs > nNewOffs)
				{
					nOldOffs--;
					Scr_sub_NewLn(nPlane, gScrollM.pPlanePosX[i] >> 12, nOldOffs);
					// Monstres. Test ligne du haut.
					if (nPlane == gMap.nHeroPlane)
						MstCheckNewLine(nOldOffs - MST_CLIP_VAL, (gScrollM.pPlanePosX[i] >> 12) - MST_CLIP_VAL, -1);
				}
			}
		}
		// New scroll pos.
		gScrollM.pPlanePosY[i] = gScrollM.pPlaneNewPosY[i];

	}

}

extern	u8	gnFrameMissed;

// Blitte le plan x à l'écran.
void ScrollDisplayPlane(u32 nPlaneNo)
{
	SDL_Rect	sRctSrc;
	SDL_Rect	sRctDst;
	s32	nX1, nY1, nX2, nY2;

//	if (nPlaneNo >= gMap.nPlanesNb) return;
	if (nPlaneNo >= gMap.nPlanesNb || gnFrameMissed) return;

	// Coordonées de la fenêtre de base.
	nX1 = (gScrollM.pPlanePosX[nPlaneNo] >> 8) % SCROLLBUF_LG;
	nY1 = (gScrollM.pPlanePosY[nPlaneNo] >> 8) % SCROLLBUF_HT;
	nX2 = nX1 + SCR_Width - 1;
	nY2 = nY1 + SCR_Height - 1;

	if (nX2 >= SCROLLBUF_LG && nY2 >= SCROLLBUF_HT)
	{
		// 4 Blits.
		sRctSrc.x = nX1;
		sRctSrc.y = nY1;
		sRctSrc.w = SCROLLBUF_LG - nX1;
		sRctSrc.h = SCROLLBUF_HT - nY1;
		sRctDst.x = 0;
		sRctDst.y = 0;
		sRctDst.w = sRctSrc.w;
		sRctDst.h = sRctSrc.h;
		if (SDL_BlitSurface(gScrollM.ppPlanesScrollBuf[nPlaneNo], &sRctSrc, gVar.pScreen, &sRctDst) < 0) fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());

		sRctSrc.x = 0;
		sRctSrc.y = nY1;
		sRctSrc.w = nX2 - SCROLLBUF_LG + 1;
		sRctSrc.h = SCROLLBUF_HT - nY1;
		sRctDst.x = SCROLLBUF_LG - nX1;
		sRctDst.y = 0;
		sRctDst.w = sRctSrc.w;
		sRctDst.h = sRctSrc.h;
		if (SDL_BlitSurface(gScrollM.ppPlanesScrollBuf[nPlaneNo], &sRctSrc, gVar.pScreen, &sRctDst) < 0) fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());

		sRctSrc.x = nX1;
		sRctSrc.y = 0;
		sRctSrc.w = SCROLLBUF_LG - nX1;
		sRctSrc.h = nY2 - SCROLLBUF_HT + 1;
		sRctDst.x = 0;
		sRctDst.y = SCROLLBUF_HT - nY1;
		sRctDst.w = sRctSrc.w;
		sRctDst.h = sRctSrc.h;
		if (SDL_BlitSurface(gScrollM.ppPlanesScrollBuf[nPlaneNo], &sRctSrc, gVar.pScreen, &sRctDst) < 0) fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());

		sRctSrc.x = 0;
		sRctSrc.y = 0;
		sRctSrc.w = nX2 - SCROLLBUF_LG + 1;
		sRctSrc.h = nY2 - SCROLLBUF_HT + 1;
		sRctDst.x = SCROLLBUF_LG - nX1;
		sRctDst.y = SCROLLBUF_HT - nY1;
		sRctDst.w = sRctSrc.w;
		sRctDst.h = sRctSrc.h;
		if (SDL_BlitSurface(gScrollM.ppPlanesScrollBuf[nPlaneNo], &sRctSrc, gVar.pScreen, &sRctDst) < 0) fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
	}
	else
	if (nX2 >= SCROLLBUF_LG)
	{
		// 2 Blits.
		sRctSrc.x = nX1;
		sRctSrc.y = nY1;
		sRctSrc.w = SCROLLBUF_LG - nX1;
		sRctSrc.h = SCR_Height;
		sRctDst.x = 0;
		sRctDst.y = 0;
		sRctDst.w = sRctSrc.w;
		sRctDst.h = SCR_Height;
		if (SDL_BlitSurface(gScrollM.ppPlanesScrollBuf[nPlaneNo], &sRctSrc, gVar.pScreen, &sRctDst) < 0) fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());

		sRctSrc.x = 0;
		sRctSrc.y = nY1;
		sRctSrc.w = nX2 - SCROLLBUF_LG + 1;
		sRctSrc.h = SCR_Height;
		sRctDst.x = SCROLLBUF_LG - nX1;
		sRctDst.y = 0;
		sRctDst.w = sRctSrc.w;
		sRctDst.h = SCR_Height;
		if (SDL_BlitSurface(gScrollM.ppPlanesScrollBuf[nPlaneNo], &sRctSrc, gVar.pScreen, &sRctDst) < 0) fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
	}
	else
	if (nY2 >= SCROLLBUF_HT)
	{
		// 2 Blits.
		sRctSrc.x = nX1;
		sRctSrc.y = nY1;
		sRctSrc.w = SCR_Width;
		sRctSrc.h = SCROLLBUF_HT - nY1;
		sRctDst.x = 0;
		sRctDst.y = 0;
		sRctDst.w = SCR_Width;
		sRctDst.h = sRctSrc.h;
		if (SDL_BlitSurface(gScrollM.ppPlanesScrollBuf[nPlaneNo], &sRctSrc, gVar.pScreen, &sRctDst) < 0) fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());

		sRctSrc.x = nX1;
		sRctSrc.y = 0;
		sRctSrc.w = SCR_Width;
		sRctSrc.h = nY2 - SCROLLBUF_HT + 1;
		sRctDst.x = 0;
		sRctDst.y = SCROLLBUF_HT - nY1;
		sRctDst.w = SCR_Width;
		sRctDst.h = sRctSrc.h;
		if (SDL_BlitSurface(gScrollM.ppPlanesScrollBuf[nPlaneNo], &sRctSrc, gVar.pScreen, &sRctDst) < 0) fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
	}
	else
	{
		// 1 Blit.
		sRctSrc.x = nX1;
		sRctSrc.y = nY1;
		sRctSrc.w = SCR_Width;
		sRctSrc.h = SCR_Height;
		if (SDL_BlitSurface(gScrollM.ppPlanesScrollBuf[nPlaneNo], &sRctSrc, gVar.pScreen, NULL) < 0) fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
	}

}

//=============================================================================

// Pour les blocs animés, on a besoin de savoir ou se trouve chaque plan.
void ScrollGetPlanePosXY(s32 *pPosX, s32 *pPosY, u32 nPlane)
{
	*pPosX = gScrollM.pPlanePosX[nPlane];
	*pPosY = gScrollM.pPlanePosY[nPlane];
}

// Update du dessin d'un bloc pour les blocs animés.
void Scroll_BlkAnm_BlockUpdate(u32 nPlane, s32 sBlMapX, s32 sBlMapY, s32 nOffset)
{
	u32	k;
	s32	nBlockNo;
	u32	nBlX, nBlY;
	u16	*pSrc, *pDst;

	// Trace la colonne.
	SDL_LockSurface(gScrollM.ppPlanesScrollBuf[nPlane]);

	nBlockNo = *(*(gMap.ppPlanesBlocks + nPlane) + (sBlMapY * gMap.nMapLg) + sBlMapX) + nOffset;
	// Coordonées x,y du bloc dans son plan.
	nBlY = nBlockNo / (gMap.ppPlanesGfx[nPlane]->w / 16);
	nBlX = nBlockNo - (nBlY * (gMap.ppPlanesGfx[nPlane]->w / 16));
	// Src et Dst.
	pSrc = (u16 *)gMap.ppPlanesGfx[nPlane]->pixels + (nBlY * 16 * gMap.ppPlanesGfx[nPlane]->w) + (nBlX * 16);
	pDst = (u16 *)gScrollM.ppPlanesScrollBuf[nPlane]->pixels +
		(((sBlMapY % (SCROLLBUF_HT / 16)) * 16) * SCROLLBUF_LG) +
		((sBlMapX % (SCROLLBUF_LG / 16)) * 16);
	// Bloc.
	for (k = 0; k < 16; k++)
	{
		*(u32 *)pDst = *(u32 *)pSrc;
		*(u32 *)(pDst + 2) = *(u32 *)(pSrc + 2);
		*(u32 *)(pDst + 4) = *(u32 *)(pSrc + 4);
		*(u32 *)(pDst + 6) = *(u32 *)(pSrc + 6);
		*(u32 *)(pDst + 8) = *(u32 *)(pSrc + 8);
		*(u32 *)(pDst + 10) = *(u32 *)(pSrc + 10);
		*(u32 *)(pDst + 12) = *(u32 *)(pSrc + 12);
		*(u32 *)(pDst + 14) = *(u32 *)(pSrc + 14);
		pSrc += gMap.ppPlanesGfx[nPlane]->w;
		pDst += SCROLLBUF_LG;
	}

	SDL_UnlockSurface(gScrollM.ppPlanesScrollBuf[nPlane]);

}

