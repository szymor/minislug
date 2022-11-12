
#include "includes.h"

//
// Routines de lecture du EDT.
// + Fonctions de récupération de hauteur des blocs, ce genre de trucs.
//

#define	MAP_BLK_HERO	120
#define	MAP_BLK_PATH_GND_MIN	80
#define	MAP_BLK_PATH_GND_MAX	97
#define	MAP_BLK_PATH_AIR_MIN	100
#define	MAP_BLK_PATH_AIR_MAX	117

// Map data.
struct SMap	gMap;
// Mst data.
struct SLoadedMst	gLoadedMst;


//=============================================================================
// Le système de sprites "durs".
// Le truc est prévu uniquement pour des objets dont la base est au sol !
// On utilise un sprite dont les bords ne doivent pas avoir de partie "concave".

#define	HARDSPR_MAX_SLOTS	16
struct SHardSpr
{
	u8	nUsed;
	u8	nFlags;
	s32	nX1, nY1, nX2, nY2;		// En pixels.
	struct SSprite	*pSpr;

};

struct SHardSpr	gpHardSprSlots[HARDSPR_MAX_SLOTS];
u32	gnHardSprLastUsed;


// RAZ moteur.
void HardSpr_Init(void)
{
	u32	i;

	// RAZ de tous les slots.
	for(i = 0; i < HARDSPR_MAX_SLOTS; i++)
	{
		gpHardSprSlots[i].nUsed = 0;
	}
	gnHardSprLastUsed = 0;
}

// Cherche un slot libre.
// Out : N° d'un slot libre. -1 si erreur.
s32 HardSpr_GetSlot(void)
{
	u32	i;

	for (i = gnHardSprLastUsed; i < HARDSPR_MAX_SLOTS; i++)
	{
		if (gpHardSprSlots[i].nUsed == 0)
		{
			gpHardSprSlots[i].nUsed = 1;
			gnHardSprLastUsed = i + 1;		// La recherche suivante commencera au suivant.
			return (i);
		}
	}
	return (-1);
}

// Libère un slot.
void HardSpr_ReleaseSlot(u32 nSlotNo)
{
	// Pour accélérer la recherche des slots libres.
	if (nSlotNo < gnHardSprLastUsed)
	{
		gnHardSprLastUsed = nSlotNo;
	}
	gpHardSprSlots[nSlotNo].nUsed = 0;
}

// Ajoute un sprite à la liste.
s32 HardSpr_AddToList(u32 nSpr, s32 nPosX, s32 nPosY, u32 nFlags)
{
	struct SSprite	*pSpr;
	s32	nSlotNo;

	if ((nSlotNo = HardSpr_GetSlot()) == -1)
	{
		fprintf(stderr, "HardSpr_AddToList(): Warning, out of slots!\n");
		return (-1);
	}

	nSpr &= ~(SPR_Flip_X | SPR_Flip_Y);
	// On récupère le descripteur de sprite.
	if ((pSpr = SprGetDesc(nSpr)) == NULL) { HardSpr_ReleaseSlot(nSlotNo); return (-1);	}
	gpHardSprSlots[nSlotNo].nX1 = nPosX - pSpr->nPtRefX;
	gpHardSprSlots[nSlotNo].nY1 = nPosY - pSpr->nPtRefY;
	gpHardSprSlots[nSlotNo].nX2 = nPosX - pSpr->nPtRefX + pSpr->nLg - 1;
	gpHardSprSlots[nSlotNo].nY2 = nPosY - pSpr->nPtRefY + pSpr->nHt - 1;
	gpHardSprSlots[nSlotNo].pSpr = pSpr;
	gpHardSprSlots[nSlotNo].nFlags = nFlags;

	return (nSlotNo);
}

// Teste si un point est dans un sprite dur ou pas.
// Out : 0 = Pas dans du dur, sinon -y = nb de pixels duquel il faut monter.
s32 HardSpr_TestHt(s32 nPosX, s32 nPosY, u32 nFlags)
{
	u32	i;
	u8	*pGfx;
	u32	nLg;
	s32	nTmpY;

	for(i = 0; i < HARDSPR_MAX_SLOTS; i++)
	if (gpHardSprSlots[i].nUsed)
	if ((gpHardSprSlots[i].nFlags & nFlags) == 0)
	{
		// Dans le rectangle englobant ?
		if (nPosX >= gpHardSprSlots[i].nX1 && nPosX <= gpHardSprSlots[i].nX2 &&
			nPosY >= gpHardSprSlots[i].nY1 && nPosY <= gpHardSprSlots[i].nY2)
		{
			pGfx = gpHardSprSlots[i].pSpr->pGfx8;
			pGfx += (nPosX - gpHardSprSlots[i].nX1);		// Offs X.
			nLg = gpHardSprSlots[i].pSpr->nLg;
			pGfx += (nPosY - gpHardSprSlots[i].nY1) * nLg;	// Offs Y.

			nTmpY = nPosY;
			if (*pGfx)
			{
				// On est dans du dur, on remonte.
				while (nTmpY >= gpHardSprSlots[i].nY1)
				{
					if (*pGfx == 0) break;
					nTmpY--;
					pGfx -= nLg;
				}
//SprDisplay(e_Spr_Tstrct_Cross, nPosX, nTmpY, e_Prio_HUD);	// tst
				return (nTmpY - nPosY);
			}
		}
	}

	return (0);

}

/*
// Teste si un point est dans un sprite dur ou pas.
//>> Avec haut et bas. Ok, mais bas inutile en fait.
void HardSpr_TestHt(s32 nPosX, s32 nPosY)
{
	u32	i;
	u8	*pGfx;
	u32	nLg;
	s32	nTmpY;

	for (i = 0; i < gnHardSprInList; i++)
	{
		// Dans le rectangle englobant ?
		if (nPosX >= gpHardSprSlots[i].nX1 && nPosX <= gpHardSprSlots[i].nX2 &&
			nPosY >= gpHardSprSlots[i].nY1 && nPosY <= gpHardSprSlots[i].nY2)
		{
			pGfx = gpHardSprSlots[i].pSpr->pGfx8;
			pGfx += (nPosX - gpHardSprSlots[i].nX1);	// Offs X.
			nLg = gpHardSprSlots[i].pSpr->nLg;
			pGfx += (nPosY - gpHardSprSlots[i].nY1) * nLg;	// Offs X.

			nTmpY = nPosY;
			if (*pGfx)
			{
				// On est dans du dur, on remonte.
				while (nTmpY >= gpHardSprSlots[i].nY1)
				{
					if (*pGfx == 0) break;
					nTmpY--;
					pGfx -= nLg;
				}
			}
			else
			{
				// Dans le vide, on descend.
				while (nTmpY <= gpHardSprSlots[i].nY2)
				{
					if (*pGfx) break;
					nTmpY++;
					pGfx += nLg;
				}
			}

SprDisplay(e_Spr_Tstrct_Cross, nPosX, nTmpY, e_Prio_HUD);	// tst
Font_Print(10,120,"IN BOX", 0);

		}
	}

}
*/

// Teste si un point est dans un sprite dur ou pas (sans calcul de hauteur).
// Out : 0 = Pas dans du dur, 1 = Dans du dur.
s32 HardSpr_TestIn(s32 nPosX, s32 nPosY, u32 nFlags)
{
	u32	i;
	u8	*pGfx;
	u32	nLg;

	for(i = 0; i < HARDSPR_MAX_SLOTS; i++)
	if (gpHardSprSlots[i].nUsed)
	if ((gpHardSprSlots[i].nFlags & nFlags) == 0)
	{
		// Dans le rectangle englobant ?
		if (nPosX >= gpHardSprSlots[i].nX1 && nPosX <= gpHardSprSlots[i].nX2 &&
			nPosY >= gpHardSprSlots[i].nY1 && nPosY <= gpHardSprSlots[i].nY2)
		{
			pGfx = gpHardSprSlots[i].pSpr->pGfx8;
			pGfx += (nPosX - gpHardSprSlots[i].nX1);		// Offs X.
			nLg = gpHardSprSlots[i].pSpr->nLg;
			pGfx += (nPosY - gpHardSprSlots[i].nY1) * nLg;	// Offs Y.
			if (*pGfx) return (1);
		}
	}
	return (0);
}

//=============================================================================

// Renvoie la hauteur d'un bloc de plafond sur une colonne x.
// Out : 0 => Rien, sinon 1>16.
u32 BlockCeilingGetHeight(u32 nBlockNo, u32 nPosX)
{
	struct SBlockCol	*pCol;
	u32	nHt;

	pCol = gMap.ppColCodes[gMap.nHeroPlane] + nBlockNo;

/*
// Fonctionne, mais revoir pour des niveaux de plateforme. Il faudra la même chose pour le BlockGetHeight() (à l'envers bien sûr) mais surtout revoir la routine SideCheck() dans le déplacement du héros.
	if ((pCol->nCol & 0x0F) == e_BlockCode_Hard)	// Test du plafond, bloc dur => On renvoie 0 si la colonne est vide, 16 dans tous les autres cas.
	{
		nPosX &= 0x0F;
		return ((pCol->nZero & (1 << nPosX)) == 0 ? 0 : 16);
	}
*/

	if ((pCol->nCol & 0x0F) != e_BlockCode_Ceiling) return (0);		// Ne traite que les plafonds.

	nPosX &= 0x0F;
	if ((pCol->nZero & (1 << nPosX)) == 0) return (16 - 0);	// Col vide => Col pleine pour les plafonds !

	nHt = ( pCol->pHt[(nPosX >> 3) & 1] >> ((nPosX & 7) << 2) ) & 0xF;
	return (16 - (nHt + 1));

}

//=============================================================================

// Renvoie la hauteur d'un bloc sur une colonne x.
// Out : 0 => Rien, sinon 1>16.
	// prévoir un code "dur" qui renvoie 16 immédiatement (?).
u32 BlockGetHeight(u32 nBlockNo, u32 nPosX)
{
	struct SBlockCol	*pCol;
	u32	nHt;

//if (nBlockNo >= gMap.nMapLg * gMap.nMapHt) return (0);	// Au cas ou.	// non, ne doit pas arriver en fait.

	pCol = gMap.ppColCodes[gMap.nHeroPlane] + nBlockNo;

	if ((pCol->nCol & 0x0F) == e_BlockCode_Ceiling) return (0);		// Ignore les plafonds.

	if ((pCol->nCol & 0x0F) == 0) return (0);
	nPosX &= 0x0F;
	if ((pCol->nZero & (1 << nPosX)) == 0) return (0);	// Col vide.

	nHt = ( pCol->pHt[(nPosX >> 3) & 1] >> ((nPosX & 7) << 2) ) & 0xF;
	return (nHt + 1);

}

// Fonction pour récupérer la hauteur jusqu'au sol à partir d'un point (x,y).
// Out : Offset. - si dans du dur, + si en l'air.
s32 BlockGetGroundLevel2(s32 nPixPosX, s32 nPixPosY)
{
	s32	*pBlocks;
	u32	nBlockNo;
	u32	nHt;
	s32	nBlkX, nBlkY;
	s32	nTotHt;

	// Out of map ?
	if ((u32)nPixPosX >= gMap.pPlanesLg[gMap.nHeroPlane] * 16 ||
		(u32)nPixPosY >= gMap.pPlanesHt[gMap.nHeroPlane] * 16) return (0);

	pBlocks = gMap.ppPlanesBlocks[gMap.nHeroPlane];	// Blocs.

	nBlkX = nPixPosX >> 4;
	nBlkY = nPixPosY >> 4;

	// Bloc actuel.
	nBlockNo = *(pBlocks + (nBlkY * gMap.nMapLg) + nBlkX);
	nHt = BlockGetHeight(nBlockNo, nPixPosX);
	// Sur du vide ? > On descend.
	if (nHt == 0)
	{
		nTotHt = 16 - (nPixPosY & 0x0F);
		// On descend jusqu'à trouver du dur.
		while (nHt == 0)
		{
			nBlkY++;
			if (nBlkY >= gMap.pPlanesHt[gMap.nHeroPlane]) return (nTotHt);		// Map limit.
			nBlockNo = *(pBlocks + (nBlkY * gMap.nMapLg) + nBlkX);
			nHt = BlockGetHeight(nBlockNo, nPixPosX);
			nTotHt += 16;
		}
		nTotHt -= 16;
		nHt = 16 - nHt;
		nTotHt += nHt;
		return (nTotHt);
	}

	// Offset dans le bloc actuel.
	nTotHt = (16 - nHt) - (nPixPosY & 0x0F);
	if (nHt != 16) return (nTotHt);		// Stop.

	// Dans du dur > On remonte.
	// On monte jusqu'à trouver un bloc pas rempli jusqu'en haut.
	while (nHt == 16)
	{
		nBlkY--;
		if (nBlkY < 0) return (nTotHt);		// Map limit.
		nBlockNo = *(pBlocks + (nBlkY * gMap.nMapLg) + nBlkX);
		nHt = BlockGetHeight(nBlockNo, nPixPosX);
		nTotHt -= 16;
	}
	nHt = 16 - nHt;
	nTotHt += nHt;
	return (nTotHt);

}
// Encapsulation de la fct précédente + test du hard sprite.
s32 BlockGetGroundLevel(s32 nPixPosX, s32 nPixPosY)
{
	s32	nVal;
	nVal = BlockGetGroundLevel2(nPixPosX, nPixPosY);
	nVal += HardSpr_TestHt(nPixPosX, nPixPosY + nVal, 0);
	return (nVal);
}

//=============================================================================

// Recherche un bloc de chemin.
// Out = -1 : Rien, sinon direction [0;15] + stop : 16.
s32 Map_PathGetBlock(s32 nPosX, s32 nPosY, struct SPathBlock *pCurPath, u32 nCurPathNb)
{
/*
	// Recherche séquentielle.
	u32	i;

u32	nNbIter = 0;

//	nPosX >>= 12;
//	nPosY >>= 12;
	for (i = 0; i < nCurPathNb; i++)
	{
nNbIter++;
		if (pCurPath[i].nPosY > nPosY) return (-1);	// Break.
		if (pCurPath[i].nPosY == nPosY)
		{
			if (pCurPath[i].nPosX == nPosX)
			{
printf("Found: Nb iter=%d\n", nNbIter);
				return (pCurPath[i].nBlockNo);
			}
		}
	}
printf("Not found: Nb iter=%d\n", nNbIter);
	return (-1);
*/


	// Recherche dichotomique.
	s32	nStarting, nEnding, nMid, nLength;

	nStarting = 0;
	nEnding = nCurPathNb - 1;

//u32	nNbIter = 0;

//	nPosX >>= 12;
//	nPosY >>= 12;

	while (1)
	{
//nNbIter++;
		if (nStarting > nEnding) return (-1);
 		nLength = nEnding - nStarting;

		// Les bornes se sont rejointes. Il ne reste qu'un élément à tester. C'est le bon ?
		if (nLength == 0)
		{
			if (pCurPath[nStarting].nPosY == nPosY && pCurPath[nStarting].nPosX == nPosX)
			{
//printf("Found: Nb iter=%d\n", nNbIter);
				return (pCurPath[nStarting].nBlockNo);
			}
//printf("Not found: Nb iter=%d\n", nNbIter);
			return (-1);
		}

		nMid = nStarting + (nLength / 2);
		if (nPosY < pCurPath[nMid].nPosY)
		{
			nEnding = nMid - 1;
		}
		else
		if (nPosY > pCurPath[nMid].nPosY)
		{
			nStarting = nMid + 1;
		}
		else
		{
//			return mid

			// Au même Y, choix sur le X.
//			if (pCurPath[nMid].nPosY == nPosY && pCurPath[nMid].nPosX == nPosX)		// le test du Y doit être inutile...
			if (pCurPath[nMid].nPosX == nPosX)
			{
//printf("Found2: Nb iter=%d\n", nNbIter);
				return (pCurPath[nMid].nBlockNo);
			}

			// Optim possible, sur le même Y, rechercher séquentiellement.

			if (nPosX < pCurPath[nMid].nPosX)
			{
				nEnding = nMid - 1;
			}
			else
			//if (nPosX > pCurPath[nMid].nPosX)	// Seul choix restant, "if" inutile.
			{
				nStarting = nMid + 1;
			}
		}
	}

//	return (-1);
/*
*/

}

// Récupération d'un bloc de path au sol.
s32 Map_PathGndGetBlock(s32 nPosX, s32 nPosY)
{
	if (gMap.pPathGnd == NULL) return (-1);
	return (Map_PathGetBlock(nPosX, nPosY, gMap.pPathGnd, gMap.nPathGndNb));
}
// Récupération d'un bloc de path en l'air.
s32 Map_PathAirGetBlock(s32 nPosX, s32 nPosY)
{
	if (gMap.pPathAir == NULL) return (-1);
	return (Map_PathGetBlock(nPosX, nPosY, gMap.pPathAir, gMap.nPathAirNb));
}

//=============================================================================

//#define	DEBUG_INFO	1	// Commenter pour supprimer.

// Calcul d'un Checksum.
u32 ChecksumCalc(u8 *pBuf, u32 nSz)
{
	u32	i;
	u32	nSum = 0;

	for (i = 0; i < nSz / 4; i++)
	{
		nSum ^= *(u32 *)pBuf;
		pBuf += sizeof(u32);
	}
	for (i = 0; i < (nSz & 3); i++)
		nSum += *pBuf++;

	return (nSum);
}

// Lecture du fichier, calcul du checksum et comparaison avec le checksum sauvegardé.
// Out: 1 = Ok / 0 = Failed.
u32 ChecksumVerify(u32 nLevNo)
{
	FILE	*fPt = NULL;
	u8	*pBuf = NULL;

	char	pFilename[256];
	snprintf(pFilename, sizeof(pFilename), "lev%d/lev%d.edt", (int)nLevNo, (int)nLevNo);

	// Ouverture du fichier.
	if ((fPt = fopen(pFilename, "rb")) == NULL)
	{
		fprintf(stderr, "ChecksumVerify(): Error opening file '%s'.\n", pFilename);
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
		fprintf(stderr, "ChecksumVerify(): malloc failed (pBuf).\n");
		goto _err_exit1;
	}
	// Lecture.
	nSz = fread(pBuf, 1, nFileSzToLoad, fPt);
	if (nSz != nFileSzToLoad)
	{
		fprintf(stderr, "ChecksumVerify(): fread error.\n");
		goto _err_exit1;
	}
	fclose(fPt); fPt = NULL;

	// Calcul du Checksum.
	u32	nSum0, nSum1;
	nSum0 = ChecksumCalc(pBuf, nFileSzToLoad - sizeof(u32));
	nSum1 = *(u32 *)(pBuf + nFileSzToLoad - sizeof(u32));
	free(pBuf); pBuf = NULL;

	// Ok ?
	if (nSum0 != nSum1)
	{
		fprintf(stderr, "File '%s' has been altered. Aborted.\n", pFilename);
		exit(1);
	}
#ifdef DEBUG_INFO
printf("File = %s / Checksum: Calculated = %X - Read = %X\n", pFilename, nSum0, nSum1);
#endif
	return (nSum0 == nSum1);

_err_exit1:
	if (fPt != NULL) fclose(fPt);
	if (pBuf != NULL) free(pBuf);
	exit (1);
}

//=============================================================================

// Libère les ressources utilisées par le niveau en cours.
void LevelRelease(void)
{
	u32	i;

	for (i = 0; i < gMap.nPlanesNb; i++)
	{
		SDL_FreeSurface(gMap.ppPlanesGfx[i]);
		free(*(gMap.ppPlanesBlocks + i));
		*(gMap.ppPlanesBlocks + i) = NULL;

		if (*(gMap.ppColCodes + i) != NULL) free(*(gMap.ppColCodes + i));
	}
	gMap.nPlanesNb = 0;

	// Libère les ressources des monstres.
	if (gLoadedMst.pMstData != NULL) free(gLoadedMst.pMstData);
	gLoadedMst.pMstData = NULL;
	if (gLoadedMst.ppMstPtrX != NULL) free(gLoadedMst.ppMstPtrX);	// Les ptrs pointent dans pMstData.
	gLoadedMst.ppMstPtrX = NULL;
	gLoadedMst.ppMstPtrY = NULL;	// Ce ptr pointe dans l'espace de ppMstPtrX.
	if (gLoadedMst.pMstState != NULL) free(gLoadedMst.pMstState);
	gLoadedMst.pMstState = NULL;
	gLoadedMst.nMstNbInList = 0;

	// Clean Path.
	if (gMap.pPath != NULL) free(gMap.pPath);
	gMap.pPath = NULL;
	gMap.pPathGnd = NULL;
	gMap.nPathGndNb = 0;
	gMap.pPathAir = NULL;
	gMap.nPathAirNb = 0;

	// Blocs animés.
	if (gMap.pBlkAnmMem != NULL) free(gMap.pBlkAnmMem);
	gMap.pBlkAnmMem = NULL;
	for (i = 0; i < MAP_PLANES_MAX; i++) gMap.ppBlkAnmPlanes[i] = NULL;		// Seulement des pointeurs, pointant dans gMap.pBlkAnmMem.

}



// La comparaison du qsort pour trier les monstres sur le X (puis sur le Y).
int qscmpMstXY(const void *pEl1, const void *pEl2)
{
	if ( (*(struct SMst0 **)pEl1)->nPosX == (*(struct SMst0 **)pEl2)->nPosX )
		return ((*(struct SMst0 **)pEl1)->nPosY - (*(struct SMst0 **)pEl2)->nPosY);
	else
		return ((*(struct SMst0 **)pEl1)->nPosX - (*(struct SMst0 **)pEl2)->nPosX);
}
/*
//>> Ok, mais inutile, les monstres sont enregistrés de gauche à droite et de haut en bas.
// La comparaison du qsort pour trier les monstres sur le Y (puis sur le X).
int qscmpMstYX(const void *pEl1, const void *pEl2)
{
	if ( (*(struct SMst0 **)pEl1)->nPosY == (*(struct SMst0 **)pEl2)->nPosY )
		return ((*(struct SMst0 **)pEl1)->nPosX - (*(struct SMst0 **)pEl2)->nPosX);
	else
		return ((*(struct SMst0 **)pEl1)->nPosY - (*(struct SMst0 **)pEl2)->nPosY);
}
*/

// Différents types de sections du fichier EDT.
enum
{
	e_FileSection_Map = 0,
	e_FileSection_Plane,
	e_FileSection_Cmd,
	e_FileSection_Misc,
};


// Pragma pack 1 sur les structures à lire/écrire.
#pragma pack(1)

// Header de chaque container.
struct SContainer0
{
	u32	nSz;
	u16	nType;
	u16	nPadding;
};

// Struct pour datas de la map.
struct SMap0
{
	u32	nMapLg, nMapHt, nMapPosX, nMapPosY, nPlanesNb, nFirstWnd, nActivePlane;
};

#define	e_FlgFile_Plane_Codes	(1 << 31)		// Lecture/sauvegarde d'une page de code.
// Struct pour datas d'un plan.
struct SPlane2
{
	u32	nPlaneNo, nFlags, nWndPosX, nWndPosY;
	u32	nBlkLg, nBlkHt;			// Largeur et hauteur du plan en blocs 16. Pour éventuel remap à la relecture.
	u8	nHasCmd;				// Sera surtout utile à la lecture par le jeu, pour skipper le plan et récupérer les monstres dans la section suivante.
	u8	nPad0, nPad1, nPad2;	// Padding.
};

/*
// Struct pour datas misc.
struct SMisc0
{
	u32	nFlags;						// gMisc.nFlags.
	u8	nTransR, nTransG, nTransB;	// RGB de la couleur de transparence.
	u16	nVisuLg, nVisuHt;			// Taille de la visu (en pixels).

	u8	nPad0;	// Padding.
	u32	nBlank0, nBlank1, nBlank2, nBlank3;	// Au cas ou on ait besoin de sauver plus de trucs par la suite.
};
*/
// Struct pour datas misc. (Hum, corrigée et paddée correctement pour pack).
struct SMisc0
{
	u32	nFlags;						// gMisc.nFlags.
	u8	nTransR, nTransG, nTransB;	// RGB de la couleur de transparence.
	u8	nPad0;						// Padding.
	u16	nVisuLg, nVisuHt;			// Taille de la visu (en pixels).

	u32	nBlank0, nBlank1, nBlank2, nBlank3, nChecksum;	// Au cas ou on ait besoin de sauver plus de trucs par la suite.
};

#pragma pack()

// Lecture d'un fichier EdTile.
// In : No du level. On lit le fichier "lev<no>.edt" dans le répertoire "lev<no>".
void LevelLoad(u32 nLevelNo)
{
	FILE	*fPt;
	u8	*pBuf;	// Pour lire les containers.
	u8	*pCur;
	//char	*pBufTxt;
	u32	nSz;//, nDate;
	u32	j, k;
	u32	nType;
	u32	nMapSection = 0;	// Flag, on doit trouver une section map avant de trouver des plans.
	u32	nPlaneNext = 0;		// Pour controler l'ordre de lecture des plans. Doit être dans l'ordre.
	//char	pFilename[256];
	//CPlane	*pPlane;
	struct SMap0	sMap;
	u32	nPlaneSav_BlkLg, nPlaneSav_BlkHt;	// Lg et Ht de la planche sauvée dans le EDT.

	char	pDir[256];	// Pour créer le nom du répertoire, à ajouter avant les noms de fichiers.
	char	pLevFilename[256];	// Nom du fichier, précédé du répertoire.
	char	pFilename[256];		// Noms de fichiers annexes à lire (planches...).


	// RAZ monstres.
	gLoadedMst.nMstNbInList = 0;
	gLoadedMst.pMstData = NULL;
	gLoadedMst.ppMstPtrX = NULL;
	gLoadedMst.ppMstPtrY = NULL;
	gLoadedMst.pMstState = NULL;
	gLoadedMst.nMstRechIdxX = 0;
	gLoadedMst.nMstRechIdxY = 0;


/*
	// Malloc du buffer de lecture.
	if ((pBuf = (u8 *)malloc(1000000)) == NULL)
	{
		fprintf(stderr, "LoadLevel(): malloc failed (pBuf).\n");
		exit(1);
	}
*/

	// Création du nom du répertoire et du nom de fichier.
//	itoa(nLevelNo, pFilename, 10);
/*
	snprintf(pFilename, sizeof(pFilename), "%d", (int)nLevelNo);
	strcpy(pDir, "lev");
	strcat(pDir, pFilename);	// buf itoa
	strcat(pDir, "/");
	strcpy(pLevFilename, pDir);
	strcat(pLevFilename, "lev");
	strcat(pLevFilename, pFilename);	// buf itoa
	strcat(pLevFilename, ".edt");
*/
	snprintf(pDir, sizeof(pDir), "lev%d/", (int)nLevelNo);
	snprintf(pLevFilename, sizeof(pLevFilename), "%slev%d.edt", pDir, (int)nLevelNo);

#ifdef DEBUG_INFO
printf("lev fn: %s\n", pLevFilename);
#endif

	// Ouverture du fichier.
	if ((fPt = fopen(pLevFilename, "rb")) == NULL)
	{
		fprintf(stderr, "LoadLevel(): Error opening file '%s'.\n", pLevFilename);
		exit(1);
	}
	// Récupération de la taille du fichier. (Note : Les containers sont lus un par un, donc on alloue déjà trop, mais c'est pas bien grave).
	s32	nFileSzToLoad;
	fseek(fPt, 0L, SEEK_END);
	nFileSzToLoad = ftell(fPt);
	fseek(fPt, 0L, SEEK_SET);
//printf("fsize = %d\n", nFileSzToLoad);
	// Malloc du buffer de lecture.
	if ((pBuf = (u8 *)malloc(nFileSzToLoad)) == NULL)
	{
		fclose(fPt);
		fprintf(stderr, "LoadLevel(): malloc failed (pBuf).\n");
		exit(1);
	}


	// En-tête.
	fread(pBuf, 1, 5, fPt);
	// Vérif "EDT".
	if (strncmp((char *)pBuf, "EDT", 3) != 0)
	{
		fprintf(stderr, "LoadLevel(): Wrong header.\n");
		exit(1);
	}
	// Vérif version du fichier.
	if (*(u16 *)(pBuf + 3) != 0x0100)
	{
		fprintf(stderr, "LoadLevel(): Unknown file version.\n");
		exit(1);
	}

#ifdef DEBUG_INFO
printf("Header ok.\n");
#endif

	gMap.nPlanesNb = 0;		// RAZ du nombre de plans lus.
	gMap.nHeroPlane = 0;	// RAZ du plan de ref, au cas ou il n'y ait pas de monstres ?

	// Lecture des containers.
	while (1)
	{
		// Lecture du container.
		nSz = fread(pBuf, 1, sizeof(struct SContainer0), fPt);
		if (nSz != sizeof(struct SContainer0)) break;

		nSz = ((struct SContainer0 *)pBuf)->nSz;
		nType = ((struct SContainer0 *)pBuf)->nType;

		// Lecture des données.
		nSz -= sizeof(struct SContainer0);
		fread(pBuf, 1, nSz, fPt);

		switch (nType)
		{
		case e_FileSection_Map :
#ifdef DEBUG_INFO
			printf("Map section\n");
#endif
			if (nMapSection)
			{
				printf("Duplicate map section found! Section ignored.\n");
				break;
			}
//			printf("lg=%d / ht=%d / posx=%d / posy=%d / nbpln=%d / firstwnd=%d / activepln=%d\n",
//			(int)((SMap0 *)pBuf)->nMapLg, (int)((SMap0 *)pBuf)->nMapHt,
//			(int)((SMap0 *)pBuf)->nMapPosX, (int)((SMap0 *)pBuf)->nMapPosY,
//			(int)((SMap0 *)pBuf)->nPlanesNb, (int)((SMap0 *)pBuf)->nFirstWnd, (int)((SMap0 *)pBuf)->nActivePlane);

			gMap.nMapLg = ((struct SMap0 *)pBuf)->nMapLg;
			gMap.nMapHt = ((struct SMap0 *)pBuf)->nMapHt;
			//gMap.nMapPosX = ((SMap0 *)pBuf)->nMapPosX;
			//gMap.nMapPosY = ((SMap0 *)pBuf)->nMapPosY;
			// Pour plus tard.
			sMap.nPlanesNb = ((struct SMap0 *)pBuf)->nPlanesNb;
			//sMap.nFirstWnd = ((SMap0 *)pBuf)->nFirstWnd;
			//sMap.nActivePlane = ((SMap0 *)pBuf)->nActivePlane;

#ifdef DEBUG_INFO
			printf("lg=%d / ht=%d / nb pln=%d\n", (int)gMap.nMapLg, (int)gMap.nMapHt, (int)sMap.nPlanesNb);
#endif

			// Par défaut, position de départ du joueur.
			gMap.nPlayerStartPosX = 5;//gMap.nMapLg / 2;
			gMap.nPlayerStartPosY = 5;//gMap.nMapHt - 2;

			nMapSection = 1;	// Flag.
			break;

		case e_FileSection_Plane :
#ifdef DEBUG_INFO
			printf("Plane section\n");
#endif
			if (nMapSection == 0)
			{
				printf("Plane section found prior to map section! Section ignored.\n");
				break;
			}
//			printf("plnno=%d / flags=%d / wposx=%d / wposy=%d\n",
//			(int)((SPlane2 *)pBuf)->nPlaneNo, (int)((SPlane2 *)pBuf)->nFlags,
//			(int)((SPlane2 *)pBuf)->nWndPosX, (int)((SPlane2 *)pBuf)->nWndPosY);
			if (((struct SPlane2 *)pBuf)->nPlaneNo != nPlaneNext)
			{
				printf("Plane section: Wrong plane number found. Section ignored.\n");
				break;
			}



			// Si c'est un plan de monstres, on recherche les chemins, puis break.
			// On va séparer les chemins de type air et les chemins de type sol.
			if (((struct SPlane2 *)pBuf)->nHasCmd)
			{
#ifdef DEBUG_INFO
				printf("Plane section: Monsters plane: Searching path blocks.\n");
#endif
				//
				pCur = pBuf + sizeof(struct SPlane2);
				j = *pCur;
				pCur++;
				pCur += j;
				// Passe 1 : On compte le nombre de blocs de type 'chemin'.
				gMap.nPathGndNb = 0;
				gMap.nPathAirNb = 0;
				for (j = 0; j < gMap.nMapLg * gMap.nMapHt; j++)
				{
					// Au passage, note la position de départ du héros.
					if (*(((s32 *)pCur) + j) == MAP_BLK_HERO)
					{
						u32	nBlX, nBlY;
						nBlY = j / gMap.nMapLg;
						nBlX = j - (nBlY * gMap.nMapLg);
						gMap.nPlayerStartPosX = nBlX;
						gMap.nPlayerStartPosY = nBlY;
						gMap.nHeroPlane = gMap.nPlanesNb - 1;	// -1 car le plan des monstres est le plan suivant ! Donc gMap.nPlanesNb a déjà été incrémenté.
#ifdef DEBUG_INFO
printf("Hero at (%d, %d), Ref Plane #%d.\n", (int)gMap.nPlayerStartPosX, (int)gMap.nPlayerStartPosY, (int)gMap.nHeroPlane);
#endif
					}
					// Bloc de chemin ?
					if (*(((s32 *)pCur) + j) >= MAP_BLK_PATH_GND_MIN && *(((s32 *)pCur) + j) <= MAP_BLK_PATH_GND_MAX) gMap.nPathGndNb++;
					if (*(((s32 *)pCur) + j) >= MAP_BLK_PATH_AIR_MIN && *(((s32 *)pCur) + j) <= MAP_BLK_PATH_AIR_MAX) gMap.nPathAirNb++;
				}
#ifdef DEBUG_INFO
printf("Path search : gnd = %d / air = %d\n", (int)gMap.nPathGndNb, (int)gMap.nPathAirNb);
#endif
				// Passe 2 : Malloc.
				gMap.pPath = NULL;
				gMap.pPathGnd = NULL;
				gMap.pPathAir = NULL;
				if (gMap.nPathGndNb + gMap.nPathAirNb)
				{
					if ((gMap.pPath = (struct SPathBlock *)malloc((gMap.nPathGndNb + gMap.nPathAirNb) * sizeof(struct SPathBlock))) == NULL)
					{
						fprintf(stderr, "LoadLevel(): Path: malloc failed.\n");
						exit(1);
					}
					if (gMap.nPathGndNb) gMap.pPathGnd = gMap.pPath;
					if (gMap.nPathAirNb) gMap.pPathAir = gMap.pPath + gMap.nPathGndNb;
					//
					u32	kGnd = 0, kAir = 0;
					for (j = 0; j < gMap.nMapLg * gMap.nMapHt; j++)
					{
						u32	nBlX, nBlY;
						struct SPathBlock	*pPathBlk;
						u16	nBlockNo;

						pPathBlk = NULL;
						nBlockNo = (u16)-1;
						if (*(((s32 *)pCur) + j) >= MAP_BLK_PATH_GND_MIN && *(((s32 *)pCur) + j) <= MAP_BLK_PATH_GND_MAX)
						{
							nBlockNo = *(((s32 *)pCur) + j) - MAP_BLK_PATH_GND_MIN;
							pPathBlk = &gMap.pPathGnd[kGnd];
							kGnd++;
#ifdef DEBUG_INFO
printf("Path GND : ");
#endif
						}
						if (*(((s32 *)pCur) + j) >= MAP_BLK_PATH_AIR_MIN && *(((s32 *)pCur) + j) <= MAP_BLK_PATH_AIR_MAX)
						{
							nBlockNo = *(((s32 *)pCur) + j) - MAP_BLK_PATH_AIR_MIN;
							pPathBlk = &gMap.pPathAir[kAir];
							kAir++;
#ifdef DEBUG_INFO
printf("Path Air : ");
#endif
						}
						if (nBlockNo != (u16)-1)
						{
							nBlY = j / gMap.nMapLg;
							nBlX = j - (nBlY * gMap.nMapLg);
							pPathBlk->nPosX = nBlX;
							pPathBlk->nPosY = nBlY;
							pPathBlk->nBlockNo = nBlockNo;
#ifdef DEBUG_INFO
printf("Path: (%d, %d) Blk:%d.\n", (int)pPathBlk->nPosX, (int)pPathBlk->nPosY, (int)pPathBlk->nBlockNo);
#endif
						}

/*
						if ((*(((s32 *)pCur) + j) >= 80 && *(((s32 *)pCur) + j) <= 96) ||
							(*(((s32 *)pCur) + j) >= 100 && *(((s32 *)pCur) + j) <= 116))
						{
							u32	nBlX, nBlY;
							nBlY = j / gMap.nMapLg;
							nBlX = j - (nBlY * gMap.nMapLg);
							//printf("Path: (%d, %d) Blk:%d.\n", (int)nBlX, (int)nBlY, (int)(*(((s32 *)pCur) + j)));
							gMap.pPath[k].nPosX = nBlX;
							gMap.pPath[k].nPosY = nBlY;
							gMap.pPath[k].nBlockNo = *(((s32 *)pCur) + j) - 80;
							printf("Path: (%d, %d) Blk:%d.\n", (int)gMap.pPath[k].nPosX, (int)gMap.pPath[k].nPosY, (int)gMap.pPath[k].nBlockNo);
							k++;
						}
*/
						//pCur += sizeof(s32);

					}
				}
				//
				//printf("Plane section: Monsters planes, skipped.\n");
				nPlaneNext++;
				break;
			}



			// On récupère les datas qui seront utiles plus tard.
			nPlaneSav_BlkLg = ((struct SPlane2 *)pBuf)->nBlkLg;
			nPlaneSav_BlkHt = ((struct SPlane2 *)pBuf)->nBlkHt;

			// Saute la struct SPlane2.
			pCur = pBuf + sizeof(struct SPlane2);

			// Gfx filename.
			/*
			j = *pCur;
			pCur++;
			strncpy(pFilename, (char *)pCur, j);
			pFilename[j] = 0;
			pCur += j;
			printf("fn=%s\n", pFilename);
			*/
			j = *pCur;
			pCur++;
			strcpy(pFilename, pDir);
			strncpy(pFilename + strlen(pDir), (char *)pCur, j);
			pFilename[strlen(pDir) + j] = 0;
			pCur += j;
#ifdef DEBUG_INFO
			printf("file = %s\n", pFilename);
#endif

			// Lecture de la planche de blocs.
/*
// C'était ça qui fonctionnait à l'origine.
			// Lecture du BMP.
			// Lecture du BMP et conversion en 16 bits.
			SDL_Surface	*pGfx2;
			pGfx2 = SDL_LoadBMP(pFilename);
			if (pGfx2 == NULL) {
				fprintf(stderr, "LoadLevel(): Couldn't load picture: %s\n", SDL_GetError());
				exit(1);
			}
			gMap.ppPlanesGfx[gMap.nPlanesNb] = SDL_ConvertSurface(pGfx2, gVar.pScreen->format, SDL_SWSURFACE);
			if (gMap.ppPlanesGfx[gMap.nPlanesNb] == NULL)
			{
				fprintf(stderr, "LoadLevel(): '%s': 16 bits conversion failed.\n", pFilename);
				exit(1);
			}
			SDL_FreeSurface(pGfx2);	// Libère le gfx 8 bits.
*/



			// Lecture de la planche de blocs et conversion en 16 bits.
			SDL_Surface	*pGfx2;
			struct SPSDPicture	*pPlanche = NULL;

			// On regarde l'extension du fichier.
			if (stricmp(pFilename + strlen(pFilename) - 3, "bmp") == 0)
			{
				// BMP.
				pGfx2 = SDL_LoadBMP(pFilename);
				if (pGfx2 == NULL) {
					fprintf(stderr, "LoadLevel(): Couldn't load picture '%s': %s\n", pFilename, SDL_GetError());
					exit(1);
				}
			}
			else
			if (stricmp(pFilename + strlen(pFilename) - 3, "psd") == 0)
			{
				// PSD.
				pPlanche = PSDLoad(pFilename);
				if (pPlanche == NULL)
				{
					fprintf(stderr, "LoadLevel(): Error while loading PSD file '%s'.\n", pFilename);
					exit(1);
				}
				pGfx2 = SDL_CreateRGBSurface(SDL_SWSURFACE, pPlanche->nWidth, pPlanche->nHeight, 8, 0, 0, 0, 0);
				if (pGfx2 == NULL)
				{
					fprintf(stderr, "LoadLevel(): Error creating SDL surface (pGfx2): %s\n", SDL_GetError());
					exit(1);
				}
				// Recopie de la palette.
				SDL_SetColors(pGfx2, pPlanche->pColors, 0, 256);
				// Recopie du premier plan.
				SDL_LockSurface(pGfx2);
//sans pitch				memcpy((u8 *)pGfx2->pixels, pPlanche->pPlanes, pPlanche->nWidth * pPlanche->nHeight);
				u32	k;
				for (k = 0; k < pPlanche->nHeight; k++)		// Avec prise en compte du pitch.
					memcpy((u8 *)pGfx2->pixels + (k * pGfx2->pitch), pPlanche->pPlanes + (k * pPlanche->nWidth), pPlanche->nWidth);
				SDL_UnlockSurface(pGfx2);

				// Libère les ressources du PSD.
				//free(pPlanche->pPlanes);		// > On garde la planche en mémoire pour l'alpha, traité plus bas.
				//free(pPlanche);
			}
			else
			{
				fprintf(stderr, "LoadLevel(): %s: Unhandled file format.\n", pFilename);
				exit(1);
			}

			// On vérifie que la taille de la planche soit bien égale à celle sauvegardée.
			if (nPlaneSav_BlkLg != (pGfx2->w / 16) || nPlaneSav_BlkHt != (pGfx2->h / 16))
			{
				fprintf(stderr, "LoadLevel(): Size of graphic file '%s' has changed since last edition of level '%s'. Please edit and save level.\n", pFilename, pLevFilename);
//fprintf(stderr, "prev lg=%d ht=%d / new lg=%d ht=%d\n", nPlaneSav_BlkLg, nPlaneSav_BlkHt, (pGfx2->w / 16), (pGfx2->h / 16));
				exit(1);
			}

			// Conversion de l'image en 16 bits.
			gMap.ppPlanesGfx[gMap.nPlanesNb] = SDL_ConvertSurface(pGfx2, gVar.pScreen->format, SDL_SWSURFACE);
			SDL_FreeSurface(pGfx2);
			if (gMap.ppPlanesGfx[gMap.nPlanesNb] == NULL)
			{
				fprintf(stderr, "LoadLevel(): '%s': 16 bits conversion failed.\n", pFilename);
				exit(1);
			}



			// Une page de codes est présente ?
			*(gMap.ppColCodes + gMap.nPlanesNb) = NULL;
			if (((struct SPlane2 *)pBuf)->nFlags & e_FlgFile_Plane_Codes)
			{
#ifdef DEBUG_INFO
printf("Plane #%d, codes found.\n", (int)gMap.nPlanesNb);
#endif
//printf("struct SBlockCol size: %d\n", sizeof(struct SBlockCol));
/*struct SBlockCol
{
	u32	pHt[2];		// [0;15] * 16 => 64 bits.
	u16	nZero;		// 1 bit en plus pour chaque colonne.

	u8	nCol;		// Code de collision.
};*/

				// Malloc des codes des blocs.
				//if ((*(gMap.ppColCodes + gMap.nPlanesNb) = (u8 *)malloc(nPlaneSav_BlkLg * nPlaneSav_BlkHt * sizeof(u8))) == NULL)
				if ((*(gMap.ppColCodes + gMap.nPlanesNb) = (struct SBlockCol *)malloc(nPlaneSav_BlkLg * nPlaneSav_BlkHt * sizeof(struct SBlockCol))) == NULL)
				{
					fprintf(stderr, "LoadLevel(): malloc failed (gMap.ppColCodes[%d]).\n", (int)gMap.nPlanesNb);
					exit(1);
				}
				memset(*(gMap.ppColCodes + gMap.nPlanesNb), 0, nPlaneSav_BlkLg * nPlaneSav_BlkHt * sizeof(struct SBlockCol));
				// Copie.
				//memcpy(*(gMap.ppColCodes + gMap.nPlanesNb), pCur, nPlaneSav_BlkLg * nPlaneSav_BlkHt * sizeof(u8));
				for (j = 0; j < nPlaneSav_BlkLg * nPlaneSav_BlkHt; j++)
				{
					(*(*(gMap.ppColCodes + gMap.nPlanesNb) + j)).nCol = *(pCur + j);
				}

				// Maintenant, on s'occupe des blocs alpha (s'il y en a).
				if (pPlanche != NULL)
				if (pPlanche->nNbPlanes >= 2)
				{
					//u8	*pCodes = *(gMap.ppColCodes + gMap.nPlanesNb);
					struct SBlockCol	*pCodes = *(gMap.ppColCodes + gMap.nPlanesNb);
					u8	*pAlpha;
					u32	nCode;
					u32	ri, rj, k;
					for (k = 0; k < nPlaneSav_BlkHt; k++)
					for (j = 0; j < nPlaneSav_BlkLg; j++)
					{
						nCode = (*(pCodes + (k * nPlaneSav_BlkLg) + j)).nCol;
						if (nCode & 0x0F)
						{
//printf("Block #%d / Code %x\n", (int)((k * nPlaneSav_BlkLg) + j), (int)nCode);

 							pAlpha = pPlanche->pPlanes + (pPlanche->nWidth * (pPlanche->nHeight + (k * 16))) + (j * 16);
//printf("ht : ");
							for (ri = 0; ri < 16; ri++)
							{

								// Plafond ? On fait un NOT sur la hauteur (On sauve les hauteurs du négatif du bloc).
								if ((nCode & 0x0F) == e_BlockCode_Ceiling)
								{
									for (rj = 0; rj < 16; rj++)
										*(pAlpha + ri + (rj * pPlanche->nWidth)) = ~*(pAlpha + ri + (rj * pPlanche->nWidth));
								}

								for (rj = 0; rj < 16; rj++)
								{
									if (*(pAlpha + ri + (rj * pPlanche->nWidth)) != 0) break;
								}
//printf("%d ", 16 - rj);
								if (16 - rj)
								{
									(*(pCodes + (k * nPlaneSav_BlkLg) + j)).nZero |= 1 << ri;	// Colonne ne vaut pas 0.
									(*(pCodes + (k * nPlaneSav_BlkLg) + j)).pHt[ri >> 3] |= (16 - rj - 1) << ((ri & 7) << 2);	// Ht col - 1.
								}
							}
/*
printf("\n");
printf("code=%x / zero=%p / p[1]=%p / p[0]=%p\n",
nCode, (*(pCodes + (k * nPlaneSav_BlkLg) + j)).nZero, (*(pCodes + (k * nPlaneSav_BlkLg) + j)).pHt[1], (*(pCodes + (k * nPlaneSav_BlkLg) + j)).pHt[0] );
*/
						}
					}
				}

				// Avance le pointeur.
				pCur += nPlaneSav_BlkLg * nPlaneSav_BlkHt;
			}
			// Si nécessaire, libère les ressources du PSD.
			if (pPlanche != NULL)
			{
				free(pPlanche->pPlanes);
				free(pPlanche);
			}



			// Malloc des blocs.
			if ((*(gMap.ppPlanesBlocks + gMap.nPlanesNb) = (s32 *)malloc(gMap.nMapLg * gMap.nMapHt * sizeof(s32))) == NULL)
			{
				fprintf(stderr, "LoadLevel(): malloc failed / Plane %d.\n", (int)gMap.nPlanesNb);
				exit(1);
			}
/*
//> v.1.0 - ok. Il faut rajouter la détection des tailles des plans.
			// Lecture des blocs.
			for (j = 0; j < gMap.nMapLg * gMap.nMapHt; j++)
			{
				*(*(gMap.ppPlanesBlocks + gMap.nPlanesNb) + j) = *(s32 *)pCur;	// < On pourrait tester les n°s de blocs ici (càd que ça ne dépasse pas le n° maximum du bloc de la planche).
				// Tests sur les n°s de blocs.
				if (*(s32 *)pCur == -1)
				{
					u32	nBlX, nBlY;
					nBlY = j / gMap.nMapLg;
					nBlX = j - (nBlY * gMap.nMapLg);
					fprintf(stderr, "LoadLevel(): Plane %d : Empty bloc found (%d, %d).\n", (int)gMap.nPlanesNb, (int)nBlX, (int)nBlY);
					exit(1);
				}
				pCur += sizeof(s32);
			}
//< v.1.0
*/

//> v.2.0
			// Lecture des blocs.
			memcpy(*(gMap.ppPlanesBlocks + gMap.nPlanesNb), pCur, gMap.nMapLg * gMap.nMapHt * 4);
			{
			// Recherche de la largeur et de la hauteur du plan.
				u32	ri, rj;
				for (ri = 0; ri < gMap.nMapLg; ri++) if (*(*(gMap.ppPlanesBlocks + gMap.nPlanesNb) + ri) == -1) break;
				for (rj = 0; rj < gMap.nMapHt; rj++) if (*(*(gMap.ppPlanesBlocks + gMap.nPlanesNb) + (rj * gMap.nMapLg)) == -1) break;
				// Size ok ?
				if (ri == 0 || rj == 0)
				{
					fprintf(stderr, "LoadLevel(): Plane %d: Null plane size found! (w=%d, h=%d). Aborted.\n", (int)gMap.nPlanesNb, (int)ri, (int)rj);
					exit(1);
				}
				// Sauvegarde.
				gMap.pPlanesLg[gMap.nPlanesNb] = ri;	// Largeur et hauteur de chaque plan en blocs 16, dans une surface de nMapLg * nMapHt.
				gMap.pPlanesHt[gMap.nPlanesNb] = rj;
#ifdef DEBUG_INFO
printf("Plane #%d, lg=%d, ht=%d\n", (int)gMap.nPlanesNb, (int)gMap.pPlanesLg[gMap.nPlanesNb], (int)gMap.pPlanesHt[gMap.nPlanesNb]);
#endif

			// Recherche des cases vides dans la zone valide (il ne doit pas y en avoir).
				for (rj = 0; rj < gMap.pPlanesHt[gMap.nPlanesNb]; rj++)
				for (ri = 0; ri < gMap.pPlanesLg[gMap.nPlanesNb]; ri++)
				if (*(*(gMap.ppPlanesBlocks + gMap.nPlanesNb) + (rj * gMap.nMapLg) + ri) == -1)
				{
					fprintf(stderr, "LoadLevel(): Plane %d: Empty bloc found at (%d, %d). Aborted.\n", (int)gMap.nPlanesNb, (int)ri, (int)rj);
					exit(1);
				}

// Faire une passe pour transformer les -1 des bords en 0 ? (au cas ou le scroll afficherait en dehors des blocs ?)

			}
//< v.2.0


/*
			// Ajout du plan.
			gMap.PlaneAdd(pFilename);

			// Misc datas.
			pPlane = *(gMap.ppPlanes + ((SPlane2 *)pBuf)->nPlaneNo);
			pPlane->nPlaneNo = ((SPlane2 *)pBuf)->nPlaneNo;
			pPlane->nFlags = ((SPlane2 *)pBuf)->nFlags;
			pPlane->nWndPosX = ((SPlane2 *)pBuf)->nWndPosX;
			pPlane->nWndPosY = ((SPlane2 *)pBuf)->nWndPosY;
			//((SPlane2 *)pBuf)->nWndPosY->nHasCmd

			// Le plan.
			for (j = 0; j < gMap.nMapLg * gMap.nMapHt; j++)
			{
				*(pPlane->pPlane + j) = *(s32 *)pCur;	// < On pourrait tester les nos de blocs ici.
				pCur += sizeof(s32);
			}
*/

			gMap.nPlanesNb++;	// Un plan en plus.

			// Le prochain qui arrivera devrait être le...
			nPlaneNext++;
			break;

		case e_FileSection_Cmd :		// On rajoute le fichier de commandes au dernier plan ajouté.
#ifdef DEBUG_INFO
			printf("Cmd section\n");
#endif
			if (nMapSection == 0)
			{
				printf("Cmd section found prior to map section! Section ignored.\n");
				break;
			}

			// On skippe le fichier de commandes.

			// u8 : Nb de bytes du nom de fichier.
			// char * x : Nom de fichier, sans 0 terminal.
			// u32 : Date du fichier.
			// u32 : Nb de bytes dans le fichier de commandes.
			// char * x : Le fichier de commandes, sans 0 terminal.
			pCur = pBuf;

			j = *pCur;
			pCur++;
			//- strncpy(pFilename, (char *)pCur, j);
			//- pFilename[j] = 0;
			pCur += j;
//			printf("cmd fn=%s\n", pFilename);

			//- nDate = *(u32 *)pCur;
			pCur += 4;
//			printf("cmd date=%d\n", (int)nDate);

			j = *(u32 *)pCur;
			pCur += 4;
/*
			//printf("sz=%d\n",(int)j);
			// Malloc du buffer de lecture.
			if ((pBufTxt = (char *)malloc(j + 1)) == NULL)
			{
				printf("Load/cmd : malloc failed.\n");
				break;
			}
			strncpy(pBufTxt, (char *)pCur, j);
			pBufTxt[j] = 0;
*/
			pCur += j;
			//printf("cmd file:\n%s\n", pBufTxt);
			//- // Ajoute le fichier de commandes au dernier plan.
			//- gMap.PlaneCmdfileAddFromBuf(gMap.nPlanesNb - 1, pFilename, pBufTxt, nDate);
			//- free(pBufTxt);

			// On arrive aux monstres. On va stocker la liste directement.

			// u32 : Nb de monstres.
			// x : Datas des monstres, les uns à la suite des autres : SMst0 + x bytes de data.
			j = *(u32 *)pCur;
			pCur += 4;

#ifdef DEBUG_INFO
			printf("nb mst=%d\n",(int)j);
			printf("nb bytes to read: %d\n", (int)((pBuf + nSz) - pCur) );
#endif

			// Allocations.
//***			if ((gLoadedMst.pMstData = (u8 *)malloc((pBuf + nSz) - pCur)) == NULL)	// Pour copie directe.
//printf("SMst0:%d / SMstEdt0:%d / diff:%d\n", sizeof(struct SMst0), sizeof(struct SMstEdt0), sizeof(struct SMst0) - sizeof(struct SMstEdt0) );
			if ((gLoadedMst.pMstData = (u8 *)malloc( ((pBuf + nSz) - pCur) + (j * (sizeof(struct SMst0) - sizeof(struct SMstEdt0))) ) ) == NULL)	// Pour insertion n° ordre.
			{
				fprintf(stderr, "Load/cmd: malloc (#2) failed.\n");
				exit(1);
			}
			if ((gLoadedMst.ppMstPtrX = (struct SMst0 **)malloc(j * 2 * sizeof(struct SMst0 *))) == NULL)	// j * 2 : On alloue 2 fois l'espace pour le tri en X et le tri en Y.
			{
				fprintf(stderr, "Load/cmd: malloc (#3) failed.\n");
				exit(1);
			}
			gLoadedMst.ppMstPtrY = gLoadedMst.ppMstPtrX + j;
			if ((gLoadedMst.pMstState = (u8 *)malloc(j * sizeof(u8))) == NULL)
			{
				fprintf(stderr, "Load/cmd: malloc (#4) failed.\n");
				exit(1);
			}
			memset(gLoadedMst.pMstState, e_MstState_Asleep, j * sizeof(u8));	// RAZ etat.
			gLoadedMst.nMstNbInList = j;

//***			// Copie des datas.
//***			memcpy(gLoadedMst.pMstData, pCur, (pBuf + nSz) - pCur);	// on rajoute l'index => plus de memcpy possible...

			// Copie des datas + insertion d'un n° d'ordre.
			// Ce n° d'ordre sera utile pour l'accès à la table pMstState. Avec deux listes de ptrs sur les datas, c'est obligatoire.
			{
				u8	*pSrcPt, *pDstPt;
				u32	nb, m;

				pSrcPt = pCur;
				pDstPt = gLoadedMst.pMstData;
				nb = j;
				k = 0;
				while (nb)
				{
					// Header.
					memcpy(pDstPt, pSrcPt, sizeof(struct SMstEdt0));
					pSrcPt += sizeof(struct SMstEdt0);
					(*(struct SMst0 *)pDstPt).nIdx = k;		// Le fameux n° d'ordre.
					m = (*(struct SMst0 *)pDstPt).nNbBytes;	// Nb de bytes de datas.
					pDstPt += sizeof(struct SMst0);
					// Datas.
					memcpy(pDstPt, pSrcPt, m);
					pSrcPt += m;
					pDstPt += m;
					// Indexs.
					nb--;
					k++;
				}
			}




/*
// shoot
			// Initialisation des pointeurs. On en profite pour retourner la liste : de bas en haut (et de droite à gauche, mais c'est pas important).
			pCur = gLoadedMst.pMstData;
			//k = 0;
			while (j)
			{
				struct SMst0 *pMstCur;

				// *(gLoadedMst.ppMstPtr + k) = (struct SMst0 *)pCur;
				//pMstCur = *(gLoadedMst.ppMstPtr + k);
				*(gLoadedMst.ppMstPtr + j-1) = (struct SMst0 *)pCur;
				pMstCur = *(gLoadedMst.ppMstPtr + j-1);
				pCur += pMstCur->nNbBytes + sizeof(struct SMst0);

				printf("Mst no=%d / x=%d / y=%d / nb nytes=%d\n", (int)pMstCur->nMstNo,
					(int)pMstCur->nPosX, (int)pMstCur->nPosY, (int)pMstCur->nNbBytes);

				//k++;
				j--;
			}
*/


			// Initialisation des tableaux de pointeurs.
			pCur = gLoadedMst.pMstData;
			k = 0;
			while (j)
			{
				struct SMst0 *pMstCur;

				*(gLoadedMst.ppMstPtrX + k) = (struct SMst0 *)pCur;
				*(gLoadedMst.ppMstPtrY + k) = (struct SMst0 *)pCur;
				pMstCur = *(gLoadedMst.ppMstPtrX + k);
				pCur += pMstCur->nNbBytes + sizeof(struct SMst0);

//				printf("Mst no=%d / x=%d / y=%d / nb nytes=%d\n", (int)pMstCur->nMstNo,
//					(int)pMstCur->nPosX, (int)pMstCur->nPosY, (int)pMstCur->nNbBytes);

				k++;
				j--;
			}
			// Tri sur le X.
			qsort(gLoadedMst.ppMstPtrX, gLoadedMst.nMstNbInList, sizeof(struct SMst0 *), qscmpMstXY);
			// Tri sur le Y.
			//>> Ok, mais inutile, les monstres sont enregistrés de gauche à droite et de haut en bas.
			//qsort(gLoadedMst.ppMstPtrY, gLoadedMst.nMstNbInList, sizeof(struct SMst0 *), qscmpMstYX);
/*
//>>debug:
printf("*** Après tri X :\n");
for (k = 0; k < gLoadedMst.nMstNbInList; k++)
{
	struct SMst0 *pMstCur;
	pMstCur = *(gLoadedMst.ppMstPtrX + k);
	printf("Mst no=%d / x=%d / y=%d / nb nytes=%d\n", (int)pMstCur->nMstNo,
		(int)pMstCur->nPosX, (int)pMstCur->nPosY, (int)pMstCur->nNbBytes);
}
printf("*** Après tri Y :\n");
for (k = 0; k < gLoadedMst.nMstNbInList; k++)
{
	struct SMst0 *pMstCur;
	pMstCur = *(gLoadedMst.ppMstPtrY + k);
	printf("Mst no=%d / x=%d / y=%d / nb nytes=%d\n", (int)pMstCur->nMstNo,
		(int)pMstCur->nPosX, (int)pMstCur->nPosY, (int)pMstCur->nNbBytes);
}
//<<debug:
*/



/*
			pPlane = *(gMap.ppPlanes + (gMap.nPlanesNb - 1));
			while(j)
			{
//printf("x=%d y=%d no=%d nb bytes=%d\n",
//(int)((SMst0 *)pCur)->nPosX,(int)((SMst0 *)pCur)->nPosY,
//(int)((SMst0 *)pCur)->nMstNo,(int)((SMst0 *)pCur)->nNbBytes);

				*(pPlane->ppMstPlane + (((SMst0 *)pCur)->nPosY * gMap.nMapLg) + ((SMst0 *)pCur)->nPosX) = new CMstUnit;
				(*(pPlane->ppMstPlane + (((SMst0 *)pCur)->nPosY * gMap.nMapLg) + ((SMst0 *)pCur)->nPosX))->InitFromSavedFile(pCur);

				pCur += ((SMst0 *)pCur)->nNbBytes;
				pCur += sizeof(SMst0);
				j--;
			}

			// Compare le fichier de commandes chargé et celui sur disque. Et fait ce qu'il faut.
			pPlane->CmdFileCompare();
*/
			break;

		case e_FileSection_Misc :
#ifdef DEBUG_INFO
			printf("Misc section\n");
#endif
/*
			gMisc.nFlags = ((SMisc0 *)pBuf)->nFlags;
			gMisc.nTransR = ((SMisc0 *)pBuf)->nTransR;
			gMisc.nTransG = ((SMisc0 *)pBuf)->nTransG;
			gMisc.nTransB = ((SMisc0 *)pBuf)->nTransB;
*/
#ifdef DEBUG_INFO
			printf("Transparency: R=%d G=%d B=%d\n", ((struct SMisc0 *)pBuf)->nTransR, ((struct SMisc0 *)pBuf)->nTransG, ((struct SMisc0 *)pBuf)->nTransB);
#endif

			gMap.nTranspColorKey = SDL_MapRGB(gVar.pScreen->format, ((struct SMisc0 *)pBuf)->nTransR, ((struct SMisc0 *)pBuf)->nTransG, ((struct SMisc0 *)pBuf)->nTransB);

			break;

		default:
#ifdef DEBUG_INFO
			printf("LoadLevel: Unknown section type: %d.\n", (int)nType);
#endif
			break;
		}

	}

	// Fermeture du fichier.
	fclose(fPt);
	free(pBuf);

#ifdef DEBUG_INFO
printf("Level loaded.\n");
#endif

/*
	// Fini ! Derniers tests.
	if (gMap.nPlanesNb != sMap.nPlanesNb)
	{
		printf("CFile::Load : Wrong number of planes loaded! Abort.\n");
		exit(1);
	}

	gMap.nFirstWindow = sMap.nFirstWnd;
	gMap.nActivePlane = sMap.nActivePlane;
*/


	// Allocation de 'plans' pour stocker les index des anims de blocs.
	for (j = 0; j < MAP_PLANES_MAX; j++) gMap.ppBlkAnmPlanes[j] = NULL;
	if ((gMap.pBlkAnmMem = (u8 *)malloc(gMap.nPlanesNb * gMap.nMapLg * gMap.nMapHt)) == NULL)
	{
		printf("LoadLevel(): malloc (gMap.pBlkAnmMem) failed.\n");
		return;
	}
	memset(gMap.pBlkAnmMem, -1, gMap.nPlanesNb * gMap.nMapLg * gMap.nMapHt);	// Tout à 0xFF.
	for (j = 0; j < gMap.nPlanesNb; j++) gMap.ppBlkAnmPlanes[j] = gMap.pBlkAnmMem + (j * gMap.nMapLg * gMap.nMapHt);

}


