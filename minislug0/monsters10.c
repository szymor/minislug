
#include "includes.h"
#include "monsters_inc.h"

// Mst 14 : Rifle+cachette : S'arranger pour qu'on n'ait pas de chemin en dessous. (Petit flick droite/gauche quand il se rel�ve).

//=============================================================================
// Mst 10 : M�duse.

#define	MST10_SPD	0x100
#define	MST10_DEC	0x008
void Mst10_Init_Jellyfish(struct SMstCommon *pMst, u8 *pData)
{
	pMst->nSpd = -MST10_SPD;
	pMst->nSpdY = -(rand() & 0x38);	// Un peu d'al�atoire dans la vitesse.

}

s32 Mst10_Main_Jellyfish(struct SMstCommon *pMst)
{
	u32	nSpr;

	nSpr = AnmGetImage(pMst->nAnm);

	// D�placement.
	pMst->nPosY += pMst->nSpd;
	pMst->nPosY += pMst->nSpdY;
	if (pMst->nSpd + MST10_DEC <= 0) pMst->nSpd += MST10_DEC;
	if (AnmCheckStepFlag(pMst->nAnm)) pMst->nSpd = -MST10_SPD;
	// Sortie de l'�cran par le haut ?
	if (pMst->nPosY < -16 << 8) return (e_MstState_Dead);

	// Se prend un tir ?
	u32	nDamage;
	if ((nDamage = FireHitCheck(nSpr, pMst->nPosX, pMst->nPosY, e_ShotOrg_Player, 0)) != 0)
	{
_10JellyfishDeath:
		gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;	// Score.
		DustSet(gAnm_JellyfishSmall_Death_Dust, pMst->nPosX, pMst->nPosY, e_Prio_Joueur + 4, 0);	// Dust.
		return (e_MstState_Dead);	// Mort.
	}
	// Contact avec le joueur ?
	struct SSprRect sRect1;
	if (SprGetRect(nSpr, e_SprRectZone_RectCol, &sRect1))
	if (sRect1.nType == e_SprRect_Rect)
	if (Enemy_PlayerBlowRect(&sRect1, pMst->nPosX, pMst->nPosY, MST_DAMAGE_BULLET)) goto _10JellyfishDeath;

	ChaserTarget_AddToList(pMst->nPosX >> 8, (pMst->nPosY >> 8) - 8);	// Ajout dans la table de vis�e des missiles.

	// Affichage.
	SprDisplay(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies + gnMstPrio);

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 11 : Objet donnant un itm de qu�te par contact.

struct SMst11_AnmRecord
{
	u64	*pAnmWait;		// Attente ctc.
	u64	*pAnmCtcAnm;	// Anim quand contact, anim doit encha�ner sur AnmDead.
	u64	*pAnmDead;		// Anim objet inactif.
	u64	*pAnmDust;		// Si pas de AnmDead, dust de disparition.
};
struct SMst11_AnmRecord gpMst11AnmTb[] =
{
	{ gAnm_Lev8_Lever_Wait, gAnm_Lev8_Lever_CtcAnm, gAnm_Lev8_Lever_Dead, NULL },
	{ gAnm_Lev8_Lock_Wait, NULL, NULL, NULL },
};

// Phases.
enum
{
	e_Mst11_Wait = 0,	// Attente du joueur.
	e_Mst11_Dead,		// Item donn�, inactif.

};

// Structure sp�cifique.
struct SMst11_ObjItemCtc
{
	u8	nType;
	u8	nTouchType;		// bit 0, par touch� / bit 1, par tir.
	u8	nItemGive;		// N� item de qu�te � donner.
	u8	nPrio;
};

void Mst11_Init_ObjItemCtc(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst11_ObjItemCtc	*pSpe = (struct SMst11_ObjItemCtc *)pMst->pData;
	u32	nVal;

	nVal = GetBits(0, 7, pData, 0);
	pSpe->nType = nVal;
	nVal = GetBits(8, 9, pData, 0);
	pSpe->nTouchType = nVal + 1;
	nVal = GetBits(10, 11, pData, 0);
	pSpe->nPrio = (nVal ? e_Prio_Ennemies - 1 : 255);
	nVal = GetBits(16, 23, pData, 0);
	pSpe->nItemGive = nVal;

	// L'objet a d�j� �t� activ� ?
	if (gpMstQuestItems[pSpe->nItemGive])
	{
		pMst->nPhase = e_Mst11_Dead;	// D�j� activ�.
		AnmSet(gpMst11AnmTb[pSpe->nType].pAnmDead, pMst->nAnm);		// Anim.
	}
	else
	{
		pMst->nPhase = e_Mst11_Wait;	// Par d�faut, wait.
		AnmSet(gpMst11AnmTb[pSpe->nType].pAnmWait, pMst->nAnm);		// Anim de wait.
	}

}

s32 Mst11_Main_ObjItemCtc(struct SMstCommon *pMst)
{
	struct SMst11_ObjItemCtc	*pSpe = (struct SMst11_ObjItemCtc *)pMst->pData;
	u32	nSpr;

	// Sortie de l'�cran ?
	if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);

	switch (pMst->nPhase)
	{
	case e_Mst11_Wait:		// Attente touch�.
		{
			u32	nCtc = 0;

			nSpr = AnmGetLastImage(pMst->nAnm);
			// Si touch� par contact, contact avec le joueur ?
			if (pSpe->nTouchType & 1)
			{
				if (SprCheckColBox(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8,
					gShoot.nPlayerSprCol, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8)) nCtc = 1;
			}
			// Touch� par un tir ?
			if (pSpe->nTouchType & 2)
			{
				if (FireHitCheck(nSpr, pMst->nPosX, pMst->nPosY, e_ShotOrg_Player, 0) != 0) nCtc = 1;
			}

			// S'il y a eu activation
			if (nCtc)
			{
				pMst->nPhase = e_Mst11_Dead;
				gpMstQuestItems[pSpe->nItemGive] = 1;

				// Il y a un dust ?
				if (gpMst11AnmTb[pSpe->nType].pAnmDust != NULL)
					DustSet(gpMst11AnmTb[pSpe->nType].pAnmDust, pMst->nPosX, pMst->nPosY, e_Prio_DustUnder, 0);

				// Il y a une anim de contact (qui enchainera sur l'anim inactive) ?
				if (gpMst11AnmTb[pSpe->nType].pAnmCtcAnm != NULL)
				{
					AnmSet(gpMst11AnmTb[pSpe->nType].pAnmCtcAnm, pMst->nAnm);
					break;
				}
				// Sinon, il y a une anim de mort seule ?
				if (gpMst11AnmTb[pSpe->nType].pAnmDead != NULL)
				{
					AnmSet(gpMst11AnmTb[pSpe->nType].pAnmDead, pMst->nAnm);
					break;
				}
				// Si on a pas quitt� le case, il n'y a pas d'anims, on tue le monstre.
				return (e_MstState_Dead);
			}

		}
		break;

	case e_Mst11_Dead:		// D�j� activ�.
		break;
	}

	// Affichage.
	nSpr = AnmGetImage(pMst->nAnm);
	SprDisplay(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8, pSpe->nPrio);

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 12 : Monstre invisble pour bloquer le h�ros.

// Structure sp�cifique.
struct SMst12_HeroBlocker
{
	u8	nHeight;
	u8	nWidth;
	u8	nShotBlk;		// 0 = Ne bloque pas les tirs / 1 = Bloque les tirs.
	u8	nItemWait;		// N� item de qu�te attendu pour dispara�tre.
};

void Mst12_Init_HeroBlocker(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst12_HeroBlocker	*pSpe = (struct SMst12_HeroBlocker *)pMst->pData;

	pSpe->nHeight = GetBits(0, 7, pData, 0);
	pSpe->nWidth = GetBits(8, 14, pData, 0);
	pSpe->nShotBlk = GetBits(15, 15, pData, 0);
	pSpe->nItemWait = GetBits(16, 23, pData, 0);

}

s32 Mst12_Main_HeroBlocker(struct SMstCommon *pMst)
{
	struct SMst12_HeroBlocker	*pSpe = (struct SMst12_HeroBlocker *)pMst->pData;
	struct SSprRect sRect1;

	// Sortie de l'�cran ?
	if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);

	// Disparition ? (En d�but de routine en cas de r�init du monstre).
	if (pSpe->nItemWait)
	if (gpMstQuestItems[pSpe->nItemWait])
		return (e_MstState_Dead);

	// H�ros en contact ?
	if (SprGetRect(gShoot.nPlayerSprCol, e_SprRectZone_RectCol, &sRect1))
	if (sRect1.nType == e_SprRect_Rect)
	if (gShoot.nPlayerPosY + (sRect1.nY1 << 8) < pMst->nPosY)
	if (pSpe->nHeight == 0 || gShoot.nPlayerPosY + (sRect1.nY2 << 8) > pMst->nPosY - ((s32)pSpe->nHeight << 12))
	{
		s32	nMstX;
		if (gShoot.nPlayerPosX <= pMst->nPosX)
		{
			// Le joueur est � gauche.
			nMstX = pMst->nPosX & ~0xFFF;
			if (gShoot.nPlayerPosX + (sRect1.nX2 << 8) > nMstX)
			{
				gShoot.nPlayerPosX = nMstX - (sRect1.nX2 << 8);
				gShoot.nPlayerSpdX = 0;
			}
		}
		else
		{
			// Le joueur est � droite.
			nMstX = (pMst->nPosX & ~0xFFF) + (pSpe->nWidth << 12) - 0x100;
			if (gShoot.nPlayerPosX + (sRect1.nX1 << 8) < nMstX)
			{
				gShoot.nPlayerPosX = nMstX - (sRect1.nX1 << 8);
				gShoot.nPlayerSpdX = 0;
			}
		}
	}

	// Bloque les tirs ?
	if (pSpe->nShotBlk)
	{
		// On cr�� un rectangle de collision correspondant � la taille du monstre.
		sRect1.nType = e_SprRect_Rect;
		sRect1.nX1 = -((pMst->nPosX >> 8) & 0x0F);
		sRect1.nX2 = sRect1.nX1 + (pSpe->nWidth << 4) - 1;
		sRect1.nY1 = (pSpe->nHeight == 0 ? -(pMst->nPosY >> 8) : -((pSpe->nHeight << 4) - 1));
		sRect1.nY2 = 0;
		// Teste les tirs.
		FireHitCheckRect(&sRect1, pMst->nPosX, pMst->nPosY, e_ShotOrg_Player, 0);
	}

/*
	// Disparition ?
	if (pSpe->nItemWait)
	if (gpMstQuestItems[pSpe->nItemWait])
		return (e_MstState_Dead);
*/

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 13 : Monstre pour passer � la map suivante (=> Donne l'item de qu�te 255).

/*
[mst13] Next level	; Monstre pour passer � la map suivante.
Height = 0:7:		; Hauteur sur laquelle tester, en blocs. (Depuis le monstre et au dessus, 0 = Hauteur infinie).
*/

// Structure sp�cifique.
struct SMst13_NextLevel
{
	u8	nHeight;
};

void Mst13_Init_NextLevel(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst13_NextLevel	*pSpe = (struct SMst13_NextLevel *)pMst->pData;
	u32	nVal;

	nVal = GetBits(0, 7, pData, 0);
	pSpe->nHeight = nVal;

}

s32 Mst13_Main_NextLevel(struct SMstCommon *pMst)
{
	struct SMst13_NextLevel	*pSpe = (struct SMst13_NextLevel *)pMst->pData;
	struct SSprRect sRect1;

	// Sortie de l'�cran ?
	if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);

	// H�ros en contact ?
	if (SprGetRect(gShoot.nPlayerSprCol, e_SprRectZone_RectCol, &sRect1))
	if (sRect1.nType == e_SprRect_Rect)
	if (gShoot.nPlayerPosY + (sRect1.nY1 << 8) < pMst->nPosY)
	if (pSpe->nHeight == 0 || gShoot.nPlayerPosY + (sRect1.nY2 << 8) > pMst->nPosY - ((s32)pSpe->nHeight << 12))
	{
		s32	nMstX;
		if (gShoot.nPlayerPosX <= pMst->nPosX)
		{
			// Le joueur est � gauche.
			nMstX = pMst->nPosX & ~0xFFF;
			if (gShoot.nPlayerPosX + (sRect1.nX2 << 8) <= nMstX) return (e_MstState_Managed);
		}
		else
		{
			// Le joueur est � droite.
			nMstX = (pMst->nPosX & ~0xFFF) + (1 << 12) - 0x100;
			if (gShoot.nPlayerPosX + (sRect1.nX1 << 8) >= nMstX) return (e_MstState_Managed);
		}
		// Donne l'item qui va bien.
		gpMstQuestItems[MST_QUEST_ITEM_NEXT_LEVEL] |= 1;
		return (e_MstState_Dead);
	}
	return (e_MstState_Managed);

}

//=============================================================================
// Mst 14 : Rebel soldier (Rifle / Mortar / LRAC / Gun (std) / Grenade (std) / Shield).

// Anims de mort.
u64 *gpRebSoldierDeatAnm[e_ShotDamageType_Max] = { gAnm_RebSoldier_DeathStd_Dust, gAnm_RebSoldier_DeathBlow_Dust, gAnm_RebSoldier_DeathBurn_Dust };
// Anims du Mst14.
u64	*gpMST14_AnmWaitTb[] = { gAnm_RebSoldier_Rifle_WaitUp, gAnm_RebSoldier_Mortar_CrouchWait, gAnm_RebSoldier_LRAC_Idle, gAnm_RebSoldier_Std_Idle, gAnm_RebSoldier_Std_Idle, gAnm_RebSoldier_Shield_Idle, };
u64	*gpMST14_AnmRunTb[] = { gAnm_RebSoldier_Rifle_Run, gAnm_RebSoldier_Std_Run, gAnm_RebSoldier_LRAC_Run, gAnm_RebSoldier_Std_Run, gAnm_RebSoldier_Std_Run, gAnm_RebSoldier_Shield_Run, };
u64	*gpMST14_AnmShotTb[] = { gAnm_RebSoldier_Rifle_Shot, gAnm_RebSoldier_Mortar_Shot, gAnm_RebSoldier_LRAC_Shot, gAnm_RebSoldier_Std_Shot_Gun, gAnm_RebSoldier_Std_Shot_Handgrenade, gAnm_RebSoldier_Shield_Idle, };
u64	*gpMST14_AnmShotDustTb[] = { gAnm_RebSoldier_Rifle_Shot_Dust, gAnm_RebSoldier_Mortar_Shot_Dust, gAnm_RebSoldier_LRAC_Shot_Dust, NULL, NULL, NULL, };
u64	*gpMST14_AnmJumpTb[] = { gAnm_RebSoldier_Std_Jump, gAnm_RebSoldier_Std_Jump, gAnm_RebSoldier_LRAC_Jump, gAnm_RebSoldier_Std_Jump, gAnm_RebSoldier_Std_Jump, gAnm_RebSoldier_Shield_Jump, };
u64	*gpMST14_AnmFallTb[] = { gAnm_RebSoldier_Std_Fall, gAnm_RebSoldier_Std_Fall, gAnm_RebSoldier_LRAC_Fall, gAnm_RebSoldier_Std_Fall, gAnm_RebSoldier_Std_Fall, gAnm_RebSoldier_Shield_Fall, };
u64	*gpMST14_AnmParachutingTb[] = { gAnm_RebSoldier_Std_Parachuting, gAnm_RebSoldier_Std_Parachuting, gAnm_RebSoldier_Std_Parachuting, gAnm_RebSoldier_Std_Parachuting, gAnm_RebSoldier_Std_Parachuting,  gAnm_RebSoldier_Std_Parachuting, };
u8	gpMST14_ShotTb[] = { e_Shot_Enemy_RebSoldier_Bullet0, e_Shot_Enemy_RebSoldier_MortarRocket0, e_Shot_Enemy_RebSoldier_LRAC_Rocket0, e_Shot_Enemy_RebSoldier_Bullet0, e_Shot_Enemy_RebSoldier_Handgrenade0, e_Shot_Enemy_RebSoldier_Bullet0, };

#define	MST14_ZONE_MIN	8
#define	MST14_ZONE_MAX	9
u8	gpnMST14_ZoneMin[] = { MST14_ZONE_MIN, MST14_ZONE_MIN, MST14_ZONE_MIN, MST14_ZONE_MIN, MST14_ZONE_MIN, MST14_ZONE_MIN-5 };
u8	gpnMST14_ZoneMax[] = { MST14_ZONE_MAX, MST14_ZONE_MAX, MST14_ZONE_MAX, MST14_ZONE_MAX, MST14_ZONE_MAX, MST14_ZONE_MAX-5 };

/*
[mst14] Reb Soldier 0
Type = 0:7: Rifle - Mortar - LRAC - Gun_Grenade - RESERVED - Shield
OffsX = -8:11:			; D�calage X.
Move = 12:12: No - Yes		; Le soldat bouge ou pas.
From = 13:13: Right - Left	; Apparition � droite ou � gauche de l'�cran.
Parachute = 14:14: No - Yes	; Parachut� ?
Jump = 15:16: No - Random - Always	; Saut en bout de pf.
Org OffsX = -17:22:		; D�calage de l'origine de la zone.
Special Init = 23:26: None - Truck Jump - SuspSub Jump - Hero Height -	; Inits sp�ciales.
RESERVED2 = 27:31:
*/

// Structure en commun pour tous les types de rebel soldier. (En vue d'un passage d'un type � l'autre).
struct SMst14_RebelSoldier0
{
	u8	nType;
	//
	u8	nCntWait;
	u8	nCachette;		// b0 = mvt impossible � gauche / b1 = mvt imposs � droite + b2 et b3 pour Rifle.
	u8	nMove;
	u16	nOrgX;
	u8	nCnt0;			// Compteur misc. Sinus du parachute. etc...
	s8	nParachuteAnm;	// Slot de l'anim du parachute.
	u8	nJump;
	u8	nOrgPhase;		// !!! Attention, vrai bug vicieux !!! La phase peut changer dans les traitements communs. Du coup, quand on appelle les traitements sp�cifiques, on n'est plus dans la m�me phase ! => Ex : Le rifle qui se l�ve/descend/se l�ve...
	s32	nTargetX, nTargetY;
	u16	nHeroInactivity;	// Compteur d'inactivit� du h�ros (shield).
	u8	nStrike1Time;	// Flag pour ne faire le toucher qu'une fois.

};

// Phases communes.
enum
{
	e_Mst14_FromLeftWait = 0,	// Attente pour apparition depuis la gauche.
	e_Mst14_ParachuteWait,		// Parachute, attente pour apparition.
	// > Au del�, couteau + attracteur de roquette.
	e_Mst14_Parachuting,	// Parachute descente.
	// > Au d�l�, gravtit�.
	e_Mst14_Wait,		// Attente.
	e_Mst14_Move,		// D�placement.
	e_Mst14_Shot,		// Tir.
	e_Mst14_Jump,		// Saut.
	e_Mst14_Fall,		// Chute.
	//
	e_Mst14_CommonLast,		// equ pour phase suivante dans les phases sp�cifiques.
};

// plus tard en prm...
#define	MST14_MORTAR_AREA	8
#define	MST14_MORTAR_SPD	0x180

#define	MST14_PARACHUTE_SPDY_MAX	0x100
#define MST14_JUMP_SPDY	-0x280

// Calcul des zones min et max pour les fct move et wait.
// On modifie les min et max pour forcer les soldats � entrer dans l'�cran quand ils sont au del� de l'�cran, mais dans les bornes "normales" (donc ils n'entrent pas).
void Mst14_sub_ZoneMinMax(s32 *pnZoneMin, s32 *pnZoneMax, struct SMstCommon *pMst)
{
	struct SMst14_RebelSoldier0	*pSpe = (struct SMst14_RebelSoldier0 *)pMst->pData;

	*pnZoneMax = (u32)gpnMST14_ZoneMax[pSpe->nType] << 12;
	if (pMst->nPosX <= gShoot.nPlayerPosX)
	{	// Le mst est � gauche du joueur.
		if (gShoot.nPlayerPosX - *pnZoneMax < gScrollPos.nPosX + (1 << 12))
			*pnZoneMax = gShoot.nPlayerPosX - (gScrollPos.nPosX + (1 << 12));
	}
	else
	{	// Le mst est � droite du joueur.
		if (gShoot.nPlayerPosX + *pnZoneMax > gScrollPos.nPosX + (SCR_Width << 8) - (1 << 12))
			*pnZoneMax = (gScrollPos.nPosX + (SCR_Width << 8) - (1 << 12)) - gShoot.nPlayerPosX;
	}
	if (*pnZoneMax < 0x2000) *pnZoneMax = 0x2000;
//	if (*pnZoneMax < 0x1000) *pnZoneMax = 0x1000;

	*pnZoneMin = (u32)gpnMST14_ZoneMin[pSpe->nType] << 12;
	if (*pnZoneMin > *pnZoneMax - 0x1000) *pnZoneMin = *pnZoneMax - 0x1000;


/*
//>tst
char	pTb[8+1] = "       0";
MyItoA(*pnZoneMin>>12, pTb);
Font_Print(((pMst->nPosX - gScrollPos.nPosX)>>8)-48, (pMst->nPosY - gScrollPos.nPosY)>>8, pTb, 0);
char	pTb2[8+1] = "       0";
MyItoA(*pnZoneMax>>12, pTb2);
Font_Print(((pMst->nPosX - gScrollPos.nPosX)>>8)-48, ((pMst->nPosY - gScrollPos.nPosY)>>8)+8, pTb2, 0);
//<tst
*/

}

// Rebel soldier : Phase commune e_Mst14_Move.
void Mst14_Phase_Move(struct SMstCommon *pMst)
{
	struct SMst14_RebelSoldier0	*pSpe = (struct SMst14_RebelSoldier0 *)pMst->pData;
	u32	nFlipX = 0;

	pSpe->nCachette = 0;	// RAZ des bits de blocage.


s32	nZoneMin, nZoneMax;
Mst14_sub_ZoneMinMax(&nZoneMin, &nZoneMax, pMst);


	// Si perso dans la zone de tir, stop.
////	if (ABS(pMst->nPosX - gShoot.nPlayerPosX) < (MST14_ZONE_MAX << 12) &&
////		ABS(pMst->nPosX - gShoot.nPlayerPosX) >= (MST14_ZONE_MIN << 12))
//	if (ABS(pMst->nPosX - gShoot.nPlayerPosX) < ((u32)gpnMST14_ZoneMax[pSpe->nType] << 12) &&
//		ABS(pMst->nPosX - gShoot.nPlayerPosX) >= ((u32)gpnMST14_ZoneMin[pSpe->nType] << 12))
	if (ABS(pMst->nPosX - gShoot.nPlayerPosX) < nZoneMax &&
		ABS(pMst->nPosX - gShoot.nPlayerPosX) >= nZoneMin)
	{
		pMst->nPhase = e_Mst14_Wait;
		return;
	}

	// Si on est trop loin, on se tourne vers le joueur (� cause du cas de l'arriv�e d'un saut d�j� trop loin et pas tourn� dans le bon sens : Le soldat court jusqu'� un stop (mur, trou, zone) avant de revenir).
////	if (ABS(pMst->nPosX - gShoot.nPlayerPosX) >= (MST14_ZONE_MAX << 12))
//	if (ABS(pMst->nPosX - gShoot.nPlayerPosX) >= ((u32)gpnMST14_ZoneMax[pSpe->nType] << 12))
	if (ABS(pMst->nPosX - gShoot.nPlayerPosX) >= nZoneMax)
		pMst->nFlipX = (pMst->nPosX - gShoot.nPlayerPosX >= 0 ? 0 : 1);		// Sens.

/*
	// Si le joueur est tr�s proche, on court dans le sens inverse.
	if (ABS(pMst->nPosX - gShoot.nPlayerPosX) < (MST14_ZONE_MIN << 12) && pMst->nFlipX == (pMst->nPosX < gShoot.nPlayerPosX ? 1 : 0)) pMst->nFlipX ^= 1;
*/

	// Si le joueur est tr�s proche, on court dans le sens inverse.
////	if (ABS(pMst->nPosX - gShoot.nPlayerPosX) < (MST14_ZONE_MIN << 12) && pMst->nFlipX == (pMst->nPosX < gShoot.nPlayerPosX ? 1 : 0))
//	if (ABS(pMst->nPosX - gShoot.nPlayerPosX) < ((u32)gpnMST14_ZoneMin[pSpe->nType] << 12) && pMst->nFlipX == (pMst->nPosX < gShoot.nPlayerPosX ? 1 : 0))
	if (ABS(pMst->nPosX - gShoot.nPlayerPosX) < nZoneMin && pMst->nFlipX == (pMst->nPosX < gShoot.nPlayerPosX ? 1 : 0))
	{
		pMst->nFlipX ^= 1;
		nFlipX = 1;
	}
	// Zone limite.
	s32	nNextX = pMst->nPosX + (pMst->nFlipX ? MST14_MORTAR_SPD : -MST14_MORTAR_SPD);
	if ((pMst->nFlipX == 0 && nNextX >> 12 < pSpe->nOrgX - MST14_MORTAR_AREA) ||	// Test avec d�passement de zone, permet au monstre de revenir vers sa zone.
		(pMst->nFlipX == 1 && nNextX >> 12 > pSpe->nOrgX + MST14_MORTAR_AREA)) goto _M14Stop;

	// Du vide dans le bloc d'� c�t� ? (Pour s'arr�ter un peu avant les bords de pf).
//	s32	nHt = BlockGetGroundLevel((pMst->nPosX >> 8) + (pMst->nFlipX ? 14 : -14), (pMst->nPosY >> 8) + 14);
	s32	nHt = BlockGetGroundLevel((pMst->nPosX >> 8) + (pMst->nFlipX ? 14 : -14), (pMst->nPosY >> 8));
//	if (nHt > 0)
	if (nHt > 14)
	{
		// On arrive en bout de plateforme. Saut ?
		if (pSpe->nJump == 2 || (pSpe->nJump == 1 && (rand() & 1)))
		{
			pMst->nPhase = e_Mst14_Jump;	// Saut.
			pMst->nSpd = MST14_MORTAR_SPD;	// Pour amortissement de la vitesse x.
			pMst->nSpdY = MST14_JUMP_SPDY;
			pMst->nGround = 0;
			return;
		}
		// Pas de saut.
		goto _M14Stop;
	}
	// Du dur ?
	if (nHt < -14) goto _M14Stop;

	// D�placement.
	if (Mst_BasicMove(pMst, MST14_MORTAR_SPD, 0))
	{
_M14Stop:
		pSpe->nCachette |= 1 << pMst->nFlipX;	// On note le c�t� vers lequel le d�placement n'est pas possible.
		pMst->nPhase = e_Mst14_Wait;
		if (nFlipX) pMst->nFlipX ^= 1;			// Cas merdique : Si on s'est tourn� � l'oppos� du joueur dans la fonction et qu'on passe en stop � la fin, on se retourne vers le joueur. (Arrive aves les soldats qui sautent du mst15).
		return;
	}

}

// Rebel soldier : Phase commune e_Mst14_Wait.
// In: nShotDy en pixels, delta y entre le monstre et le joueur dans lequel le monstre peut d�clencher l'attaque. 0 = Pas de zone, on tire tout le temps.
void Mst14_Phase_Wait(struct SMstCommon *pMst, u32 nShotDy)
{
	struct SMst14_RebelSoldier0	*pSpe = (struct SMst14_RebelSoldier0 *)pMst->pData;


s32	nZoneMin, nZoneMax;
Mst14_sub_ZoneMinMax(&nZoneMin, &nZoneMax, pMst);


	if (AnmGetKey(pMst->nAnm) == e_AnmKey_Null)
	{
		// Tourne le monstre vers le joueur.
		if (pSpe->nMove)
		{
			u32	nNewFlipX;
			nNewFlipX = (pMst->nPosX - gShoot.nPlayerPosX >= 0 ? 0 : 1);		// Sens.
			if (pMst->nFlipX != nNewFlipX)		// Changement de sens ?
			{
				if ((pSpe->nCachette & (1 << (pMst->nFlipX ^ 1))) == 0)	// La direction oppos�e n'est pas bloqu�e ?
				{
					pMst->nFlipX = nNewFlipX;
				}
				else
				{
					pMst->nPhase = e_Mst14_Move;
					return;
				}
			}
		}
		else
		pMst->nFlipX = (pMst->nPosX - gShoot.nPlayerPosX >= 0 ? 0 : 1);		// Sens.

		// On est � port�e de tir.
////		if (ABS(pMst->nPosX - gShoot.nPlayerPosX) < ((MST14_ZONE_MAX+1) << 12))
//		if (ABS(pMst->nPosX - gShoot.nPlayerPosX) < (((u32)gpnMST14_ZoneMax[pSpe->nType] + 1) << 12))
		if (ABS(pMst->nPosX - gShoot.nPlayerPosX) < nZoneMax + 0x1000)
		{
			// Joueur trop pr�s ?
////			if (ABS(pMst->nPosX - gShoot.nPlayerPosX) < ((MST14_ZONE_MIN-1) << 12))
//			if (ABS(pMst->nPosX - gShoot.nPlayerPosX) < (((u32)gpnMST14_ZoneMin[pSpe->nType] - 1) << 12))
			if (ABS(pMst->nPosX - gShoot.nPlayerPosX) < nZoneMin - 0x1000)
			{
				if (pSpe->nMove && (pSpe->nCachette & (1 << (pMst->nFlipX ^ 1))) == 0)	// La direction oppos�e n'est pas bloqu�e ?
				{
					pMst->nPhase = e_Mst14_Move;
					return;
				}
			}

//			if (gShoot.nDeathFlag == 0)	// Si le h�ros n'est pas mort...
			if (MST_SHOT_COND)	// Si le h�ros n'est pas mort...
			if (nShotDy == 0 || ABS(pMst->nPosY - gShoot.nPlayerPosY) <= nShotDy)	// Si dans la zone Y.
			if (pMst->nPosX >= gScrollPos.nPosX - 0x1000 && pMst->nPosX <= gScrollPos.nPosX + (SCR_Width << 8) + 0x1000)	// On ne tire pas si en dehors de l'�cran.
			{
				if (pSpe->nCntWait)
					pSpe->nCntWait--;
				else
				{
					pMst->nPhase = e_Mst14_Shot;
					return;
				}
			}
		}
		else
		// Le joueur est trop loin (s'est �loign�).
		if (pSpe->nMove && (pSpe->nCachette & (1 << pMst->nFlipX)) == 0)	// Si mst avec mvt et mvt pas pr�c�dement bloqu� dans cette direction...
		{
			pMst->nPhase = e_Mst14_Move;
			return;
		}
	}

}

//

// Mst14 : Attente que le scroll soit au bon endroit.
// Note : FromLeft ignor�, s'arranger avec l'offset.
void Mst14_Parachute_Wait(struct SMstCommon *pMst)
{
	struct SMst14_RebelSoldier0	*pSpe = (struct SMst14_RebelSoldier0 *)pMst->pData;

	if (pMst->nPosX <= gScrollPos.nPosX + (SCR_Width << 8) - (1 << 12))
	{
		// Init de la descente.
		pMst->nPosX += (((pMst->nPosX >> 8) & 0x0F) - 8) << 13;		// On r�cup�re l'offset.
		pSpe->nOrgX = pMst->nPosX >> 12;	// Du coup l'origine change, important pour le d�placement.
		pMst->nPosY = gScrollPos.nPosY - (4 << 8);		// -4 pour faire sortir les pieds.
		pMst->nPosY -= ABS(((pMst->nPosX >> 8) & 0x0F) - 8) << 11;	// On d�cale selon l'offet, pour en faire tomber plusieurs � la fois et qu'ils ne soient pas au m�me Y.
		pSpe->nCnt0 = (pMst->nPosX >> 12) * 16;			// + D�calage du sinus du parachute.
		pMst->nPhase = e_Mst14_Parachuting;
	}
}

// Mst14 : Descente en parachute.
void Mst14_Parachuting(struct SMstCommon *pMst)
{
	struct SMst14_RebelSoldier0	*pSpe = (struct SMst14_RebelSoldier0 *)pMst->pData;

	// Tourne le soldier vers le h�ros.
	pMst->nFlipX = (pMst->nPosX - gShoot.nPlayerPosX >= 0 ? 0 : 1);
	// Oscillation X.
	pMst->nPosX += gVar.pSin[pSpe->nCnt0];
	pSpe->nCnt0 += 3;			// Incr�mentation de l'index pour oscillation.
	// Arriv�e au sol ?
	if (pMst->nPosY < 0)
	{
		// Cas particulier, le parachutiste est au dessus de la map.
		pMst->nSpdY = MST14_PARACHUTE_SPDY_MAX;
		pMst->nPosY += pMst->nSpdY;
	}
	else
	{
		// Gravit�.
		Mst_Gravity2(pMst, GRAVITY/2, MST14_PARACHUTE_SPDY_MAX);
		// On touche le sol ?
		if (pMst->nGround)
		{
			pMst->nPhase = (pSpe->nMove ? e_Mst14_Move : e_Mst14_Wait);
			pSpe->nParachuteAnm = AnmSet(gAnm_Parachute_Fold, pSpe->nParachuteAnm);	// Repli du parachute.
		}
	}

}

// Mst14 : Affichage du parachute.
void Mst14_Parachute_Display(struct SMstCommon *pMst)
{
	struct SMst14_RebelSoldier0	*pSpe = (struct SMst14_RebelSoldier0 *)pMst->pData;
	u32	nSpr;

	// Parachute en plus ?
	if (pSpe->nParachuteAnm != -1)
	{
		if ((nSpr = AnmGetImage(pSpe->nParachuteAnm)) == (u32)-1)
			pSpe->nParachuteAnm = -1;	// Anim termin�e.
		else
			SprDisplay(nSpr ^ (pMst->nFlipX ? 0 : SPR_Flip_X), (pMst->nPosX >> 8), (pMst->nPosY >> 8) - 1, e_Prio_Ennemies + gnMstPrio - 1);
	}

}

//

// Mst14 : Gestion de la gravit�.
void Mst14_Gravity(struct SMstCommon *pMst)
{
	struct SMst14_RebelSoldier0	*pSpe = (struct SMst14_RebelSoldier0 *)pMst->pData;

	if (pMst->nPhase > e_Mst14_Parachuting)
	{
		u32	nLastGnd = pMst->nGround;
		Mst_Gravity(pMst);
		// Le sol disparait ?
		if (pMst->nGround == 0 && nLastGnd)
		if (pMst->nPhase != e_Mst14_Fall)
		{
			// Initialisation de la chute.
			AnmSet(gpMST14_AnmFallTb[pSpe->nType], pMst->nAnm);
			pMst->nPhase = e_Mst14_Fall;
		}
	}

}

//== LRAC Soldier.

// Phases.
enum
{
	e_Mst14LRAC_Shot2 = e_Mst14_CommonLast,	// Apr�s la vis�e, le tir.
	e_Mst14LRAC_Shot3,

};

void Mst14Sub_Init_LRACSoldier0(struct SMstCommon *pMst, u8 *pData)
{
/*
exemple
	struct SMst14_RebelSoldier0	*pSpe = (struct SMst14_RebelSoldier0 *)pMst->pData;
	s32	nVal;
	nVal = GetBits(8, 11, pData, 1);	// Offset X.
*/
}

u32 gpnM14SprAim[] =	// Sprite de vis�e en fonction de l'angle (32 valeurs).
{
	(e_Spr_RebSoldier_LRAC_Aim + 4) | SPR_Flip_X,
	(e_Spr_RebSoldier_LRAC_Aim + 4) | SPR_Flip_X,
	(e_Spr_RebSoldier_LRAC_Aim + 3) | SPR_Flip_X,
	(e_Spr_RebSoldier_LRAC_Aim + 3) | SPR_Flip_X,
	(e_Spr_RebSoldier_LRAC_Aim + 2) | SPR_Flip_X,
	(e_Spr_RebSoldier_LRAC_Aim + 2) | SPR_Flip_X,
	(e_Spr_RebSoldier_LRAC_Aim + 1) | SPR_Flip_X,
	(e_Spr_RebSoldier_LRAC_Aim + 1) | SPR_Flip_X,
	(e_Spr_RebSoldier_LRAC_Aim + 0) | SPR_Flip_X,
	(e_Spr_RebSoldier_LRAC_Aim + 0),
	(e_Spr_RebSoldier_LRAC_Aim + 1),
	(e_Spr_RebSoldier_LRAC_Aim + 1),
	(e_Spr_RebSoldier_LRAC_Aim + 2),
	(e_Spr_RebSoldier_LRAC_Aim + 2),
	(e_Spr_RebSoldier_LRAC_Aim + 3),
	(e_Spr_RebSoldier_LRAC_Aim + 3),
	(e_Spr_RebSoldier_LRAC_Aim + 4),
	(e_Spr_RebSoldier_LRAC_Aim + 4),
	(e_Spr_RebSoldier_LRAC_Aim + 5),
	(e_Spr_RebSoldier_LRAC_Aim + 5),
	(e_Spr_RebSoldier_LRAC_Aim + 6),
	(e_Spr_RebSoldier_LRAC_Aim + 6),
	(e_Spr_RebSoldier_LRAC_Aim + 7),
	(e_Spr_RebSoldier_LRAC_Aim + 7),
	(e_Spr_RebSoldier_LRAC_Aim + 8),
	(e_Spr_RebSoldier_LRAC_Aim + 8) | SPR_Flip_X,
	(e_Spr_RebSoldier_LRAC_Aim + 7) | SPR_Flip_X,
	(e_Spr_RebSoldier_LRAC_Aim + 7) | SPR_Flip_X,
	(e_Spr_RebSoldier_LRAC_Aim + 6) | SPR_Flip_X,
	(e_Spr_RebSoldier_LRAC_Aim + 6) | SPR_Flip_X,
	(e_Spr_RebSoldier_LRAC_Aim + 5) | SPR_Flip_X,
	(e_Spr_RebSoldier_LRAC_Aim + 5) | SPR_Flip_X,
};

s32 Mst14Sub_Main_LRACSoldier0(struct SMstCommon *pMst)
{
	struct SMst14_RebelSoldier0	*pSpe = (struct SMst14_RebelSoldier0 *)pMst->pData;
	u32	nSpr;
	u32	nSpecialSpr = 0;	// 1 pour ne pas faire le GetAnim().

	switch (pSpe->nOrgPhase)	// !!! Attention � bien tester OrgPhase !!!
	{
	case e_Mst14_Parachuting:	// Parachute descente.
		// On rajoute le LRAC derri�re le sprite.
		SprDisplayRotoZoom(e_Spr_RebSoldier_Weapon_LRAC ^ (pMst->nFlipX ? SPR_Flip_X : 0), pMst->nPosX >> 8, (pMst->nPosY >> 8) - 24, e_Prio_Ennemies - 1 + gnMstPrio, (u8)-32, 0x100);
		break;

	case e_Mst14_Shot:
		pMst->nPhase = e_Mst14LRAC_Shot2;
		pSpe->nCntWait = 30;	// Petite attente pour "viser"...
		// Pas de break !
	case e_Mst14LRAC_Shot2:		// Tir : Phase 1 = Aim.
	case e_Mst14LRAC_Shot3:		// Tir : Phase 2 = Shot.
		nSpecialSpr = 1;	// Ne pas faire le GetAnim().
		{
			u8	nAng;
			s32	nAdd;

			if (pMst->nPhase == e_Mst14LRAC_Shot3)
			{
				nAng = pMst->nAngle;	// Tir, la vis�e ne bouge plus.
				nAdd = AnmGetImage(pMst->nAnm);
				if (AnmGetKey(pMst->nAnm) != e_AnmKey_Enemy_Shot)
				{
					nAdd = 0;
					// Repasse en wait apr�s le tir.
					pMst->nPhase = e_Mst14_Wait;
					pSpe->nCntWait = (rand() & 127) | 31;	// Petite attente...
				}
			}
			else
			{	// Vis�e normale.
				nAng = fatan2(-((gShoot.nPlayerPosY + (10 << 8)) - pMst->nPosY), gShoot.nPlayerPosX - pMst->nPosX);
				nAdd = 0;
			}
			pMst->nAngle = nAng;
			nAng += 8;		// 8 * 32 => On ajoute la moiti� de l'angle. => 8 et pas 4, car en fait il y a 16 positions seulement, doubl�es pour le milieu.
			nAng >>= 3;		// /8.

			nSpr = gpnM14SprAim[nAng];
			pMst->nFlipX = (nSpr & SPR_Flip_X ? 1 : 0);
			nSpr &= ~SPR_Flip_X;
			nSpr += nAdd;
		}

		// Anim de tir ?
		if (pMst->nPhase == e_Mst14LRAC_Shot3)
		{
			if (AnmCheckNewImgFlag(pMst->nAnm))		// Nouvelle image ?
			{
				// Est-ce qu'on a une coordonn�e d'origine de tir ?
				struct SSprRect sRect1;
				if (SprGetRect(nSpr | (pMst->nFlipX ? SPR_Flip_X : 0), e_SprRectZone_ShotOrg, &sRect1))
				if (sRect1.nType == e_SprRect_Point)
				{
//					FireAdd(e_Shot_Enemy_RebSoldier_LRAC_Rocket0, pMst->nPosX + (sRect1.nX1 << 8), pMst->nPosY + (sRect1.nY1 << 8), pMst->nAngle);
					// Recalcul de l'angle entre la cible et le point de d�part du tir.
					u8	nAng;
					nAng = fatan2(-((pSpe->nTargetY + (10 << 8)) - pMst->nPosY), pSpe->nTargetX - (pMst->nPosX + (sRect1.nX1 << 8)));
//sans ShotTb[]					FireAdd(e_Shot_Enemy_RebSoldier_LRAC_Rocket0, pMst->nPosX + (sRect1.nX1 << 8), pMst->nPosY + (sRect1.nY1 << 8), nAng);
					FireAdd(gpMST14_ShotTb[pSpe->nType], pMst->nPosX + (sRect1.nX1 << 8), pMst->nPosY + (sRect1.nY1 << 8), nAng);
					// + Dust.
					DustSet(gpMST14_AnmShotDustTb[pSpe->nType], pMst->nPosX + (sRect1.nX1 << 8), pMst->nPosY + (sRect1.nY1 << 8), e_Prio_DustOver, (pMst->nFlipX ? e_DustFlag_FlipX : 0));
				}
			}
		}
		else
		{
			// Tir ?
			if (--pSpe->nCntWait == 0)
			{
				pMst->nPhase = e_Mst14LRAC_Shot3;
				AnmSet(gpMST14_AnmShotTb[pSpe->nType], pMst->nAnm);
				// Sauvegarde de la cible � ce moment pr�cis (au cas ou le joueur soit en train de tomber, par exemple. En 1 frame, il pourrait se d�placer pas mal).
				pSpe->nTargetX = gShoot.nPlayerPosX;
				pSpe->nTargetY = gShoot.nPlayerPosY;
			}
		}
		break;

	default:
		break;
	}

	// Sprite.
	if (nSpecialSpr == 0) nSpr = AnmGetImage(pMst->nAnm);

	// Se prend un tir ?
	if (Mst_ShotCheckHit(pMst, nSpr, gpRebSoldierDeatAnm))
	{
		if (pSpe->nParachuteAnm != -1) AnmReleaseSlot(pSpe->nParachuteAnm);		// Slot de l'anim du parachute.
		// Dust du LRAC.
		gDustExg.nRotInc = SGN(pMst->nPosX - gShoot.nPlayerPosX) * -6;
		DustSetMvt(gAnm_RebSoldier_WeaponLRAC_Dust, pMst->nPosX, pMst->nPosY - 0x1000, SGN(pMst->nPosX - gShoot.nPlayerPosX) * 0x100, -0x380, e_Prio_DustUnder, e_DustFlag_Gravity | e_DustFlag_Rotation);
		return (e_MstState_Dead);
	}

	// Affichage.
	SprDisplay(nSpr ^ (pMst->nFlipX ? SPR_Flip_X : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies + gnMstPrio);
	Mst14_Parachute_Display(pMst);

	return (e_MstState_Managed);
}

//== Mortar Soldier.

/*
// Phases.
enum
{
	e_Mst14Mortar_xxx = e_Mst14_CommonLast,	// Apr�s la course ou un tir, se baisse pour se cacher.

};
*/

void Mst14Sub_Init_MortarSoldier0(struct SMstCommon *pMst, u8 *pData)
{
/*
exemple
	struct SMst14_RebelSoldier0	*pSpe = (struct SMst14_RebelSoldier0 *)pMst->pData;
	s32	nVal;
	nVal = GetBits(8, 11, pData, 1);	// Offset X.
*/
}

s32 Mst14Sub_Main_MortarSoldier0(struct SMstCommon *pMst)
{
	struct SMst14_RebelSoldier0	*pSpe = (struct SMst14_RebelSoldier0 *)pMst->pData;
	u32	nSpr;

	switch (pSpe->nOrgPhase)	// !!! Attention � bien tester OrgPhase !!!
	{
	case e_Mst14_Move:		// D�placement.
		// Un obstacle devant ? (trou ou mur).
		// Test sp�cial pour le mortar qui est plus large que les autres soldats.
		if (pMst->nPhase == e_Mst14_Move)	// ...si la phase n'a pas d�j� chang� !
		{
			s32	nHt;
			s32	nPosY;

			// En +14, test de la fct move. On teste 10 pixels plus loin.
			nPosY = pMst->nPosY >> 8;
			nHt = BlockGetGroundLevel((pMst->nPosX >> 8) + (pMst->nFlipX ? 14 : -14), nPosY);
			nPosY += nHt;
			nHt = BlockGetGroundLevel((pMst->nPosX >> 8) + (pMst->nFlipX ? 24 : -24), nPosY);

			// On arrive en bout de plateforme. Saut ?
			if (nHt > 10)
			{
				if (pSpe->nJump == 2 || (pSpe->nJump == 1 && (rand() & 1)))
				{
					pMst->nPhase = e_Mst14_Jump;	// Saut.
					pMst->nSpd = MST14_MORTAR_SPD;	// Pour amortissement de la vitesse x.
					pMst->nSpdY = MST14_JUMP_SPDY;
					pMst->nGround = 0;
					break;
				}
				goto _M14_Mortar_Stop;
			}
			// On tombe sur un mur ?
			if (nHt < -10)
			{
_M14_Mortar_Stop:
				pSpe->nCachette |= 1 << pMst->nFlipX;	// Flag de stop.
				pMst->nPhase = e_Mst14_Wait;
				break;
			}
		}
		break;

	case e_Mst14_Shot:		// Tir.
		AnmSetIfNew(gpMST14_AnmShotTb[pSpe->nType], pMst->nAnm);

		nSpr = AnmGetLastImage(pMst->nAnm);
		if (Mst_ShotLaunch(pMst, nSpr, gpMST14_ShotTb[pSpe->nType], gpMST14_AnmShotDustTb[pSpe->nType]))
		{
			// Repasse en wait apr�s le tir.
			pMst->nPhase = e_Mst14_Wait;
			pSpe->nCntWait = (rand() & 127) | 31;	// Petite attente...
			break;
		}
		break;

	default:
		break;
	}

	// Sprite.
	nSpr = AnmGetImage(pMst->nAnm);

	// Se prend un tir ?
	if (Mst_ShotCheckHit(pMst, nSpr, gpRebSoldierDeatAnm))
	{
		if (pSpe->nParachuteAnm != -1) AnmReleaseSlot(pSpe->nParachuteAnm);		// Slot de l'anim du parachute.
		return (e_MstState_Dead);
	}

	// Affichage.
	SprDisplay(nSpr ^ (pMst->nFlipX ? SPR_Flip_X : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies + gnMstPrio);
	Mst14_Parachute_Display(pMst);

	return (e_MstState_Managed);
}

//== Rifle Soldier.

// Phases sp�cifiques.
enum
{
	e_Mst14Rifle_HideGetDown = e_Mst14_CommonLast,	// Apr�s la course ou un tir, se baisse pour se cacher.
	e_Mst14Rifle_HideWait,		// Attente cach�.
	e_Mst14Rifle_HideGetUp,		// Hide > se rel�ve.
	e_Mst14Rifle_Strike,		// Frappe avec la crosse.

};

void Mst14Sub_Init_RifleSoldier0(struct SMstCommon *pMst, u8 *pData)
{
/*
exemple
	struct SMst14_RebelSoldier0	*pSpe = (struct SMst14_RebelSoldier0 *)pMst->pData;
	s32	nVal;
	nVal = GetBits(8, 11, pData, 1);	// Offset X.
*/
}

//void Gen_StepDust(u32 nGndCol, u32 nStepFlag, u32 nPosX, u32 nPosY)	// Pour la flotte.

#define	MST14RIFLE_ZONE_DOWN	4
#define	MST14RIFLE_SHOT_DY		(2 << 12)	//((2 << 12) + (8 << 8))

s32 Mst14Sub_Main_RifleSoldier0(struct SMstCommon *pMst)
{
	struct SMst14_RebelSoldier0	*pSpe = (struct SMst14_RebelSoldier0 *)pMst->pData;
	u32	nSpr;

	switch (pSpe->nOrgPhase)	// !!! Attention � bien tester OrgPhase !!!
	{
	case e_Mst14_Parachuting:	// Parachute descente.
	case e_Mst14_Jump:		// Saut.
	case e_Mst14_Fall:		// Chute.
		// On rajoute le fusil derri�re le sprite.
		SprDisplayRotoZoom(e_Spr_RebSoldier_Weapon_Rifle ^ (pMst->nFlipX ? SPR_Flip_X : 0), pMst->nPosX >> 8, (pMst->nPosY >> 8) - 24, e_Prio_Ennemies - 1 + gnMstPrio, (u8)-32, 0x100);
		break;

	case e_Mst14_Move:		// D�placement.
/*
// v1 : Emp�che de monter les pentes � 45...
		// Un obstacle devant ? => On se cache derri�re.
		if (BlockGetGroundLevel((pMst->nPosX >> 8) + (pMst->nFlipX ? 17 : -17), (pMst->nPosY >> 8) - 10) < 0 &&
			pMst->nFlipX == (pMst->nPosX < gShoot.nPlayerPosX ? 1 : 0))
		{
			pSpe->nCachette |= 4 << pMst->nFlipX;	// Flag de cachette.
			pMst->nPhase = e_Mst14Rifle_HideGetDown;
			break;
		}
*/

		// Un obstacle devant ? => On se cache derri�re.
		if (pMst->nPhase == e_Mst14_Move)	// ...si la phase n'a pas d�j� chang� !
		{
			s32	nHt;
			s32	nPosY;

			// En +14, test de la fct move. On teste 3 pixels plus loin.
			nPosY = pMst->nPosY >> 8;
			nHt = BlockGetGroundLevel((pMst->nPosX >> 8) + (pMst->nFlipX ? 14 : -14), nPosY);
			nPosY += nHt;
			nHt = BlockGetGroundLevel((pMst->nPosX >> 8) + (pMst->nFlipX ? 17 : -17), nPosY);

			if (nHt < -3)
			{
				if (pMst->nFlipX == (pMst->nPosX < gShoot.nPlayerPosX ? 1 : 0))
				{	// Se cache.
					pSpe->nCachette |= (4+1) << pMst->nFlipX;	// Flag de cachette + flag de stop.
					pMst->nPhase = e_Mst14Rifle_HideGetDown;
				}
				else
				{	// Pas dans le bon sens, stop.
					pSpe->nCachette |= 1 << pMst->nFlipX;	// Flag de stop.
					pMst->nPhase = e_Mst14_Wait;
				}
			}
		}
		break;

	case e_Mst14_Wait:		// Attente.
		// Frappe avec la crosse ?
		if (gShoot.nVehicleType < e_HeroVehicle_SlugBase)		// Pas de coup de crosse quand slug (=> tir normal).
		if (MST_SHOT_COND)	// Si le h�ros n'est pas mort...
		if (ABS(pMst->nPosY - gShoot.nPlayerPosY) <= MST14RIFLE_SHOT_DY)	// Si dans la zone Y.
		if (ABS(pMst->nPosX - gShoot.nPlayerPosX) < 3 << 12 &&
			pMst->nFlipX == (pMst->nPosX < gShoot.nPlayerPosX ? 1 : 0))
		{
			pMst->nPhase = e_Mst14Rifle_Strike;
			pSpe->nStrike1Time = 0;	// Flag pour ne faire le toucher qu'une fois.
			//
			pSpe->nCntWait = 40;	// Petite attente pour ne pas taper sans arr�t si je joueur reste � port�e.
			AnmSet(gAnm_RebSoldier_Rifle_Strike, pMst->nAnm);
		}
		break;

	case e_Mst14_Shot:		// Tir.
		AnmSetIfNew(gpMST14_AnmShotTb[pSpe->nType], pMst->nAnm);

		nSpr = AnmGetLastImage(pMst->nAnm);
		if (Mst_ShotLaunch(pMst, nSpr, gpMST14_ShotTb[pSpe->nType], gpMST14_AnmShotDustTb[pSpe->nType]))
		{
			pSpe->nCntWait = (rand() & 127) | 31;	// Petite attente...
			// Repasse en wait apr�s le tir.
			pMst->nPhase = e_Mst14_Wait;
			// Si cachette, repasse en mode cach� derri�re un obstacle. Les autres tests (dst + sens) seront faits dans l'autre phase.
			if (pSpe->nCachette & (4 << pMst->nFlipX))	// Flag de cachette ?
			{
				pMst->nPhase = e_Mst14Rifle_HideGetDown;
			}
			break;
		}
		break;

	//===

	case e_Mst14Rifle_HideGetDown:	// Apr�s la course ou un tir, se baisse pour se cacher.
		AnmSetIfNew(gAnm_RebSoldier_Rifle_GetDown, pMst->nAnm);
		if (AnmGetKey(pMst->nAnm) != e_AnmKey_Enemy_Shot)
			pMst->nPhase = e_Mst14Rifle_HideWait;
		else
		// Apr�s le tir, on arrive ici avec la fin de l'anim de tir.
		// Si le h�ros est trop pr�s ou est pass� de l'autre c�t�, on repasse en d�placement normal.
		if (ABS(pMst->nPosX - gShoot.nPlayerPosX) < MST14RIFLE_ZONE_DOWN << 12 ||
			pMst->nFlipX != (pMst->nPosX < gShoot.nPlayerPosX ? 1 : 0))
		{
			pMst->nPhase = e_Mst14_Wait;
			break;
		}
		break;

	case e_Mst14Rifle_HideGetUp:	// Cach� > Se rel�ve.
		AnmSetIfNew(gAnm_RebSoldier_Rifle_GetUp, pMst->nAnm);
		if (AnmGetKey(pMst->nAnm) == e_AnmKey_Enemy_Shot)	//e_AnmKey_Enemy_Shot => car il y a une anim de transition.
		{
			// Si le h�ros est trop pr�s ou est pass� de l'autre c�t�, on repasse en d�placement normal.
			if (ABS(pMst->nPosX - gShoot.nPlayerPosX) < MST14RIFLE_ZONE_DOWN << 12 ||
				pMst->nFlipX != (pMst->nPosX < gShoot.nPlayerPosX ? 1 : 0))
			{
				pMst->nPhase = e_Mst14_Wait;
				break;
			}
			// Et par d�faut, on passe en tir.
			pMst->nPhase = e_Mst14_Shot;
		}
		break;

	case e_Mst14Rifle_HideWait:		// Attente cach�.
		//AnmSetIfNew(gAnm_RebSoldier_Rifle_CrouchWait, pMst->nAnm);		// < Les anims mises avant enchainent sur celle-ci.
		if (AnmGetKey(pMst->nAnm) == e_AnmKey_Null)
		{
			if (MST_SHOT_COND)	// Si le h�ros n'est pas mort...
			{
				if (pMst->nFlipX != (pMst->nPosX < gShoot.nPlayerPosX ? 1 : 0))				// Le h�ros est pass� de l'autre c�t� ?
				{
					pMst->nPhase = e_Mst14Rifle_HideGetUp;
					break;
				}

				if (pMst->nPosX >= gScrollPos.nPosX - 0x1000 && pMst->nPosX <= gScrollPos.nPosX + (SCR_Width << 8) + 0x1000)	// On ne tire pas si en dehors de l'�cran.
				if (ABS(pMst->nPosY - gShoot.nPlayerPosY) <= MST14RIFLE_SHOT_DY)	// Si dans la zone Y.
				{
					if (pSpe->nCntWait)
						pSpe->nCntWait--;
					else
//					if (ABS(pMst->nPosX - gShoot.nPlayerPosX) < ((MST14_ZONE_MAX+1) << 12))		// A port�e de tir ?
					if (ABS(pMst->nPosX - gShoot.nPlayerPosX) < (((u32)gpnMST14_ZoneMax[pSpe->nType] + 1) << 12))		// A port�e de tir ?
					{
						pMst->nPhase = e_Mst14Rifle_HideGetUp;
					}
				}
			}
		}
		break;

	case e_Mst14Rifle_Strike:	// Frappe avec la crosse.
		if (AnmGetKey(pMst->nAnm) == e_AnmKey_Enemy_Strike)
		{
			if (pSpe->nStrike1Time == 0)	// Pour n'avoir qu'un contact (cas du coup sur un v�hicule).
			{
				// Le coup sur le joueur.
				struct SSprRect sRect1;
				nSpr = AnmGetLastImage(pMst->nAnm) ^ (pMst->nFlipX ? SPR_Flip_X : 0);
				if (SprGetRect(nSpr, e_SprRectZone_ShotOrg, &sRect1))
				if (sRect1.nType == e_SprRect_Rect)
					pSpe->nStrike1Time = Enemy_PlayerBlowRect(&sRect1, pMst->nPosX, pMst->nPosY, MST_DAMAGE_BULLET);
			}
		}
		else
		if (--pSpe->nCntWait == 0)	// Compteur pour �viter de frapper sans arr�t.
		{
			pMst->nPhase = e_Mst14_Wait;
			pSpe->nCntWait = (rand() & 127) | 31;	// Reset attente pour le tir...
		}
		break;

	default:
		break;
	}

	// Sprite.
	nSpr = AnmGetImage(pMst->nAnm);

	// Se prend un tir ?
	if (Mst_ShotCheckHit(pMst, nSpr, gpRebSoldierDeatAnm))
	{
		if (pSpe->nParachuteAnm != -1) AnmReleaseSlot(pSpe->nParachuteAnm);		// Slot de l'anim du parachute.
		// Dust du fusil.
		gDustExg.nRotInc = SGN(pMst->nPosX - gShoot.nPlayerPosX) * -6;
		DustSetMvt(gAnm_RebSoldier_WeaponRifle_Dust, pMst->nPosX, pMst->nPosY - 0x1000, SGN(pMst->nPosX - gShoot.nPlayerPosX) * 0x100, -0x380, e_Prio_DustUnder, e_DustFlag_Gravity | e_DustFlag_Rotation);
		return (e_MstState_Dead);
	}

	// Affichage.
	SprDisplay(nSpr ^ (pMst->nFlipX ? SPR_Flip_X : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies + gnMstPrio);
	Mst14_Parachute_Display(pMst);

	return (e_MstState_Managed);

}

//== Gun Soldier.

/*
// Phases.
enum
{
	e_Mst14Gun_xxx = e_Mst14_CommonLast,	// Apr�s la course ou un tir, se baisse pour se cacher.

};
*/

void Mst14Sub_Init_GunSoldier0(struct SMstCommon *pMst, u8 *pData)
{
/*
exemple
	struct SMst14_RebelSoldier0	*pSpe = (struct SMst14_RebelSoldier0 *)pMst->pData;
	s32	nVal;
	nVal = GetBits(8, 11, pData, 1);	// Offset X.
*/
}

#define	MST14GUN_SHOT_DY		(2 << 12)
#define	MST14GRENADE_SHOT_DY	(4 << 12)

s32 Mst14Sub_Main_GunSoldier0(struct SMstCommon *pMst)
{
	struct SMst14_RebelSoldier0	*pSpe = (struct SMst14_RebelSoldier0 *)pMst->pData;
	u32	nSpr;

	switch (pSpe->nOrgPhase)	// !!! Attention � bien tester OrgPhase !!!
	{
	case e_Mst14_Move:		// D�placement.
	case e_Mst14_Wait:		// Attente.
		// Transtypage Gun/Grenade. Tire des grenades, si on est � la m�me hauteur et trop pr�s ou trop loin pour des grenades, pistolet.
		if ((ABS(pMst->nPosX - gShoot.nPlayerPosX) < (5 << 12) + (8 << 8) ||
			ABS(pMst->nPosX - gShoot.nPlayerPosX) > (9 << 12) + (0 << 8)) &&
			ABS(pMst->nPosY - gShoot.nPlayerPosY) < 2 << 12)
			pSpe->nType = 3;	// Gun.
		else
			pSpe->nType = 4;	// Grenade.
		break;

	case e_Mst14_Shot:		// Tir.
		AnmSetIfNew(gpMST14_AnmShotTb[pSpe->nType], pMst->nAnm);

		nSpr = AnmGetLastImage(pMst->nAnm);
		if (Mst_ShotLaunch(pMst, nSpr, gpMST14_ShotTb[pSpe->nType], gpMST14_AnmShotDustTb[pSpe->nType]))
		{
			// Repasse en wait apr�s le tir.
			pMst->nPhase = e_Mst14_Wait;
			pSpe->nCntWait = (rand() & 127) | 31;	// Petite attente...
			break;
		}
		break;

	default:
		break;
	}

	// Sprite.
	nSpr = AnmGetImage(pMst->nAnm);

	// Se prend un tir ?
	if (Mst_ShotCheckHit(pMst, nSpr, gpRebSoldierDeatAnm))
	{
		if (pSpe->nParachuteAnm != -1) AnmReleaseSlot(pSpe->nParachuteAnm);		// Slot de l'anim du parachute.
		return (e_MstState_Dead);
	}

	// Affichage.
	SprDisplay(nSpr ^ (pMst->nFlipX ? SPR_Flip_X : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies + gnMstPrio);
	Mst14_Parachute_Display(pMst);

	return (e_MstState_Managed);
}

//== Shield Soldier.

// Phases.
enum
{
	e_Mst14Shield_Strike = e_Mst14_CommonLast,
	e_Mst14Shield_Block,
};

void Mst14Sub_Init_ShieldSoldier0(struct SMstCommon *pMst, u8 *pData)
{
/*
exemple
	struct SMst14_RebelSoldier0	*pSpe = (struct SMst14_RebelSoldier0 *)pMst->pData;
	s32	nVal;
	nVal = GetBits(8, 11, pData, 1);	// Offset X.
*/
}

s32 Mst14Sub_Main_ShieldSoldier0(struct SMstCommon *pMst)
{
	struct SMst14_RebelSoldier0	*pSpe = (struct SMst14_RebelSoldier0 *)pMst->pData;
	u32	nSpr;
	u8	nTstPause = 0;

	switch (pSpe->nOrgPhase)	// !!! Attention � bien tester OrgPhase !!!
	{
	case e_Mst14_Shot:		// Tir. (Pour le shield, tir == attente). Normalement, cette phase n'arrive pas.
		pMst->nPhase = e_Mst14_Wait;
	case e_Mst14_Move:		// D�placement.
	case e_Mst14_Wait:		// Attente.
		pSpe->nCntWait = 2;		// Pour emp�cher le passage en Shot.

		// Test joueur au contact ou inactif.
		if (MST_SHOT_COND)	// Si le h�ros n'est pas mort...
		if (ABS(pMst->nPosY - gShoot.nPlayerPosY) <= MST14RIFLE_SHOT_DY)	// Si dans la zone Y.
		if (pMst->nFlipX == (pMst->nPosX < gShoot.nPlayerPosX ? 1 : 0))
		{
			if (ABS(pMst->nPosX - gShoot.nPlayerPosX) < 3 << 12)
			{
				// Joueur au contact ? On frappe avec une machette.
				pMst->nPhase = e_Mst14Shield_Strike;
				pSpe->nCntWait = 40;	// Petite attente pour ne pas taper sans arr�t si je joueur reste � port�e.
				AnmSet(gAnm_RebSoldier_Shield_Strike, pMst->nAnm);
			}
			else
			{
				// A l'arr�t et � distance (x et y) ? => On va compter le temps d'inactivit� du h�ros.
				if (pMst->nPhase == e_Mst14_Wait &&
					ABS(pMst->nPosY - gShoot.nPlayerPosY) <= 1 << 12 &&	// Si dans la zone Y (un peu plus petite).
					ABS(pMst->nPosX - gShoot.nPlayerPosX) < (((u32)gpnMST14_ZoneMax[3] + 1) << 12))
					nTstPause = 1;

				// Si le h�ros est devant et ne fout rien, on se transforme en gunner qu'on place directement en phase de tir.
				if (pSpe->nHeroInactivity > 300)
				{
					// Dust du bouclier.
					DustSetMvt(gAnm_RebSoldier_WeaponShield_Dust, pMst->nPosX, pMst->nPosY - 0x1000, SGN(pMst->nPosX - gShoot.nPlayerPosX) * 0x100, -0x380, e_Prio_DustUnder, e_DustFlag_Gravity); //| e_DustFlag_Rotation
					// Transformation en soldat standard.
					pSpe->nType = 3;	// Gun.
					pMst->nPhase = e_Mst14_Shot;
					pSpe->nOrgPhase = pMst->nPhase;
					return (Mst14Sub_Main_GunSoldier0(pMst));
				}
			}
		}
		break;

	case e_Mst14Shield_Strike:		// Coup de couteau.
		if (AnmGetKey(pMst->nAnm) == e_AnmKey_Enemy_Strike)
		{
			Mst_ProximityCheck(pMst);	// Donne le signal au joueur pour un �ventuel coup de couteau.

			// Le coup sur le joueur.
			struct SSprRect sRect1;
			nSpr = AnmGetLastImage(pMst->nAnm) ^ (pMst->nFlipX ? SPR_Flip_X : 0);
			if (SprGetRect(nSpr, e_SprRectZone_ShotOrg, &sRect1))
			if (sRect1.nType == e_SprRect_Rect)
				Enemy_PlayerBlowRect(&sRect1, pMst->nPosX, pMst->nPosY, MST_DAMAGE_BULLET);
		}
		else
		{
			// Le joueur a chang� de c�t� ?
			if (pMst->nFlipX != (pMst->nPosX < gShoot.nPlayerPosX ? 1 : 0) && pSpe->nCntWait > 8) pSpe->nCntWait = 8;
			// Compteur pour �viter de frapper sans arr�t.
			if (--pSpe->nCntWait == 0) pMst->nPhase = e_Mst14_Wait;
		}
		break;

	case e_Mst14Shield_Block:
		if (AnmGetKey(pMst->nAnm) != e_AnmKey_Enemy_Key3) pMst->nPhase = e_Mst14_Wait;
		break;

	default:
		break;
	}

	// Test du joueur qui glande.
	if (nTstPause && (AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_Stance ||
			//AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_CrouchStance ||		// Non, pas si accroupi (action du joueur).
			AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_Bored))
		pSpe->nHeroInactivity++;
	else
		pSpe->nHeroInactivity = 0;

	// Sprite.
	nSpr = AnmGetImage(pMst->nAnm) ^ (pMst->nFlipX ? SPR_Flip_X : 0);

	// Se prend un tir ?
	u32	nDamage;
	nDamage = FireHitCheck(nSpr, pMst->nPosX, pMst->nPosY, e_ShotOrg_Player, 1);
	nDamage |= Mst_KnifeHitCheck(pMst, nSpr);	// Comme �a, donne la priorit� de la mort au tir.
	// Protection du bouclier.
	if (nDamage)
	if ((nDamage & 0xFFFF) < 10)	// >= 10 : C'est une grenade => Meurt dans tous les cas.
	if (AnmGetKey(pMst->nAnm) == e_AnmKey_Null ||		// Clef == NULL => Le bouclier prot�ge des tirs.
		AnmGetKey(pMst->nAnm) == e_AnmKey_Enemy_Key3)	// Key3 == Anim de block.
	if (pMst->nFlipX == (pMst->nPosX < gShoot.nPlayerPosX ? 1 : 0))		// Ne prot�ge que si tourn� vers le joueur.
	{
		// Le bouclier d�gage ? (En gros, un coup de shotgun).
		if ((nDamage & 0xFFFF) >= 5)
		{
			// Dust du bouclier.
			DustSetMvt(gAnm_RebSoldier_WeaponShield_Dust, pMst->nPosX, pMst->nPosY - 0x1000, SGN(pMst->nPosX - gShoot.nPlayerPosX) * 0x100, -0x380, e_Prio_DustUnder, e_DustFlag_Gravity); //| e_DustFlag_Rotation
			// Transformation en soldat standard.
			pSpe->nType = 3;	// Gun.
			pMst->nPhase = e_Mst14_Wait;
			// Pour ne pas avoir le dust ET le bouclier sur le sprite pendant 1 frame.
			AnmSet(gpMST14_AnmWaitTb[pSpe->nType], pMst->nAnm);
			nSpr = AnmGetImage(pMst->nAnm) ^ (pMst->nFlipX ? SPR_Flip_X : 0);
		}
		else
		// Block.
		if (pMst->nGround)
		{
			AnmSet((pMst->nPhase == e_Mst14Shield_Block ? gAnm_RebSoldier_Shield_Block2 : gAnm_RebSoldier_Shield_Block), pMst->nAnm);
			pMst->nPhase = e_Mst14Shield_Block;
		}
		nDamage = 0;	// Annule les d�gats.
	}
	// Mort si besoin.
	if (nDamage)
	{
		if (pSpe->nParachuteAnm != -1) AnmReleaseSlot(pSpe->nParachuteAnm);		// Slot de l'anim du parachute.
		// Dust du bouclier.
		DustSetMvt(gAnm_RebSoldier_WeaponShield_Dust, pMst->nPosX, pMst->nPosY - 0x1000, SGN(pMst->nPosX - gShoot.nPlayerPosX) * 0x100, -0x380, e_Prio_DustUnder, e_DustFlag_Gravity); //| e_DustFlag_Rotation
		// Dust de mort.
		if (pMst->nGround == 0)
			DustSetMvt(gpRebSoldierDeatAnm[e_ShotDamageType_Blow], pMst->nPosX, pMst->nPosY, 0, -0x300, e_Prio_DustUnder, (pMst->nFlipX ? e_DustFlag_FlipX : 0) | e_DustFlag_Gravity);
		else
			DustSet(gpRebSoldierDeatAnm[nDamage >> 16], pMst->nPosX, pMst->nPosY, e_Prio_DustUnder, (pMst->nFlipX ? e_DustFlag_FlipX : 0));
		// Score.
		gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;
		return (e_MstState_Dead);
	}

	// Affichage.
	SprDisplay(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies + gnMstPrio);
	Mst14_Parachute_Display(pMst);

	return (e_MstState_Managed);
}

//== Container.

#define	MST14_NB	6
#define	MST14_SPD_DECX	0x04

// Table de pointeurs de fonctions.
struct SMstFctTb	gpMst14Subroutines[MST14_NB] =
{
	{ Mst14Sub_Init_RifleSoldier0, Mst14Sub_Main_RifleSoldier0 },
	{ Mst14Sub_Init_MortarSoldier0, Mst14Sub_Main_MortarSoldier0 },
	{ Mst14Sub_Init_LRACSoldier0, Mst14Sub_Main_LRACSoldier0 },
	{ Mst14Sub_Init_GunSoldier0, Mst14Sub_Main_GunSoldier0 },
	{ Mst14Sub_Init_GunSoldier0, Mst14Sub_Main_GunSoldier0 },
	{ Mst14Sub_Init_ShieldSoldier0, Mst14Sub_Main_ShieldSoldier0 },

};

// Init g�n�rale.
void Mst14_Init_RebelSoldier0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst14_RebelSoldier0	*pSpe = (struct SMst14_RebelSoldier0 *)pMst->pData;
	s32	nVal;
	s32 nPosY_sav = pMst->nPosY;

	nVal = GetBits(0, 7, pData, 0);		// Type.
	pSpe->nType = nVal;

	// Inits communes.
	AnmSet(gpMST14_AnmWaitTb[pSpe->nType], pMst->nAnm);
	nVal = GetBits(8, 11, pData, 1);	// Offset X.
	pMst->nPosX += nVal << 8;
	pSpe->nOrgX = pMst->nPosX >> 12;
	nVal = GetBits(17, 22, pData, 1);	// D�calage de l'origine de la zone.
	pSpe->nOrgX += nVal;
	nVal = GetBits(12, 12, pData, 0);	// Move : 1 = Oui / 0 = Fixe.
	pSpe->nMove = nVal;
	nVal = GetBits(15, 16, pData, 0);	// Jump : 0 No - 1 Random - 2 Always.
	pSpe->nJump = nVal;

	Enemy_GroundLevel(pMst);			// Mise au niveau du sol.
	pMst->nSpdY = 0;
	pMst->nFlipX = (pMst->nPosX - gShoot.nPlayerPosX >= 0 ? 0 : 1);		// Sens.
	pSpe->nCachette = 0;				// b0 = mvt impossible � gauche / b1 = mvt imposs � droite.

	pSpe->nCntWait = 30;
	pMst->nPhase = (pSpe->nMove ? e_Mst14_Move : e_Mst14_Wait);

	pSpe->nHeroInactivity = 0;

	nVal = GetBits(13, 13, pData, 0);	// From : 0 = Left / 1 = Right.
	if (nVal) pMst->nPhase = e_Mst14_FromLeftWait;
	pSpe->nParachuteAnm = -1;			// Slot de l'anim du parachute.

	nVal = GetBits(14, 14, pData, 0);	// Parachute ?
	if (nVal)
	{
		pMst->nPhase = e_Mst14_ParachuteWait;
		pSpe->nParachuteAnm = AnmSetIfNew(gAnm_Parachute_Glide, -1);		// Parachute.
	}

	// Inits sp�cifiques � chaque type de soldat.
	gpMst14Subroutines[pSpe->nType].pFctInit(pMst, pData);

	// Inits sp�ciales.
	nVal = GetBits(23, 26, pData, 1);	// Inits sp�ciales.
	switch (nVal)
	{
	case 1:		// Jump from truck (mst 15). Saut.
		pSpe->nOrgX = (pMst->nPosX >> 12) - MST14_MORTAR_AREA - 4;//3;//pSpe->nOrgX - MST14_MORTAR_AREA
	case 2:		// Jump from susp sub (mst 49). Saut. Idem sans d�calage de l'origine.
		pMst->nPhase = e_Mst14_Jump;			// Saut.
		pMst->nSpd = MST14_MORTAR_SPD + 0x80;	// Pour amortissement de la vitesse x.
		pMst->nSpdY = -0xE0;//0x280;
		pMst->nGround = 0;
		pMst->nPosY = nPosY_sav;				// Remise au y original (et pas au sol).
		break;
	case 3:		// Hero height. Le monstre n'apparaitra que si le h�ros est plus ou moins � la m�me hauteur (voir lev 6).
		if (ABS(pMst->nPosY - gShoot.nPlayerPosY) > 0x2000)
			pMst->nPosX = gScrollPos.nPosX + ((SCR_Width * 2) << 8);	// Si pas � la bonne hauteur, on d�cale le monstre pour le placer tr�s au del� de l'�cran. => Screen out.
		break;

//	case x:	 ex : sortie � genoux...
	default:
		break;
	}

}

// Gestion des phases g�n�rales.
s32 Mst14_Main_RebelSoldier0(struct SMstCommon *pMst)
{
	struct SMst14_RebelSoldier0	*pSpe = (struct SMst14_RebelSoldier0 *)pMst->pData;

	// Sortie de l'�cran ?
	if (Screen_ObjectOutRect(pMst->nPosX - (MST14_MORTAR_AREA << 12), pMst->nPosY, MST14_MORTAR_AREA*2, 1)) return (e_MstState_Asleep);
	// Couteau + vis�e du missile.
	if (pMst->nPhase > e_Mst14_ParachuteWait)
	{
		if (pSpe->nType != 5)	// Pas de coup de couteau sur le shield.
			Mst_ProximityCheck(pMst);	// Donne le signal au joueur pour un �ventuel coup de couteau.
		ChaserTarget_AddToList(pMst->nPosX >> 8, (pMst->nPosY >> 8) - 16);	// Ajout dans la table de vis�e des missiles.
	}
	// Gravit�.
	Mst14_Gravity(pMst);

	pSpe->nOrgPhase = pMst->nPhase;		// Sauvegarde la phase originale au cas ou la phase change dans les routines communes, pour appel de la phase correcte dans les traitements sp�cifiques.
	switch (pMst->nPhase)
	{
	case e_Mst14_FromLeftWait:	// Apparition depuis la gauche, on attend que le scroll arrive o� il faut.
		if (pMst->nPosX <= gScrollPos.nPosX - 0x1000)
			pMst->nPhase = (pSpe->nMove ? e_Mst14_Move : e_Mst14_Wait);
		return (e_MstState_Managed);
		break;

	case e_Mst14_ParachuteWait:		// Parachute, attente pour apparition.
		Mst14_Parachute_Wait(pMst);
		return (e_MstState_Managed);
		break;

	case e_Mst14_Parachuting:	// Parachute descente.
		AnmSetIfNew(gpMST14_AnmParachutingTb[pSpe->nType], pMst->nAnm);
		Mst14_Parachuting(pMst);
		break;

	case e_Mst14_Jump:		// Saut.
		AnmSetIfNew((pMst->nSpdY < 0 ? gpMST14_AnmJumpTb[pSpe->nType] : gpMST14_AnmFallTb[pSpe->nType]), pMst->nAnm);
//		pMst->nPosX += (pMst->nFlipX ? MST14_MORTAR_SPD : -MST14_MORTAR_SPD);	// sans amort spd x.
		pMst->nPosX += (pMst->nFlipX ? pMst->nSpd : -pMst->nSpd);
		// Amortissement de la vitesse x.
		//if (pMst->nSpd)
		{
			if (pMst->nSpd >= MST14_SPD_DECX)
				pMst->nSpd -= MST14_SPD_DECX;
			else
				pMst->nSpd = 0;
		}
		if (pMst->nGround) pMst->nPhase = e_Mst14_Move;	// !!! Attention, Move, pas Wait !!!
		break;

	case e_Mst14_Fall:		// Chute.
		AnmSetIfNew(gpMST14_AnmFallTb[pSpe->nType], pMst->nAnm);
		if (pMst->nGround) pMst->nPhase = e_Mst14_Move;	// !!! Attention, Move, pas Wait !!!
		break;

	case e_Mst14_Move:		// D�placement.
		AnmSetIfNew(gpMST14_AnmRunTb[pSpe->nType], pMst->nAnm);
		Mst14_Phase_Move(pMst);
		break;

	case e_Mst14_Wait:		// Attente.
		{
			static u32 gpShotDy[MST14_NB] = { MST14RIFLE_SHOT_DY, 0, 0, MST14GUN_SHOT_DY, MST14GRENADE_SHOT_DY, 0 };
			AnmSetIfNew(gpMST14_AnmWaitTb[pSpe->nType], pMst->nAnm);
			Mst14_Phase_Wait(pMst, gpShotDy[pSpe->nType]);
		}
		break;

	default:
		break;
	}

	return (gpMst14Subroutines[pSpe->nType].pFctMain(pMst));
}

//=============================================================================
// Mst 15 : Transport de troupes.

/*
[mst15] Truck 0 - Trooper
Wait itm = 0:7:			; N� item de qu�te attendu pour d�marrer. Si 0, attend que le joueur soit assez proche.
Give itm = 8:15:		; N� item de qu�te � donner une fois mort pour lib�rer le scroll.
*/

// Phases.
enum
{
	e_Mst15_WaitProximity = 0,
	e_Mst15_WaitItm,
	e_Mst15_Drive,
	e_Mst15_TrunkOpen,
	e_Mst15_Wait,
	e_Mst15_Explode,
	e_Mst15_Destroyed,

};

// Structure sp�cifique.
struct SMst15_Truck0
{
	u8	nItemWait;	// N� item de qu�te attendu pour d�marrer.
	u8	nItemGive;	// N� item de qu�te � donner une fois mort pour lib�rer le scroll.

	u8	nLife;
	u8	nHitCnt;

	s32	nPosXOrg;
	s8	nHardSprSlot;
	s8	nTrunkAnm;
	u8	nFrmCnt;
	u8	nNbSoldiers;
};

#define	MST15_LIFE	30//20
void Mst15_Init_Truck0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst15_Truck0	*pSpe = (struct SMst15_Truck0 *)pMst->pData;
	u32	nVal;

	nVal = GetBits(0, 7, pData, 0);		// N� item de qu�te attendu pour d�marrer.
	pSpe->nItemWait = nVal;
	nVal = GetBits(8, 15, pData, 0);	// N� item de qu�te � donner une fois mort pour lib�rer le scroll.
	pSpe->nItemGive = nVal;

	pSpe->nPosXOrg = pMst->nPosX;		// Note la position originale.

	pSpe->nLife = MST15_LIFE;
	pSpe->nHitCnt = 0;
	pSpe->nNbSoldiers = (MST15_LIFE + 1) * 2;

	Enemy_GroundLevel(pMst);			// Mise au niveau du sol.

	pMst->nPhase = e_Mst15_WaitProximity;	// Par d�faut.
	// Attente du scroll ?
	if (pSpe->nItemWait) pMst->nPhase = e_Mst15_WaitItm;
	// D�j� cass� ?
	if (pSpe->nItemGive && gpMstQuestItems[pSpe->nItemGive]) pMst->nPhase = e_Mst15_Destroyed;

	pSpe->nTrunkAnm = -1;
	pSpe->nHardSprSlot = -1;
	pSpe->nFrmCnt = 64-16;

}

// Emp�che le joueur de passer.
void Mst_PlayerBlock(s32 nPosX, u32 nSide)
{
	if (nSide)
	{	// Joueur bloqu� � droite de la position.
		if (gShoot.nPlayerPosX < nPosX) { gShoot.nPlayerPosX = nPosX; gShoot.nPlayerSpdX = 0; }
	}
	else
	{	// Joueur bloqu� � gauche de la position.
		if (gShoot.nPlayerPosX > nPosX) { gShoot.nPlayerPosX = nPosX; gShoot.nPlayerSpdX = 0; }
	}
}

//tst
extern s32	gnScrollLimitXMin;
extern s32	gnScrollLimitXMax;
extern s32	gnScrollLimitYMin;
extern s32	gnScrollLimitYMax;

u32 MstOnScreenNb(u32 nMstType, s32 nBlkOffset);

s32 Mst15_Main_Truck0(struct SMstCommon *pMst)
{
	struct SMst15_Truck0	*pSpe = (struct SMst15_Truck0 *)pMst->pData;
	u32	nSpr;
	u32	nTouch = 0;		// Par d�faut, on ne touche pas. (On arr�te simplement les tirs).

	// Sortie de l'�cran ?
	if (Screen_ObjectOut(pSpe->nPosXOrg, pMst->nPosY))
	{
		if (pSpe->nTrunkAnm != -1) AnmReleaseSlot(pSpe->nTrunkAnm);
		if (pSpe->nHardSprSlot != -1) HardSpr_ReleaseSlot(pSpe->nHardSprSlot);
		return (e_MstState_Asleep);
	}

	// Sprite.
	nSpr = AnmGetImage(pMst->nAnm);

	switch (pMst->nPhase)
	{
	case e_Mst15_WaitItm:	// Attente de l'item de qu�te pour d�marrer.
		if (gpMstQuestItems[pSpe->nItemWait])
		{
			pMst->nPhase = e_Mst15_Drive;
			pMst->nPosX = gnScrollLimitXMax + (64 << 8);
		}
		return (e_MstState_Managed);
		break;

	case e_Mst15_Drive:		// Recule pour se mettre en position (position finale = position originale).
		AnmSetIfNew(gAnm_TruckTrooper_Drive, pMst->nAnm);
		pMst->nPosX -= 0x80;
		if (pMst->nPosX <= pSpe->nPosXOrg)
		{
			pMst->nPhase = e_Mst15_TrunkOpen;	// On ouvre la benne.
		}
		break;

	case e_Mst15_WaitProximity:		// Pas d'item pr�cis�e, on attend que le joueur soit assez proche.
		if (ABS(gShoot.nPlayerPosX - pMst->nPosX) >> 12 < 10) pMst->nPhase = e_Mst15_TrunkOpen;	// On ouvre la benne.
		break;

	case e_Mst15_TrunkOpen:		// On ouvre la benne.
		AnmSetIfNew(gAnm_TruckTrooper_Idle, pMst->nAnm);
		pSpe->nTrunkAnm = AnmSetIfNew(gAnm_TruckTrooper_TrunkOpen, pSpe->nTrunkAnm);
		// La planche.
		if (pSpe->nTrunkAnm != -1)
		{
			SprDisplay(AnmGetImage(pSpe->nTrunkAnm) | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_DustUnder - 1);
			// Anim d'ouverture finie ? On passe � la suite.
			if (AnmCheckStepFlag(pSpe->nTrunkAnm))
			{
				pMst->nPhase = e_Mst15_Wait;
				if (pSpe->nItemWait)
				{
					// On g�n�re un Girida-0 depuis l'arri�re.
					/*
					[mst26] Girida-0
					From = 0:0: Front - Back	; Attaque par devant ou derri�re.
					ZoneMax = 1:7:			; Zone de d�placement max, 0 = pas de limite.
					*/
					u8	pData[4];
					s32	nGiridaPixPosX = (gScrollPos.nPosX >> 8) - (2 << 4);	// !!! Attention au d�calage, sinon le tank n'appara�t pas !
					pData[0] = 1 + (((ABS((pSpe->nPosXOrg >> 8) - nGiridaPixPosX) >> 4) - 4) << 1);		// -4 : D�calage pour ne pas monter sur le camion.
					MstAdd(e_Mst26_Girida0, nGiridaPixPosX, (pMst->nPosY >> 8) - 16, pData, -1);
				}
			}
		}
		break;

	case e_Mst15_Wait:		// Attente.
		AnmSetIfNew(gAnm_TruckTrooper_Idle, pMst->nAnm);
		nTouch = 1;		// Touch� autoris�.

		pSpe->nFrmCnt++;

		// G�n�ration des soldats.
		if (pSpe->nNbSoldiers)
		if ((pSpe->nFrmCnt & 63) == 0)
		if (pSpe->nLife <= MST15_LIFE / 2 || pSpe->nNbSoldiers > MST15_LIFE)	// La moiti� des soldats sort sur la premi�re moiti� de la vie. Le reste ensuite.
		if (MstOnScreenNb(e_Mst14_RebelSoldier0, 1) < 3)	// Pas plus de 3 � l'�cran.
		{
			/*
			[mst14] Reb Soldier 0
			Type = 0:7: Rifle - Mortar - LRAC - Gun_Grenade - RESERVED - Shield
			OffsX = -8:11:			; D�calage X.
			Move = 12:12: No - Yes		; Le soldat bouge ou pas.
			From = 13:13: Right - Left	; Apparition � droite ou � gauche de l'�cran.
			Parachute = 14:14: No - Yes	; Parachut� ?
			Jump = 15:16: No - Random - Always	; Saut en bout de pf.
			Org OffsX = -17:22:		; D�calage de l'origine de la zone.
			Special Init = 23:26: None - Truck Jump - SuspSub Jump - Hero Height -	; Inits sp�ciales.
			RESERVED2 = 27:31:
			*/
			static u8 pM14[] = { 3, 3, 0, 1 };	// Pour g�n�rer des soldats, sauf LRAC.
			//u32	nPrm = (rand() & 1) | (1 << 12) | (1 << 23);	// 0 = Rifle - 1 = Mortar / 1 << 12 = Move / 1 << 23 = truck jump.
			u32	nPrm = ((u32)pM14[rand() & 3]) | (1 << 12) | (1 << 23);	// 0 = Rifle - 1 = Mortar - 3 = Grenade / 1 << 12 = Move / 1 << 23 = truck jump.
			MstAdd(e_Mst14_RebelSoldier0, (pMst->nPosX >> 8) - 36, (pMst->nPosY >> 8) - 30, (u8 *)&nPrm, -1);
			pSpe->nNbSoldiers--;

			// 1 fois sur 4, on en fait appara�tre 1 en plus � gauche de l'�cran.
			if ((pSpe->nNbSoldiers & 3) == 0)
			{
				nPrm = ((u32)pM14[rand() & 3]) | (1 << 12) | (16 << 17);	// 0 = Rifle - 1 = Mortar - 3 = Grenade / 1 << 12 = Move / 16 << 17, d�calage d'origine de la zone.
				MstAdd(e_Mst14_RebelSoldier0, (gScrollPos.nPosX >> 8) - 24, (pMst->nPosY >> 8), (u8 *)&nPrm, -1);
			}

		}

		// Affichage de la b�che cache.
		SprDisplay(e_Spr_Truck0_TrunkOver | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies + 32);
		// Affichage de la planche.
		if (pSpe->nTrunkAnm != -1)
			SprDisplay(AnmGetImage(pSpe->nTrunkAnm) | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_DustUnder - 1);
		break;

	case e_Mst15_Destroyed:		// D�truit.
		// Anim � jouer en plus sur le hard sprite.
		AnmSetIfNew(gAnm_TruckTrooper_DestroyedAnm, pMst->nAnm);
		// Hard sprite : Sauvegarde du slot.
		if (pSpe->nHardSprSlot == -1)
			pSpe->nHardSprSlot = HardSpr_AddToList(e_Spr_Truck0_Destroyed_Pf, pMst->nPosX >> 8, pMst->nPosY >> 8, 0);

		// Affichage du camion cass� + anim par dessus.
		SprDisplay(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_DustUnder - 1);//e_Prio_Ennemies - 3);//1);
		SprDisplay(e_Spr_Truck0_Destroyed, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_DustUnder - 1);//e_Prio_Ennemies - 3);//1);

		return (e_MstState_Managed);
		//break;
	}

	// Affichage (d�cal� ici � cause du nHitCnt qui ne doit pas �tre mis � jour entre les SprDisplay).
	SprDisplay(nSpr | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_DustUnder - 2);//e_Prio_Ennemies - 3);//1);// + gnMstPrio);
	if (pSpe->nHitCnt) pSpe->nHitCnt--;

/*
{
	// vie.
	char	pTb[8+1] = "00000000";
	MyItoA(pSpe->nLife, pTb);
	Font_Print(10, 60, pTb, 0);
}
*/

	// Ajout dans la table de vis�e des missiles.
	ChaserTarget_AddToList(pMst->nPosX >> 8, (pMst->nPosY >> 8) - 24);
	// Bloque le joueur.
	s32	nPlayerSpdX_sav = gShoot.nPlayerSpdX;
	Mst_PlayerBlock(pMst->nPosX - (4 << 12) - (8 << 8), 0);
	if (nPlayerSpdX_sav < 0) gShoot.nPlayerSpdX = nPlayerSpdX_sav;	// Si vitesse n�gative, on la remet (�vite le "coll�").
	// Se prend un tir ?
	if (nTouch)
	{
//		if (Mst_ShotCheckLife(nSpr, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife))
		if (Mst_ShotCheckLife2(nSpr, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife, e_ShotCheck_MinimizedDamage))
		{
			gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;	// Score.
			DustSet(gAnm_Explosion0_Big_Dust, pMst->nPosX, pMst->nPosY, e_Prio_DustOver, 0);
//			if (pSpe->nTrunkAnm != -1) { AnmReleaseSlot(pSpe->nTrunkAnm); pSpe->nTrunkAnm = -1; }
			pMst->nPhase = e_Mst15_Destroyed;		// D�truit.
			if (pSpe->nItemGive) gpMstQuestItems[pSpe->nItemGive] = 1;	// Donne l'item pour lib�rer le scroll.
		}
	}
	else
	{
		// Interception des tirs sans d�gats.
		u8	nHitCnt, nLife;
		nLife = 0;
		Mst_ShotCheckLife(nSpr, pMst->nPosX, pMst->nPosY, &nHitCnt, &nLife);
	}

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 16 : Update blocs du d�cor (sans anims, mais plusieurs �tapes possibles).

/*
[mst16] Bkg modif2		; Monstre � placer � l'endroit de la copie (bloc haut gauche).
Wait itm = 0:7:			; N� item de qu�te attendu pour d�marrer.
Modif Nb = 8:11:		; Nb de modifs. Chaque modif commencera � l'item de qu�te suivant.
Blk org = 16:31:		; N� du premier bloc en haut � gauche de la zone � copier.
Blk lg = 32:39:			; Lg de la zone � copier.
Blk ht = 40:47:			; Ht de la zone � copier.
Offset = 48:63:			; Offset � ajouter � chaque tour.
*/

// Structure sp�cifique.
struct SMst16_BlkBkg2
{
	u16	nBlkOrg;		// N� du permier bloc en haut � gauche de la zone � copier.
	u16	nOffset;		// Offset.
	u8	nItemWait;		// N� item de qu�te attendu.
	u8	nWaitVal;		// Valeur attendue.
	u8	nBlkLg;			// Lg de la zone � copier.
	u8	nBlkHt;			// Ht de la zone � copier.
	u8	nStatesNb;		// Nb d'�tats.
};

void Mst16_Init_BlkBkg2(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst16_BlkBkg2	*pSpe = (struct SMst16_BlkBkg2 *)pMst->pData;
	u32	nVal;

	nVal = GetBits(0, 7, pData, 0);
	pSpe->nItemWait = nVal;
	pSpe->nWaitVal = 1;
	nVal = GetBits(8, 11, pData, 0);
	if (nVal == 0) nVal = 1;
	pSpe->nStatesNb = nVal;
	nVal = GetBits(16, 31, pData, 0);
	pSpe->nBlkOrg = nVal;
	nVal = GetBits(32, 39, pData, 0);
	pSpe->nBlkLg = nVal;
	nVal = GetBits(40, 47, pData, 0);
	pSpe->nBlkHt = nVal;
	nVal = GetBits(48, 63, pData, 0);
	pSpe->nOffset = nVal;

	// Cas des asleep sur monstres avec multiples �tats possibles, r�init des valeurs qui vont bien.
	if (gpMstQuestItems[pSpe->nItemWait] && pSpe->nStatesNb > 1)
	{
		pSpe->nWaitVal = gpMstQuestItems[pSpe->nItemWait] + 1;
		pSpe->nBlkOrg += pSpe->nOffset * gpMstQuestItems[pSpe->nItemWait];
		pSpe->nStatesNb -= gpMstQuestItems[pSpe->nItemWait];
		if ((s8)pSpe->nStatesNb < 0) pSpe->nStatesNb = 0;
	}

}

s32 Mst16_Main_BlkBkg2(struct SMstCommon *pMst)
{
	struct SMst16_BlkBkg2	*pSpe = (struct SMst16_BlkBkg2 *)pMst->pData;

	// Mort ?
	if (pSpe->nStatesNb == 0) return (e_MstState_Dead);		// Ici en cas de reinit et que nStatesNb == 0 (si merdouille dans le level design).
	// Sortie de l'�cran ?
	if (Screen_ObjectOutRect(pMst->nPosX, pMst->nPosY, pSpe->nBlkLg, pSpe->nBlkHt)) return (e_MstState_Asleep);

/*
//>> tst / affichage du rect
{
	SprDisplay(e_Spr_Tstrct_CornerUL, (pMst->nPosX & ~0xFFF) >> 8, (pMst->nPosY & ~0xFFF) >> 8, e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerUR, ((pMst->nPosX & ~0xFFF) + (pSpe->nBlkLg << 12)) >> 8, (pMst->nPosY & ~0xFFF) >> 8, e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerDL, (pMst->nPosX & ~0xFFF) >> 8, ((pMst->nPosY & ~0xFFF) + (pSpe->nBlkHt << 12)) >> 8, e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerDR, ((pMst->nPosX & ~0xFFF) + (pSpe->nBlkLg << 12)) >> 8, ((pMst->nPosY & ~0xFFF) + (pSpe->nBlkHt << 12)) >> 8, e_Prio_Joueur + 3);
}
//<< tst / affichage du rect
*/

	// Item de qu�te attendu ?
	if (gpMstQuestItems[pSpe->nItemWait] == pSpe->nWaitVal)
	{
		BlkBkg(gMap.nHeroPlane, pMst->nPosX >> 12, pMst->nPosY >> 12, pSpe->nBlkLg, pSpe->nBlkHt, pSpe->nBlkOrg);
		pSpe->nStatesNb--;
		pSpe->nBlkOrg += pSpe->nOffset;
		pSpe->nWaitVal++;
	}

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 17 : Lance des explosions dans un carr�.

// Phases.
enum
{
	e_Mst17_ItemWait = 0,
	e_Mst17_Explode,
};

// Lance des explosions dans un carr�.
/*
[mst17] Explosions		; Monstre � placer sur le bloc haut gauche du carr�.
Wait itm = 0:7:			; N� item de qu�te attendu pour d�marrer.
Width = 8:15:			; Largeur du carr� en blocs.
Height = 16:23:			; Hauteur du carr� en blocs.
Frames = 24:31:			; Nb de frames.
Debris = 32:35: None - Wood - Metal
Prio = 36:36: Under - Over	; Priorit� d'affichage (toujours dessous, sauf boss 1).
RESERVED = 37:39:		;Pour plus tard (flag pour g�n�rer des explosions qui sont des tirs ?).
*/

// Structure sp�cifique.
struct SMst17_Explosions0
{
	u8	nItemWait;		// N� item de qu�te attendu pour d�marrer.
	u8	nBlkLg;			// Lg de la zone � copier.
	u8	nBlkHt;			// Ht de la zone � copier.
	u8	nFrames;		// Cnt.
	u8	nDebris;		// Type de d�bris.
	u8	nPrio;			// Priorit� d'affichage (toujours dessous, sauf boss 1).
};

void Mst17_Init_Explosions0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst17_Explosions0	*pSpe = (struct SMst17_Explosions0 *)pMst->pData;
	u32	nVal;

	nVal = GetBits(0, 7, pData, 0);
	pSpe->nItemWait = nVal;
	nVal = GetBits(8, 15, pData, 0);
	pSpe->nBlkLg = nVal;
	nVal = GetBits(16, 23, pData, 0);
	pSpe->nBlkHt = nVal;
	nVal = GetBits(24, 31, pData, 0);
	pSpe->nFrames = nVal << 1;
	nVal = GetBits(32, 35, pData, 0);
	pSpe->nDebris = nVal;
	nVal = GetBits(36, 36, pData, 0);
	pSpe->nPrio = (nVal ? e_Prio_DustOver : e_Prio_DustUnder);

	pMst->nPhase = e_Mst17_ItemWait;
}

s32 Mst17_Main_Explosions0(struct SMstCommon *pMst)
{
	struct SMst17_Explosions0	*pSpe = (struct SMst17_Explosions0 *)pMst->pData;

	switch (pMst->nPhase)
	{
	case e_Mst17_ItemWait:		// Attente de l'item qui va bien.
		// Sortie de l'�cran ?
		if (Screen_ObjectOutRect(pMst->nPosX, pMst->nPosY, pSpe->nBlkLg, pSpe->nBlkHt)) return (e_MstState_Asleep);
		// Item de qu�te ?
		if (gpMstQuestItems[pSpe->nItemWait] == 0) break;
		pMst->nPhase = e_Mst17_Explode;
		Sfx_PlaySfx(e_Sfx_Fx_Explosion2, e_SfxPrio_Explosion);
		//break;		// Pas de break.

	case e_Mst17_Explode:		// Explosions.
		if (pSpe->nFrames & 1)	// 1 frame sur 2.
		{
			s32	nPosX, nPosY;
			nPosX = pMst->nPosX + ((rand() % (pSpe->nBlkLg << 4)) << 8);
			nPosY = pMst->nPosY + ((rand() % (pSpe->nBlkHt << 4)) << 8);

			// Explosion.
			DustSet(gAnm_Explosion0_Medium_NoSfx_Dust, nPosX, nPosY, pSpe->nPrio, 0);
			// D�bris.
			if (pSpe->nDebris)
			if ((pSpe->nFrames & 7) == 7)	// 1 frame sur 8.
			{
				static	u64	*gpDebrisTb[] = { gAnm_Debris_Wood0_Dust, gAnm_Debris_Metal0_Dust };
				DustSetMvt(gpDebrisTb[pSpe->nDebris - 1], nPosX, nPosY, 0, 0, e_Prio_DustUnder - 1, e_DustFlag_Gravity);
			}
		}

		if (--pSpe->nFrames == 0) return (e_MstState_Dead);
		break;
	}

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 18 : Rectangle qui teste si des tirs arrivent (plusieurs �tapes possibles (??? pas fait apparement)).

/*
// Phases.
enum
{
	e_Mst16_ItemWait = 0,
	e_Mst8_PhaseAnim,

};
*/

// Laisse passer les tirs (ou pas ?), mais se prend des d�gats.
/*
[mst18] Bkg shots check		; Monstre � placer sur le bloc haut gauche du carr�.
Width = 0:7:			; Largeur du carr� en blocs.
Height = 8:15:			; Hauteur du carr� en blocs.
Energy = 16:23:			; Energie.
Nb(1) = 24:31:			; Nb d'�tats possibles.
Give itm = 32:39:		; 1er item donn�. +1 � chaque fois.
Offset x = 40:43:		; D�calage x dans le bloc.
Offset y = 44:47:		; D�calage y dans le bloc.
*/

// Structure sp�cifique.
struct SMst18_RectShotsCheck0
{
/*
	u8	nUpdateBkg;
	u16	nBlkOrg;		// N� du premier bloc en haut � gauche de la zone � copier.
*/
	u8	nItmGive;		// N� item de qu�te attendu pour d�marrer.
	u8	nBlkLg;			// Lg de la zone � copier.
	u8	nBlkHt;			// Ht de la zone � copier.
	u8	nLife;
};

/*
[mst18] Bkg shots check		; Monstre � placer sur le bloc haut gauche du carr�.
-Width = 0:7:			; Largeur du carr� en blocs.
-Height = 8:15:			; Hauteur du carr� en blocs.
-Energy = 16:23:			; Energie.
Nb(1) = 24:31:			; Nb d'�tats possibles.
-Give itm = 32:39:		; 1er item donn�. +1 � chaque fois.
-Offset x = 40:43:		; D�calage x dans le bloc.
-Offset y = 44:47:		; D�calage y dans le bloc.
*/

void Mst18_Init_RectShotsCheck0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst18_RectShotsCheck0	*pSpe = (struct SMst18_RectShotsCheck0 *)pMst->pData;
	u32	nVal;

	nVal = GetBits(0, 7, pData, 0);		// Width.
	pSpe->nBlkLg = nVal;
	nVal = GetBits(8, 15, pData, 0);	// Height.
	pSpe->nBlkHt = nVal;

	nVal = GetBits(16, 23, pData, 0);	// Energy.
	pSpe->nLife = nVal;

	nVal = GetBits(32, 39, pData, 0);
	pSpe->nItmGive = nVal;

	pMst->nPosX &= ~0xFFF;
	pMst->nPosY &= ~0xFFF;
	nVal = GetBits(40, 43, pData, 0);	// Offset X.
	pMst->nPosX += nVal << 8;
	nVal = GetBits(44, 47, pData, 0);	// Offset Y.
	pMst->nPosY += nVal << 8;

}

s32 Mst18_Main_RectShotsCheck0(struct SMstCommon *pMst)
{
	struct SMst18_RectShotsCheck0	*pSpe = (struct SMst18_RectShotsCheck0 *)pMst->pData;

	// Sortie de l'�cran ?
	if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);

	// Se prend un tir ?
	u32	nDamage;
	struct SSprRect sRect1;
	// On cr�� un rectangle de collision correspondant � la taille du monstre.
	sRect1.nType = e_SprRect_Rect;
	sRect1.nX1 = 0;
	sRect1.nX2 = (pSpe->nBlkLg << 4) - 1;
	sRect1.nY1 = 0;
	sRect1.nY2 = (pSpe->nBlkHt << 4) - 1;
	// Test...
	nDamage = FireHitCheckRect(&sRect1, pMst->nPosX, pMst->nPosY, e_ShotOrg_Player, 1);
	if (nDamage)
	{
// en cours... voir si on fait le coup du nombre d'�tats.
		nDamage &= 0xFFFF;
		if (nDamage > pSpe->nLife) nDamage = pSpe->nLife;
		pSpe->nLife -= nDamage;

		if (pSpe->nLife == 0)
		{
			gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;	// Score.
			//
			gpMstQuestItems[pSpe->nItmGive] = 1;
			return (e_MstState_Dead);
		}
	}


/*
//> debug
char	pScore[8+1] = "00000000";
MyItoA(pSpe->nLife, pScore);
Font_Print(10, 64, pScore, 0);
//< debug

{
	SprDisplay(e_Spr_Tstrct_CornerUL, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerUR, (pMst->nPosX + (pSpe->nBlkLg << 12)) >> 8, pMst->nPosY >> 8, e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerDL, pMst->nPosX >> 8, (pMst->nPosY + (pSpe->nBlkHt << 12)) >> 8, e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerDR, (pMst->nPosX + (pSpe->nBlkLg << 12)) >> 8, (pMst->nPosY + (pSpe->nBlkHt << 12)) >> 8, e_Prio_Joueur + 3);
}
*/


	return (e_MstState_Managed);
}

//=============================================================================
// Mst 19 : Foreground Sprite avec 2 �tats.

/*
[mst19] FG sprite 2 states
Gfx = 0:7: L5_Sub_Fence - L13_PF1L_Swamp - L13_PF1R_Swamp
Wait itm = 8:15:		; N� item de qu�te attendu pour 2�me �tat.
*/

// Structure sp�cifique.
struct SMst19_FgSpr2States0
{
	u8	nItemWait;		// N� item de qu�te attendu.
	u8	nFGPlane;		// 1 = Affichage devant le plan d'avant plan / 0 = Affichage normal.

};

void Mst19_Init_FgSpr2States0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst19_FgSpr2States0	*pSpe = (struct SMst19_FgSpr2States0 *)pMst->pData;
	u32	nVal;
	static	u32 pMst19_SprTb[] = { e_Spr_Lev5_Fence_fg, e_Spr_Lev13_SwampPF1L_fg, e_Spr_Lev13_SwampPF1R_fg, };
	static	u8	pMst19_PrioTb[] = { 0, 1, 1, };

	nVal = GetBits(0, 7, pData, 0);
	pMst->nAnm = pMst19_SprTb[nVal];
	pSpe->nFGPlane = pMst19_PrioTb[nVal];
	nVal = GetBits(8, 15, pData, 0);
	pSpe->nItemWait = nVal;

}

s32 Mst19_Main_FgSpr2States0(struct SMstCommon *pMst)
{
	struct SMst19_FgSpr2States0	*pSpe = (struct SMst19_FgSpr2States0 *)pMst->pData;

	// Sortie de l'�cran ?
	if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) { pMst->nAnm = -1; return (e_MstState_Asleep); }
	// Affichage.
	SprDisplay((u32)pMst->nAnm + (gpMstQuestItems[pSpe->nItemWait] ? 1 : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, (pSpe->nFGPlane ? 0x100 + 10 : e_Prio_HUD-1));
	// Note : Si on veut plus que 2 �tats, on peut ajouter directement la valeur contenue dans la table au lieu de 0 ou 1.

	return (e_MstState_Managed);
}

//=============================================================================

/*
struct SAAB { u8	nTb0[80]; };
assert(sizeof(struct SAAB) < MST_COMMON_DATA_SZ);
*/

#ifndef NDEBUG
// Debug, v�rification de la taille des structures.
void Mst10CheckStructSizes(void)
{
	// Mst10: Pas de struct sp�cifique.
	assert(sizeof(struct SMst11_ObjItemCtc) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst12_HeroBlocker) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst13_NextLevel) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst14_RebelSoldier0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst15_Truck0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst16_BlkBkg2) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst17_Explosions0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst18_RectShotsCheck0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst19_FgSpr2States0) < MST_COMMON_DATA_SZ);
}
#endif

