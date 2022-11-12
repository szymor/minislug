
#include "includes.h"
#include "monsters_inc.h"

//=============================================================================

// Variables générales spécifiques.
/*
#define	MST_SEQ_MAX	16
struct SMstMisc
{
	u8	pSeqCount[MST_SEQ_MAX];		// Compte des séquences/séries de monstres. Pour donner les bonus, ne finit pas forcément à 0.
	u8	pSeqCount2[MST_SEQ_MAX];	// Compte des séquences/séries de monstres. Pour compte général, finit à 0 (L11).
	u8	nSeqCur;
//	s32	pSeqScroll[MST_SEQ_MAX];	// Pour décalage écran de la séquence complète (niveau de la fusée).
	u32	pSeqLeader[MST_SEQ_MAX];	// Pour recherche des leaders (32 mst max dans la série !).
};
*/
struct SMstMisc	gMstMisc;

// Initialise une séquence et renvoie son n° d'index.
// Il n'y a plus qu'à décrémenter quand un monstre de la série meure. Quand 0, bonus (ou autre).
u32	SeqInit(u32 nNbInSeq)
{
	gMstMisc.nSeqCur = (gMstMisc.nSeqCur + 1) & (MST_SEQ_MAX - 1);	// Pas besoin d'init !
	gMstMisc.pSeqCount[gMstMisc.nSeqCur] = nNbInSeq;
	gMstMisc.pSeqCount2[gMstMisc.nSeqCur] = nNbInSeq;
	gMstMisc.pSeqLeader[gMstMisc.nSeqCur] = 0;
	return (gMstMisc.nSeqCur);
}

//=============================================================================
// Mst 21 : Scroll Lock.

/*
[mst21] Scroll Y
OffsetY_L = -0:7:		; Offset par rapport au monstre / côté gauche.
BlkHt_L(14) = 8:15:		; Hauteur en blocs du débattement / côté gauche.
OffsetY_R = -16:23:		; Offset par rapport au monstre / côté droite.
BlkHt_R(14) = 24:31:		; Hauteur en blocs du débattement / côté droite.
*/

// tst
extern s32	gnScrollLimitXMin;
extern s32	gnScrollLimitXMax;
extern s32	gnScrollLimitYMin;
extern s32	gnScrollLimitYMax;

// Structure spécifique.
struct SMst21_ScrollY1
{
	s8	nOffsetY[2];	// Offset par rapport au monstre / gauche / droite.
	u8	nBlkHt[2];	// Hauteur en blocs du débattement / gauche / droite.
};

void Mst21_Init_ScrollY1(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst21_ScrollY1	*pSpe = (struct SMst21_ScrollY1 *)pMst->pData;
	s32	nVal;

	pMst->nPosX &= ~0xFFF;	// Recalage sur la colonne 0.
	pMst->nPosY &= ~0xFFF;	// Recalage sur la ligne 0.

	nVal = GetBits(0, 7, pData, 1);		// Offset par rapport au monstre / côté gauche.
	pSpe->nOffsetY[0] = nVal;
	nVal = GetBits(8, 15, pData, 0);	// Hauteur en blocs à bloquer / côté gauche.
	if (nVal < SCR_Height / 16) nVal = SCR_Height / 16;
	pSpe->nBlkHt[0] = nVal;

	nVal = GetBits(16, 23, pData, 1);	// Offset par rapport au monstre / côté droit.
	pSpe->nOffsetY[1] = nVal;
	nVal = GetBits(24, 31, pData, 0);	// Hauteur en blocs à bloquer / côté droit.
	if (nVal < SCR_Height / 16) nVal = SCR_Height / 16;
	pSpe->nBlkHt[1] = nVal;

	// Sens pour savoir dans quel sens tester le dépassement : 0 droite / 1 gauche.
	pMst->nFlipX = (pMst->nPosX >= gScrollPos.nPosX ? 0 : 1);

	// Cas particulier pour initialiser un "rail" en début de niveau.
	if (pMst->nPosX == 0)// && gScrollPos.nPosX == 0)
	{
		gnScrollLimitYMin = pMst->nPosY + ((s32)pSpe->nOffsetY[0] << 12);
		gnScrollLimitYMax = gnScrollLimitYMin + ((u32)pSpe->nBlkHt[0] << 12);
	}

}

s32 Mst21_Main_ScrollY1(struct SMstCommon *pMst)
{
	struct SMst21_ScrollY1	*pSpe = (struct SMst21_ScrollY1 *)pMst->pData;
	u32	nIdx;

	// Sortie de l'écran ?
	if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);

	// A gauche ou à droite ?
	nIdx = (pMst->nPosX >= gScrollPos.nPosX ? 0 : 1);

	// Done ?
	if (nIdx == pMst->nFlipX) return (e_MstState_Managed);

	pMst->nFlipX = 2;	// A partir du moment ou on n'est plus du bon côté, fx=2 car il faudra absolument finir le mvt.

	if (pSpe->nBlkHt[nIdx] == 0xFF)
	{	// On passe en mode sans limites.
		gnScrollLimitYMin = gnScrollLimitYMax = -1;
		pMst->nFlipX = (pMst->nPosX >= gScrollPos.nPosX ? 0 : 1);
	}
	else
	{	// Une limite est donnée.
		s32	nTarget;
		nTarget = pMst->nPosY + ((s32)pSpe->nOffsetY[nIdx] << 12);		// Cible du déplacement.

		// Cas particulier, le scroll n'était pas contraint. On prend la position actuelle.
		if (gnScrollLimitYMin == -1)
		{
			gnScrollLimitYMin = gScrollPos.nPosY;
			gnScrollLimitYMax = gnScrollLimitYMin + ((u32)pSpe->nBlkHt[nIdx] << 12);
		}

		// Bit de flip une fois la ligne cible atteinte.
		if (gnScrollLimitYMin == nTarget)
		{
			pMst->nFlipX = (pMst->nPosX >= gScrollPos.nPosX ? 0 : 1);
			return (e_MstState_Managed);
		}

		// Déplacement jusqu'à la position voulue.
		s32	nDiff;
		nDiff = ABS(gnScrollLimitYMin - nTarget);
		if (nDiff > 0x100) nDiff = 0x100;
		gnScrollLimitYMin += (gnScrollLimitYMin > nTarget ? -nDiff : nDiff);
		gnScrollLimitYMax = gnScrollLimitYMin + ((u32)pSpe->nBlkHt[nIdx] << 12);

	}

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 22 : Exploding Barrel.

/*
[mst22] Exp barrel
Type = 0:7: Lev13_Barrel -
Give itm1 = 8:15:		; 1er item donné.
Give itm2 = 16:23:		; 2ème item donné après quelques frames.
*/

// Phases.
enum
{
	e_Mst22_Wait1 = 0,
	e_Mst22_Wait2,
};

// Structure spécifique.
struct SMst22_ExplodingBarrel0
{
	u8	nHitCnt, nLife;

	u8	nItmGive1;
	u8	nItmGive2;		// N° de l'item à donner AVANT l'item 1.
};

void Mst22_Init_ExplodingBarrel0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst22_ExplodingBarrel0	*pSpe = (struct SMst22_ExplodingBarrel0 *)pMst->pData;
	s32	nVal;

	static u32	pMst22_SprTb[] = { e_Spr_Lev13_Barrel0, };

	nVal = GetBits(0, 3, pData, 0);		// Type.
	pMst->nAnm = pMst22_SprTb[nVal];

	nVal = GetBits(4, 7, pData, 0);		// Life.
	assert(nVal != 0);
	pSpe->nLife = nVal;
	pSpe->nHitCnt = 0;

	nVal = GetBits(8, 15, pData, 0);	// 1er item donné.
	pSpe->nItmGive1 = nVal;
	nVal = GetBits(16, 23, pData, 0);	// 2ème item donné.
	pSpe->nItmGive2 = nVal;

	Enemy_GroundLevel(pMst);			// Mise au niveau du sol.

	pMst->nPhase = e_Mst22_Wait1;

}

s32 Mst22_Main_ExplodingBarrel0(struct SMstCommon *pMst)
{
	struct SMst22_ExplodingBarrel0	*pSpe = (struct SMst22_ExplodingBarrel0 *)pMst->pData;

	switch (pMst->nPhase)
	{
	case e_Mst22_Wait1:
		// Sortie de l'écran ?
		if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) { pMst->nAnm = -1; return (e_MstState_Asleep); }

		// Dégats.
		if (gpMstQuestItems[pSpe->nItmGive1] ||
			Mst_ShotCheckLife(pMst->nAnm, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife))
		{
			// Ajoute une explosion, on est gentils, ça fait un tir joueur.
			FireAdd(e_Shot_Player_MediumExplo_LargeTouch, pMst->nPosX, pMst->nPosY, -1);
			// Donne l'item 2.
			if (pSpe->nItmGive2) gpMstQuestItems[pSpe->nItmGive2] = 1;
			// Phase suivante pour donner l'item 1.
			pMst->nPhase = e_Mst22_Wait2;
		}
		break;

	case e_Mst22_Wait2:
		// Donne l'item 1.
		gpMstQuestItems[pSpe->nItmGive1] = 1;
		// Mort.
		pMst->nAnm = -1; return (e_MstState_Dead);
		break;
	}

	// Affichage.
	if (pSpe->nHitCnt) pSpe->nHitCnt--;
	SprDisplay(pMst->nAnm | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies - 1);// + gnMstPrio);

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 23 : Falling objects.

/*
[mst23] Falling objects
Type = 0:7: Lev13_Hanging_Rock -
OffsetX = 8:11:			; Offset x.
OffsetY = 12:15:		; Offset y.
Wait itm = 16:23:		; Item attendu pour chuter.
*/

u32 Fire_Mst23Get(u32 nSlotNo, s32 **ppnPosX, s32 **ppnPosY, u8 **ppnPlyr);
void FireReleaseSlot(u32 nSlotNo);

// Phases.
enum
{
	e_Mst23_Wait = 0,
	e_Mst23_Fall,
	e_Mst23_Land_Reinit,
	e_Mst23_Land,
};

// Structure spécifique.
struct SMst23_FallingObjects0
{
	u8	nType;
	u8	nItmWait;
	u8	nHardSprSlot;
	s8	nFireSlot;

};

u64	*gpMst23_HangAnmTb[] = { gAnm_Lev13_HangingRock_Wait, };
u64	*gpMst23_FallAnmTb[] = { gAnm_Lev13_HangingRock_Fall, };
u64	*gpMst23_LandAnmTb[] = { gAnm_Lev13_HangingRock_Land, };
u32	gpMst23_HardSprTb[] = { e_Spr_Lev13_Rock1_Landing_pf, };

void Mst23_Init_FallingObjects0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst23_FallingObjects0	*pSpe = (struct SMst23_FallingObjects0 *)pMst->pData;
	s32	nVal;

	nVal = GetBits(0, 7, pData, 0);		// Type.
	pSpe->nType = nVal;
	AnmSet(gpMst23_HangAnmTb[pSpe->nType], pMst->nAnm);

	nVal = GetBits(8, 11, pData, 0);	// Offset X.
	pMst->nPosX &= ~0xFFF;
	pMst->nPosX += nVal << 8;
	nVal = GetBits(12, 15, pData, 0);	// Offset Y.
	pMst->nPosY &= ~0xFFF;
	pMst->nPosY += nVal << 8;

	pSpe->nFireSlot = -1;

	nVal = GetBits(16, 23, pData, 0);	// Item attendu.
	pSpe->nItmWait = nVal;

	pMst->nPhase = e_Mst23_Wait;
	// Déjà tombé ?
	if (gpMstQuestItems[pSpe->nItmWait])
	{
		Enemy_GroundLevel(pMst);			// Mise au niveau du sol.
		pMst->nPhase = e_Mst23_Land_Reinit;//e_Mst23_Fall;		// Enchainera directement en "Land" + mettra le hard sprite.
	}

}

s32 Mst23_Main_FallingObjects0(struct SMstCommon *pMst)
{
	struct SMst23_FallingObjects0	*pSpe = (struct SMst23_FallingObjects0 *)pMst->pData;
	u32	nSpr;
	u16	nPrio;

	// Sprite.
	nSpr = AnmGetImage(pMst->nAnm);
	nPrio = e_Prio_Joueur + 10;		// Wait + fall, au dessus des ennemis.

	switch (pMst->nPhase)
	{
	case e_Mst23_Wait:		// Attente suspendu.
		// Sortie de l'écran ?
		if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);
		// Déclenchement de la chute ?
		if (gpMstQuestItems[pSpe->nItmWait])
		{
			AnmSet(gpMst23_FallAnmTb[pSpe->nType], pMst->nAnm);
			pMst->nPhase = e_Mst23_Fall;	// Chute.
			// Ajout du tir.
			pSpe->nFireSlot = FireAdd(e_Shot_Enemy_HangingRock_Fall, pMst->nPosX, pMst->nPosY, -1);
		}
		break;

	case e_Mst23_Fall:			// Chute.
	case e_Mst23_Land_Reinit:	// Ré-init au sol.
		Mst_Gravity(pMst);
		if (pMst->nGround)
		{
			// Pas de test de surface, les rochers tombent tous dans l'eau => plouf.
			if (pMst->nPhase == e_Mst23_Fall)	// Pas si réinit.
				DustSet(gAnm_Slug_Land_Water_Dust, pMst->nPosX, pMst->nPosY, e_Prio_Joueur + 10, 0);
			// Atterrissage.
			AnmSet(gpMst23_LandAnmTb[pSpe->nType], pMst->nAnm);
			pMst->nPhase = e_Mst23_Land;
			// Ajout du hard sprite.
			pSpe->nHardSprSlot = HardSpr_AddToList(gpMst23_HardSprTb[pSpe->nType], pMst->nPosX >> 8, pMst->nPosY >> 8, 0);
			// Suppression du tir.
			if (pSpe->nFireSlot != -1) FireReleaseSlot(pSpe->nFireSlot);

		}
		else
		{
			// Déplacement du tir, suit le rocher pendant sa chute.
			if (pSpe->nFireSlot != -1)
			{
				s32	*pnPosX, *pnPosY;
				u8	*pnPlyr;
				if (Fire_Mst23Get(pSpe->nFireSlot, &pnPosX, &pnPosY, &pnPlyr))
				{
					*pnPosX = pMst->nPosX;
					*pnPosY = pMst->nPosY;
					*pnPlyr = (gnFrame & 1 ? e_ShotOrg_Enemy : e_ShotOrg_Player);	// On alterne le touché ennemis/joueur.
				}
			}

		}
		break;

	case e_Mst23_Land:		// Au sol. Hard spr.
		// Sortie de l'écran ?
		if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY))
		{
			HardSpr_ReleaseSlot(pSpe->nHardSprSlot);
			return (e_MstState_Asleep);
		}
		nPrio = e_Prio_Ennemies - 1;	// Au sol, en dessous.
		break;
	}

	// Affichage.
	SprDisplay(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8, nPrio);

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 24 : Hard objects.

/*
[mst24] Hard objects
Type = 0:7: Lev13_Rock - SandBagsHeap -
*/

// Structure spécifique.
struct SMst24_HardObjects0
{
	u8	nHardSprSlot;
};

void Mst24_Init_HardObjects0(struct SMstCommon *pMst, u8 *pData)
{
	static u32	pMst24_SprTb[] = { e_Spr_Lev13_Rock0, e_Spr_Lev13_SandBagsHeap0 };
	static u32	pMst24_SprPfTb[] = { e_Spr_Lev13_Rock0_pf, e_Spr_Lev13_SandBagsHeap0_pf };

	struct SMst24_HardObjects0	*pSpe = (struct SMst24_HardObjects0 *)pMst->pData;
	s32	nVal;

	Enemy_GroundLevel(pMst);			// Mise au niveau du sol.

	nVal = GetBits(0, 7, pData, 0);		// Type.
	pMst->nAnm = pMst24_SprTb[nVal];
	// On sauvegarde le slot du hard sprite.
//	pSpe->nHardSprSlot = HardSpr_AddToList(pMst->nAnm + 1, pMst->nPosX >> 8, pMst->nPosY >> 8, 0);
	pSpe->nHardSprSlot = HardSpr_AddToList(pMst24_SprPfTb[nVal], pMst->nPosX >> 8, pMst->nPosY >> 8, 0);

}

s32 Mst24_Main_HardObjects0(struct SMstCommon *pMst)
{
	struct SMst24_HardObjects0	*pSpe = (struct SMst24_HardObjects0 *)pMst->pData;

	// Sortie de l'écran ?
	if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY))
	{
		pMst->nAnm = -1;	// RAZ du slot d'anim utilisé par un n° de spr.
		HardSpr_ReleaseSlot(pSpe->nHardSprSlot);	// Supprime le hard spr de la table.
		return (e_MstState_Asleep);
	}

	// Affichage.
	SprDisplay(pMst->nAnm, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_DustUnder - 1);//e_Prio_Ennemies - 3);//1);// + gnMstPrio);

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 25 : Rocket Diver.

// Phases.
enum
{
	e_Mst25_Wait = 0,
	e_Mst25_Jump,
	e_Mst25_Death,
};

// Structure spécifique.
struct SMst25_RocketDiver0
{
	u16	nPosYOrg;
	u8	nFrom;
	u8	nLoaded;	// Porte ou non la roquette (càd, l'a-t-on déjà lancée ou pas, pour la faire dégager si hit).
};

void Mst25_Init_RocketDiver0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst25_RocketDiver0	*pSpe = (struct SMst25_RocketDiver0 *)pMst->pData;
	s32	nVal;

	nVal = GetBits(0, 0, pData, 0);		// From.
	pSpe->nFrom = nVal;
	pSpe->nLoaded = 1;		// A l'init, porte la roquette.

	Enemy_GroundLevel(pMst);			// Mise au niveau du sol.
	pSpe->nPosYOrg = pMst->nPosY >> 8;	// On note le Y original.

	pMst->nPhase = e_Mst25_Wait;

}

s32 Mst25_Main_RocketDiver0(struct SMstCommon *pMst)
{
	struct SMst25_RocketDiver0	*pSpe = (struct SMst25_RocketDiver0 *)pMst->pData;
	u32	nSpr = SPR_NoSprite;
	u32	nDamage;

	// Sprite.
	if (pMst->nPhase != e_Mst25_Wait) nSpr = AnmGetImage(pMst->nAnm);

	switch (pMst->nPhase)
	{
	case e_Mst25_Wait:		// Attente que le joueur soit à proximité.
		// Sortie de l'écran ?
		if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);
		// Joueur assez proche ?
		if ( (pSpe->nFrom == 0 && (pMst->nPosX - gShoot.nPlayerPosX) < (12 << 12)) ||
			 (pSpe->nFrom == 1 && (gShoot.nPlayerPosX - pMst->nPosX) > (4 << 12)) )
		{
			pMst->nPhase = e_Mst25_Jump;
			pMst->nSpdY = -0x600;
//w			DustSet(gAnm_RocketDiver_WaterSplash_Dust, pMst->nPosX, pMst->nPosY, e_Prio_Joueur + 10, 0);
			DustSet(gAnm_RocketDiver_WaterSplash_Dust, pMst->nPosX, (gShoot.nSplashLevel == -1 ? pMst->nPosY : (gShoot.nSplashLevel - 8) << 8), e_Prio_DustOver, 0);
		}
		return (e_MstState_Managed);
		break;

	case e_Mst25_Jump:		// Saut.
		// Se tourne vers le joueur.
		pMst->nFlipX = (pMst->nPosX >= gShoot.nPlayerPosX ? 0 : 1);

		// Pseudo-gravité, sans tester les blocs de sol.
		pMst->nSpdY += GRAVITY/2;
		if (pMst->nSpdY > SPDY_MAX/2) pMst->nSpdY = SPDY_MAX/2;
		pMst->nPosY += pMst->nSpdY;

		//Mst_ProximityCheck(pMst);	// Donne le signal au joueur pour un éventuel coup de couteau. => Pas évident de toucher avec le coup de couteau.
		ChaserTarget_AddToList(pMst->nPosX >> 8, (pMst->nPosY >> 8) - 16);	// Ajout dans la table de visée des missiles.

		// Se prend un tir ?
//s		nSpr = AnmGetLastImage(pMst->nAnm);
		nDamage = FireHitCheck(nSpr, pMst->nPosX, pMst->nPosY, e_ShotOrg_Player, 0);
		if (nDamage)
		{
			// La roquette qui dégage.
			if (pSpe->nLoaded)
			{
				gDustExg.nRotInc = SGN(pMst->nPosX - gShoot.nPlayerPosX) * -3;
//w				gDustExg.pLinkAnm = gAnm_RocketDiver_WaterSplash_Dust;
				gDustExg.pLinkAnm = (gShoot.nSplashLevel == -1 ? gAnm_ShotEnemy_Explosion0_Medium : gAnm_RocketDiver_WaterSplash_Dust);
				DustSetMvt(gAnm_RocketDiver_RocketRot_Dust, pMst->nPosX, pMst->nPosY, SGN(pMst->nPosX - gShoot.nPlayerPosX) * 0x300, -0x280, e_Prio_DustUnder, e_DustFlag_Gravity | e_DustFlag_Rotation | e_DustFlag_LinkAnm);
				// Score.
				gShoot.nPlayerScore += 50;		// Bonus si on bute le monstre avec sa roquette attachée.
			}
			// Le perso.
			pMst->nSpd = SGN(pMst->nPosX - gShoot.nPlayerPosX) * 0x200;
			pMst->nSpdY = -0x280;
			pMst->nGround = 0;
			AnmSetIfNew(gAnm_RocketDiver_DeathFall, pMst->nAnm);
			pMst->nPhase = e_Mst25_Death;
			// Score.
			gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;
			break;
		}

		// Tir ?
		if (pMst->nSpdY < 0 && pMst->nSpdY > -0x180) AnmSetIfNew(gAnm_RocketDiver_Attack, pMst->nAnm);	// Anim.
		if (Mst_ShotLaunch(pMst, nSpr, e_Shot_Enemy_RocketDiver_Rocket0, NULL)) pSpe->nLoaded = 0;		// Tir.

		// Retouche le sol ? (dépasse le Y original).
		if (pMst->nPosY >> 8 >= pSpe->nPosYOrg)
		{
//w			DustSet(gAnm_RocketDiver_WaterSplash_Dust, pMst->nPosX, pMst->nPosY, e_Prio_Joueur + 10, 0);
			DustSet(gAnm_RocketDiver_WaterSplash_Dust, pMst->nPosX, (gShoot.nSplashLevel == -1 ? pMst->nPosY : (gShoot.nSplashLevel - 8) << 8), e_Prio_DustOver, 0);
			return (e_MstState_Dead);
		}
		break;

	case e_Mst25_Death:
		// Dépl. X + gravité.
		pMst->nPosX += pMst->nSpd;
		Mst_Gravity(pMst);
		// Retouche le sol ? (dépasse le Y original).
		if (pMst->nPosY >> 8 >= pSpe->nPosYOrg)
		{
			DustSet(gAnm_RocketDiver_WaterSplash_Dust, pMst->nPosX, pMst->nPosY, e_Prio_Joueur + 10, 0);
			return (e_MstState_Dead);
		}
		// Sol ?
		if (pMst->nGround)
		{
			DustSet(gAnm_RocketDiver_DeathLand_Dust, pMst->nPosX, pMst->nPosY, e_Prio_DustUnder, 0);
			return (e_MstState_Dead);
		}
		break;
	}

//s	// Sprite.
//s	nSpr = AnmGetImage(pMst->nAnm);
	// Affichage.
	SprDisplay(nSpr ^ (pMst->nFlipX ? SPR_Flip_X : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies + gnMstPrio);

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 26 : Girida-0.

/*
[mst26] Girida-0
From = 0:0: Front - Back	; Attaque par devant ou derrière.
ZoneMax = 1:7:			; Zone de déplacement max, 0 = pas de limite.
*/

// Récupère l'angle en fonction de la pente. On prend l'angle entre les deux points extremes.
u8 Mst_SlopeAngle(struct SMstCommon *pMst, s32 nOffset, s32 *nOffsYDisp)
{
	s32	nHtL, nHtR;

	nHtL = BlockGetGroundLevel((pMst->nPosX - nOffset) >> 8, pMst->nPosY >> 8);
	nHtR = BlockGetGroundLevel((pMst->nPosX + nOffset) >> 8, pMst->nPosY >> 8);
	*nOffsYDisp = (nHtL + nHtR) / 2;		// Décalage Y pour l'affichage.
	return (fatan2((nHtL - nHtR) << 8, nOffset * 2));
}

// Phases.
enum
{
	e_Mst26_Wait = 0,
	e_Mst26_WaitScroll,
	e_Mst26_Move,
	e_Mst26_Death,
};

// Structure spécifique.
struct SMst26_Girida0
{
	s32	nPosY;			// Pos Y corrigée avec le décalage.
	u16	nPosXOrg;		// Position X originale.
	s16	nTurretOffsY;	// Offset y pour faire sauter la tourelle quand explosion.
	s16	nDispOffsY;		// Offset y pour décalage du rotozoom.
	s8	nTurretAnm;		// Anim de la tourelle.
	u8	nAng;			// Angle du tank.
	u8	nLife;
	u8	nHitCnt;
	u8	nSens;			// 0 = Vers la gauche / 1 = Vers la droite. => On se sert du bit de flip pour l'avance dans une direction ou l'autre...
	u8	nTurretBaseAdd;	// Add sur sprite de la tourelle pour angle.
	u8	nShotCnt;		// Compteur pour tirer.
	u8	nZoneMax;		// Zone de déplacement max. 0 = Pas de limite.
};

#define	MST26_REF_OFFSETX	0x1200	//0x1000
void Mst26_Init_Girida0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst26_Girida0	*pSpe = (struct SMst26_Girida0 *)pMst->pData;
	s32	nVal;

	pSpe->nTurretAnm = AnmSet(gAnm_Girida0_Turret_Idle, -1);	// Anim de la tourelle.
	pSpe->nAng = 0;			// Angle du tank.
	pSpe->nTurretOffsY = 0;

	nVal = GetBits(0, 0, pData, 0);		// Depuis devant ou derrière.
	pSpe->nSens = nVal;

	Enemy_GroundLevel(pMst);			// Mise au niveau du sol.
	pSpe->nAng = Mst_SlopeAngle(pMst, MST26_REF_OFFSETX, &nVal);
	pSpe->nDispOffsY = nVal << 8;
	if (pSpe->nSens) pSpe->nAng = 256 - pSpe->nAng;
	pSpe->nPosXOrg = (pMst->nPosX >> 12) + (pSpe->nSens ? -1 : 1);		// PosX originale.

	nVal = GetBits(1, 7, pData, 0);		// Zone max.
	pSpe->nZoneMax = nVal;

	pSpe->nLife = 10;
	pSpe->nHitCnt = 0;
	pSpe->nTurretBaseAdd = 0;
	pSpe->nShotCnt = 64;

	pMst->nPhase = (pSpe->nSens ? e_Mst26_WaitScroll : e_Mst26_Wait);

}

// Angle de la tourelle.
void Mst26_TurretAngle(struct SMstCommon *pMst, s32 nDist)
{
	struct SMst26_Girida0	*pSpe = (struct SMst26_Girida0 *)pMst->pData;

	nDist = ABS(nDist);
	nDist = nDist >> 12;
	nDist /= 2;
	if (nDist > 4) nDist = 4;
	pSpe->nTurretBaseAdd = 4 - nDist;
}

// Tir ?
u32 Mst26_Shot(struct SMstCommon *pMst, s32 nDist)
{
	struct SMst26_Girida0	*pSpe = (struct SMst26_Girida0 *)pMst->pData;

	if (AnmGetKey(pSpe->nTurretAnm) != e_AnmKey_Null) return (1);	// Si déjà en train de tirer...
//	if (gShoot.nDeathFlag) return (0);	// On ne tire plus quand le héros est mort.
	if (MST_SHOT_COND == 0) return (0);	// On ne tire plus quand le héros est mort.

	if ((nDist < 0 ? 0 : 1) ^ pSpe->nSens)	// Le héros est bien devant ?
	if (ABS(nDist) <= 10 << 12)		// Pas trop loin ?
	if (pMst->nPosX >= gScrollPos.nPosX - (1 << 12) && pMst->nPosX <= gScrollPos.nPosX + (SCR_Width << 8) + (1 << 12))
	if (--pSpe->nShotCnt == 0)		// !! Test en dernier pour ne pas passer à 0xFF si les autres tests ratent.
	{
		// Anims de tir.
		AnmSet(gAnm_Girida0_Turret_Shot, pSpe->nTurretAnm);
		AnmSet(gAnm_Girida0_Body_Shot, pMst->nAnm);
		// Tir
		struct SSprRect sRect1;
		if (SprGetRect((e_Spr_Girida0_Turret_Shot + ((u32)pSpe->nTurretBaseAdd << 2)) | (pSpe->nSens ? SPR_Flip_X : 0), e_SprRectZone_ShotOrg, &sRect1))
		if (sRect1.nType == e_SprRect_Point)
		{
			// Offsets pour origine avec la rotation.
			s32	nOffsX, nOffsY;
			nOffsX = sRect1.nX1;
			nOffsY = sRect1.nY1;
			Rot2D_RotatePoint(&nOffsX, &nOffsY, (pSpe->nSens ? 256 - pSpe->nAng : pSpe->nAng));
			// Tir.
			s32	nSlot;
			nSlot = FireAdd(e_Shot_Enemy_Girida0_Bullet0, pMst->nPosX + (nOffsX << 8), pSpe->nPosY + (nOffsY << 8), (pSpe->nSens ? 0 : 128));
			// Ajustement de la vitesse x du tir.
			if (nSlot != -1)
			{
				s16	*pSpdX;
				pSpdX = FireGetSpdXPtr(nSlot);
				*pSpdX = ABS(nDist) / 50;	// Version 1, au pif et parfaite sauf dans les pentes.
			}
			// Dust de tir.
			DustSet(gAnm_Girida0_Turret_ShotFx_Dust, pMst->nPosX + (nOffsX << 8), pSpe->nPosY + (nOffsY << 8), e_Prio_DustOver, 0);
		}
		// Next shot.
		pSpe->nShotCnt = (rand() & 127) | 32;
		return (1);
	}
	return (0);
}

#define	MST26_LIM_MAX	(7 << 12)	//(10 << 12)
#define	MST26_LIM_MIN	(5 << 12)	//(7 << 12)
#define	MST26_TURRET_OFFSY_MAX	(1 << 12)
#define	MST26_GIRIDA0_AREA	10
s32 Mst26_Main_Girida0(struct SMstCommon *pMst)
{
	struct SMst26_Girida0	*pSpe = (struct SMst26_Girida0 *)pMst->pData;
	u32	nSpr;
	s32	nOffsX, nOffsY;
	s32	t;


	// Merdier pour dégager les monstres qui sortent à gauche. Tout ça à cause du bruit du moteur...
	if (gScrollPos.nScrollType == e_ScrollType_RightOnly)// && pSpe->nSens == 0)
	{
		s32	nVal;
		if (gnScrollLimitXMin != -1)
			nVal = gnScrollLimitXMin;	// Quand scroll bloqué.
		else
			nVal = gScrollPos.nLastPosX - (((SCR_Width / 16) / 3) << 12);	// Quand scroll normal.
		if ((pSpe->nPosXOrg + (pSpe->nSens == 0 ? 0 : pSpe->nZoneMax) + 2) << 12 <= nVal)
		{
			AnmReleaseSlot(pSpe->nTurretAnm);
			return (e_MstState_Dead);	// e_MstState_Asleep ? e_MstState_Dead ?
		}
	}


	// Spr body.
	nSpr = AnmGetImage(pMst->nAnm);
	// PosY corrigée.
	pSpe->nPosY = pMst->nPosY + (s32)pSpe->nDispOffsY;

	switch (pMst->nPhase)
	{
	case e_Mst26_WaitScroll:	// Attente que le scroll ait dépassé la position du monstre pour apparaître.
		if (gScrollPos.nPosX >= pMst->nPosX + (2 << 12)) pMst->nPhase = e_Mst26_Wait;
		return (e_MstState_Managed);
		//break;

	case e_Mst26_Wait:		// Attente.
		// Sortie de l'écran ? (Si pas de zone max, pas de test de sortie. Mais comme le tank fera chier sans arrêt, on ne devrait pas l'oublier).
		if (pSpe->nZoneMax)
			if (Screen_ObjectOutRect((pSpe->nPosXOrg - (pSpe->nSens ? MST26_GIRIDA0_AREA : pSpe->nZoneMax + MST26_GIRIDA0_AREA)) << 12, pMst->nPosY, pSpe->nZoneMax + (MST26_GIRIDA0_AREA * 2), 1))
			{
				AnmReleaseSlot(pSpe->nTurretAnm);
				return (e_MstState_Asleep);
			}
		//
		t = pMst->nPosX - gShoot.nPlayerPosX;
		Mst26_TurretAngle(pMst, t);	// Angle de la tourelle.
		Mst26_Shot(pMst, t);		// Déclenchement d'un tir ?
		// Anim Idle.
		AnmSetIfNew(gAnm_Girida0_Body_Idle, pMst->nAnm);
		//
		if (ABS(t) > (MST26_LIM_MAX + 0xA00) || ABS(t) < (MST26_LIM_MIN - 0xA00) ||
			((t < 0 ? 1 : 0) ^ pSpe->nSens) )	// Perso derrière, on force le mouvement.
		{
			pMst->nPhase = e_Mst26_Move;
			break;
		}
		break;

	case e_Mst26_Move:		// Déplacement pour se remettre à bonne distance du héros.
		t = pMst->nPosX - gShoot.nPlayerPosX;
		Mst26_TurretAngle(pMst, t);	// Angle de la tourelle.
		if (Mst26_Shot(pMst, t))	// Déclenchement d'un tir ?
		{
			pMst->nPhase = e_Mst26_Wait;
			break;
		}
		//
		if ((t < 0 ? 1 : 0) ^ pSpe->nSens)
		{	// Héros derrière.
			pMst->nFlipX = pSpe->nSens ^ 1;		// On recule.
		}
		else
		{	// Héros devant.
			// Distance.
			if (ABS(t) <= MST26_LIM_MAX && ABS(t) >= MST26_LIM_MIN)		// Distance ok ?
			{
				pMst->nPhase = e_Mst26_Wait;
				break;
			}
			// Sens du déplacement.
			if (ABS(t) > MST26_LIM_MAX) pMst->nFlipX = pSpe->nSens;
			if (ABS(t) < MST26_LIM_MIN) pMst->nFlipX = pSpe->nSens ^ 1;
		}
		// Zone de déplacement.
		if (pMst->nFlipX ^ pSpe->nSens)
		{
			// Marche arrière ? => On ne dépasse pas la position de départ.
			if ( ((pMst->nPosX + (pMst->nFlipX ? -0x100 : 0x100)) >> 12) - pSpe->nPosXOrg == 0)
			{
				pMst->nPhase = e_Mst26_Wait;
				break;
			}
		}
		else
		if (pSpe->nZoneMax)
		{
			// Marche avant, on ne dépasse pas la zone max.
			if (ABS((pMst->nPosX >> 12) - pSpe->nPosXOrg) >= pSpe->nZoneMax)
			{
				pMst->nPhase = e_Mst26_Wait;
				break;
			}
		}
		// Déplacement.
		Mst_BasicMove(pMst, 0x100, 0);
		// Angle du tank.
		pSpe->nAng = Mst_SlopeAngle(pMst, MST26_REF_OFFSETX, &t);
		pSpe->nDispOffsY = t << 8;
		pSpe->nPosY = pMst->nPosY + (s32)pSpe->nDispOffsY;
		if (pSpe->nSens) pSpe->nAng = 256 - pSpe->nAng;
		// Anim de déplacement.
		AnmSetIfNew((pMst->nFlipX ^ pSpe->nSens ? gAnm_Girida0_Body_DriveBackwards : gAnm_Girida0_Body_DriveForward), pMst->nAnm);
		break;

	case e_Mst26_Death:		// Mort.
		// Anim de mort terminée ? => Grosse explosion.
		if (AnmCheckEnd(pMst->nAnm))
		{
			nOffsX = (MST26_REF_OFFSETX >> 9) * gVar.pSin[pSpe->nAng];
			nOffsY = ABS(nOffsX);
			DustSet(gAnm_Explosion0_Big_Dust, pMst->nPosX + nOffsX, pSpe->nPosY + nOffsY, e_Prio_DustUnder, 0);
			DustSetMvt(gAnm_Debris_Metal0_Dust, pMst->nPosX, pSpe->nPosY - 0x1000, 0x180, -0x480, e_Prio_DustUnder - 1, e_DustFlag_Gravity);
			DustSetMvt(gAnm_Debris_Metal0_Dust, pMst->nPosX, pSpe->nPosY - 0x1000, -0x180, -0x380, e_Prio_DustUnder - 1, e_DustFlag_Gravity);
			//
			AnmReleaseSlot(pSpe->nTurretAnm);
			return (e_MstState_Dead);
		}
		// Saut de la tourelle.
		pMst->nSpdY += GRAVITY / 2;
		if (pMst->nSpdY >= SPDY_MAX) pMst->nSpdY = SPDY_MAX;
		pSpe->nTurretOffsY += pMst->nSpdY;
		if (pSpe->nTurretOffsY > MST26_TURRET_OFFSY_MAX) pSpe->nTurretOffsY = MST26_TURRET_OFFSY_MAX;
		break;
	}

	// Se prend un tir ?
	if (pSpe->nLife)
	{
		// Offsets du rect de col pour le tank en rotozoom.
		nOffsX = (MST26_REF_OFFSETX >> 9) * gVar.pSin[pSpe->nAng];
		if (pSpe->nSens) nOffsX = -nOffsX;
		nOffsY = ABS(nOffsX);
		// Test.
//		if (Mst_ShotCheckLife(e_Spr_Girida0_Turret_Shot, pMst->nPosX + nOffsX, pSpe->nPosY + nOffsY, &pSpe->nHitCnt, &pSpe->nLife))
		if (Mst_ShotCheckLife2(e_Spr_Girida0_Turret_Shot, pMst->nPosX + nOffsX, pSpe->nPosY + nOffsY, &pSpe->nHitCnt, &pSpe->nLife, e_ShotCheck_MinimizedDamage))
		{
			DustSet(gAnm_Explosion0_Medium_Dust, pMst->nPosX + nOffsX, pSpe->nPosY + nOffsY - (1<<12), e_Prio_DustUnder, 0);
			AnmSet(gAnm_Girida0_Body_Destroyed, pMst->nAnm);
			pMst->nSpdY = -0x600/2;		// On se sert de nSpdY pour faire sauter la tourelle.
			pMst->nPhase = e_Mst26_Death;
			gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;	// Score.
		}
	}

	// Affichage.
	if (pSpe->nHitCnt) pSpe->nHitCnt--;
	// Petit truc pour la priorité entre Giridas.
	static	u8	nSubPrio;
	if (++nSubPrio >= gpnMstCount[e_Mst26_Girida0]) nSubPrio = 0;
	nSubPrio <<= 1;
	// 1 - Body.
	SprDisplayRotoZoom(nSpr | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0) | (pSpe->nSens ? SPR_Flip_X : 0), pMst->nPosX >> 8, pSpe->nPosY >> 8, e_Prio_Ennemies + 1 - 2 - nSubPrio, pSpe->nAng, 0x100);
	// 2 - Turret.
	if (pSpe->nTurretOffsY < MST26_TURRET_OFFSY_MAX)	// Cas de la tourelle détruite.
	{
		nSpr = e_Spr_Girida0_Turret_Shot + (pSpe->nTurretAnm == -1 ? 0 : AnmGetImage(pSpe->nTurretAnm)) + ((u32)pSpe->nTurretBaseAdd << 2);
		SprDisplayRotoZoom(nSpr | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0) | (pSpe->nSens ? SPR_Flip_X : 0), pMst->nPosX >> 8, (pSpe->nPosY + pSpe->nTurretOffsY) >> 8, e_Prio_Ennemies - 2 - nSubPrio, pSpe->nAng, 0x100);
	}

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 27 : Half-bosses (Container).

/*
[mst27] Half-Boss
Wait itm = 0:7:			; N° item de quête attendu pour démarrer.
Give itm = 8:15:		; N° item de quête à donner une fois mort pour libérer le scroll.
Type = 16:19: RShobu - Masknell -
Nb(1) = 20:23:			; Nb de monstres dans la séquence.
Delay = 24:27:			; Timing entre les monstres.
Prm = 28:63:			; tmp
*/

// Phases.
enum
{
	e_Mst27_WaitItem = 0,
	e_Mst27_Generate,
};

// Structure spécifique.
struct SMst27_HalfBoss0
{
	u8	nType;
	u8	nItemWait;		// N° item de quête attendu pour démarrer.
	u8	nItemGive;		// N° item de quête à donner à la fin.
	u8	nSeqNo;			// N° de la séquence (si séquence il y à).
	u8	nOrder;			// N° d'ordre dans la génération (Masknell).
	u8	nMstNb;			// Nb de monstres dans la séquence.
	u8	nDelay;			// Délai entre les apparitions.
	u8	nCnt0;			// Compteur.
};

void Mst27_Init_HalfBoss0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst27_HalfBoss0	*pSpe = (struct SMst27_HalfBoss0 *)pMst->pData;
	s32	nVal;

	nVal = GetBits(0, 7, pData, 0);			// N° item de quête attendu pour démarrer.
	pSpe->nItemWait = nVal;
	nVal = GetBits(8, 15, pData, 0);		// N° item de quête à donner à la fin.
	pSpe->nItemGive = nVal;

	nVal = GetBits(16, 19, pData, 0);		// Type.
	pSpe->nType = nVal;

	nVal = GetBits(20, 23, pData, 0);		// Nb de monstres.
	if (nVal == 0) nVal = 1;
	pSpe->nMstNb = nVal;
	// Séquence ?
	if (pSpe->nMstNb > 1)
	{
		// Initialise le nb de monstres de la séquence.
		pSpe->nSeqNo = SeqInit(pSpe->nMstNb);
		pSpe->nOrder = 0;
	}
	nVal = GetBits(24, 27, pData, 0);		// Délai entre les apparitions.
	pSpe->nDelay = nVal << 3;
	pSpe->nCnt0 = pSpe->nDelay;				// Init compteur.

	pMst->nPhase = e_Mst27_WaitItem;
}

s32 Mst27_Main_HalfBoss0(struct SMstCommon *pMst)
{
	struct SMst27_HalfBoss0	*pSpe = (struct SMst27_HalfBoss0 *)pMst->pData;

	switch (pMst->nPhase)
	{
	case e_Mst27_WaitItem:		// Attend une item de quête (celle du scroll lock) avant de faire apparaître le demi-boss.
		if (gpMstQuestItems[pSpe->nItemWait])
		{
			pMst->nPhase = e_Mst27_Generate;
		}
		else
		{
			// Sortie de l'écran ?
			if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);
		}
		return (e_MstState_Managed);
		break;

	case e_Mst27_Generate:		// Génération des monstres.
		if (pSpe->nCnt0 == 0)
		{
			static u8 pnBossNoTb[] = { e_Mst6_RShobu, e_Mst28_Masknell0, };
			u8	pData[4];

			// Génération d'un monstre.
			switch (pSpe->nType)
			{
			case 0:		// R-Shobu.
/*
[mst6] R-Shobu (half-boss)
Wait itm = 0:7:			; N° item de quête attendu pour démarrer.
Give itm = 8:15:		; N° item de quête à donner une fois mort pour libérer le scroll.
*/
				pData[0] = pSpe->nItemWait;
				pData[1] = pSpe->nItemGive;
				break;

			case 1:		// Masknell.
/*
[mst28] Masknell (half-boss)
Wait itm = 0:7:			; N° item de quête attendu pour démarrer.
Give itm = 8:15:		; N° item de quête à donner une fois mort pour libérer le scroll.
SeqNo = 16:19:			; N° de séquence.
Order = 20:23:			; N° d'ordre dans la séquence (pour trouver le leader).
Type = 24:26: Semi_Boss - L02 -

Drop = 27:30: Nothing - S_Shotgun - H_Machinegun - F_Flamethrower - R_Rocket - Gas_Tank - Ammo_Box1
Prm = 31:31:			; tmp

//Prm = 27:31:			; tmp
*/
				pData[0] = pSpe->nItemWait;
				pData[1] = pSpe->nItemGive;
				pData[2] = (pSpe->nOrder << 4) | pSpe->nSeqNo;
				pData[3] = 0;
				pSpe->nOrder++;
				break;
			}
			MstAdd(pnBossNoTb[pSpe->nType], (pMst->nPosX >> 8), (pMst->nPosY >> 8), pData, -1);

			if (--pSpe->nMstNb == 0) return (e_MstState_Dead);	// Nb de monstres dans la séquence.
			pSpe->nCnt0 = pSpe->nDelay;				// Reinit compteur.
		}
		else
			pSpe->nCnt0--;		// Tic tac...
		break;
	}

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 28 : Masknell (Half-boss).

/*
[mst28] Masknell (half-boss)
Wait itm = 0:7:			; N° item de quête attendu pour démarrer.
Give itm = 8:15:		; N° item de quête à donner une fois mort pour libérer le scroll.
SeqNo = 16:19:			; N° de séquence.
Order = 20:23:			; N° d'ordre dans la séquence (pour trouver le leader).
Type = 24:26: Semi_Boss - L02 -

Drop = 27:30: Nothing - S_Shotgun - H_Machinegun - F_Flamethrower - R_Rocket - Gas_Tank - Ammo_Box1
Prm = 31:31:			; tmp
*/

//Drop = 8:11: Nothing - H_Machinegun - Gas_Tank - Ammo_Box1	; H non utilisé.

// Phases.
enum
{
	e_Mst28_T0_Fly = 0,
	e_Mst28_T0_Turn,
	e_Mst28_T0_GoDown,
	e_Mst28_T0_Boss,

	e_Mst28_T1_ToPlayer,
	e_Mst28_T1_Shot,
	e_Mst28_T1_GoAway,

};

// Structure spécifique.
struct SMst28_Masknell0
{
	u8	nItemWait;	// N° item de quête attendu pour démarrer.
	u8	nItemGive;	// N° item de quête à donner une fois mort pour libérer le scroll.
	u8	nSeqNo;		// N° de séquence.
	u8	nOrder;		// N° d'ordre dans la séquence.
	u8	nHitCnt;	// Hit counter.
	u8	nCurAdd;	// Nombre à ajouter au sprite de base.
	u8	nLife;		// Pts de vie.
	s8	nShotAnm;	// Anim de tir.

	u8	nType;		// Type : Semi-boss - L02.
	u8	nAccCnt;	// Pour phase de dégagement (type 1).
	u8	nShotNb;	// Nb de tirs (type 1).
	u8	nShotCnt;	// Compteur pour ne pas enchaîner les tirs si le joueur reste dans l'alignement.

	struct SMstPath	sPath;

	u8	nDrop;		// Si item à larguer.

	u8	nSinIdx;	// Pour sinus sur vol stationnaire.

};

#define	MST28_T0_ACC	0x08
#define	MST28_T1_OFFSX	4

#define	MST28_T0_LIFE	20
#define	MST28_T1_LIFE	10
void Mst28_Init_Masknell0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst28_Masknell0	*pSpe = (struct SMst28_Masknell0 *)pMst->pData;
	s32	nVal;

	pMst->nSpd = 0;		// Avant, pour ne pas écraser la vitesse du MstPathInit du type 0.
	pMst->nSpdY = 0;
	pSpe->nSinIdx = 0;

	pSpe->nCurAdd = 0;
	pSpe->nLife = MST28_T0_LIFE;	// Val par défaut.

	nVal = GetBits(24, 26, pData, 0);		// Type.
	pSpe->nType = nVal;
	switch (pSpe->nType)
	{
	case 0:		// Semi-boss (séquence).
		nVal = GetBits(0, 7, pData, 0);		// N° item de quête attendu pour démarrer.
		pSpe->nItemWait = nVal;
		nVal = GetBits(8, 15, pData, 0);	// N° item de quête à donner une fois mort pour libérer le scroll.
		pSpe->nItemGive = nVal;
		nVal = GetBits(16, 19, pData, 0);	// N° de séquence.
		pSpe->nSeqNo = nVal;
		nVal = GetBits(20, 23, pData, 0);	// N° d'ordre dans la séquence.
		pSpe->nOrder = nVal;
		// Bits leader.
		gMstMisc.pSeqLeader[pSpe->nSeqNo] |= 1 << pSpe->nOrder;	// Pour recherche des leaders.

		pMst->nPosX = gScrollPos.nPosX + (SCR_Width << 8) + (2 << 12);	// Replacement hors de l'écran.
		Mst_PathInit(pMst, &pSpe->sPath, 0x200, 128);	// Init path.
		pMst->nPhase = e_Mst28_T0_Fly;
		pSpe->nCurAdd = 3;
		pSpe->nLife = MST28_T0_LIFE;

		pSpe->nSinIdx = 1;

		Rumi_Reset();
		break;

	case 1:		// L02.
		// Init à droite ou à gauche de l'écran en fct de la pos du joueur par rapport au centre de l'écran.
		if (gShoot.nPlayerPosX < gScrollPos.nPosX + ((SCR_Width / 2) << 8))
		{
			pMst->nPosX = gScrollPos.nPosX + (((SCR_Width / 16) - MST28_T1_OFFSX) << 12);
			pMst->nFlipX = 0;
		}
		else
		{
			pMst->nPosX = gScrollPos.nPosX + (MST28_T1_OFFSX << 12);
			pMst->nFlipX = 1;
		}
		pMst->nPosY = gScrollPos.nPosY - (1 << 12);
		pSpe->nAccCnt = 0;
		pSpe->nShotNb = 2;	// Nb de tirs.

		gpMstQuestItems[MST_QUEST_ITEM_MST32]++;	// Scroll lock.

		pMst->nPhase = e_Mst28_T1_ToPlayer;
		pSpe->nLife = MST28_T1_LIFE;
		break;

	default:
		break;
	}

	nVal = GetBits(27, 30, pData, 0);		// Drop.
	pSpe->nDrop = nVal;

	pSpe->nHitCnt = 0;	// Hit counter.
	pSpe->nShotAnm = -1;
	pSpe->nShotCnt = 0;

}

// Fait ce qu'il faut au moment de la mort, en fct du type.
void Mst28_DeathCommon(struct SMstCommon *pMst)
{
	struct SMst28_Masknell0	*pSpe = (struct SMst28_Masknell0 *)pMst->pData;
	u8	nItmDrop = 0;	// Item à lacher. Par défaut non.
	u8	nItmLoc = 0;	// "Location" de l'item à lacher.

	switch (pSpe->nType)
	{
	case 0:		// Semi-boss (séquence).
		// Cleare le bit de lead.
		gMstMisc.pSeqLeader[pSpe->nSeqNo] &= ~(1 << pSpe->nOrder);
		// Décrémente la séquence.
		if (--gMstMisc.pSeqCount[pSpe->nSeqNo] == 0)
		{
			nItmDrop = pSpe->nDrop;
			nItmLoc = 0;
			gpMstQuestItems[pSpe->nItemGive] = 1;	// Item de quête quand le dernier monstre de la séquence explose.
		}
		break;

	case 1:		// L02.
		gpMstQuestItems[MST_QUEST_ITEM_MST32]--;		// Scroll release.
		nItmDrop = pSpe->nDrop;
		nItmLoc = 3;
		break;

	default:
		break;
	}

	// Libère l'anim de tir si besoin.
	if (pSpe->nShotAnm != -1)
	{
		AnmReleaseSlot(pSpe->nShotAnm);
		pSpe->nShotAnm = -1;
	}

	// Largage d'une item.
	if (nItmDrop)
	{
/*
[mst4] Weapon Capsule
Type = 0:3: S_Shotgun - H_Machinegun - F_Flamethrower - R_Rocket - Gas_Tank - Ammo_Box1
Boxed = 4:4: No - Yes		; Une caisse contient l'item ?
Location = 5:6: Normal - Space - Underwater - Air
*/
		u32	nData;
		nData = (nItmDrop - 1) | (nItmLoc << 5);
		MstAdd(e_Mst4_WeaponCapsule, pMst->nPosX >> 8, pMst->nPosY >> 8, (u8 *)&nData, -1);
	}

}

void Mst6_MoveToPlayer(struct SMstCommon *pMst);

s32 Mst28_Main_Masknell0(struct SMstCommon *pMst)
{
	struct SMst28_Masknell0	*pSpe = (struct SMst28_Masknell0 *)pMst->pData;
	u32	nSpr;

	// Est-on le leader ?
	u32	nLeader = 1;
	if (pSpe->nType == 0)
	{
		nLeader = ((gMstMisc.pSeqLeader[pSpe->nSeqNo] & ~(0xFF << (pSpe->nOrder + 1))) == 1 << pSpe->nOrder ? 1 : 0);	// Le leader = le bit le plus à droite !
		if (nLeader)
		{
			Rumi_Check(18 * 16, (gnFrame & 1 ? e_Player_Weapon_Rocket : e_Player_Weapon_Machinegun));
			if ((gnFrame & 7) == 0) Sfx_PlaySfx(e_Sfx_Heli0, e_SfxPrio_30);		// Pour limiter le bordel sonore, seul le leader lance le son des hélices.
		}
	}

	switch (pMst->nPhase)
	{

	// *** Type 0. Half-Boss. (Lev 9).

	case e_Mst28_T0_Fly:	// Vol.

// tst de largage de bombe.

//		if (MST_SHOT_COND)	// Si le héros n'est pas mort...
//			if (pSpe->nFrmShot == 0 && ABS(nDist) < (3 << 12)) pSpe->nFrmShot = (16 * 3) << 1;	// Tir ?
/* shobu
	// Petite série de largage de bombe ?
	if (pSpe->nFrmShot)
	{
		if ((pSpe->nFrmShot & 15) == 0 && (pSpe->nFrmShot & 64))
		{
			FireAdd(e_Shot_Enemy_RShobu_Bomb_Drop, pMst->nPosX, pMst->nPosY, -1);	// Tir.
			DustSet(gAnm_RShobu_BombDrop_Dust, pMst->nPosX, pMst->nPosY, e_Prio_DustUnder, 0);	// Dust.
		}
		pSpe->nFrmShot--;
	}
*/

/*
if ((gMstMisc.pSeqLeader[pSpe->nSeqNo] & ~(0xFF << (pSpe->nOrder + 1))) == 1 << pSpe->nOrder)
if (pMst->nAngle == 128 && (gnFrame & 31) == 0)
{
	FireAdd(e_Shot_Enemy_RShobu_Bomb_Drop, pMst->nPosX - (1<<12), pMst->nPosY, -1);	// Tir.
}
*/

		// Le leader passe en phase d'attaque.
		if (nLeader && pMst->nAngle == 192)
			pMst->nPhase = e_Mst28_T0_GoDown;

		break;

	case e_Mst28_T0_Turn:	// Phase de "tourné" sur le vol.
		{
			if (pMst->nAngle == pSpe->sPath.nNewAngle)
			{
				pMst->nPhase = e_Mst28_T0_Fly;
				break;
			}
			pMst->nAngle += pSpe->sPath.nSensRot << 1;

			// Inclinaison de 3 à 7 / 3 coté > 7 de face
			static u8 pnAng[] =		// Note : Deux 7 slt, car dans le mvt, l'hélico reste quelques frames sur la verticale.
			{
				 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7,
				 7, 6, 6, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3,
			};
			pSpe->nCurAdd = pnAng[(pMst->nAngle >> 2) & 0x1F];
			pMst->nFlipX = ((u8)(pMst->nAngle + 64) > 128 ? 0 : 1);
		}
		break;

	case e_Mst28_T0_GoDown:		// Le leader sort de la série et descend un peu.
		{
			s32	nSpdY;
			nSpdY = (gVar.pSin[pMst->nAngle] * pMst->nSpd) >> 8;
			pMst->nPosY += nSpdY;
			pSpe->nCurAdd = 7;
			// Ralentissement.
			if (++pMst->nAngle == 0)
			{
				pMst->nPhase = e_Mst28_T0_Boss;
				pMst->nSpd = 0;
				pSpe->nShotCnt = 64+16;
			}
		}
		break;

	case e_Mst28_T0_Boss:
		{
			u8	nAng;
			nAng = fatan2(-((gShoot.nPlayerPosY - (1 << 12)) - (pMst->nPosY - (1 << 12))), gShoot.nPlayerPosX - pMst->nPosX);

			static u8 pnAng2[] = { 3,4,4,5,5,6,6,7,7,6,6,5,5,4,4,3 };
			pSpe->nCurAdd = pnAng2[(nAng >> 3) & 0x0F];
			pMst->nFlipX = ((u8)(nAng + 64) > 128 ? 0 : 1);

			if (MST_SHOT_COND == 0) pSpe->nShotCnt = 64+16;	// Si héros mort, RAZ compteur pour ne pas tirer immédiatement au "revive" du joueur.

			pSpe->nShotCnt++;
			if ((pSpe->nShotCnt & 7) == 0 && (pSpe->nShotCnt & (32|64)) == 0)
				pSpe->nShotAnm = AnmSet(gAnm_Masknell_ShotFx, pSpe->nShotAnm);	// Anim de tir.

			if (pSpe->nShotCnt & (32|64))
			{
				s32	nOffset = (gnFrame & 64 ? 0x1800 : -0x1800);	// Offset pour changement de position par rapport au joueur. Le truc, c'est de ne pas rester pile au dessus, par avec les tirs verticaux, un passe de chaque côté du joueur !
				pMst->nPosX += nOffset;
				Mst6_MoveToPlayer(pMst);	// On se déplace comme le R-Shobu, pour se mettre au dessus du joueur.
				pMst->nPosX -= nOffset;
			}
			else
			{
				// Ralentissement.
				pMst->nSpd += (pMst->nSpd < 0 ? 0x20 : -0x20);
				if (ABS(pMst->nSpd) <= 0x20) pMst->nSpd = 0;
				pMst->nPosX += pMst->nSpd;
			}

			// Oscillation sinus.
			pSpe->nSinIdx += 2;
		}
		break;

	// *** Type 1. (Lev 2 - Air).

	case e_Mst28_T1_ToPlayer:	// Type 1 : Déplacement pour se mettre au Y du joueur.
		// Avance vers la destination finale.
		pMst->nSpdY = (gShoot.nPlayerPosY - pMst->nPosY) / 16;
		if (ABS(pMst->nSpdY) > 0x400) pMst->nSpdY = SGN(pMst->nSpdY) << 10;
		pMst->nPosY += pMst->nSpdY;

		// Tir ?
		if (pSpe->nShotCnt) pSpe->nShotCnt--;
		if (ABS(pMst->nPosY - gShoot.nPlayerPosY) < 0x200 && pSpe->nShotCnt == 0)
		{
			pMst->nPhase = e_Mst28_T1_Shot;
			// Anim de tir.
			pSpe->nShotAnm = AnmSet(gAnm_Masknell_ShotFx, pSpe->nShotAnm);
		}
		break;

	case e_Mst28_T1_Shot:		// Type 1 : Tir (ne bouge plus).
		// Décélération.
		if (pMst->nSpdY)
		{
			s32	nVal;
			nVal = (ABS(pMst->nSpdY) > 0x10 ? SGN(pMst->nSpdY) * -0x10 : -pMst->nSpdY);
			pMst->nSpdY += nVal;
			pMst->nPosY += pMst->nSpdY;
		}
		// On attend la fin de l'anim.
		if (pSpe->nShotAnm != -1) break;
		// Il reste des tirs ?
		pMst->nPhase = (--pSpe->nShotNb == 0 ? e_Mst28_T1_GoAway : e_Mst28_T1_ToPlayer);
		pSpe->nShotCnt = 48;
		break;

	case e_Mst28_T1_GoAway:		// Type 1 : Avance jusqu'à avoir dégagé.
		if (ABS(pMst->nSpd) < 0x400) pMst->nSpd += (pMst->nFlipX ? 0x40 : -0x40);
		pMst->nPosX += pMst->nSpd;
		if (pSpe->nAccCnt < 0x30) pSpe->nAccCnt += 2;
		pSpe->nCurAdd = pSpe->nAccCnt >> 4;
		// Test de sortie d'écran.
		if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY))
		{
			Mst28_DeathCommon(pMst);
			return (e_MstState_Dead);
		}
		break;

	default:
		break;
	}

	// Déplacement sur path (type 0).
	if (pSpe->nType == 0 && pMst->nPhase <= e_Mst28_T0_Turn)
		Mst_PathMove(pMst, &pSpe->sPath, MST28_T0_ACC, e_Mst28_T0_Turn);

	nSpr = e_Spr_Masknell_Body + pSpe->nCurAdd;
	nSpr |= (pMst->nFlipX ? SPR_Flip_X : 0);

	// Petite oscillation quand vol stationnaire.
	s32	nPosY_sav = pMst->nPosY;
	pMst->nPosY += gVar.pSin[pSpe->nSinIdx] << 1;

	// Se prend un tir ?
	if (pSpe->nType == 1)
	{	// Type 1.
		Mst_ShotCheckLife(nSpr, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife);
	}
	else
	{	// Type 0.
		if (pMst->nPhase == e_Mst28_T0_Boss)
			Mst_ShotCheckLife2(nSpr, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife, e_ShotCheck_MinimizedDamage);	// Dégats minimisés sur le leader.
		else
		{	// Interception des tirs sans dégats pour les autres.
			u8	nHitCnt, nLife;
			nLife = 0;
			Mst_ShotCheckLife(nSpr, pMst->nPosX, pMst->nPosY, &nHitCnt, &nLife);
		}
	}
	// Mort ?
//	if (Mst_ShotCheckLife(nSpr, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife))	// tst original.
	if (pSpe->nLife == 0)
	{
		gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;	// Score.
_28MasknellDeath:
		// Différents dusts.
		DustSet(gAnm_Explosion0_Big_Dust, pMst->nPosX, pMst->nPosY, e_Prio_DustUnder, 0);
		DustSetMvt(gAnm_Debris_Metal0_Dust, pMst->nPosX, pMst->nPosY, 0x180, -0x480, e_Prio_DustUnder - 1, e_DustFlag_Gravity);
		DustSetMvt(gAnm_Debris_Metal0_Dust, pMst->nPosX, pMst->nPosY, -0x180, -0x380, e_Prio_DustUnder - 1, e_DustFlag_Gravity);
		// Mort.
		Mst28_DeathCommon(pMst);
		return (e_MstState_Dead);
	}

	// Contact avec le joueur ?
	struct SSprRect sRect1;
	if (pSpe->nType != 0)
	if (SprGetRect(nSpr, e_SprRectZone_RectCol, &sRect1))
	if (sRect1.nType == e_SprRect_Rect)
	if (Enemy_PlayerBlowRect(&sRect1, pMst->nPosX, pMst->nPosY, MST_DAMAGE_EXPLO)) goto _28MasknellDeath;

	// Gestion du tir frontal.
	u32	nSprShot = SPR_NoSprite;
	if (pSpe->nShotAnm != -1)
	{
		nSprShot = AnmGetImage(pSpe->nShotAnm);
		nSprShot += e_Spr_Masknell_ShotFx + pSpe->nCurAdd;
		nSprShot ^= (pMst->nFlipX ? SPR_Flip_X : 0);
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
			u32	pnShotOffsX[2], pnShotOffsY[2];
			static u8	pnShotAng[] = { 64, 57, 51, 44, 38, 32, 16, 0 };
			u8	nAng;

			if (SprGetRect(nSpr, e_SprRectZone_ShotOrg, &sRect1))	// nSpr déjà flippé.
			if (sRect1.nType == e_SprRect_Rect)
			{
				pnShotOffsY[0] = sRect1.nY1;
				pnShotOffsY[1] = sRect1.nY2;
				pnShotOffsX[pMst->nFlipX & 1] = sRect1.nX1;
				pnShotOffsX[(pMst->nFlipX ^ 1) & 1] = sRect1.nX2;
				nAng = 192 - (pMst->nFlipX ? -pnShotAng[pSpe->nCurAdd] : pnShotAng[pSpe->nCurAdd]);

				FireAdd(e_Shot_Enemy_RebSoldier_Bullet0, pMst->nPosX + (pnShotOffsX[0] << 8), pMst->nPosY + (pnShotOffsY[0] << 8), nAng);
				FireAdd(e_Shot_Enemy_RebSoldier_Bullet0, pMst->nPosX + (pnShotOffsX[1] << 8), pMst->nPosY + (pnShotOffsY[1] << 8), nAng);
			}
		}
	}

	// On ajoute le leader dans la table de visée.
	if (nLeader) ChaserTarget_AddToList(pMst->nPosX >> 8, (pMst->nPosY >> 8) - 8);	// Ajout dans la table de visée des missiles.

	// Affichage.
	if (pSpe->nHitCnt) pSpe->nHitCnt--;
	nSpr = e_Spr_Masknell_Body + pSpe->nCurAdd;
	SprDisplay(nSpr | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0) | (pMst->nFlipX ? SPR_Flip_X : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies + 32 - gnMstPrio);
	nSpr = AnmGetImage(pMst->nAnm);
	nSpr += e_Spr_Masknell_Blades + pSpe->nCurAdd;
	SprDisplay(nSpr | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0) | (pMst->nFlipX ? SPR_Flip_X : 0), pMst->nPosX >> 8, (pMst->nPosY >> 8) - 40, e_Prio_Ennemies + 32 - gnMstPrio);
	// Affichage de l'éclat du tir.
	if (nSprShot != SPR_NoSprite)
		SprDisplay(nSprShot | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), (pMst->nPosX >> 8) + (pMst->nFlipX ? 0 : 0), (pMst->nPosY >> 8), e_Prio_Ennemies + 32+1 - gnMstPrio);

	pMst->nPosY = nPosY_sav;	// Restore pos y.

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 29 : Orque (qui monte du bas de l'écran).

// Phases.
enum
{
	e_Mst29_Wait = 0,
	e_Mst29_GoUp,
	e_Mst29_GoDown,
};

// Structure spécifique.
struct SMst29_Whale0
{
	u16	nOffsY;
	u16	nCnt;		// La variable qui sert à tout : Timer, vitesse...
};

#define	MST29_SPDY_MAX	0x800
#define	MST29_OFFSY	(163 << 8)
#define	MST29_DOWN_WAIT	140

void Mst29_Init_Whale0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst29_Whale0	*pSpe = (struct SMst29_Whale0 *)pMst->pData;

	pSpe->nOffsY = MST29_OFFSY;		// Décaler le monstre en dessous de la map.
	pSpe->nCnt = 0;
	pMst->nPhase = e_Mst29_Wait;

}

s32 Mst29_Main_Whale0(struct SMstCommon *pMst)
{
	struct SMst29_Whale0	*pSpe = (struct SMst29_Whale0 *)pMst->pData;
	s32	nDist;

	switch (pMst->nPhase)
	{
	case e_Mst29_Wait:		// Attente que le joueur soit à proximité.
		// Sortie de l'écran ?
		if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);
		// Compteur d'attente en bas.
		if (pSpe->nCnt) pSpe->nCnt--;
		// Joueur à proximité ?
		nDist = pMst->nPosX - gShoot.nPlayerPosX;
		if (nDist < (8 << 12))
		if (pSpe->nCnt == 0)
		{
			pMst->nAnm = AnmSet(gAnm_Whale_Attack, pMst->nAnm);	// Reinit de l'anim d'attaque.
			pSpe->nCnt = 16;			// Attente en haut.
			pMst->nPhase = e_Mst29_GoUp;
		}
		// Dust bubbles.
		if ((gnFrame & 63) == 0)
		{
			DustSetMvt(gAnm_Whale_Bubbles_Dust, pMst->nPosX + ((64 - (rand() & 127)) << 8), pMst->nPosY - (8 << 8), 0, -0x40, e_Prio_Ennemies +32 +1, 0);
		}
		// Dans cette phase, on s'arrête là.
		return (e_MstState_Managed);
		break;

	case e_Mst29_GoUp:		// Monte.
		nDist = pSpe->nOffsY / 4;
		if (nDist > MST29_SPDY_MAX) nDist = MST29_SPDY_MAX;
		pSpe->nOffsY -= nDist;
		// En haut, pause finie ? => Descente.
		if (nDist == 0)
		if (--pSpe->nCnt == 0)
		{
			pSpe->nCnt = 0;
			pMst->nPhase = e_Mst29_GoDown;
		}
		break;

	case e_Mst29_GoDown:		// Descend.
		pSpe->nCnt += 0x80;
		if (pSpe->nCnt > MST29_SPDY_MAX) pSpe->nCnt = MST29_SPDY_MAX;
		pSpe->nOffsY += pSpe->nCnt;
		// En bas ?
		if (pSpe->nOffsY >= MST29_OFFSY)
		{
			pSpe->nOffsY = MST29_OFFSY;
			pMst->nPhase = e_Mst29_Wait;
			pSpe->nCnt = MST29_DOWN_WAIT;
		}
		break;
	}

	// Affichage.
	static	u32	gpMst29Spr[] = { e_Spr_Whale_Mouth, e_Spr_Whale_Mouth+1, e_Spr_Whale_BodyMid, e_Spr_Whale_BodyBotL, e_Spr_Whale_BodyBotR };
	static	s16	gpMst29OffsX[] = { -20, -10, 0, -20, 0 };
	static	s16	gpMst29OffsY[] = { -91, -91, -59, 0, 0 };
	struct SSprRect sRect1;

	u32	i;
	u32	nImg = AnmGetImage(pMst->nAnm);	// Anim de la gueule.
	for (i = 0; i < 5; i++)
	{
		if (i >= 2) nImg = 0;	// Après les 2 premières images, plus d'anim.
		SprDisplay(gpMst29Spr[i] + nImg, (pMst->nPosX >> 8) + gpMst29OffsX[i], (pMst->nPosY >> 8) + gpMst29OffsY[i] + (pSpe->nOffsY >> 8), e_Prio_Ennemies +32 +2);
		// Arrête les tirs, mais pas de dégats.
		FireHitCheck(gpMst29Spr[i] + nImg, pMst->nPosX  + (gpMst29OffsX[i] << 8), pMst->nPosY + (gpMst29OffsY[i] << 8) + pSpe->nOffsY, e_ShotOrg_Player, 0);
		// Player hit.
		// On récupère la zone de col du sprite.
		if (SprGetRect(gpMst29Spr[i] + nImg, e_SprRectZone_RectCol, &sRect1) == 0) continue;
		if (sRect1.nType != e_SprRect_Rect) continue;
		Enemy_PlayerBlowRect(&sRect1, pMst->nPosX + (gpMst29OffsX[i] << 8), pMst->nPosY + (gpMst29OffsY[i] << 8) + pSpe->nOffsY, SLUG_ENERGY_MAX);
	}

	return (e_MstState_Managed);
}

//=============================================================================

/*
struct SAAC { u8	nTb0[80]; };
assert(sizeof(struct SAAC) < MST_COMMON_DATA_SZ);
*/

#ifndef NDEBUG
// Debug, vérification de la taille des structures.
void Mst20CheckStructSizes(void)
{
	// Mst20: Pas de struct spécifique => Boss.
	assert(sizeof(struct SMst21_ScrollY1) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst22_ExplodingBarrel0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst23_FallingObjects0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst24_HardObjects0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst25_RocketDiver0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst26_Girida0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst27_HalfBoss0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst28_Masknell0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst29_Whale0) < MST_COMMON_DATA_SZ);
}
#endif

