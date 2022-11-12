
#include "includes.h"
#include "font8x8.h"

struct SFont	gFont;

// Reset texte.
void Font_StrReset(struct SFont *psFont)
{
	psFont->nIdx = 0;
	psFont->pString[0] = 0;
}

// Ajout d'une chaîne dans la chaîne locale.
void Font_StrAdd(struct SFont *psFont, char *pStr)
{
	strcpy(&psFont->pString[psFont->nIdx], pStr);
	psFont->nIdx += strlen(pStr);

}

// Ajout d'un entier dans la chaîne locale.
void Font_StrItoa(struct SFont *psFont, s32 nVal)
{
	char	pStr[33];
	//itoa(nVal, pStr, 10);
	snprintf(pStr, sizeof(pStr), "%d", (int)nVal);	// pas de itoa en Linux...
	Font_StrAdd(psFont, pStr);
}

// Conversion d'un u8 en hexa.
void Font_StrU8toAHex(struct SFont *psFont, u8 nVal)
{
	static char pConv[] = "0123456789ABCDEF";
	char	pStr[3];

	pStr[0] = pConv[(nVal >> 4) & 0x0F];
	pStr[1] = pConv[nVal & 0x0F];
	pStr[2] = 0;
	Font_StrAdd(psFont, pStr);
}

// Printf.
void Font_Printf(u32 nPosX, u32 nPosY, char *pStr, u16 nColor)
{
	if (pStr == NULL) return;

	u32	i, j;

	// Affichage.
	SDL_LockSurface(gMisc.pScreen);
	u16	*pScr = (u16 *)gMisc.pScreen->pixels;

	while (*pStr && nPosX < gMisc.nScrW)
	{
		char	cChr = *pStr;
		if (cChr >= 'a' && cChr <= 'z') cChr &= ~0x20;
		if (cChr > ' ' && cChr <= 127)//'`')
		{
			u32	nOffs = ((cChr) - ' ') * 64;
			for (j = 0; j < 8; j++)
			{
				if (nPosY + j >= gMisc.nScrH) break;
				for (i = 0; i < 8; i++)
				{
					if (nPosX + i >= gMisc.nScrW)
					{
						nOffs += 8 - i;
						break;	// Pas de poke en dehors de l'écran.
					}
					if (gpFont8[nOffs++])
					{
						*(pScr + (nPosX + i) + (((nPosY + j) * gMisc.nScrW))) = nColor;
					}
				}
			}

		}
		nPosX += 8;
		pStr++;
	}

	SDL_UnlockSurface(gMisc.pScreen);

}


