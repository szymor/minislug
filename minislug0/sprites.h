

#define	SPR_Flip_X		(1 << 31)
#define	SPR_Flip_Y		(1 << 30)
#define	SPR_Flag_HitPal		(1 << 29)

#define	SPR_NoSprite	((u32)-2 & ~(SPR_Flip_X | SPR_Flip_Y))		// Sprite qui n'affiche rien.

// Structures.

#define	SPRRECT_MAX_ZONES	3	// Nb de zones max à relire.
enum
{
	e_SprRect_NDef = 0,		// Zone non définie.
	e_SprRect_Point,		// C'est un point : (x1, y1).
	e_SprRect_Rect,			// C'est un rectangle.
};
enum
{
	e_SprRectZone_PtRef = 0,	// Alpha 1 = Point de ref.
	e_SprRectZone_RectCol,		// Alpha 2 = Zone de collision.
	e_SprRectZone_ShotOrg,		// Alpha 3 = Origine des tirs.
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
//	s32	nPtRefX, nPtRefY;	// Points de ref.
//	u32	nLg, nHt;			// Largeur et hauteur du sprite.
	s16	nPtRefX, nPtRefY;	// Points de ref.
	u16	nLg, nHt;			// Largeur et hauteur du sprite.
	union
	{
		u8	*pGfx8;
		u32	nGfx8Offset;	// Pour stockage temporaire de l'offset pendant la lecture des planches.
	};
//	u32	nRemapPalNo;		// N° de la palette de remappage.
	u16	nRemapPalNo;		// N° de la palette de remappage.

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


