

enum
{
	e_CheckBlk_Bkg = 0,		// Bloc damier : Background.
	e_CheckBlk_Empty,		// Vide.
	e_CheckBlk_BlueDark,
	e_CheckBlk_BlueLight,
	e_CheckBlk_RedDark,
	e_CheckBlk_RedDark2,
	e_CheckBlk_RedLight,

};


// Structure pour la map.
struct SMap
{
	u32	nPlanesNb;					// Nb de plans.
	struct SPlane	**ppsPlanes;	// Les plans.

	u32	nMapLg, nMapHt;				// Lg et ht de la map.
	u32	nMapPosX, nMapPosY;			// Pos (x,y) d'affichage de la map.

	u32	nFirstWindow;				// Première fenêtre de planches affichée.
	u32	nActivePlane;				// Plan actif.

	struct SVisu	*psVisu;		// Pour visualisation.
	u32	nVisuOnOff;					// On/Off.

	u32	nModified;					// Map modifiée.

};

struct SMap * Map_New(void);
void Map_Delete(struct SMap *psMap);

void Map_PlaneAdd(struct SMap *psMap, char *pFilename);
void Map_PlaneInsert(struct SMap *psMap, char *pFilename, u32 nInsertBefore);
void Map_PlaneRemove(struct SMap *psMap, u32 nPlaneNo);
void Map_GfxConvert(struct SMap *psMap);		// 1 seul appel !
void Map_PlanesSetNo(struct SMap *psMap);

void Map_PlanesDisplayWnds(struct SMap *psMap);
void Map_MapDisplay(struct SMap *psMap);

void Gfx_DrawCheckBlock(u32 nType, u32 nPosX, u32 nPosY, bool bLock);

void Map_ActivePlaneSet(struct SMap *psMap, u32 nPlaneNo);
u32 Map_ActivePlaneGet(struct SMap *psMap);
void Map_PlaneWndMoveXY(struct SMap *psMap, u32 nPlaneNo, s32 nSensX, s32 nSensY);
void Map_MapMoveXY(struct SMap *psMap, s32 nSensX, s32 nSensY);

void Map_FirstWndMove(struct SMap *psMap, s32 nSens);
void Map_PlaneToggle(struct SMap *psMap, u32 nPlaneNo);

void Map_StatusBarDisplay(struct SMap *psMap);

void Map_MapColumnInsert(struct SMap *psMap);
void Map_MapColumnDelete(struct SMap *psMap);
void Map_MapLineInsert(struct SMap *psMap);
void Map_MapLineDelete(struct SMap *psMap);

void Map_PlaneAlphaToggle(struct SMap *psMap);
u32 Map_PlaneHasAlpha(struct SMap *psMap, u32 nPlaneNo);

void Map_VisuToggle(struct SMap *psMap);
void Map_VisuDisplay(struct SMap *psMap);
u32 Map_VisuOnOff(struct SMap *psMap);

u32 Map_PlaneHasCmd(struct SMap *psMap, u32 nPlaneNo);
void Map_PlaneCmdfileAdd(struct SMap *psMap, u32 nPlaneNo, char *pCmdFilename);
void Map_PlaneCmdfileAddFromBuf(struct SMap *psMap, u32 nPlaneNo, char *pCmdFilename, char *pCmdBuf, u32 nCmdDate);

void Map_PlaneCodesAdd(struct SMap *psMap, u32 nPlaneNo);
void Map_PlaneCodesRemove(struct SMap *psMap, u32 nPlaneNo);
void Map_PlaneCodesToggle(struct SMap *psMap);
u32 Map_PlaneHasCodes(struct SMap *psMap, u32 nPlaneNo);
void Map_PlaneCodeAdd(struct SMap *psMap, u32 nPlaneNo, u32 nBlockNo, s8 nInc);
void Map_PlaneCodeSet(struct SMap *psMap, u32 nPlaneNo, u32 nBlockNo, u8 nVal);


u32 Map_UndoCopy(struct SMap *psMapDst);
void Map_UndoUndo(struct SMap *psMapSrc);




