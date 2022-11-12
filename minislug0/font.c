
#include "includes.h"

// itoa.
void MyItoA(s32 nNb, char *pDst)
{
	char	cMin = ' ';
	char	*pPtr;
	u32	nTmp;

	// Cas des nombres négatifs.
	if (nNb < 0)
	{
		cMin = '-';
		nNb = -nNb;
	}

	pPtr = pDst + strlen(pDst) - 1;
	nTmp = nNb;
	do
	{
		*pPtr-- = (char)((nTmp % 10) + '0');
	} while (pPtr >= pDst && (nTmp /= 10) > 0);

	// Négatifs.
	if (cMin != ' ' && pPtr >= pDst) *pPtr = cMin;

}


// Affichage d'une phrase en sprites.
// Renvoie la largeur en pixels de la phrase.
u32 Font_Print(s32 nPosX, s32 nPosY, char *pStr, u32 nFlags)
{
	char	cChr;
	struct SSprite	*pSpr;
	s32	nPosXOrg = nPosX;
	u32	nSprHiglight = (nFlags & FONT_Highlight ? SPR_Flag_HitPal : 0);

	while (*pStr)
	{
		cChr = *pStr++;
		cChr -= ' ';
		if (cChr != 0)
		{
			// Char normal.
			pSpr = SprGetDesc(e_Spr_FontSmall - 1 + cChr);
//			if ((nFlags & FONT_NoDisp) == 0) SprDisplayAbsolute(e_Spr_FontSmall - 1 + cChr, nPosX, nPosY, 0x100 + e_Prio_HUD);
			if ((nFlags & FONT_NoDisp) == 0) SprDisplayAbsolute((e_Spr_FontSmall - 1 + cChr) | nSprHiglight, nPosX, nPosY, 0x100 + e_Prio_HUD);
		}
		else
		{
			// Espace, on avance de la taille d'un 'I'.
			pSpr = SprGetDesc(e_Spr_FontSmall - 1 + 'I' - ' ');
		}
		nPosX += pSpr->nLg + 1;
	}

	return ((u32)(nPosX - nPosXOrg));
}

// Affichage d'une phrase en sprites, espacement entre les lettres fixé.
// Renvoie la largeur en pixels de la phrase.
u32 Font_PrintSpc(s32 nPosX, s32 nPosY, char *pStr, u32 nFlags, u32 nSpc)
{
	char	cChr;
	s32	nPosXOrg = nPosX;
	u32	nSprHiglight = (nFlags & FONT_Highlight ? SPR_Flag_HitPal : 0);

	while (*pStr)
	{
		cChr = *pStr++;
		cChr -= ' ';
		if (cChr != 0)
		{
			// Char normal.
//			if ((nFlags & FONT_NoDisp) == 0) SprDisplayAbsolute(e_Spr_FontSmall - 1 + cChr, nPosX, nPosY, 0x100 + e_Prio_HUD);
			if ((nFlags & FONT_NoDisp) == 0) SprDisplayAbsolute((e_Spr_FontSmall - 1 + cChr) | nSprHiglight, nPosX + (cChr == '1' - ' ' ? 2 : 0), nPosY, 0x100 + e_Prio_HUD);
		}
		nPosX += nSpc;
	}

	return ((u32)(nPosX - nPosXOrg));
}

