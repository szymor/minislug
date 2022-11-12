
#include "includes.h"


struct SBlkAnmStockage
{
	u32	nPlane;				// Plan concerné.
	u32	nFirstBlk;			// Premier bloc de l'anim.
	u32 nAnmLg, nAnmHt;		// Lg et ht du rectangle.
	u32	nImgNb;				// Nombre d'images de l'anim.
	s32 nOffset;			// Offset du décalage pour chaque rectangle (en nombre de blocs).
	u32 nSpeed;				// Vitesse (en frames).
	u32 nDesync;			// Pause (en frames) au moment de la première anim, pour désynchroniser les anims entre elles.
};


/*
struct SBlkAnmStockage	gAnmBlk_lev2[] =
{
	{ 0, 1813, 7, 4, 4, 7, 8, 0 },	// Ventilateur 1.
	{ 0, 2069, 7, 4, 4, 7, 8, 4 },	// Ventilateur 2.
	{ (u32)-1, 0, 0, 0, 0, 0, 0, 0 },
};
*/

struct SBlkAnmStockage	gAnmBlk_lev4[] =
{
	{ 0, 192, 8, 5, 8, 8, 8, 0 },	// Ville en flames (rouge).
	{ (u32)-1, 0, 0, 0, 0, 0, 0, 0 },
};

struct SBlkAnmStockage	gAnmBlk_lev5[] =
{
	{ 1, 1640, 24, 1, 6, 64, 16, 0 },	// Anim d'eau.
	{ (u32)-1, 0, 0, 0, 0, 0, 0, 0 },
};

struct SBlkAnmStockage	gAnmBlk_lev7[] =
{
	{ 0,   0,  4, 7, 8,   4, 16, 0 },	// Eau bkg (haut).
	{ 0, 256, 32, 7, 4, 224, 16, 8 },	// Rochers bkg (bas).
	{ 1,   0, 32, 4, 8, 128, 16, 12 },	// Rochers fg (bas).
	{ 1,1024, 32, 4, 8, 128, 16, 4 },	// Rochers fg (haut).
	{ 1,2080, 29, 3, 8,  96, 16, 10 },	// Sortie.
	{ (u32)-1, 0, 0, 0, 0, 0, 0, 0 },
};

struct SBlkAnmStockage	gAnmBlk_lev8[] =
{
	{ 2, 2487, 35, 2, 4, 196, 8, 0 },	// Fumée loco 1.
	{ 2, 1346, 26, 5, 4, 490, 8, 0 },	// Fumée loco 2.
	{ 2, 1274, 66, 1, 4,  98, 8, 4 },	// Roues + chaînes.
	{ 2,  980, 10, 3, 8,  10, 8, 2 },	// Roues loco.
	{ (u32)-1, 0, 0, 0, 0, 0, 0, 0 },
};

struct SBlkAnmStockage	gAnmBlk_lev13[] =
{
	{ 3,  282, 46, 1, 8,  47, 7, 0 },	// Eau 1.
	{ 3,  658, 47, 2, 8,  94, 7, 2 },	// Eau 2.
	{ 3, 1410, 28, 3, 8, 141, 7, 4 },	// Eau 3.
	{ 2, 1890, 24, 4, 8, 484, 7, 3 },	// Cascade 1.
	{ 2, 1914, 22, 4, 8, 484, 7, 5 },	// Cascade 2.
	{ 2, 4737, 57, 1, 8, 121, 7, 1 },	// Eau 4.
	{ 0,   20,  8,15, 8,   8, 7, 6 },	// Cascade bkg.
	{ (u32)-1, 0, 0, 0, 0, 0, 0, 0 },
};

struct SBlkAnmStockage	gAnmBlk_lev15[] =
{
	{ 1,  350, 23, 2, 4,  50, 16, 0 },	// Eau plan 1.
	{ 2,  200, 23, 2, 4,  50, 16, 0 },	// Eau plan 2.
	{ (u32)-1, 0, 0, 0, 0, 0, 0, 0 },
};

struct SBlkAnmStockage	gAnmBlk_lev16[] =
{
	{ 1,  238, 2, 1, 8,   2, 16, 0 },	// Petit symbole avec la flèche.
	{ 1,  255, 2, 1, 8,   2,  8, 4 },	// Lumières aériennes.
	{ (u32)-1, 0, 0, 0, 0, 0, 0, 0 },
};


// Table des anims par niveau.
struct SBlkAnmStockage	*gpAnmBlk[LEVEL_MAX] =
{
	NULL,
	NULL,
	NULL,	//gAnmBlk_lev2,
	NULL,
	gAnmBlk_lev4,
	gAnmBlk_lev5,
	NULL,
	gAnmBlk_lev7,
	gAnmBlk_lev8,
	NULL,
	NULL,
	NULL,
	NULL,
	gAnmBlk_lev13,
	NULL,
	gAnmBlk_lev15,
	gAnmBlk_lev16,
	NULL,
};

//=== === ===

#define	ANMBLK_MAX	8

// Pour les anims.
struct SAnmBlkAnm
{
	struct SBlkAnmStockage	*pOrgAnm;	// Pointeur sur les datas de base.
	u32	nCurImg;		// Image en cours.
	u32	nTimeLeft;		// Temps restant.

};

// La structure générale.
struct SAnmBlkGen
{
	struct SAnmBlkAnm	pAnmTb[ANMBLK_MAX];
	u32	nAnmBlkNb;		// Nombre d'anims définies.

};
struct SAnmBlkGen	gAnmBlk;

//=== === ===

// Avance dans une anim.
u32 AnmBlkAdvance(u32 nAnmNo)
{
	struct SBlkAnmStockage *pOrg;

	if (--gAnmBlk.pAnmTb[nAnmNo].nTimeLeft) return(0);	// Décrémentation du temps.

	pOrg = gAnmBlk.pAnmTb[nAnmNo].pOrgAnm;
	if (++gAnmBlk.pAnmTb[nAnmNo].nCurImg >= pOrg->nImgNb) gAnmBlk.pAnmTb[nAnmNo].nCurImg = 0;
	gAnmBlk.pAnmTb[nAnmNo].nTimeLeft = pOrg->nSpeed;
//printf("anmblk #%d / img %d\n", nAnmNo, gAnmBlk.pAnmTb[nAnmNo].nCurImg);
	return(1);
}

// Taggue dans les 'plans' d'anim les blocs affectés par une anim.
// Note : Algo pas terrible, car même si ce n'est qu'à l'init, on parcourt la map autant de fois qu'il y a d'anims...
void AnmBlkBlocksTag(struct SBlkAnmStockage *pAnm, u32 nAnmNo)
{
	s32	*pBlkPlane;
	u8	*pAnmPlane;
	u32	bi, bj;
	u32	aj;
	s32	nBlkNo;
	u32	nGfxPlaneLg;

	pBlkPlane = gMap.ppPlanesBlocks[pAnm->nPlane];
	pAnmPlane = gMap.ppBlkAnmPlanes[pAnm->nPlane];
	nGfxPlaneLg = gMap.ppPlanesGfx[pAnm->nPlane]->w / 16;
//todo: voir pour optimiser sur les tailles des plans, pas de la map.
	for (bj = 0; bj < gMap.nMapHt; bj++)
	for (bi = 0; bi < gMap.nMapLg; bi++)
	{
		nBlkNo = *(pBlkPlane + (bj * gMap.nMapLg) + bi);
		// On teste si chaque bloc de la map appartient à l'anim ou pas.
		for (aj = 0; aj < pAnm->nAnmHt; aj++)
		{
			if (nBlkNo >= pAnm->nFirstBlk + (aj * nGfxPlaneLg) && nBlkNo < pAnm->nFirstBlk + (aj * nGfxPlaneLg) + pAnm->nAnmLg)
			{
				*(pAnmPlane + (bj * gMap.nMapLg) + bi) = nAnmNo;	// Oui, on note le n° de l'anim en (x,y).
				break;
			}
		}
	}

}

// Init d'une anim.
void AnmBlkAnmInit(struct SBlkAnmStockage *pAnm)
{
	if (gAnmBlk.nAnmBlkNb >= ANMBLK_MAX)
	{
		fprintf(stderr, "AnmBlkAnmInit(): Out of blocks animation slots!\n");
		return;
	}
	if (pAnm->nPlane >= gMap.nPlanesNb)
	{
		fprintf(stderr, "AnmBlkAnmInit(): Anim #%d, plane #%d doesn't exist.\n", (int)gAnmBlk.nAnmBlkNb, (int)pAnm->nPlane);	//!!! Si plusieurs anm merdent, le n° d'anim sera faux.
		return;
	}

	// Ok.
	gAnmBlk.pAnmTb[gAnmBlk.nAnmBlkNb].pOrgAnm = pAnm;
	// On se place sur la dernière image, avec un timeleft = 1 => Le advance remettra tout à 0.
	gAnmBlk.pAnmTb[gAnmBlk.nAnmBlkNb].nCurImg = pAnm->nImgNb - 1;
	gAnmBlk.pAnmTb[gAnmBlk.nAnmBlkNb].nTimeLeft = 1;
	AnmBlkAdvance(gAnmBlk.nAnmBlkNb);
	gAnmBlk.pAnmTb[gAnmBlk.nAnmBlkNb].nTimeLeft += pAnm->nDesync;	// Première désynchro.
	// Taggue les blocs dans les 'plans' de blocs animés.
	AnmBlkBlocksTag(pAnm, gAnmBlk.nAnmBlkNb);

	// Une anim de plus.
	gAnmBlk.nAnmBlkNb++;

}

// Init des anims de blocs (1 fois par niveau).
void AnmBlkInit(u32 nLevel)
{
	struct SBlkAnmStockage *pAnmTb;

//pAnmTb = gAnmBlk_lev4;	// A passer en prm.
	pAnmTb = gpAnmBlk[nLevel];//NULL;

	gAnmBlk.nAnmBlkNb = 0;
	if (pAnmTb == NULL) return;
	while (1)
	{
		if (pAnmTb->nPlane == (u32)-1) break;
		AnmBlkAnmInit(pAnmTb);
		pAnmTb++;
	}

}


// Gestion des anims de blocs.
void AnmBlkManage(void)
{
	u32	i;

//printf("anmblk manage\n");
	for (i = 0; i < gAnmBlk.nAnmBlkNb; i++)
	{
		if (AnmBlkAdvance(i))
		{
			u32	pi, pj;
			s32	nPosX, nPosY;
			u8	*pAnmPlane;

			ScrollGetPlanePosXY(&nPosX, &nPosY, gAnmBlk.pAnmTb[i].pOrgAnm->nPlane);
			pAnmPlane = gMap.ppBlkAnmPlanes[gAnmBlk.pAnmTb[i].pOrgAnm->nPlane];
			nPosX >>= 12;
			nPosY >>= 12;
			for (pj = 0; pj < (SCR_Height / 16) + 1 && nPosY + pj < gMap.nMapHt; pj++)
			for (pi = 0; pi < (SCR_Width / 16) + 1 && nPosX + pi < gMap.nMapLg; pi++)
			{
				if (*(pAnmPlane + ((nPosY + pj) * gMap.nMapLg) + nPosX + pi) == i)
				{
//printf("update blk at %d %d\n", nPosX + pi, nPosY + pj);
					Scroll_BlkAnm_BlockUpdate(gAnmBlk.pAnmTb[i].pOrgAnm->nPlane, nPosX + pi, nPosY + pj, gAnmBlk.pAnmTb[i].nCurImg * gAnmBlk.pAnmTb[i].pOrgAnm->nOffset);
				}
			}
		}

	}
}

//=== === ===

// Une nouvelle colonne est entrée dans un plan du scroll, on teste si des blocs doivent être updatés de force à ce moment là.
// (Les blocs entrent avec leur image de base, on force la bonne image).
// In : (x,y) = bloc du haut de la colonne.
void AnmBlkScrollNewCol(u32 nPlane, u32 nPosX, u32 nPosY)
{
	u32	pj;
	u8	*pAnmPlane;
	u32	nAnmNo;

	if (gAnmBlk.nAnmBlkNb == 0) return;

	pAnmPlane = gMap.ppBlkAnmPlanes[nPlane];
	for (pj = 0; pj < (SCR_Height / 16) + 1 && nPosY + pj < gMap.nMapHt; pj++)
	{
		nAnmNo = *(pAnmPlane + ((nPosY + pj) * gMap.nMapLg) + nPosX);
		if (nAnmNo != 0xFF)
		{
//printf("AnmBlkScrollNewCol: update blk at %d %d, plane %d\n", nPosX, nPosY + pj, nPlane);
			Scroll_BlkAnm_BlockUpdate(nPlane, nPosX, nPosY + pj, gAnmBlk.pAnmTb[nAnmNo].nCurImg * gAnmBlk.pAnmTb[nAnmNo].pOrgAnm->nOffset);
		}
	}
}

// Une nouvelle ligne est entrée dans un plan du scroll, on teste si des blocs doivent être updatés de force à ce moment là.
// (Les blocs entrent avec leur image de base, on force la bonne image).
// In : (x,y) = bloc de gauche de la ligne.
void AnmBlkScrollNewLn(u32 nPlane, u32 nPosX, u32 nPosY)
{
	u32	pi;
	u8	*pAnmPlane;
	u32	nAnmNo;

	if (gAnmBlk.nAnmBlkNb == 0) return;

	pAnmPlane = gMap.ppBlkAnmPlanes[nPlane];
	for (pi = 0; pi < (SCR_Width / 16) + 1 && nPosX + pi < gMap.nMapLg; pi++)
	{
		nAnmNo = *(pAnmPlane + (nPosY * gMap.nMapLg) + nPosX + pi);
		if (nAnmNo != 0xFF)
		{
//printf("AnmBlkScrollNewCol: update blk at %d %d, plane %d\n", nPosX+pi, nPosY, nPlane);
			Scroll_BlkAnm_BlockUpdate(nPlane, nPosX + pi, nPosY, gAnmBlk.pAnmTb[nAnmNo].nCurImg * gAnmBlk.pAnmTb[nAnmNo].pOrgAnm->nOffset);
		}
	}
}

