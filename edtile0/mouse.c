
#include "includes.h"


// Constructeur.
struct SMouse * Mouse_New(void)
{
	struct SMouse	*psMouse;

	if ((psMouse = (struct SMouse *)malloc(sizeof(struct SMouse))) == NULL)
	{
		fprintf(stderr, "Mouse_New(): malloc failed (psMouse).\n");
		exit(1);
	}

	// Init.
	psMouse->nBlkPosX = psMouse->nBlkPosY = -1;	// Pos de la souris en blocs. (-1,-1) si nulle part.
	psMouse->nPixPosX = psMouse->nPixPosY = 0;
	psMouse->pWhere = NULL;		// Où est la souris. NULL si nulle part.
	psMouse->nState = e_MouseState_Idle;
	psMouse->pBrush = NULL;
	psMouse->ppMstBrush = NULL;
	Mouse_RectReset(psMouse);
	psMouse->nTile = 0;			// Tiling.
	psMouse->nVisuDrag = 0;		// Drag de la visu.

	return (psMouse);
}

// Destructeur.
void Mouse_Delete(struct SMouse *psMouse)
{
	Mouse_BrushRelease(psMouse);

	free(psMouse);
}

// Reset les rectangles.
void Mouse_RectReset(struct SMouse *psMouse)
{
	psMouse->nRectNb = 0;
}

// Ajout d'un rectangle.
u32 Mouse_RectAdd(struct SMouse *psMouse, u32 nX1, u32 nY1, u32 nX2, u32 nY2, s32 nType, u32 nVar)
{
	if (psMouse->nRectNb >= MOUSE_MAX_RECT) return (1);

	// Ajout du rectangle.
	psMouse->pRect[psMouse->nRectNb].nX1 = nX1;
	psMouse->pRect[psMouse->nRectNb].nY1 = nY1;
	psMouse->pRect[psMouse->nRectNb].nX2 = nX2;
	psMouse->pRect[psMouse->nRectNb].nY2 = nY2;
	psMouse->pRect[psMouse->nRectNb].nWhere = nType;
	psMouse->pRect[psMouse->nRectNb].nVar = nVar;
	psMouse->nRectNb++;

	return (0);
}

// MAJ rectangle de la map.
void Mouse_RectMap(struct SMouse *psMouse, u32 nX1, u32 nY1, u32 nX2, u32 nY2)
{
	psMouse->sMapRect.nX1 = nX1;
	psMouse->sMapRect.nY1 = nY1;
	psMouse->sMapRect.nX2 = nX2;
	psMouse->sMapRect.nY2 = nY2;
	psMouse->sMapRect.nWhere = e_MouseLoc_Map;
	psMouse->sMapRect.nVar = 0;
}

// MAJ rectangle de la visu.
void Mouse_RectVisu(struct SMouse *psMouse, u32 nX1, u32 nY1, u32 nX2, u32 nY2)
{
	psMouse->sVisuRect.nX1 = nX1;
	psMouse->sVisuRect.nY1 = nY1;
	psMouse->sVisuRect.nX2 = nX2;
	psMouse->sVisuRect.nY2 = nY2;
	psMouse->sVisuRect.nWhere = e_MouseLoc_Visu;
	psMouse->sVisuRect.nVar = 0;
}


// Détermine la position de la souris.
void Mouse_LocationGet(struct SMouse *psMouse)
{
	struct SMap	*psMap = gMisc.psMap;
	u32	i;

	psMouse->pWhere = NULL;

	// Test du rect de la map.
	if (psMouse->nPixPosX >= psMouse->sMapRect.nX1 && psMouse->nPixPosX <= psMouse->sMapRect.nX2 &&
		psMouse->nPixPosY >= psMouse->sMapRect.nY1 && psMouse->nPixPosY <= psMouse->sMapRect.nY2)
	{
		psMouse->pWhere = &psMouse->sMapRect;
		psMouse->nBlkPosX = (psMouse->nPixPosX - psMouse->pWhere->nX1) / 16;
		psMouse->nBlkPosY = (psMouse->nPixPosY - psMouse->pWhere->nY1) / 16;

		// Gestion du décalage.
		psMouse->nBlkPosX += psMap->nMapPosX;
		psMouse->nBlkPosY += psMap->nMapPosY;
		// Au delà de la map ? (= Dans le vide).
		if ((u32)psMouse->nBlkPosX >= psMap->nMapLg || (u32)psMouse->nBlkPosY >= psMap->nMapHt)
		{
			psMouse->nBlkPosX = -1;
		}
	}
	else
	{
		// Test des rectangles des planches.
		for (i = 0; i < psMouse->nRectNb; i++)
		{
			if (psMouse->nPixPosX >= psMouse->pRect[i].nX1 && psMouse->nPixPosX <= psMouse->pRect[i].nX2 &&
				psMouse->nPixPosY >= psMouse->pRect[i].nY1 && psMouse->nPixPosY <= psMouse->pRect[i].nY2)
			{
				psMouse->pWhere = &psMouse->pRect[i];
				psMouse->nBlkPosX = (psMouse->nPixPosX - psMouse->pWhere->nX1) / 16;
				psMouse->nBlkPosY = (psMouse->nPixPosY - psMouse->pWhere->nY1) / 16;

				// Gestion du décalage.
				struct SPlane	*pPln = *(psMap->ppsPlanes + psMouse->pWhere->nVar);

				psMouse->nBlkPosX += pPln->nWndPosX;
				psMouse->nBlkPosY += pPln->nWndPosY;
				// Au delà de la planche ? (= Dans le vide).
				if (psMouse->nBlkPosX >= pPln->pGfx->w / 16 || psMouse->nBlkPosY >= pPln->pGfx->h / 16)
				{
					psMouse->nBlkPosX = -1;
				}
				break;
			}
		}
	}

	// Test du rect de la visu (toujours au dessus).
	if (Map_VisuOnOff(psMap))
	if (psMouse->nState != e_MouseState_GrabStarted)		// On ne passe pas sur la visu si un grab est démarré.
	if (psMouse->nPixPosX >= psMouse->sVisuRect.nX1 && psMouse->nPixPosX <= psMouse->sVisuRect.nX2 &&
		psMouse->nPixPosY >= psMouse->sVisuRect.nY1 && psMouse->nPixPosY <= psMouse->sVisuRect.nY2)
	{
		psMouse->pWhere = &psMouse->sVisuRect;
		psMouse->nBlkPosX = -1;
	}

	// Affiche/cache le pointeur de la souris.
	if (psMouse->pWhere == NULL || psMouse->nBlkPosX == -1)
	{
		SDL_ShowCursor(SDL_ENABLE);		// Affiche le pointeur de la souris.
	}
	else
	{
		SDL_ShowCursor(SDL_DISABLE);	// Cache le pointeur de la souris.
	}

}

// Vérifie si on peut déplacer la souris comme demandé.
// Out : O = Ok / b0 = Pas bon en x / b1 = Pas bon en y.
u32	Mouse_LocationCheck(struct SMouse *psMouse, u32 nNewPosX, u32 nNewPosY)
{
	u32	nRet = 0;

	if (psMouse->nState != e_MouseState_GrabStarted) return (0);	// Pas en capture => pas de pb.
	if (psMouse->pWhere == NULL) return (3);	// Ne devrait pas arriver.

	// Test du débordement du rectangle dans lequel se trouve la souris.
	if (nNewPosX < psMouse->pWhere->nX1 || nNewPosX > psMouse->pWhere->nX2) nRet |= 1;
	if (nNewPosY < psMouse->pWhere->nY1 || nNewPosY > psMouse->pWhere->nY2) nRet |= 2;

	// Test du débordement de la page de blocs ou de la map.
	// !!! Ne pas skipper le test si nRet != 0, car ça peut déborder dans un sens au premier test et dans l'autre au second.
	struct SPlane	*pPln;
	struct SMap		*psMap = gMisc.psMap;
	if (psMouse->pWhere->nWhere == e_MouseLoc_Planes)
	{
		// Sur une page de blocs.
		pPln = psMap->ppsPlanes[psMap->nActivePlane];

		if (nNewPosX > psMouse->pWhere->nX1 + (pPln->pGfx->w - (pPln->nWndPosX * 16)) - 1) nRet |= 1;
		if (nNewPosY > psMouse->pWhere->nY1 + (pPln->pGfx->h - (pPln->nWndPosY * 16)) - 1) nRet |= 2;
	}
	else if (psMouse->pWhere->nWhere == e_MouseLoc_Map)
	{
		// Sur la map.
		pPln = psMap->ppsPlanes[psMap->nActivePlane];

		if (nNewPosX > psMouse->pWhere->nX1 + ((psMap->nMapLg - psMap->nMapPosX) * 16) - 1) nRet |= 1;
		if (nNewPosY > psMouse->pWhere->nY1 + ((psMap->nMapHt - psMap->nMapPosY) * 16) - 1) nRet |= 2;
	}

	return (nRet);
}

// Gestion de l'enfoncement du bouton.
void Mouse_ButtonPressed(struct SMouse *psMouse, u32 nButtonNo, u32 nButtonX, u32 nButtonY)
{
	struct SMap	*psMap = gMisc.psMap;

	// Bouton droit et brosse ? On release la brosse.
	if (psMouse->nState == e_MouseState_Brush && nButtonNo == SDL_BUTTON_RIGHT)
	{
		Mouse_BrushRelease(psMouse);
		gMisc.nUpdateDisp |= e_Update_All;
		return;
	}

	if (psMouse->pWhere == NULL) return;

	// Dans les plans ?
	if (psMouse->pWhere->nWhere == e_MouseLoc_Planes)
	{
		// Changement de plan actif ?
		if (psMap->nActivePlane != psMouse->pWhere->nVar)
		{
			Mouse_BrushRelease(psMouse);
			if (Map_PlaneHasCodes(psMap, psMap->nActivePlane)) gMisc.nUpdateDisp |= e_Update_Map;	// Si page précédente a des codes, update de la map.
			psMap->nActivePlane = psMouse->pWhere->nVar;
			gMisc.nUpdateDisp |= e_Update_Planes;
			if (Map_PlaneHasCodes(psMap, psMap->nActivePlane)) gMisc.nUpdateDisp |= e_Update_Map;	// Si nouvelle page a des codes, update de la map.
		}
		else
		{
			// Clic dans le plan actif, on commence une capture.
			Mouse_BrushGrabStart(psMouse);
			gMisc.nUpdateDisp |= e_Update_Planes | e_Update_StatusBar;		// e_Update_Planes pour clearer l'info box des monstres au cas ou.
		}
	}
	// Sur la map ?
	else if (psMouse->pWhere->nWhere == e_MouseLoc_Map)
	{
		// La brosse est vide ?
		if (psMouse->nState == e_MouseState_Idle)
		{
			// On commence une capture.
			Mouse_BrushGrabStart(psMouse);
			gMisc.nUpdateDisp |= 2|4;	// 2 pour clearer l'info box des monstres au cas ou.
		}
		else
		// On a une brosse ?
		if (psMouse->nState == e_MouseState_Brush)
		{
			// Sur la map, poke.
			Mouse_BrushPoke(psMouse);
			gMisc.nUpdateDisp |= 2;

			psMouse->nTile = 1;		// Tiling commencé.
			psMouse->nTileOrgX = psMouse->nBlkPosX;	// Origine X, Y.
			psMouse->nTileOrgY = psMouse->nBlkPosY;

		}
	}
	// Sur la visu ?
	else if (psMouse->pWhere->nWhere == e_MouseLoc_Visu)
	{
		psMouse->nVisuOffsX = psMap->psVisu->nWndPosX - psMouse->nPixPosX;
		psMouse->nVisuOffsY = psMap->psVisu->nWndPosY - psMouse->nPixPosY;
		psMouse->nVisuDrag = 1;
	}

}

// Gestion du relachement du bouton.
void Mouse_ButtonReleased(struct SMouse *psMouse, u32 nButtonNo, u32 nButtonX, u32 nButtonY)
{

	// Capture commencée ? => On capture.
	if (psMouse->nState == e_MouseState_GrabStarted)
	{
		Mouse_BrushGrabEnd(psMouse, (nButtonNo == SDL_BUTTON_RIGHT ? 1 : 0));
		gMisc.nUpdateDisp |= 4|3;
	}
	psMouse->nTile = 0;		// RAZ Tiling.
	psMouse->nVisuDrag = 0;	// RAZ le drag de la visu.

}

// Capture d'une brosse, début.
// On note les coordonnées de départ.
void Mouse_BrushGrabStart(struct SMouse *psMouse)
{
	if (psMouse->pBrush != NULL) Mouse_BrushRelease(psMouse);	// Si il y a une brosse, on la libère avant de commencer une nouvelle capture.

	psMouse->nBrushXOrigin = psMouse->nBlkPosX;
	psMouse->nBrushYOrigin = psMouse->nBlkPosY;
	psMouse->nState = e_MouseState_GrabStarted;

}

// Capture d'une brosse, fin.
// nCut = 0 : Grab / 1 : Coupe.
void Mouse_BrushGrabEnd(struct SMouse *psMouse, u32 nCut)
{
	struct SMap	*psMap = gMisc.psMap;
	s32	nX1, nX2, nY1, nY2;
	u32	i, j;

	// Si on pointe dans le vide, rien.
	if (psMouse->nBlkPosX == -1)
	{
		psMouse->nState = e_MouseState_Idle;
		return;
	}

	// Rectangle.
	if (psMouse->nBrushXOrigin <= psMouse->nBlkPosX)
	{
		nX1 = psMouse->nBrushXOrigin;
		nX2 = psMouse->nBlkPosX;
	}
	else
	{
		nX1 = psMouse->nBlkPosX;
		nX2 = psMouse->nBrushXOrigin;
	}
	if (psMouse->nBrushYOrigin <= psMouse->nBlkPosY)
	{
		nY1 = psMouse->nBrushYOrigin;
		nY2 = psMouse->nBlkPosY;
	}
	else
	{
		nY1 = psMouse->nBlkPosY;
		nY2 = psMouse->nBrushYOrigin;
	}

	// Allocation mémoire.
	psMouse->nBrushLg = nX2 - nX1 + 1;
	psMouse->nBrushHt = nY2 - nY1 + 1;
	psMouse->pBrush = (s32 *)malloc(psMouse->nBrushLg * psMouse->nBrushHt * sizeof(s32));
	if (psMouse->pBrush == NULL)
	{
		fprintf(stderr, "Mouse_BrushGrabEnd(): malloc failed (pBrush).\n");
		exit(1);
	}

	// Capture des blocs (table).
	struct SPlane	*pPln;
	if (psMouse->pWhere->nWhere == e_MouseLoc_Planes)
	{
		// Depuis une page de blocs.
		pPln = psMap->ppsPlanes[psMap->nActivePlane];

		for (j = 0; j < psMouse->nBrushHt; j++)
		{
			for (i = 0; i < psMouse->nBrushLg; i++)
			{
				*(psMouse->pBrush + (j * psMouse->nBrushLg) + i) =
					((j + nY1) * (pPln->pGfx->w / 16)) + i + nX1;	// Décalage de la planche inclus dans nX1 et nY1.
			}
		}

		// Des monstres ?
		if (Plane_HasCmd(pPln))
		{
			// malloc.
			if ((psMouse->ppMstBrush = (struct SMstUnit **)malloc(psMouse->nBrushLg * psMouse->nBrushHt * sizeof(struct SMstUnit *))) == NULL)
			{
				fprintf(stderr, "Mouse_BrushGrabEnd(): malloc failed (ppMstBrush (1)).\n");
				exit(1);
			}
			// Initialisation des ptrs + des monstres si nécessaire.
//printf("pln %d : mst grab.\n", (int)pPln->nPlaneNo);
			for (i = 0; i < psMouse->nBrushLg * psMouse->nBrushHt; i++)
			{
				*(psMouse->ppMstBrush + i) = NULL;		// Par défaut, ptr NULL.
//-				if (pPln->_pMst->CmdMstExists(*(psMouse->pBrush + i)) != NULL)
				if (Cmdfile_CmdMstExists(pPln->pMst, *(psMouse->pBrush + i)) != NULL)
				{
//printf("Grab mst %d.\n", (int)*(pBrush + i));
					*(psMouse->ppMstBrush + i) = MstUnit_New();
//-					(*(psMouse->ppMstBrush + i))->InitFromCmdFile(*(psMouse->pBrush + i));
					MstUnit_InitFromCmdFile(*(psMouse->ppMstBrush + i), *(psMouse->pBrush + i));
				}
			}
		}

	}
	else if (psMouse->pWhere->nWhere == e_MouseLoc_Map)
	{
		// Depuis la map.
		pPln = psMap->ppsPlanes[psMap->nActivePlane];

		for (j = 0; j < psMouse->nBrushHt && j + nY1 < psMap->nMapHt; j++)
		{
			for (i = 0; i < psMouse->nBrushLg && i + nX1 < psMap->nMapLg; i++)
			{
				*(psMouse->pBrush + (j * psMouse->nBrushLg) + i) =
					*(pPln->pPlane + ((j + nY1) * psMap->nMapLg) + i + nX1);	// Décalage de la map inclus dans nX1 et nY1.
				if (nCut)	// Cut ?
				{
					*(pPln->pPlane + ((j + nY1) * psMap->nMapLg) + i + nX1) = -1;
				}
			}
		}

		// Des monstres ?
		if (Plane_HasCmd(pPln))
		{
			// malloc.
			if ((psMouse->ppMstBrush = (struct SMstUnit **)malloc(psMouse->nBrushLg * psMouse->nBrushHt * sizeof(struct SMstUnit *))) == NULL)
			{
				fprintf(stderr, "Mouse_BrushGrabEnd(): malloc failed (ppMstBrush (2)).\n");
				exit(1);
			}
			// Initialisation des ptrs.
//printf("map : mst grab.\n");
			for (i = 0; i < psMouse->nBrushLg * psMouse->nBrushHt; i++)
			{
				*(psMouse->ppMstBrush + i) = NULL;		// Par défaut, ptr NULL.
			}
			// Parcours des pointeurs.
			for (j = 0; j < psMouse->nBrushHt && j + nY1 < psMap->nMapHt; j++)
			{
				for (i = 0; i < psMouse->nBrushLg && i + nX1 < psMap->nMapLg; i++)
				{
					if (*(pPln->ppMstPlane + ((j + nY1) * psMap->nMapLg) + i + nX1) != NULL)
					{
//printf("Grab mst %d.\n", (*(pPln->_ppMstPlane + ((j + nY1) * psMap->nMapLg) + i + nX1))->nMstNo);
						*(psMouse->ppMstBrush + (j * psMouse->nBrushLg) + i) = MstUnit_New();
//-						(*(psMouse->ppMstBrush + (j * psMouse->nBrushLg) + i))->InitFromMst(*(pPln->_ppMstPlane + ((j + nY1) * psMap->nMapLg) + i + nX1));
						MstUnit_InitFromMst(*(psMouse->ppMstBrush + (j * psMouse->nBrushLg) + i), *(pPln->ppMstPlane + ((j + nY1) * psMap->nMapLg) + i + nX1));
						if (nCut)	// Cut ?
						{
//-							delete *(pPln->ppMstPlane + ((j + nY1) * psMap->nMapLg) + i + nX1);
							MstUnit_Delete(*(pPln->ppMstPlane + ((j + nY1) * psMap->nMapLg) + i + nX1));
							*(pPln->ppMstPlane + ((j + nY1) * psMap->nMapLg) + i + nX1) = NULL;
						}
					}
				}
			}

		}

		if (nCut) Undo_Save(gMisc.psUndo);	// Sauvegarde quand cut.

	}

	// Et hop !
	psMouse->nState = e_MouseState_Brush;

	// Replacement de la souris à la position haut gauche de capture.
	s32	nWarpOffsX = psMouse->nPixPosX, nWarpOffsY = psMouse->nPixPosY;
	if (psMouse->nBlkPosX > psMouse->nBrushXOrigin) nWarpOffsX -= (psMouse->nBrushLg - 1) * 16;
	if (psMouse->nBlkPosY > psMouse->nBrushYOrigin) nWarpOffsY -= (psMouse->nBrushHt - 1) * 16;
	SDL_WarpMouse(nWarpOffsX, nWarpOffsY);

}

// Libère la brosse.
void Mouse_BrushRelease(struct SMouse *psMouse)
{
	free(psMouse->pBrush);
	psMouse->pBrush = NULL;

	// Libère les monstres.
	if (psMouse->ppMstBrush != NULL)
	{
//printf("CMouse::BrushRelease : mst.\n");
		u32 i;
		for (i = 0; i < psMouse->nBrushLg * psMouse->nBrushHt; i++)
		{
			if (*(psMouse->ppMstBrush + i) != NULL)
			{
				MstUnit_Delete(*(psMouse->ppMstBrush + i));
				*(psMouse->ppMstBrush + i) = NULL;
			}
		}
		free(psMouse->ppMstBrush);
		psMouse->ppMstBrush = NULL;
	}

	psMouse->nState = e_MouseState_Idle;
	psMouse->nTile = 0;		// Tiling.

}

// Poke.
void Mouse_BrushPoke(struct SMouse *psMouse)
{
	if (psMouse->pWhere->nWhere != e_MouseLoc_Map) return;	// Ne devrait pas arriver.

	struct SMap		*psMap = gMisc.psMap;
	struct SPlane	*pPln = psMap->ppsPlanes[psMap->nActivePlane];
	u32	i, j;

	if (Plane_IsHidden(pPln)) return;	// Pas de poke sur un plan caché.

	// Les blocs.
	for (j = 0; j < psMouse->nBrushHt && j + psMouse->nBlkPosY < psMap->nMapHt; j++)
	{
		for (i = 0; i < psMouse->nBrushLg && i + psMouse->nBlkPosX < psMap->nMapLg; i++)
		{
			*(pPln->pPlane + ((j + psMouse->nBlkPosY) * psMap->nMapLg) + i + psMouse->nBlkPosX) =
				*(psMouse->pBrush + (j * psMouse->nBrushLg) + i);
		}
	}

	// Des monstres ?
	if (psMouse->ppMstBrush != NULL && Plane_HasCmd(pPln))
	{

	// Les monstres.
	for (j = 0; j < psMouse->nBrushHt && j + psMouse->nBlkPosY < psMap->nMapHt; j++)
	{
		for (i = 0; i < psMouse->nBrushLg && i + psMouse->nBlkPosX < psMap->nMapLg; i++)
		{
			// S'il y avait un mst, delete.
			if (*(pPln->ppMstPlane + ((j + psMouse->nBlkPosY) * psMap->nMapLg) + i + psMouse->nBlkPosX) != NULL)
			{
//printf("mst brush poke: delete\n");
				MstUnit_Delete(*(pPln->ppMstPlane + ((j + psMouse->nBlkPosY) * psMap->nMapLg) + i + psMouse->nBlkPosX));
				*(pPln->ppMstPlane + ((j + psMouse->nBlkPosY) * psMap->nMapLg) + i + psMouse->nBlkPosX) = NULL;
			}
			// S'il y a un monstre à poker, new + recopie du monstre (parce que si plusieurs pokes, il faut plusieurs monstres !).
			if (*(psMouse->ppMstBrush + (j * psMouse->nBrushLg) + i) != NULL)
			{
//printf("mst brush poke: new\n");
				*(pPln->ppMstPlane + ((j + psMouse->nBlkPosY) * psMap->nMapLg) + i + psMouse->nBlkPosX) = MstUnit_New();
//				(*(pPln->ppMstPlane + ((j + psMouse->nBlkPosY) * psMap->nMapLg) + i + psMouse->nBlkPosX))->InitFromMst(*(psMouse->ppMstBrush + (j * psMouse->nBrushLg) + i));
				MstUnit_InitFromMst(*(pPln->ppMstPlane + ((j + psMouse->nBlkPosY) * psMap->nMapLg) + i + psMouse->nBlkPosX), *(psMouse->ppMstBrush + (j * psMouse->nBrushLg) + i));
			}
		}
	}

	}

	Undo_Save(gMisc.psUndo);	// Sauvegarde de la map après le poke.

}

// Gestion du tiling.
void Mouse_BrushTiling(struct SMouse *psMouse)
{
	if (psMouse->nTile == 0) return;
	if (psMouse->pWhere == NULL) return;
	if (psMouse->pWhere->nWhere != e_MouseLoc_Map) return;
	if (psMouse->nBlkPosX == -1) return;

	// On est sur la map, en mode tiling.
	s32	dx, dy;
	dx = psMouse->nBlkPosX - psMouse->nTileOrgX;
	dy = psMouse->nBlkPosY - psMouse->nTileOrgY;

	if ((ABS(dx) % psMouse->nBrushLg) == 0 &&	// !!! Attention à la macro !!!
		(ABS(dy) % psMouse->nBrushHt) == 0)
	{
		Mouse_BrushPoke(psMouse);
	}

}

// Teste si la souris est sur la map.
u32 Mouse_MouseOnMap(struct SMouse *psMouse)
{
	if (psMouse->pWhere == NULL) return (0);
	if (psMouse->pWhere->nWhere != e_MouseLoc_Map) return (0);
	if (psMouse->nBlkPosX == -1) return (0);
	return (1);
}

// Dessin de la brosse.
void Mouse_BrushDraw(struct SMouse *psMouse)
{
	s32	i, j;
	s32	nX1, nY1, nX2, nY2;
	s32 nLg, nHt;
	u32	nMotif = 0xFFFFFFFF;	// Par défaut, ligne pleine.

	if (psMouse->pWhere == NULL) return;
	if (psMouse->nBlkPosX == -1) return;

	// On détermine les coordonnées du rectangle.
	if (psMouse->nState == e_MouseState_Idle)
	{
		nX1 = psMouse->nBlkPosX;
		nY1 = psMouse->nBlkPosY;
		nLg = nHt = 1;
	}
	else
	if (psMouse->nState == e_MouseState_GrabStarted)
	{
		if (psMouse->nBrushXOrigin <= psMouse->nBlkPosX)
		{
			nX1 = psMouse->nBrushXOrigin;
			nX2 = psMouse->nBlkPosX;
		}
		else
		{
			nX1 = psMouse->nBlkPosX;
			nX2 = psMouse->nBrushXOrigin;
		}
		if (psMouse->nBrushYOrigin <= psMouse->nBlkPosY)
		{
			nY1 = psMouse->nBrushYOrigin;
			nY2 = psMouse->nBlkPosY;
		}
		else
		{
			nY1 = psMouse->nBlkPosY;
			nY2 = psMouse->nBrushYOrigin;
		}
		nLg = nX2 - nX1 + 1;
		nHt = nY2 - nY1 + 1;

		nMotif = 0xF0F0F0F0;	// En grab : pointillé.
	}
	else
	if (psMouse->nState == e_MouseState_Brush)
	{
		nX1 = psMouse->nBlkPosX;
		nY1 = psMouse->nBlkPosY;
		nLg = psMouse->nBrushLg;
		nHt = psMouse->nBrushHt;

		nMotif = 0xAAAAAAAA;	// En brosse : pointillé fin.
	}
	else return;

	// ...suite...
	struct SMap	*psMap = gMisc.psMap;

	nX1 *= 16;
	nY1 *= 16;
	nX1 += psMouse->pWhere->nX1;
	nY1 += psMouse->pWhere->nY1;

	if (psMouse->pWhere->nWhere == e_MouseLoc_Map)
	{
		// Sur la map.
		nX1 -= psMap->nMapPosX * 16;
		nY1 -= psMap->nMapPosY * 16;
	}
	else if (psMouse->pWhere->nWhere == e_MouseLoc_Planes)
	{
		// Sur les plans
		nX1 -= psMap->ppsPlanes[psMouse->pWhere->nVar]->nWndPosX * 16;
		nY1 -= psMap->ppsPlanes[psMouse->pWhere->nVar]->nWndPosY * 16;
	}
	else return;

	nX2 = nX1 + (nLg * 16) - 1;
	nY2 = nY1 + (nHt * 16) - 1;

	SDL_LockSurface(gMisc.pScreen);

	// Tracé des blocs si brosse capturée et sur la map.
	if (psMouse->nState == e_MouseState_Brush && psMouse->pWhere->nWhere == e_MouseLoc_Map)
	{
		s32	nBlockNo;
		s32	nBX1, nBY1;
		nBY1 = nY1 / 16;
		for (j = 0; j < (s32)psMouse->nBrushHt; j++, nBY1++)
		{
			nBX1 = nX1 / 16;
			for (i = 0; i < (s32)psMouse->nBrushLg; i++, nBX1++)
			{
				nBlockNo = *(psMouse->pBrush + (j * psMouse->nBrushLg) + i);
				if (nBlockNo != -1)
				{
					if (nBX1 * 16 >= (s32)psMouse->pWhere->nX1 && nBX1 * 16 < (s32)psMouse->pWhere->nX2 &&
						nBY1 * 16 >= (s32)psMouse->pWhere->nY1 && nBY1 * 16 < (s32)psMouse->pWhere->nY2)
						Plane_BlockDraw(*(psMap->ppsPlanes + psMap->nActivePlane), nBlockNo, nBX1, nBY1, false);
				}
			}
		}
	}

	// Tracé du fantôme (merdique, à optimiser).
	u16	*pScr = (u16 *)gMisc.pScreen->pixels;
	u16	nClr;

	for (i = nX1; i <= nX2; i++)
	{
		nClr = (nMotif & (1 << (i & 31)) ? 0xFFFF : 0);
		if (i >= (s32)psMouse->pWhere->nX1 && i <= (s32)psMouse->pWhere->nX2)
		{
			if (nY1 >= (s32)psMouse->pWhere->nY1 && nY1 <= (s32)psMouse->pWhere->nY2)
				*(pScr + (nY1 * gMisc.nScrW) + i) = nClr;
			if (nY2 >= (s32)psMouse->pWhere->nY1 && nY2 <= (s32)psMouse->pWhere->nY2)
				*(pScr + (nY2 * gMisc.nScrW) + i) = nClr;
		}
	}
	for (i = nY1; i <= nY2; i++)
	{
		nClr = (nMotif & (1 << (i & 31)) ? 0xFFFF : 0);
		if (i >= (s32)psMouse->pWhere->nY1 && i <= (s32)psMouse->pWhere->nY2)
		{
			if (nX1 >= (s32)psMouse->pWhere->nX1 && nX1 <= (s32)psMouse->pWhere->nX2)
				*(pScr + (i * gMisc.nScrW) + nX1) = nClr;
			if (nX2 >= (s32)psMouse->pWhere->nX1 && nX2 <= (s32)psMouse->pWhere->nX2)
				*(pScr + (i * gMisc.nScrW) + nX2) = nClr;
		}
	}

	SDL_UnlockSurface(gMisc.pScreen);

}

