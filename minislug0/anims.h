
// Codes de contrôle.
#define	BIT31	(1 << 31)
enum
{
	e_Anm_Jump		= BIT31 | 1,	// Ptr + offset. e_Anm_Jump, offset.
	e_Anm_Goto		= BIT31 | 2,	// Initialise une autre anim. e_Anm_Goto, adresse.
	e_Anm_End		= BIT31 | 3,	// Fin d'une anim. Renvoie SPR_NoSprite, place e_AnmFlag_End, ne libère pas le slot.
	e_Anm_Kill		= BIT31 | 4,	// Fin d'une anim. Renvoie -1 et libère le slot (ex: dust).
	e_Anm_Sfx		= BIT31 | 5,	// Joue un son. e_Anm_Sfx, No sfx, Prio sfx.
	e_Anm_StepFlag	= BIT31 | 6,	// Flag pour laisser des traces de pas.
	e_Anm_RndGoto	= BIT31 | 7,	// Goto aléatoire. e_Anm_RndGoto, proba (sur 255), adr si <, adr si >.
	e_Anm_RndJump	= BIT31 | 8,	// Jump aléatoire. e_Anm_RndJump, proba (sur 255), offs si <, offs si >.
	e_Anm_SfxStop	= BIT31 | 9,	// Stoppe un son. e_Anm_SfxStop, No sfx.

};

// Clefs d'anim.	16b = Priorité (à faire) | 16b = No.
#define	ANMPRIO(x)	(x << 16)
enum
{
	e_AnmKey_Null			= 0,

	e_AnmKey_Hero_Stance	= 1,
	e_AnmKey_Hero_Run		= 2,
	e_AnmKey_Hero_CrouchStance	= 3,
	e_AnmKey_Hero_CrouchWalk	= 4,
	e_AnmKey_Hero_Jump		= 5,
	e_AnmKey_Hero_Shot		= ANMPRIO(1) + 6,
	e_AnmKey_Hero_ShotEnd	= ANMPRIO(1) + 7,
	e_AnmKey_Hero_ThrowHG	= 8,
	e_AnmKey_Hero_Knife		= ANMPRIO(2) + 8,
	e_AnmKey_Hero_KnifeEnd	= ANMPRIO(1) + 10,
	e_AnmKey_Hero_SlugOut	= ANMPRIO(1) + 9,
	e_AnmKey_Hero_Death		= ANMPRIO(3) + 1,
	e_AnmKey_Hero_DeathAir	= ANMPRIO(3) + 2,
	e_AnmKey_Hero_RunStop			= 20,
	e_AnmKey_Hero_CrouchWalkStop	= 21,
	e_AnmKey_Hero_UpDown			= 22,
	e_AnmKey_Hero_Victory	= ANMPRIO(4) + 1,
	e_AnmKey_Hero_Bored		= 30,

	e_AnmKey_HeroSub_Stance	= 11,
	e_AnmKey_HeroSub_Run	= 12,

	e_AnmKey_Enemy_Flip	= 1,
	e_AnmKey_Enemy_Shot	= ANMPRIO(1) + 2,
	e_AnmKey_Enemy_Key3	= 3,
	e_AnmKey_Enemy_Strike	= ANMPRIO(1) + 4,

};


// Définition des anims.

extern u64	gAnm_Hero_Stance[];
extern u64	gAnm_Hero_Run[];
extern u64	gAnm_Hero_CrouchStance[];
extern u64	gAnm_Hero_CrouchWalk[];
extern u64	gAnm_Hero_Jump[];
extern u64	gAnm_Hero_Fall[];
extern u64	gAnm_HeroTop_Bored0_Gun[];
extern u64	gAnm_HeroTop_Bored1_Gun[];
extern u64	gAnm_HeroTop_Bored0_Shotgun[];
extern u64	gAnm_HeroTop_Bored1_Shotgun[];
extern u64	gAnm_HeroTop_Bored0_Machinegun[];
extern u64	gAnm_Hero_SlugOutGen[];
extern u64	gAnm_Hero_ThrowHG_Gen[];
extern u64	gAnm_Hero_Shot_Knife1[];
extern u64	gAnm_Hero_Shot_Gun[];
extern u64	gAnm_Hero_Shot_Shotgun[];
extern u64	gAnm_Hero_Shot_RocketLauncher[];
extern u64	gAnm_Hero_Shot_Machinegun[];
extern u64	gAnm_Hero_ShotEnd_Machinegun[];
extern u64	gAnm_Hero_Shot_Flamethrower[];
extern u64	gAnm_Hero_ShotEnd_Flamethrower[];
extern u64	gAnm_Hero_Shot_SubMachinegun[];
extern u64	gAnm_Hero_DeathGen_Gnd[];
extern u64	gAnm_Hero_DeathGen_Air[];
extern u64	gAnm_Hero_DeathGen_Land[];
extern u64	gAnm_HumanStep_Sand_Dust[];
extern u64	gAnm_HumanStep_Water_Dust[];
extern u64	gAnm_Land_Water_Dust[];
extern u64	gAnm_Slug_Land_Water_Dust[];
extern u64	gAnm_Parachute_Glide[];
extern u64	gAnm_Parachute_Fold[];

extern u64	gAnm_HeroSub_Stance[];
extern u64	gAnm_HeroSub_Run[];
extern u64	gAnm_HeroSub_DeathGen[];
extern u64	gAnm_HeroSub_HelmetDeath_Dust[];
extern u64	gAnm_HeroSub_SlugOutGen[];

extern u64	gAnm_HeroSlugNoExit_DeathGen[];

extern u64	gAnm_Hero_SlugGetIn[];
extern u64	gAnm_Hero_Victory[];

extern u64	gAnm_ShotHero_PistolH[];
extern u64	gAnm_ShotHero_PistolH_Dust[];
extern u64	gAnm_ShotHero_PistolV[];
extern u64	gAnm_ShotHero_PistolV_Dust[];
extern u64	gAnm_ShotHero_MachinegunH[];
extern u64	gAnm_ShotHero_MachinegunV[];
extern u64	gAnm_ShotHero_SlugRocket_MachinegunV[];
extern u64	gAnm_ShotHero_ShotgunH[];
extern u64	gAnm_ShotHero_ShotgunV[];
extern u64	gAnm_ShotHero_FlameH[];
extern u64	gAnm_ShotHero_FlameV[];
extern u64	gAnm_ShotHero_FlameH_Dust[];
extern u64	gAnm_ShotHero_FlameV_Dust[];
extern u64	gAnm_ShotHero_HandGrenade_Air[];
extern u64	gAnm_ShotHero_HandGrenade_Explo[];

extern u64	gAnm_Weapon_Gun_Dust[];
extern u64	gAnm_Weapon_Shotgun_Dust[];
extern u64	gAnm_Weapon_Machinegun_Dust[];

extern u64	gAnm_BrainBot_Idle[];
extern u64	gAnm_BrainBot_Shot[];
extern u64	gAnm_BrainBot_Walk[];
extern u64	gAnm_BrainBot_Flip[];
extern u64	gAnm_BrainBot_Death_Dust[];
extern u64	gAnm_BrainBot_DeathLand_Dust[];
extern u64	gAnm_BrainBot_BrainExplo_Dust[];
extern u64	gAnm_ShotEnemy_BrainBot_RingLow[];
extern u64	gAnm_ShotEnemy_BrainBot_RingHigh[];

extern u64	gAnm_MarsPeople_Idle[];
extern u64	gAnm_MarsPeople_Shot[];
extern u64	gAnm_MarsPeople_Flip[];
extern u64	gAnm_MarsPeople_DeathGen_Dust[];
extern u64	gAnm_MarsPeople_DeathFire_Dust[];

extern u64	gAnm_MarsTIE_Fly[];
extern u64	gAnm_MarsTIE_Flip[];
extern u64	gAnm_MarsEye_Fly[];
extern u64	gAnm_MarsEye_Flip[];
extern u64	gAnm_MarsUFO_Fly[];
extern u64	gAnm_MarsUFO_ShotFx[];
extern u64	gAnm_ShotEnemy_MarsUFO_GreenBullet0[];
extern u64	gAnm_ShotEnemy_MarsUFO_GreenBullet0_Dust[];

extern u64	gAnm_MiniUFO_Fly[];
extern u64	gAnm_MiniUFO_Explode_Dust[];

extern u64	gAnm_AsteroidMedium_Fly[];

extern u64	gAnm_BigAsteroid_Cracks[];
extern u64	gAnm_BigAsteroid_Cracks_Dust[];

extern u64	gAnm_AlienMothershipBlink[];

extern u64	gAnm_Itm_WeaponCapsuleH[];
extern u64	gAnm_Itm_WeaponCapsuleF[];
extern u64	gAnm_Itm_WeaponCapsuleS[];
extern u64	gAnm_Itm_WeaponCapsuleR[];
extern u64	gAnm_Itm_WeaponCapsulePickUp_Dust[];
extern u64	gAnm_Itm_AmmoBox1[];
extern u64	gAnm_Itm_GasTank1[];
extern u64	gAnm_Itm_Container1[];
extern u64	gAnm_Itm_Container1_Dust[];

extern u64	gAnm_Debris_Metal0_Dust[];
extern u64	gAnm_Debris_Wood0_Dust[];
extern u64	gAnm_Debris_RockSmall0_Dust[];
extern u64	gAnm_Debris_RockMedium0_Dust[];

extern u64	gAnm_POW_Idle[];
extern u64	gAnm_POW_SittingFree[];
extern u64	gAnm_POW_TotemFree[];
extern u64	gAnm_POW_SittingWait[];
extern u64	gAnm_POW_TotemWait[];
extern u64	gAnm_POW_TotemAlt[];
extern u64	gAnm_POW_BigPants[];
extern u64	gAnm_POW_TotemDisappear_Dust[];
extern u64	gAnm_POW_SaluteDisappear_Dust[];
extern u64	gAnm_POW_Salute[];
extern u64	gAnm_POW_Fall[];
extern u64	gAnm_POW_Run[];
extern u64	gAnm_POW_HangingWait[];
extern u64	gAnm_POW_Hanging_RopeEndDisappear_Dust[];

extern u64	gAnm_AikawaRumi_Walk[];
extern u64	gAnm_AikawaRumi_GiveItem[];
extern u64	gAnm_AikawaRumi_Flee[];

extern u64	gAnm_RShobu_Flying[];
extern u64	gAnm_RShobu_Flip[];
extern u64	gAnm_RShobu_BombDrop_Dust[];
extern u64	gAnm_ShotEnemy_RShobu_Bomb[];
extern u64	gAnm_ShotEnemy_Explosion0_Medium[];
extern u64	gAnm_ShotHero_Explosion0_Medium_LargeTouch[];

extern u64	gAnm_Masknell_Blades[];
extern u64	gAnm_Masknell_ShotFx[];

extern u64	gAnm_ZombieTeen_Idle[];
extern u64	gAnm_ZombieTeen_Walk[];
extern u64	gAnm_ZombieTeen_Fall[];
extern u64	gAnm_ZombieTeen_Rise[];
extern u64	gAnm_ZombieTeen_Flip[];
extern u64	gAnm_ZombieTeen_Shot[];
extern u64	gAnm_ZombieTeen_ShotFX_Dust[];
extern u64	gAnm_Blood0_FX_Dust[];
extern u64	gAnm_Zombie_DeathShotgunGen_Dust[];
extern u64	gAnm_ZombieTeen_DeathBurnWalk[];
extern u64	gAnm_ZombieTeen_DeathBurn_Dust[];
extern u64	gAnm_ShotEnemy_ZombieBrain[];
extern u64	gAnm_ShotEnemy_ZombieBrain_DustAir[];
extern u64	gAnm_ShotEnemy_ZombieBrain_DustGnd[];

extern u64	gAnm_ZombieFat_Idle[];
extern u64	gAnm_ZombieFat_Walk[];
extern u64	gAnm_ZombieFat_Fall[];
extern u64	gAnm_ZombieFat_Rise[];
extern u64	gAnm_ZombieFat_Flip[];
extern u64	gAnm_ZombieFat_Shot[];
extern u64	gAnm_ZombieFat_ShotFX_Dust[];
extern u64	gAnm_ZombieFat_DeathBurnWalk[];
extern u64	gAnm_ZombieFat_DeathBurn_Dust[];

extern u64	gAnm_ZombieBits_Arm1_Dust[];
extern u64	gAnm_ZombieBits_Arm2_Dust[];
extern u64	gAnm_ZombieBits_Leg1_Dust[];
extern u64	gAnm_ZombieBits_Leg2_Dust[];

extern u64	gAnm_Explosion0_Big_Dust[];
extern u64	gAnm_Explosion0_Medium_Dust[];
extern u64	gAnm_Explosion0_Medium_NoSfx_Dust[];

extern u64	gAnm_Lev7_TubeUp_fg[];
extern u64	gAnm_Lev7_TubeDown_fg[];

extern u64	gAnm_Lev8_PF1_Opening[];
extern u64	gAnm_Lev8_Door1_Opening[];
extern u64	gAnm_Lev8_Lever_Wait[];
extern u64	gAnm_Lev8_Lever_Dead[];
extern u64	gAnm_Lev8_Lever_CtcAnm[];
extern u64	gAnm_Lev8_Lock_Wait[];
extern u64	gAnm_Lev8_WagonDoor_Opening[];

extern u64	gAnm_Lev13_HangingRock_Wait[];
extern u64	gAnm_Shot_Lev13_HangingRock_Fall[];
extern u64	gAnm_Lev13_HangingRock_Fall[];
extern u64	gAnm_Lev13_HangingRock_Land[];

extern u64	gAnm_Slug_InSign[];
extern u64	gAnm_Slug_OutSign[];

extern u64	gAnm_VulcanCannon_Idle[];
extern u64	gAnm_VulcanCannon_Shot[];
extern u64	gAnm_ShotHero_VulcanCannon[];
extern u64	gAnm_ShotHero_VulcanCannon_Dust[];

extern u64	gAnm_ShotHero_RocketSmall[];
extern u64	gAnm_RocketSmall_Trail_Dust[];

extern u64	gAnm_SlugTank_GetIn[];
extern u64	gAnm_SlugTank_GetOut[];
extern u64	gAnm_SlugTank_Idle[];
extern u64	gAnm_SlugTank_DriveForward[];
extern u64	gAnm_SlugTank_DriveBackward[];
extern u64	gAnm_SlugTank_DriveStop[];
extern u64	gAnm_SlugTank_CrouchIdle[];
extern u64	gAnm_SlugTank_CrouchDriveForward[];
extern u64	gAnm_SlugTank_CrouchDriveBackward[];
extern u64	gAnm_SlugTank_CrouchDriveStop[];
extern u64	gAnm_SlugTank_Jump[];
extern u64	gAnm_SlugTank_Fall[];
extern u64	gAnm_SlugTank_Shot[];
extern u64	gAnm_SlugTank_Up2Down[];
extern u64	gAnm_SlugTank_Down2Up[];

extern u64	gAnm_SlugSubmarine_GetIn[];
extern u64	gAnm_SlugSubmarine_GetOut[];
extern u64	gAnm_SlugSubmarine_Idle[];
extern u64	gAnm_ShotHero_SubmarineRocket[];
extern u64	gAnm_SubmarineRocket_Trail_Dust[];
extern u64	gAnm_SubmarineBubble_Dust[];

extern u64	gAnm_SlugCopter_Idle[];
extern u64	gAnm_SlugCopter_GetIn[];
extern u64	gAnm_SlugCopter_GetOut[];
extern u64	gAnm_SlugCopter_WaitPlayer[];
extern u64	gAnm_ShotHero_HelicopterBomb[];

extern u64	gAnm_SlugRocket_Idle[];
extern u64	gAnm_HeroInSlug_Rocket_LookR[];
extern u64	gAnm_HeroInSlug_Rocket_LookL[];
extern u64	gAnm_SlugRocket_GetIn[];
extern u64	gAnm_SlugRocket_MainThruster[];
extern u64	gAnm_SlugRocket_MainThruster_Dust[];
extern u64	gAnm_SlugRocket_ThrustUp[];
extern u64	gAnm_SlugRocket_ThrustDown[];
extern u64	gAnm_SlugRocket_ThrustLeft[];
extern u64	gAnm_SlugRocket_ThrustRight[];
extern u64	gAnm_SlugRocket_ThrustEmpty[];
extern u64	gAnm_SlugRocket_WeaponL_Out[];
extern u64	gAnm_SlugRocket_WeaponL_In[];
extern u64	gAnm_SlugRocket_WeaponR_Out[];
extern u64	gAnm_SlugRocket_WeaponR_In[];
extern u64	gAnm_SlugRocket_NoSprite[];
extern u64	gAnm_SlugRocket_ShotSparkMain[];
extern u64	gAnm_SlugRocket_ShotSparkSideL[];
extern u64	gAnm_SlugRocket_ShotSparkSideR[];
extern u64	gAnm_ShotHero_RocketBomb[];

extern u64	gAnm_RebSoldier_DeathStd_Dust[];
extern u64	gAnm_RebSoldier_DeathBlow_Dust[];
extern u64	gAnm_RebSoldier_DeathBurn_Dust[];

extern u64	gAnm_RebSoldier_Rifle_CrouchWait[];
extern u64	gAnm_RebSoldier_Rifle_GetDown[];
extern u64	gAnm_RebSoldier_Rifle_GetUp[];
extern u64	gAnm_RebSoldier_Rifle_Shot[];
extern u64	gAnm_RebSoldier_Rifle_Shot_Dust[];
extern u64	gAnm_ShotEnemy_Bullet[];
extern u64	gAnm_RebSoldier_Rifle_WaitUp[];
extern u64	gAnm_RebSoldier_Rifle_Run[];
extern u64	gAnm_RebSoldier_Rifle_Strike[];
extern u64	gAnm_RebSoldier_WeaponRifle_Dust[];

extern u64	gAnm_RebSoldier_Mortar_CrouchWait[];
extern u64	gAnm_RebSoldier_Mortar_Shot[];
extern u64	gAnm_RebSoldier_Mortar_Shot_Dust[];
extern u64	gAnm_ShotEnemy_MortarRocket[];

extern u64	gAnm_RebSoldier_Std_Run[];
extern u64	gAnm_RebSoldier_Std_Parachuting[];
extern u64	gAnm_RebSoldier_Std_Idle[];
extern u64	gAnm_RebSoldier_Std_Jump[];
extern u64	gAnm_RebSoldier_Std_Fall[];
extern u64	gAnm_RebSoldier_Std_Shot_Gun[];
extern u64	gAnm_RebSoldier_Std_Shot_Handgrenade[];
extern u64	gAnm_ShotEnemy_StdHandgrenade[];

extern u64	gAnm_RebSoldier_LRAC_Idle[];
extern u64	gAnm_RebSoldier_LRAC_Run[];
extern u64	gAnm_RebSoldier_LRAC_Jump[];
extern u64	gAnm_RebSoldier_LRAC_Fall[];
extern u64	gAnm_RebSoldier_LRAC_Shot[];
extern u64	gAnm_BlackSmoke_Dust[];
extern u64	gAnm_RebSoldier_LRAC_Shot_Dust[];
extern u64	gAnm_ShotEnemy_LRAC_Rocket[];
extern u64	gAnm_RebSoldier_WeaponLRAC_Dust[];

extern u64	gAnm_RebSoldier_Shield_Idle[];
extern u64	gAnm_RebSoldier_Shield_Block[];
extern u64	gAnm_RebSoldier_Shield_Block2[];
extern u64	gAnm_RebSoldier_Shield_Run[];
extern u64	gAnm_RebSoldier_Shield_Jump[];
extern u64	gAnm_RebSoldier_Shield_Fall[];
extern u64	gAnm_RebSoldier_Shield_Strike[];
extern u64	gAnm_RebSoldier_WeaponShield_Dust[];

extern u64	gAnm_RebSoldier_Sunbath0_Idle[];
extern u64	gAnm_RebSoldier_Sunbath0_DeathStd_Dust[];

extern u64	gAnm_RebSoldier_Biker_Idle[];
extern u64	gAnm_RebSoldier_Biker_Drive[];
extern u64	gAnm_RebSoldier_Biker_Jump[];
extern u64	gAnm_RebSoldier_Biker_Land[];
extern u64	gAnm_RebSoldier_Biker_Land_Dust[];

extern u64	gAnm_TruckTrooper_Idle[];
extern u64	gAnm_TruckTrooper_Drive[];
extern u64	gAnm_TruckTrooper_TrunkOpen[];
extern u64	gAnm_TruckTrooper_DestroyedAnm[];

extern u64	gAnm_FlyingTara_DebrisBody1_Dust[];
extern u64	gAnm_FlyingTara_DebrisBody2_Dust[];
extern u64	gAnm_FlyingTara_DebrisWing_Dust[];
extern u64	gAnm_FlyingTara_DebrisEngine_Dust[];
extern u64	gAnm_FlyingTara_ShotFx[];

extern u64	gAnm_JellyfishSmall_Swim[];
extern u64	gAnm_JellyfishSmall_Death_Dust[];

extern u64	gAnm_Squid_Swim[];
extern u64	gAnm_Squid_Death_Dust[];

extern u64	gAnm_Whale_Attack[];
extern u64	gAnm_Whale_Bubbles_Dust[];

extern u64	gAnm_JellyfishBig_Swim[];
extern u64	gAnm_JellyfishBig_Death_Dust[];

extern u64	gAnm_Piranha_Swim[];
extern u64	gAnm_Piranha_Death_Dust[];

extern u64	gAnm_SensorMine_Idle[];

extern u64	gAnm_RocketDiver_Jump[];
extern u64	gAnm_RocketDiver_Fall[];
extern u64	gAnm_RocketDiver_Attack[];
extern u64	gAnm_ShotEnemy_RocketDiverRocket[];
extern u64	gAnm_RocketDiver_DeathFall[];
extern u64	gAnm_RocketDiver_DeathLand_Dust[];
extern u64	gAnm_RocketDiver_RocketRot_Dust[];
extern u64	gAnm_RocketDiver_WaterSplash_Dust[];

extern u64	gAnm_Girida0_Turret_ShotFx_Dust[];
extern u64	gAnm_Girida0_Turret_Idle[];
extern u64	gAnm_Girida0_Turret_Shot[];
extern u64	gAnm_Girida0_Body_Idle_NoFx[];
extern u64	gAnm_Girida0_Body_Idle[];
extern u64	gAnm_Girida0_Body_DriveForward[];
extern u64	gAnm_Girida0_Body_DriveBackwards[];
extern u64	gAnm_Girida0_Body_Shot[];
extern u64	gAnm_Girida0_Body_Destroyed[];
extern u64	gAnm_ShotEnemy_Girida0Bullet[];

extern u64	gAnm_Boss1_CannonUp_Idle[];
extern u64	gAnm_Boss1_CannonUp_Shot[];
extern u64	gAnm_Boss1_CannonUp_Destroyed[];
extern u64	gAnm_ShotEnemy_Boss1_ParachuteBomb[];
extern u64	gAnm_Boss1_CannonFront_Idle[];
extern u64	gAnm_Boss1_CannonFront_Shot[];
extern u64	gAnm_ShotEnemy_Boss1_BulletFront[];
extern u64	gAnm_Boss1_Tarp_Dust[];

extern u64	gAnm_Boss2A_FrontCannonGetUp[];
extern u64	gAnm_Boss2A_Turret_ShotSpark[];
extern u64	gAnm_Boss2A_FrontCannon_ShotSpark[];

extern u64	gAnm_Boss2B_FlameShot[];
extern u64	gAnm_Boss2B_FlameShot_Cut[];

extern u64	gAnm_ShotEnemy_HairBusterRibert_Mine[];
extern u64	gAnm_HairBusterRibert_HeatExhaust[];
extern u64	gAnm_HairBusterRibert_DoorOpening[];
extern u64	gAnm_HairBusterRibert_DoorClosed[];
extern u64	gAnm_HairBusterRibert_DoorClosing[];
extern u64	gAnm_HairBusterRibert_DoorClose_Dust[];
extern u64	gAnm_HairBusterRibert_SoldierJumpDive[];
extern u64	gAnm_HairBusterRibert_SoldierJumpDive_Dust[];

extern u64	gAnm_Monoeye_Idle[];
extern u64	gAnm_Monoeye_Appear[];

extern u64	gAnm_Core_Glow[];
extern u64	gAnm_CoreCannon_Idle[];
extern u64	gAnm_CoreCannon_Shot[];
extern u64	gAnm_CoreCannon_Bullet[];
extern u64	gAnm_CoreCannon_BulletSparkDust[];





