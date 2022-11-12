
#define	PLN_MIN_BLOCKS_DISP_W	20	// 20 blocs mini.
#define	PLN_MAX_BLOCKS_H	16		// 16 blocs en hauteur.

enum
{
	e_FlgPln_Hide = 1 << 0,		// Pour cacher/afficher le plan.
	e_FlgPln_Alpha = 1 << 1,	// Pour cacher/afficher l'alpha si le plan a une couche alpha.
	e_FlgPln_Codes = 1 << 2,	// Pour cacher/afficher les codes si le plan a une couche de codes.

	e_FlgPln_Reserved = 1 << 31,	// Servira pour indiquer si il y a des codes à la lecture/sauvegarde.

};

// Structure pour un plan.
struct SPlane
{
	s32	*pPlane;		// Le plan (taille de la map).
	u32	nPlaneNo;		// No du plan.

	u8	*pCodes;		// Les codes des blocs (taille de la planche de blocs).

	char	pPlnFilename[256];		// Nom du fichier graphique.
	SDL_Surface	*pGfx;				// Planche.
	SDL_Surface	*pAlpha;			// Alpha layer 1.
	u32	nWndPosX, nWndPosY;			// (x,y) de départ dans la fenêtre (en blocs).

	u32	nFlags;			// Flags.

	struct SCmdfile	*pMst;				// Pour les monstres.
	struct SMstUnit	**ppMstPlane;		// Quand il y a des monstres, il y a un "plan" de pointeurs.

};

struct SPlane * Plane_New(void);

struct SMap;
void Plane_Delete(struct SPlane *psPlane, struct SMap *psMap);

void Plane_PlaneInit(struct SPlane *psPlane, char *pFilename);
void Plane_GfxConvert(struct SPlane *psPlane);
void Plane_PlaneClear(struct SPlane *psPlane);
u32 Plane_WndDisplay(struct SPlane *psPlane, u32 nOffs);
void Plane_MapDisplay(struct SPlane *psPlane);
void Plane_WndMoveXY(struct SPlane *psPlane, s32 nSensX, s32 nSensY);
void Plane_BlockDraw(struct SPlane *psPlane, u32 nBlockNo, u32 nPosX, u32 nPosY, bool bLock);
void Plane_BlockDrawPix(struct SPlane *psPlane, u32 nBlockNo, u32 nPosX, u32 nPosY, bool bLock);

void Plane_CodesPlaneAddDefaultSz(struct SPlane *psPlane);
void Plane_CodesPlaneAdd(struct SPlane *psPlane, u32 nBlkLg, u32 nBlkHt);
void Plane_CodesPlaneRemove(struct SPlane *psPlane);
void Plane_CodeAdd(struct SPlane *psPlane, u32 nBlockNo, s8 nInc);
void Plane_CodeSet(struct SPlane *psPlane, u32 nBlockNo, u8 nVal);

void Plane_PlaneColumnInsert(struct SPlane *psPlane, u32 nCol);
void Plane_PlaneColumnDelete(struct SPlane *psPlane, u32 nCol);
void Plane_PlaneLineInsert(struct SPlane *psPlane, u32 nLin);
void Plane_PlaneLineDelete(struct SPlane *psPlane, u32 nLin);

u32 Plane_CmdfileAdd(struct SPlane *psPlane, char *pCmdFilename);
u32 Plane_CmdfileAddFromBuf(struct SPlane *psPlane, char *pCmdFilename, char *pCmdBuf, u32 nCmdDate);
void Plane_CmdFileCompare(struct SPlane *psPlane);

void Plane_MstPlaneInit(struct SPlane *psPlane);
void Plane_MstPlaneClear(struct SPlane *psPlane);

//	inline void Toggle(void) { _nFlags ^= e_FlgPln_Hide; }
//	inline u32 IsHidden(void) { return (_nFlags & e_FlgPln_Hide); }
//	inline void AlphaToggle(void) { _nFlags ^= e_FlgPln_Alpha; }
//	inline u32 HasAlpha(void) { return (_pAlpha == NULL ? 0 : 1); }
//	inline u32 HasCodes(void) { return (_pCodes == NULL ? 0 : 1); }
//	inline void CodesToggle(void) { _nFlags ^= e_FlgPln_Codes; }
//	inline u32 HasCmd(void) { return (_pMst == NULL ? 0 : 1); }
void Plane_Toggle(struct SPlane *psPlane);
u32 Plane_IsHidden(struct SPlane *psPlane);
void Plane_AlphaToggle(struct SPlane *psPlane);
u32 Plane_HasAlpha(struct SPlane *psPlane);
u32 Plane_HasCodes(struct SPlane *psPlane);
void Plane_CodesToggle(struct SPlane *psPlane);
u32 Plane_HasCmd(struct SPlane *psPlane);

