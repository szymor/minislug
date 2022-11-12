

enum
{
	e_MouseLoc_Map = 0,
	e_MouseLoc_Planes,
	e_MouseLoc_Visu,

};

enum
{
	e_MouseState_Idle = 0,		// Vide.
	e_MouseState_GrabStarted,	// Grab de brosse en cours.
	e_MouseState_Brush,			// Brosse.

};

#define	MOUSE_MAX_RECT	5

// Struct pour les rectangles.
struct SRect
{
	u32	nX1, nY1, nX2, nY2;
	s32	nWhere;
	u32	nVar;	// Var en plus, par ex no du plan au cas on soit sur un plan.
};

// Struct pour la brosse.
struct SMouse
{
	u32	nPixPosX, nPixPosY;		// Pos de la souris en pixels.
	s32	nBlkPosX, nBlkPosY;		// Pos de la souris en blocs. (-1,-1) si nulle part.
	struct SRect	*pWhere;	// Où est la souris.

	struct SRect	sMapRect;	// Rect de la map, toujours présente.
	struct SRect	pRect[MOUSE_MAX_RECT];	// Pour détermination de la pos de la souris.
	u32	nRectNb;		// Nb de rect inscrits dans la liste.
	struct SRect	sVisuRect;	// Rect de la visu. Variable prise en considération selon gMap.VisuOnOff().

	u32	nState;			// Etat dans lequel se trouve la souris.

	s32	*pBrush;				// Brosse.
	u32	nBrushLg, nBrushHt;		// Lg et Ht de la brosse en blocs.
	//u32	nBrushPlane;			// Plan auquel appartient la brosse.
	s32	nBrushXOrigin, nBrushYOrigin;	// (x,y) de départ de la capture.
	struct SMstUnit	**ppMstBrush;		// Quand il y a des monstres, il y a un "plan" de pointeurs.

	u32	nTile;		// Tiling commencé ou pas.
	u32	nTileOrgX, nTileOrgY;	// Origine X, Y. (= Premier poke).

	u32	nVisuDrag;					// Drag de la visu.
	s32	nVisuOffsX, nVisuOffsY;		// Offs x et y de la souris dans la visu lors du drag.

};

struct SMouse * Mouse_New(void);
void Mouse_Delete(struct SMouse *psMouse);

void Mouse_RectReset(struct SMouse *psMouse);
void Mouse_RectMap(struct SMouse *psMouse, u32 nX1, u32 nY1, u32 nX2, u32 nY2);
u32 Mouse_RectAdd(struct SMouse *psMouse, u32 nX1, u32 nY1, u32 nX2, u32 nY2, s32 nType, u32 nVar);
void Mouse_RectVisu(struct SMouse *psMouse, u32 nX1, u32 nY1, u32 nX2, u32 nY2);

void Mouse_LocationGet(struct SMouse *psMouse);
u32	Mouse_LocationCheck(struct SMouse *psMouse, u32 nNewPosX, u32 nNewPosY);
u32 Mouse_MouseOnMap(struct SMouse *psMouse);

void Mouse_ButtonPressed(struct SMouse *psMouse, u32 nButtonNo, u32 nButtonX, u32 nButtonY);
void Mouse_ButtonReleased(struct SMouse *psMouse, u32 nButtonNo, u32 nButtonX, u32 nButtonY);

void Mouse_BrushDraw(struct SMouse *psMouse);
void Mouse_BrushGrabStart(struct SMouse *psMouse);
void Mouse_BrushGrabEnd(struct SMouse *psMouse, u32 nCut);
void Mouse_BrushRelease(struct SMouse *psMouse);
void Mouse_BrushPoke(struct SMouse *psMouse);
void Mouse_BrushTiling(struct SMouse *psMouse);




