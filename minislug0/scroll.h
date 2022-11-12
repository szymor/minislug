

enum
{
	e_ScrollType_Free = 0,
	e_ScrollType_RightOnly,
};

// Structures.
struct SScrollPos
{
	s32	nPosX;		// Position générale.
	s32	nPosY;

//	u8	nLockX;		// 0 : Normal / 1 : Locké, ne bouge plus.
	s32	nLastPosX;
	u8	nScrollType;

	// Variable spéciale pour certains scrolls spéciaux (train, espace... etc).
	union
	{
		s32 nSpecialVar;		// Pour init de la valeur.
		s32	nL02SpdX;
		s32 nL08PosX;
		s32 nL11SpdY;
	};

};
extern	struct SScrollPos	gScrollPos;


// Prototypes.
void ScrollAllocate(void);
void ScrollRelease(void);
void ScrollInitScreen(u32 nScrollType);
void ScrollManage(void);
void ScrollDisplayPlane(u32 nPlaneNo);

void ScrollGetPlanePosXY(s32 *pPosX, s32 *pPosY, u32 nPlane);
void Scroll_BlkAnm_BlockUpdate(u32 nPlane, s32 sBlMapX, s32 sBlMapY, s32 nOffset);

