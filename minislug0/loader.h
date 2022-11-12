


// Struct pour lecture/sauvegarde d'un monstre.
// !!! Ne surtout pas toucher, c'est aussi le format utilisé dans l'éditeur de map et qui est relu directement !!!
#pragma pack(1)
struct SMstEdt0
{
	u32	nPosX, nPosY;
	u16	nMstNo;		// = No du bloc.
	u16	nNbBytes;	// Nb de bytes utilisés.
};
#pragma pack()

struct SMst0
{
	u32	nPosX, nPosY;
	u16	nMstNo;		// = No du bloc.
	u16	nNbBytes;	// Nb de bytes utilisés.
	u16	nIdx;		// Index du monstre, pour taper dans table pMstState.
	u16 _padding;
};

struct SLoadedMst
{
	u8	*pMstData;			// Les datas des monstres.
	struct	SMst0 **ppMstPtrX;	// Les monstres triés sur le X.
	struct	SMst0 **ppMstPtrY;	// Les monstres triés sur le Y.
	u8	*pMstState;			// Etat des monstres (pas entré, déjà géré, mort...).
	u32	nMstNbInList;
	s32	nMstRechIdxX, nMstRechIdxY;	// Indexs des recherches dans les tables ppMstPtrX et Y.

};

enum
{
	e_MstState_Asleep = 0,		// Le monstre est endormi, il attend d'être réveillé par le scroll.
	e_MstState_Managed,			// Le monstre est géré (il est dans le jeu).
	e_MstState_Dead,			// Le monstre est mort.

};

extern	struct SMap	gMap;
extern	struct SLoadedMst	gLoadedMst;

// Struct pour le stockage des paths.
struct SPathBlock
{
	s32	nPosX, nPosY;
	u16	nBlockNo;
};

// Struct pour les codes de collision et les hauteurs.
struct SBlockCol
{
	u32	pHt[2];		// [0;15] * 16 => 64 bits.
	u16	nZero;		// 1 bit en plus pour chaque colonne.

	u8	nCol;		// Code de collision.
};

struct SMap
{
	u32	nMapLg;
	u32	nMapHt;
	u32	nPlanesNb;

	SDL_Surface	*ppPlanesGfx[MAP_PLANES_MAX];	// Graphs des plans.
	s32	*ppPlanesBlocks[MAP_PLANES_MAX];		// Les plans (n° de blocs).
	s32	pPlanesLg[MAP_PLANES_MAX];		// Largeur et hauteur de chaque plan en blocs 16, dans une surface de nMapLg * nMapHt.
	s32	pPlanesHt[MAP_PLANES_MAX];
	u32	nHeroPlane;						// Plan du héros, servira de référence pour le scroll différentiel.

	//u8	*ppColCodes[MAP_PLANES_MAX];	// Les codes de collision.
	struct SBlockCol	*ppColCodes[MAP_PLANES_MAX];	// Les codes de collision.

	struct SPathBlock	*pPath;
	struct SPathBlock	*pPathGnd;	// Ptr dans pPath pour les blocs au sol.
	u32	nPathGndNb;					// Nb de blocs de path.
	struct SPathBlock	*pPathAir;	// Ptr dans pPath pour les blocs en l'air.
	u32	nPathAirNb;					// Nb de blocs de path.

	u32	nPlayerStartPosX, nPlayerStartPosY;

	u32	nTranspColorKey;			// Valeur à passer à SDL_SetColorKey() pour la transparence des plans.

	u8	*pBlkAnmMem;	// Bloc mémoire pour les 'plans' d'anims de blocs.
	u8	*ppBlkAnmPlanes[MAP_PLANES_MAX];	// Les plans d'anim de blocs (les pointeurs vont pointer dans pBlkAnmMem).

};

extern	struct SMap	gMap;



void LevelLoad(u32 nLevelNo);
void LevelRelease(void);

s32 Map_PathGndGetBlock(s32 nPosX, s32 nPosY);
s32 Map_PathAirGetBlock(s32 nPosX, s32 nPosY);


enum
{
	e_BlockCode_Hard = 1,	// Dur.
	e_BlockCode_Soft,		// Plateformes traversables.
	e_BlockCode_Ceiling,	// Dur plafond, pour l'instant restreint à la nage. Routines de 'naked' à revoir pour utilisation en mode 'naked'.
};

u32 BlockGetHeight(u32 nBlockNo, u32 nCol);
s32 BlockGetGroundLevel(s32 nPixPosX, s32 nPixPosY);

u32 BlockCeilingGetHeight(u32 nBlockNo, u32 nPosX);

//=====================================

enum
{
	e_HardSpr_ShotsIgnore = 1,
};

void HardSpr_Init(void);
s32 HardSpr_TestIn(s32 nPosX, s32 nPosY, u32 nFlags);
s32 HardSpr_TestHt(s32 nPosX, s32 nPosY, u32 nFlags);
s32 HardSpr_AddToList(u32 nSpr, s32 nPosX, s32 nPosY, u32 nFlags);
void HardSpr_ReleaseSlot(u32 nSlotNo);


