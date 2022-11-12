
// Tir ennemi / tir du joueur.
enum
{
	e_ShotOrg_Enemy = 0,
	e_ShotOrg_Player,
	e_ShotOrg_NoMoreCtc,	// Pour supprimer les tests de contact.

};

// Numéros des tirs dans la table.
enum
{
	e_Shot_Player_PistolH = 0,
	e_Shot_Player_PistolV,
	e_Shot_Player_ShotgunH,
	e_Shot_Player_ShotgunV,
	e_Shot_Player_MachinegunH,
	e_Shot_Player_MachinegunV,
	e_Shot_Player_FlameH,
	e_Shot_Player_FlameV,
	e_Shot_Player_HandGrenade_Throw,
	e_Shot_Player_HandGrenade_Explo,
	e_Shot_Player_SlugRocket_MachinegunV,
	e_Shot_Player_VulcanCannon,
	e_Shot_Player_SlugSubmarine_Rocket_Launch,
	e_Shot_Player_SlugSubmarine_Rocket_Explo,
	e_Shot_Player_RocketSmall_Launch,
	e_Shot_Player_RocketSmall_Explo,
	e_Shot_Player_SlugCopter_Bomb_Drop,
	e_Shot_Player_SlugCopter_Bomb_Explo,
	e_Shot_Player_SlugRocket_Rocket_Launch,
	e_Shot_Player_SlugRocket_Rocket_Explo,
	e_Shot_Player_MediumExplo_LargeTouch,
	e_Shot_Player_SwimGrenade_Throw,
	e_Shot_Player_SwimGrenade_Explo,

	e_Shot_Enemy_ZombieBrain,
	e_Shot_Enemy_RShobu_Bomb_Drop,
	e_Shot_Enemy_RShobu_Bomb_Explo,
	e_Shot_Enemy_RebSoldier_Bullet0,
	e_Shot_Enemy_RebSoldier_MortarRocket0,
	e_Shot_Enemy_RocketDiver_Rocket0,
	e_Shot_Enemy_Girida0_Bullet0,
	e_Shot_Enemy_Boss1_ParachuteBomb,
	e_Shot_Enemy_Boss1_BulletFront,
	e_Shot_Enemy_RebSoldier_Handgrenade0,
	e_Shot_Enemy_RebSoldier_LRAC_Rocket0,
	e_Shot_Enemy_MarsUFO_Bullet0,
	e_Shot_Enemy_HangingRock_Fall,
	e_Shot_Enemy_HairBusterRibert_Mine,
	e_Shot_Enemy_MothershipCore_Bullet,
	e_Shot_Enemy_BrainBot_RingLow,
	e_Shot_Enemy_BrainBot_RingHigh,

};

// Type des dégats.
enum
{
	e_ShotDamageType_Gen = 0,		// Général.
	e_ShotDamageType_Blow,			// Souffle (shotgun).
	e_ShotDamageType_Flame,			// Flame.

	e_ShotDamageType_Max
};


// Prototypes.
void FireInitEngine(void);
void FireManage(void);
s32 FireAdd(u32 nShot, s32 nPosX, s32 nPosY, s32 nAngle);
u32 FireHitCheck(u32 nSpr, s32 nPosX, s32 nPosY, u32 nToCheck, u8 nNoCtc);
u32 FireHitCheckRect(struct SSprRect *pRect1, s32 nPosX, s32 nPosY, u32 nToCheck, u8 nNoCtc);
s16 * FireGetSpdXPtr(u32 nSlotNo);
void FireDestructibleCheck(void);
void FireRemoveFromList(s32 nShotIdx);

void ChaserTarget_ClearList(void);
void ChaserTarget_AddToList(s32 nPosX, s32 nPosY);





