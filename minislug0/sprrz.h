

#define	ROT2D_BUF_Width		256
#define	ROT2D_BUF_Height	256

struct SSprite * SprZoom_PreRender(u32 nSpr, u16 nZoomX, u16 nZoomY, void **ppRenderFct);
struct SSprite * SprRotoZoom_PreRender(u32 nSpr, u16 nMultiplier, u16 nAngle, void **ppRenderFct);

typedef struct SSprite * (*pRZFctPreRender)(u32, u16, u16, void **);
typedef void (*pRZFctRender)(void);


struct	S2DPoint
{
	s32	nX, nY;
};
void Rotation2D(u32 nNbPts, struct S2DPoint *pPtsOrg, struct S2DPoint *pPtsRot, u8 nAngle, u16 nMultiplier);


