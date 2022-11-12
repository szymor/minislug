

// Struct pour l'affichage du texte avec la fonte 8*8.
struct SFont
{
	char	pString[256];
	u32		nIdx;

};

extern	struct SFont	gFont;

void Font_Printf(u32 nPosX, u32 nPosY, char *pStr, u16 nColor);

void Font_StrReset(struct SFont *psFont);
void Font_StrAdd(struct SFont *psFont, char *pStr);
void Font_StrItoa(struct SFont *psFont, s32 nVal);
void Font_StrU8toAHex(struct SFont *psFont, u8 nVal);


