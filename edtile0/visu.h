
#define	VISU_DEFAULT_Width	320
#define	VISU_DEFAULT_Height	240

struct SScrollVar
{
	u32	nValidPlane;		// Plan valide dans le scroll.
	s32	nPlaneLg;			// Largeur et hauteur du plan (en blocs 16).
	s32	nPlaneHt;
	s32	nPlaneNewPosX;		// Nouvelles positions du plan, utilisées lors du calcul du différentiel.
	s32	nPlaneNewPosY;

};

struct SVisu
{
	u32	nInit;			// 0: Pas initialisé / 1: Initialisé.
	u32	nDisplayWidth, nDisplayHeight;	// Lg/Ht de la visu.

	struct SScrollVar	*pScroll;
	s32	nPosX, nPosY;			// Position générale dans la visualisation.
	u32	nRefPlane;				// Plan de ref pour le différentiel.

	s32	nWndPosX, nWndPosY;		// Position de la fenêtre d'affichage.

};


struct SVisu * Visu_New(u32 nVisuLg, u32 nVisuHt);
void Visu_Delete(struct SVisu *psVisu);

void Visu_ScrollPosition(struct SVisu *psVisu, u8 *pKeys);
void Visu_DrawVisu(struct SVisu *psVisu);
void Visu_WindowLocation(struct SVisu *psVisu, s32 nNewPosX, s32 nNewPosY);
void Visu_VisuToggle(struct SVisu *psVisu, u32 nOnOff);


