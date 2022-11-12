
/*
// Classe pour la fonte.
class CFont
{
public:
	char	_pString[256];
	u32	_nIdx;

public:
	CFont(void) { StrReset(); }

	void Printf(u32 nPosX, u32 nPosY, const char *pStr, u16 nColor = 0xFFFF);

	void StrReset(void) { _nIdx = 0; _pString[0] = 0; }
	void StrAdd(const char *pStr);
	void StrItoa(s32 nVal);
	void StrU8toAHex(u8 nVal);

};

extern CFont	gFont;
*/

u32 FntPrintf(u32 nPosX, u32 nPosY, const char *pStr, u16 nColor);

