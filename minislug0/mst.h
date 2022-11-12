

// Structure commune à tous les monstres.
#define MST_COMMON_DATA_SZ  64
struct SMstCommon
{
	u8	nUsed;			// 0 = slot vide, 1 = slot occupé.
	u8	nMstNo;			// No du monstre (dans le fichier de commandes).
	s32	nLoadIdx;		// Index du monstre dans les tables gLoadedMst. -1 si monstre généré (=> Il ne faudra pas toucher à son 'état').

	void (*pFctInit) (struct SMstCommon *pMst, u8 *pData);	// Fct d'init du monstre.
	s32 (*pFctMain) (struct SMstCommon *pMst);		// Fct principale du monstre.

	s32	nAnm;			// Anim.
	s32	nPosX, nPosY;
	s32	nSpd;
	s32	nSpdY;
	u8	nAngle;
	u8	nPhase;
	u8	nFlipX;
	u8	nGround;

	u8	pData[MST_COMMON_DATA_SZ];	// On fera pointer les structures spécifiques ici.

};

#define	MSTPRIO_AND	31
extern	u32	gnMstPrio;		// Pour priorité de l'affichage.

#define	MST_QUEST_ITEMS_MAX	256
extern	u8	gpMstQuestItems[MST_QUEST_ITEMS_MAX];	// Items de quête.

// Prototypes.
void MstInitEngine(void);
void MstManage(void);
s32 MstAdd(u32 nMstNo, s32 nPosX, s32 nPosY, u8 *pData, s32 nLoadIdx);

#define	MST_CLIP_VAL	6//4	// Distance en blocs hors écran pour entrée/sortie de la gestion.
void MstCheckNewCol(s32 nCol, s32 nPosY, s32 nSens);
void MstCheckNewLine(s32 nLine, s32 nPosX, s32 nSens);




