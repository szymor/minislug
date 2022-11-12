
#include "includes.h"


// Constructeur.
struct SInfoBox * InfoBox_New(void)
{
	struct SInfoBox *psInfoBox;

	if ((psInfoBox = (struct SInfoBox *)malloc(sizeof(struct SInfoBox))) == NULL)
	{
		fprintf(stderr, "InfoBox_New(): malloc failed (psInfoBox).\n");
		exit(1);
	}

	// Init.
	InfoBox_ResetValues(psInfoBox);

	return (psInfoBox);
}

// Destructeur.
void InfoBox_Delete(struct SInfoBox *psInfoBox)
{

	free(psInfoBox);
}

// RAZ des valeurs d'info box.
void InfoBox_ResetValues(struct SInfoBox *psInfoBox)
{
	psInfoBox->nMouseOnMonster = 0;
	psInfoBox->pMstBuf[0] = 0;
	psInfoBox->pPln = NULL;
	psInfoBox->pMstCur = NULL;
	psInfoBox->nLnSel = -1;
	psInfoBox->nLnFirst = 0;

}


#define	MST_InfoBox_Lg	(11+1)
#define	MST_InfoBox_Ht	(9)
#define MSTTXT_TitleBar_Sz	(23)
#define MSTTXT_PrmName_Sz	(9)
#define MSTTXT_PrmVal_Sz	(13)
// Trace la boîte d'info d'un monstre.
//
// Attention, les valeurs pPln et pMstBuf doivent obligatoirement être initialisées.
// Pas obligatoire pour pMstCur et nLnSel, mais bien appeler ResetValues() avant pour
// mettre pMstCur à NULL et nLnSel à -1.
//
// Si pMstCur == NULL, info seulement (souris sur les plans).
// Si pMstCur != NULL, on donne les infos du monstre.
// Si nLnSel == -1, pas de sélection, sinon ligne en cours.
void InfoBox_MstInfoBoxDraw(struct SInfoBox *psInfoBox)
{
	u32	ix, iy;
	u32	nBlkX, nBlkY;
	struct SMouse	*psMouse = gMisc.psMouse;

	nBlkX = psMouse->nPixPosX / 16;
	nBlkY = (psMouse->nPixPosY / 16) + 1;

	// Position de la boîte.
	// Trop à droite ?
	if ((nBlkX + MST_InfoBox_Lg) * 16 > (u32)gMisc.pScreen->w)
	{
		nBlkX -= MST_InfoBox_Lg;
	}
	// Trop bas ?
	if ((nBlkY + MST_InfoBox_Ht) * 16 > (u32)gMisc.pScreen->h)
	{
		nBlkY -= MST_InfoBox_Ht + 1;
	}

	// La boîte.
	SDL_LockSurface(gMisc.pScreen);
	// Barre de titre.
	for (ix = 0; ix < MST_InfoBox_Lg; ix++)
	{
		Gfx_DrawCheckBlock(e_CheckBlk_RedLight, nBlkX + ix, nBlkY, false);
	}
	// Corps de la boîte.
	for (iy = 1; iy < MST_InfoBox_Ht; iy++)
	{
		for (ix = 0; ix < MST_InfoBox_Lg; ix++)
		{
			//Gfx_DrawCheckBlock(e_CheckBlk_RedDark, nBlkX + ix, nBlkY + iy, false);
			Gfx_DrawCheckBlock(((psInfoBox->nLnFirst + iy + (ix <= 4)) & 1) ? e_CheckBlk_RedDark : e_CheckBlk_RedDark2, nBlkX + ix, nBlkY + iy, false);
		}
	}
	// Barre de selection.
	if (psInfoBox->nLnSel != -1)
	{
		for (ix = 0; ix < MST_InfoBox_Lg; ix++)
		{
			Gfx_DrawCheckBlock(e_CheckBlk_RedLight, nBlkX + ix, nBlkY + 1 + psInfoBox->nLnSel - psInfoBox->nLnFirst, false);
		}
	}
	SDL_UnlockSurface(gMisc.pScreen);

	// Le texte.
	// Barre de titre.
	char	pName[1024];
	char	pEndOfLine[] = "\n";
	char	*pDst;
	if ((pDst = strstr(psInfoBox->pMstBuf, pEndOfLine)) != NULL)
	{
		iy = pDst - psInfoBox->pMstBuf;
		strncpy(pName, psInfoBox->pMstBuf, iy);
		pName[iy] = 0;
//		while (iy && (pName[iy - 1] == '\r' || pName[iy - 1] == ' ')) pName[--iy] = 0;	// Suppression 0x0D (win) et espaces de fin de ligne. => 0x0D fait dans CmdMstSeekInFile().
		while (iy && pName[iy - 1] == ' ') pName[--iy] = 0;		// Suppression des espaces de fin de ligne.
		pName[MSTTXT_TitleBar_Sz] = 0;	// Limite la taille d'affichage.
		if (iy > MSTTXT_TitleBar_Sz) pName[MSTTXT_TitleBar_Sz - 1] = 0x7F;	// "..." si trop long.
		Font_Printf((nBlkX * 16) + 4, (nBlkY * 16) + 4, pName, 0xFFFF);
	}
	// Paramètres.
//-	u32	nNbPrm = pInfoBox->pPln->_pMst->CmdMstGetPrmNb(pInfoBox->pMstBuf);
	u32	nNbPrm = CmdMstGetPrmNb(psInfoBox->pMstBuf);
	for (iy = 0; iy < nNbPrm && iy < MST_InfoBox_Ht - 1; iy++)
	{
		char	pPrm[1024];
//-		if (pInfoBox->pPln->_pMst->CmdMstGetPrmNo(pInfoBox->pMstBuf, pInfoBox->nLnFirst + iy, pPrm))
		if (CmdMstGetPrmNo(psInfoBox->pMstBuf, psInfoBox->nLnFirst + iy, pPrm))
		{
			u32	nPrmBitStart, nPrmBitEnd, nPrmSign;
			char	pPrmName[1024];
			char	pPrmArgs[1024];
			u32	nSplit;

//-			nSplit = pInfoBox->pPln->_pMst->CmdPrmSplit(pPrm, pPrmName, &nPrmBitStart, &nPrmBitEnd, &nPrmSign, pPrmArgs);
			nSplit = CmdPrmSplit(pPrm, pPrmName, &nPrmBitStart, &nPrmBitEnd, &nPrmSign, pPrmArgs);
			if (nSplit >= CMD_PRM_SPLIT_MIN)
			{
				// Le nom du paramètre.
				if (pPrmName[MSTTXT_PrmName_Sz - 1] && pPrmName[MSTTXT_PrmName_Sz]) pPrmName[MSTTXT_PrmName_Sz - 1] = 0x7F;	// "..." si trop long.
				pPrmName[MSTTXT_PrmName_Sz] = 0;	// Limite la taille d'affichage.
				Font_Printf((nBlkX * 16) + 4, ((nBlkY + 1 + iy) * 16) + 4, pPrmName, 0xFFFF);

				// La valeur s'il y a un monstre.
				if (psInfoBox->pMstCur != NULL)
				{
					char	pArg[1024];
					s32	nVal;

					nVal = GetBits(psInfoBox->pMstCur->pData, nPrmBitStart, nPrmBitEnd, nPrmSign);
					if (nSplit == 3)
					{
						// Valeur numérique.
						Font_StrReset(&gFont);
						Font_StrItoa(&gFont, nVal);
						Font_Printf((nBlkX * 16) + 4 + (10 * 8), ((nBlkY + 1 + iy) * 16) + 4, gFont.pString, 0xFFFF);
					}
					else
					{
						// Argument.
//-						if (pInfoBox->pPln->_pMst->CmdMstGetArgNo(pPrmArgs, nVal, pArg) == 0) strcpy(pArg, "undefined");
						if (CmdMstGetArgNo(pPrmArgs, nVal, pArg) == 0) strcpy(pArg, "undefined");
						if (pArg[MSTTXT_PrmVal_Sz - 1] && pArg[MSTTXT_PrmVal_Sz]) pArg[MSTTXT_PrmVal_Sz - 1] = 0x7F;	// "..." si trop long.
						pArg[MSTTXT_PrmVal_Sz] = 0;	// Limite la taille d'affichage.
						Font_Printf((nBlkX * 16) + 4 + (10 * 8), ((nBlkY + 1 + iy) * 16) + 4, pArg, 0xFFFF);
					}
				}
			}
		}

	}

	// '...' au dessus et en dessous si trop de prm.
	if (psInfoBox->nLnFirst)
	{
		Font_Printf(((nBlkX) * 16) + 4, ((nBlkY + 1) * 16) + 4-8, "\x7F", 0xFFFF);
		Font_Printf(((nBlkX + MST_InfoBox_Lg - 8) * 16) + 4+8, ((nBlkY + 1) * 16) + 4-8, "\x7F", 0xFFFF);
		Font_Printf(((nBlkX + MST_InfoBox_Lg - 1) * 16) + 4, ((nBlkY + 1) * 16) + 4-8, "\x7F", 0xFFFF);
	}
	if (psInfoBox->nLnFirst + MST_InfoBox_Ht - 1 < nNbPrm)
	{
		Font_Printf(((nBlkX) * 16) + 4, ((nBlkY + MST_InfoBox_Ht - 1) * 16) + 4+4, "\x7F", 0xFFFF);
		Font_Printf(((nBlkX + MST_InfoBox_Lg - 8) * 16) + 4+8, ((nBlkY + MST_InfoBox_Ht - 1) * 16) + 4+4, "\x7F", 0xFFFF);
		Font_Printf(((nBlkX + MST_InfoBox_Lg - 1) * 16) + 4, ((nBlkY + MST_InfoBox_Ht - 1) * 16) + 4+4, "\x7F", 0xFFFF);
	}

}


// Infos des monstres, test des conditions, si ok, remplit les infos + lance l'affichage.
void InfoBox_MstInfoDisplay(struct SInfoBox *psInfoBox)
{
	struct SMap		*psMap = gMisc.psMap;
	struct SMouse	*psMouse = gMisc.psMouse;

	InfoBox_ResetValues(psInfoBox);	// !!! TRES IMPORTANT !!!

	// Slt quand la souris est idle.
	if (psMouse->nState != e_MouseState_Idle) return;
	// Pas si la souris est n'importe où.
	if (psMouse->pWhere == NULL || psMouse->nBlkPosX == -1) return;

	if (psMouse->pWhere->nWhere == e_MouseLoc_Planes)
	{
		// Dans les plans.

		// Pas si le plan courant n'a pas de fichier de commandes ou s'il est grisé.
//-		if ((*(psMap->ppsPlanes + psMouse->pWhere->nVar))->HasCmd() == 0) return;
//-		if ((*(psMap->ppsPlanes + psMouse->pWhere->nVar))->IsHidden()) return;
		if (Plane_HasCmd(*(psMap->ppsPlanes + psMouse->pWhere->nVar)) == 0) return;
		if (Plane_IsHidden(*(psMap->ppsPlanes + psMouse->pWhere->nVar))) return;

		psInfoBox->pPln = *(psMap->ppsPlanes + psMouse->pWhere->nVar);

		// Un monstre défini dans le cmd.txt sur le bloc pointé par la souris ?
//-		if (psInfoBox->pPln->_pMst->CmdMstSeek((psMouse->nBlkPosY * (psInfoBox->pPln->_pGfx->w / 16)) + psMouse->nBlkPosX, psInfoBox->pMstBuf) == 0) return;
		if (Cmdfile_CmdMstSeek(psInfoBox->pPln->pMst, (psMouse->nBlkPosY * (psInfoBox->pPln->pGfx->w / 16)) + psMouse->nBlkPosX, psInfoBox->pMstBuf) == 0) return;

		InfoBox_MstInfoBoxDraw(psInfoBox);	// pMstBuf / pPln / pMstCur=NULL / nLnSel=-1.

	}
	else
	if (psMouse->pWhere->nWhere == e_MouseLoc_Map)
	{
		// Sur la map.

		// Pas si le plan courant n'a pas de fichier de commandes ou s'il est grisé.
//-		if ((*(psMap->ppsPlanes + psMap->nActivePlane))->HasCmd() == 0) return;
//-		if ((*(psMap->ppsPlanes + psMap->nActivePlane))->IsHidden()) return;
		if (Plane_HasCmd(*(psMap->ppsPlanes + psMap->nActivePlane)) == 0) return;
		if (Plane_IsHidden(*(psMap->ppsPlanes + psMap->nActivePlane))) return;

		psInfoBox->pPln = *(psMap->ppsPlanes + psMap->nActivePlane);

		// Un monstre défini dans le cmd.txt sur le bloc pointé par la souris ?
		if (*(psInfoBox->pPln->pPlane + (psMouse->nBlkPosY * psMap->nMapLg) + psMouse->nBlkPosX) == -1) return;
		if (*(psInfoBox->pPln->ppMstPlane + (psMouse->nBlkPosY * psMap->nMapLg) + psMouse->nBlkPosX) == NULL) return;

		psInfoBox->pMstCur = *(psInfoBox->pPln->ppMstPlane + (psMouse->nBlkPosY * psMap->nMapLg) + psMouse->nBlkPosX);
// On peut faire le test suivant au cas ou :
//	if (*(pPln->pPlane + (psMouse->nBlkPosY * nMapLg) + psMouse->nBlkPosX) != pMstCur->nMstNo) return;
//-		if (pInfoBox->pPln->_pMst->CmdMstSeek(pInfoBox->pMstCur->nMstNo, pInfoBox->pMstBuf) == 0) return;
		if (Cmdfile_CmdMstSeek(psInfoBox->pPln->pMst, psInfoBox->pMstCur->nMstNo, psInfoBox->pMstBuf) == 0) return;

		InfoBox_MstInfoBoxDraw(psInfoBox);	// pMstBuf / pPln / pMstCur / nLnSel=-1.

		// Note les valeurs utiles pour une éventuelle édition du monstre.
		psInfoBox->nMouseOnMonster = 1;	// Flag : Le curseur pointe un monstre éditable.

	}

}

// Déplacement du sélecteur.
// (Evidement, il faut être en mode édition !).
void InfoBox_SelMove(struct SInfoBox *psInfoBox, s32 nVal)
{
	if (psInfoBox->nMouseOnMonster == 0) return;
	if (psInfoBox->pPln == NULL || psInfoBox->pMstCur == NULL) return;

	if (nVal < 0)
	{
		psInfoBox->nLnSel += nVal;
		if (psInfoBox->nLnSel < 0) psInfoBox->nLnSel = 0;
		if (psInfoBox->nLnSel < (s32)psInfoBox->nLnFirst) psInfoBox->nLnFirst = psInfoBox->nLnSel;
	}
	else
	{
//-		u32	nNbPrm = psInfoBox->pPln->_pMst->CmdMstGetPrmNb(psInfoBox->pMstBuf);
		u32	nNbPrm = CmdMstGetPrmNb(psInfoBox->pMstBuf);
		if (nNbPrm == 0) return;
		psInfoBox->nLnSel += nVal;
		if (psInfoBox->nLnSel > (s32)nNbPrm - 1) psInfoBox->nLnSel = nNbPrm - 1;
		if (psInfoBox->nLnSel - psInfoBox->nLnFirst > MST_InfoBox_Ht - 2) psInfoBox->nLnFirst = psInfoBox->nLnSel - (MST_InfoBox_Ht - 2);
	}

}

// Changement de la valeur de l'item sélectionné.
// (Ici aussi, il faut être en mode édition !).
void InfoBox_SelChange(struct SInfoBox *psInfoBox, s32 nVal)
{
	if (psInfoBox->nMouseOnMonster == 0) return;
	if (psInfoBox->pPln == NULL || psInfoBox->pMstCur == NULL) return;

	char	pPrm[1024];
//-	if (pInfoBox->pPln->_pMst->CmdMstGetPrmNo(pInfoBox->pMstBuf, pInfoBox->nLnSel, pPrm))
	if (CmdMstGetPrmNo(psInfoBox->pMstBuf, psInfoBox->nLnSel, pPrm))
	{
		u32	nPrmBitStart, nPrmBitEnd, nPrmSign;
		char	pPrmName[1024];
		char	pPrmArgs[1024];
		u32	nSplit;

//-		nSplit = psInfoBox->pPln->_pMst->CmdPrmSplit(pPrm, pPrmName, &nPrmBitStart, &nPrmBitEnd, &nPrmSign, pPrmArgs);
		nSplit = CmdPrmSplit(pPrm, pPrmName, &nPrmBitStart, &nPrmBitEnd, &nPrmSign, pPrmArgs);
		if (nSplit >= CMD_PRM_SPLIT_MIN)
		{
			s32	nCurVal;

			nCurVal = GetBits(psInfoBox->pMstCur->pData, nPrmBitStart, nPrmBitEnd, nPrmSign);
			nCurVal += nVal;
			SetBits(psInfoBox->pMstCur->pData, nPrmBitStart, nPrmBitEnd, nCurVal);
		}
	}

}

