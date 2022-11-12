
// Flags.
#define	FONT_NoDisp		(1 << 0)
#define	FONT_Highlight	(1 << 1)

// Prototypes.
u32 Font_Print(s32 nPosX, s32 nPosY, char *pStr, u32 nFlags);
u32 Font_PrintSpc(s32 nPosX, s32 nPosY, char *pStr, u32 nFlags, u32 nSpc);
void MyItoA(s32 nNb, char *pDst);


