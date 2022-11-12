
#include "includes.h"
#include "font8x8.h"

//CFont	gFont;

/*
// Ajout d'une chaîne dans la chaîne locale.
void CFont::StrAdd(const char *pStr)
{
	strcpy(&_pString[_nIdx], pStr);
	_nIdx += strlen(pStr);

}

// Ajout d'un entier dans la chaîne locale.
void CFont::StrItoa(s32 nVal)
{
	char	pStr[33];
	//itoa(nVal, pStr, 10);
	snprintf(pStr, sizeof(pStr), "%d", (int)nVal);	// pas de itoa en Linux...
	StrAdd(pStr);
}

// Conversion d'un u8 en hexa.
void CFont::StrU8toAHex(u8 nVal)
{
	static char pConv[] = "0123456789ABCDEF";
	char	pStr[3];

	pStr[0] = pConv[(nVal >> 4) & 0x0F];
	pStr[1] = pConv[nVal & 0x0F];
	pStr[2] = 0;
	StrAdd(pStr);
}
*/

// Printf.
u32 FntPrintf(u32 nPosX, u32 nPosY, const char *pStr, u16 nColor)
{
	if (pStr == NULL) return (0);

	u32	i, j;
	u32	nPosX_org = nPosX;

	// Affichage.
	SDL_LockSurface(gMisc.pScreen);
	u16	*pScr = (u16 *)gMisc.pScreen->pixels;

	while (*pStr && nPosX < SCR_Width)
	{
		char	cChr = *pStr;
		if (cChr >= 'a' && cChr <= 'z') cChr &= ~0x20;
		if (cChr > ' ' && cChr <= '`')
		{
			u32	nOffs = ((cChr) - ' ') * 64;
			for (j = 0; j < 8; j++)
			{
				if (nPosY + j >= SCR_Height) break;
				for (i = 0; i < 8; i++)
				{
					if (nPosX + i >= SCR_Width)
					{
						nOffs += 8 - i;
						break;	// Pas de poke en dehors de l'écran.
					}
					if (gpFont8[nOffs++])
					{
						*(pScr + (nPosX + i) + (((nPosY + j) * (gMisc.pScreen->pitch / sizeof(u16)) ))) = nColor;
					}
				}
			}
		}
		nPosX += 8;
		pStr++;
	}

	SDL_UnlockSurface(gMisc.pScreen);

	return (nPosX - nPosX_org);
}

