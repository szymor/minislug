
#include "includes.h"
#include "monsters_inc.h"

//=============================================================================
// Mst 30 : Générateur de squids.

/*
[mst30] Squid Generator
Zone W = 0:7:			; Largeur de la zone dans laquelle générer les squids.
Zone H = 8:15:			; Hauteur de la zone dans laquelle générer les squids.
Type = 16:17: Squids - Piranhas - Jellyfishes
prm = 18:23:
*/

// Phases.
enum
{
	e_Mst30_WaitX = 0,
	e_Mst30_Generate,
};

// Structure spécifique.
struct SMst30_SquidGenerator0
{
	u8	nZoneW;			// Largeur de la zone dans laquelle générer les squids.
	u8	nZoneH;			// Hauteur de la zone dans laquelle générer les squids.
	u8	nCnt0;
	u8	nType;			// Type de monstres. 0 = Squids / 1 = Méduses.

	u16	nMstNb;			// Nb de monstres à générer.
	u8	nDist;
};

#define	MST30_TIMER0	32

void Mst30_Init_SquidGenerator0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst30_SquidGenerator0	*pSpe = (struct SMst30_SquidGenerator0 *)pMst->pData;
	s32	nVal;

	nVal = GetBits(0, 7, pData, 0);			// Largeur de la zone dans laquelle générer les squids.
	pSpe->nZoneW = nVal;
	pSpe->nMstNb = 16;						// Nb de monstres à générer.
	pSpe->nDist = 0;
	nVal = GetBits(8, 15, pData, 0);		// Hauteur de la zone dans laquelle générer les squids.
	pSpe->nZoneH = nVal;
	nVal = GetBits(16, 17, pData, 0);		// Type de monstres à générer.
	pSpe->nType = nVal;

	pMst->nPosX &= ~0xFFF;	// Replacement dans le bloc.
	pMst->nPosY &= ~0xFFF;

	pMst->nPhase = e_Mst30_WaitX;

}

s32 Mst30_Main_SquidGenerator0(struct SMstCommon *pMst)
{
	struct SMst30_SquidGenerator0	*pSpe = (struct SMst30_SquidGenerator0 *)pMst->pData;

/*
SprDisplay(e_Spr_Tstrct_CornerUL, ((pMst->nPosX & ~0xFFF) >> 8), ((pMst->nPosY & ~0xFFF) >> 8), 0x100);
SprDisplay(e_Spr_Tstrct_CornerDL, ((pMst->nPosX & ~0xFFF) >> 8), ((pMst->nPosY & ~0xFFF) >> 8) + ((u32)pSpe->nZoneH * 16) - 1, 0x100);
SprDisplay(e_Spr_Tstrct_CornerUR, ((pMst->nPosX & ~0xFFF) >> 8) + ((u32)pSpe->nZoneW * 16) - 1, ((pMst->nPosY & ~0xFFF) >> 8), 0x100);
SprDisplay(e_Spr_Tstrct_CornerDR, ((pMst->nPosX & ~0xFFF) >> 8) + ((u32)pSpe->nZoneW * 16) - 1, ((pMst->nPosY & ~0xFFF) >> 8) + ((u32)pSpe->nZoneH * 16) - 1, 0x100);
*/

	switch (pMst->nPhase)
	{
	case e_Mst30_WaitX:			// Attend que le début de zone (le monstre) entre dans l'écran.
		if (pMst->nPosX < gScrollPos.nPosX)
		{
			pMst->nPhase = e_Mst30_Generate;
			pSpe->nCnt0 = 1;
		}
		break;

	case e_Mst30_Generate:		// On génère des squids.
/*
{
//>tst
char	pTb[8+1] = "00000000";
MyItoA(pSpe->nDist, pTb);
Font_Print(10, 160, pTb, 0);
memset(&pTb[0], '0', 8);
MyItoA(pSpe->nMstNb, pTb);
Font_Print(10, 170, pTb, 0);
//<tst
}
*/
		// Au fur et a mesure qu'on avance, on augmente le nombre de monstres à générer.
		if (gScrollPos.nPosX >> 12 > (pMst->nPosX >> 12) + (u32)pSpe->nDist)
		{
			pSpe->nDist += 4;		// Par pas de 4 blocs.
			pSpe->nMstNb += 12;
		}

		if (pSpe->nMstNb)
		if (--pSpe->nCnt0 == 0)
		{
			// Add monstre.
			u8	pData[4];
			if (pSpe->nType <= 1)
			{
/*
[mst31] Squid
Type = 0:3: Squid - Piranha
prm = 4:7:			; tmp.
*/
				pData[0] = pSpe->nType;
				MstAdd(e_Mst31_Squid0, (gScrollPos.nPosX >> 8) + SCR_Width + 64, (pMst->nPosY >> 8) + (rand() % ((u32)pSpe->nZoneH * 16)), pData, -1);
			}
			else
			{
/*
[mst10] Jellyfish
Type = 0:3: Small - 	// << mais inutilisé.
*/
//pData[0] = xxx;	// Rien pour le moment.
				MstAdd(e_Mst10_Jellyfish, (gScrollPos.nPosX >> 8) + (rand() % (SCR_Width + (SCR_Width/2))), (gScrollPos.nPosY >> 8) + SCR_Height + 16, pData, -1);
			}

			// Reinit compteur.
			pSpe->nCnt0 = MST30_TIMER0;
			pSpe->nMstNb--;
		}
		// La zone du générateur sort de l'écran ? (Uniquement par la gauche).
		if (pMst->nPosX + (((u32)pSpe->nZoneW * 16) << 8) < gScrollPos.nPosX) return (e_MstState_Dead);
		break;
	}

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 31 : Squid.

/*
[mst31] Squid
Type = 0:3: Squid - Piranha
prm = 4:7:			; tmp.
*/

// Structure spécifique.
struct SMst31_Squid0
{
	u8	nType;		// 0 = Squid / 1 = Piranha.

	u8	nLife;		// Pts de vie.
	u8	nHitCnt;	// Hit counter.
};

#define	MST31_MAX	2
s8	gpnM31Spd[MST31_MAX] = { 0x40, 0x30, };
s8	gpnM31Dec[MST31_MAX] = { 0x10, 0x0C, };
u8	gpnM31Life[MST31_MAX] = { 5, 3, };
s8	gpnM31AngAdd[MST31_MAX] = { 0, 10, };
u64	*gpM31AnmSwim[MST31_MAX] = { gAnm_Squid_Swim, gAnm_Piranha_Swim, };
u64	*gpM31AnmDeath[MST31_MAX] = { gAnm_Squid_Death_Dust, gAnm_Piranha_Death_Dust, };

void Mst31_Init_Squid0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst31_Squid0	*pSpe = (struct SMst31_Squid0 *)pMst->pData;

	pSpe->nType = GetBits(0, 3, pData, 0);	// Type.

	pSpe->nLife = gpnM31Life[pSpe->nType];
	pSpe->nHitCnt = 0;	// Hit counter.

	pMst->nSpd = (s32)gpnM31Spd[pSpe->nType] << 4;
	pMst->nSpdY = (s32)gpnM31Dec[pSpe->nType];	// = Dec !!!
	pMst->nAngle = 128 + gpnM31AngAdd[pSpe->nType];

	AnmSet(gpM31AnmSwim[pSpe->nType], pMst->nAnm);		// Anm swim.

}

s32 Mst31_Main_Squid0(struct SMstCommon *pMst)
{
	struct SMst31_Squid0	*pSpe = (struct SMst31_Squid0 *)pMst->pData;

	u32	nSpr;
	nSpr = AnmGetImage(pMst->nAnm);

	// Déplacement.
	pMst->nPosX += ((s32)gVar.pCos[pMst->nAngle] * pMst->nSpd) >> 8;
	pMst->nPosY += ((s32)gVar.pSin[pMst->nAngle] * pMst->nSpd) >> 8;
	//
	if (pMst->nSpd - pMst->nSpdY >= 0) pMst->nSpd -= pMst->nSpdY;	// pMst->nSpdY = Dec !!!
	if (AnmCheckStepFlag(pMst->nAnm))
	{
		pMst->nSpd = (s32)gpnM31Spd[pSpe->nType] << 4;
		pMst->nAngle = -pMst->nAngle;
	}

	// Sortie de l'écran ?
	if (pMst->nPosX + (64 << 8) < gScrollPos.nPosX) return (e_MstState_Dead);

	// Se prend un tir ?
	if (Mst_ShotCheckLife(nSpr, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife))
	{
_31SquidDeath:
		gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;	// Score.
		DustSet(gpM31AnmDeath[pSpe->nType], pMst->nPosX, pMst->nPosY, e_Prio_DustUnder, 0);	// Dust.
		return (e_MstState_Dead);	// Mort.
	}
	// Contact avec le joueur ?
	struct SSprRect sRect1;
	if (SprGetRect(nSpr, e_SprRectZone_RectCol, &sRect1))
	if (sRect1.nType == e_SprRect_Rect)
	if (Enemy_PlayerBlowRect(&sRect1, pMst->nPosX, pMst->nPosY, MST_DAMAGE_EXPLO)) goto _31SquidDeath;

	ChaserTarget_AddToList(pMst->nPosX >> 8, (pMst->nPosY >> 8));	// Ajout dans la table de visée des missiles.

	// Affichage.
	if (pSpe->nHitCnt) pSpe->nHitCnt--;
	SprDisplay(nSpr | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies + gnMstPrio);

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 32 : L11 Scroll control : Arrête/redémarre le scroll sur le plan des monstres en fct d'une item.

// Phases.
enum
{
	e_Mst32_Move = 0,
	e_Mst32_Stop,
};

// Structure spécifique.
struct SMst32_ScrollCtrl
{
	s32	nSpdSav;	// Sauvegarde de la vitesse du scroll.
};

void Mst32_Init_L11ScrollCtrl(struct SMstCommon *pMst, u8 *pData)
{
//	struct SMst32_ScrollCtrl	*pSpe = (struct SMst32_ScrollCtrl *)pMst->pData;

	pMst->nPhase = e_Mst32_Move;

}

s32 Mst32_Main_L11ScrollCtrl(struct SMstCommon *pMst)
{
	struct SMst32_ScrollCtrl	*pSpe = (struct SMst32_ScrollCtrl *)pMst->pData;

//if (gVar.pKeys[SDLK_j]) { gpMstQuestItems[MST_QUEST_ITEM_MST32] ^= 1; gVar.pKeys[SDLK_j] = 0; }	// tst

	switch (pMst->nPhase)
	{
	case e_Mst32_Move:
		if (gpMstQuestItems[MST_QUEST_ITEM_MST32])
		{	// Stop.
			pMst->nPhase = e_Mst32_Stop;
			pSpe->nSpdSav = gScrollPos.nL11SpdY;
			gScrollPos.nL11SpdY = 0;
		}
		break;

	case e_Mst32_Stop:
		if (gpMstQuestItems[MST_QUEST_ITEM_MST32] == 0)
		{	// Redémarre.
			pMst->nPhase = e_Mst32_Move;
			gScrollPos.nL11SpdY = pSpe->nSpdSav;
		}
		break;
	}

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 33 : L11 Generator.

void Scroll_HeroPlaneScrollPosXY_Get(s32 *pnPosX, s32 *pnPosY);

/*
[mst33] L11 Generator
Type = 0:3: MarsEyeTIE - MarsEye2 - Asteroids - Big_Asteroids - MarsUFO - L02_FlyingTara - L11_EndOfLevel -
Drop = 4:7: Nothing - H_Machinegun - Gas_Tank -
Nb(1) = 8:11:
Offset = 12:15:			; Décalage Y pour ceux qui viennent des côtés.
Scroll Stop = 16:17: No - While_gen - After_last_disp	; Stoppe le scroll pendant cette séquence ? (Non - Pendant la génération - Jusqu'à disparition du dernier monstre).
prm0 = 18:19:			; tmp
Shot Pattern = 20:23: One_By_One - All_Together	; Type de tir : Un par un - Tous ensemble.
Shot 1st Delay = 24:27:		; Délai avant le premier tir (* 16 frames).
Shot Delay = 28:31:		; Délai entre les tirs (* 16 frames).
Shot Nb = 32:35:		; Nb de tirs.
Scroll type = 36:36: Y - X	; Scroll V ou H.
prm1 = 37:39:			; tmp
*/

// Phases.
enum
{
	e_Mst33_Wait = 0,
	e_Mst33_WaitLastMstDisp,
};

#define	MST33_TIMER0	24
#define	MST33_TIMER_FTARA0	(24+32)

// Structure spécifique.
struct SMst33_L11Generator0
{
	u8	nType;			// Type des monstres.
	u8	nDrop;			// Item lachée par le dernier monstre de la séquence.
	u8	nNb;			// Nb de monstres à générer.
	u8	nOffset;		// Décalage Y pour ceux qui viennent des côtés.
	u8	nScrollStop;	// Stoppe le scroll pendant cette séquence.
	u8	nSeqNo;			// N° de la séquence.
	u8	nOrder;			// Incrément pour compter les monstres dans une séquence.
	u8	nCnt0;

	u16	sav_nShot;		// Sauvegarde des prm de tir.
	u8	sav_Bits0;
};

void Mst33_Init_L11Generator0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst33_L11Generator0	*pSpe = (struct SMst33_L11Generator0 *)pMst->pData;
	s32	nVal;

	nVal = GetBits(0, 3, pData, 0);
	pSpe->nType = nVal;
	nVal = GetBits(4, 7, pData, 0);
	pSpe->nDrop = nVal;
	nVal = GetBits(8, 11, pData, 0);
	if (nVal < 1) nVal = 1;
	if (pSpe->nType == 2) nVal *= 16;		// Multiplicateur pour les astéroïdes.
	else if (pSpe->nType == 4) nVal = 3;	// Chiffre fixe pour les Mars UFO.
	pSpe->nNb = nVal;
	nVal = GetBits(12, 15, pData, 0);
	pSpe->nOffset = nVal;
	nVal = GetBits(16, 17, pData, 0);
	pSpe->nScrollStop = nVal;
	if (pSpe->nScrollStop) gpMstQuestItems[MST_QUEST_ITEM_MST32]++;		// Si scroll stop, qque soit le type, stop.

	pSpe->nSeqNo = SeqInit(pSpe->nNb);
	pSpe->nOrder = 0;

	pSpe->nCnt0 = 1;
	pMst->nPhase = e_Mst33_Wait;

	// Sauvegarde des prm de tir.
	nVal = GetBits(20, 35, pData, 0);
	pSpe->sav_nShot = nVal;

	nVal = GetBits(36, 36, pData, 0);
	pSpe->sav_Bits0 = nVal;

}

s32 Mst33_Main_L11Generator0(struct SMstCommon *pMst)
{
	struct SMst33_L11Generator0	*pSpe = (struct SMst33_L11Generator0 *)pMst->pData;
	u8	pData[5];
	s32	nPosX, nPosY;

	// Cas de la disparition après la dernière disparition de monstre.
	if (pMst->nPhase == e_Mst33_WaitLastMstDisp)
	{
		if (gMstMisc.pSeqCount2[pSpe->nSeqNo] == 0)
		{
			gpMstQuestItems[MST_QUEST_ITEM_MST32]--;		// -- sur l'item de stop.
			return (e_MstState_Dead);
		}
		return (e_MstState_Managed);
	}

	// Cas normal, génération.
	if (--pSpe->nCnt0 == 0)
	{
		switch (pSpe->nType)
		{
		case 0:		// MarsEyeTIE
		case 1:		// MarsEye2
		case 4:		// MarsUFO
			// Add monstre.
/*
[mst34] L11 Mars Eye
Type = 0:3: MarsEyeTIE - MarsEye2 -
Drop = 4:7: Nothing - H_Machinegun - Gas_Tank -
SeqNo = 8:11:	; No de la séquence.
Offset = 12:15:	; Offset.
Order = 16:19:			; N° d'ordre dans la séquence (pour trouver le leader).
Shot Pattern = 20:23: One_By_One - All_Together	; Type de tir : Un par un - Tous ensemble.
Shot 1st Delay = 24:27:		; Délai avant le premier tir (* 16 frames).
Shot Delay = 28:31:		; Délai entre les tirs (* 16 frames).
Shot Nb = 32:35:		; Nb de tirs.
Scroll type = 36:36: Y - X	; Scroll V ou H.
prm = 37:39:			; tmp
*/
			pData[0] = pSpe->nType + (pSpe->nDrop << 4);
			pData[1] = pSpe->nSeqNo + (pSpe->nOffset << 4);
			pData[2] = pSpe->nOrder;

			pData[2] |= (pSpe->sav_nShot << 4) & 0xF0;
			pData[3] = (pSpe->sav_nShot >> 4) & 0xFF;
			pData[4] = (pSpe->sav_nShot >> 12) & 0x0F;

			pData[4] |= (pSpe->sav_Bits0 << 4) & 0xF0;

			if (pSpe->nType == 4)
			{	// Mars UFO.
				u32	nRnd = rand();
				// On génère les 3 monstres d'un coup.
				MstAdd(e_Mst41_L11MarsUFO0, (gScrollPos.nPosX >> 8) + SCR_Width + 32, (gScrollPos.nPosY >> 8) + (nRnd & 0x7F), pData, -1);
				pSpe->nOrder++;
				pData[2] &= 0xF0;
				pData[2] |= pSpe->nOrder;
				nRnd += 32;
				MstAdd(e_Mst41_L11MarsUFO0, (gScrollPos.nPosX >> 8) + ((SCR_Width - 256) / 2) + (nRnd & 0xFF), (gScrollPos.nPosY >> 8) - 16, pData, -1);
				pSpe->nOrder++;
				pData[2] &= 0xF0;
				pData[2] |= pSpe->nOrder;
				nRnd += 32;
				MstAdd(e_Mst41_L11MarsUFO0, (gScrollPos.nPosX >> 8) - 32, (gScrollPos.nPosY >> 8) + (nRnd & 0x7F), pData, -1);
				pSpe->nNb = 1;		// ...Et on arrête la génération.
			}
			else
			{	// Mars Eye / Mars TIE.
				MstAdd(e_Mst34_L11MarsEye0, pMst->nPosX >> 8, pMst->nPosY >> 8, pData, -1);
				pSpe->nOrder++;
			}

			// Reinit compteur.
			pSpe->nCnt0 = MST33_TIMER0;
			break;

		case 2:		// Medium Asteroid
			// Add monstre.
/*
[mst36] L11 Asteroids
SeqNo = 0:3:		; No de la séquence.
prm = 4:7:			; tmp
*/
			// Position au hasard.
//			nPosX = ((rand() % (SCR_Width * 2)) - (SCR_Width / 2)) << 8;
			nPosX = ((rand() % SCR_Width) + (pSpe->nNb & 1 ? SCR_Width : 0) - (SCR_Width / 2)) << 8;
			nPosY = gScrollPos.nPosY - (16 << 8);
			//
			pData[0] = pSpe->nSeqNo;
			MstAdd(e_Mst36_L11Asteroid0, nPosX >> 8, nPosY >> 8, pData, -1);

			// Reinit compteur.
			pSpe->nCnt0 = MST33_TIMER0 - (pSpe->nOffset << 2);  // Pour les astéroïdes, on se sert de l'offset pour augmenter la fréquence d'apparition.
			break;

		case 3:		// Big Asteroid
			gMstMisc.pSeqCount2[pSpe->nSeqNo] = 3;	// On va génèrer 3 astéroïdes.
			pSpe->nNb = 1;		// Au cas ou...
			// Add monstre.
/*
[mst37] L11 Big Asteroids
SeqNo = 0:3:		; No de la séquence.
Order = 4:7:		; N° d'ordre dans la séquence (pour trouver le leader).
*/
			// Au milieu.
			nPosX = (SCR_Width / 2) << 8;
			nPosY = gScrollPos.nPosY - (16 << 8);
			pData[0] = pSpe->nSeqNo | (pSpe->nOrder << 4);
			MstAdd(e_Mst37_L11BigAsteroid0, nPosX >> 8, nPosY >> 8, pData, -1);
			pSpe->nOrder++;
			// A gauche.
			nPosX = (SCR_Width / 4) << 8;
			nPosY = gScrollPos.nPosY - (24 << 8);
			pData[0] = pSpe->nSeqNo | (pSpe->nOrder << 4);
			MstAdd(e_Mst37_L11BigAsteroid0, nPosX >> 8, nPosY >> 8, pData, -1);
			pSpe->nOrder++;
			// A droite.
			nPosX = (3 * SCR_Width / 4) << 8;
			nPosY = gScrollPos.nPosY - (32 << 8);
			pData[0] = pSpe->nSeqNo | (pSpe->nOrder << 4);
			MstAdd(e_Mst37_L11BigAsteroid0, nPosX >> 8, nPosY >> 8, pData, -1);
			pSpe->nOrder++;

			// Reinit compteur.
			pSpe->nCnt0 = MST33_TIMER0;
			break;

		case 5:		// Flying Tara.
/*
[mst43] Flying Tara
SeqNo = 0:3:	; No de la séquence.
Order = 4:7:	; N° d'ordre dans la séquence (pour trouver le leader).
Drop = 8:11: Nothing - H_Machinegun - Gas_Tank - Ammo_Box1	; H non utilisé.
Mode = 12:14: L02_Path - L02_To_Player - Std_Level -
prm = 15:15:			; tmp
*/
			pData[0] = pSpe->nSeqNo + (pSpe->nOrder << 4);
			pData[1] = pSpe->nDrop;

			MstAdd(e_Mst43_FlyingTara0, pMst->nPosX >> 8, pMst->nPosY >> 8, pData, -1);
			pSpe->nOrder++;

			// Reinit compteur.
			pSpe->nCnt0 = MST33_TIMER_FTARA0;
			break;

		case 6:		// L11 - Fin de niveau.
			{
				s32	nPlane1PosX, nPlane1PosY;
				Scroll_HeroPlaneScrollPosXY_Get(&nPlane1PosX, &nPlane1PosY);
				// On dépasse la position de fin ?
				if (pMst->nPosY >= nPlane1PosY)
				{
					// Donne l'item qui va bien.
					gpMstQuestItems[MST_QUEST_ITEM_NEXT_LEVEL] |= 1;
				}
				pSpe->nCnt0 = 4;	// Test toutes les x frames.
			}
			return (e_MstState_Managed);
			//break;

		case 7:		// Weapon capsule.
/*
[mst4] Weapon Capsule
Type = 0:3: S_Shotgun - H_Machinegun - F_Flamethrower - R_Rocket - Gas_Tank - Ammo_Box1
Boxed = 4:4: No - Yes		; Une caisse contient l'item ?
Location = 5:6: Normal - Space - Underwater - Air
*/
			if (pSpe->nDrop)
			{
				static	u8 gpnMst33ItmTb[] = { 0, e_Player_Weapon_Machinegun, 5, 6 };

				pData[0] = (gpnMst33ItmTb[pSpe->nDrop & 3] - 1);
				if (pSpe->sav_Bits0)
				{
					nPosX = gScrollPos.nPosX + ((SCR_Width + 16) << 8);
					nPosY = pMst->nPosY;
					pData[0] |= 3 << 5;
				}
				else
				{
					nPosX = pMst->nPosX;
					nPosY = gScrollPos.nPosY - (16 << 8);
					pData[0] |= 1 << 5;
				}
				MstAdd(e_Mst4_WeaponCapsule, nPosX >> 8, nPosY >> 8, pData, -1);
			}
			//return (e_MstState_Dead);
			break;

		}

		// Tous les monstres ont été générés.
		if (--pSpe->nNb == 0)
		switch (pSpe->nScrollStop)
		{
		case 1:		// Stop pendant la génération.
			gpMstQuestItems[MST_QUEST_ITEM_MST32]--;		// -- sur l'item de stop.
		default:	// Pas de stop.
			return (e_MstState_Dead);
		case 2:		// Passe en phase d'attente que le dernier monstre de la série disparaisse.
			pMst->nPhase = e_Mst33_WaitLastMstDisp;
			break;
		}

	}

	return (e_MstState_Managed);
}

//=============================================================================
// Petites fonctions de 'chemin'.

/*
struct SMstPath
{
	s32	nPathPosX, nPathPosY;	// Pour recherche des chemins.
	s16 nSpdGoal;		// Pour accélérations.
	u8	nNewAngle;		// Nouvel angle à atteindre quand on tourne.
	s8	nSensRot;		// Sens de rotation.
};
*/

// Init.
void Mst_PathInit(struct SMstCommon *pMst, struct SMstPath *pPath, s32 nSpdInit, u8 nAngInit)
{
	s32	nVal;

	// Placement au centre du bloc.
	pMst->nPosX &= ~0xFFF;
	pMst->nPosX |= 0x800;
	pMst->nPosY &= ~0xFFF;
	pMst->nPosY |= 0x800;
	pMst->nSpd = nSpdInit;
	pPath->nSpdGoal = pMst->nSpd;
	pPath->nPathPosX = pMst->nPosX;	// Svg des pos de départ.
	pPath->nPathPosY = pMst->nPosY;
	// Un bloc de chemin ?
	pMst->nAngle = nAngInit;				// Direction par défaut.
	if ((nVal = Map_PathGndGetBlock(pMst->nPosX >> 12, (pMst->nPosY >> 12) + 1)) != -1)
	if (nVal < 16)
	{
		pMst->nAngle = nVal << 4;
	}
	pMst->nFlipX = ((u8)(pMst->nAngle + 64) < 128 ? 1 : 0);
	if (pMst->nAngle == 64 || pMst->nAngle == 192)
		pMst->nFlipX = (pMst->nPosX >> 8 < SCR_Width / 2 ? 1 : 0);

}

// Déplacement sur un chemin.
void Mst_PathMove(struct SMstCommon *pMst, struct SMstPath *pPath, s32 nSpdAcc, u32 nPhaseTurn)
{
	s32	nSpdX, nSpdY;
	s32	nVal;

	// Accélération ?
	if (pMst->nSpd != (s32)pPath->nSpdGoal)
	{
		nVal = ABS(pMst->nSpd - (s32)pPath->nSpdGoal);
		if (nVal < nSpdAcc) nSpdAcc = nVal;
		pMst->nSpd += ((s32)pPath->nSpdGoal > pMst->nSpd ? nSpdAcc : -nSpdAcc);
	}
	// Déplacement du monstre.
	nSpdX = (gVar.pCos[pMst->nAngle] * pMst->nSpd) >> 8;
	nSpdY = (gVar.pSin[pMst->nAngle] * pMst->nSpd) >> 8;
	pMst->nPosX += nSpdX;
	pMst->nPosY += nSpdY;
	// Test du changement de direction, sauf pendant qu'on se tourne.
	if (pMst->nPhase != nPhaseTurn)
	{
		u32	nOld, nNew;
		nOld = ((pPath->nPathPosX >> 12) & 0xFF) | ((pPath->nPathPosY >> 4) & 0xFF00);		// Old Pos.
		pPath->nPathPosX += nSpdX;	// Deplacement du bloc path.
		pPath->nPathPosY += nSpdY;
		nNew = ((pPath->nPathPosX >> 12) & 0xFF) | ((pPath->nPathPosY >> 4) & 0xFF00);		// New Pos.
		// Changement de bloc. Nouvelle direction ?
		if (nOld != nNew)
		{
			nNew = Map_PathGndGetBlock(pPath->nPathPosX >> 12, pPath->nPathPosY >> 12);
			if (nNew != -1)
			{
//printf("Order=%d / x=%d y=%d / pathx=%d pathy=%d\n",pMst->nLoadIdx, pMst->nPosX>>12, pMst->nPosY>>12, pPath->nPathPosX>>12, pPath->nPathPosY>>12);
				// Recalage au centre du bloc de path pour les recherches à suivre.
				pPath->nPathPosX &= ~0xFFF;
				pPath->nPathPosX |= 0x700;	//0x800;
				pPath->nPathPosY &= ~0xFFF;
				pPath->nPathPosY |= 0x700;	//0x800;
				//
				if (nNew < 16)	// Direction.
				{
					u8	nDiff;
					// Détermine le sens de rotation.
					pPath->nNewAngle = nNew << 4;
					nDiff = pMst->nAngle - pPath->nNewAngle;
					pPath->nSensRot = (nDiff & 128 ? 2 : -2);
					pMst->nPhase = nPhaseTurn;
					// Ajustement du recalage pour les chemins qui bouclent.
					if (nNew >= 8) pPath->nPathPosY += 0x100;
//					if (nNew >= 4 && nNew < 12) pPath->nPathPosX += 0x100;
					if (nNew < 4 || nNew >= 12) pPath->nPathPosX += 0x100;
				}
				else if (nNew == 17)	// Speed up.
				{
					pPath->nSpdGoal += 0x100;
				}
			}
		}
	}

}

//=============================================================================
// Mst 34 : L11 Mars Eye.

/*
[mst34] L11 Mars Eye
Type = 0:3: MarsEyeTIE - MarsEye2 -
Drop = 4:7: Nothing - H_Machinegun - Gas_Tank -
SeqNo = 8:11:			; No de la séquence.
Offset = 12:15:			; Offset.
Order = 16:19:			; N° d'ordre dans la séquence (pour trouver le leader).
Shot Pattern = 20:23: One_By_One - All_Together	; Type de tir : Un par un - Tous ensemble.
Shot 1st Delay = 24:27:		; Délai avant le premier tir (* 16 frames).
Shot Delay = 28:31:		; Délai entre les tirs (* 16 frames).
Shot Nb = 32:35:		; Nb de tirs.
Scroll type = 36:36: Y - X	; Scroll V ou H.
prm = 37:39:			; tmp
*/

// Phases.
enum
{
	e_Mst34_Fly = 0,
	e_Mst34_Turn,
};

#define	MST34_SCREEN_OFFSET	24

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

u64	*gpMst34AnmFlyTb[] = { gAnm_MarsTIE_Fly, gAnm_MarsEye_Fly };
u64	*gpMst34AnmFlipTb[] = { gAnm_MarsTIE_Flip, gAnm_MarsEye_Flip };
u8 gpnMst34ItmTb[] = { 0, e_Player_Weapon_Machinegun, 5, 6 };

void Mst34_Init_L11MarsEye0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst34_L11MarsEye0	*pSpe = (struct SMst34_L11MarsEye0 *)pMst->pData;
	s32	nVal;
	u32	nOffset;

	nVal = GetBits(0, 3, pData, 0);
	pSpe->nType = nVal;
	AnmSet(gpMst34AnmFlyTb[pSpe->nType], pMst->nAnm);	// Anim.
	nVal = GetBits(4, 7, pData, 0);
	pSpe->nDrop = gpnMst34ItmTb[nVal];
//pSpe->nDrop = gnItmTb[2];//2;	//test test test test
	nVal = GetBits(8, 11, pData, 0);
	pSpe->nSeqNo = nVal;	// N° de séquence.
	nVal = GetBits(16, 19, pData, 0);
	pSpe->nOrder = nVal;	// N° d'ordre dans la séquence.
	nOffset = GetBits(12, 15, pData, 0);
	// Bits leader.
	gMstMisc.pSeqLeader[pSpe->nSeqNo] |= 1 << pSpe->nOrder;	// Pour recherche des leaders.

	Mst_PathInit(pMst, &pSpe->sPath, 0x200, 192);	// Init path.

	pMst->nPhase = e_Mst34_Fly;
	pSpe->nHitCnt = 0;
	pSpe->nLife = 3;//4;//1;

	// Prm de tir.
	pSpe->nShotAnm = -1;
	nVal = GetBits(24, 27, pData, 0);	// First delay.
	pSpe->nShotCnt = nVal << 4;
	nVal = GetBits(20, 23, pData, 0);	// Shot pattern. 0 = 1 par 1 / 1 = Tous ensemble.
	if (nVal)
		pSpe->nShotCnt -= (24 * pSpe->nOrder);		// Tous ensemble => On décale le timer mst par mst.
	nVal = GetBits(28, 31, pData, 0);	// Fréquence.
	pSpe->nShotFreq = nVal << 4;
	nVal = GetBits(32, 35, pData, 0);	// Nb de tirs.
	pSpe->nShotNb = nVal;

	// Replacement aux abords de l'écran.
	nVal = GetBits(36, 36, pData, 0);
	pSpe->nScrollXY = nVal;
	if (pSpe->nScrollXY)
	{	// Scroll X.
		pMst->nPosX = gScrollPos.nPosX + (SCR_Width << 8);
		pMst->nPosX -= nOffset << 12;
	}
	else
	{	// Scroll Y.
		pMst->nPosY = gScrollPos.nPosY;
		pMst->nPosY += nOffset << 12;
	}
	// Et décalage en dehors.
	Mst_PutOutOfScreen(pMst, MST34_SCREEN_OFFSET);

}

// Fonction commune aux monstres 34 et 41.
// Affichage, gestions des chocs et des tirs.
u32 Mst34_41_Common(struct SMstCommon *pMst)
{
	struct SMst34_L11MarsEye0	*pSpe = (struct SMst34_L11MarsEye0 *)pMst->pData;
	u32	nSpr;

	nSpr = AnmGetImage(pMst->nAnm);
	if (AnmCheckStepFlag(pMst->nAnm)) pMst->nFlipX ^= 1;	// Flip ?

	// Se prend un tir ?
	if (Mst_ShotCheckLife(nSpr, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife))
	{
		gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;	// Score.
_34MarsEyeDeath:
		// Différents dusts.
		DustSet(gAnm_Explosion0_Medium_Dust, pMst->nPosX, pMst->nPosY + (5 << 8), e_Prio_DustUnder, 0);
//	DustSetMvt(gAnm_Debris_Metal0_Dust, pMst->nPosX, pMst->nPosY, 0x180, -0x480, e_Prio_DustUnder - 1, e_DustFlag_Gravity);
//	DustSetMvt(gAnm_Debris_Metal0_Dust, pMst->nPosX, pMst->nPosY, -0x180, -0x380, e_Prio_DustUnder - 1, e_DustFlag_Gravity);
		// Cleare le bit de lead.
		gMstMisc.pSeqLeader[pSpe->nSeqNo] &= ~(1 << pSpe->nOrder);
		// Décrémente la séquence.
		if (--gMstMisc.pSeqCount[pSpe->nSeqNo] == 0)
		if (pSpe->nDrop)
		{
			// Largage d'une item quand le dernier monstre de la séquence explose.
/*
[mst4] Weapon Capsule
Type = 0:3: S_Shotgun - H_Machinegun - F_Flamethrower - R_Rocket - Gas_Tank - Ammo_Box1
Boxed = 4:4: No - Yes		; Une caisse contient l'item ?
Location = 5:6: Normal - Space - Underwater - Air
*/
			u32	nData;
			nData = (pSpe->nDrop - 1) | ((pSpe->nScrollXY ? 3 : 1) << 5);	// Location : Space ou Air.
			MstAdd(e_Mst4_WeaponCapsule, pMst->nPosX >> 8, pMst->nPosY >> 8, (u8 *)&nData, -1);
		}
		// Mort.
		gMstMisc.pSeqCount2[pSpe->nSeqNo]--;
		if (pSpe->nShotAnm != -1) AnmReleaseSlot(pSpe->nShotAnm);
		return (e_MstState_Dead);
	}

	// Contact avec le joueur ?
	struct SSprRect sRect1;
	if (SprGetRect(nSpr, e_SprRectZone_RectCol, &sRect1))
	if (sRect1.nType == e_SprRect_Rect)
	if (Enemy_PlayerBlowRect(&sRect1, pMst->nPosX, pMst->nPosY, MST_DAMAGE_EXPLO)) goto _34MarsEyeDeath;

	// Est-on le leader ? (ok mais inutile pour l'instant)
//	u32	nLeader = ((gMstMisc.pSeqLeader[pSpe->nSeqNo] & ~(0xFF << (pSpe->nOrder + 1))) == 1 << pSpe->nOrder ? 1 : 0);	// Le leader = le bit le plus à droite !

	// Déclenchement de l'anim de tir ?
	if (MST_SHOT_COND)		// Si le héros n'est pas mort...
	if (pSpe->nShotCnt) pSpe->nShotCnt--;
	if (pSpe->nShotCnt == 1)
	{
		pSpe->nShotAnm = AnmSet(gAnm_MarsUFO_ShotFx, pSpe->nShotAnm);
	}
	// Gestion du tir.
	u32	nSprShot = SPR_NoSprite;
	u32	nShotOffsX = 0, nShotOffsY = 0;
	if (pSpe->nShotAnm != -1)
	{
		nSprShot = AnmGetImage(pSpe->nShotAnm);
		// Offset du tir.
		if (SprGetRect(nSpr ^ (pMst->nFlipX ? SPR_Flip_X : 0), e_SprRectZone_ShotOrg, &sRect1))
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
			FireAdd(e_Shot_Enemy_MarsUFO_Bullet0, pMst->nPosX + (nShotOffsX << 8), pMst->nPosY + (nShotOffsY << 8),
					fatan2(-((gShoot.nPlayerPosY - (2 << 12) + (gShoot.nVehicleType < e_HeroVehicle_SlugBase ? 4 << 12 : 0)) - (pMst->nPosY + (nShotOffsY << 8))), gShoot.nPlayerPosX - (pMst->nPosX + (nShotOffsX << 8))) );
//					fatan2(-((gShoot.nPlayerPosY - (2 << 12)) - (pMst->nPosY + (nShotOffsY << 8))), gShoot.nPlayerPosX - (pMst->nPosX + (nShotOffsX << 8))) );
			// Reinitialisation du tir.
			if (--pSpe->nShotNb) pSpe->nShotCnt = pSpe->nShotFreq;
		}
	}

	// Affichage body.
	if (pSpe->nHitCnt) pSpe->nHitCnt--;
	SprDisplay((nSpr | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0)) ^ (pMst->nFlipX ? SPR_Flip_X : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies + gnMstPrio);
	// Affichage de l'éclat du tir.
	if (nSprShot != SPR_NoSprite)
		SprDisplay(nSprShot | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), (pMst->nPosX >> 8) + nShotOffsX, (pMst->nPosY >> 8) + nShotOffsY, e_Prio_Ennemies + 1 + gnMstPrio);

//> tst de visu
//if (nLeader)
//SprDisplay(e_Spr_FontMission_Chrs, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies + gnMstPrio + 1);
//< tst de visu

	return (e_MstState_Managed);
}

#define	MST34_ACC	0x08
s32 Mst34_Main_L11MarsEye0(struct SMstCommon *pMst)
{
	struct SMst34_L11MarsEye0	*pSpe = (struct SMst34_L11MarsEye0 *)pMst->pData;

	// Sortie de l'écran ?
	if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY))
	{
		gMstMisc.pSeqCount2[pSpe->nSeqNo]--;
		if (pSpe->nShotAnm != -1) AnmReleaseSlot(pSpe->nShotAnm);	// Libère le slot de tir.
		return (e_MstState_Dead);
	}

	switch (pMst->nPhase)
	{
	case e_Mst34_Fly:
		break;

	case e_Mst34_Turn:
		{
			if (pMst->nAngle == pSpe->sPath.nNewAngle)
			{
				pMst->nPhase = e_Mst34_Fly;
				break;
			}
			pMst->nAngle += pSpe->sPath.nSensRot;
			// Sens.
			u8	nReqFlipX;
			nReqFlipX = ((u8)(pMst->nAngle + 64) < 128 ? 1 : 0);
			if (pMst->nAngle == 64 || pMst->nAngle == 192)
				nReqFlipX = (pMst->nPosX >> 8 < SCR_Width / 2 ? 1 : 0);
			if (nReqFlipX != pMst->nFlipX)
				AnmSetIfNew(gpMst34AnmFlipTb[pSpe->nType], pMst->nAnm);
		}
		break;
	}

	// Déplacement sur path.
	Mst_PathMove(pMst, &pSpe->sPath, MST34_ACC, e_Mst34_Turn);

	// Affichage, tir, contact, etc...
	return (Mst34_41_Common(pMst));

}

//=============================================================================
// Mst 35 : L11 Space Rocks.

/*
[mst35] L11 Space Rocks
prm = 0:7:			; tmp
*/

#define	L11_SCROLLSPD	0x100

#define	L11SPACEROCKS_CNT	16
#define	L11SPACEROCKS_MAX	16
struct SL11SpaceRock
{
	s32	nPosX, nPosY;
	s16	nSpd;
	u8	nType;
	u8	nCnt;
};
struct SL11SpaceRock	gpL11SpaceRocks[L11SPACEROCKS_MAX];	// Tableau général pour stockage des cailloux.
u8	gnL11SpaceRockAngle;	// Angle, utile pour les mst 35 et 36.

// Structure spécifique.
struct SMst35_L11SpaceRocks0
{
	u32	nPrevBlkY;
	u8	nReqAngle;	// Angle cible lors des changements de direction.
};

// Init d'un caillou.
void Mst35_sub_NewRock(u32 nIdx)
{
	u32	nRnd = rand();

	gpL11SpaceRocks[nIdx].nPosX = ((nRnd % (SCR_Width * 2)) - (SCR_Width / 2)) << 8;
	gpL11SpaceRocks[nIdx].nPosY = gScrollPos.nPosY - (10 << 8);
	gpL11SpaceRocks[nIdx].nType = (nRnd >> 1) & 1;
	gpL11SpaceRocks[nIdx].nSpd = 0x050 + (nRnd & 0x30);	//0x110 + (nRnd & 0x70)
	gpL11SpaceRocks[nIdx].nSpd += (gpL11SpaceRocks[nIdx].nType ? 0 : 0x80);
	gpL11SpaceRocks[nIdx].nCnt = nRnd;
}

void Mst35_Init_L11SpaceRocks0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst35_L11SpaceRocks0	*pSpe = (struct SMst35_L11SpaceRocks0 *)pMst->pData;
	u32	i;

	// Init sur tout l'écran.
	for (i = 0; i < L11SPACEROCKS_MAX; i++)
	{
		Mst35_sub_NewRock(i);
		gpL11SpaceRocks[i].nPosY = gScrollPos.nPosY + (((rand() % (SCR_Height + 20)) - 10) << 8);
	}
	gnL11SpaceRockAngle = 192;	// Par défaut, vers le bas.
	pSpe->nReqAngle = gnL11SpaceRockAngle;

	// Init du bloc Y pour recherche des chemins.
	s32	nPlane1PosX, nPlane1PosY;
	Scroll_HeroPlaneScrollPosXY_Get(&nPlane1PosX, &nPlane1PosY);
	pSpe->nPrevBlkY = nPlane1PosY >> 12;

}

s32 Mst35_Main_L11SpaceRocks0(struct SMstCommon *pMst)
{
	struct SMst35_L11SpaceRocks0	*pSpe = (struct SMst35_L11SpaceRocks0 *)pMst->pData;
	u32	i;

	// On change de bloc ?
	s32	nPlane1PosX, nPlane1PosY;
	Scroll_HeroPlaneScrollPosXY_Get(&nPlane1PosX, &nPlane1PosY);
	nPlane1PosY >>= 12;
	if (pSpe->nPrevBlkY != nPlane1PosY)
	{
		pSpe->nPrevBlkY = nPlane1PosY;
		if ((i = Map_PathAirGetBlock(pMst->nPosX >> 12, nPlane1PosY - MST_CLIP_VAL)) != -1)
		if (i < 16)
			pSpe->nReqAngle = i << 4;
	}

	// Changement de direction ?
	if (pSpe->nReqAngle != gnL11SpaceRockAngle)
	if ((gnFrame & 1) == 0)
	{
		i = gnL11SpaceRockAngle - pSpe->nReqAngle;
		gnL11SpaceRockAngle += (i & 0x80 ? 1 : -1);
	}

	// Management des cailloux.
	for (i = 0; i < L11SPACEROCKS_MAX; i++)
	{
		// Déplacement.
		gpL11SpaceRocks[i].nPosX += (gVar.pCos[gnL11SpaceRockAngle] * (s32)gpL11SpaceRocks[i].nSpd) >> 8;
		gpL11SpaceRocks[i].nPosY += (gVar.pSin[gnL11SpaceRockAngle] * (s32)gpL11SpaceRocks[i].nSpd) >> 8;
		gpL11SpaceRocks[i].nPosY += L11_SCROLLSPD;	// Scroll spd.
		// Sortie de l'écran ?
		if (gpL11SpaceRocks[i].nPosY > gScrollPos.nPosY + ((SCR_Height + 10) << 8))
			Mst35_sub_NewRock(i);
		// Affichage.
		SprDisplay((e_Spr_SpaceRock_Medium + ((u32)gpL11SpaceRocks[i].nCnt >> 5) + ((u32)gpL11SpaceRocks[i].nType << 3)) ^ (pMst->nFlipX ? SPR_Flip_X : 0), gpL11SpaceRocks[i].nPosX >> 8, gpL11SpaceRocks[i].nPosY >> 8, e_Prio_EnnemiesBg - 1 - gpL11SpaceRocks[i].nType);
		gpL11SpaceRocks[i].nCnt += 8 - ((gpL11SpaceRocks[i].nSpd >> 4) & 1);//(gpL11SpaceRocks[i].nPosX & 1);
	}

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 36 : L11 Asteroid.

/*
[mst36] L11 Asteroids
SeqNo = 0:3:		; No de la séquence.
prm = 4:7:			; tmp
*/

// Structure spécifique.
struct SMst36_L11Asteroid0
{
	u8	nHitCnt, nLife;
	u8	nSeqNo;
};

void Mst36_Init_L11Asteroid0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst36_L11Asteroid0	*pSpe = (struct SMst36_L11Asteroid0 *)pMst->pData;
	s32	nVal;

	nVal = GetBits(0, 3, pData, 0);
	pSpe->nSeqNo = nVal;

	pSpe->nHitCnt = 0;
	pSpe->nLife = 4;//20;
	pMst->nSpd = 0x100;//0x200;

}

s32 Mst36_Main_L11Asteroid0(struct SMstCommon *pMst)
{
	struct SMst36_L11Asteroid0	*pSpe = (struct SMst36_L11Asteroid0 *)pMst->pData;
	u32	nSpr;

	// Sortie de l'écran ?
	if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) { gMstMisc.pSeqCount2[pSpe->nSeqNo]--; return (e_MstState_Dead); }

	// Déplacement du monstre.
	s32	nSpdX, nSpdY;
	nSpdX = (gVar.pCos[gnL11SpaceRockAngle] * pMst->nSpd) >> 8;
	nSpdY = (gVar.pSin[gnL11SpaceRockAngle] * pMst->nSpd) >> 8;
	pMst->nPosX += nSpdX;
	pMst->nPosY += nSpdY;
	pMst->nPosY += L11_SCROLLSPD;

	nSpr = AnmGetImage(pMst->nAnm);

	// Se prend un tir ?
	if (Mst_ShotCheckLife(nSpr, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife))
	{
		gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;	// Score.
_36AsteroidDeath:
		// Dust.
		DustSet(gAnm_Explosion0_Medium_Dust, pMst->nPosX, pMst->nPosY + (16 << 8), e_Prio_DustUnder, 0);
		// Mort.
		gMstMisc.pSeqCount2[pSpe->nSeqNo]--;
		return (e_MstState_Dead);
	}

	// Contact avec le joueur ?
	struct SSprRect sRect1;
	if (SprGetRect(nSpr, e_SprRectZone_RectCol, &sRect1))
	if (sRect1.nType == e_SprRect_Rect)
	if (Enemy_PlayerBlowRect(&sRect1, pMst->nPosX, pMst->nPosY, MST_DAMAGE_EXPLO)) goto _36AsteroidDeath;

	// Affichage.
	if (pSpe->nHitCnt) pSpe->nHitCnt--;
	SprDisplay((nSpr | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0)) ^ (pMst->nFlipX ? SPR_Flip_X : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies + gnMstPrio);

	return (e_MstState_Managed);

}

//=============================================================================
// Mst 37 : L11 Big Asteroid.

void Player_Damage(u32 nDamage);
u32 Rocket_EnemyCheckScreenOut(void);
void Game_Player_DeathSet(void);

/*
[mst37] L11 Big Asteroids
SeqNo = 0:3:		; No de la séquence.
Order = 4:7:		; N° d'ordre dans la séquence (pour trouver le leader).
*/

// Phases.
enum
{
	e_Mst37_Fly = 0,
	e_Mst37_Cracks,
	e_Mst37_Broken,
};

// Structure spécifique.
struct SMst37_L11BigAsteroid0
{
	u8	nHitCnt, nLife;
	u8	nSeqNo;			// N° de la séquence.
	u8	nOrder;			// N° d'ordre dans la séquence.
	s32	nPosX2;			// PosX seconde partie.
	u8	nDustNo;		// Pour décalage des dusts.
};

void Mst37_Init_L11BigAsteroid0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst37_L11BigAsteroid0	*pSpe = (struct SMst37_L11BigAsteroid0 *)pMst->pData;
	s32	nVal;

	nVal = GetBits(0, 3, pData, 0);
	pSpe->nSeqNo = nVal;
	nVal = GetBits(4, 7, pData, 0);
	pSpe->nOrder = nVal;

	pSpe->nHitCnt = 0;
	pSpe->nLife = 20;
	pMst->nSpdY = 0x80 + (pSpe->nOrder << 4);
//	pMst->nSpd = (pSpe->nOrder << 4) * (pSpe->nOrder & 1 ? -1 : 1);
	pMst->nSpd = (pSpe->nOrder ? (pSpe->nOrder & 1 ? -0x20 : 0x20) : 0);

	pSpe->nPosX2 = pMst->nPosX;
	pSpe->nDustNo = 0;

	pMst->nPhase = e_Mst37_Fly;

}

#define	MST37_SPD_SEPARATE	0x080
s32 Mst37_Main_L11BigAsteroid0(struct SMstCommon *pMst)
{
	struct SMst37_L11BigAsteroid0	*pSpe = (struct SMst37_L11BigAsteroid0 *)pMst->pData;
	u32	nSprDisp;			// Sprite à afficher (spr + spr+1).
	u32	pnSprCtc[2];		// Sprites de contact.
	u32	nSprCtcInList = 0;
	u32	i;

	// Sort de l'écran par le bas ?
	if (pMst->nPosY > gScrollPos.nPosY + (SCR_Height << 8) + (7 << 12))
	{
//		gMstMisc.pSeqCount2[pSpe->nSeqNo]--;
		return (e_MstState_Dead);
	}

	nSprDisp = e_Spr_BigAsteroid;	// Spr par défaut.

	switch (pMst->nPhase)
	{
	case e_Mst37_Fly:
		pnSprCtc[nSprCtcInList++] = nSprDisp;	// Sprite de contact.

		// Se prend un tir ?
		if (Mst_ShotCheckLife(e_Spr_BigAsteroid + 1, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife))
		{
			gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;	// Score.
			pMst->nPhase = e_Mst37_Cracks;
		}
		break;

	case e_Mst37_Cracks:
		nSprDisp = AnmGetImage(pMst->nAnm);
		pnSprCtc[nSprCtcInList++] = nSprDisp;	// Sprite de contact.
		// Dusts quand signal donné.
		if (AnmCheckStepFlag(pMst->nAnm))
		{
			DustSetMvt(gAnm_BigAsteroid_Cracks_Dust, pMst->nPosX + ((pSpe->nDustNo & 1) << (3+8)), pMst->nPosY - ((80 - (23 * pSpe->nDustNo)) << 8), pMst->nSpd, pMst->nSpdY, e_Prio_DustOver, 0);
			pSpe->nDustNo++;
		}
		// Fin de l'anim ?
		if (AnmCheckEnd(pMst->nAnm))
		{
			nSprDisp = e_Spr_BigAsteroid + 14;
			pMst->nPhase = e_Mst37_Broken;
		}
		break;

	case e_Mst37_Broken:
		nSprDisp = e_Spr_BigAsteroid + 14;
		// Sprites de contact.
		pnSprCtc[nSprCtcInList++] = nSprDisp;
		pnSprCtc[nSprCtcInList++] = nSprDisp + 1;
		// Eloignement des morceaux droite et gauche.
		pMst->nPosX -= MST37_SPD_SEPARATE;
		pSpe->nPosX2 += MST37_SPD_SEPARATE;
		break;
	}

	// Déplacement du monstre.
	s32	nOldY = pMst->nPosY;
	pMst->nPosX += pMst->nSpd;
	pSpe->nPosX2 += pMst->nSpd;
	pMst->nPosY += pMst->nSpdY;

	// Décrémente la séquence ?
	if (nOldY < gScrollPos.nPosY + ((SCR_Height - 32) << 8) && pMst->nPosY >= gScrollPos.nPosY + ((SCR_Height - 32) << 8))
		gMstMisc.pSeqCount2[pSpe->nSeqNo]--;

	// Blocage des tirs + touchés.
	for (i = 0; i < nSprCtcInList; i++)
	{
		u32	nSpr;
		nSpr = pnSprCtc[i];
		// Blocage des tirs (sans dégats sur le monstre, gérés plus haut dans la phase e_Mst37_Fly).
		FireHitCheck(nSpr, (i == 0 ? pMst->nPosX : pSpe->nPosX2), pMst->nPosY, e_ShotOrg_Player, 1);

		// Contact avec le joueur ?
		if (gShoot.nInvulnerability == 0)
		{
			struct SSprRect sRect1, sRect2;
			s32	nXMin1, nXMax1, nYMin1, nYMax1;
			s32	nXMin2, nXMax2, nYMin2, nYMax2;
			// Rect ennemi.
			if (SprGetRect(nSpr, e_SprRectZone_RectCol, &sRect1) == 0) continue;
			if (sRect1.nType != e_SprRect_Rect) continue;
			// Appel du dégat normal pour dégager les cannons. > Ou pas ?
//			if (gShoot.nVehicleHitCnt == 0)
//				Enemy_PlayerBlowRect(&sRect1, (i == 0 ? pMst->nPosX : pSpe->nPosX2), pMst->nPosY, MST_DAMAGE_BULLET);
			// Rect joueur.
			if (gShoot.nPlayerSprCol == SPR_NoSprite) continue;
			if (SprGetRect(gShoot.nPlayerSprCol, e_SprRectZone_RectCol, &sRect2) == 0) continue;
			if (sRect2.nType != e_SprRect_Rect) continue;
			// Rectangle de col de l'ennemi.
			nXMin1 = (i == 0 ? pMst->nPosX : pSpe->nPosX2) + (sRect1.nX1 << 8);
			nXMax1 = (i == 0 ? pMst->nPosX : pSpe->nPosX2) + (sRect1.nX2 << 8);
			nYMin1 = pMst->nPosY + (sRect1.nY1 << 8);
			nYMax1 = pMst->nPosY + (sRect1.nY2 << 8);
			// Rectangle de col du joueur.
			nXMin2 = gShoot.nPlayerPosX + (sRect2.nX1 << 8);
			nXMax2 = gShoot.nPlayerPosX + (sRect2.nX2 << 8);
			nYMin2 = gShoot.nPlayerPosY + (sRect2.nY1 << 8);
			nYMax2 = gShoot.nPlayerPosY + (sRect2.nY2 << 8);

			// Collisions entre les rectangles ?
			if (nXMax1 >= nXMin2 && nXMin1 <= nXMax2 && nYMax1 >= nYMin2 && nYMin1 <= nYMax2)
			{
				s32	nCtrX1, nCtrY1, nCtrX2, nCtrY2;
				s32	nDX, nDY;
				nCtrX1 = nXMin1 + ((nXMax1 - nXMin1) / 2);
				nCtrY1 = nYMin1 + ((nYMax1 - nYMin1) / 2);
				nCtrX2 = nXMin2 + ((nXMax2 - nXMin2) / 2);
				nCtrY2 = nYMin2 + ((nYMax2 - nYMin2) / 2);
				nDX = (nCtrX2 >= nCtrX1 ? nXMax1 - nXMin2 : nXMin1 - nXMax2);
				nDY = (nCtrY2 >= nCtrY1 ? nYMax1 - nYMin2 : nYMin1 - nYMax2);

				if (nCtrX2 >= nXMin1 && nCtrX2 <= nXMax1 &&
					nCtrY2 >= nYMin1 && nCtrY2 <= nYMax1)
				{	// Si le centre du vaisseau du héros est dans l'astéroïde (fin d'invulnérabilité...), mort immédiate.
					Player_Damage(SLUG_ENERGY_MAX);
				}
				else
				if (nCtrX2 >= nXMin1 && nCtrX2 <= nXMax1)
				{	// Recalage Y.
					gShoot.nPlayerPosY += nDY;
				}
				else
				if (nCtrY2 >= nYMin1 && nCtrY2 <= nYMax1)
				{	// Recalage X.
					gShoot.nPlayerPosX += nDX;
				}
				else
				{	// On est dans les "coins externe" de la box de col (ni entre les x, ni entre les y). => Recalage du sens le plus proche.
					if (ABS(nDY) <= ABS(nDX))
						gShoot.nPlayerPosY += nDY;
					else
						gShoot.nPlayerPosX += nDX;
				}

				// Si on s'est fait pousser hors de l'écran, mort immédiate.
				if (Rocket_EnemyCheckScreenOut()) Player_Damage(SLUG_ENERGY_MAX);
			}
		}

	}

	// Affichage.
	if (pSpe->nHitCnt) pSpe->nHitCnt--;
	SprDisplay((nSprDisp | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0)) ^ (pMst->nFlipX ? SPR_Flip_X : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies + 32+6 - (pSpe->nOrder * 2));	// + gnMstPrio => non pour ce monstre.
	SprDisplay(((nSprDisp + 1) | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0)) ^ (pMst->nFlipX ? SPR_Flip_X : 0), pSpe->nPosX2 >> 8, pMst->nPosY >> 8, e_Prio_Ennemies  + +32+5 - (pSpe->nOrder*2));	// + gnMstPrio => non pour ce monstre.

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 38 : Big Jellyfish (half-boss).

/*
[mst38] Big Jellyfish (half-boss)
Wait itm = 0:7:			; N° item de quête attendu pour démarrer.
Give itm = 8:15:		; N° item de quête à donner une fois mort pour libérer le scroll.
*/

// Phases.
enum
{
	e_Mst38_BigJellyfish_WaitItem = 0,
	e_Mst38_BigJellyfish_Stop,
	e_Mst38_BigJellyfish_JfishRing,
	e_Mst38_BigJellyfish_Move,

};

// Structure spécifique.
#define	MST38_SAT_OFFSET	48
#define	MST38_SAT_NB		8
#define	MST38_WAVES_NB		3
#define	MST38_WAVES_MULT	48//56
#define	MST38_STOP_FRM		(196 - (MST38_WAVES_NB * MST38_WAVES_MULT))
struct SMst38_BigJellyfish0
{
	u8	nItemWait;	// N° item de quête attendu pour démarrer.
	u8	nItemGive;	// N° item de quête à donner une fois mort pour libérer le scroll.

	u8	nHitCnt, nLife;

	u8	nAngle;		// Angle pour faire tourner les "satellites".
	u16	nSatPres;	// Bits pour la présence des satellites. 0 = Tous morts.
	u8	pnSatLife[MST38_SAT_NB];	// Vie des différents satellites.
	u8	pnSatHitCnt[MST38_SAT_NB];	// HitCnt des différents satellites.
	u8	nOffset;
	u8	nWavesNb;		// Nb de cercles successifs de méduses.

	s32	nTargetX, nTargetY;		// Cible du déplacement.
	u8	nStopWait;				// Attente après déplacement.

};

void Mst38_Init_BigJellyfish0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst38_BigJellyfish0	*pSpe = (struct SMst38_BigJellyfish0 *)pMst->pData;
	u32	nVal;

	nVal = GetBits(0, 7, pData, 0);		// N° item de quête attendu pour démarrer.
	pSpe->nItemWait = nVal;
	nVal = GetBits(8, 15, pData, 0);	// N° item de quête à donner une fois mort pour libérer le scroll.
	pSpe->nItemGive = nVal;

	pSpe->nHitCnt = 0;
	pSpe->nLife = 20;
	pSpe->nWavesNb = MST38_WAVES_NB;

	pSpe->nAngle = 0;

	pMst->nPhase = e_Mst38_BigJellyfish_WaitItem;

}

// Initialisation des satellites.
void Mst38_sub_SatellitesInit(struct SMst38_BigJellyfish0 *pSpe, u32 nOffset)
{
	u32	i;

	// Init des satellites.
	for (i = 0; i < MST38_SAT_NB; i++)
	{
		pSpe->pnSatLife[i] = 4;
		pSpe->pnSatHitCnt[i] = 0;
	}
	pSpe->nSatPres = (1 << MST38_SAT_NB) - 1;	// Bits à 1 pour les présents.
	pSpe->nOffset = nOffset;

}

s32 Mst38_Main_BigJellyfish0(struct SMstCommon *pMst)
{
	struct SMst38_BigJellyfish0	*pSpe = (struct SMst38_BigJellyfish0 *)pMst->pData;
	u32	nSpr;
	u32	nTouch = 0;
	struct SSprRect sRect1;

	switch (pMst->nPhase)
	{
	case e_Mst38_BigJellyfish_WaitItem:		// Attend une item de quête avant d'apparaître.
		if (gpMstQuestItems[pSpe->nItemWait])
		{
			// Init des satellites.
			Mst38_sub_SatellitesInit(pSpe, MST38_SAT_OFFSET);
			// Position. (On pourrait l'initialiser de l'autre côté en fct de la pos du joueur).
			pMst->nPosX = gScrollPos.nPosX + ((SCR_Width + 64) << 8);
			pMst->nPosY = gScrollPos.nPosY + ((SCR_Height / 2) << 8);
			pMst->nSpd = 0;
			// Init move.
			pMst->nPhase = e_Mst38_BigJellyfish_Move;
			pSpe->nTargetX = gScrollPos.nPosX + ((2 * SCR_Width / 3) << 8);
			pSpe->nTargetY = pMst->nPosY;
		}
		else
		{
			// Sortie de l'écran ?
			if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);
		}
		return (e_MstState_Managed);
		break;

	case e_Mst38_BigJellyfish_Stop:		// Attente.
		nTouch = 1;
		// Déclenchement du mvt (si plus de satellites, on skippe la phase de ring).
		if (MST_SHOT_COND)	// Si le héros n'est pas mort...
		if (--pSpe->nStopWait == 0)
		{
			pMst->nPhase = (pSpe->nSatPres ==  0 ? e_Mst38_BigJellyfish_Move : e_Mst38_BigJellyfish_JfishRing);
			// On note la cible.
			pSpe->nTargetX = pMst->nPosX + (((32 - (pSpe->nWavesNb * 4)) * (gShoot.nPlayerPosX - pMst->nPosX)) / 32);
			pSpe->nTargetY = pMst->nPosY + (((32 - (pSpe->nWavesNb * 4)) * (gShoot.nPlayerPosY - 0x800 - pMst->nPosY)) / 32);
		}
		break;

	case e_Mst38_BigJellyfish_JfishRing:	// Anneau qui grossit/réduit avant la charge.
		if (pSpe->nOffset < MST38_SAT_OFFSET) break;	// Si en phase de régénération, on attend.
		// Grossissement / Réduction.
		pSpe->nOffset = MST38_SAT_OFFSET + (pSpe->nStopWait > 16 ? 32 - pSpe->nStopWait : pSpe->nStopWait);
		// Fini ?
		if (++pSpe->nStopWait > 32)
		{
			pSpe->nOffset = MST38_SAT_OFFSET;
			pMst->nPhase = e_Mst38_BigJellyfish_Move;
		}
		break;

	case e_Mst38_BigJellyfish_Move:		// Déplacement vers sa cible.
		nTouch = 1;
		{
			s32	nIncX, nIncY;
			s32	nSub = (pSpe->nWavesNb ? 0 : 0x40);		// Pour ne pas s'arrêter avant le joueur quand plus de cercles de méduses.
			// Avance vers la destination finale.
			nIncX = (pSpe->nTargetX - pMst->nPosX) / 32;
			nIncY = (pSpe->nTargetY - pMst->nPosY) / 32;
			pMst->nPosX += nIncX;
			pMst->nPosY += nIncY;
			// Arrivé pas trop loin ?
			if (ABS(nIncX) < 0x80 - nSub && ABS(nIncY) < 0x80 - nSub)
			{
				pMst->nPhase = e_Mst38_BigJellyfish_Stop;
				pSpe->nStopWait = MST38_STOP_FRM + (pSpe->nWavesNb * MST38_WAVES_MULT);		// Attente après déplacement.
			}
		}
		break;
	}

	nSpr = AnmGetImage(pMst->nAnm);		// Maître.

	// Affichage des satellites.
	if (pSpe->nSatPres)
	{
		u32	i;
		s32	nOffsX, nOffsY;
		u8	nAng = pSpe->nAngle;
		u32	nOffsImg = (nAng >> 2) % 10;	// Pour anim manuelle des petites méduses.
		for (i = 0; i < MST38_SAT_NB; i++)
		{
			if (pSpe->nSatPres & (1 << i))
			{
				// Position.
				nOffsX = gVar.pCos[nAng] * (s32)pSpe->nOffset;
				nOffsY = gVar.pSin[nAng] * (s32)pSpe->nOffset;
				// Display.
				if (pSpe->pnSatHitCnt[i]) pSpe->pnSatHitCnt[i]--;
				SprDisplay((e_Spr_JellyfishSmall_Swim + nOffsImg) | (pSpe->pnSatHitCnt[i] ? SPR_Flag_HitPal : 0), (pMst->nPosX + nOffsX) >> 8, (pMst->nPosY + nOffsY) >> 8, e_Prio_Ennemies-1);	// + gnMstPrio => non pour ce monstre.
				//tst SprDisplayRotoZoom((e_Spr_JellyfishSmall_Swim + nOffsImg) | (pSpe->pnSatHitCnt[i] ? SPR_Flag_HitPal : 0), (pMst->nPosX + nOffsX) >> 8, (pMst->nPosY + nOffsY) >> 8, e_Prio_Ennemies-1, nAng, 0x100);	// + gnMstPrio => non pour ce monstre.

				// Contact avec le joueur ?
				if (SprGetRect(e_Spr_JellyfishSmall_Swim, e_SprRectZone_RectCol, &sRect1))
				if (sRect1.nType == e_SprRect_Rect)
				if (Enemy_PlayerBlowRect(&sRect1, pMst->nPosX + nOffsX, pMst->nPosY + nOffsY, MST_DAMAGE_BULLET)) goto _38SmallJellyfishDeath;

				//if (nTouch)
				if (nTouch && pSpe->nOffset > (3 * MST38_SAT_OFFSET) / 4)	// (On empêche le toucher trop tôt à cause des bombes).
				{
					// Se prend un tir ?
					if (Mst_ShotCheckLife(e_Spr_JellyfishSmall_Swim, pMst->nPosX + nOffsX, pMst->nPosY + nOffsY, &pSpe->pnSatHitCnt[i], &pSpe->pnSatLife[i]))
					{
_38SmallJellyfishDeath:
						// Mort.
						gShoot.nPlayerScore += 50;	// Score.
						DustSet(gAnm_JellyfishSmall_Death_Dust, pMst->nPosX + nOffsX, pMst->nPosY + nOffsY, e_Prio_DustUnder, 0);
						pSpe->nSatPres &= ~(1 << i);
					}
				}
				else
				{
					// Interception des tirs sans dégats.
					u8	nHitCnt, nLife;
					nLife = 0;
					Mst_ShotCheckLife(e_Spr_JellyfishSmall_Swim, pMst->nPosX + nOffsX, pMst->nPosY + nOffsY, &nHitCnt, &nLife);
				}
			}
			nAng += 256 / 8;
			if (++nOffsImg >= 10) nOffsImg = 0;
		}
//		pSpe->nAngle++;
		pSpe->nAngle += (pSpe->nWavesNb & 1 ? 1 : -1);		// On fait tourner le cercle de méduses dans un sens ou dans l'autre.

		// Ré-init des satellites.
		if (pSpe->nSatPres == 0)
		if (--pSpe->nWavesNb)
			Mst38_sub_SatellitesInit(pSpe, 0);
		// Si l'offset n'est pas au max, on l'incrémente.
		if (pSpe->nOffset < MST38_SAT_OFFSET) pSpe->nOffset++;

		// Tant qu'il y a des satellites, le maître ne se prend pas de dégats.
		FireHitCheck(nSpr, pMst->nPosX, pMst->nPosY, e_ShotOrg_Player, 1);
	}
	else
	{
		// Plus de satellites, dégats sur le maître. (nTouch inutile, puisqu'on ne se retrouvera pas sans satellites ici, et les satellites font le test).
		// Se prend un tir ?
		if (Mst_ShotCheckLife2(nSpr, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife, e_ShotCheck_MinimizedDamage))
		{
			// Mort.
			gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;	// Score.
			DustSet(gAnm_JellyfishBig_Death_Dust, pMst->nPosX, pMst->nPosY, e_Prio_DustUnder, 0);
			gpMstQuestItems[pSpe->nItemGive] = 1;	// Item de quête.
			return (e_MstState_Dead);
		}
	}

	// Contact du corps de la méduse avec le joueur.
	if (SprGetRect(nSpr, e_SprRectZone_RectCol, &sRect1))
	if (sRect1.nType == e_SprRect_Rect)
		Enemy_PlayerBlowRect(&sRect1, pMst->nPosX, pMst->nPosY, MST_DAMAGE_BULLET);

	// Affichage du maître.
	if (pSpe->nHitCnt) pSpe->nHitCnt--;
	SprDisplay(nSpr | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies);	// + gnMstPrio => non pour ce monstre.

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 39 : Sensor Mine (mine aimantée).

/*
[mst39] Sensor Mine
prm = 0:7:			; tmp
*/

// Structure spécifique.
struct SMst39_SensorMine0
{
	u8	nHitCnt, nLife;

};

void Mst39_Init_SensorMine0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst39_SensorMine0	*pSpe = (struct SMst39_SensorMine0 *)pMst->pData;
	pSpe->nHitCnt = 0;
	pSpe->nLife = 4;

}

#define	MST39_OFFSY	14
#define	MST39_ATTK_PERIM	(120 * 120)	//(100 * 100)	//(90 * 90)
s32 Mst39_Main_SensorMine0(struct SMstCommon *pMst)
{
	struct SMst39_SensorMine0	*pSpe = (struct SMst39_SensorMine0 *)pMst->pData;
	u32	nSpr;

	nSpr = AnmGetImage(pMst->nAnm);

	ChaserTarget_AddToList(pMst->nPosX >> 8, pMst->nPosY >> 8);	// Ajout dans la table de visée des missiles.

	// Si le joueur est en slug et dans le périmètre, on se dirige vers le slug.
	struct SSprRect sRect1;
	if (gShoot.nVehicleType >= e_HeroVehicle_SlugBase)
	if (SprGetRect(gShoot.nPlayerSprCol, e_SprRectZone_RectCol, &sRect1))
	{
		s32	nTargetX, nTargetY;
		s32	nDX, nDY;

		nTargetX = gShoot.nPlayerPosX + ((sRect1.nX1 + ((sRect1.nX2 - sRect1.nX1) / 2)) << 8);
		nTargetY = gShoot.nPlayerPosY + ((sRect1.nY1 + ((sRect1.nY2 - sRect1.nY1) / 2)) << 8);
		nDX = (nTargetX - pMst->nPosX) >> 8;
		nDY = (nTargetY - pMst->nPosY) >> 8;
		if ((nDX * nDX) + (nDY * nDY) <= MST39_ATTK_PERIM)
		{
			u8	nAng;

//SprDisplay(e_Spr_Tstrct_Cross, nTargetX >> 8, nTargetY >> 8, e_Prio_HUD);	//debug
			nAng = fatan2(-(nTargetY - pMst->nPosY), nTargetX - pMst->nPosX);
			pMst->nPosX += gVar.pCos[nAng] / 2;
			pMst->nPosY += gVar.pSin[nAng] / 2;
		}
	}

	// Contact avec le joueur ? => Explosion.
	if (SprCheckColBox(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8,
		gShoot.nPlayerSprCol, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8))
	{
		FireAdd(e_Shot_Enemy_RShobu_Bomb_Explo, pMst->nPosX, pMst->nPosY + (MST39_OFFSY << 8), -1);
		return (e_MstState_Dead);
	}

	// Se prend un tir ?
	if (Mst_ShotCheckLife(nSpr, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife))
	{
		// Mort.
		gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;	// Score.
		DustSet(gAnm_Explosion0_Medium_Dust, pMst->nPosX, pMst->nPosY + (MST39_OFFSY << 8), e_Prio_DustUnder, 0);
		return (e_MstState_Dead);
	}

	// Affichage.
	if (pSpe->nHitCnt) pSpe->nHitCnt--;
	SprDisplay(nSpr | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies);	// + gnMstPrio => non pour ce monstre.

	return (e_MstState_Managed);
}

//=============================================================================

/*
struct SAAD { u8	nTb0[80]; };
assert(sizeof(struct SAAD) < MST_COMMON_DATA_SZ);
*/

#ifndef NDEBUG
// Debug, vérification de la taille des structures.
void Mst30CheckStructSizes(void)
{
	assert(sizeof(struct SMst30_SquidGenerator0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst31_Squid0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst32_ScrollCtrl) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst33_L11Generator0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst34_L11MarsEye0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst35_L11SpaceRocks0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst36_L11Asteroid0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst37_L11BigAsteroid0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst38_BigJellyfish0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst39_SensorMine0) < MST_COMMON_DATA_SZ);
}
#endif

