
#include "includes.h"
#include "monsters_inc.h"

//=============================================================================
// Mst 40 : L11 Space Trash.

// Spr dispo :
//e_Spr_Lev11_SpaceJunk_Bkg : 4 spr
//e_Spr_Lev11_SpaceJunk_Fg : 3 spr

/*
[mst40] L11 Space Trash
Nb = 0:2: 3 - 5 - 7		; Nb de merdes.
*/

void Scroll_HeroPlaneScrollPosXY_Get(s32 *pnPosX, s32 *pnPosY);

// Structure spécifique.
#define	L11SPACETRASH_MAX	7
struct SMst40_L11SpaceTrash0
{
	u8	nWait[L11SPACETRASH_MAX];
	s16	nPosY[L11SPACETRASH_MAX];
	u8	nNb;
};

void Mst40_Init_L11SpaceTrash0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst40_L11SpaceTrash0	*pSpe = (struct SMst40_L11SpaceTrash0 *)pMst->pData;
	s32	nVal;

	nVal = GetBits(0, 2, pData, 0);
	pSpe->nNb = (nVal * 2) + 3;		// 0 1 2 => 3 5 7

	u32	i;
	for (i = 0; i < pSpe->nNb; i++)
	{
		pSpe->nPosY[i] = (i & 1 ? -(SCR_Height / 2)+40 : 0) << 4;
		pSpe->nPosY[i] -= 50 << 4;
		pSpe->nWait[i] = (i + pSpe->nNb) * 96;//32;
	}

}

s32 Mst40_Main_L11SpaceTrash0(struct SMstCommon *pMst)
{
	struct SMst40_L11SpaceTrash0	*pSpe = (struct SMst40_L11SpaceTrash0 *)pMst->pData;
	s32	nPosX;
	u32	nInc;
	u32	i;
	u32	nCnt = 0;


	nInc = SCR_Width / (pSpe->nNb + 1);
//	nPosX = nInc / 2;
	nPosX = nInc;
	for (i = 0; i < pSpe->nNb; i++)
	{
		// Wait.
		if (pSpe->nWait[i]) pSpe->nWait[i]--;
		// Avance.
		if (pSpe->nWait[i] == 0)
		{
			if (pSpe->nPosY[i] >> 4 < SCR_Height + 50)
			{
				pSpe->nPosY[i] += (i & 1 ? 0x18 : 0x12);
				// Affichage.
				SprDisplay((i & 1 ? e_Spr_Lev11_SpaceJunk_Fg : e_Spr_Lev11_SpaceJunk_Bkg) + (i >> 1), nPosX, (gScrollPos.nPosY >> 8) + ((s32)pSpe->nPosY[i] >> 4), e_Prio_EnnemiesBg - 4 + (i & 1));
			}
			else
				nCnt++;		// Fin.
		}
		nPosX += nInc;
	}

	// Terminé ?
	if (nCnt == pSpe->nNb) return (e_MstState_Dead);

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 41 : L11 Mars UFO.

/*
[mst41] (Use mst33) L11 Mars UFO
prm = 0:39:			; Mêmes prm que Mst34.

[mst34] (Use mst33) L11 Mars Eye
Type = 0:3: MarsEyeTIE - MarsEye2 -
Drop = 4:7: Nothing - H_Machinegun - Gas_Tank -
SeqNo = 8:11:			; No de la séquence.
Offset = 12:15:			; Offset.
Order = 16:19:			; N° d'ordre dans la séquence (pour trouver le leader).
Shot Pattern = 20:23: One_By_One - All_Together	; Type de tir : Un par un - Tous ensemble.
Shot 1st Delay = 24:27:		; Délai avant le premier tir (* 16 frames).
Shot Delay = 28:31:		; Délai entre les tirs (* 16 frames).
Shot Nb = 32:35:		; Nb de tirs.
prm = 36:39:			; tmp
*/

// Phases.
enum
{
	e_Mst41_Arrival = 0,
	e_Mst41_GoAway,
};

// Structure spécifique. !!! Attention, MST41 utilise la même struct !!!
struct SMst34_L11MarsEye0
{
	struct SMstPath	sPath;
	u8	nType;			// Type des monstres.
	u8	nDrop;			// Item lachée par le dernier monstre de la séquence.
	u8	nSeqNo;			// N° de la séquence.
	u8	nOrder;			// N° d'ordre dans la séquence.
	u8	nHitCnt, nLife;

	s8	nShotAnm;
	u8	nShotCnt;		// Compteur pour lancer le tir.
	u8	nShotFreq;		// Frequence des tirs.
	u8	nShotNb;		// Nb de tirs restants.

	u8	nScrollXY;		// 0 = Scroll Y / 1 = Scroll X.
};

/*
// Structure spécifique. !!! On utilise la même struct que MST34 !!!
struct SMst34_L11MarsEye0
{
	s32	nPathPosX, nPathPosY;	// Pour recherche des chemins.
	u8	nType;			// Type des monstres.
	u8	nDrop;			// Item lachée par le dernier monstre de la séquence.
	u8	nSeqNo;			// N° de la séquence.
	u8	nOrder;			// N° d'ordre dans la séquence.
	u8	nNewAngle;		// Nouvel angle à atteindre quand on tourne.
	s8	nSensRot;		// Sens de rotation.
	u8	nHitCnt, nLife;
	s16 nSpdGoal;		// Pour accélérations.

	s8	nShotAnm;
	u8	nShotCnt;		// Compteur pour lancer le tir.
	u8	nShotFreq;		// Frequence des tirs.
	u8	nShotNb;		// Nb de tirs restants.

	u8	nScrollXY;		// 0 = Scroll Y / 1 = Scroll X.
};
*/

extern u8 gpnMst34ItmTb[];

void Mst41_Init_L11MarsUFO0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst34_L11MarsEye0	*pSpe = (struct SMst34_L11MarsEye0 *)pMst->pData;
	s32	nVal;

	nVal = GetBits(4, 7, pData, 0);		// Drop item.
	pSpe->nDrop = gpnMst34ItmTb[nVal];
//pSpe->nDrop = gnItmTb[2];//2;	//test test test test
	nVal = GetBits(8, 11, pData, 0);
	pSpe->nSeqNo = nVal;	// N° de séquence.
	nVal = GetBits(16, 19, pData, 0);
	pSpe->nOrder = nVal;	// N° d'ordre dans la séquence.

// Note : Si on veut un peu varier la cible, on pourrait utiliser 'offset' qui ne sert à rien pour ce monstre.

	// Bits leader.
	gMstMisc.pSeqLeader[pSpe->nSeqNo] |= 1 << pSpe->nOrder;	// Pour recherche des leaders.

	// Misc.
	pMst->nPhase = e_Mst41_Arrival;
	pSpe->nHitCnt = 0;
	pSpe->nLife = 4;

	// Prm de tir.
	pSpe->nShotAnm = -1;
	nVal = GetBits(24, 27, pData, 0);	// First delay.
	pSpe->nShotCnt = (4 + nVal) << 4;	// + 4 pour laisser le temps aux soucoupes d'arriver.
	nVal = GetBits(28, 31, pData, 0);	// Fréquence.
	pSpe->nShotFreq = nVal << 4;
	nVal = GetBits(32, 35, pData, 0);	// Nb de tirs.
	pSpe->nShotNb = nVal;

	nVal = GetBits(36, 36, pData, 0);	// Scroll type, pour drop item.
	pSpe->nScrollXY = nVal;

}

u32 Mst34_41_Common(struct SMstCommon *pMst);

#define	MST41_ACC		0x20
#define	MST41_SPD_MAX	0x600
s32 Mst41_Main_L11MarsUFO0(struct SMstCommon *pMst)
{
	struct SMst34_L11MarsEye0	*pSpe = (struct SMst34_L11MarsEye0 *)pMst->pData;

	// Destination (x,y) en fonction du n° d'ordre de la séquence. 0 = Right / 1 = Top / 2 = Left. L'ordre sert pour la phase "Go Away".
	static s32	gpnDstX[] = { ((SCR_Width/2) + 48) << 8, (SCR_Width/2) << 8, ((SCR_Width/2) - 48) << 8 };
	static s32	gpnDstY[] = { ((SCR_Height/4) + 32) << 8, (SCR_Height/4) << 8, ((SCR_Height/4) + 32) << 8 };
	s32	nIncX, nIncY;

	switch (pMst->nPhase)
	{
	case e_Mst41_Arrival:
		// Avance vers la destination finale.
		nIncX = ((gScrollPos.nPosX + gpnDstX[pSpe->nOrder]) - pMst->nPosX) / 16;
		nIncY = ((gScrollPos.nPosY + gpnDstY[pSpe->nOrder]) - pMst->nPosY) / 16;
		pMst->nPosX += nIncX;
		pMst->nPosY += nIncY;
		// Tirs terminés ?
		if (pSpe->nShotNb == 0)
		{
			// On repart sur un angle de 90° orienté dans le sens d'arrivée.
			pMst->nAngle = ((rand() & 63) - 32) + (pSpe->nOrder * 64);
			pMst->nPhase = e_Mst41_GoAway;
			pMst->nSpd = 0;
		}
		break;

	case e_Mst41_GoAway:
		// Accélération.
		if (pMst->nSpd < MST41_SPD_MAX) pMst->nSpd += MST41_ACC;
		// Déplacement du monstre.
		nIncX = (gVar.pCos[pMst->nAngle] * pMst->nSpd) >> 8;
		nIncY = (gVar.pSin[pMst->nAngle] * pMst->nSpd) >> 8;
		pMst->nPosX += nIncX;
		pMst->nPosY += nIncY;
		// Sortie de l'écran ?
		if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY))
		{
			gMstMisc.pSeqCount2[pSpe->nSeqNo]--;
			if (pSpe->nShotAnm != -1) AnmReleaseSlot(pSpe->nShotAnm);	// Libère le slot de tir.
			return (e_MstState_Dead);
		}
		break;
	}

	// Affichage, tir, contact, etc...
	return (Mst34_41_Common(pMst));

}

//=============================================================================
// Mst 42 : Porte à détruire.

/*
[mst42] Door to destroy
Type = 0:3: L3 Door - L14 Door - L6 Door -
Use itm = 8:15:			; N° item de quête à utiliser pour stocker l'état.
Give itm = 16:23:		; N° item de quête à donner une fois la porte cassée.
*/

// Phases.
enum
{
	e_Mst42_Wait = 0,
	e_Mst42_Destroyed,
};

// Structure spécifique.
struct SMst42_DoorToDestroy0
{
	u8	nType;			// Type du monstre.
	u8	nItemUse;		// Item utilisée pour sauvegarde de l'état. On garde le 'state' sur les bits 5>7 et la vie sur les bits 0>4.
	u8	nItemGive;		// Item à donner (0 si rien).
	u8	nState;			// Etat.

	u8	nHitCnt, nLife;

};

// Principe : Il y a toujours un spr bg, mais pas forcément fg.
u8	gpnM42StateMax[] = { 3, 1, 1, };	// State max utilisé, pas nb d'états total (évite le -1 à chaque fois).
u32	gpnM42SprBgTb[] = { e_Spr_Lev3_DoorR, e_Spr_Lev14_DoorR, e_Spr_Lev6_Door2, };
u32	gpnM42SprFgTb[] = { e_Spr_Lev3_DoorR_fg-3, e_Spr_Lev14_DoorR_fg-1, SPR_NoSprite, };
u8	gpnM42SprFgStates[] = { 0x08, 0x02, 0, };		// Bits pour indiquer si fg ou pas selon le 'state'.
u8	gpnM42BlockDist[] = { 0x20, 0x2C, 0, }; // Distance de blocage (<< 8).

#define	MST42_LIFE	8
void Mst42_Init_DoorToDestroy0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst42_DoorToDestroy0	*pSpe = (struct SMst42_DoorToDestroy0 *)pMst->pData;
	s32	nVal;

	nVal = GetBits(0, 3, pData, 0);
	pSpe->nType = nVal;			// Type du monstre.
	nVal = GetBits(8, 15, pData, 0);
	pSpe->nItemUse = nVal;		// Item utilisée pour sauvegarde de l'état.
	nVal = GetBits(16, 23, pData, 0);
	pSpe->nItemGive = nVal;		// Item à donner (0 si rien).

	// Valeurs par défaut.
	pSpe->nState = 0;
	pSpe->nLife = MST42_LIFE;
	// Si déjà entamée, on restaure les paramètres.
	if (gpMstQuestItems[pSpe->nItemUse])
	{
		pSpe->nState = gpMstQuestItems[pSpe->nItemUse] >> 5;
		pSpe->nLife = gpMstQuestItems[pSpe->nItemUse] & 0x1F;
	}
	pSpe->nHitCnt = 0;
	if (pSpe->nState >= gpnM42StateMax[pSpe->nType]) pSpe->nState = gpnM42StateMax[pSpe->nType];
	pMst->nPhase = (pSpe->nState == gpnM42StateMax[pSpe->nType] ? e_Mst42_Destroyed : e_Mst42_Wait);

}

s32 Mst42_Main_DoorToDestroy0(struct SMstCommon *pMst)
{
	struct SMst42_DoorToDestroy0	*pSpe = (struct SMst42_DoorToDestroy0 *)pMst->pData;
	u32	nSpr, nSprFg;

	// Screen out ?
	if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY))
	{
		gpMstQuestItems[pSpe->nItemUse] = (pSpe->nState << 5) | pSpe->nLife;		// MAJ quest item avant de quitter.
		return (e_MstState_Asleep);
	}

	// Spr bg et fg.
	nSpr = gpnM42SprBgTb[pSpe->nType] + pSpe->nState;
	nSprFg = gpnM42SprFgTb[pSpe->nType] + pSpe->nState;
	if ((gpnM42SprFgStates[pSpe->nType] & (1 << pSpe->nState)) == 0) nSprFg = SPR_NoSprite;

	switch (pMst->nPhase)
	{
	case e_Mst42_Wait:			// Porte présente, empèche le joueur de passer + encaisse les tirs.
		// Bloque le joueur.
//		Mst_PlayerBlock(pMst->nPosX - (2 << 12), 0);
		if (gpnM42BlockDist[pSpe->nType]) Mst_PlayerBlock(pMst->nPosX - ((u32)gpnM42BlockDist[pSpe->nType] << 8), 0);

		// Se prend un tir ?
		if (Mst_ShotCheckLife(nSpr, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife))
		{
			pSpe->nHitCnt = MST_HIT_CNT;
			pSpe->nLife = MST42_LIFE;
			// Porte détruite ?
			if (++pSpe->nState == gpnM42StateMax[pSpe->nType])
			{
				pSpe->nHitCnt = 0;
				gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;	// Score.
				pMst->nPhase = e_Mst42_Destroyed;
				// Donne une item si nécessaire.
				if (pSpe->nItemGive) gpMstQuestItems[pSpe->nItemGive] = 1;
			}
		}
		break;

	case e_Mst42_Destroyed:		// Porte détruite.
		break;
	}

	// Affichage.
	if (pSpe->nHitCnt) pSpe->nHitCnt--;
	// Affichage Bg.
	SprDisplay(nSpr | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies - 1);
	// Affichage Fg.
	if (nSprFg != SPR_NoSprite)
		SprDisplay(nSprFg | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_HUD - 2);	// Mst 2 (Fg sprite) à une priorité de e_Prio_HUD-1.

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 43 : L02 Flying Tara.

extern u8 gpnMst34ItmTb[];

/*
[mst43] Flying Tara
SeqNo = 0:3:	; No de la séquence.
Order = 4:7:	; N° d'ordre dans la séquence (pour trouver le leader).
Drop = 8:11: Nothing - H_Machinegun - Gas_Tank - Ammo_Box1	; H non utilisé.
Mode = 12:14: L02_Path - L02_To_Player - Std_Level -
prm = 15:15:			; tmp
*/

// Phases.
enum
{
	e_Mst43_Fly = 0,
	e_Mst43_Turn,
};

#define	MST43_SPEED	0x200
#define	MST43_ACC	0x08
#define	MST43_OFFSET_Y_PLYR	0x800
#define	MST43_SHOT_FREQ	32

// Structure spécifique.
struct SMst43_FlyingTara0
{
	struct SMstPath	sPath;
	u8	nHitCnt, nLife;

	u8	nSeqNo;			// N° de la séquence (Valide slt si mst généré).
	u8	nOrder;			// N° d'ordre dans la séquence (Valide slt si mst généré).
	u8	nDrop;			// Item à lacher.

	u8	nMode;			// Mode : L02_Path - L02_To_Player - Std_level
	u8	nBodyBomb;		// Porte-t-on une bombe ?
	s8	nShotAnm;
	u8	nShotCnt;

};

void Mst43_Init_FlyingTara0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst43_FlyingTara0	*pSpe = (struct SMst43_FlyingTara0 *)pMst->pData;
	s32	nVal;

	Mst_PathInit(pMst, &pSpe->sPath, MST43_SPEED, 128);	// Init path.

	pMst->nPhase = e_Mst43_Fly;
	pSpe->nHitCnt = 0;
	pSpe->nLife = 12;
	pSpe->nShotAnm = -1;	// Slot de tir.

	nVal = GetBits(0, 3, pData, 0);		// No de la séquence (Valide slt si mst généré).
	pSpe->nSeqNo = nVal;
	nVal = GetBits(4, 7, pData, 0);		// N° d'ordre dans la séquence (Valide slt si mst généré).
	pSpe->nOrder = nVal;

	nVal = GetBits(8, 11, pData, 0);	// Item à lacher.
	pSpe->nDrop = gpnMst34ItmTb[nVal];

	nVal = GetBits(12, 14, pData, 0);	// Mode.
	pSpe->nMode = nVal;
	pSpe->nBodyBomb = 0;
	pSpe->nShotCnt = MST43_SHOT_FREQ;

	switch (pSpe->nMode)
	{
	case 0:		// L02_Path
	case 1:		// L02_To_Player
		{
		// Replacement par rapport à la fenêtre d'affichage (qui ne bouge pas).
		s32	nP1PosX, nP1PosY;
		Scroll_HeroPlaneScrollPosXY_Get(&nP1PosX, &nP1PosY);
		pMst->nPosX += gScrollPos.nPosX - nP1PosX;
		pMst->nPosY += gScrollPos.nPosY - nP1PosY;
		}
		break;

	case 2:		// Std_level
	default:
		pSpe->nBodyBomb = 1;
		if (pMst->nFlipX) pMst->nSpd = 0x300;
//		pSpe->sPath.nSpdGoal = pMst->nSpd;
		break;
	}

	if (pMst->nAngle + 64 < 128) pMst->nPosX -= (SCR_Width << 8) + ((MST_CLIP_VAL * 2) << 12);

}

s32 Mst43_Main_FlyingTara0(struct SMstCommon *pMst)
{
	struct SMst43_FlyingTara0	*pSpe = (struct SMst43_FlyingTara0 *)pMst->pData;
	u8	nClearBodyBomb = 0;

//Font_Print(10 + ((pMst->nLoadIdx & 0x0F) * 20),200, "m43", 0);

	// Sortie de l'écran ?
	if (pSpe->nMode != 2 || pSpe->nBodyBomb == 0)	// Pour le mode niveaux normaux, on ne le tue pas avant qu'il ait laché sa bombe.
	if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY))
	{
		if (pMst->nLoadIdx == -1) gMstMisc.pSeqCount2[pSpe->nSeqNo]--;
		if (pSpe->nShotAnm != -1) AnmReleaseSlot(pSpe->nShotAnm);	// Libère le slot de tir.
		return (e_MstState_Dead);
	}

	switch (pMst->nPhase)
	{
	case e_Mst43_Fly:

		// Lacher de bombe dans les levels normaux.
		if (pSpe->nMode == 2 && pSpe->nBodyBomb)
		{
			if ((pMst->nFlipX == 0 && pMst->nPosX < gScrollPos.nPosX + ((2 * SCR_Width / 3) << 8)) ||
				(pMst->nFlipX == 1 && pMst->nPosX > gScrollPos.nPosX + ((1 * SCR_Width / 3) << 8)))
			{
/*
[mst45] F-Tara Bomb (DO NOT USE)
Direction = 0:0: Left - Right
tmp = 1:7:
*/
				u32	nData = pMst->nFlipX;
				MstAdd(e_Mst45_FlyingTaraBomb0, pMst->nPosX >> 8, (pMst->nPosY >> 8) + 4, (u8 *)&nData, -1);

				nClearBodyBomb = 1;
				pSpe->sPath.nSensRot = (pMst->nFlipX - 1) | 1;
				pSpe->sPath.nNewAngle = pMst->nAngle + (pSpe->sPath.nSensRot << 5);
				pMst->nPhase = e_Mst43_Turn;
			}
		}
		break;

	case e_Mst43_Turn:
		{
			if (pMst->nAngle == pSpe->sPath.nNewAngle)
			{
				pMst->nPhase = e_Mst43_Fly;
				break;
			}
			pMst->nAngle += pSpe->sPath.nSensRot;
		}
		break;
	}

	// * Déplacement.
	s32	nPosX_sav;
	nPosX_sav = pMst->nPosX;
	switch (pSpe->nMode)
	{
	case 0:		// L02_Path
		// Déplacement sur path.
		Mst_PathMove(pMst, &pSpe->sPath, MST43_ACC, e_Mst43_Turn);
		break;

	case 1:		// L02_To_Player
		{
			s32	nDiff;
			u8	nTargetAng;
			s8	nSensRot;

			nTargetAng = (pMst->nFlipX ? 0 : 128);		// Angle par défaut.
			nDiff = (gShoot.nPlayerPosX - pMst->nPosX) >> 8;
			if (SGN(nDiff) == SGN((pMst->nFlipX - 1) | 1))	// Si on est bien devant le héros.
			{
				if (MST_SHOT_COND)	// Si le héros n'est pas mort...
				{
					nDiff = ((gShoot.nPlayerPosY - MST43_OFFSET_Y_PLYR) - pMst->nPosY) >> 8;
					if (ABS(nDiff) > 32) nDiff = SGN(nDiff) * 32;
					nTargetAng = (pMst->nFlipX ? 0 - nDiff : 128 + nDiff);
				}
			}
			else
			{
				// On a dépassé le joueur, accélération.
				if (ABS(pMst->nSpd) < MST43_SPEED + 0x100)
					pMst->nSpd += SGN(pMst->nSpd) * MST43_ACC;
			}

			// Vers angle cible.
			if (pMst->nAngle != nTargetAng)
			{
				nSensRot = ((nTargetAng - pMst->nAngle) & 0x80 ? -1 : 1);
				pMst->nAngle += nSensRot;
			}
			// Déplacement y.
			pMst->nPosY += (gVar.pSin[pMst->nAngle] * pMst->nSpd) >> 8;
		}
		break;

	case 2:		// Std_level
		pMst->nPosY += (gVar.pSin[pMst->nAngle] * pMst->nSpd) >> 8;
		if ((gnFrame & 7) == 0) Sfx_PlaySfx(e_Sfx_Tank1, e_SfxPrio_30);		// Sfx (seulement sur les niveaux normaux).
		break;

	default:
		break;
	}
	// En X, déplacement à vitesse constante.
	pMst->nPosX = nPosX_sav + (pMst->nFlipX ? pMst->nSpd : -pMst->nSpd);

	// Sprite.
	u32	nSpr;
	nSpr = e_Spr_FlyingTara_Flying + 2;
	// Inclinaison de l'avion en fonction de l'angle.
	s32 nAdd = pMst->nAngle;
	if (nAdd & 0x80) nAdd = -nAdd;
	if (nAdd & 0x40) nAdd = -nAdd;
	nAdd &= 0x3F;
	if (nAdd > 32) nAdd = 32;
	if (pMst->nAngle < 128) nAdd = -nAdd;
	nAdd >>= 4;
	nSpr += nAdd;
	nSpr ^= (pMst->nFlipX ? SPR_Flip_X : 0);

	// Se prend un tir ?
	if (Mst_ShotCheckLife(nSpr, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife))
	{
		gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;	// Score.
_43FlyingTaraDeath:
		// Différents dusts.
		DustSet(gAnm_Explosion0_Big_Dust, pMst->nPosX, pMst->nPosY + (5 << 8), e_Prio_DustUnder, 0);
		DustSetMvt(gAnm_FlyingTara_DebrisEngine_Dust, pMst->nPosX + ((pMst->nFlipX ? 20 : -20) << 8), pMst->nPosY, (pMst->nFlipX ? 0x180 : -0x180), -0x120, e_Prio_DustUnder - 1, e_DustFlag_Gravity | (pMst->nFlipX ? e_DustFlag_FlipX : 0));
		DustSetMvt(gAnm_FlyingTara_DebrisWing_Dust, pMst->nPosX, pMst->nPosY, (pMst->nFlipX ? 0x080 : -0x080), 0, e_Prio_DustUnder - 1, e_DustFlag_Gravity | (pMst->nFlipX ? e_DustFlag_FlipX : 0));
		DustSetMvt(gAnm_FlyingTara_DebrisBody1_Dust, pMst->nPosX + ((pMst->nFlipX ? -10 : 10) << 8), pMst->nPosY, (pMst->nFlipX ? -0x080 : 0x080), -0x200, e_Prio_DustUnder - 1, e_DustFlag_Gravity | (pMst->nFlipX ? e_DustFlag_FlipX : 0));
		DustSetMvt(gAnm_FlyingTara_DebrisBody2_Dust, pMst->nPosX + ((pMst->nFlipX ? -20 : 20) << 8), pMst->nPosY, (pMst->nFlipX ? -0x200 : 0x200), -0x100, e_Prio_DustUnder - 1, e_DustFlag_Gravity | (pMst->nFlipX ? e_DustFlag_FlipX : 0));

		u8	nDrop = 0;	// Item à lacher. Par défaut non.
		// Si mst généré, gestion de la séquence.
		if (pMst->nLoadIdx == -1)
		{
			// Cleare le bit de lead.
			gMstMisc.pSeqLeader[pSpe->nSeqNo] &= ~(1 << pSpe->nOrder);
			// Mort.
			gMstMisc.pSeqCount2[pSpe->nSeqNo]--;
			// Décrémente la séquence pour drop.
			if (--gMstMisc.pSeqCount[pSpe->nSeqNo] == 0) nDrop = pSpe->nDrop;
		}
		else
			nDrop = pSpe->nDrop;	// Pas une séquence.

		if (nDrop)
		{
			// Largage d'une item quand le dernier monstre de la séquence explose.
/*
[mst4] Weapon Capsule
Type = 0:3: S_Shotgun - H_Machinegun - F_Flamethrower - R_Rocket - Gas_Tank - Ammo_Box1
Boxed = 4:4: No - Yes		; Une caisse contient l'item ?
Location = 5:6: Normal - Space - Underwater - Air
*/
			u32	nData;
			nData = (nDrop - 1) | (3 << 5);
			MstAdd(e_Mst4_WeaponCapsule, pMst->nPosX >> 8, pMst->nPosY >> 8, (u8 *)&nData, -1);
		}

		if (pSpe->nShotAnm != -1) AnmReleaseSlot(pSpe->nShotAnm);	// Libère le slot de tir.
		return (e_MstState_Dead);
	}

	// Contact avec le joueur ?
	struct SSprRect sRect1;
	if (SprGetRect(nSpr, e_SprRectZone_RectCol, &sRect1))
	if (sRect1.nType == e_SprRect_Rect)
	if (Enemy_PlayerBlowRect(&sRect1, pMst->nPosX, pMst->nPosY, MST_DAMAGE_EXPLO)) goto _43FlyingTaraDeath;

	// Déclenchement du tir frontal ?
	if (pSpe->nMode != 2)	// Pas dans les niveaux normaux.
	if (pSpe->nShotAnm == -1)
	{
		if (pSpe->nShotCnt) pSpe->nShotCnt--;
		if (MST_SHOT_COND)	// Si le héros n'est pas mort...
		if (pSpe->nShotCnt == 0)
		{
			s32	nDiff;
			// Si on est bien devant le héros en x.
			nDiff = (gShoot.nPlayerPosX - pMst->nPosX) >> 8;
			if (SGN(nDiff) == SGN((pMst->nFlipX - 1) | 1))
			{
				// Si on est bien au niveau du héros en y, tir.
				nDiff = ((gShoot.nPlayerPosY - MST43_OFFSET_Y_PLYR) - pMst->nPosY) >> 8;
				if (ABS((gShoot.nPlayerPosY - MST43_OFFSET_Y_PLYR) - pMst->nPosY) < 0x800)
					pSpe->nShotAnm = AnmSet(gAnm_FlyingTara_ShotFx, -1);
			}
		}
	}

	// Gestion du tir frontal.
	u32	nSprShot = SPR_NoSprite;
	if (pSpe->nShotAnm != -1)
	{
		nSprShot = AnmGetImage(pSpe->nShotAnm);
		nSprShot ^= (pMst->nFlipX ? SPR_Flip_X : 0);
		// Offset du tir.
		u32	nShotOffsX = 0, nShotOffsY = 0;
		if (SprGetRect(nSpr, e_SprRectZone_ShotOrg, &sRect1))	// nSpr déjà flippé.
		if (sRect1.nType == e_SprRect_Point)
		{
			nShotOffsX = sRect1.nX1;
			nShotOffsY = sRect1.nY1;
		}
		// Fin d'anim ?
		if (AnmCheckEnd(pSpe->nShotAnm))
		{
			AnmReleaseSlot(pSpe->nShotAnm);
			pSpe->nShotAnm = -1;
			nSprShot = SPR_NoSprite;
		}
		// Tir ?
		else if (AnmCheckStepFlag(pSpe->nShotAnm))
		{
			FireAdd(e_Shot_Enemy_RebSoldier_Bullet0, pMst->nPosX + (nShotOffsX << 8), pMst->nPosY + (nShotOffsY << 8), (pMst->nFlipX ? 0 : 128));
			// Reinitialisation du tir.
			pSpe->nShotCnt = MST43_SHOT_FREQ;
		}
	}

	// * Affichage.
	if (pSpe->nHitCnt) pSpe->nHitCnt--;
	// Affichage corps.
	SprDisplay(nSpr | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies + gnMstPrio);
	// Si nécessaire, affichage de la bombe sous le corps de l'avion.
	if (pSpe->nBodyBomb) SprDisplay((nSpr + 5) | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies+1 + gnMstPrio);
	// Affichage hélice.
	nSpr = e_Spr_FlyingTara_Propeller + 1 + ((gnFrame >> 2) & 7);
	if (gnFrame & 2) nSpr = e_Spr_FlyingTara_Propeller;
	SprDisplay((nSpr | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0)) ^ (pMst->nFlipX ? SPR_Flip_X : 0), (pMst->nPosX >> 8) + (pMst->nFlipX ? 30 : -30), pMst->nPosY >> 8, e_Prio_Ennemies + gnMstPrio + 1);
	// Affichage de l'éclat du tir.
	if (nSprShot != SPR_NoSprite)
		SprDisplay(nSprShot | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), (pMst->nPosX >> 8) + (pMst->nFlipX ? 30 : -30), (pMst->nPosY >> 8), e_Prio_Ennemies + 2 + gnMstPrio);

	// Clear body bomb ? (Car le monstre peut n'apparaitre qu'à la prochaine frame, suivant sa position dans la liste).
	if (nClearBodyBomb) pSpe->nBodyBomb = 0;

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 45 : Bombe du Flying Tara.

/*
[mst45] F-Tara Bomb (DO NOT USE)
Direction = 0:0: Left - Right
tmp = 1:7:
*/

// Phases.
enum
{
	e_Mst45_Fall = 0,
	e_Mst45_Explode,
};

#define	MST45_CNT	8

// Structure spécifique.
struct SMst45_FlyingTaraBomb0
{
	u8	nCnt;		// Pour lancer les explosions toutes les x frames.
	u8	nExploNb;	// Nb d'explosions à générer.
};

void Mst45_Init_FlyingTaraBomb0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst45_FlyingTaraBomb0	*pSpe = (struct SMst45_FlyingTaraBomb0 *)pMst->pData;
	s32	nVal;

	pMst->nPhase = e_Mst45_Fall;

	pSpe->nCnt = 1;		// Pour lancer les explosions toutes les x frames.
	pSpe->nExploNb = 4;	// Nb d'explosions.

	nVal = GetBits(0, 0, pData, 0);		// No de la séquence (Valide slt si mst généré).
	pMst->nFlipX = nVal;

}

s32 Mst45_Main_FlyingTaraBomb0(struct SMstCommon *pMst)
{
	struct SMst45_FlyingTaraBomb0	*pSpe = (struct SMst45_FlyingTaraBomb0 *)pMst->pData;
	u32	nSpr;
	u32	nAng;

	switch (pMst->nPhase)
	{
	case e_Mst45_Fall:
		// Déplacement.
		pMst->nPosX += (pMst->nFlipX ? 0x200 : -0x200);

		// Affichage de la bombe, angle en fct de la vitesse de chute.
		nSpr = e_Spr_FlyingTara_BodyBomb;
		nAng = (pMst->nSpdY * 8) / 256;
		SprDisplayRotoZoom(nSpr | (pMst->nFlipX ? SPR_Flip_X : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies, nAng, 0x100);// + gnMstPrio);

		// Arrive au sol ?
		pMst->nPosY += 0x800;
		Mst_Gravity(pMst);
		if (pMst->nGround == 0)
		{
			pMst->nPosY -= 0x800;
			return (e_MstState_Managed);
		}
		// Oui, phase suivante.
		pMst->nPhase = e_Mst45_Explode;
		// !! Pas de break, tir lancé immédiatement !!
	case e_Mst45_Explode:
		// Sortie de l'écran ?
		if (pMst->nPosY >> 8 >= (gMap.pPlanesHt[gMap.nHeroPlane] << 4) - 1 ||
			(u32)(pMst->nPosY >> 8) >= gMap.pPlanesLg[gMap.nHeroPlane] << 4)
			return (e_MstState_Dead);

		// Une explosion ?
		if (--pSpe->nCnt == 0)
		{
			// Explosion.
			FireAdd(e_Shot_Enemy_RShobu_Bomb_Explo, pMst->nPosX, pMst->nPosY, -1);
			// Dec.
			if (--pSpe->nExploNb == 0) return (e_MstState_Dead);
			// Déplacement pour la prochaine explosion.
			s32	nHt;
			pMst->nPosX += (pMst->nFlipX ? 24 : -24) << 8;
			nHt = BlockGetGroundLevel(pMst->nPosX >> 8, pMst->nPosY >> 8);
			pMst->nPosY += nHt << 8;
			// Reinit cnt.
			pSpe->nCnt = MST45_CNT;
		}
		break;
	}

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 44 : Image du slug pour transitions entre les niveaux.
// Dédié au sous-marin pour le moment, on pourrait étendre ça à d'autres véhicules.

u32 LvlDataSave_GetVehicle(void);

void Mst44_Init_SlugWaitTransit0(struct SMstCommon *pMst, u8 *pData)
{
	// Teste si le perso est arrivé dans le niveau en slug.
	pMst->nPhase = (LvlDataSave_GetVehicle() == e_HeroVehicle_Submarine ? 1 : 0);
	pMst->nAngle = 1;	// Utilisé pour l'incrément de l'oscillation. On commence à 1 pour sauter les 256.

}

s32 Mst44_Main_SlugWaitTransit0(struct SMstCommon *pMst)
{
	// Si on n'est pas arrivé en slug, on quitte.
	if (pMst->nPhase == 0) return (e_MstState_Dead);
	// Sortie de l'écran ?
	if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Dead);

	u32	nSpr;
	s32	nPosY;

	// Affichage du slug.
	nSpr = e_Spr_SlugSubmarine_WaitOpen;
	nPosY = (gShoot.nSplashLevel == -1 ? pMst->nPosY : (gShoot.nSplashLevel - 8 - 4) << 8);
	pMst->nAngle += 2;//3;
	SprDisplay(nSpr, pMst->nPosX >> 8, (nPosY >> 8) + (gVar.pCos[pMst->nAngle] >> 7)+1, e_Prio_Ennemies - 2);
	// Affichage du splash.
	SprDisplay(e_Spr_SlugSubmarine_WaitOpenSplash + ((gnFrame >> 3) & 3), pMst->nPosX >> 8, nPosY >> 8, e_Prio_Ennemies - 1);

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 46 : Hair Buster Ribert (Half-boss).

/*
[mst46] Hair Buster Ribert (Half-boss)
prm = 0:7:			; tmp
*/

#define	MST46_LIFE	30//2//30
#define	MST46_DECAY_MAX	3

// Phases.
enum
{
	e_Mst46_ComeDistance = 0,
	e_Mst46_ComeIn,
	e_Mst46_LRAC1,
	e_Mst46_LRAC2,
	e_Mst46_GoUp,
	e_Mst46_DroppingBombs,
	e_Mst46_FinalFall,
};

// Sous structure pour le petit LRAC soldier.
struct SMst46_Sub0
{
	u8	nPhase;
	u8	nAngle;		// Angle de la visée.
	u8	nDisp;		// On affiche le soldat (1) ou pas (0).
	s8	nAnm;
	u8	nCntWait;
};

void Mst46_Sub0_Init(struct SMst46_Sub0 *pSub0);

// Structure spécifique.
struct SMst46_HairBusterRibert0
{
	u8	nDecay;
	u8	nLife;
	u8	nHitCnt;

	s8	nAnmExhaust;
	u8	nPropeller;

	u8	nExplosions;
	u8	nSinIdx;		// Pour oscillation y.
	u8	nDir;			// Direction pendant la phase de lacher de bombes.

	u8	nShotFrm;
	u8	nShotWait;

	struct SMst46_Sub0	sSub0;
};

void Mst46_Init_HairBusterRibert0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst46_HairBusterRibert0	*pSpe = (struct SMst46_HairBusterRibert0 *)pMst->pData;

	pSpe->nAnmExhaust = AnmSet(gAnm_HairBusterRibert_HeatExhaust, -1);
	pSpe->nPropeller = 0;

	pSpe->nDecay = 0;
	pSpe->nLife = MST46_LIFE;
	pSpe->nHitCnt = 0;

	pSpe->nExplosions = 0;
	pSpe->nSinIdx = 1;		// 1, car on avance de 2 en 2, et comme ça on saute les 256.

	pSpe->nDir = 0;
	pSpe->nShotFrm = -1;
	pSpe->nShotWait = 0;

	pMst->nPhase = e_Mst46_ComeDistance;
	pMst->nPosX = gScrollPos.nPosX - (48 << 8);

	Mst46_Sub0_Init(&pSpe->sSub0);

	gpMstQuestItems[MST_QUEST_ITEM_MST32]++;	// Blocage du scroll.

}

//===========================
// Sous-routine du petit LRAC soldier qui sort par la trappe.

enum
{
	e_Mst46Sub0_Aim = 0,
	e_Mst46Sub0_Shot,
	e_Mst46Sub0_DiveInit,
	e_Mst46Sub0_Dive,
	e_Mst46Sub0_End,
	e_Mst46Sub0_JumpInit,
};

void Mst46_Sub0_Init(struct SMst46_Sub0 *pSub0)
{
	pSub0->nPhase = e_Mst46Sub0_Aim;
	pSub0->nDisp = 0;
	pSub0->nAnm = AnmSet(gAnm_RebSoldier_LRAC_Idle, -1);
	pSub0->nCntWait = (rand() & 127) | 63;//31;

}

extern	u32 gpnM14SprAim[];	// Sprite de visée du rabel soldier LRAC en fonction de l'angle (32 valeurs).

// struct pour passage de prm.
struct SM46SubPrm
{
	struct SMstCommon	*pMst;
	s32	nOffsX, nOffsY;
	struct SMst46_Sub0	*pSub0;
	u8	nExplosions;
	u8	nDecay;
	u8	nDecayMax;
	u8	nPhaseEnd;
};

// Fct externalisée pour M46 (Ribert) et M49 (Suspended Sub).
void Mst_SubLRAC0_Main(struct SM46SubPrm *pPrm)
{
	u32	nSpr = SPR_NoSprite;
	u32	nAdd = 0;
	u8	nAng;

	switch (pPrm->pSub0->nPhase)
	{
	case e_Mst46Sub0_Aim:
		// Visée normale.
		nAng = fatan2(-((gShoot.nPlayerPosY + (10 << 8)) - (pPrm->pMst->nPosY + (pPrm->nOffsY << 8))), gShoot.nPlayerPosX - (pPrm->pMst->nPosX + (pPrm->nOffsX << 8)));
		pPrm->pSub0->nAngle = nAng;
		// Déclenchement du tir ?
		if (pPrm->nExplosions == 0)
		if (MST_SHOT_COND)
		if (--pPrm->pSub0->nCntWait == 0)
		{
			pPrm->pSub0->nAnm = AnmSet(gAnm_RebSoldier_LRAC_Shot, pPrm->pSub0->nAnm);
			if (pPrm->pSub0->nAnm != -1)
			{
				pPrm->pSub0->nPhase = e_Mst46Sub0_Shot;
				nAdd = AnmGetImage(pPrm->pSub0->nAnm);	// Premier GetImage pour déclenchement du tir à la même frame.
			}
			pPrm->pSub0->nCntWait = (rand() & 127) | 63;//31;
		}

		// Peur pendant les explosions.
		if (pPrm->nExplosions)
		{
			SprDisplay(e_Spr_RebSoldier_LRAC_Fright | (pPrm->pMst->nPosX + (pPrm->nOffsX << 8) < gShoot.nPlayerPosX ? SPR_Flip_X : 0),
				(pPrm->pMst->nPosX >> 8) + pPrm->nOffsX, (pPrm->pMst->nPosY >> 8) + pPrm->nOffsY, e_Prio_Ennemies + 2);
			// Decay au max ? => Action qui va bien.
			if (pPrm->nExplosions == 1 && pPrm->nDecay >= pPrm->nDecayMax)
				pPrm->pSub0->nPhase = pPrm->nPhaseEnd;
			return;
		}
		break;

	case e_Mst46Sub0_Shot:
		nAdd = AnmGetImage(pPrm->pSub0->nAnm);	// L'anim est forcément passée, sinon on n'arrive pas dans cette phase.
		if (AnmGetKey(pPrm->pSub0->nAnm) != e_AnmKey_Enemy_Shot)
		{
			pPrm->pSub0->nPhase = e_Mst46Sub0_Aim;
			nAdd = 0;
			break;
		}
		break;

	case e_Mst46Sub0_JumpInit:	// Mst 49, le soldat LRAC saute à terre => Génération d'un M14.
		{
		/*
		[mst14] Reb Soldier 0
		Type = 0:7: Rifle - Mortar - LRAC - Gun_Grenade - RESERVED - Shield
		OffsX = -8:11:			; Décalage X.
		Move = 12:12: No - Yes		; Le soldat bouge ou pas.
		From = 13:13: Right - Left	; Apparition à droite ou à gauche de l'écran.
		Parachute = 14:14: No - Yes	; Parachuté ?
		Jump = 15:16: No - Random - Always	; Saut en bout de pf.
		Org OffsX = -17:22:		; Décalage de l'origine de la zone.
		Special Init = 23:26: None - Truck Jump - SuspSub Jump - Hero Height -	; Inits spéciales.
		RESERVED2 = 27:31:
		*/
		u32	nPrm = (2) | (1 << 12) | (2 << 23);	// 2 = LRAC / 1 << 12 = Move / 1 << 23 = sub jump.
		MstAdd(e_Mst14_RebelSoldier0, (pPrm->pMst->nPosX >> 8) + pPrm->nOffsX, (pPrm->pMst->nPosY >> 8) + pPrm->nOffsY, (u8 *)&nPrm, -1);

		// Première frame de saut superposée au nouveau monstre au cas ou le monstre soit généré avant le mst en cours dans la liste.
		SprDisplay(e_Spr_RebSoldier_LRAC_Jump | ((pPrm->pMst->nPosX + (pPrm->nOffsX << 8)) - gShoot.nPlayerPosX >= 0 ? 0 : SPR_Flip_X),
			(pPrm->pMst->nPosX >> 8) + pPrm->nOffsX, (pPrm->pMst->nPosY >> 8) + pPrm->nOffsY, e_Prio_Ennemies + 2);

		pPrm->pSub0->nPhase = e_Mst46Sub0_End;
		}
		return;

	case e_Mst46Sub0_DiveInit:	// Mst 46, le soldat plonge dans le vide.
		pPrm->pSub0->nAnm = AnmSet(gAnm_HairBusterRibert_SoldierJumpDive, pPrm->pSub0->nAnm);
		pPrm->pSub0->nPhase = e_Mst46Sub0_Dive;
		// Dust du LRAC.
		gDustExg.nRotInc = -6;
		DustSetMvt(gAnm_RebSoldier_WeaponLRAC_Dust, pPrm->pMst->nPosX + (pPrm->nOffsX << 8), pPrm->pMst->nPosY + (pPrm->nOffsY << 8) - 0x1000, 0x100, -0x380, e_Prio_Ennemies + 2, e_DustFlag_Gravity | e_DustFlag_Rotation | e_DustFlag_CheckGnd);
	case e_Mst46Sub0_Dive:
		nSpr = SPR_NoSprite;
		if (pPrm->pSub0->nAnm != -1) nSpr = AnmGetImage(pPrm->pSub0->nAnm);
		// Anim finie => Dust du saut. / Note, le slot sera libéré à la mort du Ribert.
		if (nSpr == SPR_NoSprite)
		{
			DustSetMvt(gAnm_HairBusterRibert_SoldierJumpDive_Dust, pPrm->pMst->nPosX + (pPrm->nOffsX << 8), pPrm->pMst->nPosY + (pPrm->nOffsY << 8), -0x180, -0x200, e_Prio_Ennemies + 2, e_DustFlag_Gravity | e_DustFlag_CheckGnd);
			pPrm->pSub0->nPhase = e_Mst46Sub0_End;
		}
		else
			SprDisplay(nSpr, (pPrm->pMst->nPosX >> 8) + pPrm->nOffsX, (pPrm->pMst->nPosY >> 8) + pPrm->nOffsY, e_Prio_Ennemies + 3);	// +2 => +3 pour le dust du LRAC.
	case e_Mst46Sub0_End:
		return;
	}

	// Affichage.
	nAng = pPrm->pSub0->nAngle;
	nAng += 8;		// 8 * 32 => On ajoute la moitié de l'angle. => 8 et pas 4, car en fait il y a 16 positions seulement, doublées pour le milieu.
	nAng >>= 3;		// /8.
	nSpr = gpnM14SprAim[nAng];
	nSpr += nAdd;

	// Déclenchement du tir ?
	if (pPrm->pSub0->nPhase == e_Mst46Sub0_Shot)	// Phase de tir ? (=> l'anim est passée, plus de test à faire).
	if (AnmCheckNewImgFlag(pPrm->pSub0->nAnm))	// Nouvelle image ?
	{
		// Est-ce qu'on a une coordonnée d'origine de tir ?
		struct SSprRect sRect1;
		if (SprGetRect(nSpr, e_SprRectZone_ShotOrg, &sRect1))
		if (sRect1.nType == e_SprRect_Point)
		{
			FireAdd(e_Shot_Enemy_RebSoldier_LRAC_Rocket0, pPrm->pMst->nPosX + (sRect1.nX1 << 8) + (pPrm->nOffsX << 8), pPrm->pMst->nPosY + (sRect1.nY1 << 8) + (pPrm->nOffsY << 8), pPrm->pSub0->nAngle);
			DustSet(gAnm_RebSoldier_LRAC_Shot_Dust, pPrm->pMst->nPosX + (sRect1.nX1 << 8) + (pPrm->nOffsX << 8), pPrm->pMst->nPosY + (sRect1.nY1 << 8) + (pPrm->nOffsY << 8), e_Prio_DustOver, (nSpr & SPR_Flip_X ? e_DustFlag_FlipX : 0));
		}
	}

	SprDisplay(nSpr, (pPrm->pMst->nPosX >> 8) + pPrm->nOffsX, (pPrm->pMst->nPosY >> 8) + pPrm->nOffsY, e_Prio_Ennemies + 2);
}

#define	MST14SUB0_OFFSX	(-34)
#define	MST14SUB0_OFFSY	(-35)
void Mst46_Sub0_Main(struct SMstCommon *pMst)
{
	struct SMst46_HairBusterRibert0	*pSpe = (struct SMst46_HairBusterRibert0 *)pMst->pData;
	struct SMst46_Sub0	*pSub0 = (struct SMst46_Sub0 *)&pSpe->sSub0;

	// Si on est sur une anim qui va bien (porte autrement que fermée).
	if (AnmGetKey(pMst->nAnm) != e_AnmKey_Enemy_Key3) return;
	// Teste le flag d'apparition/disparition du soldat.
	if (AnmCheckStepFlag(pMst->nAnm)) pSub0->nDisp ^= 1;
	if (pSub0->nDisp == 0) return;

	struct SM46SubPrm	sPrm;
	sPrm.pMst = pMst;
	sPrm.nOffsX = MST14SUB0_OFFSX;
	sPrm.nOffsY = MST14SUB0_OFFSY;
	sPrm.pSub0 = pSub0;
	sPrm.nExplosions = pSpe->nExplosions;
	sPrm.nDecay = pSpe->nDecay;
	sPrm.nDecayMax = MST46_DECAY_MAX - 1;
	sPrm.nPhaseEnd = e_Mst46Sub0_DiveInit;
	Mst_SubLRAC0_Main(&sPrm);

}

//===========================

u32 Mst_ShotCheckLifeRect2(struct SSprRect *pRect, s32 nPosX, s32 nPosY, u8 *pnHitCnt, u8 *pnLife, u32 nFlags);
void Boss2_MoveIn(struct SMstCommon *pMst, s32 nSens);

#define	HBRIBERT_PIECESNB	(3)
#define	HBRIBERT_Y_HIGH	(100)
#define	HBRIBERT_Y_LOW	(200)
#define	HBRIBERT_X_MIN	(100)
#define	HBRIBERT_X_MAX	(210)
#define	HBRIBERT_SHOTWAIT	(64)//(48)//(64)	// 64 pour touché seulement sur la pause. 48 si touché tout le temps.

// Sous-routine de déplacement y.
u32	Mst46_sub_MoveY(struct SMstCommon *pMst, s32 nDst)
{
	if (pMst->nPosY >> 8 != (gScrollPos.nPosY >> 8) + nDst)
	{
		pMst->nSpdY += (pMst->nPosY < gScrollPos.nPosY + ((HBRIBERT_Y_HIGH + ((HBRIBERT_Y_LOW - HBRIBERT_Y_HIGH) / 2)) << 8) ? 0x10 : -0x10);
		pMst->nPosY += pMst->nSpdY;
		return (1);
	}
	return (0);
}

s32 Mst46_Main_HairBusterRibert0(struct SMstCommon *pMst)
{
	static u32	gpHBRibertSpr[][HBRIBERT_PIECESNB] =
	{
		{ e_Spr_Ribert_P0_0, e_Spr_Ribert_P1_012, e_Spr_Ribert_P2_0 },
		{ e_Spr_Ribert_P0_1, e_Spr_Ribert_P1_012, e_Spr_Ribert_P2_1 },
		{ e_Spr_Ribert_P0_2, e_Spr_Ribert_P1_012, e_Spr_Ribert_P2_2 },
		{ e_Spr_Ribert_P0_3, e_Spr_Ribert_P1_3, e_Spr_Ribert_P2_3 },
	};
	static s8	gpHBRibertOffsX[HBRIBERT_PIECESNB] = { -70,-40,0 };	// Offsets x des sprites.

	struct SMst46_HairBusterRibert0	*pSpe = (struct SMst46_HairBusterRibert0 *)pMst->pData;
	u32	i;
	u32	nTouch = 0;

	switch (pMst->nPhase)
	{
	case e_Mst46_ComeDistance:	// L'avion traverse l'écran au loin.
		// Display.
		SprDisplay(e_Spr_Ribert_InTheDistance + ((gnFrame >> 2) & 1), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_EnnemiesBg);
		// Avance.
		pMst->nPosX += 0x100;
		if (pMst->nPosX > gScrollPos.nPosX + ((SCR_Width + 48) << 8))
		{
//			gpMstQuestItems[MST_QUEST_ITEM_MST32]++;	// Blocage du scroll.
			pMst->nPhase = e_Mst46_ComeIn; // Phase suivante.
			//
			pMst->nPosY = gScrollPos.nPosY + (HBRIBERT_Y_HIGH << 8);	// Pos haute.
			pMst->nPosX = gScrollPos.nPosX + ((SCR_Width + 64+32) << 8);
			pMst->nSpd = 0x780;		// Vitesse empirique pour l'arrivée.
		}
		return (e_MstState_Managed);
		//break;

	case e_Mst46_ComeIn:	// Arrivée depuis la droite.
		Boss2_MoveIn(pMst, -1);
		if (pMst->nPosX <= gScrollPos.nPosX + (210 << 8))
		{
			pMst->nPhase = e_Mst46_LRAC1;
			pMst->nSpdY = 0;
			AnmSetIfNew(gAnm_HairBusterRibert_DoorOpening, pMst->nAnm);
		}
		break;

	case e_Mst46_LRAC1:		// Avion en position haute.
		nTouch = !pSpe->nExplosions;	// Prise en compte des touchés, sauf pendant les explosions.
		nTouch &= pSpe->sSub0.nDisp;	// Pour attendre que le soldat soit sorti.

		// Passage en phase suivante ?
		if (pSpe->nDecay == 1 && pSpe->nExplosions == 0)
		{
			pMst->nPhase = e_Mst46_LRAC2;
			pMst->nSpdY = 0;
		}
		break;

	case e_Mst46_LRAC2:		// Avion en position basse.
		nTouch = !pSpe->nExplosions;	// Prise en compte des touchés, sauf pendant les explosions.
		if (Mst46_sub_MoveY(pMst, HBRIBERT_Y_LOW))	// Placement en position basse.
		{
			nTouch = 0;						// Pas de touché pendant le déplacement.
			pSpe->sSub0.nCntWait = 16;		// Et on empêche le soldat de tirer.
		}
		// Passage en phase suivante ?
		if (pSpe->nDecay == 2 && pSpe->nExplosions == 0)
		{
			pMst->nPhase = e_Mst46_GoUp;
			AnmSetIfNew(gAnm_HairBusterRibert_DoorClosing, pMst->nAnm);
			// Dust claquement de porte, avec wait intégré.
			DustSet(gAnm_HairBusterRibert_DoorClose_Dust, pMst->nPosX - (40 << 8), pMst->nPosY - (40 << 8), e_Prio_DustOver, 0);
		}
		break;

	case e_Mst46_GoUp:	// Repasse en position haute.
//		nTouch = !pSpe->nExplosions;	// Prise en compte des touchés, sauf pendant les explosions.
		if (AnmCheckEnd(pMst->nAnm) == 0) break;	// Attente que la porte soit fermée.
		// Placement en position haute.
		if (Mst46_sub_MoveY(pMst, HBRIBERT_Y_HIGH) == 0)
		{
			pMst->nPhase = e_Mst46_DroppingBombs;
			pSpe->nDir = 0;			// Direction pendant la phase de lacher de bombes.
			pSpe->nShotWait = 0;
			pSpe->nShotFrm = -1;
		}
		break;

	case e_Mst46_DroppingBombs:	// Lâcher de mini bombes.
//		nTouch = !pSpe->nExplosions;	// Prise en compte des touchés, sauf pendant les explosions.

		// Phase d'attente.
		if (pSpe->nShotWait)
		{
			nTouch = 1;				// On ne touche que pendant la phase de pause.
			pSpe->nShotWait--;
			pSpe->nShotFrm = -1;	// Pour tirer à la frame à laquelle on redémarre.
			break;
		}

		// Déplacement (Gauche, droite, bas, haut, loop).
		static s16	gpnMst46AddX[] = { -0x100, 0x100, 0, 0 };
		static s16	gpnMst46AddY[] = { 0, 0, 0x100, -0x100 };
		pMst->nPosX += (s32)gpnMst46AddX[pSpe->nDir];
		pMst->nPosY += (s32)gpnMst46AddY[pSpe->nDir];

		switch (pSpe->nDir)
		{
		case 0:	// Gauche.
			if (pMst->nPosX <= gScrollPos.nPosX + (HBRIBERT_X_MIN << 8)) pSpe->nDir++;
			break;
		case 1:	// Droite.
			if (pMst->nPosX >= gScrollPos.nPosX + (HBRIBERT_X_MAX << 8))
			{
				pSpe->nDir++;
				pSpe->nShotWait = HBRIBERT_SHOTWAIT;
			}
			break;
		case 2:	// Bas.
			if (pMst->nPosY >= gScrollPos.nPosY + (HBRIBERT_Y_LOW << 8)) pSpe->nDir++;
			break;
		case 3:	// Haut.
			if (pMst->nPosY <= gScrollPos.nPosY + (HBRIBERT_Y_HIGH << 8))
			{
				pSpe->nDir = 0;
				pSpe->nShotWait = HBRIBERT_SHOTWAIT;
			}
			break;
		}

		// Lâcher de mini bombes.
		if ((++pSpe->nShotFrm & 15) == 0)
			FireAdd(e_Shot_Enemy_HairBusterRibert_Mine, pMst->nPosX - (60 << 8), pMst->nPosY + (3 << 8), 144 - 8 + (rand() & 15));
		break;

	case e_Mst46_FinalFall:		// Chute finale.
		// Fumée derrière.
		if ((gnFrame & 15) == 0)
			DustSetMvt(gAnm_RebSoldier_LRAC_Shot_Dust, pMst->nPosX - (60 << 8), pMst->nPosY - (24 << 8), -0x200, (pMst->nSpdY / 2), e_Prio_DustUnder, 0);
		// On attend la fin des explosions.
		if (pSpe->nExplosions) break;
		// Chute.
		if (pMst->nSpdY < 0x280) pMst->nSpdY += 0x08;
		pMst->nPosY += pMst->nSpdY;
		if (pMst->nPosY > gScrollPos.nPosY + ((SCR_Height + 96) << 8))
		{
			// Disparition.
			if (pSpe->nAnmExhaust != -1) AnmReleaseSlot(pSpe->nAnmExhaust);
			if (pSpe->sSub0.nAnm != -1) AnmReleaseSlot(pSpe->sSub0.nAnm);
			gpMstQuestItems[MST_QUEST_ITEM_MST32]--;	// Déblocage du scroll.
			return (e_MstState_Dead);
		}
		break;
	}

	if (pMst->nPhase >= e_Mst46_ComeIn && pMst->nPhase <= e_Mst46_DroppingBombs)
		ChaserTarget_AddToList(pMst->nPosX >> 8, (pMst->nPosY >> 8) - 16);	// Ajout dans la table de visée des missiles.

	// Petite oscillation sur les phases "fixes". (Pas pendant les explosions).
	s32 nPosY_sav = pMst->nPosY;
	pMst->nPosY += (s32)gVar.pSin[pSpe->nSinIdx] << 1;
	if (pMst->nPhase > e_Mst46_ComeIn && pMst->nPhase < e_Mst46_FinalFall && pSpe->nExplosions == 0) pSpe->nSinIdx += 2;

	// Les rectangles de col (x,y,lg,ht).
	static s16 pRectCol[12] = { -14,-80,50,88,  -78,-59,64,58,  36,-56,34,59 };
	struct SSprRect sRect1;
	for (i = 0; i < 3; i++)
	{
		// On créé un rectangle de collision.
		sRect1.nType = e_SprRect_Rect;
		sRect1.nX1 = pRectCol[(i * 4)];
		sRect1.nY1 = pRectCol[(i * 4) + 1];
		sRect1.nX2 = sRect1.nX1 + pRectCol[(i * 4) + 2];
		sRect1.nY2 = sRect1.nY1 + pRectCol[(i * 4) + 3];

		// Se prend un tir ?
		if (nTouch)
		{
			// Test des tirs.
			if (Mst_ShotCheckLifeRect2(&sRect1, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife, e_ShotCheck_MinimizedDamage))
			{
				pSpe->nLife = MST46_LIFE;		// Reset energy.
				pSpe->nExplosions = 67 + MEDIUMEXPLO_ANM_DURATION;
				nTouch = 0;
				// Mort ?
				if (++pSpe->nDecay >= MST46_DECAY_MAX)
				{
					pMst->nSpdY = 0;
					pMst->nPhase = e_Mst46_FinalFall;
					FireRemoveFromList((1<<31) | e_ShotOrg_Enemy);		// Retire les tirs ennemis.
					gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;	// Score.
				}
			}
		}
		else
		{
			// Interception des tirs sans dégats.
			u8	nHitCnt, nLife;
			nLife = 0;
			Mst_ShotCheckLifeRect2(&sRect1, pMst->nPosX, pMst->nPosY, &nHitCnt, &nLife, 0);
		}

		// Contact avec le joueur ?
		Enemy_PlayerBlowRect(&sRect1, pMst->nPosX, pMst->nPosY, MST_DAMAGE_EXPLO);
	}

	// Gestion du soldat LRAC.
	AnmGetImage(pMst->nAnm);		// GetImage sur la porte pour affichage/disparition du soldat à la bonne frame.
	Mst46_Sub0_Main(pMst);

	// Explosions ?
	if (pSpe->nExplosions)
	{
		pSpe->nExplosions--;
		if (pSpe->nExplosions > MEDIUMEXPLO_ANM_DURATION && (pSpe->nExplosions & 0x7) == 0)
		{
			i = rand();
			DustSet(gAnm_Explosion0_Medium_Dust, pMst->nPosX + (pSpe->nExplosions & 8 ? -64 << 8 : 0) + ((i & 63) << 8), pMst->nPosY - (40 << 8) + (((i >> 8) & 63) << 8), e_Prio_Ennemies+3, 0);

//if (((pSpe->sFrontC[i].nExplo >> 4) + i) & 1)	// + Débris 1 fois sur 2.
//DustSetMvt(gAnm_Debris_Metal0_Dust, x, y, -0x180, -0x380, e_Prio_Ennemies-1, e_DustFlag_Gravity);

		}
	}

	// Affichage des différentes parties du boss.
	if (pSpe->nHitCnt) pSpe->nHitCnt--;
	u32	nFlagHit = (pSpe->nHitCnt ? SPR_Flag_HitPal : 0);
	u32	nSpr, k;
	k = AnmGetLastImage(pMst->nAnm);		// Anim de la porte arrière.
	for (i = 0; i < HBRIBERT_PIECESNB; i++)
	{
		nSpr = gpHBRibertSpr[pSpe->nDecay][i];
		if (i == 1 && k != SPR_NoSprite) nSpr = k;		// Partie centrale / porte.
		SprDisplay(nSpr | nFlagHit, (pMst->nPosX >> 8) + (s32)gpHBRibertOffsX[i], (pMst->nPosY >> 8), e_Prio_Ennemies);
	}
	// Hélice.
	if (pMst->nPhase == e_Mst46_FinalFall)
	{	// En chute.
		pSpe->nPropeller++;
		pSpe->nPropeller &= 0xF;
		i = pSpe->nPropeller;
	}
	else
	{	// Cas normal.
		pSpe->nPropeller &= 0xF;
		i = pSpe->nPropeller;
		if (gnFrame & 2) i = 17;
		if ((gnFrame & 3) == 3) pSpe->nPropeller += 3;
	}
	SprDisplay((e_Spr_Ribert_EnginePropeller + i) | nFlagHit, (pMst->nPosX >> 8) - 7, (pMst->nPosY >> 8) - 70, e_Prio_Ennemies+1);
	// Exhaust.
	if (pSpe->nAnmExhaust != -1)
	SprDisplay(AnmGetImage(pSpe->nAnmExhaust) | nFlagHit, (pMst->nPosX >> 8) + 13, (pMst->nPosY >> 8) - 77, e_Prio_Ennemies+1);

	pMst->nPosY = nPosY_sav;		// Restore org posy.
	return (e_MstState_Managed);
}

//=============================================================================
// Mst 47 : Lev9 - Chapeau de la fusée.

/*
[mst47] L09 Rocket Top
Wait itm = 0:7:			; N° item de quête attendu pour démarrer.
Give itm = 8:15:		; N° item de quête à donner une fois explosé.
*/

u32 MstOnScreenNb(u32 nMstType, s32 nBlkOffset);

// Phases.
enum
{
	e_Mst47_Idle = 0,
	e_Mst47_Explode,
};

// Structure spécifique.
struct SMst47_Lev9RocketTop0
{
	u8	nHitCnt, nLife;
	u8	nItemWait;
	u8	nItemGive;

	s8	nOpenOffs;		// Pour l'ouverture.
	u8	nOpenCnt;
	u8	nExplo;
	u16	nNbSoldiers;
};

#define	MST47_LIFE	40
//#define	M47_SOLDIERS	40
void Mst47_Init_Lev9RocketTop0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst47_Lev9RocketTop0	*pSpe = (struct SMst47_Lev9RocketTop0 *)pMst->pData;
	s32	nVal;

	nVal = GetBits(0, 7, pData, 0);
	pSpe->nItemWait = nVal;		// Item attendue.
	nVal = GetBits(8, 15, pData, 0);
	pSpe->nItemGive = nVal;		// Item à donner (0 si rien).

	pSpe->nOpenOffs = 0;
	pSpe->nOpenCnt = 0;

	pSpe->nHitCnt = 0;
	pSpe->nLife = MST47_LIFE;
//	pSpe->nNbSoldiers = M47_SOLDIERS;
//	pSpe->nNbSoldiers = (M47_LIFE + 1) * 4;
	pSpe->nNbSoldiers = (MST47_LIFE + 1) * 2;

	pMst->nPhase = e_Mst47_Idle;

}

void Boss_Explosions(u8 *pnExplo, s32 nPosX, s32 nPosY, s32 nWidth2n, s32 nHeight2n);

#define	M47_PIECES_NB	5
s32 Mst47_Main_Lev9RocketTop0(struct SMstCommon *pMst)
{
	struct SMst47_Lev9RocketTop0	*pSpe = (struct SMst47_Lev9RocketTop0 *)pMst->pData;

	static	u32	gnSprTb[M47_PIECES_NB] = { e_Spr_Lev9_RocketTopL2, e_Spr_Lev9_RocketTopL1, e_Spr_Lev9_RocketTopL0, e_Spr_Lev9_RocketTopR2, e_Spr_Lev9_RocketTopR1 };
	static	u32	gnOffsY[M47_PIECES_NB] = { 0, -90, -150, 0, -90 };

	u32	i;
	u32	nFlag;

	// Screen out ?
	if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);

	// Bloque le joueur.
	Mst_PlayerBlock(pMst->nPosX - (5 << 12), 0);

	switch (pMst->nPhase)
	{
	case e_Mst47_Idle:		// Encaisse les coups.
		{
			struct SSprRect sRect1;
			// On créé un rectangle de collision.
			sRect1.nType = e_SprRect_Rect;
			sRect1.nX1 = -62;
			sRect1.nX2 = 62;
			sRect1.nY1 = -120;
			sRect1.nY2 = 0;

			// Au cas où, interception des tirs sans dégats. (Si on tirait sur le chapeau sans avoir avancé suffisament).
			if (gpMstQuestItems[pSpe->nItemWait] == 0)
			{
				u8	nHitCnt, nLife;
				nLife = 0;
				Mst_ShotCheckLifeRect2(&sRect1, pMst->nPosX, pMst->nPosY, &nHitCnt, &nLife, 0);
				break;
			}

			// Se prend un tir ?
			if (Mst_ShotCheckLifeRect2(&sRect1, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife, e_ShotCheck_MinimizedDamage))
			{
				gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;	// Score.
				pMst->nPhase = e_Mst47_Explode;
				pSpe->nExplo = 100;
				// Donne l'item qui va bien (qui fait apparaitre une partie du sol).
				if (pSpe->nItemGive) gpMstQuestItems[pSpe->nItemGive] = 1;
			}

			// Le chapeau s'entre ouvre à la moitié de la vie.
			if (pSpe->nLife < MST47_LIFE/2)
			if (pSpe->nOpenOffs < 3)
			if ((++pSpe->nOpenCnt & 7) == 0) pSpe->nOpenOffs++;

			// Génération des parachutistes.
			if (pSpe->nNbSoldiers)
			if ((gnFrame & 31) == 0)
			if (pSpe->nLife <= MST47_LIFE / 2 || pSpe->nNbSoldiers > MST47_LIFE)	// La moitié des soldats sort sur la première moitié de la vie. Le reste ensuite.
			if (MstOnScreenNb(e_Mst14_RebelSoldier0, 1) < 3)	// Pas plus de 3 à l'écran.
			{
				/*
				[mst14] Reb Soldier 0
				Type = 0:7: Rifle - Mortar - LRAC - Gun_Grenade - RESERVED - Shield
				OffsX = -8:11:			; Décalage X.
				Move = 12:12: No - Yes		; Le soldat bouge ou pas.
				From = 13:13: Right - Left	; Apparition à droite ou à gauche de l'écran.
				Parachute = 14:14: No - Yes	; Parachuté ?
				Jump = 15:16: No - Random - Always	; Saut en bout de pf.
				Org OffsX = -17:22:		; Décalage de l'origine de la zone.
				Special Init = 23:26: None - Truck Jump - SuspSub Jump - Hero Height -	; Inits spéciales.
				RESERVED2 = 27:31:
				*/
				u32	nPrm = (2) | (1 << 12) | (1 << 14);		// 2 = LRAC / 1 << 12 = Move / 1 << 14 = Parachute.
				MstAdd(e_Mst14_RebelSoldier0, (pMst->nPosX >> 8) - ((7 + (rand()& 7)) * 16), (gScrollPos.nPosY >> 8), (u8 *)&nPrm, -1);
				pSpe->nNbSoldiers--;
			}

		}
		break;

	case e_Mst47_Explode:	// Explose !
		// Petites explosions.
		Boss_Explosions(&pSpe->nExplo, pMst->nPosX - (64 << 8), pMst->nPosY - (112 << 8), 128, 128);
		pSpe->nExplo++;
		Boss_Explosions(&pSpe->nExplo, pMst->nPosX - (64 << 8), pMst->nPosY - (112 << 8), 128, 128);
		// Débris.
		if ((pSpe->nExplo & 7) == 0)
		{
			s32	nPosX, nPosY;
			i = rand();
			nPosX = pMst->nPosX + ((-64 + (i & 127)) << 8);
			i += 64;
			nPosY = pMst->nPosY + ((-112 + (i & 127)) << 8);
			i &= 127;
			DustSetMvt(gAnm_Debris_Metal0_Dust, nPosX, nPosY, 0x180 + i, -0x480, e_Prio_Ennemies + 32, e_DustFlag_Gravity);
			DustSetMvt(gAnm_Debris_Metal0_Dust, nPosX, nPosY, -0x180 - i, -0x380, e_Prio_Ennemies + 32, e_DustFlag_Gravity);
		}

		// Grosses explosions.
		if (pSpe->nExplo == MEDIUMEXPLO_ANM_DURATION - 16)
		{
			// Dusts.
			DustSet(gAnm_Explosion0_Big_Dust, pMst->nPosX, pMst->nPosY, e_Prio_DustOver, 0);
			DustSet(gAnm_Explosion0_Big_Dust, pMst->nPosX - 0x2000, pMst->nPosY - 0x4000, e_Prio_DustOver+1, 0);
			DustSet(gAnm_Explosion0_Big_Dust, pMst->nPosX + 0x1800, pMst->nPosY - 0x6000, e_Prio_DustOver+2, 0);
			// Génération du slug rocket. (A cause de la priorité de la flèche "in").
			/*
			[mst9] Slug
			Type = 0:3: Submarine - Tank - Helicopter - Helicopter_No_Exit - Rocket
			State = 4:7 : Wait - Dies(reserved)
			Energy(192) = 8:15:
			Vulcan = 16:23:			; Angle du vulcan cannon.
			Bombs(50) = 24:31:
			Slug Angle(0) = 32:39:		; Angle du slug (Tank).
			*/
			u8	pTb9[5] = { 4, SLUG_ENERGY_MAX, 0, SLUG_BOMBS_DEFAULT, 0 };
			MstAdd(e_Mst9_Slug, (pMst->nPosX >> 8), (pMst->nPosY >> 8) - 32, pTb9, -1);
		}

		if (pSpe->nExplo <= MEDIUMEXPLO_ANM_DURATION - 16) pSpe->nOpenOffs++;

		// Fin.
		if (pSpe->nExplo <= MEDIUMEXPLO_ANM_DURATION - 32)
			return (e_MstState_Asleep);
		break;
	}

	// Affichage.
	if (pSpe->nHitCnt) pSpe->nHitCnt--;
	nFlag = (pSpe->nHitCnt ? SPR_Flag_HitPal : 0);
	for (i = 0; i < M47_PIECES_NB; i++)
	{
		SprDisplay(gnSprTb[i] | nFlag, (pMst->nPosX >> 8) + (i <= 2 ? -(s32)pSpe->nOpenOffs : (s32)pSpe->nOpenOffs), (pMst->nPosY >> 8) + gnOffsY[i], e_Prio_Ennemies + (i <= 2 ? 0 : 1));
	}

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 48 : Monstre pour petites blagues, les monstres uniques, etc...

/*
[mst48] Jokes
Type = 0:6: Rebel Sunbath - L11 Mothership - L2 Clouds - L6 Main Door - L6 Computer - L8 Wagon Door - L17 Monoeye - L14 Generator - L8 Biker - L7 Rock Slug Sub -
Flip X = 7:7: No - Yes
prm0 = 8:15:
prm1 = 16:23:
*/

//==
#define	M48T2_NB	3

// Structure spécifique.
struct SMst48_Joke0
{
	u8	nType;
	u8	nHitCnt, nLife;

// Note : Si on manque de place, faire des unions ou faire pointer une sous structure sur une table data.
	u8	nCloudsY[M48T2_NB];
	u8	nItemWait;
	u8	nItemGive;
	u8	nCnt;
	u8	nCnt2;

};

//== L7 Rocher à détruire qui cache un slug sous-marin. + modif pour cabane du L16.

#define	M48_T9_LIFE	15
#define	M48_T9_TYPE_MAX	2

u32	gpnM48_T9_Spr[M48_T9_TYPE_MAX] = { e_Spr_Lev7_RockBottom, e_Spr_Lev16_ToolShed, };
u32	gpnM48_T9_SprPf[M48_T9_TYPE_MAX] = { e_Spr_Lev7_RockBottom_pf, e_Spr_Lev16_ToolShed_pf, };
s8	gpnM48_T9_SlugOffsY[M48_T9_TYPE_MAX] = { -16, -1, };
s8	gpnM48_T9_OffsExplo[M48_T9_TYPE_MAX] = { -8, 0, };
u64	*gpM48_T9_Dust[][M48_T9_TYPE_MAX] =
{
	{ gAnm_Debris_RockSmall0_Dust, gAnm_Debris_RockMedium0_Dust, },
	{ gAnm_Debris_Metal0_Dust, gAnm_Debris_Metal0_Dust, },
};

// Init.
void Mst48Sub_Init_L7RockBottom0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst48_Joke0	*pSpe = (struct SMst48_Joke0 *)pMst->pData;
	s32	nVal;

	nVal = GetBits(8, 15, pData, 0);		// Sous-type.
	pSpe->nItemWait = nVal;
	nVal = GetBits(16, 23, pData, 0);		// Slug n°.
	pSpe->nItemGive = nVal;

	// Hard sprite.
//	pSpe->nCnt = HardSpr_AddToList(e_Spr_Lev7_RockBottom_pf, pMst->nPosX >> 8, pMst->nPosY >> 8, e_HardSpr_ShotsIgnore);
	pSpe->nCnt = HardSpr_AddToList(gpnM48_T9_SprPf[pSpe->nItemWait], pMst->nPosX >> 8, pMst->nPosY >> 8, e_HardSpr_ShotsIgnore);

	pSpe->nHitCnt = 0;
	pSpe->nLife = M48_T9_LIFE;
	pSpe->nCnt2 = M48_T9_LIFE;

}

// Main.
s32 Mst48Sub_Main_L7RockBottom0(struct SMstCommon *pMst)
{
	struct SMst48_Joke0	*pSpe = (struct SMst48_Joke0 *)pMst->pData;
	u32	nSpr;

	// Screen out ?
	if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY))
	{
		// Libère le hard sprite.
		if ((s8)pSpe->nCnt != -1) HardSpr_ReleaseSlot(pSpe->nCnt);
		return (e_MstState_Asleep);
	}

	nSpr = gpnM48_T9_Spr[pSpe->nItemWait];

	// Se prend des tirs ?
//	if (Mst_ShotCheckLife(e_Spr_Lev7_RockBottom, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife))
	if (Mst_ShotCheckLife(nSpr, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife))
	{
		// Explosion.
//		DustSet(gAnm_Explosion0_Big_Dust, pMst->nPosX, (pMst->nPosY) - 0x800, e_Prio_Ennemies + 1, 0);
		DustSet(gAnm_Explosion0_Big_Dust, pMst->nPosX, (pMst->nPosY) + ((s32)gpnM48_T9_OffsExplo[pSpe->nItemWait] << 8), e_Prio_Ennemies + 1, 0);
		// Débris sur la cabane.
//		if (pSpe->nItemWait == 1)	// nItemWait = Type.
		{
//			DustSetMvt(gAnm_Debris_Metal0_Dust, pMst->nPosX, pMst->nPosY - 0x200, 0x180, -0x480, e_Prio_DustUnder - 1, e_DustFlag_Gravity);
//			DustSetMvt(gAnm_Debris_Metal0_Dust, pMst->nPosX, pMst->nPosY - 0x200, -0x180, -0x380, e_Prio_DustUnder - 1, e_DustFlag_Gravity);
			s32	nPosY = pMst->nPosY - 0x1000 + ((s32)gpnM48_T9_OffsExplo[pSpe->nItemWait] << 8);
			DustSetMvt(gpM48_T9_Dust[pSpe->nItemWait][0], pMst->nPosX, nPosY, 0x200, -0x480, e_Prio_DustUnder - 1, e_DustFlag_Gravity);
			DustSetMvt(gpM48_T9_Dust[pSpe->nItemWait][1], pMst->nPosX, nPosY, -0x200, -0x380, e_Prio_DustUnder - 1, e_DustFlag_Gravity);
		}

		// Libère le hard sprite.
		if ((s8)pSpe->nCnt != -1) HardSpr_ReleaseSlot(pSpe->nCnt);
//		// Genère le slug sous-marin.
		// Genère le slug qui va bien.
		/*
		[mst9] Slug
		Type = 0:3: Submarine - Tank - Helicopter - Helicopter_No_Exit - Rocket
		State = 4:7 : Wait - Dies(reserved)
		Energy(192) = 8:15:
		Vulcan = 16:23:			; Angle du vulcan cannon.
		Bombs(50) = 24:31:
		Slug Angle(0) = 32:39:		; Angle du slug (Tank).
		*/
		u8	pTb[5] = { 0, SLUG_ENERGY_MAX, 0, SLUG_BOMBS_DEFAULT, 0 };
		pTb[0] = pSpe->nItemGive & 0x0F;
//		MstAdd(e_Mst9_Slug, (pMst->nPosX >> 8), (pMst->nPosY >> 8) - 16, pTb, -1);
		MstAdd(e_Mst9_Slug, (pMst->nPosX >> 8), (pMst->nPosY >> 8) + (s32)gpnM48_T9_SlugOffsY[pSpe->nItemWait], pTb, -1);

		//
		return (e_MstState_Dead);
	}


/*
	// Génération des soldats.
//	if (ABS(gShoot.nPlayerPosX - pMst->nPosX) < ((2 * SCR_Width) / 3) << 8)		// Pas top...
	if (pSpe->nLife < M48_T9_LIFE)	// On attend le premier tir.
	if (pSpe->nItemWait == 1)	// nItemWait = Type.
	if (pSpe->nCnt2)
	if ((gnFrame & 63) == 0)
	if (pSpe->nLife <= M48_T9_LIFE / 2 || pSpe->nCnt2 > M48_T9_LIFE / 2)	// La moitié des soldats sort sur la première moitié de la vie. Le reste ensuite.
	if (MstOnScreenNb(e_Mst14_RebelSoldier0, 1) < 3)	// Pas plus de 3 à l'écran.
	{
		/ *
		[mst14] Reb Soldier 0
		Type = 0:7: Rifle - Mortar - LRAC - Gun_Grenade - RESERVED - Shield
		OffsX = -8:11:			; Décalage X.
		Move = 12:12: No - Yes		; Le soldat bouge ou pas.
		From = 13:13: Right - Left	; Apparition à droite ou à gauche de l'écran.
		Parachute = 14:14: No - Yes	; Parachuté ?
		Jump = 15:16: No - Random - Always	; Saut en bout de pf.
		Org OffsX = -17:22:		; Décalage de l'origine de la zone.
		Special Init = 23:26: None - Truck Jump - SuspSub Jump - Hero Height -	; Inits spéciales.
		RESERVED2 = 27:31:
		* /
		static u8 pM14[] = { 3, 3, 0, 1 };	// Pour générer des soldats, sauf LRAC.
		u32	nPrm = ((u32)pM14[rand() & 3]) | (1 << 12) | (10 << 17);	// 0 = Rifle - 1 = Mortar - 3 = Grenade / 1 << 12 = Move / 10 << 17 = Décalage de la zone.
		MstAdd(e_Mst14_RebelSoldier0, (gScrollPos.nPosX >> 8) - 32, pMst->nPosY >> 8, (u8 *)&nPrm, -1);
		pSpe->nCnt2--;
	}
*/


	// Affichage.
	if (pSpe->nHitCnt) pSpe->nHitCnt--;
//	SprDisplay((e_Spr_Lev7_RockBottom + ((gnFrame >> 4) & 7)) | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies - 1);
	if (pSpe->nItemWait == 0) nSpr += ((gnFrame >> 4) & 7);	// Anim du rocher sous-marin.
	SprDisplay(nSpr | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies - 1);

	return (e_MstState_Managed);
}


//== L8 Rebel Biker qui sort du wagon.

enum
{
	e_Mst48T8_Wait = 0,
	e_Mst48T8_Opening,
	e_Mst48T8_Move,
	e_Mst48T8_Move2,
};

// Init.
void Mst48Sub_Init_L8RebelBiker0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst48_Joke0	*pSpe = (struct SMst48_Joke0 *)pMst->pData;
	s32	nVal;

	nVal = GetBits(8, 15, pData, 0);		// Item qui déclenche l'ouverture.
	pSpe->nItemWait = nVal;

	// Sauvegarde de la pos originale pour la porte du wagon. !!! Attention, 8 bits !!!
	pSpe->nCloudsY[0] = pMst->nPosX >> 12;
	pSpe->nCloudsY[1] = pMst->nPosY >> 12;

	AnmSet(gAnm_RebSoldier_Biker_Idle, pMst->nAnm);	// (Slot réservé, pas de pb).
	pMst->nFlipX = 1;
	pMst->nPhase = e_Mst48T8_Wait;

}

// Main.
s32 Mst48Sub_Main_L8RebelBiker0(struct SMstCommon *pMst)
{
	struct SMst48_Joke0	*pSpe = (struct SMst48_Joke0 *)pMst->pData;
	u8	nCache = 0;
	s8	nSfx = -1;

	switch (pMst->nPhase)
	{
	case e_Mst48T8_Wait:	// Attente du déclenchement.
		// Screen out ?
		if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);
		// Déclenchement ?
		if (gpMstQuestItems[pSpe->nItemWait] == 0) return (e_MstState_Managed);
		// Oui.
		pMst->nPhase = e_Mst48T8_Opening;
		pSpe->nCnt = 96;
		//break;

	case e_Mst48T8_Opening:		// Petite attente au début de l'ouverture.
		if (--pSpe->nCnt == 0)
		{
			pMst->nPhase = e_Mst48T8_Move;
			pMst->nSpd = 0;
		}
		nCache = 1;
		break;

	case e_Mst48T8_Move:		// Déplacement.
		nSfx = e_Sfx_Tank0;
//		nSfx = e_Sfx_Tank1;
		AnmSetIfNew(gAnm_RebSoldier_Biker_Drive, pMst->nAnm);

		if (pMst->nSpd < 0x200) pMst->nSpd += 0x20;
		pMst->nPosX += pMst->nSpd;

		if (AnmGetKey(pMst->nAnm) == e_AnmKey_Null)
		{	// Avance dans le wagon + déclenchement de la chute.
			if (pMst->nPosX >> 8 >= ((u32)pSpe->nCloudsY[0] << 4) + 40)
			{
				AnmSet(gAnm_RebSoldier_Biker_Jump, pMst->nAnm);
				pMst->nSpdY = 0;
			}
			nCache = 1;
		}
		else
		{	// Chute.
			pMst->nSpdY += GRAVITY/2;
			if (pMst->nSpdY > SPDY_MAX/2) pMst->nSpdY = SPDY_MAX/2;
			pMst->nPosY += pMst->nSpdY;
			// Atterrissage ? (En dur, comme un saguoin).
			if (pMst->nPosY >= (gMap.pPlanesHt[gMap.nHeroPlane] << 12) - (16 << 8))
			{
				pMst->nPosY = (gMap.pPlanesHt[gMap.nHeroPlane] << 12) - (16 << 8);
				AnmSetIfNew(gAnm_RebSoldier_Biker_Land, pMst->nAnm);
				pMst->nPhase = e_Mst48T8_Move2;
				pMst->nSpd = 0x100;		// Ralentissement quand on touche le sol.
				DustSet(gAnm_RebSoldier_Biker_Land_Dust, pMst->nPosX, pMst->nPosY, e_Prio_Ennemies + 1, e_DustFlag_FlipX);
			}
		}
		break;

	case e_Mst48T8_Move2:		// Déplacement au sol jusqu'à quitter l'écran.
		nSfx = e_Sfx_Tank1;
		if (pMst->nSpd < 0x400) pMst->nSpd += 0x10;
		pMst->nPosX += pMst->nSpd;
		// Screen out ?
		if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Dead);
		break;
	}

	if (nSfx != -1 && (gnFrame & 7) == 0) Sfx_PlaySfx(nSfx, e_SfxPrio_30);		// Sfx.

	// Affichage.
	u32	nSpr;
	nSpr = AnmGetImage(pMst->nAnm);
	nSpr |= (pMst->nFlipX ? SPR_Flip_X : 0);
	SprDisplay(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies);

	// Cache wagon.
	if (nCache)
		SprDisplay(e_Spr_Lev8_Cache1, ((u32)pSpe->nCloudsY[0] << 4) + 8, ((u32)pSpe->nCloudsY[1] << 4) + 15, e_Prio_Ennemies + 1);

	return (e_MstState_Managed);
}

//== L14 Générateur de monstres.

void Mst48Sub_Init_L14Generator0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst48_Joke0	*pSpe = (struct SMst48_Joke0 *)pMst->pData;
	s32	nVal;

	nVal = GetBits(8, 15, pData, 0);		// Zone.
	pSpe->nItemWait = nVal;					// Stockée ici.
	pSpe->nItemGive = (u8)(gShoot.nPlayerPosX >> 12);		// Stockée ici.
	pSpe->nCnt = 0;

}

// Main.
s32 Mst48Sub_Main_L14Generator0(struct SMstCommon *pMst)
{
	struct SMst48_Joke0	*pSpe = (struct SMst48_Joke0 *)pMst->pData;

	// Joueur dans la zone ?
	if (gShoot.nPlayerPosX > pMst->nPosX + ((u32)pSpe->nItemWait << 12)) return (e_MstState_Dead);
	if (gShoot.nPlayerPosX < pMst->nPosX) return (e_MstState_Managed);
	if (gnFrame & 31) return (e_MstState_Managed);
	// On attend le changement de bloc du joueur.
	if (pSpe->nItemGive >= (u8)(gShoot.nPlayerPosX >> 12)) return (e_MstState_Managed);
	pSpe->nItemGive = (u8)(gShoot.nPlayerPosX >> 12);
	// Et on ne les fait apparaître qu'un par un.
	if (gpnMstCount[e_Mst7_Zombie1] >= 1) return (e_MstState_Managed);

	/*
	[mst7] Gauche/Droite
	Type = 0:3: Zombie Teen - Zombie Fat - Mars People - Brain Bot -
	Zone = 4:7:
	*/
	pSpe->nCnt++;
	u32	nPrm = (2 + 0) | (10 << 4);
	MstAdd(e_Mst7_Zombie1, (gScrollPos.nPosX >> 8) + ((pSpe->nCnt & 3) == 0 ? -32 : SCR_Width + 32), pMst->nPosY >> 8, (u8 *)&nPrm, -1);

	return (e_MstState_Managed);
}

//== L17 Monoeye - Place holder.

enum
{
	e_Mst48T6_Appearing = 0,
	e_Mst48T6_Wait,
	e_Mst48T6_GoUp,
	e_Mst48T6_ToBeContinued,
	e_Mst48T6_ToBeContinued2,
};

// Init.
void Mst48Sub_Init_L17Monoeye0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst48_Joke0	*pSpe = (struct SMst48_Joke0 *)pMst->pData;

	AnmSet(gAnm_Monoeye_Appear, pMst->nAnm);	// (Slot réservé, pas de pb).
	pMst->nPhase = e_Mst48T6_Appearing;

	pMst->nPosY = gScrollPos.nPosY + (((SCR_Height / 2) + 40) << 8);
	if (gShoot.nPlayerPosX > gScrollPos.nPosX + ((SCR_Width / 2) << 8))
	{
		pMst->nPosX = gScrollPos.nPosX + ((SCR_Width / 4) << 8);
		pMst->nFlipX = 1;
	}
	else
	{
		pMst->nPosX = gScrollPos.nPosX + ((SCR_Width - (SCR_Width / 4)) << 8);
	}

	pSpe->nCnt = 1;			// Pour sin. 1 pour skipper les 256. (Step 2).
	pSpe->nItemGive = 256-32;	// Compteur pour attente.
	pMst->nSpdY = 0;

}

// Main.
s32 Mst48Sub_Main_L17Monoeye0(struct SMstCommon *pMst)
{
	struct SMst48_Joke0	*pSpe = (struct SMst48_Joke0 *)pMst->pData;
	u32	nSpr;
	u32	i;
	char	*pToBeCnt = "To be continued...";
	char	pCpy[20];

	switch (pMst->nPhase)
	{
	case e_Mst48T6_Appearing:	// Apparition.
		if (AnmGetKey(pMst->nAnm) != e_AnmKey_Enemy_Key3)	// Après l'apparition, la clef devient NULL.
			pMst->nPhase = e_Mst48T6_Wait;
		break;

	case e_Mst48T6_Wait:
		pSpe->nCnt += 2;	// Sinus.
		// Compteur pour attente.
		if (--pSpe->nItemGive == 0) pMst->nPhase = e_Mst48T6_GoUp;
		break;

	case e_Mst48T6_GoUp:	// Dégage vers le haut.
		pMst->nSpdY -= 0x10;
		pMst->nPosY += pMst->nSpdY;
		if (pMst->nPosY < -32 << 8)
		{
			pMst->nPhase = e_Mst48T6_ToBeContinued;
			// Init writer.
			pSpe->nCnt = 0;		// Nb de lettres affichées.
			pSpe->nItemGive = Font_Print(0, 0, pToBeCnt, FONT_NoDisp) / 2;	// Sz.
		}
		break;

	case e_Mst48T6_ToBeContinued:	// A suivre...
		// On affiche la partie de phrase qui va bien.
		for (i = 0; pToBeCnt[i] && i < pSpe->nCnt; i++) pCpy[i] = pToBeCnt[i];
		pCpy[i] = 0;
		Font_Print((SCR_Width / 2) - pSpe->nItemGive, 140, pCpy, 0);
		// Avance la lettre.
		if ((gnFrame & 15) == 0)
		{
			if (pToBeCnt[pSpe->nCnt])
			{	// Avance...
				pSpe->nCnt++;
				if (pToBeCnt[pSpe->nCnt] == ' ') pSpe->nCnt++;
			}
			else
			{	// Phase suivante.
				pMst->nPhase = e_Mst48T6_ToBeContinued2;
				pSpe->nCnt = 128+64+32+16;
			}
		}
		return (e_MstState_Managed);

	case e_Mst48T6_ToBeContinued2:
		if (--pSpe->nCnt == 0)
		{
			gpMstQuestItems[MST_QUEST_ITEM_NEXT_LEVEL] |= 1;	// Item de quête : Fin de niveau.
			return (e_MstState_Dead);
		}

		if (pSpe->nCnt < 64+16 && (pSpe->nCnt & 4)) return (e_MstState_Managed);	// Clignotement final.
		if (pSpe->nCnt < 32) return (e_MstState_Managed);

		Font_Print((SCR_Width / 2) - pSpe->nItemGive, 140, pToBeCnt, 0);
		return (e_MstState_Managed);
	}

	s32	nPosY_sav = pMst->nPosY;
	// Sinus sur le Y.
	pMst->nPosY += gVar.pSin[pSpe->nCnt] << 3;

	nSpr = AnmGetImage(pMst->nAnm);
	nSpr ^= (pMst->nFlipX ? SPR_Flip_X : 0);

	// Affichage.
	SprDisplay(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_DustOver - 1);

	pMst->nPosY = nPosY_sav;

	return (e_MstState_Managed);
}

//== L8 Wagon Door.

enum
{
	e_Mst48T5_Wait = 0,
	e_Mst48T5_Opening,
};

// Init.
void Mst48Sub_Init_L8WagonDoor0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst48_Joke0	*pSpe = (struct SMst48_Joke0 *)pMst->pData;
	s32	nVal;

	nVal = GetBits(8, 15, pData, 0);		// Item qui déclenche l'ouverture.
	pSpe->nItemWait = nVal;

	pMst->nSpdY = -0x280;	// vy initiale pour rebond de la porte.
	pSpe->nCnt = 0;			// Pour zoom.

	AnmSet(gAnm_Lev8_WagonDoor_Opening, pMst->nAnm);	// (Slot réservé, pas de pb).
	pMst->nPhase = e_Mst48T5_Wait;

}

// Main.
s32 Mst48Sub_Main_L8WagonDoor0(struct SMstCommon *pMst)
{
	struct SMst48_Joke0	*pSpe = (struct SMst48_Joke0 *)pMst->pData;
	u32	nSpr;

	nSpr = e_Spr_Lev8_WagonDoorOpening;		// Spr par défaut.

	switch (pMst->nPhase)
	{
	case e_Mst48T5_Wait:	// Attente du déclenchement.
		// Screen out ?
		if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);

		// Déclenchement ?
		if (gpMstQuestItems[pSpe->nItemWait] == 0) break;
		// Oui.
		pMst->nPhase = e_Mst48T5_Opening;
		//break;

	case e_Mst48T5_Opening:	// Ouverture.
		nSpr = AnmGetImage(pMst->nAnm);

		// Rebond ?
		if (AnmGetKey(pMst->nAnm) != e_AnmKey_Enemy_Key3)
		{
			// Déplacement.
			pMst->nSpdY += GRAVITY/2;
			if (pMst->nSpdY > SPDY_MAX/2) pMst->nSpdY = SPDY_MAX/2;
			pMst->nPosY += pMst->nSpdY;
			pMst->nPosX += 0x100;
			pMst->nPosX += (s32)pSpe->nCnt;
			// Out ?
			if (pMst->nPosY > gScrollPos.nPosY + (SCR_Height << 8))
				return (e_MstState_Dead);
			// Zoom.
			pSpe->nCnt += 4;
			SprDisplayZoom(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_DustOver - 1, 0x100 + pSpe->nCnt, 0x100 + pSpe->nCnt);
			return (e_MstState_Managed);
		}
		break;
	}

	// Affichage.
	SprDisplay(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_DustOver - 1);

	return (e_MstState_Managed);
}

//== L6 Computer.

enum
{
	e_Mst48T4_Active = 0,
	e_Mst48T4_Destroyed,
};

#define	M48S_L6COMPUTER_LIFE	5	// Pour que ça tienne un peu, mais que ça ne résiste pas à un coup de shotgun.

// Init.
void Mst48Sub_Init_L6Computer0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst48_Joke0	*pSpe = (struct SMst48_Joke0 *)pMst->pData;
	s32	nVal;

	nVal = GetBits(8, 15, pData, 0);		// Item utilisée pour état.
	pSpe->nItemWait = nVal;

	pMst->nPosY -= 8 << 8;		// Décalage par rapport au sol.
	pSpe->nHitCnt = 0;
	pSpe->nLife = M48S_L6COMPUTER_LIFE;

	pMst->nPhase = (gpMstQuestItems[pSpe->nItemWait] ? e_Mst48T4_Destroyed : e_Mst48T4_Active);

}

// Main.
s32 Mst48Sub_Main_L6Computer0(struct SMstCommon *pMst)
{
	struct SMst48_Joke0	*pSpe = (struct SMst48_Joke0 *)pMst->pData;
	u32	nSpr;

	// Screen out ?
	if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);

	nSpr = e_Spr_Lev6_ComputerDestroyed;

	switch (pMst->nPhase)
	{
	case e_Mst48T4_Active:
		nSpr = e_Spr_Lev6_Computer + ((gnFrame >> 1) & 1);	// spr : e_Spr_Lev6_Computer + 2,

		// Se prend des tirs ?
		if (Mst_ShotCheckLife(nSpr, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife))
		{
			DustSet(gAnm_Explosion0_Medium_Dust, pMst->nPosX, pMst->nPosY, e_Prio_Ennemies + 1, 0);
			gpMstQuestItems[pSpe->nItemWait] |= 1;
			pMst->nPhase = e_Mst48T4_Destroyed;
		}
		break;

	case e_Mst48T4_Destroyed:
		break;
	}

	// Affichage.
	if (pSpe->nHitCnt) pSpe->nHitCnt--;
	SprDisplay(nSpr | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies);

	return (e_MstState_Managed);
}

//== L6 Main Door.

enum
{
	e_Mst48T3_Down = 0,
	e_Mst48T3_Opening,
	e_Mst48T3_Up,
};

// Init.
void Mst48Sub_Init_MainDoor0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst48_Joke0	*pSpe = (struct SMst48_Joke0 *)pMst->pData;
	s32	nVal;

	nVal = GetBits(8, 15, pData, 0);		// Item utilisée.
	pSpe->nItemWait = nVal;
	nVal = GetBits(16, 23, pData, 0);		// Item à donner pour le bloqueur.
	pSpe->nItemGive = nVal;

	pMst->nPosX -= 0x800;
	pSpe->nCnt = 0;		// = Offset ouverture.

	pMst->nPhase = gpMstQuestItems[pSpe->nItemWait];


}

// Main.
#define	M48T3_HT	64
s32 Mst48Sub_Main_MainDoor0(struct SMstCommon *pMst)
{
	struct SMst48_Joke0	*pSpe = (struct SMst48_Joke0 *)pMst->pData;
	u32	i;

	// Screen out ?
	if (pMst->nPhase != e_Mst48T3_Opening)	// Sauf pendant l'ouverture.
	if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);

	switch (pMst->nPhase)
	{
	case e_Mst48T3_Down:	// Porte fermée.
		if (gpMstQuestItems[pSpe->nItemWait])
		{
			pMst->nPhase = e_Mst48T3_Opening;
			gpMstQuestItems[pSpe->nItemWait] = e_Mst48T3_Up;
		}
		break;

	case e_Mst48T3_Opening:	// Ouverture.
		if ((pSpe->nCnt & 0x1F) == 0) Sfx_PlaySfx(e_Sfx_Fx_DoorClang, e_SfxPrio_30);

		if (++pSpe->nCnt >= M48T3_HT)
		{
			pMst->nPhase = e_Mst48T3_Up;
			Sfx_StopSfx(e_Sfx_Fx_DoorClang);
		}
		// A mi-hauteur, coupe le bloqueur du héros.
		if (pSpe->nCnt == 20) gpMstQuestItems[pSpe->nItemGive] |= 1;
		break;

	case e_Mst48T3_Up:		// Porte ouverte.
		pSpe->nCnt = M48T3_HT;
		break;
	}

	// Affichage (5 parties).
	for (i = 0; (i < 5) && ((u32)pSpe->nCnt + (i << 4) < (M48T3_HT+16)); i++)
		SprDisplay(e_Spr_Lev6_HardDoor + i, pMst->nPosX >> 8, (pMst->nPosY >> 8) - (u32)pSpe->nCnt - (i << 4), e_Prio_Ennemies);

	return (e_MstState_Managed);
}

//== L2 Nuages.

// Init.
void Mst48Sub_Init_L2Clouds0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst48_Joke0	*pSpe = (struct SMst48_Joke0 *)pMst->pData;
	u32	i;

	// Init des hauteurs.
	for (i = 0; i < M48T2_NB; i++) pSpe->nCloudsY[i] = rand() & 127;
	pMst->nPosX = 0;

}

// Main.
s32 Mst48Sub_Main_L2Clouds0(struct SMstCommon *pMst)
{
	struct SMst48_Joke0	*pSpe = (struct SMst48_Joke0 *)pMst->pData;
	u32	i;
	s32	nPosX;

	// Spd.
	pMst->nPosX -= 0x180;
	pMst->nPosX &= 0x1FFFF;
	// Affichage.
	for (i = 0; i < M48T2_NB; i++)
	{
		nPosX = ((pMst->nPosX >> 8) + (i * (512 / M48T2_NB))) & 0x1FF;
		if (nPosX == 0) pSpe->nCloudsY[i] = rand() & 127;		// Réinit hauteur.
		SprDisplay(e_Spr_Lev2_Clouds + (i & 1), (gScrollPos.nPosX >> 8) - 96 + nPosX, (gScrollPos.nPosY >> 8) + 256 - (u32)pSpe->nCloudsY[i], e_Prio_EnnemiesBg + 1);
	}

	return (e_MstState_Managed);
}

//== L11 Mothership.

// Phases.
enum
{
	e_Mst48T1_Arrival = 0,
	e_Mst48T1_ForcePlayer,
	e_Mst48T1_SucksIn,
};

// Init.
void Mst48Sub_Init_Mothership0(struct SMstCommon *pMst, u8 *pData)
{
	pMst->nPosX = gScrollPos.nPosX + ((SCR_Width / 2) << 8);
	pMst->nPosY = gScrollPos.nPosY + (-40 << 8);
	pMst->nSpd = 0x3D0;		// Spd empirique pour première avance.

	AnmSet(gAnm_AlienMothershipBlink, pMst->nAnm);	// Anim qui va bien.

	gpMstQuestItems[MST_QUEST_ITEM_MST32]++;	// Scroll stop.

	pMst->nPhase = e_Mst48T1_Arrival;

}

// Main.
s32 Mst48Sub_Main_Mothership0(struct SMstCommon *pMst)
{
	struct SSprRect sRect1;

	switch (pMst->nPhase)
	{
	case e_Mst48T1_Arrival:		// Arrivée.
		// Descente du vaisseau.
		{
			s32	nTmp;

			nTmp = pMst->nPosX;
			pMst->nPosX = pMst->nPosY;
			pMst->nPosY = nTmp;
			Boss2_MoveIn(pMst, 1);		// La fct additionne la spd sur le x. => Inversion.
			nTmp = pMst->nPosX;
			pMst->nPosX = pMst->nPosY;
			pMst->nPosY = nTmp;
		}
		if (pMst->nPosY >= gScrollPos.nPosY + (48 << 8)) pMst->nPhase = e_Mst48T1_ForcePlayer;
		break;

	case e_Mst48T1_ForcePlayer:		// On force le joueur à se placer vaguement sous le trou.
		// Si le héros meurt, pause.
		if (gShoot.nDeathFlag) break;

		// Annule le déplacement x de la fusée.
		if (SGN(gShoot.nPlayerPosX - ((SCR_Width / 2) << 8)) == SGN(gShoot.nPlayerSpdX))
		{
			gShoot.nPlayerPosX -= gShoot.nPlayerSpdX;
			gShoot.nPlayerSpdX = 0;
		}
		// Force le déplacement vers le centre de l'écran en x.
		gShoot.nPlayerPosX += (gShoot.nPlayerPosX < gScrollPos.nPosX + ((SCR_Width / 2) << 8) ? 0x100 : -0x100);
		// Sous le trou ?
		if (gShoot.nPlayerPosX > ((SCR_Width / 2) - 25) << 8 && gShoot.nPlayerPosX < ((SCR_Width / 2) + 25) << 8)
		{
			gShoot.nVehicleAutoPilot = e_KbDir_Up;	// Autopilot !
			pMst->nPhase = e_Mst48T1_SucksIn;
		}
		break;

	case e_Mst48T1_SucksIn:		// Apsiration du joueur à l'intérieur.
//		gShoot.nPlayerPosX -= gShoot.nPlayerSpdX;		// Annule le déplacement x.
		// Au cas ou le héros meure pendant cette phase (ce qui ne devrait pas arriver, mais bon).
		gShoot.nVehicleAutoPilot = e_KbDir_Up;	// Autopilot !
		// Fin de niveau ?
		if (gShoot.nPlayerPosY <= pMst->nPosY)
			gpMstQuestItems[MST_QUEST_ITEM_NEXT_LEVEL] |= 1;
		break;
	}

	// Limite Y du joueur.
	if (gShoot.nVehicleAutoPilot == 0)
	if (gShoot.nPlayerPosY <= pMst->nPosY + (80 << 8))
	{
		gShoot.nPlayerPosY = pMst->nPosY + (80 << 8);
		if (gShoot.nPlayerSpdY < 0) gShoot.nPlayerSpdY = 0;
	}

	// Interception des tirs sans dégats.
	u8	nHitCnt, nLife;
	sRect1.nType = e_SprRect_Rect;
	//
	sRect1.nX1 = -160;
	sRect1.nY1 = -80;
	sRect1.nX2 = sRect1.nX1 + 92;
	sRect1.nY2 = sRect1.nY1 + 93;
	nLife = 0;
	Mst_ShotCheckLifeRect2(&sRect1, pMst->nPosX, pMst->nPosY, &nHitCnt, &nLife, 0);
	sRect1.nX1 = 160 - 102;
	sRect1.nX2 = sRect1.nX1 + 102;
	nLife = 0;
	Mst_ShotCheckLifeRect2(&sRect1, pMst->nPosX, pMst->nPosY, &nHitCnt, &nLife, 0);

	// Affichage.
	u32	nSprOffs = AnmGetImage(pMst->nAnm);
	SprDisplay(e_Spr_AlienMothership + nSprOffs, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Joueur+10);
	SprDisplay(e_Spr_AlienMothership+1 + nSprOffs, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Joueur+10);
	SprDisplay(e_Spr_AlienMothership+2 + nSprOffs, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies-1);

	return (e_MstState_Managed);
}

//== Rebel Soldier Sunbathing.

// Init.
void Mst48Sub_Init_RebelSunbath0(struct SMstCommon *pMst, u8 *pData)
{
//	struct SMst48_Joke0	*pSpe = (struct SMst48_Joke0 *)pMst->pData;
	s32	nVal;

	nVal = GetBits(7, 7, pData, 0);		// Flip X.
	pMst->nFlipX = nVal;

	AnmSet(gAnm_RebSoldier_Sunbath0_Idle, pMst->nAnm);	// Anim qui va bien.
	Enemy_GroundLevel(pMst);			// Mise au niveau du sol.

}

// Main.
s32 Mst48Sub_Main_RebelSunbath0(struct SMstCommon *pMst)
{
//	struct SMst48_Joke0	*pSpe = (struct SMst48_Joke0 *)pMst->pData;
	u32	nSpr;

	// Screen out ?
	if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);

	nSpr = AnmGetImage(pMst->nAnm);
	nSpr ^= (pMst->nFlipX ? SPR_Flip_X : 0);

	// Se prend un tir ?
	u32	nDamage;
	nDamage = FireHitCheck(nSpr, pMst->nPosX, pMst->nPosY, e_ShotOrg_Player, 0);
	nDamage |= Mst_KnifeHitCheck(pMst, nSpr);	// Comme ça, donne la priorité de la mort au tir.
	if (nDamage)
	{
		DustSet(gAnm_RebSoldier_Sunbath0_DeathStd_Dust, pMst->nPosX, pMst->nPosY, e_Prio_DustUnder, (pMst->nFlipX ? e_DustFlag_FlipX : 0));
		return (e_MstState_Dead);
	}

	// Affichage.
	SprDisplay(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies);

	return (e_MstState_Managed);
}

//== Container.

#define	MST48_NB	10

// Table de pointeurs de fonctions.
struct SMstFctTb	gpMst48Subroutines[MST48_NB] =
{
	{ Mst48Sub_Init_RebelSunbath0, Mst48Sub_Main_RebelSunbath0 },
	{ Mst48Sub_Init_Mothership0, Mst48Sub_Main_Mothership0 },
	{ Mst48Sub_Init_L2Clouds0, Mst48Sub_Main_L2Clouds0 },
	{ Mst48Sub_Init_MainDoor0, Mst48Sub_Main_MainDoor0 },
	{ Mst48Sub_Init_L6Computer0, Mst48Sub_Main_L6Computer0 },
	{ Mst48Sub_Init_L8WagonDoor0, Mst48Sub_Main_L8WagonDoor0 },
	{ Mst48Sub_Init_L17Monoeye0, Mst48Sub_Main_L17Monoeye0 },
	{ Mst48Sub_Init_L14Generator0, Mst48Sub_Main_L14Generator0 },
	{ Mst48Sub_Init_L8RebelBiker0, Mst48Sub_Main_L8RebelBiker0 },
	{ Mst48Sub_Init_L7RockBottom0, Mst48Sub_Main_L7RockBottom0 },

};

// Init.
void Mst48_Init_Joke0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst48_Joke0	*pSpe = (struct SMst48_Joke0 *)pMst->pData;
	s32	nVal;

	nVal = GetBits(0, 6, pData, 0);		// Type.
	pSpe->nType = nVal;

	pMst->nPhase = 0;

	// Inits spécifiques à chaque monstre unique.
	gpMst48Subroutines[pSpe->nType].pFctInit(pMst, pData);

}

// Main.
s32 Mst48_Main_Joke0(struct SMstCommon *pMst)
{
	struct SMst48_Joke0	*pSpe = (struct SMst48_Joke0 *)pMst->pData;

	return (gpMst48Subroutines[pSpe->nType].pFctMain(pMst));
}

//=============================================================================
// Mst 49 : Suspended Submarine (Lev 5).

/*
[mst49] Suspended Submarine
Wait itm = 0:7:			; N° item de quête attendu pour démarrer.
Give itm = 8:15:		; N° item de quête à donner une fois explosé.
*/

// Phases.
enum
{
	e_Mst49_Wait = 0,
	e_Mst49_Arrival,
	e_Mst49_Idle,
	e_Mst49_Dead,
};

#define	MST49_DECAY_MAX	3
// Structure spécifique.
struct SMst49_SuspSub
{
	u8	nHitCnt, nLife;
	u8	nItemWait;
	u8	nItemGive;

	u16	nPosXOrg;
	u8	nDecay;
	u8	nExplosions;

	struct SMst46_Sub0	sSub0;

};

#define	MST49_LIFE	10
#define	MST49_EXPLO_CNT	(67 + MEDIUMEXPLO_ANM_DURATION)
void Mst49_Init_SuspSub0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst49_SuspSub	*pSpe = (struct SMst49_SuspSub *)pMst->pData;
	s32	nVal;

	nVal = GetBits(0, 7, pData, 0);
	pSpe->nItemWait = nVal;		// Item attendue.
	nVal = GetBits(8, 15, pData, 0);
	pSpe->nItemGive = nVal;		// Item à donner (0 si rien).

	pSpe->nHitCnt = 0;
	pSpe->nLife = MST49_LIFE;
	pSpe->nDecay = 0;
	pSpe->nExplosions = 0;

	pSpe->nPosXOrg = pMst->nPosX >> 8;	// Position originale.

	Mst46_Sub0_Init(&pSpe->sSub0);		// Init LRAC soldier.
	pSpe->sSub0.nPhase = e_Mst46Sub0_End;		// Par défaut, pas d'affichage.

	// Etat initial.
	pMst->nPhase = (gpMstQuestItems[pSpe->nItemGive] ? e_Mst49_Dead : e_Mst49_Wait);

	Rumi_Reset();

}

#define	MST14SUB1_OFFSX	(-28)
#define	MST14SUB1_OFFSY	(44)
void Mst49_Sub0_Main(struct SMstCommon *pMst)
{
	struct SMst49_SuspSub	*pSpe = (struct SMst49_SuspSub *)pMst->pData;
	struct SMst46_Sub0	*pSub0 = (struct SMst46_Sub0 *)&pSpe->sSub0;

	struct SM46SubPrm	sPrm;
	sPrm.pMst = pMst;
	sPrm.nOffsX = MST14SUB1_OFFSX;
	sPrm.nOffsY = MST14SUB1_OFFSY;
	sPrm.pSub0 = pSub0;
	sPrm.nExplosions = pSpe->nExplosions;
	sPrm.nDecay = pSpe->nDecay;
	sPrm.nDecayMax = MST49_DECAY_MAX;
	sPrm.nPhaseEnd = e_Mst46Sub0_JumpInit;
	Mst_SubLRAC0_Main(&sPrm);

}

s32 Mst49_Main_SuspSub0(struct SMstCommon *pMst)
{
	struct SMst49_SuspSub	*pSpe = (struct SMst49_SuspSub *)pMst->pData;
	u8	nTouch = 0;

	// Screen out ?
	if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY))
	{
		if (pSpe->sSub0.nAnm != -1) AnmReleaseSlot(pSpe->sSub0.nAnm);
		return (e_MstState_Asleep);
	}

	switch (pMst->nPhase)
	{
	case e_Mst49_Wait:		// Attente item de quête qui va bien.
		if (gpMstQuestItems[pSpe->nItemWait] == 0) return (e_MstState_Managed);
		// On y va, on décale le sprite à droite de l'écran pour l'arrivée.
		pMst->nPhase = e_Mst49_Arrival;
		pSpe->sSub0.nPhase = e_Mst46Sub0_Aim;	// Le soldat est là !
		pMst->nPosX = gScrollPos.nPosX + ((SCR_Width + 64) << 8);
		pMst->nSpd = 0x100;
	case e_Mst49_Arrival:	// Arrivée du sub de la gauche.
		if (((pMst->nPosX >> 8) & 0x1F) == 0) Sfx_PlaySfx(e_Sfx_Fx_DoorClang, e_SfxPrio_30);

		pSpe->sSub0.nCntWait = 16;	// On empêche le soldat de tirer pendant le mvt.
		if (pMst->nPosX <= ((u32)pSpe->nPosXOrg + 16) << 8)
		{
			pMst->nPhase = e_Mst49_Idle;
			break;
		}
		// Déplacement.
		pMst->nPosX -= pMst->nSpd;
		break;

	case e_Mst49_Idle:		// Normal.
		nTouch = !pSpe->nExplosions;	// Prise en compte des touchés, sauf pendant les explosions.

		Rumi_Check(10*16, e_Player_Weapon_Machinegun);

		// Ralentissement sur la fin de l'arrivée.
		if (pMst->nSpd)
		{
			pMst->nSpd -= 0x08;
			pMst->nPosX -= pMst->nSpd;
		}

		// Génération de soldats à droite et à gauche de temps en temps.
		if ((gnFrame & 255) == 0)	//63
		if (MstOnScreenNb(e_Mst14_RebelSoldier0, 1) < 2 +2)	// Pas plus de x à l'écran. (+2 pour les 2 qui attendent pour arriver de la gauche).
		{
			/*
			[mst14] Reb Soldier 0
			Type = 0:7: Rifle - Mortar - LRAC - Gun_Grenade - RESERVED - Shield
			OffsX = -8:11:			; Décalage X.
			Move = 12:12: No - Yes		; Le soldat bouge ou pas.
			From = 13:13: Right - Left	; Apparition à droite ou à gauche de l'écran.
			Parachute = 14:14: No - Yes	; Parachuté ?
			Jump = 15:16: No - Random - Always	; Saut en bout de pf.
			Org OffsX = -17:22:		; Décalage de l'origine de la zone.
			Special Init = 23:26: None - Truck Jump - SuspSub Jump - Hero Height -	; Inits spéciales.
			RESERVED2 = 27:31:
			*/
			static u8 pM14[] = { 0, 1, 3, 3 };
			u32	nSide = rand() & 1;
			u32	nPrm = ((u32)pM14[rand() & 3]) | (1 << 12) | (((nSide ? -11 : 11) & 0x3F) << 17);
			MstAdd(e_Mst14_RebelSoldier0, (gScrollPos.nPosX >> 8) + (nSide ? SCR_Width + 16 : -16), 10 * 16, (u8 *)&nPrm, -1);
		}
		break;

	case e_Mst49_Dead:		// Mort.
		pSpe->nDecay = MST49_DECAY_MAX;
		break;
	}

	// Gestion du LRAC soldier.
	Mst49_Sub0_Main(pMst);

	// Se prend un tir ?
	if (nTouch)
	{
		// Test des tirs.
		if (Mst_ShotCheckLife2(e_Spr_SuspendedSub0, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife, e_ShotCheck_MinimizedDamage))
		{
			pSpe->nLife = MST49_LIFE;		// Reset energy.
			pSpe->nExplosions = MST49_EXPLO_CNT;//67 + MEDIUMEXPLO_ANM_DURATION;
			// Mort ?
			if (++pSpe->nDecay >= MST49_DECAY_MAX)
			{
				pMst->nPhase = e_Mst49_Dead;
				gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;	// Score.
				gpMstQuestItems[pSpe->nItemGive] |= 1;		// Item de quête.
			}
		}
	}
	else
	{
		// Interception des tirs sans dégats.
		u8	nHitCnt, nLife;
		nLife = 0;
		Mst_ShotCheckLife2(e_Spr_SuspendedSub0, pMst->nPosX, pMst->nPosY, &nHitCnt, &nLife, 0);
	}

	// Explosions ?
	if (pSpe->nExplosions)
	{
		pSpe->nExplosions--;
		if (pSpe->nExplosions > MEDIUMEXPLO_ANM_DURATION && (pSpe->nExplosions & 0x7) == 0)
		{
			u32	i;
			i = rand();
			DustSet(gAnm_Explosion0_Medium_Dust, pMst->nPosX + (pSpe->nExplosions & 8 ? -32 << 8 : 0) + ((i & 31) << 8), pMst->nPosY + (80 << 8) + (((i >> 8) & 31) << 8),
				e_Prio_Ennemies + 4 + (((MST49_EXPLO_CNT - pSpe->nExplosions) >> 2) & 0x1F), 0);

//if (((pSpe->sFrontC[i].nExplo >> 4) + i) & 1)	// + Débris 1 fois sur 2.
//DustSetMvt(gAnm_Debris_Metal0_Dust, x, y, -0x180, -0x380, e_Prio_Ennemies-1, e_DustFlag_Gravity);

		}
	}

	// Affichage.
	if (pSpe->nHitCnt) pSpe->nHitCnt--;
	u32	nFlag;
	nFlag = (pSpe->nHitCnt ? SPR_Flag_HitPal : 0);
	SprDisplay((e_Spr_SuspendedSub0 + (u32)pSpe->nDecay) | nFlag, (pMst->nPosX >> 8), (pMst->nPosY >> 8), e_Prio_Ennemies);
	if (pMst->nPhase != e_Mst49_Dead)	// Cache.
		SprDisplay((e_Spr_SuspendedSub0_fg + (u32)pSpe->nDecay) | nFlag, (pMst->nPosX >> 8), (pMst->nPosY >> 8) + 40, e_Prio_Ennemies + 3);

	return (e_MstState_Managed);
}

//=============================================================================

/*
struct SAAE { u8	nTb0[80]; };
assert(sizeof(struct SAAE) < MST_COMMON_DATA_SZ);
*/

#ifndef NDEBUG
// Debug, vérification de la taille des structures.
void Mst40CheckStructSizes(void)
{
	assert(sizeof(struct SMst40_L11SpaceTrash0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst34_L11MarsEye0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst42_DoorToDestroy0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst43_FlyingTara0) < MST_COMMON_DATA_SZ);
	// Mst44: Pas de struct spécifique.
	assert(sizeof(struct SMst45_FlyingTaraBomb0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst46_HairBusterRibert0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst47_Lev9RocketTop0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst48_Joke0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst49_SuspSub) < MST_COMMON_DATA_SZ);

}
#endif

