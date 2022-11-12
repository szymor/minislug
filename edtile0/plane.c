
#include "includes.h"


// Prototypes fonctions "privées".
void Plane_MstPlaneColumnInsert(struct SPlane *psPlane, u32 nCol);
void Plane_MstPlaneColumnDelete(struct SPlane *psPlane, u32 nCol);
void Plane_MstPlaneLineInsert(struct SPlane *psPlane, u32 nLin);
void Plane_MstPlaneLineDelete(struct SPlane *psPlane, u32 nLin);


#ifdef __LINUX__
// stricmp n'existe pas en Linux : C'est strcasecmp à la place, dans strings.h.
int stricmp(char *pStr1, char *pStr2)
{
	return (strcasecmp(pStr1, pStr2));
}
#endif

// Constructeur.
struct SPlane * Plane_New(void)
{
	struct SPlane *psPlane;

	if ((psPlane = (struct SPlane *)malloc(sizeof(struct SPlane))) == NULL)
	{
		fprintf(stderr, "Plane_New(): malloc failed (psPlane).\n");
		exit(1);
	}

	// Init.
	psPlane->pPlane = NULL;		// Le plan.
	psPlane->ppMstPlane = NULL;	// Quand il y a des monstres, il y a un "plan" de pointeurs.
	psPlane->pCodes = NULL;		// Les codes des blocs.
	psPlane->pGfx = NULL;
	psPlane->pAlpha = NULL;
	psPlane->nWndPosX = 0;	// (x,y) de départ dans la fenêtre (en blocs).
	psPlane->nWndPosY = 0;
	psPlane->nFlags = 0;
	psPlane->pMst = NULL;		// Fichier de commande.

	return (psPlane);
}

// Destructeur.
void Plane_Delete(struct SPlane *psPlane, struct SMap *psMap)
{
	u32	i;

	// On libère la mémoire du plan de blocs.
	if (psPlane->pPlane != NULL) free(psPlane->pPlane);
	// Le plan de monstres.
	if (psPlane->ppMstPlane != NULL)
	{
//printf("~CPlane : Plane %d : delete mst.\n", (int)_nPlaneNo);
//-		for (i = 0; i < gMap.nMapLg * gMap.nMapHt; i++)		//<< !!! Attention lors du undo !!!
		for (i = 0; i < psMap->nMapLg * psMap->nMapHt; i++)
		{
			if (*(psPlane->ppMstPlane + i) != NULL)
			{
				MstUnit_Delete(*(psPlane->ppMstPlane + i));
				*(psPlane->ppMstPlane + i) = NULL;
			}
		}
		free(psPlane->ppMstPlane);
	}
	// Les codes des blocs.
	if (psPlane->pCodes != NULL) free(psPlane->pCodes);
	// On libère les surfaces SDL.
	if (psPlane->pGfx != NULL) SDL_FreeSurface(psPlane->pGfx);
	if (psPlane->pAlpha != NULL) SDL_FreeSurface(psPlane->pAlpha);
	// On libère le fichier de commande.
	if (psPlane->pMst != NULL) Cmdfile_Delete(psPlane->pMst);

	free(psPlane);
}

typedef struct SPSDPicture * (*pLoadingFct)(char *);
// Initialisation du plan.
void Plane_PlaneInit(struct SPlane *psPlane, char *pFilename)
{
	struct SMap	*psMap = gMisc.psMap;
	u32	i;

	if (pFilename != NULL)		// Sert au moment du undo.
	{
	pLoadingFct	pfFct = NULL;

	// Nom du fichier graphique.
	strcpy(psPlane->pPlnFilename, pFilename);

	// On regarde l'extension du fichier graphique.
	if (stricmp(pFilename + strlen(pFilename) - 3, "bmp") == 0)
	{
		// Lecture du BMP.
		psPlane->pGfx = SDL_LoadBMP(pFilename);
		if (psPlane->pGfx == NULL) {
			fprintf(stderr, "Plane_PlaneInit(): Couldn't load picture: %s\n", SDL_GetError());
			exit(1);
		}
	}
	else
	if (stricmp(pFilename + strlen(pFilename) - 3, "psd") == 0)
	{
		pfFct = PSDLoad;
	}
	else
	if (stricmp(pFilename + strlen(pFilename) - 3, "xcf") == 0)
	{
		pfFct = XCFLoad;
	}
	else
	{
		fprintf(stderr, "Plane_PlaneInit(): Unrecognised file format: %s\n", pFilename);
	}

	if (pfFct != NULL)
	{
		// Lecture du PSD ou du XCF.
		struct SPSDPicture *pPic = pfFct(pFilename);
		if (pPic == NULL) {
			fprintf(stderr, "Plane_PlaneInit(): Couldn't load picture: %s\n", pFilename);
			exit(1);
		}

		// Création d'une surface pour l'image.
		psPlane->pGfx = SDL_CreateRGBSurface(SDL_SWSURFACE, pPic->nWidth, pPic->nHeight, 8, 0, 0, 0, 0);
		if (psPlane->pGfx == NULL)
		{
			fprintf(stderr, "Plane_PlaneInit(): %s: Couldn't create surface for picture.\n", pFilename);
			free(pPic->pPlanes); free(pPic);
			exit(1);
		}
		// Recopie de la palette.
		SDL_SetColors(psPlane->pGfx, pPic->pColors, 0, 256);
		// Recopie du premier plan.
		SDL_LockSurface(psPlane->pGfx);
		memcpy(psPlane->pGfx->pixels, pPic->pPlanes, pPic->nWidth * pPic->nHeight);
		SDL_UnlockSurface(psPlane->pGfx);

		// S'il y a une couche alpha (on ne prend en compte que la première)...
		if (pPic->nNbPlanes > 1)
		{
			// Création d'une surface pour la couche alpha.
			psPlane->pAlpha = SDL_CreateRGBSurface(SDL_SWSURFACE, pPic->nWidth, pPic->nHeight, 8, 0, 0, 0, 0);
			if (psPlane->pAlpha == NULL)
			{
				fprintf(stderr, "Plane_PlaneInit(): %s: Couldn't create surface for alpha layer.\n", pFilename);
				free(pPic->pPlanes); free(pPic);
				exit(1);
			}
			// Création d'une palette alpha.
			for (i = 0; i < 256; i++)
			{
				pPic->pColors[i].r = i;
				pPic->pColors[i].g = i;
				pPic->pColors[i].b = i;
			}
			// Recopie de la palette alpha.
			SDL_SetColors(psPlane->pAlpha, pPic->pColors, 0, 256);
			// Recopie du premier plan alpha.
			SDL_LockSurface(psPlane->pAlpha);
			memcpy(psPlane->pAlpha->pixels, pPic->pPlanes + (pPic->nWidth * pPic->nHeight), pPic->nWidth * pPic->nHeight);
			SDL_UnlockSurface(psPlane->pAlpha);

		} // if couche alpha.

		// Libère le buffer PSD.
		free(pPic->pPlanes);
		free(pPic);
	}

	// Taille divisible par 16 ?
	if (psPlane->pGfx->w % 16 != 0 || psPlane->pGfx->h % 16 != 0)
	{
		fprintf(stderr, "Plane_PlaneInit(): %s: Wrong dimensions. Should be divisible by 16.\n", pFilename);
		exit(1);
	}

	}

	// Malloc.
	if ((psPlane->pPlane = (s32 *)malloc(psMap->nMapLg * psMap->nMapHt * sizeof(s32))) == NULL)
	{
		fprintf(stderr, "Plane_PlaneInit(): malloc failed (pPlane).\n");
		exit(1);
	}
	Plane_PlaneClear(psPlane);

}

// Initialisation du plan de monstres.
void Plane_MstPlaneInit(struct SPlane *psPlane)
{
	struct SMap	*psMap = gMisc.psMap;

	if ((psPlane->ppMstPlane = (struct SMstUnit **)malloc(psMap->nMapLg * psMap->nMapHt * sizeof(struct SMstUnit *))) == NULL)
	{
		fprintf(stderr, "Plane_MstPlaneInit(): malloc failed (ppMstPlane).\n");
		exit(1);
	}
	Plane_MstPlaneClear(psPlane);
}

// Insertion d'une colonne dans le plan.
void Plane_PlaneColumnInsert(struct SPlane *psPlane, u32 nCol)
{
	struct SMap	*psMap = gMisc.psMap;

	// malloc.
	s32	*pNewPlane;
	if ((pNewPlane = (s32 *)malloc((psMap->nMapLg + 1) * psMap->nMapHt * sizeof(s32))) == NULL)
	{
		fprintf(stderr, "Plane_PlaneColumnInsert(): malloc failed (pNewPlane).\n");
		exit(1);
	}

	// Copie + Décalage des blocs.
	u32	i, j;
//	for (i = 0; i < (psMap->nMapLg + 1) * psMap->nMapHt; i++) *(pNewPlane + i) = -1;
	for (i = 0, j = 0; i < psMap->nMapLg * psMap->nMapHt; i++)
	{
		if ((i % psMap->nMapLg) == nCol)
		{
			*(pNewPlane + j) = -1;
			j++;
		}
		*(pNewPlane + j) = *(psPlane->pPlane + i);
		j++;
	}

	// Switch du ptr.
	free(psPlane->pPlane);
	psPlane->pPlane = pNewPlane;

	if (Plane_HasCmd(psPlane)) Plane_MstPlaneColumnInsert(psPlane, nCol);
}
void Plane_MstPlaneColumnInsert(struct SPlane *psPlane, u32 nCol)
{
	struct SMap	*psMap = gMisc.psMap;

	// malloc.
	struct SMstUnit	**ppNewPlane;
	if ((ppNewPlane = (struct SMstUnit **)malloc((psMap->nMapLg + 1) * psMap->nMapHt * sizeof(struct SMstUnit *))) == NULL)
	{
		fprintf(stderr, "Plane_MstPlaneColumnInsert(): malloc failed (ppNewPlane).\n");
		exit(1);
	}

	// Copie + Décalage des blocs.
	u32	i, j;
	for (i = 0, j = 0; i < psMap->nMapLg * psMap->nMapHt; i++)
	{
		if ((i % psMap->nMapLg) == nCol)
		{
			*(ppNewPlane + j) = NULL;
			j++;
		}
		*(ppNewPlane + j) = *(psPlane->ppMstPlane + i);
		j++;
	}

	// Switch du ptr.
	free(psPlane->ppMstPlane);
	psPlane->ppMstPlane = ppNewPlane;

}

// Suppression d'une colonne dans le plan.
void Plane_PlaneColumnDelete(struct SPlane *psPlane, u32 nCol)
{
	struct SMap	*psMap = gMisc.psMap;

	// malloc.
	s32	*pNewPlane;
	if ((pNewPlane = (s32 *)malloc((psMap->nMapLg - 1) * psMap->nMapHt * sizeof(s32))) == NULL)
	{
		fprintf(stderr, "Plane_PlaneColumnDelete(): malloc failed (pNewPlane).\n");
		exit(1);
	}

	// Copie + Décalage des blocs.
	u32	i, j;
	for (i = 0, j = 0; i < psMap->nMapLg * psMap->nMapHt; i++)
	{
		if ((i % psMap->nMapLg) != nCol)
		{
			*(pNewPlane + j) = *(psPlane->pPlane + i);
			j++;
		}
	}

	// Switch du ptr.
	free(psPlane->pPlane);
	psPlane->pPlane = pNewPlane;

	if (Plane_HasCmd(psPlane)) Plane_MstPlaneColumnDelete(psPlane, nCol);
}
void Plane_MstPlaneColumnDelete(struct SPlane *psPlane, u32 nCol)
{
	struct SMap	*psMap = gMisc.psMap;

	// malloc.
	struct SMstUnit	**ppNewPlane;
	if ((ppNewPlane = (struct SMstUnit **)malloc((psMap->nMapLg - 1) * psMap->nMapHt * sizeof(struct SMstUnit *))) == NULL)
	{
		fprintf(stderr, "Plane_MstPlaneColumnDelete(): malloc failed (ppNewPlane).\n");
		exit(1);
	}

	// Libère les monstres de la colonne.
	u32	i, j;
	for (i = 0; i < psMap->nMapHt; i++)
	{
		if (*(psPlane->ppMstPlane + nCol + (i * psMap->nMapLg)) != NULL)
		{
			MstUnit_Delete(*(psPlane->ppMstPlane + nCol + (i * psMap->nMapLg)));
			*(psPlane->ppMstPlane + nCol + (i * psMap->nMapLg)) = NULL;
		}
	}
	// Copie + Décalage des blocs.
	for (i = 0, j = 0; i < psMap->nMapLg * psMap->nMapHt; i++)
	{
		if ((i % psMap->nMapLg) != nCol)
		{
			*(ppNewPlane + j) = *(psPlane->ppMstPlane + i);
			j++;
		}
	}

	// Switch du ptr.
	free(psPlane->ppMstPlane);
	psPlane->ppMstPlane = ppNewPlane;

}

// Insertion d'une ligne dans le plan.
void Plane_PlaneLineInsert(struct SPlane *psPlane, u32 nLin)
{
	struct SMap	*psMap = gMisc.psMap;

	// malloc.
	s32	*pNewPlane;
	if ((pNewPlane = (s32 *)malloc(psMap->nMapLg * (psMap->nMapHt + 1) * sizeof(s32))) == NULL)
	{
		fprintf(stderr, "Plane_PlaneLineInsert(): malloc failed (pNewPlane).\n");
		exit(1);
	}

	// Copie + Décalage des blocs.
	u32	i, j, k;
//	for (i = 0; i < psMap->nMapLg * (psMap->nMapHt + 1); i++) *(pNewPlane + i) = -1;
	for (i = 0, j = 0; i < psMap->nMapLg * psMap->nMapHt; i++)
	{
		if ((i / psMap->nMapLg) == nLin && (i % psMap->nMapLg) == 0)
		{
			for (k = 0; k < psMap->nMapLg; k++)
			{
				*(pNewPlane + j) = -1;
				j++;
			}
		}
		*(pNewPlane + j) = *(psPlane->pPlane + i);
		j++;
	}

	// Switch du ptr.
	free(psPlane->pPlane);
	psPlane->pPlane = pNewPlane;

	if (Plane_HasCmd(psPlane)) Plane_MstPlaneLineInsert(psPlane, nLin);
}
void Plane_MstPlaneLineInsert(struct SPlane *psPlane, u32 nLin)
{
	struct SMap	*psMap = gMisc.psMap;

	// malloc.
	struct SMstUnit	**ppNewPlane;
	if ((ppNewPlane = (struct SMstUnit **)malloc(psMap->nMapLg * (psMap->nMapHt + 1) * sizeof(struct SMstUnit *))) == NULL)
	{
		fprintf(stderr, "Plane_MstPlaneLineInsert(): malloc failed (ppNewPlane).\n");
		exit(1);
	}

	// Copie + Décalage des blocs.
	u32	i, j, k;
	for (i = 0, j = 0; i < psMap->nMapLg * psMap->nMapHt; i++)
	{
		if ((i / psMap->nMapLg) == nLin && (i % psMap->nMapLg) == 0)
		{
			for (k = 0; k < psMap->nMapLg; k++)
			{
				*(ppNewPlane + j) = NULL;
				j++;
			}
		}
		*(ppNewPlane + j) = *(psPlane->ppMstPlane + i);
		j++;
	}

	// Switch du ptr.
	free(psPlane->ppMstPlane);
	psPlane->ppMstPlane = ppNewPlane;

}

// Suppression d'une ligne dans le plan.
void Plane_PlaneLineDelete(struct SPlane *psPlane, u32 nLin)
{
	struct SMap	*psMap = gMisc.psMap;

	// malloc.
	s32	*pNewPlane;
	if ((pNewPlane = (s32 *)malloc(psMap->nMapLg * (psMap->nMapHt - 1) * sizeof(s32))) == NULL)
	{
		fprintf(stderr, "Plane_PlaneLineDelete(): malloc failed (pNewPlane).\n");
		exit(1);
	}

	// Copie + Décalage des blocs.
	u32	i, j;
	for (i = 0, j = 0; i < psMap->nMapLg * psMap->nMapHt; i++)
	{
		if ((i / psMap->nMapLg) != nLin)
		{
			*(pNewPlane + j) = *(psPlane->pPlane + i);
			j++;
		}
	}

	// Switch du ptr.
	free(psPlane->pPlane);
	psPlane->pPlane = pNewPlane;

	if (Plane_HasCmd(psPlane)) Plane_MstPlaneLineDelete(psPlane, nLin);
}
void Plane_MstPlaneLineDelete(struct SPlane *psPlane, u32 nLin)
{
	struct SMap	*psMap = gMisc.psMap;

	// malloc.
	struct SMstUnit	**ppNewPlane;
	if ((ppNewPlane = (struct SMstUnit **)malloc(psMap->nMapLg * (psMap->nMapHt - 1) * sizeof(struct SMstUnit *))) == NULL)
	{
		fprintf(stderr, "Plane_MstPlaneLineDelete(): malloc failed (ppNewPlane).\n");
		exit(1);
	}

	// Libère les monstres de la ligne.
	u32	i, j;
	for (i = 0; i < psMap->nMapLg; i++)
	{
		if (*(psPlane->ppMstPlane + i + (nLin * psMap->nMapLg)) != NULL)
		{
			MstUnit_Delete(*(psPlane->ppMstPlane + i + (nLin * psMap->nMapLg)));
			*(psPlane->ppMstPlane + i + (nLin * psMap->nMapLg)) = NULL;
		}
	}
	// Copie + Décalage des blocs.
	for (i = 0, j = 0; i < psMap->nMapLg * psMap->nMapHt; i++)
	{
		if ((i / psMap->nMapLg) != nLin)
		{
			*(ppNewPlane + j) = *(psPlane->ppMstPlane + i);
			j++;
		}
	}

	// Switch du ptr.
	free(psPlane->ppMstPlane);	// Et surtout on ne libère pas les monstres qui doivent rester !
	psPlane->ppMstPlane = ppNewPlane;

}

// Conversion des planches 8 bits en 16 bits.
void Plane_GfxConvert(struct SPlane *psPlane)
{
	SDL_Surface	*pGfx2;
	pGfx2 = SDL_ConvertSurface(psPlane->pGfx, gMisc.pScreen->format, SDL_SWSURFACE);
	if (pGfx2 == NULL)
	{
		fprintf(stderr, "Plane_GfxConvert(): Failed (pGfx2).\n");
		exit(1);
	}
	SDL_FreeSurface(psPlane->pGfx);	// Libère le gfx 8 bits.
	psPlane->pGfx = pGfx2;			// Place la planche 16 bits à la place.

	// Conversion du plan alpha si nécessaire.
	if (Plane_HasAlpha(psPlane) == 0) return;
	SDL_Surface	*pAlpha2;
	pAlpha2 = SDL_ConvertSurface(psPlane->pAlpha, gMisc.pScreen->format, SDL_SWSURFACE);
	if (pAlpha2 == NULL)
	{
		fprintf(stderr, "Plane_GfxConvert(): Failed (pAlpha2).\n");
		exit(1);
	}
	SDL_FreeSurface(psPlane->pAlpha);	// Libère le gfx 8 bits.
	psPlane->pAlpha = pAlpha2;			// Place la planche 16 bits à la place.

}

// Effacement du plan.
void Plane_PlaneClear(struct SPlane *psPlane)
{
	struct SMap	*psMap = gMisc.psMap;
	u32	i;

	for (i = 0; i < psMap->nMapLg * psMap->nMapHt; i++)
	{
		psPlane->pPlane[i] = -1;
	}

}

// Effacement du plan de monstres.
void Plane_MstPlaneClear(struct SPlane *psPlane)
{
	struct SMap	*psMap = gMisc.psMap;
	u32	i;

	for (i = 0; i < psMap->nMapLg * psMap->nMapHt; i++)
	{
		psPlane->ppMstPlane[i] = NULL;
	}

}

// Affichage du plan dans sa fenêtre.
// In : nOffs = offset en blocs par rapport à la gauche de l'écran.
// Out : Nb de blocs de lg utilisés.
u32 Plane_WndDisplay(struct SPlane *psPlane, u32 nOffs)
{
	struct SMap	*psMap = gMisc.psMap;
	u32	nRem, i, j;
	u32	nBlitH;

	nRem = gMisc.pScreen->w - (nOffs * 16);		// Nb de blocs 16 restant jusqu'à la gauche de l'écran.
//printf("rem = %d\n", (int)nRem);
	nRem >>= 4;
	if (nRem <= 1) return (0);
	nRem--;		// On garde 1 bloc pour la bordure.

	// Offsets.
	nBlitH = (PLN_MAX_BLOCKS_H - 1);
	if (((u32)psPlane->pGfx->h / 16) - psPlane->nWndPosY < nBlitH) nBlitH = (psPlane->pGfx->h / 16) - psPlane->nWndPosY;

//	if (nRem > (u32)psPlane->pGfx->w / 16) nRem = psPlane->pGfx->w / 16;
	if (nRem > ((u32)psPlane->pGfx->w / 16) - psPlane->nWndPosX) nRem = (psPlane->pGfx->w / 16) - psPlane->nWndPosX;

	// Blit de la partie de l'image intéressante.
	SDL_Rect srcrect;
	SDL_Rect dstrect;		// w et h ignorés dans dst !
	dstrect.x = nOffs * 16;
	dstrect.y = 0;
	srcrect.x = psPlane->nWndPosX * 16;
	srcrect.y = psPlane->nWndPosY * 16;
	srcrect.w = (nRem) * 16;
	srcrect.h = nBlitH * 16;

//	// MAJ rectangle souris.
//	gMouse.RectAdd(dstrect.x, dstrect.y, dstrect.x + srcrect.w - 1, dstrect.y + srcrect.h - 1, e_MouseLoc_Planes, 0);	// Rectangle souris.

	SDL_BlitSurface(psPlane->pGfx, &srcrect, gMisc.pScreen, &dstrect);

	// Alpha layer.
	if ((psPlane->nFlags & e_FlgPln_Alpha) && Plane_HasAlpha(psPlane))
	{
		// 1 / Blit en opacité, pour assombrir au niveau du noir.
		SDL_SetAlpha(psPlane->pAlpha, SDL_SRCALPHA, 128);
		SDL_BlitSurface(psPlane->pAlpha, &srcrect, gMisc.pScreen, &dstrect);
		// 2 / Blit en transparence, pour voir le blanc.
		SDL_SetAlpha(psPlane->pAlpha, SDL_SRCALPHA, 255);
		SDL_SetColorKey(psPlane->pAlpha, SDL_SRCCOLORKEY, SDL_MapRGB(gMisc.pScreen->format, 0, 0, 0));
		SDL_BlitSurface(psPlane->pAlpha, &srcrect, gMisc.pScreen, &dstrect);
		SDL_SetColorKey(psPlane->pAlpha, 0, 0);		// Efface la couleur de transparence.
	}

	// Codes.
	if ((psPlane->nFlags & e_FlgPln_Codes) && Plane_HasCodes(psPlane))
	{
		u16	nAltColor = SDL_MapRGB(gMisc.pScreen->format, 0, 255, 255);

		for (j = 0; j < nBlitH; j++)
		for (i = 0; i < nRem; i++)
		{
			Font_StrReset(&gFont);
			Font_StrU8toAHex(&gFont, *(psPlane->pCodes + ((psPlane->nWndPosY + j) * (psPlane->pGfx->w / 16)) + (psPlane->nWndPosX + i) ) );
			Font_Printf(((nOffs + i) * 16) + 1, (j * 16) + 4 + 1, gFont.pString, 0);	// Ombre.
			Font_Printf(((nOffs + i) * 16), (j * 16) + 4 - 1, gFont.pString, 0);	// Ombre haut (pour faire un détourage noir).
			Font_Printf((nOffs + i) * 16, (j * 16) + 4, gFont.pString, ((psPlane->nWndPosX + i + psPlane->nWndPosY + j) & 1 ? 0xFFFF : nAltColor));
		}
	}

	// Complément avec des damiers si planche trop petite.
	// Colonnes.
	if (nRem < PLN_MIN_BLOCKS_DISP_W && (nOffs + nRem + 1) * 16 < (u32)gMisc.pScreen->w)
	{
		SDL_LockSurface(gMisc.pScreen);
		while (nRem < PLN_MIN_BLOCKS_DISP_W && (nOffs + nRem + 1) * 16 < (u32)gMisc.pScreen->w)
		{
			//for (i = 0; i < PLN_MAX_BLOCKS_H - 1; i++)
			for (i = 0; i < nBlitH; i++)
			{
				Gfx_DrawCheckBlock(e_CheckBlk_Bkg, nOffs + nRem, i, false);
			}
			nRem++;
		}
		SDL_UnlockSurface(gMisc.pScreen);
	}
	// Lignes.
	if (nBlitH < PLN_MAX_BLOCKS_H - 1)
	{
		SDL_LockSurface(gMisc.pScreen);
		while (nBlitH < PLN_MAX_BLOCKS_H - 1)
		{
			for (i = 0; i < nRem; i++)
			{
				Gfx_DrawCheckBlock(e_CheckBlk_Bkg, nOffs + i, nBlitH, false);
			}
			nBlitH++;
		}
		SDL_UnlockSurface(gMisc.pScreen);
	}

	// MAJ rectangle souris.
	Mouse_RectAdd(gMisc.psMouse, dstrect.x, dstrect.y, dstrect.x + (nRem * 16) - 1, dstrect.y + (nBlitH * 16) - 1, e_MouseLoc_Planes, psPlane->nPlaneNo);

	// Plan inactif ? => planche grisée.
	if (psPlane->nFlags & e_FlgPln_Hide)
	{
		SDL_LockSurface(gMisc.pScreen);
		u16	*pScr = (u16 *)gMisc.pScreen->pixels;
		for (j = dstrect.y; j <= dstrect.y + (nBlitH * 16) - 1; j++)
		{
			for (i = dstrect.x; i <= dstrect.x + (nRem * 16) - 1; i++)
			{
				if ((i + j) & 1)
				{
					*(pScr + (j * gMisc.nScrW) + i) = 0;
				}
			}
		}
		SDL_UnlockSurface(gMisc.pScreen);
	}

	// Bordure.
	u32	nClr = (psMap->nActivePlane == psPlane->nPlaneNo ? e_CheckBlk_BlueLight : e_CheckBlk_BlueDark);

	SDL_LockSurface(gMisc.pScreen);
	// Barre verticale à droite.
	for (i = 0; i < PLN_MAX_BLOCKS_H - 1; i++)
	{
		Gfx_DrawCheckBlock(nClr, nOffs + nRem, i, false);
	}
	// Barre Horizontale en dessous.
	for (i = 0; i < nRem + 1; i++)
	{
		Gfx_DrawCheckBlock(nClr, nOffs + i, PLN_MAX_BLOCKS_H - 1, false);
	}
	SDL_UnlockSurface(gMisc.pScreen);

	// Affichage du nom de la planche.
	Font_StrReset(&gFont);
	Font_StrItoa(&gFont, psPlane->nPlaneNo);
	Font_StrAdd(&gFont, ":");
	Font_StrAdd(&gFont, psPlane->pPlnFilename);
	// + le nom du fichier de commandes si il y en a un.
	if (Plane_HasCmd(psPlane))
	{
		Font_StrAdd(&gFont, " / ");
		Font_StrAdd(&gFont, psPlane->pMst->pCmdFilename);
	}
	// + (AC) pour l'alpha et/ou les codes.
	u16	nAltColor = SDL_MapRGB(gMisc.pScreen->format, 255, 100, 100);
	u32	nOffsA, nOffsC;
	nOffsA = nOffsC = 0;
	if (Plane_HasAlpha(psPlane) || Plane_HasCodes(psPlane))
	{
		Font_StrAdd(&gFont, " (");
		if (Plane_HasAlpha(psPlane)) { nOffsA = strlen(gFont.pString); Font_StrAdd(&gFont, "A"); }
		if (Plane_HasCodes(psPlane)) { nOffsC = strlen(gFont.pString); Font_StrAdd(&gFont, "C"); }
		Font_StrAdd(&gFont, ")");
	}
	Font_Printf(nOffs * 16, ((PLN_MAX_BLOCKS_H - 1) * 16) + 4, gFont.pString, 0xFFFF);//pPlnFilename);
	// ...avec A et C en rouge si flag actif.
	if (nOffsA && (psPlane->nFlags & e_FlgPln_Alpha))
		Font_Printf((nOffs * 16) + (nOffsA * 8), ((PLN_MAX_BLOCKS_H - 1) * 16) + 4, "A", nAltColor);
	if (nOffsC && (psPlane->nFlags & e_FlgPln_Codes))
		Font_Printf((nOffs * 16) + (nOffsC * 8), ((PLN_MAX_BLOCKS_H - 1) * 16) + 4, "C", nAltColor);

	return (nRem + 1);
}

// Affichage la map du plan concerné.
void Plane_MapDisplay(struct SPlane *psPlane)
{
	struct SMap	*psMap = gMisc.psMap;
	u32	ix, iy, nMapX, nMapY;
	s32	nBlockNo;

	if (psPlane->nFlags & e_FlgPln_Hide) return;	// Si plan caché...

	u32	nStartY = (Misc_FullScreenGet(&gMisc) ? 0 : PLN_MAX_BLOCKS_H);

	SDL_LockSurface(gMisc.pScreen);
	for (iy = nStartY; iy < (gMisc.nScrH / 16) - 1; iy++)
	{
		for (ix = 0; ix < (gMisc.nScrW / 16); ix++)
		{
			nMapX = ix + psMap->nMapPosX;
			nMapY = iy - nStartY + psMap->nMapPosY;
			if (nMapX < psMap->nMapLg && nMapY < psMap->nMapHt)
			{
				nBlockNo = *(psPlane->pPlane + (nMapY * psMap->nMapLg) + nMapX);
				if (nBlockNo != -1)
				{
//	printf("x = %d / y = %d / blk = %d\n", (int)nMapX, (int)nMapY, (int) nBlockNo);
					Plane_BlockDraw(psPlane, nBlockNo, ix, iy, false);
				}
			}
		}
	}
	SDL_UnlockSurface(gMisc.pScreen);

}

// Affichage d'un bloc du plan sur la map.
// In : nPosX, nPosY en blocs.
void Plane_BlockDraw(struct SPlane *psPlane, u32 nBlockNo, u32 nPosX, u32 nPosY, bool bLock)
{
	Plane_BlockDrawPix(psPlane, nBlockNo, nPosX * 16, nPosY * 16, bLock);
}

// Affichage d'un bloc, coordonnées en pixels.
void Plane_BlockDrawPix(struct SPlane *psPlane, u32 nBlockNo, u32 nPosX, u32 nPosY, bool bLock)
{
	struct SMap	*psMap = gMisc.psMap;
	u32	nBlX, nBlY;

	// Petit contrôle...
	if (nBlockNo >= (u32)((psPlane->pGfx->w / 16) * (psPlane->pGfx->h / 16)))
	{
		printf("Plane_BlockDrawPix(): Plane %d: Block #%d non existent!\n", (int)psPlane->nPlaneNo, (int)nBlockNo);
		return;
	}
	if (nPosX + 15 >= (u32)gMisc.pScreen->w) return;	// Pour ne pas tracer en dehors de l'écran.
	if (nPosY + 15 >= (u32)gMisc.pScreen->h) return;

//	nBlX = nBlockNo % (psPlane->pGfx->w / 16);
	nBlY = nBlockNo / (psPlane->pGfx->w / 16);
	nBlX = nBlockNo - (nBlY * (psPlane->pGfx->w / 16));

	// Affichage.
	if (bLock) SDL_LockSurface(gMisc.pScreen);

	u16	*pDst = (u16 *)gMisc.pScreen->pixels;
	u16	*pSrc = ((u16 *)psPlane->pGfx->pixels + (nBlY * 16 * psPlane->pGfx->w) + (nBlX * 16));

	u32	i, j;
	if ((gMisc.nFlags & e_Flg_Transparence) && psPlane->nPlaneNo && Plane_HasCmd(psPlane) == 0)	// Pas de transparence sur le plan 0 ni sur les monstres !
	{
		// Affichage avec de la transparence. (2 boucles (transparence et affichage classique) pour éviter le 'if' si pas de transparence).
		u16	nClrTrans = SDL_MapRGB(gMisc.pScreen->format, gMisc.nTransR, gMisc.nTransG, gMisc.nTransB);
		for (j = 0; j < 16; j++)
		{
			for (i = 0; i < 16; i++)
			{
				if (*(pSrc + (j * psPlane->pGfx->w) + i) != nClrTrans)
				{
					*(pDst + ((nPosY + j) * gMisc.nScrW) + nPosX + i) =
						*(pSrc + (j * psPlane->pGfx->w) + i);
				}
			}
		}
	}
	else
	{
		// Affichage classique.
		for (j = 0; j < 16; j++)
		{
			for (i = 0; i < 16; i++)
			{
				*(pDst + ((nPosY + j) * gMisc.nScrW) + nPosX + i) =
					*(pSrc + (j * psPlane->pGfx->w) + i);
			}
		}
	}

	// Superposition de l'alpha.
	if ((psPlane->nFlags & e_FlgPln_Alpha) && Plane_HasAlpha(psPlane))
	{
		u16	*pAlpha = ((u16 *)psPlane->pAlpha->pixels + (nBlY * 16 * psPlane->pGfx->w) + (nBlX * 16));
		u32	nShadowsAndMask =
			((gMisc.pScreen->format->Rmask >> 1) & gMisc.pScreen->format->Rmask) |
			((gMisc.pScreen->format->Gmask >> 1) & gMisc.pScreen->format->Gmask) |
			((gMisc.pScreen->format->Bmask >> 1) & gMisc.pScreen->format->Bmask);

		// Affichage classique.
		for (j = 0; j < 16; j++)
		{
			for (i = 0; i < 16; i++)
			{
				*(pDst + ((nPosY + j) * gMisc.nScrW) + nPosX + i) =
					((*(pDst + ((nPosY + j) * gMisc.nScrW) + nPosX + i) >> 1) & nShadowsAndMask) |
					*(pAlpha + (j * psPlane->pGfx->w) + i);
			}
		}
	}

	if (bLock) SDL_UnlockSurface(gMisc.pScreen);

	// Codes (Uniquement sur le plan actif) (Cette routine ne concerne que la map !).
	if ((psPlane->nFlags & e_FlgPln_Codes) && Plane_HasCodes(psPlane) && psMap->nActivePlane == psPlane->nPlaneNo)
	{
		u16	nAltColor = SDL_MapRGB(gMisc.pScreen->format, 0, 255, 255);

		Font_StrReset(&gFont);
		Font_StrU8toAHex(&gFont, *(psPlane->pCodes + nBlockNo) );
		Font_Printf(nPosX + 1, nPosY + 4 + 1, gFont.pString, 0);	// Ombre.
		Font_Printf(nPosX, nPosY + 4 - 1, gFont.pString, 0);	// Ombre haut (pour faire un détourage noir).
		Font_Printf(nPosX, nPosY + 4, gFont.pString, ((nPosX + nPosY) & 16 ? 0xFFFF : nAltColor));
	}

}

// Déplacement de la planche dans sa fenêtre.
void Plane_WndMoveXY(struct SPlane *psPlane, s32 nSensX, s32 nSensY)
{
	if (nSensX)
	{
		if (nSensX < 0)
		{
			if (psPlane->nWndPosX) psPlane->nWndPosX--;
		}
		else
		{
			if (psPlane->nWndPosX < ((u32)psPlane->pGfx->w / 16) - 1) psPlane->nWndPosX++;
		}
	}

	if (nSensY)
	{
		if (nSensY < 0)
		{
			if (psPlane->nWndPosY) psPlane->nWndPosY--;
		}
		else
		{
			if (psPlane->nWndPosY < ((u32)psPlane->pGfx->h / 16) - 1) psPlane->nWndPosY++;
		}
	}

}

// Ajoute un fichier de commandes.
u32 Plane_CmdfileAdd(struct SPlane *psPlane, char *pCmdFilename)
{
	if (Plane_HasCmd(psPlane)) return (0);

	psPlane->pMst = Cmdfile_New();
	Cmdfile_CmdfileLoad(psPlane->pMst, pCmdFilename);

	Plane_MstPlaneInit(psPlane);

	return (1);
}

// Ajoute un fichier de commandes, depuis un buffer.
u32 Plane_CmdfileAddFromBuf(struct SPlane *psPlane, char *pCmdFilename, char *pCmdBuf, u32 nCmdDate)
{
	if (Plane_HasCmd(psPlane)) return (0);

	psPlane->pMst = Cmdfile_New();
	Cmdfile_CmdfileLoadFromBuf(psPlane->pMst, pCmdFilename, pCmdBuf, nCmdDate);

	Plane_MstPlaneInit(psPlane);

	return (1);
}

// Compare le fichier de commandes chargé avec celui sur disque.
void Plane_CmdFileCompare(struct SPlane *psPlane)
{
	if (Plane_HasCmd(psPlane) == 0) return;

	struct stat sStat;
	u32	nExtFileDate;
	if (stat(psPlane->pMst->pCmdFilename, &sStat) == -1)
	{
		// Le fichier de commandes n'existe plus. On le recréé.
printf("export cmd\n");
		Cmdfile_CmdFileExport(psPlane->pMst);
	}
	else
	{
		// Récupère la date la plus récente (créé/modifié) du fichier sur disque.
		nExtFileDate = sStat.st_ctime;
		if ((u32)sStat.st_mtime > nExtFileDate) nExtFileDate = sStat.st_mtime;
printf("mem date=%d / ext date=%d\n", (int)psPlane->pMst->nFileDate, (int)nExtFileDate);

		// Le fichier sur disque est plus récent ?
		if (nExtFileDate > psPlane->pMst->nFileDate)
		{
			// Lecture du nouveau fichier dans un nouvel objet.
			printf("Loading new command file.\n");

			struct SCmdfile	*pNewCmdFile;
			pNewCmdFile = Cmdfile_New();
			Cmdfile_CmdfileLoad(pNewCmdFile, psPlane->pMst->pCmdFilename);

			// Parcours des blocs du plan (pas du plan de monstres).
			struct SMap	*psMap = gMisc.psMap;
			u32	ix, iy;
			s32	nBlockNo;
			u32	nNbRemap, nNbDelete, nNbCreate;
			nNbRemap = nNbDelete = nNbCreate = 0;
			for (iy = 0; iy < psMap->nMapHt; iy++)
			{
				for (ix = 0; ix < psMap->nMapLg; ix++)
				{
					// Un bloc ?
					nBlockNo = *(psPlane->pPlane + (iy * psMap->nMapLg) + ix);
					if (nBlockNo != -1)
					{
						printf("Bloc found: no=%d x=%d y=%d\n", (int)nBlockNo, (int)ix, (int)iy);

						// Reprendre les 2 mst correspondant dans chaque fichier.
						// Si les 2 sont la, remap. Note : Les paramètres sont pris PAR ORDRE ! Pas par nom.
						// Si slt ancien => delete du monstre existant.
						// Si slt nouveau => creation d'un monstre.

						// Récupération du texte des du monstre dans les deux fichiers.
						char	pOldBuf[1024], pNewBuf[1024];	// Le texte du monstre affiché/édité.
						u32	nOld, nNew;
						struct SMstUnit	*pNewMst = NULL;
						struct SMstUnit	*pOldMst = NULL;

//-						nOld = _pMst->CmdMstSeek(nBlockNo, pOldBuf);
//-						nNew = pNewCmdFile->CmdMstSeek(nBlockNo, pNewBuf);
						nOld = Cmdfile_CmdMstSeek(psPlane->pMst, nBlockNo, pOldBuf);
						nNew = Cmdfile_CmdMstSeek(pNewCmdFile, nBlockNo, pNewBuf);

						if (nOld && nNew)
						{
printf("Remap\n");
							// Il y a un record correspondant dans les 2 fichiers. Remap.
							pOldMst = *(psPlane->ppMstPlane + (iy * psMap->nMapLg) + ix);
							if (pOldMst != NULL)
							{
								// Nouveau monstre.
								pNewMst = MstUnit_New();
//								pNewMst->InitFromMstTxtBuf(pNewBuf, nBlockNo);
								MstUnit_InitFromMstTxtBuf(pNewMst, pNewBuf, nBlockNo);
								// Nb de paramètres de l'ancien monstre.
//-								u32	nNbPrm = _pMst->CmdMstGetPrmNb(pOldBuf);
								u32	nNbPrm = CmdMstGetPrmNb(pOldBuf);
								u32	p;
								for (p = 0; p < nNbPrm; p++)
								{
									char	pOldPrm[1024], pNewPrm[1024];
									u32	nOldPrmBitStart, nOldPrmBitEnd, nNewPrmBitStart, nNewPrmBitEnd, nOldPrmSign, nNewPrmSign;
									char	pOldPrmName[1024], pNewPrmName[1024];
									char	pOldPrmArgs[1024], pNewPrmArgs[1024];
									u32	nSplit;
									bool	bRemap;

									bRemap = false;
//-									if (_pMst->CmdMstGetPrmNo(pOldBuf, p, pOldPrm))
									if (CmdMstGetPrmNo(pOldBuf, p, pOldPrm))
									{
//-										nSplit = _pMst->CmdPrmSplit(pOldPrm, pOldPrmName, &nOldPrmBitStart, &nOldPrmBitEnd, &nOldPrmSign, pOldPrmArgs);
										nSplit = CmdPrmSplit(pOldPrm, pOldPrmName, &nOldPrmBitStart, &nOldPrmBitEnd, &nOldPrmSign, pOldPrmArgs);
										if (nSplit >= CMD_PRM_SPLIT_MIN)
										{
//-											if (pNewCmdFile->CmdMstGetPrmNo(pNewBuf, p, pNewPrm))
											if (CmdMstGetPrmNo(pNewBuf, p, pNewPrm))
											{
//-												nSplit = pNewCmdFile->CmdPrmSplit(pNewPrm, pNewPrmName, &nNewPrmBitStart, &nNewPrmBitEnd, &nNewPrmSign, pNewPrmArgs);
												nSplit = CmdPrmSplit(pNewPrm, pNewPrmName, &nNewPrmBitStart, &nNewPrmBitEnd, &nNewPrmSign, pNewPrmArgs);
												if (nSplit >= CMD_PRM_SPLIT_MIN)
												{
													s32	nVal;
													nVal = GetBits(pOldMst->pData, nOldPrmBitStart, nOldPrmBitEnd, nOldPrmSign);
													//pNewMst->SetBits(nOldPrmBitStart, nOldPrmBitEnd, nVal);	// OldStart et OldEnd ?? Normalement ça devrait être avec NewStart et NewEnd.
													SetBits(pNewMst->pData, nNewPrmBitStart, nNewPrmBitEnd, nVal);
													bRemap = true;
												}
											}
										}
									}

									if (bRemap == false)
									{
										printf("Remap monster: Shit happened with the following parameter:\nOld : %s\nNew : %s\n", pOldPrm, pNewPrm);
									}

								}	// for p

								// Hop, on remplace l'ancien monstre par le nouveau remappé.
								*(psPlane->ppMstPlane + (iy * psMap->nMapLg) + ix) = pNewMst;
								MstUnit_Delete(pOldMst);

								nNbRemap++;

							}	// if pOld != NULL

						}
						else
						if (nOld)
						{
							// Il n'y a de record que dans le vieux fichier. Effacement du monstre.
printf("Delete\n");
							if (*(psPlane->ppMstPlane + (iy * psMap->nMapLg) + ix) != NULL)
							{
								MstUnit_Delete(*(psPlane->ppMstPlane + (iy * psMap->nMapLg) + ix));
								*(psPlane->ppMstPlane + (iy * psMap->nMapLg) + ix) = NULL;
								nNbDelete++;
							}
						}
						else
						if (nNew)
						{
							// Il n'y a de record que dans le nouveau fichier. Creation d'un nouveau monstre.
printf("Create\n");
							pNewMst = MstUnit_New();
//							pNewMst->InitFromMstTxtBuf(pNewBuf, nBlockNo);
							MstUnit_InitFromMstTxtBuf(pNewMst, pNewBuf, nBlockNo);
							*(psPlane->ppMstPlane + (iy * psMap->nMapLg) + ix) = pNewMst;
							nNbCreate++;
						}

					}	// if nBlock != -1
				}	// for x
			}	// for y
			printf("Monsters remapped : %d / deleted : %d / created : %d.\n", (int)nNbRemap, (int)nNbDelete, (int)nNbCreate);

			// On remplace l'ancien fichier de commandes par le nouveau.
			Cmdfile_Delete(psPlane->pMst);
			psPlane->pMst = pNewCmdFile;

		}

	}

}


// Ajoute un 'plan' de codes.
void Plane_CodesPlaneAdd(struct SPlane *psPlane, u32 nBlkLg, u32 nBlkHt)
{
	if (psPlane->pCodes != NULL) return;

	// malloc.
	if ((psPlane->pCodes = (u8 *)malloc(nBlkLg * nBlkHt)) == NULL)
	{
		fprintf(stderr, "Plane_CodesPlaneAdd(): malloc failed (pCodes).\n");
		exit(1);
	}
	// RAZ mémoire.
	memset(psPlane->pCodes, 0, nBlkLg * nBlkHt);
}
void Plane_CodesPlaneAddDefaultSz(struct SPlane *psPlane)
{
	Plane_CodesPlaneAdd(psPlane, psPlane->pGfx->w / 16, psPlane->pGfx->h / 16);
}


// Retire le 'plan' de codes.
void Plane_CodesPlaneRemove(struct SPlane *psPlane)
{
	if (psPlane->pCodes == NULL) return;
	free(psPlane->pCodes);
	psPlane->pCodes = NULL;

}

// Changement de valeur d'un code.
void Plane_CodeAdd(struct SPlane *psPlane, u32 nBlockNo, s8 nInc)
{
	if (psPlane->pCodes == NULL) return;
	if (nBlockNo >= (u32)((psPlane->pGfx->w / 16) * (psPlane->pGfx->h / 16)))
	{
		printf("Plane_CodeAdd(): Plane %d: Block #%d non existent!\n", (int)psPlane->nPlaneNo, (int)nBlockNo);
		return;
	}
	*(psPlane->pCodes + nBlockNo) += nInc;

}

// Force la valeur d'un code.
void Plane_CodeSet(struct SPlane *psPlane, u32 nBlockNo, u8 nVal)
{
	if (psPlane->pCodes == NULL) return;
	if (nBlockNo >= (u32)((psPlane->pGfx->w / 16) * (psPlane->pGfx->h / 16)))
	{
		printf("Plane_CodeSet(): Plane %d: Block #%d non existent!\n", (int)psPlane->nPlaneNo, (int)nBlockNo);
		return;
	}
	*(psPlane->pCodes + nBlockNo) = nVal;

}

//

void Plane_Toggle(struct SPlane *psPlane)
{
	psPlane->nFlags ^= e_FlgPln_Hide;
}

u32 Plane_IsHidden(struct SPlane *psPlane)
{
	return (psPlane->nFlags & e_FlgPln_Hide);
}

void Plane_AlphaToggle(struct SPlane *psPlane)
{
	psPlane->nFlags ^= e_FlgPln_Alpha;
}

u32 Plane_HasAlpha(struct SPlane *psPlane)
{
	return (psPlane->pAlpha == NULL ? 0 : 1);
}

u32 Plane_HasCodes(struct SPlane *psPlane)
{
	return (psPlane->pCodes == NULL ? 0 : 1);
}

void Plane_CodesToggle(struct SPlane *psPlane)
{
	psPlane->nFlags ^= e_FlgPln_Codes;
}

u32 Plane_HasCmd(struct SPlane *psPlane)
{
	return (psPlane->pMst == NULL ? 0 : 1);
}
