

#define	MST_HIT_CNT	8				// Nb de frames pendant lesquelles afficher un sprite rougi quand touché par un tir.

#define	MST_DAMAGE_BULLET	4		// Dégats des tirs de type balle.
#define	MST_DAMAGE_EXPLO	10		// Dégats des tirs autres que les balles (explosions, touché par un mst,...).
// (Valeurs à comparer à SLUG_ENERGY_MAX).

#define	MST_QUEST_ITEM_NEXT_LEVEL		(MST_QUEST_ITEMS_MAX - 1)	// N° item de quête pour fin de niveau.
// Bit0 = Fin de niveau.
// Bit1 = Héros : Victory.


// Structures.
struct SMstTb
{
	void (*pFctInit) (struct SMstCommon *pMst, u8 *pData);
	s32 (*pFctMain) (struct SMstCommon *pMst);
	u64	*pAnm;
	u16	nPoints;
};

// Liste des monstres.
enum
{
	e_Mst0_Platform1 = 0,
	e_Mst1_ForegroundSpr1,
	e_Mst2_Enemy1,
	e_Mst3_POW,
	e_Mst4_WeaponCapsule,
	e_Mst5_ScrollLockX,
	e_Mst6_RShobu,
	e_Mst7_Zombie1,
	e_Mst8_BlkBkg1,
	e_Mst9_Slug,

	e_Mst10_Jellyfish,
	e_Mst11_ObjItemCtc,
	e_Mst12_HeroBlocker,
	e_Mst13_NextLevel,
	e_Mst14_RebelSoldier0,
	e_Mst15_Truck0,
	e_Mst16_BlkBkg2,
	e_Mst17_Explosions0,
	e_Mst18_RectShotsCheck0,
	e_Mst19_FgSpr2States0,

	e_Mst20_Boss,
	e_Mst21_ScrollY,
	e_Mst22_ExplodingBarrel0,
	e_Mst23_FallingObjects0,
	e_Mst24_HardObjects0,
	e_Mst25_RocketDiver0,
	e_Mst26_Girida0,
	e_Mst27_HalfBoss,
	e_Mst28_Masknell0,
	e_Mst29_Whale0,

	e_Mst30_SquidGenerator0,
	e_Mst31_Squid0,
	e_Mst32_L11ScrollCtrl,
	e_Mst33_L11Generator0,
	e_Mst34_L11MarsEye0,
	e_Mst35_L11SpaceRocks0,
	e_Mst36_L11Asteroid0,
	e_Mst37_L11BigAsteroid0,
	e_Mst38_BigJellyfish0,
	e_Mst39_SensorMine0,

	e_Mst40_L11SpaceTrash0,
	e_Mst41_L11MarsUFO0,
	e_Mst42_DoorToDestroy0,
	e_Mst43_FlyingTara0,
	e_Mst44_SlugWaitTransit0,
	e_Mst45_FlyingTaraBomb0,
	e_Mst46_HairBusterRibert0,
	e_Mst47_Lev9RocketTop0,
	e_Mst48_Joke0,
	e_Mst49_SuspendedSubmarine0,

	e_Mst50_AikawaRumi0,
	e_Mst51_HowToPlay0,
	e_Mst52_Credits0,

	e_Mst_MAX
};

extern u8	gpnMstCount[e_Mst_MAX];		// Compte des monstres par type. Ici à cause de 'e_Mst-MAX'.

// Prototypes.
void MstSlug_EntrancePtGet(u32 nSlugType, s32 *pnOffsX, s32 *pnOffsY);
void Rot2D_RotatePoint(s32 *pnOffsX, s32 *pnOffsY, u8 nAngle);


