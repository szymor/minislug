
// Flags.
enum
{
	e_DustFlag_FlipX = 1,
	e_DustFlag_Mvt = (1 << 1),
	e_DustFlag_Gravity = (1 << 2),
	e_DustFlag_CheckGnd = (1 << 3),
	e_DustFlag_Bounce = (1 << 4),
	e_DustFlag_Rotation = (1 << 5),
	e_DustFlag_LinkAnm = (1 << 6),

};

struct SDustExt
{
	u64	*pLinkAnm;	// Pour enchaîner une autre anim quand un dust avec grv touche le sol.
	u8	nRotInc;	// Pour passer l'incrément lors des rotations.
};
extern struct SDustExt	gDustExg;


// Prototypes.
void DustInitEngine(void);
void DustManage(void);
s32 DustSet(u64 *pAnm, s32 nPosX, s32 nPosY, u8 nPrio, u32 nFlags);
s32 DustSetMvt(u64 *pAnm, s32 nPosX, s32 nPosY, s32 nSpdX, s32 nSpdY, u8 nPrio, u32 nFlags);

