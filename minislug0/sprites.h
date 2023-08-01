

#define	SPR_Flip_X		(1 << 31)
#define	SPR_Flip_Y		(1 << 30)
#define	SPR_Flag_HitPal		(1 << 29)

#define	SPR_NoSprite	((u32)-2 & ~(SPR_Flip_X | SPR_Flip_Y))		// Sprite that displays nothing.

// Structures.

#define	SPRRECT_MAX_ZONES	3	// Max number of zones to reread.
enum
{
	e_SprRect_NDef = 0,		// Area not defined.
	e_SprRect_Point,		// A point: (x1, y1).
	e_SprRect_Rect,			// A rectangle.
};
enum
{
	e_SprRectZone_PtRef = 0,	// Alpha 1 = Ref point
	e_SprRectZone_RectCol,		// Alpha 2 = Collision zone.
	e_SprRectZone_ShotOrg,		// Alpha 3 = Origin of shots.
};

#pragma pack(1)
struct SSprRect
{
//	u32	nType;
//	s32	nX1, nY1, nX2, nY2;
	u16	nType;
	s16	nX1, nY1, nX2, nY2;
};

struct SSprite
{
//	s32	nPtRefX, nPtRefY;	// Ref points
//	u32	nLg, nHt;			// Sprite width and height.
	s16	nPtRefX, nPtRefY;	// Ref points
	u16	nLg, nHt;			// Sprite width and height.
	union
	{
		u8	*pGfx8;
		u32	nGfx8Offset;	// For temporary offset storage while plates are being read.
	};
//	u32	nRemapPalNo;		// Remapping palette number.
	u16	nRemapPalNo;		// Remapping palette number.

	struct SSprRect	pRect[SPRRECT_MAX_ZONES];	// Rectangles, points...

};
#pragma pack()


// Prototypes.
void SprInitEngine(void);
void SprRelease(void);
void SprLoadGfx(char *pFilename);
void SprEndCapture(void);

void SprPaletteConversion(void);
void SprBinariesSave(void);
void SprBinariesLoad(void);

void SprDisplay(u32 nSprNo, s32 nPosX, s32 nPosY, u16 nPrio);
void SprDisplayZoom(u32 nSprNo, s32 nPosX, s32 nPosY, u16 nPrio, u16 nZoomX, u16 nZoomY);
void SprDisplayRotoZoom(u32 nSprNo, s32 nPosX, s32 nPosY, u16 nPrio, u8 nAngle, u16 nZoom);

void SprDisplayAbsolute(u32 nSprNo, s32 nPosX, s32 nPosY, u16 nPrio);
void SprDisplayZoomAbsolute(u32 nSprNo, s32 nPosX, s32 nPosY, u16 nPrio, u16 nZoomX, u16 nZoomY);
void SprDisplayRotoZoomAbsolute(u32 nSprNo, s32 nPosX, s32 nPosY, u16 nPrio, u8 nAngle, u16 nZoom);

void SprDisplayAll(void);
void SprDisplayAll_Pass1(void);
void SprDisplayAll_Pass2(void);

struct SSprite *SprGetDesc(u32 nSprNo);
u32 SprCheckColBox(u32 nSpr1, s32 nPosX1, s32 nPosY1, u32 nSpr2, s32 nPosX2, s32 nPosY2);
u32 SprGetRect(u32 nSprNo, u32 nZone, struct SSprRect *pRectDst);

//#ifdef __LINUX__
#if defined (__LINUX__) || defined (__APPLE__)
int stricmp(char *pStr1, char *pStr2);
#endif


