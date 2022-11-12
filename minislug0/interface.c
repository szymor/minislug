
#include "includes.h"

//=============================================================================
// HUD.

#ifdef DEBUG_DISP
extern void anm_tst_dispnb(u32 nPosY);
extern void mst_tst_dispnb(u32 nPosY);
extern void shot_tst_dispnb(u32 nPosY);
extern void sfx_tst_dispnb(u32 nPosY);
#endif

// Affichage du HUD.
#define	HUD_ARMSFRAME_POSX	95	//100
#define	HUD_ARMSFRAME_POSY	12
#define	HUD_GAUGE_POSX	(HUD_ARMSFRAME_POSX - 86)
#define	HUD_GAUGE_POSY	(HUD_ARMSFRAME_POSY + 6)
#define	HUD_HEAD_POSX	(HUD_GAUGE_POSX)
#define	HUD_HEAD_POSY	(HUD_GAUGE_POSY + 22)
#define	HUD_PRISONER_POSX	4
#define	HUD_PRISONER_POSY	(SCR_Height - 4)
#define	HUD_SPR_PRIO	(0x100 + e_Prio_HUD)
void HUDDisplay(void)
{
	char	pTbConv[8+1] = "        ";
	char	*pTbPtr;
	s32	i;
	s32	nPosX;

	// Cadre ammo/bombs.
	SprDisplayAbsolute(e_Spr_HUD_ArmsFrameTopLeft, HUD_ARMSFRAME_POSX, HUD_ARMSFRAME_POSY, HUD_SPR_PRIO - 1);
	SprDisplayAbsolute(e_Spr_HUD_ArmsFrameTopRight, HUD_ARMSFRAME_POSX, HUD_ARMSFRAME_POSY, HUD_SPR_PRIO - 1);
	SprDisplayAbsolute(e_Spr_HUD_ArmsFrameBottom, HUD_ARMSFRAME_POSX, HUD_ARMSFRAME_POSY, HUD_SPR_PRIO - 1);

	// Nombre de balles.
	i = (gShoot.nVehicleType < e_HeroVehicle_SlugBase ? gShoot.nAmmo : gShoot.nVehicleAmmo);
	if (i >= 0)
	{
		MyItoA(i, pTbConv);
		Font_PrintSpc(HUD_ARMSFRAME_POSX - 24, HUD_ARMSFRAME_POSY + 7, pTbConv + 5, 0, 8);
	}
	else
		SprDisplayAbsolute(e_Spr_FontSmall + 94, HUD_ARMSFRAME_POSX - 24 + 4, HUD_ARMSFRAME_POSY + 7, HUD_SPR_PRIO);	// Infini.
	// Nombre de bombes.
	memset(pTbConv, ' ', 8);
	MyItoA((gShoot.nVehicleType < e_HeroVehicle_SlugBase ? gShoot.nBombAmmo : gShoot.nVehicleBombAmmo), pTbConv);
	Font_PrintSpc(HUD_ARMSFRAME_POSX + 4, HUD_ARMSFRAME_POSY + 7, pTbConv + 5, 0, 8);

	// Score.
	memset(pTbConv, ' ', 8);
	MyItoA(gShoot.nPlayerScore, pTbConv);
	Font_PrintSpc(HUD_GAUGE_POSX - 13, HUD_GAUGE_POSY - 7, pTbConv, 0, 8);

	// Vies.
	SprDisplayAbsolute(e_Spr_HUD_HeroHeads, HUD_HEAD_POSX, HUD_HEAD_POSY, HUD_SPR_PRIO);	// Si plusieurs persos, e_Spr_HUD_HeroHeads + <hero n°>.
	memset(pTbConv, ' ', 8);
	MyItoA(gShoot.nPlayerLives, pTbConv);
	pTbPtr = &pTbConv[1];	// Au cas ou, pour le 'x'.
	while (*pTbPtr == ' ') pTbPtr++;
	*(--pTbPtr) = 'x';
	//Font_PrintSpc(HUD_HEAD_POSX + 22, HUD_HEAD_POSY, pTbPtr, 0, 8);
	if (gShoot.nHUDPlayerLivesBlink) gShoot.nHUDPlayerLivesBlink--;	// Pour clignotement quand 1UP.
	Font_PrintSpc(HUD_HEAD_POSX + 22, HUD_HEAD_POSY, pTbPtr, ((gShoot.nHUDPlayerLivesBlink & 8) ? FONT_Highlight : 0), 8);

	// La jauge.
	SprDisplayAbsolute(e_Spr_HUD_SlugGauge, HUD_GAUGE_POSX, HUD_GAUGE_POSY, HUD_SPR_PRIO);
	// Dans un slug ? => Barre d'énergie.
	if (gShoot.nVehicleType >= e_HeroVehicle_SlugBase)
	{
		// Les points dedans.
		nPosX = HUD_GAUGE_POSX;
		i = gShoot.nVehicleEnergy;// >> 2;
		for (; i > 0; i -= 8, nPosX += 8)
			SprDisplayAbsolute(e_Spr_HUD_SlugGauge + 0 + (i > 7 ? 8 : i), nPosX, HUD_GAUGE_POSY, HUD_SPR_PRIO + 1);
	}

	// Nb de prisonniers libérés en bas.
	for (i = 0; i < gShoot.nFreedPrisoners; i++)
		SprDisplayAbsolute(e_Spr_HUD_Prisoner, HUD_PRISONER_POSX + (i * 7), HUD_PRISONER_POSY, HUD_SPR_PRIO);


#ifdef DEBUG_DISP
//anm_tst_dispnb(64);		// nb d'anims.
//mst_tst_dispnb(64+8);	// nb de monstres.
//shot_tst_dispnb(64+16);	// nb de tirs.
//sfx_tst_dispnb(64);
#endif

}


//=============================================================================
// Affichage du "Mission x Start!".

// Grosse fonte : 25 x 25 avec 1 pix d'ombre en bas et à droite.

#define	MSE_TXT_LEN	11
#define	MSE_LN_NB	2
#define	MSE_SPR_PRIO	(0x100 + e_Prio_HUD - 10)
struct SMissionStartEffect
{
	u32	pSprTb[MSE_LN_NB][MSE_TXT_LEN];
	s16	pPosXEnd[MSE_LN_NB][MSE_TXT_LEN];
	u8	pChrNb[MSE_LN_NB];
	u16	nCnt[3];
	u8	nContinueSec;
	u8	nContinueFrm;
	u8	nEnd;
};
struct SMissionStartEffect	gMSE;

// Récupère le nombre de caractères réel d'une phrase et note la pos x de chaque car pour effet.
u32 MSE_sub_CalcTxtPos(char *pStr, s16 *pPosXTb, u32 *pSprTb, u8 *pChrNb)
{
	u32	cChr;
	struct SSprite	*pSpr;
	s32	nPosX = 0;

	*pChrNb = 0;	// RAZ nb de caractères.
	while (*pStr)
	{
		cChr = *pStr++;
		if (cChr >= 'A' && cChr <= 'Z')
		{
			cChr = cChr - 'A' + e_Spr_FontMission_Letters;
		}
		else
		if (cChr >= '0' && cChr <= '9')
		{
			cChr = cChr - '0' + e_Spr_FontMission_Figures;
		}
		else
		if (cChr == '!')
		{
			cChr = e_Spr_FontMission_Chrs;
		}
		else
		if (cChr == '?')
		{
			cChr = e_Spr_FontMission_Chrs + 1;
		}
		else
			cChr = 0;

		if (cChr)
		{
			// Char normal.
			pSprTb[*pChrNb] = cChr;		// N° du sprite.
			pPosXTb[*pChrNb] = nPosX;
			pSpr = SprGetDesc(pSprTb[*pChrNb]);
			*pChrNb += 1;	// Nb de caractères++.
		}
		else
		{
			// Espace, on avance de la taille d'un 'I'.
			pSpr = SprGetDesc(e_Spr_FontMission_Letters + 'I' - 'A');
		}
		nPosX += pSpr->nLg + 1;
	}

	return ((u32)nPosX);
}

// Affichage du mission start : Préparation.
void MSE_MissionStartReset(u32 nEnd)
{
	char	pMiss[] = "MISSION 00";
	u32	nDiz, nUnit, i, j;
	u32	pLgPix[MSE_LN_NB];

	gMSE.nEnd = nEnd;	// Sauvegarde de nEnd.

	// N° de mission.
	nDiz = gGameVar.nMissionNo / 10;
	nUnit = gGameVar.nMissionNo % 10;
	char *pPtr = strchr(pMiss, '0');
	if (pPtr != NULL)
	{
		if (nDiz) *pPtr++ = nDiz + '0';
		*pPtr++ = nUnit + '0';
		*pPtr = 0;
	}
	// Positions initiales.
	pLgPix[0] = MSE_sub_CalcTxtPos(pMiss, &gMSE.pPosXEnd[0][0], &gMSE.pSprTb[0][0], &gMSE.pChrNb[0]);
	pLgPix[1] = MSE_sub_CalcTxtPos((nEnd ? "COMPLETE!" : "START!"), &gMSE.pPosXEnd[1][0], &gMSE.pSprTb[1][0], &gMSE.pChrNb[1]);
	// Centrage.
	for (j = 0; j < MSE_LN_NB; j++)
	{
		for (i = 0; i < gMSE.pChrNb[j]; i++)
		{
			gMSE.pPosXEnd[j][i] += (SCR_Width - pLgPix[j]) / 2;		// Centrage x.
		}
	}
	gMSE.nCnt[0] = 40;//48;	// Zoom in sur [0;31] + petit délai avant pour la transition.
	gMSE.nCnt[1] = 32+48;//64+48;	// Pause sur l'affichage.
	gMSE.nCnt[2] = 0;	// Zoom out sur [0;31] et quitte.

}

// Affichage du mission start.
// Out : 1 = Terminé.
u32 MSE_MissionStartDisplay(void)
{
	s32	i, j;
	s32	nOffsX, nOffsY, k;
	u32	nNoLn2;
	u32	nRetVal = 0;

	// Gestion de l'effet.
	if (gMSE.nCnt[0]) gMSE.nCnt[0]--;	// Zoom in.
	k = gMSE.nCnt[0];
	if (k > 32) return (0);//k = 32;

	nNoLn2 = 0;
	if (gMSE.nCnt[0] == 0)
	{
		if (gMSE.nCnt[1])
		{
			gMSE.nCnt[1]--;
			if (((gMSE.nCnt[1] + 16) & 16) == 0) nNoLn2 = 1;
		}
		else
		{
			if (gMSE.nEnd)
				nRetVal = 1;	// Cas spécial de la fin de mission, on laisse le texte affiché.
			else
				if (++gMSE.nCnt[2] >= 32) return (1);	// Zoom out.

			k = gMSE.nCnt[2];

// Version originale du bloc :
//			if (++gMSE.nCnt[2] >= 32) return (1);	// Zoom out.
//			k = gMSE.nCnt[2];
		}
	}

	// Affichage.
	for (j = 0; j < MSE_LN_NB - nNoLn2; j++)
	{
		nOffsY = -(((SCR_Height / 2) + 64) * k) / 32;	// +64 /32
		if (j)	// Ligne du bas ?
		{
			nOffsY = -nOffsY;
			nOffsY += (25 * (k * 16)) / 256;	// Compense la taille du zoom (pt de ref sur la ligne du bas).
		}

		for (i = 0; i < gMSE.pChrNb[j]; i++)
		{
			nOffsX = -(((SCR_Width / 2) - gMSE.pPosXEnd[j][i]) * 2 * k) / 32;
			SprDisplayZoomAbsolute(gMSE.pSprTb[j][i], gMSE.pPosXEnd[j][i] + nOffsX, (SCR_Height / 2) - 12 + (j * (25 + 12+12)) + nOffsY, MSE_SPR_PRIO, 0x100 + (k * 16), 0x100 + (k * 16));
		}
	}

//	return (0);
	return (nRetVal);
}

//=============================================================================

// Affichage du "Continue?" : Préparation.
void MSE_ContinueReset(void)
{
	u32	nLgPix;
	u32	i;

	// Positions initiales.
	nLgPix = MSE_sub_CalcTxtPos("CONTINUE?", &gMSE.pPosXEnd[0][0], &gMSE.pSprTb[0][0], &gMSE.pChrNb[0]);
	// Centrage x.
	for (i = 0; i < gMSE.pChrNb[0]; i++)
	{
		gMSE.pPosXEnd[0][i] += (SCR_Width - nLgPix) / 2;
		gMSE.pPosXEnd[1][i] = 32 + (i * 4);		// Timer/compteur pour effet de zoom sur les lettres.
	}

	// Init du timer.
	gMSE.nContinueSec = 9;
	gMSE.nContinueFrm = FRM_1S;

}

// Renvoie 1 quand le compteur est arrivé à 0.
u32 MSE_sub_ContinueCountDown(void)
{
	if (gMSE.nContinueFrm == 0)
	{
		if (gMSE.nContinueSec == 0) return (1);
		gMSE.nContinueSec--;
		gMSE.nContinueFrm = FRM_1S + 1;		// Reset du compteur pour la prochaine seconde.
	}
	gMSE.nContinueFrm--;
	return (0);
}

// Accélère le count down (Bouton B).
void MSE_ContinueCountDown_Faster(void)
{
	gMSE.nContinueFrm = 0;
}

// Teste si le texte a fini d'arriver.
u32 MSE_ContinueTxtInPos(void)
{
	return (gMSE.pPosXEnd[1][gMSE.pChrNb[0] - 1] == 0);
}

// Affichage du "Continue?".
u32 MSE_ContinueDisplay(void)
{
	u32	i;
	u32	nRet = 0;

	for (i = 0; i < gMSE.pChrNb[0]; i++)
	{
//		SprDisplayAbsolute(gMSE.pSprTb[0][i], gMSE.pPosXEnd[0][i], (SCR_Height / 2) - 8, MSE_SPR_PRIO);

		if (gMSE.pPosXEnd[1][i]) gMSE.pPosXEnd[1][i]--;
		if (gMSE.pPosXEnd[1][i] <= 32)
		{
			struct SSprite	*pSpr;
			pSpr = SprGetDesc(gMSE.pSprTb[0][i]);
			SprDisplayZoomAbsolute(gMSE.pSprTb[0][i],
				gMSE.pPosXEnd[0][i] + ( (pSpr->nLg - ( (pSpr->nLg * (0x100 - (gMSE.pPosXEnd[1][i] * 8))) / 256)) / 2),
				(SCR_Height / 2) - 8 - (256 - gVar.pCos[gMSE.pPosXEnd[1][i]]), MSE_SPR_PRIO, 0x100 - (gMSE.pPosXEnd[1][i] * 8), 0x100);
		}
	}

	// Quand toutes les lettres sont arrivées....
//	if (gMSE.pPosXEnd[1][gMSE.pChrNb[0] - 1] == 0)
	if (MSE_ContinueTxtInPos())
	{
		// Compteur.
		nRet = MSE_sub_ContinueCountDown();
		// Les gros chiffres.
		u16	nZoomX = 0x100;
		if (gMSE.nContinueFrm <= 32) nZoomX = gMSE.nContinueFrm * 8;
		SprDisplayZoomAbsolute(e_Spr_FontMission_BigFigures + gMSE.nContinueSec,
			SCR_Width/2, (SCR_Height/2) + 64 - 8, MSE_SPR_PRIO, nZoomX, 0x100);
//			SCR_Width/2, (SCR_Height/2) + 64 - 8 + (256 - gVar.pCos[(gMSE.nContinueFrm <= 32 ? 32 - gMSE.nContinueFrm : 0)]), e_Prio_HUD, nZoomX, 0x100);
	}

	// Les credits restants.
	Credits_Display(gGameVar.nCreditsNb);

	return (nRet);

}

//=============================================================================
// Affichage des bonus de fin de mission.

struct SEndMissionStatus
{
	u32	nNbDisp;	// Nb de prisonniers.
	s8	nPrisonerAnm;
	u8	nPhase;
	u8	nFrmCnt;
	u8	nSinIdx;
	s32	nOffs;		// Décalage pour arrivée/départ.
};
struct SEndMissionStatus	gEMS;

enum
{
	e_EMS_Begin = 0,
	e_EMS_Display,
	e_EMS_Wait,
	e_EMS_End,
};

#define	EMS_CONVTB_SZ	5
#define	EMS_OFFSX	((SCR_Width / 2) - ((48 * 5) / 2))
#define	EMS_OFFSY	((SCR_Height / 2) - (64 / 2))
#define	EMS_SIN_MIN	18
#define	EMS_SIN_MAX	64
#define	EMS_SIN_SPD	2

// Préparation.
void MSE_EndMissionStatusReset(void)
{
	gEMS.nNbDisp = 0;
	gEMS.nPrisonerAnm = AnmSet(gAnm_POW_Idle, -1);
	gEMS.nPhase = e_EMS_Begin;
	gEMS.nOffs = (SCR_Height / 2) + ((64+6+6) / 2);//- SCR_Width - (48 * 2);	// Décalage pour arrivée/départ.
	gEMS.nFrmCnt = 0;
	gEMS.nSinIdx = EMS_SIN_MIN-4;

}

// Affichage. !!! Ne pas interrompre, la fonction update le score !!!
u32 MSE_EndMissionStatusDisplay(void)
{
	char *pRecapturedP = "Recaptured Prisoners:";
	char *pMissionResults = "MISSION RESULTS";

	u32	i;
	u32 nLg;
	s32	nPosX, nPosY;

	switch (gEMS.nPhase)
	{
	case e_EMS_Begin:		// Arrive.
		gEMS.nSinIdx += EMS_SIN_SPD;
		if (gEMS.nSinIdx >= EMS_SIN_MAX)
		{
			gEMS.nPrisonerAnm = AnmSet(gAnm_POW_Salute, gEMS.nPrisonerAnm);//-1);
			gEMS.nPhase = e_EMS_Display;
//gShoot.nFreedPrisoners = 12;	// <<<< test !!!
		}
		break;

	case e_EMS_Display:		// Décompte.
		if ((++gEMS.nFrmCnt & 3) == 0)
		{
			if (gShoot.nFreedPrisoners)
			{
				gShoot.nFreedPrisoners--;
				gShoot.nPlayerScore += 1000;
				gEMS.nNbDisp++;
			}
			else
			if (AnmGetKey(gEMS.nPrisonerAnm) == e_AnmKey_Null)	// Si l'anim de salut est bien terminée.
			{
				gEMS.nFrmCnt = 63+32+15;
				gEMS.nPhase = e_EMS_Wait;
			}
		}
		break;

	case e_EMS_Wait:	// Petite attente avec l'affichage terminé.
		if (--gEMS.nFrmCnt == 0) gEMS.nPhase = e_EMS_End;
		break;

	case e_EMS_End:		// Repart.
		if (gEMS.nSinIdx <= EMS_SIN_MIN) return (1);
		gEMS.nSinIdx -= EMS_SIN_SPD;
		break;
	}

	nPosX = EMS_OFFSX;
	nPosY = EMS_OFFSY + (256 + gVar.pSin[gEMS.nSinIdx]);

	// Affichage de la plaque.
	for (i = 0; i < 5; i++)
	{
		SprDisplayAbsolute(e_Spr_MSE_BarTop, nPosX + (i * 48), nPosY - 6, 0x100 + e_Prio_HUD - 1);
		SprDisplayAbsolute(e_Spr_MSE_Plate, nPosX + (i * 48), nPosY, 0x100 + e_Prio_HUD - 1);
		SprDisplayAbsolute(e_Spr_MSE_BarBottom, nPosX + (i * 48), nPosY + 64, 0x100 + e_Prio_HUD - 1);
	}
	// "Mission results".
	Font_Print(nPosX + 64, nPosY + 15, pMissionResults, 0);
	// Affichage du décompte.
	Font_Print(nPosX + 53, nPosY + 22+16, pRecapturedP, 0);
	char	pTb[EMS_CONVTB_SZ + 1] = "     ";
	MyItoA(gEMS.nNbDisp, &pTb[2]);
//	nLg = Font_PrintSpc(nPosX + 53, nPosY + 22+16 + 11, &pTb[2], 0, 8);
	nLg = Font_PrintSpc(nPosX + 53, nPosY + 22+16 + 11, &pTb[2], ((gEMS.nPhase != e_EMS_Wait || ((gEMS.nFrmCnt + 16) & 16)) ? 0 : FONT_NoDisp), 8);
	nLg += Font_PrintSpc(nPosX + 53 + nLg, nPosY + 22+16 + 11, " x 1000 = ", 0, 8);
	for (i = 0; i < EMS_CONVTB_SZ; i++) pTb[i] = ' ';
	MyItoA(gEMS.nNbDisp * 1000, pTb);
	Font_PrintSpc(nPosX + 53 + nLg, nPosY + 22+16 + 11, pTb, 0, 8);
	// Affichage du prisonnier qui salue.
	i = AnmGetImage(gEMS.nPrisonerAnm);
	SprDisplayAbsolute(i ^ SPR_Flip_X, nPosX + 24, nPosY + 43+16, 0x100 + e_Prio_HUD);

	return (0);
}

