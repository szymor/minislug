
// include spécial pour les fichiers de monstres.

s32 GetBits(u32 nBitStart, u32 nBitEnd, u8 *pData, u32 nSign);
void Mst_Gravity(struct SMstCommon *pMst);
void Mst_Gravity2(struct SMstCommon *pMst, s32 nGravity, s32 nSpdYMax);
void Enemy_GroundLevel(struct SMstCommon *pMst);
u32 Screen_ObjectOut(s32 nPosX, s32 nPosY);
u32 Screen_ObjectOutRect(s32 nPosX, s32 nPosY, u32 nBlkLg, u32 nBlkHt);
void Mst_ProximityCheck(struct SMstCommon *pMst);
u32 Mst_KnifeHitCheck(struct SMstCommon *pMst, u32 nMstSpr);
u32 Mst_ShotLaunch(struct SMstCommon *pMst, u32 nSpr, u32 nShotNo, u64 *pDustAnm);
u32 Mst_ShotCheckHit(struct SMstCommon *pMst, u32 nSpr, u64 *pDeathAnmTb[]);
u32 Mst_BasicMove(struct SMstCommon *pMst, s32 nSpdX, u32 nFlags);
void Mst_PutOutOfScreen(struct SMstCommon *pMst, s32 nOffset);
void Mst_PlayerBlock(s32 nPosX, u32 nSide);

enum
{
	e_ShotCheck_MinimizedDamage = 1,
};
u32 Mst_ShotCheckLife(u32 nSpr, s32 nPosX, s32 nPosY, u8 *pnHitCnt, u8 *pnLife);
u32 Mst_ShotCheckLife2(u32 nSpr, s32 nPosX, s32 nPosY, u8 *pnHitCnt, u8 *pnLife, u32 nFlags);

extern struct SMstTb gpMstTb[e_Mst_MAX];


struct SMstPath
{
	s32	nPathPosX, nPathPosY;	// Pour recherche des chemins.
	s16	nSpdGoal;		// Pour accélérations.
	u8	nNewAngle;		// Nouvel angle à atteindre quand on tourne.
	s8	nSensRot;		// Sens de rotation.
};
void Mst_PathInit(struct SMstCommon *pMst, struct SMstPath *pPath, s32 nSpdInit, u8 nAngInit);
void Mst_PathMove(struct SMstCommon *pMst, struct SMstPath *pPath, s32 nSpdAcc, u32 nPhaseTurn);


#define	MST_QUEST_ITEM_MST32		(MST_QUEST_ITEM_NEXT_LEVEL - 1)	// N° item pour contrôle du scroll niveau fusée.

#define	MST_SEQ_MAX	16
struct SMstMisc
{
	u8	pSeqCount[MST_SEQ_MAX];		// Compte des séquences/séries de monstres. Pour donner les bonus, ne finit pas forcément à 0.
	u8	pSeqCount2[MST_SEQ_MAX];	// Compte des séquences/séries de monstres. Pour compte général, finit à 0 (L11).
	u8	nSeqCur;
//	s32	pSeqScroll[MST_SEQ_MAX];	// Pour décalage écran de la séquence complète (niveau de la fusée).
	u32	pSeqLeader[MST_SEQ_MAX];	// Pour recherche des leaders (32 mst max dans la série !).
};
extern struct SMstMisc	gMstMisc;
u32	SeqInit(u32 nNbInSeq);

// Condition de tir (héros pas mort et pas en victoire ou fin de niveau).
#define MST_SHOT_COND	(gShoot.nDeathFlag == 0 && gpMstQuestItems[MST_QUEST_ITEM_NEXT_LEVEL] == 0)

#define	MEDIUMEXPLO_ANM_DURATION	(54)

// Pour les tables de pointeurs de foncitions.
struct SMstFctTb
{
	void (*pFctInit) (struct SMstCommon *pMst, u8 *pData);
	s32 (*pFctMain) (struct SMstCommon *pMst);
};

void Rumi_Reset(void);
void Rumi_Check(s32 nPosY, u32 nWeapon);


