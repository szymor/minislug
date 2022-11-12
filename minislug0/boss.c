
#include "includes.h"
#include "monsters_inc.h"

//s32 GetBits(u32 nBitStart, u32 nBitEnd, u8 *pData, u32 nSign);
void Mst_PlayerBlock(s32 nPosX, u32 nSide);

/*
// Version qui renvoie le dernier dégat. Ok, mais voir si ça sert à quelque chose.

// Idem Mst_ShotCheckLife mais en passant un rectangle au lieu d'un sprite.
// Out: 0 = Le monstre n'est pas mort / !0 = Le dernier dégat (utile pour le type de dégats).
u32 Mst_ShotCheckLifeRect(struct SSprRect *pRect, s32 nPosX, s32 nPosY, u8 *pnHitCnt, u8 *pnLife)
{
	u32	nDamage, nPts;
	if ((nDamage = FireHitCheckRect(pRect, nPosX, nPosY, e_ShotOrg_Player, 1)) != 0)
	{
		*pnHitCnt = MST_HIT_CNT;	// Compteur de frames du hit, pour palette 'hit'.

		nPts = nDamage & 0xFFFF;
		if (nPts > *pnLife) nPts = *pnLife;
		*pnLife -= nPts;

		if (*pnLife == 0) { *pnHitCnt = 0; return (nDamage); }	// Mort.
	}
	return (0);
}
*/

/*
enum
{
	e_ShotCheck_MinimizedDamage = 1,
};
*/
// Idem Mst_ShotCheckLife mais en passant un rectangle au lieu d'un sprite.
u32 Mst_ShotCheckLifeRect2(struct SSprRect *pRect, s32 nPosX, s32 nPosY, u8 *pnHitCnt, u8 *pnLife, u32 nFlags)
{
/*
	u32	nDamage;
	if ((nDamage = FireHitCheckRect(pRect, nPosX, nPosY, e_ShotOrg_Player, 1)) != 0)
	{
		*pnHitCnt = MST_HIT_CNT;	// Compteur de frames du hit, pour palette 'hit'.

		nDamage &= 0xFFFF;

		if (nFlags & e_ShotCheck_MinimizedDamage)
		{
			static	u8	nMiniCnt = 0;
			if ((++nMiniCnt) & 3) nDamage /= 2;
			if (nDamage == 0) nDamage = 1;
			else if (nDamage > 3) nDamage = 3;
		}

		if (nDamage > *pnLife) nDamage = *pnLife;
		*pnLife -= nDamage;

		if (*pnLife == 0) { *pnHitCnt = 0; return (1); }	// Mort.
	}
	return (0);
*/

	u32	nDamage, nPts;
	if ((nDamage = FireHitCheckRect(pRect, nPosX, nPosY, e_ShotOrg_Player, 1)) != 0)
	{
		*pnHitCnt = MST_HIT_CNT;	// Compteur de frames du hit, pour palette 'hit'.

		nPts = nDamage & 0xFFFF;

		if (nFlags & e_ShotCheck_MinimizedDamage)
		{
			static	u8	nMiniCnt = 0;
			if ((++nMiniCnt) & 3) nPts /= 2;
			if (nPts == 0) nPts = 1;
			else if (nPts > 3) nPts = 3;
		}

		if (nPts > *pnLife) nPts = *pnLife;
		*pnLife -= nPts;

		if (*pnLife == 0) { *pnHitCnt = 0; return (nDamage); }	// Mort.
	}
	return (0);
}
u32 Mst_ShotCheckLifeRect(struct SSprRect *pRect, s32 nPosX, s32 nPosY, u8 *pnHitCnt, u8 *pnLife)
{
	return (Mst_ShotCheckLifeRect2(pRect, nPosX, nPosY, pnHitCnt, pnLife, 0));
}

//=============================================================================
// Boss 1 : Avion écrasé (MS1 - Mission 1).

#define	BOSS1_SCORE		2000

// Phases.
enum
{
	e_MstB1_ItemWait = 0,
	e_MstB1_Intro,
	e_MstB1_Fight,
	e_MstB1_Death,
};

struct SMst20_CannonUp
{
	u8	nPhase;
	s8	nAnm;
	s16	nOffsY;
	u8	nShotCnt;
	u8	nShotNo;			// Pour comptage des tirs et décalage.
	u8	nShotHole;			// Position du "trou" pour le joueur.
};

// Structure spécifique.
struct SMst20_Boss1
{
	u8	nReserved_Boss0;	// Pour mappage sur le Mst20 container.
	//
	u8	nWaitItm;			// Item de quête attendu pour démarrer.
	u8	nHitCnt;
	u8	nDecayState;		// Etat de délabrement [0;4].
	u8	nLife;
	u8	nDeathCnt;
	u8	nSoldierAppCnt;		// Compteur pour ne pas faire apparaitre les soldats trop vite.
	u8	nExplosions;		// Compteur pour pas de touché pendant les explosions.

	struct SMst20_CannonUp	sCannonUp;		// Canon vers le haut.
	struct SMst20_CannonUp	sCannonFront;	// Canon frontal.

};

#define	BOSS1_ENERGY	30//25//20//4//50	// Vie, * nombre d'états.
#define	BOSS1_DECAY_MAX	5	// 4 + 1 : Le dernier == mort.

#define	BOSS1_SOLDIERAPPCNT		64

//=== Canon vertical.
#define	BOSS1_CANNONUP_OFFSX	-26
#define	BOSS1_CANNONUP_OFFSY_DOWN	-34
#define	BOSS1_CANNONUP_OFFSY_UP		-66

enum
{
	e_MstB1_CannonUp_Wait = 0,
	e_MstB1_CannonUp_GoUp,
	e_MstB1_CannonUp_Shot,
	e_MstB1_CannonUp_GoDown,
};

// Init.
void Mst20_CannonUp_Init(struct SMst20_CannonUp *pCannonUp)
{
	pCannonUp->nPhase = e_MstB1_CannonUp_Wait;
	pCannonUp->nAnm = AnmSet(gAnm_Boss1_CannonUp_Idle, -1);		// Anim du canon vers le haut.
	pCannonUp->nOffsY = BOSS1_CANNONUP_OFFSY_DOWN;
}

// Gestion.
void Mst20_CannonUp_Manage(struct SMstCommon *pMst, u16 nPrio)
{
	struct SMst20_Boss1	*pSpe = (struct SMst20_Boss1 *)pMst->pData;
	struct SMst20_CannonUp *pCannonUp = &pSpe->sCannonUp;
	u32	nSpr;

	// Boss mort ?
	if (pSpe->nDecayState >= BOSS1_DECAY_MAX)
	if (pCannonUp->nPhase != e_MstB1_CannonUp_GoDown)
	{
		pCannonUp->nPhase = e_MstB1_CannonUp_GoDown;
		AnmSet(gAnm_Boss1_CannonUp_Destroyed, pCannonUp->nAnm);
	}

	switch (pCannonUp->nPhase)
	{
	case e_MstB1_CannonUp_Wait:		// Attente caché.
		if (pSpe->nDecayState >= 3) pCannonUp->nPhase = e_MstB1_CannonUp_GoUp;
		return;
		break;

	case e_MstB1_CannonUp_GoUp:		// Monte en position de tir.
		if (pCannonUp->nOffsY <= BOSS1_CANNONUP_OFFSY_UP)	// En position ?
		{
			pCannonUp->nPhase = e_MstB1_CannonUp_Shot;
			pCannonUp->nShotCnt = 1;
			pCannonUp->nShotNo = 0;
			break;
		}
		if (gnFrame & 1) pCannonUp->nOffsY--;		// Monte.
		break;

	case e_MstB1_CannonUp_Shot:		// En phase de tir.
		// Déclenchement de l'anim de tir ?
		if (AnmGetKey(pCannonUp->nAnm) == e_AnmKey_Null)
		if ((gnFrame & 3) == 0)
		if (--pCannonUp->nShotCnt == 0)
		{
			static u8 pCntTb[BOSS1_DECAY_MAX] = { 140, 140, 140, 120, 100 };
			//
			AnmSet(gAnm_Boss1_CannonUp_Shot, pCannonUp->nAnm);
			pCannonUp->nShotCnt = pCntTb[pSpe->nDecayState];
			pCannonUp->nShotNo = 0;
			pCannonUp->nShotHole = rand() % 7;		// Position du "trou".
		}
		break;

	case e_MstB1_CannonUp_GoDown:	// Cassé, descente.
		if (pCannonUp->nOffsY < BOSS1_CANNONUP_OFFSY_DOWN)
			if (gnFrame & 1) pCannonUp->nOffsY++;	// On redescend.
		break;
	}

	nSpr = AnmGetImage(pCannonUp->nAnm);

	// Tirs.
	if (AnmCheckStepFlag(pCannonUp->nAnm))
	{
		FireAdd(e_Shot_Enemy_Boss1_ParachuteBomb, pMst->nPosX - (10 << 12) - (((14<<4) - ((s32)pCannonUp->nShotNo * 0x28) - (pCannonUp->nShotHole <= pCannonUp->nShotNo ? 0x28 : 0)) << 8), gScrollPos.nPosY - (2 << 12), -1);
		pCannonUp->nShotNo++;
	}

	// Affichage.
	u32	nFlagHit = (pSpe->nHitCnt ? SPR_Flag_HitPal : 0);
	SprDisplay(nSpr | nFlagHit, (pMst->nPosX >> 8) + BOSS1_CANNONUP_OFFSX, (pMst->nPosY >> 8) + (s32)pCannonUp->nOffsY, nPrio);

}

//=== Canon frontal.
#define	BOSS1_CANNONFRONT_OFFSX	-85
#define	BOSS1_CANNONFRONT_OFFSY_DOWN	1
#define	BOSS1_CANNONFRONT_OFFSY_UP		-25
#define	BOSS1_CANNONFRONT_OFFSY_BASE	-20		// Pos d'attente de base.

enum
{
	e_MstB1_CannonFront_Wait = 0,
	e_MstB1_CannonFront_Move,
	e_MstB1_CannonFront_Shot,
};

// Init.
void Mst20_CannonFront_Init(struct SMst20_CannonUp *pCannonFront)
{
	pCannonFront->nPhase = e_MstB1_CannonFront_Wait;
	pCannonFront->nAnm = AnmSet(gAnm_Boss1_CannonFront_Idle, -1);	// Anim du canon frontal.
	pCannonFront->nOffsY = BOSS1_CANNONFRONT_OFFSY_BASE;
	pCannonFront->nShotHole = 0;		// b7 = Sens de déplacement, b0-3 compteur.
}

// Gestion.
void Mst20_CannonFront_Manage(struct SMstCommon *pMst, u16 nPrio)
{
	struct SMst20_Boss1	*pSpe = (struct SMst20_Boss1 *)pMst->pData;
	struct SMst20_CannonUp *pCannonFront = &pSpe->sCannonFront;
	u32	nSpr;

	// Canon HS ?
	if (pSpe->nDecayState >= 3)
	{
		if (pCannonFront->nPhase != e_MstB1_CannonFront_Wait)		// Phase uniquement pour ne lancer les explosions qu'une fois.
		{
			pCannonFront->nPhase = e_MstB1_CannonFront_Wait;
			// Dusts d'explosion.
			s32	nDustPosX, nDustPosY;
			nDustPosX = pMst->nPosX + (BOSS1_CANNONFRONT_OFFSX << 8) - (10 << 8);
			nDustPosY = pMst->nPosY + ((s32)pCannonFront->nOffsY << 8) + (8 << 8);
			DustSet(gAnm_Explosion0_Big_Dust, nDustPosX, nDustPosY, e_Prio_DustOver, 0);
			DustSetMvt(gAnm_Debris_Metal0_Dust, nDustPosX, nDustPosY, -0x080, -0x480, e_Prio_DustUnder, e_DustFlag_Gravity);
			DustSetMvt(gAnm_Debris_Metal0_Dust, nDustPosX, nDustPosY, -0x180, -0x380, e_Prio_DustUnder, e_DustFlag_Gravity);
		}
		return;
	}

	switch (pCannonFront->nPhase)
	{
	case e_MstB1_CannonFront_Wait:		// Attente.
		if (pSpe->nDecayState >= 1) pCannonFront->nPhase = e_MstB1_CannonFront_Move;
		break;

	case e_MstB1_CannonFront_Move:		// Bouge de haut en bas.
		if (AnmGetKey(pCannonFront->nAnm) != e_AnmKey_Null) break;
		// Déplacement.
		{
			u8	nFlip = 0;
			if (pCannonFront->nShotHole & 0x80)
			{	// Vers le bas.
				if (++pCannonFront->nOffsY > BOSS1_CANNONFRONT_OFFSY_DOWN) nFlip = 1;
			}
			else
			{	// Vers le haut.
				if (--pCannonFront->nOffsY < BOSS1_CANNONFRONT_OFFSY_UP) nFlip = 1;
			}
			if (nFlip)
			{
				pCannonFront->nShotHole ^= 0x80;
				if (MST_SHOT_COND) pCannonFront->nShotHole++;
			}
		}
		// Déclenchement du tir ?
		if ((pCannonFront->nShotHole & 0xF) == 3)
		{
			pCannonFront->nPhase = e_MstB1_CannonFront_Shot;
			pCannonFront->nShotHole &= ~0xF;
			pCannonFront->nShotCnt = 63;
		}
		break;

	case e_MstB1_CannonFront_Shot:		// Attend un peu et tire.
		if (--pCannonFront->nShotCnt) break;
		//
		AnmSet(gAnm_Boss1_CannonFront_Shot, pCannonFront->nAnm);
		pCannonFront->nPhase = e_MstB1_CannonFront_Move;
		break;
	}
	nSpr = AnmGetImage(pCannonFront->nAnm);

	// Tir.
	if (AnmCheckStepFlag(pCannonFront->nAnm))
		FireAdd(e_Shot_Enemy_Boss1_BulletFront, pMst->nPosX + (BOSS1_CANNONFRONT_OFFSX << 8) - (44 << 8), pMst->nPosY + ((s32)pCannonFront->nOffsY << 8) - (8 << 8), -1);

	// Affichage.
	u32	nFlagHit = (pSpe->nHitCnt ? SPR_Flag_HitPal : 0);
	SprDisplay(nSpr | nFlagHit, (pMst->nPosX >> 8) + BOSS1_CANNONFRONT_OFFSX, (pMst->nPosY >> 8) + (s32)pCannonFront->nOffsY, nPrio);

}

//===

void Mst20_Init_Boss1(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst20_Boss1	*pSpe = (struct SMst20_Boss1 *)pMst->pData;
	u32	nVal;

	pMst->nPosX = (gMap.pPlanesLg[gMap.nHeroPlane] - 0) << 12;		// Replace le monstre complètement à droite.
	nVal = GetBits(8, 15, pData, 0);
	pSpe->nWaitItm = nVal;

	pSpe->nDecayState = 0;
	pSpe->nLife = BOSS1_ENERGY;

	pMst->nPhase = e_MstB1_ItemWait;
	pSpe->nHitCnt = 0;
	pSpe->nExplosions = 0;

	pSpe->nSoldierAppCnt = BOSS1_SOLDIERAPPCNT;

	Mst20_CannonUp_Init(&pSpe->sCannonUp);
	Mst20_CannonFront_Init(&pSpe->sCannonFront);

	Rumi_Reset();

}

extern s32	gnScrollLimitYMin;
extern s32	gnScrollLimitYMax;

// Supprime les collisions des blocs demandés.
void Boss1_sub_BlkColRemove(u32 nFirst, u32 nLast)
{
	struct SBlockCol	*pCol;
	u32	i;
	assert(nFirst <= nLast);

	pCol = gMap.ppColCodes[gMap.nHeroPlane] + nFirst;
	for (i = nFirst; i <= nLast; i++)
	{
		pCol->nCol = 0;
		pCol++;
	}
}

// Avance de la phase.
	//>>> méthode pas forcément définitive, on pourrait faire ça en fct des points de vie.
void Boss1_DecayAdvance(struct SMstCommon *pMst)
{
	struct SMst20_Boss1	*pSpe = (struct SMst20_Boss1 *)pMst->pData;

	if (pSpe->nDecayState >= BOSS1_DECAY_MAX) return;

	pSpe->nDecayState++;
	switch (pSpe->nDecayState)
	{
	case 3:
		// Suppression de la pf gauche : 2291 > 2295.
		Boss1_sub_BlkColRemove(2291, 2295);
		break;

	case 4:
		// Suppression de la pf droite : 2611 > 2615.
		Boss1_sub_BlkColRemove(2611, 2615);
		break;

	default:
		break;
	}
}

#define	BOSS1_PIECESNB	(10)
#define	BOSS1_TARP_OFFSX	(-61)
#define	BOSS1_TARP_OFFSY	(-2)
s32 Mst20_Main_Boss1(struct SMstCommon *pMst)
{
	struct SMst20_Boss1	*pSpe = (struct SMst20_Boss1 *)pMst->pData;
	// Table de tous les sprites composant le boss, 5 états.
	static u32	gpBoss1Spr[][BOSS1_PIECESNB] =
	{
		{ e_Spr_Boss1_P1_1, e_Spr_Boss1_P2_12, e_Spr_Boss1_P3_12345, e_Spr_Boss1_P4_12, e_Spr_Boss1_P5_12,
			e_Spr_Boss1_P6_12, e_Spr_Boss1_P7_123, e_Spr_Boss1_P8_12, e_Spr_Boss1_P9_123, e_Spr_Boss1_P10_1 },
		{ e_Spr_Boss1_P1_2345, e_Spr_Boss1_P2_12, e_Spr_Boss1_P3_12345, e_Spr_Boss1_P4_12, e_Spr_Boss1_P5_12,
			e_Spr_Boss1_P6_12, e_Spr_Boss1_P7_123, e_Spr_Boss1_P8_12, e_Spr_Boss1_P9_123, e_Spr_Boss1_P10_2345 },
		{ e_Spr_Boss1_P1_2345, e_Spr_Boss1_P2_345, e_Spr_Boss1_P3_12345, e_Spr_Boss1_P4_345, e_Spr_Boss1_P5_3,
			e_Spr_Boss1_P6_3, e_Spr_Boss1_P7_123, e_Spr_Boss1_P8_3, e_Spr_Boss1_P9_123, e_Spr_Boss1_P10_2345 },
		{ e_Spr_Boss1_P1_2345, e_Spr_Boss1_P2_345, e_Spr_Boss1_P3_12345, e_Spr_Boss1_P4_345, e_Spr_Boss1_P5_4,
			SPR_NoSprite, e_Spr_Boss1_P7_4, e_Spr_Boss1_P8_4, e_Spr_Boss1_P9_45, e_Spr_Boss1_P10_2345 },
		{ e_Spr_Boss1_P1_2345, e_Spr_Boss1_P2_345, e_Spr_Boss1_P3_12345, e_Spr_Boss1_P4_345, e_Spr_Boss1_P5_5,
			SPR_NoSprite, e_Spr_Boss1_P7_5, SPR_NoSprite, e_Spr_Boss1_P9_45, e_Spr_Boss1_P10_2345 },
		// Le dernier état ci-dessous est identique au précédent.
		{ e_Spr_Boss1_P1_2345, e_Spr_Boss1_P2_345, e_Spr_Boss1_P3_12345, e_Spr_Boss1_P4_345, e_Spr_Boss1_P5_5,
			SPR_NoSprite, e_Spr_Boss1_P7_5, SPR_NoSprite, e_Spr_Boss1_P9_45, e_Spr_Boss1_P10_2345 },
	};
	static u32	gpBoss1OffsX[BOSS1_PIECESNB] = { -32,-52,-104,-52,-111,-283,-229,-283,-229, -32 };	// Offsets x et y des sprites.
	static u32	gpBoss1OffsY[BOSS1_PIECESNB] = { -2,-113,-2,-113,-84,-135,-66,-135,-66, -2 };
	static u8	gpBoss1Prio[BOSS1_PIECESNB] = {
		e_Prio_Ennemies-1, e_Prio_Ennemies+32, e_Prio_DustUnder-8, e_Prio_DustUnder-8,
		e_Prio_DustUnder-8, e_Prio_DustUnder-8, e_Prio_DustUnder-8, e_Prio_DustUnder-8,
		e_Prio_DustUnder-8, e_Prio_Ennemies+34 };

	u8	nTouch = 0;		// Pas de gestion du touché.

	// Bloque le joueur au niveau de l'avion.
	Mst_PlayerBlock(pMst->nPosX - (8 << 12) - (12 << 8), 0);

	switch (pMst->nPhase)
	{
	case e_MstB1_ItemWait:		// Attente de l'item de quête.
		if (gpMstQuestItems[pSpe->nWaitItm]) pMst->nPhase = e_MstB1_Intro;
		// Affichage de la bâche.
		SprDisplay(e_Spr_Boss1_Tarp_Idle, (pMst->nPosX >> 8) + BOSS1_TARP_OFFSX, (pMst->nPosY >> 8) + BOSS1_TARP_OFFSY, e_Prio_DustOver);
		break;

	case e_MstB1_Intro:			// Montée du scroll.
		// Recentrage du scroll en y au début.
		if ((u32)gnScrollLimitYMin >> 8 > 0)
		{
			gnScrollLimitYMin = gScrollPos.nPosY - 0x100;
			gnScrollLimitYMax = gnScrollLimitYMin + (SCR_Height << 8);
		}
		else
		{
			pMst->nPhase = e_MstB1_Fight;
			// Dust bâche qui dégage vers la droite.
			DustSetMvt(gAnm_Boss1_Tarp_Dust, pMst->nPosX + (BOSS1_TARP_OFFSX << 8), pMst->nPosY + (BOSS1_TARP_OFFSY << 8), 0x200, 0, e_Prio_DustOver, 0);
			break;
		}
		// Affichage de la bâche.
		SprDisplay(e_Spr_Boss1_Tarp_Idle, (pMst->nPosX >> 8) + BOSS1_TARP_OFFSX, (pMst->nPosY >> 8) + BOSS1_TARP_OFFSY, e_Prio_DustOver);
		break;

	case e_MstB1_Fight:
//		nTouch = 1;				// Prise en compte des touchés.
		nTouch = !pSpe->nExplosions;	// Prise en compte des touchés, sauf pendant les explosions.

		// Génération des soldats.
		if (gpnMstCount[e_Mst14_RebelSoldier0] < 3)
		if (--pSpe->nSoldierAppCnt == 0)
		{
			static u8	nSide = 0;
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
			static u8 pM14[] = { 3, 3, 0, 1 };	// Pour générer des soldats, sauf LRAC.
			u32	nPrm = ((u32)pM14[rand() & 3]) | (1 << 12) | (2 << 15);	// 0 = Rifle - 1 = Mortar / 1 << 12 = Move / 2 << 15 = Jump always.
			if (((++nSide) & 1) == 0)
			{
				// Arrivée par la porte à droite.
				nPrm |= ((-8 & 0x3F) << 17);	// (x & 0x3F) << 17 = décalage org.
				MstAdd(e_Mst14_RebelSoldier0, (241*16), (3*16), (u8 *)&nPrm, -1);
			}
			else
			{
				// Arrivée par la gauche.
				nPrm |= ((8 & 0x3F) << 17);	// (x & 0x3F) << 17 = décalage org.
				MstAdd(e_Mst14_RebelSoldier0, (216*16), (11*16), (u8 *)&nPrm, -1);
			}

//> tst parachutiste
			// LRAC Parachutiste.
			if ((nSide & 3) == 0)
			{
				// Parachutiste.
				nPrm &= ~0xFF;
				nPrm |= 2;	// Type = LRAC.
				nPrm |= 8 << 8;		// Offset, pour que le parachute arrive au bon endroit. (Pour le parachutiste, on se sert de ce décalagae comme offset dans l'écran, voir init).
				nPrm |= 1 << 14;	// Parachute.
				MstAdd(e_Mst14_RebelSoldier0, ((224 + (rand() & 7))*16), (0*16), (u8 *)&nPrm, -1);
			}
//< tst parachutiste

			pSpe->nSoldierAppCnt = BOSS1_SOLDIERAPPCNT;
		}


//>>>
/*
>>todo
					// On génère un Girida-0 depuis l'arrière.
/ *
[mst26] Girida-0
From = 0:0: Front - Back	; Attaque par devant ou derrière.
ZoneMax = 1:7:			; Zone de déplacement max, 0 = pas de limite.
* /
					u8	pData[4];
					s32	nGiridaPixPosX = (gScrollPos.nPosX >> 8) - (2 << 4);	// !!! Attention au décalage, sinon le tank n'apparaît pas !
					pData[0] = 1 + (((ABS((pSpe->nPosXOrg >> 8) - nGiridaPixPosX) >> 4) - 4) << 1);		// -4 : Décalage pour ne pas monter sur le camion.
					MstAdd(e_Mst26_Girida0, nGiridaPixPosX, (pMst->nPosY >> 8) - 16, pData, -1);
*/
//<<<

		Rumi_Check(pMst->nPosY >> 8, (gnFrame & 1 ? e_Player_Weapon_Flamethrower : e_Player_Weapon_Machinegun));

		break;

	case e_MstB1_Death:			// Mort.
		if (pSpe->nDeathCnt)
		{
			pSpe->nDeathCnt--;
			break;
		}
		gpMstQuestItems[MST_QUEST_ITEM_NEXT_LEVEL] |= 1;	// Item de quête : Fin de niveau.
		break;

	}


	// Se prend un tir ?
	{
		struct SSprRect sRect1;
		// On créé un rectangle de collision.
		sRect1.nType = e_SprRect_Rect;
		sRect1.nX1 = -100;
		sRect1.nX2 = 0;
		sRect1.nY1 = -90;
		sRect1.nY2 = 0;

		if (nTouch)
		{
			// Test des tirs, avec dégats minimisés.
			if (Mst_ShotCheckLifeRect2(&sRect1, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife, e_ShotCheck_MinimizedDamage))
			{
				Boss1_DecayAdvance(pMst);		// On avance le decay state.
				gpMstQuestItems[100 + pSpe->nDecayState] = 1;	// Explosions.
				pSpe->nExplosions = 64;		// Compteur pour pas de touché pendant les explosions.
				pSpe->nLife = BOSS1_ENERGY;		// Reset energy.
				pSpe->nHitCnt = MST_HIT_CNT;	// Pas vraiment mort, on met le hit cnt.
				// Mort ?
				if (pSpe->nDecayState >= BOSS1_DECAY_MAX)
				{
					gpMstQuestItems[MST_QUEST_ITEM_NEXT_LEVEL] |= 2;	// Item de quête : Victory.
					pMst->nPhase = e_MstB1_Death;
					pSpe->nHitCnt = 0;
					pSpe->nDeathCnt = 96;		// Compteur pour laisser le temps aux explosions de disparaitre.
					// Retire les tirs ennemis.
					FireRemoveFromList((1<<31) | e_ShotOrg_Enemy);
					gShoot.nPlayerScore += BOSS1_SCORE;	// Score.
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
	}
	if (pSpe->nExplosions) pSpe->nExplosions--;		// Compteur pour pas de touché pendant les explosions.

	// Dec compteur de hit *avant* l'affichage des canons Up et Front.
	if (pSpe->nHitCnt) pSpe->nHitCnt--;

	// Canon Up.
	Mst20_CannonUp_Manage(pMst, gpBoss1Prio[9] - 1);
	// Canon Frontal.
	Mst20_CannonFront_Manage(pMst, gpBoss1Prio[0] - 1);

	// Affichage de toutes les parties du sprite.
	u32	i;
	u32	nFlagHit = (pSpe->nHitCnt ? SPR_Flag_HitPal : 0);
	for (i = 0; i < BOSS1_PIECESNB; i++)
		if (gpBoss1Spr[pSpe->nDecayState][i] != SPR_NoSprite)
			SprDisplay(gpBoss1Spr[pSpe->nDecayState][i] | nFlagHit, (pMst->nPosX >> 8) + gpBoss1OffsX[i], (pMst->nPosY >> 8) + gpBoss1OffsY[i], gpBoss1Prio[i]);

	return (e_MstState_Managed);
}

//=============================================================================
// Boss 2A - Avant de l'avion.

// Phases.
enum
{
	e_MstB2A_ComeDistance = 0,
	e_MstB2A_Avance1,
	e_MstB2A_Phase1,
	e_MstB2A_Recule1,
	e_MstB2A_Avance2,
	e_MstB2A_Phase2,
	e_MstB2A_Phase2Explo,
	e_MstB2A_Recule2,
};

// Struct pour les canons frontaux.
struct SB2A_FrontalCannon
{
	u8	nHitCnt;
	u8	nLife;
	//
	u8	nExplo;
	s8	nShotAnm;
	u8	nHB;		// Tir en haut ou en bas.
};
#define	B2A_FRONTCANNON_LIFE	50
#define	B2A_FRONTCANNON_NB	2
#define	B2A_FRONTCANNON_OFFSX	-28
#define	B2A_FRONTC_SCORE	500

// Struct pour les tourelles.
struct SB2A_Turret
{
	u8	nHitCnt;
	u8	nLife;
	//
	u8	nAngle;
	s8	nShotAnm;
};
#define	B2A_TURRET_LIFE	50
#define	B2A_TURRET_NB	2
#define	B2A_TURRET_SCORE	500

// Structure spécifique.
struct SMst20_Boss2A
{
	u8	nReserved_Boss0;	// Pour mappage sur le Mst20 container.
	//
	u8	nDecayState;		// Etat de délabrement [0;1].
	u8	nHitCnt;
	u8	nLife;

	s8	nPilotsCur, nPilotsTarget;	// Pour 'angle' des pilotes.
	u8	nSensY;
	u8	nExplo;

	u8	nShotFrm;
	// Canons frontaux.
	struct SB2A_FrontalCannon	sFrontC[B2A_FRONTCANNON_NB];
	s8	nFrontCannonAnm;
	// Tourelles.
	struct SB2A_Turret	sTurret[B2A_TURRET_NB];

};

#define	B2A_DISTANCE_POSX_MIN	(-58)
#define	B2A_DISTANCE_POSX_MAX	(372)
void Mst20_Init_Boss2A(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst20_Boss2A	*pSpe = (struct SMst20_Boss2A *)pMst->pData;
	u32	i;

	pSpe->nHitCnt = 0;
	pSpe->nDecayState = 0;

	// Init des canons frontaux.
	for (i = 0; i < B2A_FRONTCANNON_NB; i++)
	{
		pSpe->sFrontC[i].nHitCnt = 0;
		pSpe->sFrontC[i].nLife = B2A_FRONTCANNON_LIFE;
		pSpe->sFrontC[i].nExplo = (16 * 3) + 1;
		pSpe->sFrontC[i].nShotAnm = -1;
		pSpe->sFrontC[i].nHB = 0;
	}
	pSpe->nFrontCannonAnm = AnmSet(gAnm_Boss2A_FrontCannonGetUp, -1);
	// Init des tourelles.
	for (i = 0; i < B2A_TURRET_NB; i++)
	{
		pSpe->sTurret[i].nAngle = 128;
		pSpe->sTurret[i].nHitCnt = 0;
		pSpe->sTurret[i].nLife = B2A_TURRET_LIFE;
		pSpe->sTurret[i].nShotAnm = -1;
	}

	pSpe->nPilotsCur = pSpe->nPilotsTarget = 0;	// Pour 'angle' des pilotes.
	pMst->nPhase = e_MstB2A_ComeDistance;

	pSpe->nSensY = 0;
	pMst->nPosX = gScrollPos.nPosX + (B2A_DISTANCE_POSX_MIN << 8);
	pMst->nPosY = gScrollPos.nPosY + ((SCR_Height / 2) << 8) - (46 << 8);

/*
	pMst->nPosX = gScrollPos.nPosX + ((SCR_Width + 72) << 8);
	pMst->nPosY = gScrollPos.nPosY + ((SCR_Height / 2) << 8);

	gpMstQuestItems[MST_QUEST_ITEM_MST32]++;	// Bloque le scroll.
	pMst->nSpd = 0x420;		// Spd empirique pour première avance.
*/

	gpMstQuestItems[MST_QUEST_ITEM_MST32]++;	// Bloque le scroll.

}

//===========================

// Haut/Bas du boss 2.
void Boss2_UpDown(s32 *pnMstPosY, s32 nMin, s32 nMax, s32 nSpd, u8 *pnSensY)
{
	if (*pnSensY)
	{
		*pnMstPosY += nSpd;
		if (*pnMstPosY >= gScrollPos.nPosY + (nMax << 8)) *pnSensY ^= 1;
	}
	else
	{
		*pnMstPosY -= nSpd;
		if (*pnMstPosY <= gScrollPos.nPosY + (nMin << 8)) *pnSensY ^= 1;
	}
}

// Explosions.
// In : nWidth2n et nHeight2n sont des puissances de 2.
void Boss_Explosions(u8 *pnExplo, s32 nPosX, s32 nPosY, s32 nWidth2n, s32 nHeight2n)
{
	u32	i;

	if (*pnExplo == 0) return;

	*pnExplo -= 1;
	if (*pnExplo > MEDIUMEXPLO_ANM_DURATION && (*pnExplo & 0x7) == 0)
	{
		nWidth2n--;
		nHeight2n >>= 1;
		nHeight2n--;

//Sfx_PlaySfx(e_Sfx_Fx_Explosion2, e_SfxPrio_Explosion);

		i = rand();
		DustSet(gAnm_Explosion0_Medium_Dust,	//gAnm_Explosion0_Medium_NoSfx_Dust,	//gAnm_Explosion0_Medium_Dust,
			nPosX + ((i & nWidth2n) << 8),
			nPosY + ((i & nHeight2n) << 8),
			e_Prio_Ennemies+3, 0);
		i += nWidth2n / 2;
		DustSet(gAnm_Explosion0_Medium_NoSfx_Dust,	//gAnm_Explosion0_Medium_Dust,
			nPosX + ((i & nWidth2n) << 8),
			nPosY + (nHeight2n << 8) + ((i & nHeight2n) << 8),
			e_Prio_Ennemies+3, 0);
	}

}

//===========================

// Datas des canons frontaux.
s32	gpnFrontCPosY[B2A_FRONTCANNON_NB] = { -29, 52 };
u32	gpnFrontCBaseSpr[B2A_FRONTCANNON_NB] = { e_Spr_Boss2Front_FrontCannonTop_GetUp, e_Spr_Boss2Front_FrontCannonBottom_GetUp };
u32	gpnFrontCDeathSpr[B2A_FRONTCANNON_NB] = { e_Spr_Boss2Front_FrontCannonTop_Destroyed, e_Spr_Boss2Front_FrontCannonBottom_Destroyed };

// Gestion des canons frontaux.
void B2A_FrontC_Manage(struct SMstCommon *pMst)
{
	struct SMst20_Boss2A	*pSpe = (struct SMst20_Boss2A *)pMst->pData;
	u32	i;
	u32	nInc;
	s32	x, y;
	struct SB2A_FrontalCannon	*pFC;

	nInc = (pSpe->nFrontCannonAnm == -1 ? 12 : AnmGetLastImage(pSpe->nFrontCannonAnm));
	if (nInc == SPR_NoSprite) return;

	for (i = 0; i < B2A_FRONTCANNON_NB; i++)
	{
		pFC = &pSpe->sFrontC[i];

		// Les canons se prennent des tirs ?
		if (pFC->nLife)
		{
			// Tirs.
//			if (Mst_ShotCheckLife(gpnFrontCBaseSpr[i] + nInc, pMst->nPosX + (B2A_FRONTCANNON_OFFSX << 8), pMst->nPosY + (gpnFrontCPosY[i] << 8), &pFC->nHitCnt, &pFC->nLife))
			if (Mst_ShotCheckLife2(gpnFrontCBaseSpr[i] + nInc, pMst->nPosX + (B2A_FRONTCANNON_OFFSX << 8), pMst->nPosY + (gpnFrontCPosY[i] << 8), &pFC->nHitCnt, &pFC->nLife, e_ShotCheck_MinimizedDamage))
			{	// Le canon explose.
				gShoot.nPlayerScore += B2A_FRONTC_SCORE;	// Score.
				if (pFC->nShotAnm != -1)
				{	// Libère le slot de l'anm d'éclat du tir.
					AnmReleaseSlot(pFC->nShotAnm);
					pFC->nShotAnm = -1;
				}
			}
			else
			{	// Le canon n'est pas mort.
				ChaserTarget_AddToList((pMst->nPosX >> 8) + B2A_FRONTCANNON_OFFSX, (pMst->nPosY >> 8) + gpnFrontCPosY[i] - 16);	// Ajout dans la table de visée des missiles.

				// Tir ?
				if ((pSpe->nShotFrm & 63) == 0 && nInc == 12)
					pFC->nShotAnm = AnmSet(gAnm_Boss2A_FrontCannon_ShotSpark, pFC->nShotAnm);

				// Déclenchement d'un tir ?
				if (pFC->nShotAnm != -1)
				{
					if (AnmGetImage(pFC->nShotAnm) == -1)
						pFC->nShotAnm = -1;		// Anim terminée.
					else
					if (AnmCheckStepFlag(pFC->nShotAnm))
					{	// Déclenchement du tir.
						struct SSprRect sRect1;
						if (SprGetRect(gpnFrontCBaseSpr[i] + nInc, e_SprRectZone_ShotOrg, &sRect1))
						if (sRect1.nType == e_SprRect_Rect)
//							FireAdd(e_Shot_Enemy_RebSoldier_Bullet0, pMst->nPosX + (B2A_FRONTCANNON_OFFSX << 8) + (sRect1.nX1 << 8), pMst->nPosY + (gpnFrontCPosY[i] << 8) + ((pFC->nHB & 1 ? sRect1.nY1 : sRect1.nY2) << 8), 128);
							FireAdd(e_Shot_Enemy_RebSoldier_Bullet0, pMst->nPosX + (B2A_FRONTCANNON_OFFSX << 8) + (sRect1.nX1 << 8), pMst->nPosY + (gpnFrontCPosY[i] << 8) + ((pFC->nHB & 1 ? sRect1.nY1 : sRect1.nY2) << 8), 128 + (pFC->nHB & 1 ? -8 : 8));
						pFC->nHB ^= 1;	// Inversion haut/bas.
					}
				} // if (pSpe->sFrontC[i].nShotAnm != -1)
			}

		}
		else
		{
			// Gestion des explosions.
			if (pFC->nExplo)
			if ((--pFC->nExplo & 0x0F) == 0)
			{
				x = pMst->nPosX + (B2A_FRONTCANNON_OFFSX << 8)   + (3 << 12) - ((pFC->nExplo & ~0x0F) << 8);
				y = pMst->nPosY + (gpnFrontCPosY[i] << 8)   - (1 << 12) + ((pFC->nExplo & ~0x0F) << 7);
				DustSet(gAnm_Explosion0_Medium_Dust, x, y, e_Prio_Ennemies+3, 0);
				if (((pFC->nExplo >> 4) + i) & 1)	// + Débris 1 fois sur 2.
					DustSetMvt(gAnm_Debris_Metal0_Dust, x, y, -0x180, -0x380, e_Prio_Ennemies+2, e_DustFlag_Gravity);
			}
		}

	} // for

}

//===========================

// Datas des tourelles.
s32	gpnTurretPosX[B2A_TURRET_NB] = { 68, 75 };
s32	gpnTurretPosY[B2A_TURRET_NB] = { -55, 67 };
u32	gpnTurretSprAng[32] =
{
	8|SPR_Flip_X, 7|SPR_Flip_X, 6|SPR_Flip_X, 5|SPR_Flip_X, 4|SPR_Flip_X, 3|SPR_Flip_X, 2|SPR_Flip_X, 1|SPR_Flip_X,
	0, 1, 2, 3, 4, 5, 6, 7,
	8, 9, 10, 11, 12, 13, 14, 15,
	16, 15|SPR_Flip_X, 14|SPR_Flip_X, 13|SPR_Flip_X, 12|SPR_Flip_X, 11|SPR_Flip_X, 10|SPR_Flip_X, 9|SPR_Flip_X,
};

// Gestion des tourelles.
void B2A_Turret_Manage(struct SMstCommon *pMst, u8 nTurretTouch)
{
	struct SMst20_Boss2A	*pSpe = (struct SMst20_Boss2A *)pMst->pData;
	u32	i, j;
	struct SB2A_Turret	*pTurret;

	// Les tourelles se prennent des tirs ?
	for (i = 0; i < B2A_TURRET_NB; i++)
	if (pSpe->sTurret[i].nLife)
	{
		pTurret = &pSpe->sTurret[i];

		// Tant qu'on y est, on tourne la tourelle vers le joueur.
		if (gShoot.nDeathFlag == 0)
			pTurret->nAngle =
			fatan2(-((gShoot.nPlayerPosY - (2 << 12) + (gShoot.nVehicleType < e_HeroVehicle_SlugBase ? 4 << 12 : 0)) - (pMst->nPosY + (gpnTurretPosY[i] << 8) - (27 << 8)) ),
				gShoot.nPlayerPosX - (pMst->nPosX + (gpnTurretPosX[i] << 8)));

		ChaserTarget_AddToList((pMst->nPosX >> 8) + gpnTurretPosX[i], (pMst->nPosY >> 8) + gpnTurretPosY[i] - 24);	// Ajout dans la table de visée des missiles.

		// Tir ?
		if ((pSpe->nShotFrm & 7) == 0 && (pSpe->nShotFrm & 64))
			pTurret->nShotAnm = AnmSet(gAnm_Boss2A_Turret_ShotSpark, pTurret->nShotAnm);

		if (nTurretTouch)
		{
			// Test des tirs.
//			if (Mst_ShotCheckLife(e_Spr_Boss2Front_Turret, pMst->nPosX + (gpnTurretPosX[i] << 8), pMst->nPosY + (gpnTurretPosY[i] << 8), &pTurret->nHitCnt, &pTurret->nLife))
			if (Mst_ShotCheckLife2(e_Spr_Boss2Front_Turret, pMst->nPosX + (gpnTurretPosX[i] << 8), pMst->nPosY + (gpnTurretPosY[i] << 8), &pTurret->nHitCnt, &pTurret->nLife, e_ShotCheck_MinimizedDamage))
			{
				DustSet(gAnm_Explosion0_Big_Dust, pMst->nPosX + (gpnTurretPosX[i] << 8), pMst->nPosY + (gpnTurretPosY[i] << 8), e_Prio_Ennemies+4, 0);//e_Prio_Ennemies+3
				gShoot.nPlayerScore += B2A_TURRET_SCORE;	// Score.
				if (pTurret->nShotAnm != -1)
				{	// Libère le slot de l'anm d'éclat du tir.
					AnmReleaseSlot(pTurret->nShotAnm);
					pTurret->nShotAnm = -1;
				}
			}
		}
		else
		{
			// Interception des tirs sans dégats.
			u8	nHitCnt, nLife;
			nLife = 0;
			Mst_ShotCheckLife(e_Spr_Boss2Front_Turret, pMst->nPosX + (gpnTurretPosX[i] << 8), pMst->nPosY + (gpnTurretPosY[i] << 8), &nHitCnt, &nLife);
		}

		// Déclenchement d'un tir ?
		if (pTurret->nShotAnm != -1)
		{
			if (AnmGetImage(pTurret->nShotAnm) == -1)
				pTurret->nShotAnm = -1;		// Anim terminée.
			else
			if (AnmCheckStepFlag(pTurret->nShotAnm))
			{	// Déclenchement du tir.
				struct SSprRect sRect1;
				j = gpnTurretSprAng[((pTurret->nAngle + 4) & 0xFF) / 8];
				if (SprGetRect(e_Spr_Boss2Front_Turret + j, e_SprRectZone_ShotOrg, &sRect1))
				if (sRect1.nType == e_SprRect_Rect)
				{
					s32	x1, y1, x2, y2;
					if ((((pTurret->nAngle + 4) & 0xFF) / 64) & 1)		// On coupe en 4, ça marche aussi pour les rectangles (plats) des angles 0, 64, 128 et 192.
					{	// Haut-gauche (1) / Bas-droite (3).
						x1 = sRect1.nX2;
						y1 = sRect1.nY1;
						x2 = sRect1.nX1;
						y2 = sRect1.nY2;
					}
					else
					{	// Haut-droite (0) / Bas-gauche (2).
						x1 = sRect1.nX1;
						y1 = sRect1.nY1;
						x2 = sRect1.nX2;
						y2 = sRect1.nY2;
					}
					FireAdd(e_Shot_Enemy_RebSoldier_Bullet0, pMst->nPosX + ((gpnTurretPosX[i] + x1) << 8), pMst->nPosY + ((gpnTurretPosY[i] + y1) << 8), pTurret->nAngle);
					FireAdd(e_Shot_Enemy_RebSoldier_Bullet0, pMst->nPosX + ((gpnTurretPosX[i] + x2) << 8), pMst->nPosY + ((gpnTurretPosY[i] + y2) << 8), pTurret->nAngle);
				}
			}
		} // if (pSpe->sTurret[i].nShotAnm != -1)

	} // for / if (pSpe->sTurret[i].nLife)
	else
	{
		// Fumée noire quand tourelle détruite.
		if ((gnFrame & 15) == 0)
			DustSetMvt(gAnm_BlackSmoke_Dust, pMst->nPosX + ((gpnTurretPosX[i] - 10) << 8), pMst->nPosY + ((gpnTurretPosY[i] - 22) << 8), 0x200, 0, e_Prio_Ennemies+3, 0);
	}

}

//===========================

// Arrivée du boss (A et B), ralentissement sur la fin du mouvement.
void Boss2_MoveIn(struct SMstCommon *pMst, s32 nSens)
{
	s32	nSpd = pMst->nSpd;
	if (nSpd > 0x200) nSpd = 0x200;
	else if (nSpd < 0x80) nSpd = 0x80;

	pMst->nSpd -= 0x10;
	if (pMst->nSpd < 0x80) pMst->nSpd = 0x80;

//printf("spd=%x\n", pMst->nSpd);

	pMst->nPosX += nSpd * nSens;
}

#define	BOSS2A_PIECESNB	(4)
#define	BOSS2A_AVANCE1X	(SCR_Width - 28)
#define	BOSS2A_AVANCE2X	(SCR_Width - (128 - 20))	// -24
#define	BOSS2A_POSY_MIN	((SCR_Height / 2) - 16)
#define	BOSS2A_POSY_MAX	((SCR_Height / 2) + 15)
s32 Mst20_Main_Boss2A(struct SMstCommon *pMst)
{
	struct SMst20_Boss2A	*pSpe = (struct SMst20_Boss2A *)pMst->pData;
	u32	i, j;
	u32	nInc;
	s32	nDiff;

	// Table de tous les sprites composant le boss, 2 états.
	static u32	gpBoss2ASpr[][BOSS2A_PIECESNB] =
	{
		{ e_Spr_Boss2Front_P0_0, e_Spr_Boss2Front_P1_0, e_Spr_Boss2Front_P2_0, e_Spr_Boss2Front_P3_0 },
		{ e_Spr_Boss2Front_P0_1, e_Spr_Boss2Front_P1_1, e_Spr_Boss2Front_P2_1, e_Spr_Boss2Front_P3_1 },
	};
	static s32	gpBoss2AOffsX[BOSS2A_PIECESNB] = { 0,1,0,1 };	// Offsets x et y des sprites.
	static s32	gpBoss2AOffsY[BOSS2A_PIECESNB] = { 0,0,1,1 };

	u8	nTurretTouch = 0;	// Gestion du touché sur les tourelles.

/*
// tst pour suite.
if (gVar.pKeys[SDLK_j])
{
	gVar.pKeys[SDLK_j] = 0;
	gpMstQuestItems[MST_QUEST_ITEM_MST32]--;	// Relache le scroll.
	return (e_MstState_Dead);
}
*/

	// Bloque le joueur au niveau de l'avion.
	if (pMst->nPhase >= e_MstB2A_Avance1)
	{
		s32	nPlayerSpdX_sav = gShoot.nPlayerSpdX;
		Mst_PlayerBlock(pMst->nPosX - (96 << 8), 0);
		if (nPlayerSpdX_sav < 0) gShoot.nPlayerSpdX = nPlayerSpdX_sav;	// Si vitesse négative, on la remet (évite le "collé").
	}

	// Déplacement pendant les phases d'avance.
	if (pMst->nPhase == e_MstB2A_Avance1 || pMst->nPhase == e_MstB2A_Avance2)
		Boss2_MoveIn(pMst, -1);

	switch (pMst->nPhase)
	{
	case e_MstB2A_ComeDistance:	// L'avion traverse l'écran au loin.
		// Display.
		SprDisplay(e_Spr_Boss2_InTheDistance_FullHealth, pMst->nPosX >> 8, (pMst->nPosY >> 8) - 20, e_Prio_EnnemiesBg);
		SprDisplay(e_Spr_Boss2_InTheDistance_FullHealth+1, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_EnnemiesBg);
		SprDisplay(e_Spr_Boss2_InTheDistance_FullHealth+2, pMst->nPosX >> 8, (pMst->nPosY >> 8) + 30, e_Prio_EnnemiesBg);
		// Avance.
		pMst->nPosX += 0x100;
		if (pMst->nPosX > gScrollPos.nPosX + ((B2A_DISTANCE_POSX_MAX + 32) << 8))
		{
//			gpMstQuestItems[MST_QUEST_ITEM_MST32]++;	// Blocage du scroll.
			pMst->nPhase = e_MstB2A_Avance1; // Phase suivante.
			//
			pMst->nPosX = gScrollPos.nPosX + ((SCR_Width + 72) << 8);
			pMst->nPosY = gScrollPos.nPosY + ((SCR_Height / 2) << 8);
			pMst->nSpd = 0x420;		// Spd empirique pour première avance.
		}
		return (e_MstState_Managed);
		//break;

	case e_MstB2A_Avance1:
		if (pMst->nPosX <= gScrollPos.nPosX + (BOSS2A_AVANCE1X << 8))
		{
			pMst->nPhase = e_MstB2A_Phase1;
			pSpe->nShotFrm = 32;
		}
		break;

	case e_MstB2A_Phase1:	// Canons frontaux.
//if (gVar.pKeys[SDLK_g]) { pSpe->sFrontC[0].nLife = 0; pSpe->sFrontC[1].nLife = 0; gVar.pKeys[SDLK_g] = 0; }

		if (gShoot.nDeathFlag) pSpe->nShotFrm = 32;		// Si héros mort, stoppe les tirs.

		// Gestion des canons frontaux.
		B2A_FrontC_Manage(pMst);

		// Haut/Bas (une fois les canons sortis, et en vie).
		pSpe->nPilotsTarget = 0;
		nInc = (pSpe->nFrontCannonAnm == -1 ? 12 : AnmGetLastImage(pSpe->nFrontCannonAnm));
		if (nInc == 12 && (pSpe->sFrontC[0].nLife || pSpe->sFrontC[1].nLife))
		{
			Boss2_UpDown(&pMst->nPosY, BOSS2A_POSY_MIN, BOSS2A_POSY_MAX, 0x80, &pSpe->nSensY);
			// 'Angle' des pilotes.
			pSpe->nPilotsTarget = (pSpe->nSensY ? 3 << 4 : -3 << 4);
			if ((pSpe->nSensY && pMst->nPosY >> 8 >= BOSS2A_POSY_MAX - 8) ||
				(pSpe->nSensY == 0 && pMst->nPosY >> 8 <= BOSS2A_POSY_MIN + 8)) pSpe->nPilotsTarget = 0;

			pSpe->nShotFrm++;	// Frame cnt pour les tirs.
		}

		// (Plus de vie et) Fin des explosions ?
		if (pSpe->sFrontC[0].nExplo == 0 && pSpe->sFrontC[1].nExplo == 0)
		{
			pMst->nPhase = e_MstB2A_Recule1;
			pSpe->nPilotsCur = 4 << 4;
			pSpe->nPilotsTarget = 7 << 4;
		}
		break;

	case e_MstB2A_Recule1:
		// Recentrage en y.
		nDiff = (gScrollPos.nPosY + ((SCR_Height / 2) << 8)) - pMst->nPosY;
		if (ABS(nDiff) >> 8) pMst->nPosY += SGN(nDiff) << 5;
		// Recule en x.
		pMst->nPosX += 0x40;//0x80;
		if (pMst->nPosX >= gScrollPos.nPosX + (SCR_Width << 8))
		{
			pMst->nSpd = 0x470;		// Spd empirique pour seconde avance.
			pMst->nPhase = e_MstB2A_Avance2;
			pSpe->nShotFrm = 32;	// Init ici, la valeur doit être bonne pendant la phase e_MstB2A_Avance2.
			pSpe->nPilotsTarget = 4 << 4;
		}

// tst de tremblement.
//if ((gnFrame & 3) == 0) pMst->nPosY += (gnFrame & 4 ? -1<<8 : 1<<8);
//if ((gnFrame & 1) == 0) pMst->nPosY += (gnFrame & 2 ? -1<<8 : 1<<8);

		break;

	case e_MstB2A_Avance2:
		if (pMst->nPosX <= gScrollPos.nPosX + (BOSS2A_AVANCE2X << 8)) pMst->nPhase = e_MstB2A_Phase2;
		break;

	case e_MstB2A_Phase2:	// Tourelles.
//if (gVar.pKeys[SDLK_g]) { pSpe->sTurret[0].nLife = 0; pSpe->sTurret[1].nLife = 0; gVar.pKeys[SDLK_g] = 0; }

		nTurretTouch = 1;	// Gestion du touché sur les tourelles.
		pSpe->nShotFrm++;	// Frame cnt pour les tirs.

		if (gShoot.nDeathFlag) pSpe->nShotFrm = 32;		// Si héros mort, stoppe les tirs.

		// Plus de vie ?
		if (pSpe->sTurret[0].nLife == 0 && pSpe->sTurret[1].nLife == 0)
		{
			pMst->nPhase = e_MstB2A_Phase2Explo;
			pSpe->nPilotsTarget = 7 << 4;
			pSpe->nExplo = 100 + MEDIUMEXPLO_ANM_DURATION + (28*2);	// +(28*2) = Temps de l'explosion de la tourelle.
			// Retire les tirs ennemis.
			FireRemoveFromList((1<<31) | e_ShotOrg_Enemy);
		}
		break;

	case e_MstB2A_Phase2Explo:	// Explosions finales.
		// Temps de l'explosion de la tourelle.
		if (pSpe->nExplo > 100 + MEDIUMEXPLO_ANM_DURATION)
		{
			pSpe->nExplo--;
			break;
		}

		// Explosions partout.
		Boss_Explosions(&pSpe->nExplo, gScrollPos.nPosX + ((SCR_Width - 64 - 8) << 8), gScrollPos.nPosY - (16 << 8), 64, 256);
		pSpe->nExplo++;
		Boss_Explosions(&pSpe->nExplo, gScrollPos.nPosX + ((SCR_Width - 128 - 16) << 8), gScrollPos.nPosY - (16 << 8), 64, 256);
		// Decay.
		if (pSpe->nExplo < MEDIUMEXPLO_ANM_DURATION + 32) pSpe->nDecayState = 1;
		// Fin ?
		if (pSpe->nExplo == 0) pMst->nPhase = e_MstB2A_Recule2;
		break;

	case e_MstB2A_Recule2:
		pMst->nPosX += 0x80;
		if (pMst->nPosX >= gScrollPos.nPosX + ((SCR_Width + 72) << 8))
		{
			// On libère les anims.
			if (pSpe->nFrontCannonAnm != -1) AnmReleaseSlot(pSpe->nFrontCannonAnm);
			for (i = 0; i < 2; i++)
			{
				if (pSpe->sFrontC[i].nShotAnm != -1) AnmReleaseSlot(pSpe->sFrontC[i].nShotAnm);
				if (pSpe->sTurret[i].nShotAnm != -1) AnmReleaseSlot(pSpe->sTurret[i].nShotAnm);
			}
			// On relâche le scroll.
			gpMstQuestItems[MST_QUEST_ITEM_MST32]--;
/*
			// On génère le Boss 2B.
			{
				/ *
				[mst20] Boss
				No = 0:3: Boss1 - Boss2A - Boss2B -
				Wait itm = 8:15:		; Item de quête attendu.
				RESERVED1 = 16:31:
				* /
				u32	nPrm = 2;
				MstAdd(e_Mst20_Boss, (gScrollPos.nPosX >> 8) + (SCR_Width / 2), (gScrollPos.nPosX >> 8) + (SCR_Height / 2), (u8 *)&nPrm, -1);
			}
*/
			// Mort.
			return (e_MstState_Dead);
		}
		break;
	}

	// 'Angle' des pilotes.
	if (pSpe->nPilotsCur != pSpe->nPilotsTarget) pSpe->nPilotsCur += (pSpe->nPilotsTarget > pSpe->nPilotsCur ? 2 : -2);

	// Gestion des tourelles.
//	if (pMst->nPhase >= e_MstB2A_Avance2 && pMst->nPhase <= e_MstB2A_Phase2)
	if (pMst->nPhase >= e_MstB2A_Avance2)
		B2A_Turret_Manage(pMst, nTurretTouch);

	// *** Affichage.

	// Affichage de toutes les parties du sprite.
	u32	nFlagHit = (pSpe->nHitCnt ? SPR_Flag_HitPal : 0);
	for (i = 0; i < BOSS2A_PIECESNB; i++)
		//if (gpBoss2ASpr[pSpe->nDecayState][i] != SPR_NoSprite)
			SprDisplay(gpBoss2ASpr[pSpe->nDecayState][i] | nFlagHit, (pMst->nPosX >> 8) + gpBoss2AOffsX[i], (pMst->nPosY >> 8) + gpBoss2AOffsY[i], e_Prio_Ennemies);

	// Les pilotes.
	static u8 pnPilotsAdd[] = { 6,5,4, 0, 7,8,9, 0,1,2,3 };		// 0-2: Haut > neutre / 3: neutre / 4-6: neutre > Bas // 7-10: neutre > arrière.
	i = (u32)pnPilotsAdd[(pSpe->nPilotsCur >> 4) + 3];
	SprDisplay((e_Spr_Boss2Front_PilotTop + i) | nFlagHit, (pMst->nPosX >> 8) - 52, (pMst->nPosY >> 8) - 14, e_Prio_Ennemies-1);
	SprDisplay((e_Spr_Boss2Front_PilotBottom + i) | nFlagHit, (pMst->nPosX >> 8) - 52, (pMst->nPosY >> 8) + 67, e_Prio_Ennemies-1);

	// Affichage des canons frontaux.
	if (pMst->nPhase >= e_MstB2A_Phase1 && pSpe->nDecayState < 1)
	{
		nInc = (pSpe->nFrontCannonAnm == -1 ? 12 : AnmGetImage(pSpe->nFrontCannonAnm));
		if (nInc != SPR_NoSprite)
		for (i = 0; i < B2A_FRONTCANNON_NB; i++)
		{
			if (pSpe->sFrontC[i].nHitCnt) pSpe->sFrontC[i].nHitCnt--;
			nFlagHit = (pSpe->sFrontC[i].nHitCnt ? SPR_Flag_HitPal : 0);
//			j = (pSpe->sFrontC[i].nLife == 0 && pSpe->sFrontC[i].nExplo < 32 ? pnFrontCDeathSpr[i] : pnFrontCBaseSpr[i] + nInc);
			j = (pSpe->sFrontC[i].nExplo < 16 ? gpnFrontCDeathSpr[i] : gpnFrontCBaseSpr[i] + nInc);
			SprDisplay(j | nFlagHit, (pMst->nPosX >> 8) + B2A_FRONTCANNON_OFFSX, (pMst->nPosY >> 8) + gpnFrontCPosY[i], e_Prio_Ennemies+1);
			// Etincelle du tir en plus ?
			if (pSpe->sFrontC[i].nShotAnm != -1)
				SprDisplay(AnmGetLastImage(pSpe->sFrontC[i].nShotAnm), (pMst->nPosX >> 8) + B2A_FRONTCANNON_OFFSX, (pMst->nPosY >> 8) + gpnFrontCPosY[i], e_Prio_Ennemies+2);
		}
	}

	// Tourelles.
	if (pMst->nPhase >= e_MstB2A_Avance2 && pSpe->nDecayState < 1)
	{
		for (i = 0; i < B2A_TURRET_NB; i++)
		if (pSpe->sTurret[i].nLife)
		{
			if (pSpe->sTurret[i].nHitCnt) pSpe->sTurret[i].nHitCnt--;
			nFlagHit = (pSpe->sTurret[i].nHitCnt ? SPR_Flag_HitPal : 0);
			SprDisplay((e_Spr_Boss2Front_Turret + gpnTurretSprAng[((pSpe->sTurret[i].nAngle + 4) & 0xFF) / 8]) | nFlagHit, (pMst->nPosX >> 8) + gpnTurretPosX[i], (pMst->nPosY >> 8) + gpnTurretPosY[i], e_Prio_Ennemies+2);
			// Etincelle du tir en plus ?
			if (pSpe->sTurret[i].nShotAnm != -1)
				SprDisplay((e_Spr_Boss2Front_Turret_ShotSpark + AnmGetLastImage(pSpe->sTurret[i].nShotAnm) + gpnTurretSprAng[((pSpe->sTurret[i].nAngle + 4) & 0xFF) / 8]) | nFlagHit, (pMst->nPosX >> 8) + gpnTurretPosX[i], (pMst->nPosY >> 8) + gpnTurretPosY[i], e_Prio_Ennemies+2);
		}
	}

	return (e_MstState_Managed);
}

//=============================================================================
// Boss 2B - Queue de l'avion.

// Phases.
enum
{
	e_MstB2B_ComeDistance = 0,
	e_MstB2B_Avance1,
	e_MstB2B_Phase1,
	e_MstB2B_Phase2,
	e_MstB2B_Death,
};

// Structure spécifique.
struct SMst20_Boss2B
{
	u8	nReserved_Boss0;	// Pour mappage sur le Mst20 container.
	//
	u8	nDecayState;		// Etat de délabrement [0;1].
	u8	nHitCnt;
	u8	nLife;
	u8	nSensY;

	s8	nFlameShotAnm;

	s8	nTailCur, nTailTarget;	// Pour 'angle' de la queue.
	u8	nCurState;

	u8	nFrmCnt;		// Pour largage des mines.
	u8	nExplo;			// Pour explosions.

};

#define	B2B_DISTANCE_POSX_MIN	(-248)
#define	B2B_DISTANCE_POSX_MAX	(364)
#define	BOSS2B_SCORE	2000
#define	BOSS2B_LIFE	100	//50
void Mst20_Init_Boss2B(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst20_Boss2B	*pSpe = (struct SMst20_Boss2B *)pMst->pData;

	pSpe->nHitCnt = 0;
	pSpe->nLife = BOSS2B_LIFE;

	pSpe->nDecayState = 0;

	pSpe->nFlameShotAnm = -1;
	pSpe->nSensY = 0;
	pSpe->nTailCur = pSpe->nTailTarget = 0;
	pSpe->nExplo = 0;

	pSpe->nCurState = 4;

	pMst->nPhase = e_MstB2B_ComeDistance;

	pMst->nPosX = gScrollPos.nPosX + ((B2B_DISTANCE_POSX_MAX + 32) << 8);
	pMst->nPosY = gScrollPos.nPosY + ((SCR_Height / 2) << 8) - (30 << 8);

	gpMstQuestItems[MST_QUEST_ITEM_MST32]++;	// Bloque le scroll.

}

#define	BOSS2B_AVANCE1X	96
#define	BOSS2B_PIECESNB	(9)
#define	BOSS2B_POSY_MIN	-4
#define	BOSS2B_POSY_MAX	216
#define	BOSS2B_FLAME_OFFSX	(-26)
#define	BOSS2B_FLAME_OFFSY1	(-85)
#define	BOSS2B_FLAME_OFFSY2	(80)
s32 Mst20_Main_Boss2B(struct SMstCommon *pMst)
{
	struct SMst20_Boss2B	*pSpe = (struct SMst20_Boss2B *)pMst->pData;

	// Table de tous les sprites composant le boss, 2 états.
	static u32	gpBoss2BSpr[][BOSS2B_PIECESNB] =
	{
		{ e_Spr_Boss2Back_P0_0, e_Spr_Boss2Back_P1_0, e_Spr_Boss2Back_P2_0, e_Spr_Boss2Back_P3_0, e_Spr_Boss2Back_TailTop+4, e_Spr_Boss2Back_TailBottom+4, e_Spr_Boss2Back_BodyTailBottomPiece2, e_Spr_Boss2Back_WingTop, e_Spr_Boss2Back_WingBottom },
		{ e_Spr_Boss2Back_P0_1, e_Spr_Boss2Back_P1_1, e_Spr_Boss2Back_P2_1, e_Spr_Boss2Back_P3_1, SPR_NoSprite, SPR_NoSprite, SPR_NoSprite, e_Spr_Boss2Back_WingTop, e_Spr_Boss2Back_WingBottom },
	};
	static s32	gpBoss2BOffsX[BOSS2B_PIECESNB] = { 0,1,0,1, 32, 30, 36, -96, -96 };	// Offsets x et y des sprites.
	static s32	gpBoss2BOffsY[BOSS2B_PIECESNB] = { 0,0,1,1, -44, 19, 129, -128, 129 };
	static u16	gpBoss2BPrio[BOSS2B_PIECESNB] = { e_Prio_Ennemies, e_Prio_Ennemies, e_Prio_Ennemies, e_Prio_Ennemies, e_Prio_Ennemies+3, e_Prio_Ennemies, e_Prio_Ennemies, e_Prio_Ennemies, e_Prio_Ennemies };

	u8	nTouch = 0;
	u8	nMaskDisp = 0;
	s32	nDiff;
	u32	i;

/*
// tst pour suite.
if (gVar.pKeys[SDLK_j])
{
	gVar.pKeys[SDLK_j] = 0;
	gpMstQuestItems[MST_QUEST_ITEM_MST32]--;	// Relache le scroll.
	if (pSpe->nFlameShotAnm != -1) AnmReleaseSlot(pSpe->nFlameShotAnm);
	return (e_MstState_Dead);
}
*/

	// Bloque le joueur au niveau de l'avion.
	if (pMst->nPhase >= e_MstB2B_Avance1)
	{
		s32	nPlayerSpdX_sav = gShoot.nPlayerSpdX;
		Mst_PlayerBlock(pMst->nPosX + (164 << 8), 1);
		if (nPlayerSpdX_sav > 0) gShoot.nPlayerSpdX = nPlayerSpdX_sav;	// Si vitesse négative, on la remet (évite le "collé").
	}

	switch (pMst->nPhase)
	{
	case e_MstB2B_ComeDistance:	// L'avion traverse l'écran au loin.
		// Display.
		i = (gnFrame >> 2) & 3;
		SprDisplay(e_Spr_Boss2_InTheDistance_HalfHealth + i, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_EnnemiesBg);
		SprDisplay(e_Spr_Boss2_InTheDistance_HalfHealth_Smoke1 + i, (pMst->nPosX >> 8) + 60, (pMst->nPosY >> 8) - 20, e_Prio_EnnemiesBg);
		SprDisplay(e_Spr_Boss2_InTheDistance_HalfHealth_Smoke2 + i, (pMst->nPosX >> 8) + 60, (pMst->nPosY >> 8) + 30, e_Prio_EnnemiesBg);
		// Avance.
		pMst->nPosX -= 0x140;
		if (pMst->nPosX < gScrollPos.nPosX + ((B2B_DISTANCE_POSX_MIN - 32) << 8))
		{
//			gpMstQuestItems[MST_QUEST_ITEM_MST32]++;	// Blocage du scroll.
			pMst->nPhase = e_MstB2B_Avance1; // Phase suivante.
			//
			pMst->nPosX = gScrollPos.nPosX - (162 << 8);
			pMst->nPosY = gScrollPos.nPosY + ((SCR_Height / 2) << 8);
			pMst->nSpd = 0x920;		// Spd empirique pour première avance.
		}
		return (e_MstState_Managed);
		//break;

	case e_MstB2B_Avance1:		// On se met en postition.
		Boss2_MoveIn(pMst, 1);
		if (pMst->nPosX >= gScrollPos.nPosX + (BOSS2B_AVANCE1X << 8)) pMst->nPhase = e_MstB2B_Phase1;
		break;

	case e_MstB2B_Phase1:		// Flammes.
		if (pSpe->nExplo) break;

		pSpe->nFlameShotAnm = AnmSetIfNew(gAnm_Boss2B_FlameShot, pSpe->nFlameShotAnm);

		// Restart du tir quand l'anim arrive à la fin.
		// Fait comme ça car l'anim qui boucle pose un problème lors de la coupure (l'anim de flammes n'est pas interrompue sur la fin, seulement si il y a un rectangle de col).
		if (pSpe->nFlameShotAnm != -1)
		if (AnmCheckEnd(pSpe->nFlameShotAnm))
		pSpe->nFlameShotAnm = AnmSet(gAnm_Boss2B_FlameShot, pSpe->nFlameShotAnm);

		break;

	case e_MstB2B_Phase2:		// Largage de mines.
		if (pSpe->nExplo) break;

		// Lâcher de mini bombes.
		pSpe->nFrmCnt++;
		if ((pSpe->nFrmCnt & 7) == 0 && (pSpe->nFrmCnt & 64))
			FireAdd(e_Shot_Enemy_HairBusterRibert_Mine, pMst->nPosX - (56 << 8), pMst->nPosY - (28 << 8), 0 + (rand() & 15) + (pSpe->nSensY ? -15 : 0));

		nMaskDisp = 1;	// On demande l'affichage du cache.
		break;

	case e_MstB2B_Death:		// Mort.
		// Recentrage en y.
		nDiff = (gScrollPos.nPosY + ((SCR_Height / 2) << 8)) - pMst->nPosY;
		if (ABS(nDiff) >> 8) pMst->nPosY += SGN(nDiff) << 5;
		// Queue neutre.
		pSpe->nTailTarget = 0;
		// Fin ?
		if (pSpe->nExplo < 100) pSpe->nDecayState = 1;
		if (pSpe->nExplo < 60) gpMstQuestItems[MST_QUEST_ITEM_NEXT_LEVEL] |= 1;	// Item de quête : Fin de niveau.
		break;
	}

	if (pMst->nPhase >= e_MstB2B_Phase1 && pMst->nPhase <= e_MstB2B_Phase2)
	{
		if (pSpe->nExplo == 0)
		{
			nTouch = 1;		// Prise en compte des touchés.
			// Haut/Bas.
			Boss2_UpDown(&pMst->nPosY, BOSS2B_POSY_MIN, BOSS2B_POSY_MAX, 0x80, &pSpe->nSensY);
			// 'Angle' de la queue.
			pSpe->nTailTarget = (pSpe->nSensY ? -4 << 4 : 4 << 4);
			if ((pSpe->nSensY && pMst->nPosY >> 8 >= BOSS2B_POSY_MAX - 16) ||
				(pSpe->nSensY == 0 && pMst->nPosY >> 8 <= BOSS2B_POSY_MIN + 16)) pSpe->nTailTarget = 0;
		}
		else
			pSpe->nTailTarget = 0;
	}
	// 'Angle' de la queue.
	if (pSpe->nTailCur != pSpe->nTailTarget) pSpe->nTailCur += (pSpe->nTailTarget > pSpe->nTailCur ? 2 : -2);
	// Explosions.
	if (pSpe->nExplo)
	{
		Boss_Explosions(&pSpe->nExplo, gScrollPos.nPosX + (16 << 8), gScrollPos.nPosY - (16 << 8), 64, 256);
		pSpe->nExplo++;
		Boss_Explosions(&pSpe->nExplo, gScrollPos.nPosX + (80 << 8), pMst->nPosY - (64 << 8), 128, 128);
	}

	struct SSprRect sRect1;
	// Se prend un tir ?
	{
		// On créé un rectangle de collision.
		sRect1.nType = e_SprRect_Rect;
		sRect1.nX1 = -96;
		sRect1.nX2 = -42;
		sRect1.nY1 = -60;
		sRect1.nY2 = 10;

		if (nTouch)
		{
			// Test des tirs, avec dégats minimisés.
			if (Mst_ShotCheckLifeRect2(&sRect1, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife, e_ShotCheck_MinimizedDamage))
			{
				if (--pSpe->nCurState == 0)
				{	// Mort.
					gShoot.nPlayerScore += BOSS2B_SCORE;	// Score.
					pMst->nPhase = e_MstB2B_Death;
					// Retire les tirs ennemis.
					FireRemoveFromList((1<<31) | e_ShotOrg_Enemy);
					gpMstQuestItems[MST_QUEST_ITEM_NEXT_LEVEL] |= 2;	// Item de quête : Victory.
					pSpe->nExplo = 255;//65 + MEDIUMEXPLO_ANM_DURATION;
				}
				else
				{	// Pas mort.
					pSpe->nHitCnt = MST_HIT_CNT;	// Pas mort, on met le hit cnt.
					pSpe->nLife = BOSS2B_LIFE;
					pMst->nPhase = (pSpe->nCurState & 1 ? e_MstB2B_Phase2 : e_MstB2B_Phase1);
					pSpe->nFrmCnt = 0;
					// Coupure des flammes. (On coupe l'anim s'il y a un rect de col, sinon on laisse la fin jouer).
					if (pMst->nPhase == e_MstB2B_Phase2)
					{
						if (pSpe->nFlameShotAnm != -1)
						if (SprGetRect(AnmGetLastImage(pSpe->nFlameShotAnm), e_SprRectZone_RectCol, &sRect1))
						if (sRect1.nType == e_SprRect_Rect)
							AnmSet(gAnm_Boss2B_FlameShot_Cut, pSpe->nFlameShotAnm);
					}
					pSpe->nExplo = 65 + MEDIUMEXPLO_ANM_DURATION;
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
	}

	// Touché du joueur avec les flammes.
	if (pSpe->nFlameShotAnm != -1 && AnmCheckEnd(pSpe->nFlameShotAnm) == 0)
	{
		//if (pSpe->nFlameShotAnm != -1)
		if (SprGetRect(AnmGetLastImage(pSpe->nFlameShotAnm), e_SprRectZone_RectCol, &sRect1))
		if (sRect1.nType == e_SprRect_Rect)
		{
			Enemy_PlayerBlowRect(&sRect1, pMst->nPosX + (BOSS2B_FLAME_OFFSX << 8), pMst->nPosY + (BOSS2B_FLAME_OFFSY1 << 8), MST_DAMAGE_BULLET);
			Enemy_PlayerBlowRect(&sRect1, pMst->nPosX + (BOSS2B_FLAME_OFFSX << 8), pMst->nPosY + (BOSS2B_FLAME_OFFSY2 << 8), MST_DAMAGE_BULLET);
		}
	}

	// *** Affichage.

	if (pSpe->nHitCnt) pSpe->nHitCnt--;

	// Affichage de toutes les parties du sprite.
	u32	j;
	u32	nFlagHit = (pSpe->nHitCnt ? SPR_Flag_HitPal : 0);
	for (i = 0; i < BOSS2B_PIECESNB; i++)
		if (gpBoss2BSpr[pSpe->nDecayState][i] != SPR_NoSprite)
		{
			j = 0;
			if (i == 4 || i == 5) j = pSpe->nTailCur >> 4;
			SprDisplay((gpBoss2BSpr[pSpe->nDecayState][i] + j) | nFlagHit, (pMst->nPosX >> 8) + gpBoss2BOffsX[i], (pMst->nPosY >> 8) + gpBoss2BOffsY[i], gpBoss2BPrio[i]);
		}
	// Cache des mines pendant les tirs.
	if (nMaskDisp)
		SprDisplay(e_Spr_Boss2Back_P0_ShotMask | nFlagHit, (pMst->nPosX >> 8), (pMst->nPosY >> 8), e_Prio_DustOver - 1);
	// Ventilateur.
	SprDisplay((e_Spr_Boss2Back_Propeller + (pSpe->nDecayState ? 5 : ((gnFrame >> 2) & 3))) | nFlagHit, (pMst->nPosX >> 8) + 45, (pMst->nPosY >> 8) - 14, e_Prio_Ennemies+1);

	// Flame shot.
	if (pSpe->nFlameShotAnm != -1)
	if ((i = AnmGetImage(pSpe->nFlameShotAnm)) != SPR_NoSprite)
	{
		SprDisplay(i, (pMst->nPosX >> 8) + BOSS2B_FLAME_OFFSX, (pMst->nPosY >> 8) + BOSS2B_FLAME_OFFSY1, e_Prio_Ennemies + 2);
		SprDisplay(i, (pMst->nPosX >> 8) + BOSS2B_FLAME_OFFSX, (pMst->nPosY >> 8) + BOSS2B_FLAME_OFFSY2, e_Prio_Ennemies + 2);
	}

	return (e_MstState_Managed);
}

//=============================================================================
// Pseudo Boss 3 - Mothership core.

#define	BOSS3_SCORE		2000

// Phases.
enum
{
	e_MstB3_Idle = 0,
	e_MstB3_Dead,
};

// Struct pour le canon.
struct SB3Cannon
{
	s32	nPosX, nPosY;
	s8	nAnm;
	u8	nPhase;

	u8	nShotSeq;
	u8	nShotCnt;

};

struct SB3Ufo
{
	s32	nPosX, nPosY;
	s8	nAnm;
	u8	nPhase;
	u8	nHitCnt;
	u8	nLife;
	u8	nLR;
};

// Structure spécifique.
struct SMst20_Boss3
{
	u8	nReserved_Boss0;	// Pour mappage sur le Mst20 container.
	//
	u8	nDecayState;		// Etat de délabrement [0;1].
	u8	nHitCnt;
	u8	nLife;
	u8	nExplo;

	struct SB3Cannon	sB3Cannon;
	struct SB3Ufo		sB3Ufo;

};

void B3C_Init(struct SMstCommon *pMst);
void B3U_Init(struct SMstCommon *pMst);

#define	BOSS3_DECAY_MAX	3
#define	BOSS3_LIFE	10
void Mst20_Init_Boss3(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst20_Boss3	*pSpe = (struct SMst20_Boss3 *)pMst->pData;

	pSpe->nHitCnt = 0;
	pSpe->nLife = BOSS3_LIFE;
	pSpe->nDecayState = 0;
	pSpe->nExplo = 0;
	pMst->nPhase = e_MstB3_Idle;

	pMst->nAnm = AnmSet(gAnm_Core_Glow, pMst->nAnm);

	B3C_Init(pMst);		// Init canon.
	B3U_Init(pMst);		// Init UFO.

}

//=====================================
// Canon du Boss 3.

enum
{
	e_MstB3_Cannon_Wait = 0,
	e_MstB3_Cannon_Move,
};

void B3C_Init(struct SMstCommon *pMst)
{
	struct SMst20_Boss3	*pSpe = (struct SMst20_Boss3 *)pMst->pData;
	struct SB3Cannon	*pCan = &pSpe->sB3Cannon;

	pCan->nAnm = AnmSet(gAnm_CoreCannon_Idle, -1);

	pCan->nPosX = gScrollPos.nPosX + ((SCR_Width / 2) << 8);
	pCan->nPosY = gScrollPos.nPosY;// + ((SCR_Width / 2) << 8);;

	pCan->nPhase = e_MstB3_Cannon_Wait;

	pCan->nShotSeq = -1;
	pCan->nShotCnt = 0;

}

void B3C_Manage(struct SMstCommon *pMst)
{
	struct SMst20_Boss3	*pSpe = (struct SMst20_Boss3 *)pMst->pData;
	struct SB3Cannon	*pCan = &pSpe->sB3Cannon;
	u32	nSpr;
	s32	nDiff;

	switch (pCan->nPhase)
	{
	case e_MstB3_Cannon_Wait:
		pCan->nAnm = AnmSet(gAnm_CoreCannon_Idle, pCan->nAnm);
		break;

	case e_MstB3_Cannon_Move:
		{

		// Déplacement vers le joueur.
		static u16	gpB3C_Spd[BOSS3_DECAY_MAX + 1] = { 0x100, 0x140, 0x180, 0 };
		u32	nMaxSpd = (u32)gpB3C_Spd[pSpe->nDecayState];
		nDiff = gShoot.nPlayerPosX - pCan->nPosX;
		nDiff /= 4;//8;
		if (ABS(nDiff) > nMaxSpd) nDiff = SGN(nDiff) * nMaxSpd;
		pCan->nPosX += nDiff;

		// Déclenchement du tir.
		static u8	gpB3C_ShotSeq[BOSS3_DECAY_MAX + 1] = { 0x11, 0x33, 0x77, 0 };
		if (MST_SHOT_COND == 0)		// Ne tire pas quand héros mort.
			pCan->nShotCnt = -1;
		else
		if (++pCan->nShotCnt >= 20)//24)
		{
			pCan->nShotCnt = 0;
			pCan->nShotSeq = (pCan->nShotSeq + 1) & 7;
			if (gpB3C_ShotSeq[pSpe->nDecayState] & (1 << pCan->nShotSeq))
				pCan->nAnm = AnmSet(gAnm_CoreCannon_Shot, pCan->nAnm);
		}

		}
		break;
	}

	nSpr = AnmGetImage(pCan->nAnm);

	// Tir ?
	if (AnmGetKey(pCan->nAnm) == e_AnmKey_Enemy_Key3)
		if (AnmCheckNewImgFlag(pCan->nAnm))
		{	// Déclenchement du tir ?
			struct SSprRect sRect1;
			if (SprGetRect(nSpr, e_SprRectZone_ShotOrg, &sRect1))
			if (sRect1.nType == e_SprRect_Point)
				FireAdd(e_Shot_Enemy_MothershipCore_Bullet, pCan->nPosX + (sRect1.nX1 << 8), pCan->nPosY + (sRect1.nY1 << 8), -1);
		}

	// Affichage.
	SprDisplay(nSpr, pCan->nPosX >> 8, pCan->nPosY >> 8, e_Prio_Ennemies+1);

}

//=====================================
// Mini UFO du Boss 3.

enum
{
	e_MstB3_Ufo_Arrival = 0,
	e_MstB3_Ufo_Wait,
	e_MstB3_Ufo_Move,
	e_MstB3_Ufo_Dead,
};

void B3U_Init(struct SMstCommon *pMst)
{
	struct SMst20_Boss3	*pSpe = (struct SMst20_Boss3 *)pMst->pData;
	struct SB3Ufo	*pUfo = &pSpe->sB3Ufo;

	pUfo->nAnm = AnmSet(gAnm_MiniUFO_Fly, -1);

	pUfo->nPosX = gScrollPos.nPosX + ((4 * SCR_Width / 5) << 8);
//	pUfo->nPosY = gScrollPos.nPosY + ((SCR_Height / 2) << 8);
	pUfo->nPosY = gScrollPos.nPosY;

//	pUfo->nPhase = e_MstB3_Ufo_Wait;
	pUfo->nPhase = e_MstB3_Ufo_Arrival;

	pUfo->nHitCnt = 0;
	pUfo->nLife = BOSS3_LIFE;

}

void B3U_Manage(struct SMstCommon *pMst)
{
	struct SMst20_Boss3	*pSpe = (struct SMst20_Boss3 *)pMst->pData;
	struct SB3Ufo	*pUfo = &pSpe->sB3Ufo;
	u32	nSpr;
	s32	nDiff;

	switch (pUfo->nPhase)
	{
	case e_MstB3_Ufo_Arrival:	// Arrivée.
		if (Transit2D_CheckEnd() == 0) return;

		nDiff = (gScrollPos.nPosY + ((SCR_Height / 2) << 8)) - pUfo->nPosY;
		nDiff /= 4;
		if (ABS(nDiff) > 0x200) nDiff = SGN(nDiff) * 0x200;
		pUfo->nPosY += nDiff;
		if (nDiff < 0x40)
		{
			pUfo->nPhase = e_MstB3_Ufo_Wait;	// Passage en mode normal.
			pSpe->sB3Cannon.nPhase = e_MstB3_Cannon_Move;	// Démarrage du canon.
		}
		break;

	case e_MstB3_Ufo_Wait:
		// Touché ? On part de l'autre côté.
		if (pUfo->nHitCnt)
		{
			pUfo->nLR = (pUfo->nPosX > gScrollPos.nPosX + ((SCR_Width / 2) << 8) ? 0 : 1);
			pUfo->nPhase = e_MstB3_Ufo_Move;
		}
		break;

	case e_MstB3_Ufo_Move:
		// Déplacement vers l'autre côté.
		nDiff = (gScrollPos.nPosX + ((pUfo->nLR ? 4 * SCR_Width / 5 : SCR_Width / 5) << 8)) - pUfo->nPosX;
		nDiff /= 4;
		if (ABS(nDiff) > 0x400) nDiff = SGN(nDiff) * 0x400;
		pUfo->nPosX += nDiff;

		// Arrivé à destination ?
		if (ABS(nDiff) < 0x80) pUfo->nPhase = e_MstB3_Ufo_Wait;
		break;

	case e_MstB3_Ufo_Dead:
		// Tourne le joueur vers le centre de l'écran (çàd vers le Monoeye).
		if (gShoot.nPlayerGnd && gShoot.nPlayerAnmTop == -1)
			gShoot.nPlayerDir = (gShoot.nPlayerPosX > gScrollPos.nPosX + ((SCR_Width / 2) << 8) ? 1 : 0);
		return;
	}

	// Oscillation.
	s32	nPosY_sav = pUfo->nPosY;
	pUfo->nPosY += gVar.pSin[(gnFrame << 1) & 0xFF] << 2;

	// Test des tirs.
	if (Mst_ShotCheckLife2(e_Spr_MiniUFO_Fly, pUfo->nPosX, pUfo->nPosY, &pUfo->nHitCnt, &pUfo->nLife, e_ShotCheck_MinimizedDamage))
	{
		// Mort ?
		if (++pSpe->nDecayState >= BOSS3_DECAY_MAX)
		{
			pMst->nPhase = e_MstB3_Dead;					// Mst.
			pSpe->nExplo = 65 + MEDIUMEXPLO_ANM_DURATION;	// Explosions sur le core.
			// Retire les tirs ennemis.
			FireRemoveFromList((1<<31) | e_ShotOrg_Enemy);
			//
			pSpe->sB3Cannon.nPhase = e_MstB3_Cannon_Wait;	// Arrêt du canon (et du tir en cours).
			gShoot.nVehicleAutoPilot = 0x80;	// Immobilise le joueur.
			//
			pUfo->nPhase = e_MstB3_Ufo_Dead;	// Ufo.
			pUfo->nHitCnt = 0;
			gShoot.nPlayerScore += BOSS3_SCORE;	// Score.
			DustSet(gAnm_MiniUFO_Explode_Dust, pUfo->nPosX, pUfo->nPosY, e_Prio_Ennemies + 1, 0);
			return;
		}
		else
		{
			pSpe->nExplo = 33 + MEDIUMEXPLO_ANM_DURATION;	// Explosions sur le core.
			//
			pUfo->nLife = BOSS3_LIFE;		// Reset energy.
			pUfo->nHitCnt = MST_HIT_CNT;	// Pas encore mort, on met le hit cnt.
		}
	}

	nSpr = AnmGetImage(pUfo->nAnm);

	// Affichage.
	if (pUfo->nHitCnt) pUfo->nHitCnt--;
	u32	nFlagHit = (pUfo->nHitCnt ? SPR_Flag_HitPal : 0);
	SprDisplay(nSpr | nFlagHit, pUfo->nPosX >> 8, pUfo->nPosY >> 8, e_Prio_Ennemies + 1);

	pUfo->nPosY = nPosY_sav;	// Restauration Y original.

}

//=====================================

#define	BOSS3_PIECESNB	3
s32 Mst20_Main_Boss3(struct SMstCommon *pMst)
{
	struct SMst20_Boss3	*pSpe = (struct SMst20_Boss3 *)pMst->pData;

	static u32	gpBoss3Spr[BOSS3_DECAY_MAX + 1][BOSS3_PIECESNB] =
	{
		{ e_Spr_Core_Base, SPR_NoSprite, SPR_NoSprite },	//	{ e_Spr_Core_Base, e_Spr_Core_CoreGlow, SPR_NoSprite },
		{ e_Spr_Core_Base, SPR_NoSprite, SPR_NoSprite },
		{ e_Spr_Core_Base, SPR_NoSprite, SPR_NoSprite },
		{ e_Spr_Core_BaseDestroyed_fg, e_Spr_Core_CoreDestroyed, e_Spr_Core_BaseDestroyed_bg },
	};
	u32	i;
	u32	nSpr2 = SPR_NoSprite;

	u8	nTouch = 0;

	B3U_Manage(pMst);	// Gestion du mini UFO.
	B3C_Manage(pMst);	// Gestion du canon.

	switch (pMst->nPhase)
	{
	case e_MstB3_Idle:	// Le coeur attend.
		nTouch = !pSpe->nExplo;

//		nSpr2 = (pMst->nAnm == -1 ? e_Spr_Core_CoreGlow : AnmGetImage(pMst->nAnm));
		// Glow + truc de lamer pour accélérer l'anim.
		nSpr2 = e_Spr_Core_CoreGlow;
		if (pMst->nAnm != -1)
		{
			i = AnmGetLastImage(pMst->nAnm);
			nSpr2 = AnmGetImage(pMst->nAnm);
			if (nSpr2 != i)
			{
				if (pSpe->nDecayState >= BOSS3_DECAY_MAX - 2) AnmGetImage(pMst->nAnm);
				if (pSpe->nDecayState >= BOSS3_DECAY_MAX - 1) AnmGetImage(pMst->nAnm);
			}
		}
		break;

	case e_MstB3_Dead:	// Mort.
		// Après la dernière explosion....
		if (pSpe->nExplo == 1)
		{
			// On génère un Monoeye (place holder).
			/*
			[mst48] Jokes
			Type = 0:6: Rebel Sunbath - L11 Mothership - L2 Clouds - L6 Main Door - L6 Computer - L8 Wagon Door - L17 Monoeye -
			Flip X = 7:7: No - Yes
			prm0 = 8:15:
			prm1 = 16:23:
			*/
			u32	nPrm = 6;
			MstAdd(e_Mst48_Joke0, pMst->nPosX >> 8, pMst->nPosY >> 8, (u8 *)&nPrm, -1);
		}
		break;
	}

	// Explosions.
	if (pSpe->nExplo)
		Boss_Explosions(&pSpe->nExplo, pMst->nPosX - ((64-8) << 8), pMst->nPosY - (80 << 8), 128, 64);

/*
	// Se prend un tir ?
	if (nTouch)
	{
		// Test des tirs, avec dégats minimisés.
		if (Mst_ShotCheckLife2(e_Spr_Core_Base, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife, e_ShotCheck_MinimizedDamage))
		{
			pSpe->nExplo = 65 + MEDIUMEXPLO_ANM_DURATION;
			// Mort ?
			if (++pSpe->nDecayState >= BOSS3_DECAY_MAX)
			{
				pMst->nPhase = e_MstB3_Dead;
				pSpe->nHitCnt = 0;
				gShoot.nPlayerScore += BOSS3_SCORE;	// Score.
				DustSet(gAnm_Explosion0_Big_Dust, pMst->nPosX + (8 << 8), pMst->nPosY - (48 << 8), e_Prio_Ennemies + 2, 0);

				nSpr2 = SPR_NoSprite;	// Plus de glow.
			}
			else
			{
				pSpe->nLife = BOSS3_LIFE;		// Reset energy.
				pSpe->nHitCnt = MST_HIT_CNT;	// Pas vraiment mort, on met le hit cnt.
			}
		}
	}
	else
	{
		// Interception des tirs sans dégats.
		u8	nHitCnt, nLife;
		nLife = 0;
		Mst_ShotCheckLife2(e_Spr_Core_Base, pMst->nPosX, pMst->nPosY, &nHitCnt, &nLife, 0);
	}
*/

	// *** Affichage.

	if (pSpe->nHitCnt) pSpe->nHitCnt--;

	// Affichage de toutes les parties du sprite.
	u32	nFlagHit = (pSpe->nHitCnt ? SPR_Flag_HitPal : 0);
	for (i = 0; i < BOSS3_PIECESNB; i++)
		if (gpBoss3Spr[pSpe->nDecayState][i] != SPR_NoSprite)
			SprDisplay((gpBoss3Spr[pSpe->nDecayState][i]) | nFlagHit, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies - 1 - i);
	// Spr en plus (glow).
	if (nSpr2 != SPR_NoSprite)
		SprDisplay(nSpr2 | nFlagHit, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies - 2);

	return (e_MstState_Managed);
}

//=============================================================================
// "Container" des boss.

/*
[mst20] Boss
No = 0:3: Boss1 - Boss2A - Boss2B - Boss3 -
Wait itm = 8:15:		; Item de quête attendu.
RESERVED1 = 16:31:
*/

struct SMst20_Boss0
{
	u8	nBossNo;		// N° du boss.
};

void Mst20_Init_Boss0(struct SMstCommon *pMst, u8 *pData)
{
	static void (*gpBossFctInit[]) (struct SMstCommon *pMst, u8 *pData) =
	{
		Mst20_Init_Boss1, Mst20_Init_Boss2A, Mst20_Init_Boss2B, Mst20_Init_Boss3,
	};
	struct SMst20_Boss0	*pSpe = (struct SMst20_Boss0 *)pMst->pData;
	u32	nVal;

	nVal = GetBits(0, 3, pData, 0);
	pSpe->nBossNo = nVal;
	assert(pSpe->nBossNo < 4);

	gpBossFctInit[pSpe->nBossNo](pMst, pData);
}

// faire un pointeur de fonction qui envoie vers le bon boss.
s32 Mst20_Main_Boss0(struct SMstCommon *pMst)
{
	static s32 (*gpBossFctMain[]) (struct SMstCommon *pMst) =
	{
		Mst20_Main_Boss1, Mst20_Main_Boss2A, Mst20_Main_Boss2B, Mst20_Main_Boss3,
	};
	struct SMst20_Boss0	*pSpe = (struct SMst20_Boss0 *)pMst->pData;

	return (gpBossFctMain[pSpe->nBossNo](pMst));
}

//=============================================================================

/*
struct SAAF { u8	nTb0[80]; };
assert(sizeof(struct SAAF) < MST_COMMON_DATA_SZ);
*/

#ifndef NDEBUG
// Debug, vérification de la taille des structures.
void BossesCheckStructSizes(void)
{
	assert(sizeof(struct SMst20_Boss0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst20_Boss1) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst20_Boss2A) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst20_Boss2B) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst20_Boss3) < MST_COMMON_DATA_SZ);

}
#endif

