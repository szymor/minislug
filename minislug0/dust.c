// Gestion des poussières.

#include "includes.h"

// Struct pour passer certains prm en fct de certains flags.
struct SDustExt	gDustExg;

// Format d'un slot.
struct SDust
{
	u8	nUsed;		// 0 = slot vide, 1 = slot occupé.

	u8	nPrio;		// Priorité d'affichage.

	s32	nAnm;		// Anim.
	s32	nPosX, nPosY;	// Avec 8 bits de virgule fixe.
	s32	nFlags;

	s32	nSpdX, nSpdY;
	u8	nAngle;		// Angle si rotation.
	u8	nAngleInc;	// Incrément de l'angle.
	u64	*pLinkAnm;	// Pour enchaîner une autre anim quand un dust avec grv touche le sol.
};

#define	DUST_MAX_SLOTS	64
struct SDust	gpDustSlots[DUST_MAX_SLOTS];
u32	gnDustLastUsed;

// RAZ moteur.
void DustInitEngine(void)
{
	u32	i;

	// RAZ de tous les slots.
	for(i = 0; i < DUST_MAX_SLOTS; i++)
	{
		gpDustSlots[i].nUsed = 0;
	}
	gnDustLastUsed = 0;

}

// Cherche un slot libre.
// Out : N° d'un slot libre. -1 si erreur.
s32 DustGetSlot(void)
{
	u32	i;

	for (i = gnDustLastUsed; i < DUST_MAX_SLOTS; i++)
	{
		if (gpDustSlots[i].nUsed == 0)
		{
			gpDustSlots[i].nUsed = 1;
			gnDustLastUsed = i + 1;		// La recherche commencera au suivant.
			return (i);
		}
	}
	return (-1);
}

// Libère un slot.
void DustReleaseSlot(u32 nSlotNo)
{
	// Libère l'anim.
	if (gpDustSlots[nSlotNo].nAnm != -1) AnmReleaseSlot(gpDustSlots[nSlotNo].nAnm);
	// Pour accélérer la recherche des slots libres.
	if (nSlotNo < gnDustLastUsed)
	{
		gnDustLastUsed = nSlotNo;
	}
	gpDustSlots[nSlotNo].nUsed = 0;

}

// Init d'une anim.
// In : nPosX + nPosY avec 8 bits de virgule fixe.
// Out : N° du slot. -1 si erreur.
s32 DustSet(u64 *pAnm, s32 nPosX, s32 nPosY, u8 nPrio, u32 nFlags)
{
	s32	nSlotNo;

	if ((nSlotNo = DustGetSlot()) == -1) //return (-1);
	{
		fprintf(stderr, "DustSet(): Warning, out of dust slots!\n");
		return (-1);
	}
//printf("Dust slot: %d\n", (int)nSlotNo);
	if ((gpDustSlots[nSlotNo].nAnm = AnmSet(pAnm, -1)) == -1) { DustReleaseSlot(nSlotNo); return (-1); }

	gpDustSlots[nSlotNo].nPrio = nPrio;
	gpDustSlots[nSlotNo].nPosX = nPosX;
	gpDustSlots[nSlotNo].nPosY = nPosY;
	gpDustSlots[nSlotNo].nFlags = nFlags;

	return (nSlotNo);

}
// Idem, mais init de mouvement avec.
s32 DustSetMvt(u64 *pAnm, s32 nPosX, s32 nPosY, s32 nSpdX, s32 nSpdY, u8 nPrio, u32 nFlags)
{
	s32	nSlotNo;

	nSlotNo = DustSet(pAnm, nPosX, nPosY, nPrio, nFlags);
	if (nSlotNo == -1) return (-1);

	gpDustSlots[nSlotNo].nFlags |= e_DustFlag_Mvt;
	if (gpDustSlots[nSlotNo].nFlags & e_DustFlag_Gravity)
		gpDustSlots[nSlotNo].nFlags ^= e_DustFlag_CheckGnd;		// Avec la gravité on teste le dur (avec un XOR au cas ou on voudrait spécifiquement de la gravité mais pas tester le dur).
	gpDustSlots[nSlotNo].nSpdX = nSpdX;
	gpDustSlots[nSlotNo].nSpdY = nSpdY;
	gpDustSlots[nSlotNo].nAngle = 0;
	gpDustSlots[nSlotNo].nAngleInc = gDustExg.nRotInc;
	gpDustSlots[nSlotNo].pLinkAnm = gDustExg.pLinkAnm;

	return (nSlotNo);
}

// Avance les anims toutes les frames.
void DustManage(void)
{
	u32	nBlockNo, nHt, nCol;
	s32	*pBlocks;
	pBlocks = gMap.ppPlanesBlocks[gMap.nHeroPlane];	// Blocs.

	u32	i;

	for (i = 0; i < DUST_MAX_SLOTS; i++)
	{
		if (gpDustSlots[i].nUsed)
		{
			s32	nSpr;

			nSpr = AnmGetImage(gpDustSlots[i].nAnm);
			if (nSpr == -1)
			{
				// L'anim est finie. On kille la poussière.
				DustReleaseSlot(i);
				continue;
			}

			// Mouvement ?
			if (gpDustSlots[i].nFlags & e_DustFlag_Mvt)
			{
				// Gravité ?
				if (gpDustSlots[i].nFlags & e_DustFlag_Gravity)
				{
					// Gravité, on tue la poussière dès qu'on arrive dans du dur.
					gpDustSlots[i].nSpdY += GRAVITY/2;
					if (gpDustSlots[i].nSpdY > SPDY_MAX/2) gpDustSlots[i].nSpdY = SPDY_MAX/2;

// Quand gravité, ralentissement X.
#define	DUST_GRV_DECX	0x04
if (gpDustSlots[i].nSpdX)
{
	s32	nInc = (ABS(gpDustSlots[i].nSpdX) > DUST_GRV_DECX ? DUST_GRV_DECX : ABS(gpDustSlots[i].nSpdX));
	gpDustSlots[i].nSpdX += (gpDustSlots[i].nSpdX > 0 ? -nInc : nInc);
}

				}
				// Déplacement.
				gpDustSlots[i].nPosX += gpDustSlots[i].nSpdX;
				gpDustSlots[i].nPosY += gpDustSlots[i].nSpdY;
				// Test du dur ?
				if (gpDustSlots[i].nFlags & e_DustFlag_CheckGnd)
				{
					// Hors du plan ?
					if ((u32)(gpDustSlots[i].nPosX >> 12) >= gMap.pPlanesLg[gMap.nHeroPlane] ||
						(u32)(gpDustSlots[i].nPosY >> 12) >= gMap.pPlanesHt[gMap.nHeroPlane])
					{
						DustReleaseSlot(i);
						continue;
					}
					// Test du dur.
					nBlockNo = *(pBlocks + ((gpDustSlots[i].nPosY >> 12) * gMap.nMapLg) + (gpDustSlots[i].nPosX >> 12) );
					// On teste seulement les blocs durs.
					nCol = (gMap.ppColCodes[gMap.nHeroPlane] + nBlockNo)->nCol;
					if ((nCol & 0x0f) == e_BlockCode_Hard)
					{
						if ((nHt = BlockGetHeight(nBlockNo, (gpDustSlots[i].nPosX >> 8))) != 0)
						{
							nHt = 16 - nHt;
							if (((gpDustSlots[i].nPosY >> 8) & 0x0F) >= nHt)	// Est-on dans le dur ?
							{
								// Rebond ?
								if (gpDustSlots[i].nFlags & e_DustFlag_Bounce)
								{
									gpDustSlots[i].nFlags &= ~e_DustFlag_Bounce;
									gpDustSlots[i].nSpdY = -gpDustSlots[i].nSpdY / 2;
								}
								else	// Link anm ? (Lors du contact avec le sol).
								if (gpDustSlots[i].nFlags & e_DustFlag_LinkAnm)
								{
									gpDustSlots[i].nFlags &= ~(e_DustFlag_LinkAnm | e_DustFlag_Mvt | e_DustFlag_Gravity | e_DustFlag_CheckGnd | e_DustFlag_Bounce | e_DustFlag_Rotation);
									AnmSet(gpDustSlots[i].pLinkAnm, gpDustSlots[i].nAnm);
								}
								else
									DustReleaseSlot(i);
							}
						}
					}
				}
			}

			// Affichage de la poussière.
			if (gpDustSlots[i].nFlags & e_DustFlag_Rotation)
			{
				SprDisplayRotoZoom(nSpr ^ (gpDustSlots[i].nFlags & e_DustFlag_FlipX ? SPR_Flip_X : 0), gpDustSlots[i].nPosX >> 8, gpDustSlots[i].nPosY >> 8, gpDustSlots[i].nPrio, gpDustSlots[i].nAngle, 0x100);
				gpDustSlots[i].nAngle += gpDustSlots[i].nAngleInc;
			}
			else
				SprDisplay(nSpr ^ (gpDustSlots[i].nFlags & e_DustFlag_FlipX ? SPR_Flip_X : 0), gpDustSlots[i].nPosX >> 8, gpDustSlots[i].nPosY >> 8, gpDustSlots[i].nPrio);

		}
	}


/*
//> debug
u32	nbm = 0;
for (i = 0; i < DUST_MAX_SLOTS; i++) if (gpDustSlots[i].nUsed) nbm++;
char	pScore[8+1] = "00000000";
MyItoA(nbm, pScore);
Font_Print(10, 100, pScore, 0);
//< debug
*/

}




