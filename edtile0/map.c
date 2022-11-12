
#include "includes.h"

#define	MAP_DEFAULT_LG	20
#define	MAP_DEFAULT_HT	20


// Constructeur.
struct SMap * Map_New(void)
{
	struct SMap *psMap;

	if ((psMap = (struct SMap *)malloc(sizeof(struct SMap))) == NULL)
	{
		fprintf(stderr, "Map_New(): malloc failed (psMap).\n");
		exit(1);
	}

	// Init.
	psMap->nPlanesNb = 0;		// Nb de plans.
	psMap->ppsPlanes = NULL;	// Les plans.

	psMap->nFirstWindow = 0;	// Première fenêtre de planches affichée.
	psMap->nActivePlane = 0;	// Plan actif.

	psMap->nMapLg = MAP_DEFAULT_LG;	// Lg et ht de la map.
	psMap->nMapHt = MAP_DEFAULT_HT;
	psMap->nMapPosX = 0;		// Pos (x,y) d'affichage de la map.
	psMap->nMapPosY = 0;

	psMap->psVisu = NULL;		// Visu non initialisée.
	psMap->nVisuOnOff = 0;		// On/Off.

	psMap->nModified = 0;

	return (psMap);
}

// Destructeur.
void Map_Delete(struct SMap *psMap)
{
//printf("CMap destruct\n");
	// Ecrasement des plans.
	if (psMap->nPlanesNb)
	{
		u32	i;
		for (i = 0; i < psMap->nPlanesNb; i++)
			Plane_Delete(psMap->ppsPlanes[i], psMap);
		free(psMap->ppsPlanes);
	}

	if (psMap->psVisu != NULL) Visu_Delete(psMap->psVisu);

	free(psMap);
}

// Ajoute un plan.
void Map_PlaneAdd(struct SMap *psMap, char *pFilename)
{
	u32	nNbP = psMap->nPlanesNb + 1;

	psMap->ppsPlanes = (struct SPlane **) realloc(psMap->ppsPlanes, sizeof(struct SPlane *) * nNbP);
	if (psMap->ppsPlanes == NULL)
	{
		fprintf(stderr, "Map_PlaneAdd(): realloc failed (ppPlanes).\n");
		exit(1);
	}
	*(psMap->ppsPlanes + psMap->nPlanesNb) = Plane_New();
	if (*(psMap->ppsPlanes + psMap->nPlanesNb) == NULL)
	{
		fprintf(stderr, "Map_PlaneAdd(): 'Plane_New' failed.\n");
		exit(1);
	}
	Plane_PlaneInit(*(psMap->ppsPlanes + psMap->nPlanesNb), pFilename);

	psMap->nPlanesNb++;		// Nb de plans + 1.

}

// Insère un plan.
void Map_PlaneInsert(struct SMap *psMap, char *pFilename, u32 nInsertBefore)
{
	// On commence par ajouter le plan.
	Map_PlaneAdd(psMap, pFilename);

	if (nInsertBefore >= psMap->nPlanesNb - 1) return;	// Si position débile, on le laisse en dernière position.

	// Puis on décale la table.
	struct SPlane	*pLastPlane = *(psMap->ppsPlanes + psMap->nPlanesNb - 1);
	s32	i;		// Signé pour le cas de l'insertion en 0.
	for (i = psMap->nPlanesNb - 2; i >= (s32)nInsertBefore; i--)
	{
		*(psMap->ppsPlanes + i + 1) = *(psMap->ppsPlanes + i);
	}
	*(psMap->ppsPlanes + nInsertBefore) = pLastPlane;

}

// Supprime un plan.
void Map_PlaneRemove(struct SMap *psMap, u32 nPlaneNo)
{
	// Petit contrôle.
	if (nPlaneNo >= psMap->nPlanesNb)
	{
		printf("Map_PlaneRemove(): Called with non existent plane number (%d).\n", (int)nPlaneNo);
		return;
	}

	// Suppression du plan.
//-	delete _ppsPlanes[nPlaneNo];
	Plane_Delete(psMap->ppsPlanes[nPlaneNo], psMap);
	// Décalage des pointeurs.
	u32	i;
	for (i = nPlaneNo; i < psMap->nPlanesNb - 1; i++) psMap->ppsPlanes[i] = psMap->ppsPlanes[i + 1];
	psMap->ppsPlanes[psMap->nPlanesNb - 1] = NULL;

	psMap->nPlanesNb--;		// Nb de plans - 1.

	Map_PlanesSetNo(psMap);	// Renumérote les plans.

	// On est toujours bon au niveau de la fenêtre active ? (Car suppresion du dernier plan + plan en cours => pb).
	if (psMap->nActivePlane >= psMap->nPlanesNb) psMap->nActivePlane = psMap->nPlanesNb - 1;
	if (psMap->nFirstWindow >= psMap->nPlanesNb) psMap->nFirstWindow = psMap->nPlanesNb - 1;

}

// Affichage des planches dans leurs fenêtres.
void Map_PlanesDisplayWnds(struct SMap *psMap)
{
	u32	i, offs, res;

	Mouse_RectReset(gMisc.psMouse);		// RAZ rectangles souris.

	if (Misc_FullScreenGet(&gMisc)) return;	// Pas en full screen.

	// Effacement de la zone des plans.
	SDL_Rect dstrect;
	dstrect.x = 0;
	dstrect.y = 0;
	dstrect.w = gMisc.nScrW;
	dstrect.h = PLN_MAX_BLOCKS_H * 16;
	SDL_FillRect(gMisc.pScreen, &dstrect, SDL_MapRGB(gMisc.pScreen->format, 0, 0, 0));

	// Affichage des plans.
	offs = 0;
	for (i = psMap->nFirstWindow; i < psMap->nPlanesNb; i++)
	{
//-		res = (*(psMap->ppsPlanes + i))->WndDisplay(offs);
//-		res = Plane_WndDisplay(*(psMap->ppsPlanes + i), offs);
		res = Plane_WndDisplay(psMap->ppsPlanes[i], offs);
		if (res == 0) break;
		offs += res;
	}

}

// Affichage de la Map.
void Map_MapDisplay(struct SMap *psMap)
{
	u32	i, ix, iy, nClr, nMapX, nMapY;
	u32	nStartY = (Misc_FullScreenGet(&gMisc) ? 0 : PLN_MAX_BLOCKS_H);

	// MAJ rect pour souris.
	Mouse_RectMap(gMisc.psMouse, 0, nStartY * 16, gMisc.nScrW - 1, (gMisc.nScrH & ~0x0F) - 1 - 16);

	// Effacement de la zone des plans.
	SDL_LockSurface(gMisc.pScreen);
	for (iy = nStartY; iy < (gMisc.nScrH / 16) - 1; iy++)
	{
		for (ix = 0; ix < (gMisc.nScrW / 16); ix++)
		{
			nMapX = ix + psMap->nMapPosX;
			nMapY = iy - nStartY + psMap->nMapPosY;
			nClr = (nMapX < psMap->nMapLg && nMapY < psMap->nMapHt ? e_CheckBlk_Empty : e_CheckBlk_Bkg);
			Gfx_DrawCheckBlock(nClr, ix, iy, false);
		}
	}
	SDL_UnlockSurface(gMisc.pScreen);

	// Affichage des plans.
	for (i = 0; i < psMap->nPlanesNb; i++)
	{
//-		Plane_MapDisplay(*(psMap->ppsPlanes + i));
		Plane_MapDisplay(psMap->ppsPlanes[i]);
	}

}

// Conversion des gfx en 16 bits.
// 1 seul appel APRES l'init vidéo !
void Map_GfxConvert(struct SMap *psMap)
{
	u32	i;
	for (i = 0; i < psMap->nPlanesNb; i++)
	{
//-		Plane_GfxConvert(*(psMap->ppsPlanes + i));
		Plane_GfxConvert(psMap->ppsPlanes[i]);
	}
}

// Insertion d'une colonne dans la map.
void Map_MapColumnInsert(struct SMap *psMap)
{
	if (Mouse_MouseOnMap(gMisc.psMouse) == 0) return;	// Il faut que la souris soit sur la map.

	// Insertion de la colonne sur tous les plans.
	u32	i;
	for (i = 0; i < psMap->nPlanesNb; i++)
	{
		Plane_PlaneColumnInsert(*(psMap->ppsPlanes + i), gMisc.psMouse->nBlkPosX);
	}
	// Toutes les insertions ont fonctionné, lg+1.
	psMap->nMapLg++;

}

// Suppression d'une colonne dans la map.
void Map_MapColumnDelete(struct SMap *psMap)
{
	if (Mouse_MouseOnMap(gMisc.psMouse) == 0) return;	// Il faut que la souris soit sur la map.
	if (psMap->nMapLg <= 1) return;

	// Suppression de la colonne sur tous les plans.
	u32	i;
	for (i = 0; i < psMap->nPlanesNb; i++)
	{
		Plane_PlaneColumnDelete(*(psMap->ppsPlanes + i), gMisc.psMouse->nBlkPosX);
	}
	// Toutes les suppressions ont fonctionné, lg-1.
	psMap->nMapLg--;

}

// Insertion d'une ligne dans la map.
void Map_MapLineInsert(struct SMap *psMap)
{
	if (Mouse_MouseOnMap(gMisc.psMouse) == 0) return;	// Il faut que la souris soit sur la map.

	// Insertion de la ligne sur tous les plans.
	u32	i;
	for (i = 0; i < psMap->nPlanesNb; i++)
	{
		Plane_PlaneLineInsert(*(psMap->ppsPlanes + i), gMisc.psMouse->nBlkPosY);
	}
	// Toutes les insertions ont fonctionné, ht+1.
	psMap->nMapHt++;

}

// Suppression d'une ligne dans la map.
void Map_MapLineDelete(struct SMap *psMap)
{
	if (Mouse_MouseOnMap(gMisc.psMouse) == 0) return;	// Il faut que la souris soit sur la map.
	if (psMap->nMapHt <= 1) return;

	// Suppression de la ligne sur tous les plans.
	u32	i;
	for (i = 0; i < psMap->nPlanesNb; i++)
	{
		Plane_PlaneLineDelete(*(psMap->ppsPlanes + i), gMisc.psMouse->nBlkPosY);
	}
	// Toutes les suppressions ont fonctionné, ht-1.
	psMap->nMapHt--;

}

// Donne leurs numéros aux plans.
void Map_PlanesSetNo(struct SMap *psMap)
{
	u32	i;
	for (i = 0; i < psMap->nPlanesNb; i++)
	{
		(*(psMap->ppsPlanes + i))->nPlaneNo = i;
	}
}

// Affiche un bloc à damiers.
// In : nPosX, nPosY en blocs.
void Gfx_DrawCheckBlock(u32 nType, u32 nPosX, u32 nPosY, bool bLock)
{
	u16	nClr1, nClr2;

	if ((nPosX * 16) + 15 >= gMisc.nScrW || (nPosY * 16) + 15 >= gMisc.nScrH) return;	// Pas de pixels en dehors de l'écran.

	switch (nType)
	{
	case e_CheckBlk_Bkg:
		nClr1 = SDL_MapRGB(gMisc.pScreen->format, 132, 130, 132);
		nClr2 = SDL_MapRGB(gMisc.pScreen->format, 99, 97, 99);
		break;

	case e_CheckBlk_RedDark:
		nClr1 = SDL_MapRGB(gMisc.pScreen->format, 132, 65, 66);
		nClr2 = nClr1;
		break;

	case e_CheckBlk_RedDark2:
		nClr1 = SDL_MapRGB(gMisc.pScreen->format, 132-20, 65-20, 66-20);
		nClr2 = nClr1;
		break;

	case e_CheckBlk_RedLight:
		nClr1 = SDL_MapRGB(gMisc.pScreen->format, 198, 97, 99);
		nClr2 = nClr1;
		break;

	case e_CheckBlk_BlueDark:
		nClr1 = SDL_MapRGB(gMisc.pScreen->format, 66, 65, 132);
		nClr2 = nClr1;
		break;

	case e_CheckBlk_BlueLight:
		nClr1 = SDL_MapRGB(gMisc.pScreen->format, 99, 97, 198);
		nClr2 = nClr1;
		break;

	case e_CheckBlk_Empty:
	default:
		nClr1 = SDL_MapRGB(gMisc.pScreen->format, 255, 255, 255);
		nClr2 = SDL_MapRGB(gMisc.pScreen->format, 0, 0, 0);
		break;
	}

	// Affichage.
	if (bLock) SDL_LockSurface(gMisc.pScreen);

	u16	*pScr = (u16 *)gMisc.pScreen->pixels;

	u32	i, j;
	for (j = 0; j < 8; j++)
	{
		for (i = 0; i < 8; i++)
		{
			*(pScr + (((nPosY * 16) + j) * gMisc.nScrW) + ((nPosX) * 16) + i) = nClr1;
			*(pScr + (((nPosY * 16) + j) * gMisc.nScrW) + ((nPosX) * 16) + i + 8) = nClr2;
			*(pScr + (((nPosY * 16) + j + 8) * gMisc.nScrW) + ((nPosX) * 16) + i) = nClr2;
			*(pScr + (((nPosY * 16) + j + 8) * gMisc.nScrW) + ((nPosX) * 16) + i + 8) = nClr1;
		}
	}

	if (bLock) SDL_UnlockSurface(gMisc.pScreen);

}

// Changement de la première fenêtre affichée.
void Map_FirstWndMove(struct SMap *psMap, s32 nSens)
{
	if (nSens < 0)
	{
		if (psMap->nFirstWindow) psMap->nFirstWindow--;
	}
	else
	{
		if (psMap->nFirstWindow < psMap->nPlanesNb - 1) psMap->nFirstWindow++;
	}
}

// Déplacement de la map.
void Map_MapMoveXY(struct SMap *psMap, s32 nSensX, s32 nSensY)
{
	if (nSensX)
	{
		if (nSensX < 0)
		{
			if (psMap->nMapPosX) psMap->nMapPosX--;
		}
		else
		{
			if (psMap->nMapPosX < psMap->nMapLg - 1) psMap->nMapPosX++;
		}
	}

	if (nSensY)
	{
		if (nSensY < 0)
		{
			if (psMap->nMapPosY) psMap->nMapPosY--;
		}
		else
		{
			if (psMap->nMapPosY < psMap->nMapHt - 1) psMap->nMapPosY++;
		}
	}

}

// Toggle d'un plan.
void Map_PlaneToggle(struct SMap *psMap, u32 nPlaneNo)
{
	if (nPlaneNo < psMap->nPlanesNb)
	{
		Plane_Toggle(*(psMap->ppsPlanes + nPlaneNo));
		//PlanesDisplayWnds();
	}
}

// Barre d'état.
void Map_StatusBarDisplay(struct SMap *psMap)
{
	struct SMouse	*psMouse = gMisc.psMouse;
	u32	ix;

	// La barre.
	SDL_LockSurface(gMisc.pScreen);
	for (ix = 0; ix < (gMisc.nScrW / 16); ix++)
	{
		Gfx_DrawCheckBlock(e_CheckBlk_BlueDark, ix, (gMisc.nScrH / 16) - 1, false);
	}
	SDL_UnlockSurface(gMisc.pScreen);

	// Affichage de la position de la souris.
	Font_StrReset(&gFont);
	if (psMouse->pWhere == NULL || psMouse->nBlkPosX == -1)
	{
		Font_StrAdd(&gFont, "(---,---)");
	}
	else
	{
		Font_StrAdd(&gFont, "(");
		Font_StrItoa(&gFont, psMouse->nBlkPosX);
		Font_StrAdd(&gFont, ",");
		Font_StrItoa(&gFont, psMouse->nBlkPosY);
		Font_StrAdd(&gFont, ")");
	}
	Font_Printf(4, (gMisc.nScrH & ~0x0F) - 16 + 4, gFont.pString, 0xFFFF);

	// Affichage du bloc pointé par la souris.
	Font_StrReset(&gFont);
	Font_StrAdd(&gFont, "BLK:");
	if (psMouse->pWhere == NULL || psMouse->nBlkPosX == -1)
	{
		Font_StrAdd(&gFont, "---");
	}
	else
	{
		struct SPlane	*pPln;
		if (psMouse->pWhere->nWhere == e_MouseLoc_Planes)
		{
//gMap?			pPln = gMap._ppsPlanes[psMouse->pWhere->nVar];	// Dans les plans, nVar indique le n° de plan.
			pPln = psMap->ppsPlanes[psMouse->pWhere->nVar];	// Dans les plans, nVar indique le n° de plan.
			Font_StrItoa(&gFont, (psMouse->nBlkPosY * (pPln->pGfx->w / 16)) + psMouse->nBlkPosX);
		}
		else
		if (psMouse->pWhere->nWhere == e_MouseLoc_Map)
		{
			s32	nBlk;
//gMap?			pPln = gMap._ppsPlanes[gMap._nActivePlane];
//gMap?			nBlk = *(pPln->pPlane + (psMouse->nBlkPosY * gMap._nMapLg) + psMouse->nBlkPosX);
			pPln = psMap->ppsPlanes[psMap->nActivePlane];
			nBlk = *(pPln->pPlane + (psMouse->nBlkPosY * psMap->nMapLg) + psMouse->nBlkPosX);
			if (nBlk == -1) Font_StrAdd(&gFont, "empty"); else Font_StrItoa(&gFont, nBlk);
		}
	}
	Font_Printf(4 + (11 * 8), (gMisc.nScrH & ~0x0F) - 16 + 4, gFont.pString, 0xFFFF);

	// Taille de la brosse.
	Font_StrReset(&gFont);
	Font_StrAdd(&gFont, "BRUSH:");
	if (psMouse->nState == e_MouseState_GrabStarted)
	{
		Font_StrAdd(&gFont, "(");
		Font_StrItoa(&gFont, abs(psMouse->nBrushXOrigin - psMouse->nBlkPosX) + 1);
		Font_StrAdd(&gFont, ",");
		Font_StrItoa(&gFont, abs(psMouse->nBrushYOrigin - psMouse->nBlkPosY) + 1);
		Font_StrAdd(&gFont, ")");
	}
	else if (psMouse->nState == e_MouseState_Brush)
	{
		Font_StrAdd(&gFont, "(");
		Font_StrItoa(&gFont, psMouse->nBrushLg);
		Font_StrAdd(&gFont, ",");
		Font_StrItoa(&gFont, psMouse->nBrushHt);
		Font_StrAdd(&gFont, ")");
	}
	else
	{
		Font_StrAdd(&gFont, "EMPTY");
	}
	Font_Printf(4 + (23 * 8), (gMisc.nScrH & ~0x0F) - 16 + 4, gFont.pString, 0xFFFF);

	// Affichage du flag 'transparence' au bout à droite.
	u16	nColor = (Misc_TransparencyGet(&gMisc) ? SDL_MapRGB(gMisc.pScreen->format, 255, 100, 100) : 0xFFFF);
	Font_Printf(gMisc.nScrW - 16 - 8, (gMisc.nScrH & ~0x0F) - 16 + 4, "T", nColor);

	// Affichage du flag 'modified' au bout à droite.
	char *pStar = "*", *pMinus = "-";
	Font_Printf(gMisc.nScrW - 16, (gMisc.nScrH & ~0x0F) - 16 + 4, (psMap->nModified == 0 ? pMinus : pStar), 0xFFFF);
//	Font_Printf(gMisc.nScrW - 16, (gMisc.nScrH & ~0x0F) - 16 + 4, (psMap->nModified == 0 ? "-" : "*"), 0xFFFF);	// ???


/*
// debug
Font_StrReset(&gFont);
Font_StrAdd(&gFont, "mouse:");
Font_StrItoa(&gFont, psMouse->nState);
Font_Printf(240, (gMisc.nScrH & ~0x0F) - 16 + 4, gFont.pString, 0xFFFF);

Font_StrReset(&gFont);
Font_StrAdd(&gFont, "on monster:");
Font_StrItoa(&gFont, gInfoBox.nMouseOnMonster);
Font_Printf(340, (gMisc.nScrH & ~0x0F) - 16 + 4, gFont.pString, 0xFFFF);
*/

}

// Ajoute un fichier de commandes à un plan.
void Map_PlaneCmdfileAdd(struct SMap *psMap, u32 nPlaneNo, char *pCmdFilename)
{
	Plane_CmdfileAdd(*(psMap->ppsPlanes + nPlaneNo), pCmdFilename);

}

// Ajoute un fichier de commandes à un plan, depuis un buffer.
void Map_PlaneCmdfileAddFromBuf(struct SMap *psMap, u32 nPlaneNo, char *pCmdFilename, char *pCmdBuf, u32 nCmdDate)
{
	Plane_CmdfileAddFromBuf(*(psMap->ppsPlanes + nPlaneNo), pCmdFilename, pCmdBuf, nCmdDate);

}

// Toggle visu.
void Map_VisuToggle(struct SMap *psMap)
{
	if (psMap->psVisu == NULL) return;	// Visu non initialisée ?

	psMap->nVisuOnOff ^= 1;
	Visu_VisuToggle(psMap->psVisu, psMap->nVisuOnOff);
	gMisc.psMouse->nVisuDrag = 0;			// RAZ du drag.
	Mouse_LocationGet(gMisc.psMouse);		// MAJ position de la souris
}

void Map_VisuDisplay(struct SMap *psMap)
{
	Visu_DrawVisu(psMap->psVisu);
}

u32 Map_VisuOnOff(struct SMap *psMap)
{
	return (psMap->nVisuOnOff);
}

void Map_ActivePlaneSet(struct SMap *psMap, u32 nPlaneNo)
{
	psMap->nActivePlane = nPlaneNo;
	//DisplayAll();
}

u32 Map_ActivePlaneGet(struct SMap *psMap)
{
	return (psMap->nActivePlane);
}

void Map_PlaneWndMoveXY(struct SMap *psMap, u32 nPlaneNo, s32 nSensX, s32 nSensY)
{
	Plane_WndMoveXY(*(psMap->ppsPlanes + nPlaneNo), nSensX, nSensY);
};

void Map_PlaneAlphaToggle(struct SMap *psMap)
{
	Plane_AlphaToggle(*(psMap->ppsPlanes + psMap->nActivePlane));
}

u32 Map_PlaneHasAlpha(struct SMap *psMap, u32 nPlaneNo)
{
	return (Plane_HasAlpha(*(psMap->ppsPlanes + nPlaneNo)));
}

u32 Map_PlaneHasCmd(struct SMap *psMap, u32 nPlaneNo)
{
	return (Plane_HasCmd(*(psMap->ppsPlanes + nPlaneNo)));
}

void Map_PlaneCodesAdd(struct SMap *psMap, u32 nPlaneNo)
{
	Plane_CodesPlaneAddDefaultSz(*(psMap->ppsPlanes + nPlaneNo));
}

void Map_PlaneCodesRemove(struct SMap *psMap, u32 nPlaneNo)
{
	Plane_CodesPlaneRemove(*(psMap->ppsPlanes + nPlaneNo));
}

void Map_PlaneCodesToggle(struct SMap *psMap)
{
	Plane_CodesToggle(*(psMap->ppsPlanes + psMap->nActivePlane));
}

u32 Map_PlaneHasCodes(struct SMap *psMap, u32 nPlaneNo)
{
	return (Plane_HasCodes(*(psMap->ppsPlanes + nPlaneNo)));
}

void Map_PlaneCodeAdd(struct SMap *psMap, u32 nPlaneNo, u32 nBlockNo, s8 nInc)
{
	Plane_CodeAdd(*(psMap->ppsPlanes + nPlaneNo), nBlockNo, nInc);
}

void Map_PlaneCodeSet(struct SMap *psMap, u32 nPlaneNo, u32 nBlockNo, u8 nVal)
{
	Plane_CodeSet(*(psMap->ppsPlanes + nPlaneNo), nBlockNo, nVal);
}


//=============================================================================

/*
_nPlanesNb = 0;		// Nb de plans.
_ppPlanes = NULL;	// Les plans.

	_nFirstWindow = 0;	// Première fenêtre de planches affichée.
	_nActivePlane = 0;	// Plan actif.

_nMapLg = MAP_DEFAULT_LG;	// Lg et ht de la map.
_nMapHt = MAP_DEFAULT_HT;
_nMapPosX = 0;		// Pos (x,y) d'affichage de la map.
_nMapPosY = 0;

	_pVisu = NULL;		// Visu non initialisée.
	_nVisuOnOff = 0;	// On/Off.
*/

/*
//v1 : On revoit parce que UndoCopy ne copie QUE gMap, et UndoUndo ne copie QUE sur gMap.

// Recopie les datas utiles pour un undo.
// Out : 0 = Ok / 1 = Failed.
u32 CMap::UndoCopy(CMap *psMapToCp)
{
	u32	i, ix;

	_nMapLg = psMapToCp->_nMapLg;		// Lg et ht de la map.
	_nMapHt = psMapToCp->_nMapHt;
	_nMapPosX = psMapToCp->_nMapPosX;	// Pos (x,y) d'affichage de la map.
	_nMapPosY = psMapToCp->_nMapPosY;

	// Rajoute les plans.
	for (i = 0; i < psMapToCp->_nPlanesNb; i++)
	{
		// Ajout.
		PlaneAdd(NULL);
//printf("ptr cmd %p\n", (*(_ppPlanes + i))->_ppMstPlane );
		// Sauvegarde des blocs.
		memcpy((*(_ppPlanes + i))->pPlane, (*(psMapToCp->_ppPlanes + i))->pPlane, _nMapLg * _nMapHt * sizeof(s32));

		// Est-ce un plan de monstres ?
		if (gMap.PlaneHasCmd(i))	// << On fait le test avec gMap car on n'ajoute pas
									// de fichier de commandes sur les maps sauvegardées dans l'undo.
									// Comme l'ajout se fait uniquement à l'ouverture, ok.
		{
//printf("hascmd %d %p\n", i, *(_ppPlanes + i));
			// Initialisation du plan de pointeurs.
			Plane_MstPlaneInit(*(_ppPlanes + i));

			// Parcours des pointeurs. Quand on trouve un monstre, on le recopie.
			for (ix = 0; ix < _nMapLg * _nMapHt; ix++)
			{
				if ( *((*(psMapToCp->_ppPlanes + i))->ppMstPlane + ix) != NULL)
				{
					*((*(_ppPlanes + i))->ppMstPlane + ix) = MstUnit_New();
//					(*((*(_ppPlanes + i))->ppMstPlane + ix))->InitFromMst( *((*(psMapToCp->_ppPlanes + i))->ppMstPlane + ix) );
					MstUnit_InitFromMst( *((*(_ppPlanes + i))->ppMstPlane + ix), *((*(psMapToCp->_ppPlanes + i))->ppMstPlane + ix) );
//printf("mst cp. idx=%d\n", ix);
				}
			}
		}

		// Est-ce qu'il y a des codes ?
		if (gMap.PlaneHasCodes(i))	// << Idem au dessus. Une fois la map ouverte, on ne rajoute/supprime plus de pages de codes. De plus, la taille des planches ne change pas.
		{
//-			(*(_ppPlanes + i))->CodesPlaneAdd( (*(gMap._ppPlanes + i))->_pGfx->w / 16, (*(gMap._ppPlanes + i))->_pGfx->h / 16 );	// Si ça rate, exit.
			Plane_CodesPlaneAdd(*(_ppPlanes + i), (*(gMap._ppPlanes + i))->pGfx->w / 16, (*(gMap._ppPlanes + i))->pGfx->h / 16 );	// Si ça rate, exit.
			memcpy( (*(_ppPlanes + i))->pCodes, (*(psMapToCp->_ppPlanes + i))->pCodes, ((*(gMap._ppPlanes + i))->pGfx->w / 16) * ((*(gMap._ppPlanes + i))->pGfx->h / 16) );
		}

	}

	return (0);
}

// Undo.
void CMap::UndoUndo(CMap *psMapSrc)
{
	u32	i, ix;

	u32	nOldMapLg, nOldMapHt;
	nOldMapLg = _nMapLg;
	nOldMapHt = _nMapHt;

	_nMapLg = psMapSrc->_nMapLg;		// Restore lg et ht de la map.
	_nMapHt = psMapSrc->_nMapHt;
	_nMapPosX = psMapSrc->_nMapPosX;	// Restore pos (x,y) d'affichage de la map.
	_nMapPosY = psMapSrc->_nMapPosY;

	// Restore des plans.
	for (i = 0; i < psMapSrc->_nPlanesNb; i++)
	{
		// Realloc.
		if (( (*(_ppPlanes + i))->pPlane = (s32 *)realloc( (*(_ppPlanes + i))->pPlane, _nMapLg * _nMapHt * sizeof(s32))) == NULL)
		{
			fprintf(stderr, "CMap::UndoUndo(): realloc failed (_pPlane).\n");
			exit(1);
		}
		// Copie des blocs sauvegardés.
		memcpy((*(_ppPlanes + i))->pPlane, (*(psMapSrc->_ppPlanes + i))->pPlane, _nMapLg * _nMapHt * sizeof(s32));

		// Est-ce un plan de monstres ?
		if (gMap.PlaneHasCmd(i))	// << Idem UndoCopy().
		{
			// Effacement des monstres.
			for (ix = 0; ix < nOldMapLg * nOldMapHt; ix++)
				if ( *((*(_ppPlanes + i))->ppMstPlane + ix) != NULL)
//-					delete *((*(_ppPlanes + i))->ppMstPlane + ix);
					MstUnit_Delete( *((*(_ppPlanes + i))->ppMstPlane + ix) );
			// Realloc.
			if (( (*(_ppPlanes + i))->ppMstPlane = (struct SMstUnit **)realloc((*(_ppPlanes + i))->ppMstPlane, _nMapLg * _nMapHt * sizeof(struct SMstUnit *))) == NULL)
			{
				fprintf(stderr, "CMap::UndoUndo(): realloc failed (_ppMstPlane).\n");
				exit(1);
			}
//-			(*(_ppPlanes + i))->MstPlaneClear();	// RAZ pointeurs.
			Plane_MstPlaneClear(*(_ppPlanes + i));	// RAZ pointeurs.

			// Parcours des pointeurs sauvegardés. Quand on trouve un monstre, on le recopie.
			for (ix = 0; ix < _nMapLg * _nMapHt; ix++)
			{
				if ( *((*(psMapSrc->_ppPlanes + i))->ppMstPlane + ix) != NULL)
				{
					*((*(_ppPlanes + i))->ppMstPlane + ix) = MstUnit_New();
//					(*((*(_ppPlanes + i))->ppMstPlane + ix))->InitFromMst( *((*(psMapSrc->_ppPlanes + i))->ppMstPlane + ix) );
					MstUnit_InitFromMst( *((*(_ppPlanes + i))->ppMstPlane + ix), *((*(psMapSrc->_ppPlanes + i))->ppMstPlane + ix) );
//printf("mst cp. idx=%d\n", ix);
				}
			}
		}

		// Est-ce qu'il y a des codes ?
		if (gMap.PlaneHasCodes(i))	// << Idem au dessus. Une fois la map ouverte, on ne rajoute/supprime plus de pages de codes. De plus, la taille des planches ne change pas.
		{
			memcpy( (*(_ppPlanes + i))->pCodes, (*(psMapSrc->_ppPlanes + i))->pCodes, ((*(gMap._ppPlanes + i))->pGfx->w / 16) * ((*(gMap._ppPlanes + i))->pGfx->h / 16) );
		}

	}

}
*/


// v2 :
// UndoCopy : Copie les datas de la map principale sur une map de sauvegarde.
// UndoUndo : Copie les datas d'une map de sauvegarde sur la map principale.

// Recopie les datas utiles pour un undo.
// Out : 0 = Ok / 1 = Failed.
u32 Map_UndoCopy(struct SMap *psMapDst)
{
	u32	i, ix;

	struct SMap *psMapSrc;
	psMapSrc = gMisc.psMap;				// !!! On ne fait des copies qu'à partir de la map principale !!!

	psMapDst->nMapLg = psMapSrc->nMapLg;		// Lg et ht de la map.
	psMapDst->nMapHt = psMapSrc->nMapHt;
	psMapDst->nMapPosX = psMapSrc->nMapPosX;	// Pos (x,y) d'affichage de la map.
	psMapDst->nMapPosY = psMapSrc->nMapPosY;

	// Rajoute les plans.
	for (i = 0; i < psMapSrc->nPlanesNb; i++)
	{
		// Ajout.
		Map_PlaneAdd(psMapDst, NULL);
		// Sauvegarde des blocs.
		memcpy((*(psMapDst->ppsPlanes + i))->pPlane, (*(psMapSrc->ppsPlanes + i))->pPlane, psMapDst->nMapLg * psMapDst->nMapHt * sizeof(s32));

		// Est-ce un plan de monstres ?
		if (Map_PlaneHasCmd(psMapSrc, i))	// << On fait le test avec gMap car on n'ajoute pas
									// de fichier de commandes sur les maps sauvegardées dans l'undo.
									// Comme l'ajout se fait uniquement à l'ouverture, ok.
		{
			// Initialisation du plan de pointeurs.
			Plane_MstPlaneInit(*(psMapDst->ppsPlanes + i));

			// Parcours des pointeurs. Quand on trouve un monstre, on le recopie.
			for (ix = 0; ix < psMapDst->nMapLg * psMapDst->nMapHt; ix++)
			{
				if ( *((*(psMapSrc->ppsPlanes + i))->ppMstPlane + ix) != NULL)
				{
					*((*(psMapDst->ppsPlanes + i))->ppMstPlane + ix) = MstUnit_New();
					MstUnit_InitFromMst( *((*(psMapDst->ppsPlanes + i))->ppMstPlane + ix), *((*(psMapSrc->ppsPlanes + i))->ppMstPlane + ix) );
				}
			}
		}

		// Est-ce qu'il y a des codes ?
		if (Map_PlaneHasCodes(psMapSrc, i))	// << Idem au dessus. Une fois la map ouverte, on ne rajoute/supprime plus de pages de codes. De plus, la taille des planches ne change pas.
		{
			Plane_CodesPlaneAdd(*(psMapDst->ppsPlanes + i), (*(psMapSrc->ppsPlanes + i))->pGfx->w / 16, (*(psMapSrc->ppsPlanes + i))->pGfx->h / 16 );	// Si ça rate, exit.
			memcpy( (*(psMapDst->ppsPlanes + i))->pCodes, (*(psMapSrc->ppsPlanes + i))->pCodes, ((*(psMapSrc->ppsPlanes + i))->pGfx->w / 16) * ((*(psMapSrc->ppsPlanes + i))->pGfx->h / 16) );
		}

	}

	return (0);
}

// Undo.
void Map_UndoUndo(struct SMap *psMapSrc)
{
	u32	i, ix;

	struct SMap *psMapDst;
	psMapDst = gMisc.psMap;				// !!! On ne fait des copies que sur la map principale !!!

	u32	nOldMapLg, nOldMapHt;
	nOldMapLg = psMapDst->nMapLg;
	nOldMapHt = psMapDst->nMapHt;

	psMapDst->nMapLg = psMapSrc->nMapLg;		// Restore lg et ht de la map.
	psMapDst->nMapHt = psMapSrc->nMapHt;
	psMapDst->nMapPosX = psMapSrc->nMapPosX;	// Restore pos (x,y) d'affichage de la map.
	psMapDst->nMapPosY = psMapSrc->nMapPosY;

	// Restore des plans.
	for (i = 0; i < psMapSrc->nPlanesNb; i++)
	{
		// Realloc.
		if (( (*(psMapDst->ppsPlanes + i))->pPlane = (s32 *)realloc( (*(psMapDst->ppsPlanes + i))->pPlane, psMapDst->nMapLg * psMapDst->nMapHt * sizeof(s32))) == NULL)
		{
			fprintf(stderr, "Map_UndoUndo(): realloc failed (pPlane).\n");
			exit(1);
		}
		// Copie des blocs sauvegardés.
		memcpy((*(psMapDst->ppsPlanes + i))->pPlane, (*(psMapSrc->ppsPlanes + i))->pPlane, psMapDst->nMapLg * psMapDst->nMapHt * sizeof(s32));

		// Est-ce un plan de monstres ?
		if (Map_PlaneHasCmd(psMapDst, i))	// << Idem UndoCopy().
		{
			// Effacement des monstres.
			for (ix = 0; ix < nOldMapLg * nOldMapHt; ix++)
				if ( *((*(psMapDst->ppsPlanes + i))->ppMstPlane + ix) != NULL)
					MstUnit_Delete( *((*(psMapDst->ppsPlanes + i))->ppMstPlane + ix) );
			// Realloc.
			if (( (*(psMapDst->ppsPlanes + i))->ppMstPlane = (struct SMstUnit **)realloc((*(psMapDst->ppsPlanes + i))->ppMstPlane, psMapDst->nMapLg * psMapDst->nMapHt * sizeof(struct SMstUnit *))) == NULL)
			{
				fprintf(stderr, "Map_UndoUndo(): realloc failed (ppMstPlane).\n");
				exit(1);
			}
			Plane_MstPlaneClear(*(psMapDst->ppsPlanes + i));	// RAZ pointeurs.

			// Parcours des pointeurs sauvegardés. Quand on trouve un monstre, on le recopie.
			for (ix = 0; ix < psMapDst->nMapLg * psMapDst->nMapHt; ix++)
			{
				if ( *((*(psMapSrc->ppsPlanes + i))->ppMstPlane + ix) != NULL)
				{
					*((*(psMapDst->ppsPlanes + i))->ppMstPlane + ix) = MstUnit_New();
					MstUnit_InitFromMst( *((*(psMapDst->ppsPlanes + i))->ppMstPlane + ix), *((*(psMapSrc->ppsPlanes + i))->ppMstPlane + ix) );
				}
			}
		}

		// Est-ce qu'il y a des codes ?
		if (Map_PlaneHasCodes(psMapDst, i))	// << Idem au dessus. Une fois la map ouverte, on ne rajoute/supprime plus de pages de codes. De plus, la taille des planches ne change pas.
		{
			memcpy( (*(psMapDst->ppsPlanes + i))->pCodes, (*(psMapSrc->ppsPlanes + i))->pCodes, ((*(psMapDst->ppsPlanes + i))->pGfx->w / 16) * ((*(psMapDst->ppsPlanes + i))->pGfx->h / 16) );
		}

	}

}


