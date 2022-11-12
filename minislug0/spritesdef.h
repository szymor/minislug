
// Define priorités.
enum
{
	e_Prio_EnnemiesBg = 16,		// Plateformes, etc... derrière le reste
	e_Prio_DustUnder = 24,
	e_Prio_Ennemies = 32,		// 32 valeurs.
	e_Prio_DustOver = e_Prio_Ennemies + 32 + 16,
	e_Prio_Tirs = 140,
	e_Prio_Joueur = 192,

	e_Prio_HUD = 250,

};

// Define sprites.
enum
{
	e_Spr_FontSmall = 0,

	e_Spr_FontMission_Letters = e_Spr_FontSmall + 95+2,
	e_Spr_FontMission_Figures = e_Spr_FontMission_Letters + 26,
	e_Spr_FontMission_Chrs = e_Spr_FontMission_Figures + 10,
	e_Spr_FontMission_BigFigures = e_Spr_FontMission_Chrs + 2,

	e_Spr_HeroLegs_Stance = e_Spr_FontMission_BigFigures + 10,
	e_Spr_HeroLegs_Run = e_Spr_HeroLegs_Stance + 4,
	e_Spr_HeroLegs_Jump = e_Spr_HeroLegs_Run + 12,
	e_Spr_Hero_DeathGen_Gnd = e_Spr_HeroLegs_Jump + 6,			// Mort générique, perso au sol.
	e_Spr_Hero_DeathGen_Air = e_Spr_Hero_DeathGen_Gnd + 19,		// Mort générique, touché en l'air...
	e_Spr_Hero_DeathGen_Land = e_Spr_Hero_DeathGen_Air + 10,	// Mort générique, ...et atterissage.
	e_Spr_Hero_SlugOutGen = e_Spr_Hero_DeathGen_Land + 14,
	e_Spr_Hero_DeathGen_Plouf = e_Spr_Hero_SlugOutGen + 12,		// Mort générique, le perso tombe dans l'eau.
	e_Spr_Human_WaterStep = e_Spr_Hero_DeathGen_Plouf + 27,
	e_Spr_Parachute_Glide = e_Spr_Human_WaterStep + 10,
	e_Spr_Parachute_Fold = e_Spr_Parachute_Glide + 5,
	e_Spr_Weapon_Gun = e_Spr_Parachute_Fold + 17,				// Armes seules pour dust.
	e_Spr_Weapon_Shotgun,
	e_Spr_Weapon_Machinegun,

	e_Spr_HeroTop_Stance_Gun,
	e_Spr_HeroTop_StanceUp_Gun = e_Spr_HeroTop_Stance_Gun + 4,
	e_Spr_HeroTop_ThrowHG_Gun = e_Spr_HeroTop_StanceUp_Gun + 4,
	e_Spr_HeroTop_Run_Gun = e_Spr_HeroTop_ThrowHG_Gun + 5,
	e_Spr_HeroTop_Jump_Gun = e_Spr_HeroTop_Run_Gun + 12,
	e_Spr_HeroCrouch_Stance_Gun = e_Spr_HeroTop_Jump_Gun + 6,
	e_Spr_HeroCrouch_ThrowHG_Gun = e_Spr_HeroCrouch_Stance_Gun + 4,
	e_Spr_HeroCrouch_Walk_Gun = e_Spr_HeroCrouch_ThrowHG_Gun + 5,
	e_Spr_HeroTop_Shoot_Gun = e_Spr_HeroCrouch_Walk_Gun + 7,
	e_Spr_HeroCrouch_Shoot_Gun = e_Spr_HeroTop_Shoot_Gun + 7,
	e_Spr_HeroTop_LookUp_Gun = e_Spr_HeroCrouch_Shoot_Gun + 7,
	e_Spr_HeroTop_ShootUp_Gun = e_Spr_HeroTop_LookUp_Gun + 1,
	e_Spr_HeroTop_ShootDown_Gun = e_Spr_HeroTop_ShootUp_Gun + 7,
	e_Spr_Hero_SlugIn_Gun = e_Spr_HeroTop_ShootDown_Gun + 7,
	e_Spr_Hero_Victory_Gun = e_Spr_Hero_SlugIn_Gun + 6,
	e_Spr_HeroTop_Knife1_Gun = e_Spr_Hero_Victory_Gun + 4,
	e_Spr_HeroCrouch_Knife1_Gun = e_Spr_HeroTop_Knife1_Gun + 6,
	e_Spr_HeroTop_Bored0_Gun = e_Spr_HeroCrouch_Knife1_Gun + 6,
	e_Spr_HeroTop_Bored1_Gun = e_Spr_HeroTop_Bored0_Gun + 10,

	e_Spr_HeroTop_Stance_Shotgun = e_Spr_HeroTop_Bored1_Gun + 18,
	e_Spr_HeroTop_StanceUp_Shotgun = e_Spr_HeroTop_Stance_Shotgun + 4,
	e_Spr_HeroTop_ThrowHG_Shotgun = e_Spr_HeroTop_StanceUp_Shotgun + 4,
	e_Spr_HeroTop_Run_Shotgun = e_Spr_HeroTop_ThrowHG_Shotgun + 5,
	e_Spr_HeroTop_Jump_Shotgun = e_Spr_HeroTop_Run_Shotgun + 12,
	e_Spr_HeroCrouch_Stance_Shotgun = e_Spr_HeroTop_Jump_Shotgun + 6,
	e_Spr_HeroCrouch_ThrowHG_Shotgun = e_Spr_HeroCrouch_Stance_Shotgun + 4,
	e_Spr_HeroCrouch_Walk_Shotgun = e_Spr_HeroCrouch_ThrowHG_Shotgun + 5,
	e_Spr_HeroTop_Shoot_Shotgun = e_Spr_HeroCrouch_Walk_Shotgun + 7,
	e_Spr_HeroCrouch_Shoot_Shotgun = e_Spr_HeroTop_Shoot_Shotgun + 14,
	e_Spr_HeroTop_LookUp_Shotgun = e_Spr_HeroCrouch_Shoot_Shotgun + 14,
	e_Spr_HeroTop_ShootUp_Shotgun = e_Spr_HeroTop_LookUp_Shotgun + 1,
	e_Spr_HeroTop_ShootDown_Shotgun = e_Spr_HeroTop_ShootUp_Shotgun + 14,
	e_Spr_Hero_SlugIn_Shotgun = e_Spr_HeroTop_ShootDown_Shotgun + 14,
	e_Spr_Hero_Victory_Shotgun = e_Spr_Hero_SlugIn_Shotgun + 6,
	e_Spr_HeroTop_Knife1_Shotgun = e_Spr_Hero_Victory_Shotgun + 4,
	e_Spr_HeroCrouch_Knife1_Shotgun = e_Spr_HeroTop_Knife1_Shotgun + 6,
	e_Spr_HeroTop_Bored0_Shotgun = e_Spr_HeroCrouch_Knife1_Shotgun + 6,
	e_Spr_HeroTop_Bored1_Shotgun = e_Spr_HeroTop_Bored0_Shotgun + 12,

	e_Spr_HeroTop_Stance_Machinegun = e_Spr_HeroTop_Bored1_Shotgun + 8,
	e_Spr_HeroTop_StanceUp_Machinegun = e_Spr_HeroTop_Stance_Machinegun + 4,
	e_Spr_HeroTop_ThrowHG_Machinegun = e_Spr_HeroTop_StanceUp_Machinegun + 4,
	e_Spr_HeroTop_Run_Machinegun = e_Spr_HeroTop_ThrowHG_Machinegun + 5,
	e_Spr_HeroTop_Jump_Machinegun = e_Spr_HeroTop_Run_Machinegun + 12,
	e_Spr_HeroCrouch_Stance_Machinegun = e_Spr_HeroTop_Jump_Machinegun + 6,
	e_Spr_HeroCrouch_ThrowHG_Machinegun = e_Spr_HeroCrouch_Stance_Machinegun + 4,
	e_Spr_HeroCrouch_Walk_Machinegun = e_Spr_HeroCrouch_ThrowHG_Machinegun + 5,
	e_Spr_HeroTop_Shoot_Machinegun = e_Spr_HeroCrouch_Walk_Machinegun + 7,
	e_Spr_HeroCrouch_Shoot_Machinegun = e_Spr_HeroTop_Shoot_Machinegun + 8,
	e_Spr_HeroTop_LookUp_Machinegun = e_Spr_HeroCrouch_Shoot_Machinegun + 8,
	e_Spr_HeroTop_ShootUp_Machinegun = e_Spr_HeroTop_LookUp_Machinegun + 1,
	e_Spr_HeroTop_ShootDown_Machinegun = e_Spr_HeroTop_ShootUp_Machinegun + 8,
	e_Spr_Hero_SlugIn_Machinegun = e_Spr_HeroTop_ShootDown_Machinegun + 8,
	e_Spr_Hero_Victory_Machinegun = e_Spr_Hero_SlugIn_Machinegun + 6,
	e_Spr_HeroTop_Knife1_Machinegun = e_Spr_Hero_Victory_Machinegun + 4,
	e_Spr_HeroCrouch_Knife1_Machinegun = e_Spr_HeroTop_Knife1_Machinegun + 6,
	e_Spr_HeroTop_Bored0_Machinegun = e_Spr_HeroCrouch_Knife1_Machinegun + 6,

	e_Spr_HeroSubLegs_Run = e_Spr_HeroTop_Bored0_Machinegun + 8,	// (5+5) x 5 angles - medium = 2.
	e_Spr_HeroSub_DeathGen = e_Spr_HeroSubLegs_Run + 50,			// Mort générique sous l'eau.
	e_Spr_HeroSub_HelmetDeath = e_Spr_HeroSub_DeathGen + 23,
	e_Spr_HeroSub_SlugOutGen = e_Spr_HeroSub_HelmetDeath + 12,

	e_Spr_HeroSubTop_Run_Gun = e_Spr_HeroSub_SlugOutGen + 6,
	e_Spr_HeroSub_SlugIn_Gun = e_Spr_HeroSubTop_Run_Gun + 50,
	e_Spr_HeroSubTop_Shoot_Gun = e_Spr_HeroSub_SlugIn_Gun + 6,	// 7 spr x 5 angles.
	e_Spr_HeroSubTop_Knife1_Gun = e_Spr_HeroSubTop_Shoot_Gun + 35,	// 6 spr x 5 angles.

	e_Spr_HeroSubTop_Run_Shotgun = e_Spr_HeroSubTop_Knife1_Gun + 30,
	e_Spr_HeroSub_SlugIn_Shotgun = e_Spr_HeroSubTop_Run_Shotgun + 50,
	e_Spr_HeroSubTop_Shoot_Shotgun = e_Spr_HeroSub_SlugIn_Shotgun + 6,	// 14 spr x 5 angles.
	e_Spr_HeroSubTop_Knife1_Shotgun = e_Spr_HeroSubTop_Shoot_Shotgun + 70,	// 6 spr x 5 angles.

	e_Spr_Hero_InSlug_Sub = e_Spr_HeroSubTop_Knife1_Shotgun + 30,
	e_Spr_Hero_InSlug_Copter_GetIn = e_Spr_Hero_InSlug_Sub + 30,
	e_Spr_Hero_InSlug_Copter = e_Spr_Hero_InSlug_Copter_GetIn + 6,
	e_Spr_Hero_InSlug_Rocket = e_Spr_Hero_InSlug_Copter + 25,

	e_Spr_Shot_ShotgunH = e_Spr_Hero_InSlug_Rocket + 10,
	e_Spr_Shot_ShotgunV = e_Spr_Shot_ShotgunH + 12,
	e_Spr_Shot_BulletH = e_Spr_Shot_ShotgunV + 12,
	e_Spr_Shot_BulletH_Dust = e_Spr_Shot_BulletH + 1,
	e_Spr_Shot_BulletV = e_Spr_Shot_BulletH_Dust + 3,
	e_Spr_Shot_BulletV_Dust = e_Spr_Shot_BulletV + 1,
	e_Spr_Shot_MachgunH = e_Spr_Shot_BulletV_Dust + 3,
	e_Spr_Shot_MachgunV = e_Spr_Shot_MachgunH + 1,
	e_Spr_Shot_Grenade_Explo = e_Spr_Shot_MachgunV + 1,
	e_Spr_Shot_Grenade_Rotate = e_Spr_Shot_Grenade_Explo + 21,
	e_Spr_Shot_FlameH = e_Spr_Shot_Grenade_Rotate + 32,
	e_Spr_Shot_FlameV = e_Spr_Shot_FlameH + 14,
	e_Spr_Shot_RocketSmall = e_Spr_Shot_FlameV + 14,	// 32 frames.
	e_Spr_Shot_RocketSmall_Trail = e_Spr_Shot_RocketSmall + 32 + 32,

	e_Spr_Steps_Sand = e_Spr_Shot_RocketSmall_Trail + 16,
	e_Spr_Steps_WaterL13 = e_Spr_Steps_Sand + 8,

	e_Spr_Platform_1 = e_Spr_Steps_WaterL13 + 4,
	e_Spr_Platform_Lev5,
	e_Spr_Platform_Lev6,

	e_Spr_Lev2_Clouds = e_Spr_Platform_Lev6 + 1,

	e_Spr_Lev3_WallR_fg = e_Spr_Lev2_Clouds + 2,
	e_Spr_Lev3_DoorR = e_Spr_Lev3_WallR_fg + 1,		// 4 (différents états) + 1 (foreground qd porte cassée).
	e_Spr_Lev3_DoorR_fg = e_Spr_Lev3_DoorR + 4,

	e_Spr_Lev5_Fence_fg = e_Spr_Lev3_DoorR_fg + 1,
	e_Spr_Lev5_FenceBroken_fg = e_Spr_Lev5_Fence_fg + 1,

	e_Spr_Lev6_HardDoor = e_Spr_Lev5_FenceBroken_fg + 1,
	e_Spr_Lev6_Computer = e_Spr_Lev6_HardDoor + 5,
	e_Spr_Lev6_ComputerDestroyed = e_Spr_Lev6_Computer + 2,
	e_Spr_Lev6_Door2,
	e_Spr_Lev6_Door2_fg,

	e_Spr_Lev7_TubeUp_fg = e_Spr_Lev6_Door2_fg + 1,
	e_Spr_Lev7_TubeDown_fg = e_Spr_Lev7_TubeUp_fg + 8,
	e_Spr_Lev7_RockBottom = e_Spr_Lev7_TubeDown_fg + 8,
	e_Spr_Lev7_RockBottom_pf = e_Spr_Lev7_RockBottom + 8,

	e_Spr_Lev8_WagonL_fg = e_Spr_Lev7_RockBottom_pf + 1,
	e_Spr_Lev8_WagonR_fg = e_Spr_Lev8_WagonL_fg + 1,
	e_Spr_Lev8_Loco_fg = e_Spr_Lev8_WagonR_fg + 1,
	e_Spr_Lev8_Lever1 = e_Spr_Lev8_Loco_fg + 1,
	e_Spr_Lev8_Lock1 = e_Spr_Lev8_Lever1 + 3,
	e_Spr_Lev8_PF1Desc = e_Spr_Lev8_Lock1 + 3,
	e_Spr_Lev8_Door1Open = e_Spr_Lev8_PF1Desc + 4,
	e_Spr_Lev8_WagonDoorOpening = e_Spr_Lev8_Door1Open + 3,

	e_Spr_Lev9_RocketTopL0 = e_Spr_Lev8_WagonDoorOpening + 8,
	e_Spr_Lev9_RocketTopL1,
	e_Spr_Lev9_RocketTopR1,
	e_Spr_Lev9_RocketTopL2,
	e_Spr_Lev9_RocketTopR2,

	e_Spr_Lev11_SpaceJunk_Bkg = e_Spr_Lev9_RocketTopR2 + 1,
	e_Spr_Lev11_SpaceJunk_Fg = e_Spr_Lev11_SpaceJunk_Bkg + 4,

	e_Spr_Lev12_WallR_fg = e_Spr_Lev11_SpaceJunk_Fg + 3,

	e_Spr_Lev13_SubmarinePart1L_fg = e_Spr_Lev12_WallR_fg + 1,
	e_Spr_Lev13_SubmarinePart1R_fg,
	e_Spr_Lev13_SubmarinePart0_fg,
	e_Spr_Lev13_Rock0,
	e_Spr_Lev13_Rock0_pf,
	e_Spr_Lev13_Barrel0,
	e_Spr_Lev13_SandBagsHeap0,
	e_Spr_Lev13_SandBagsHeap0_pf,
	e_Spr_Lev13_WaterfallBottom_fg,
	e_Spr_Lev13_SwampPF1L_fg,
	e_Spr_Lev13_SwampPF1LBroken_fg,
	e_Spr_Lev13_SwampPF1R_fg,
	e_Spr_Lev13_SwampPF1RBroken_fg,
	e_Spr_Lev13_Rock1_Hanging,
	e_Spr_Lev13_Rock1_Falling_Shot = e_Spr_Lev13_Rock1_Hanging + 5,
	e_Spr_Lev13_Rock1_Falling,
	e_Spr_Lev13_Rock1_Landing,
	e_Spr_Lev13_Rock1_Landing_pf = e_Spr_Lev13_Rock1_Landing + 5,

	e_Spr_Lev14_DoorR = e_Spr_Lev13_Rock1_Landing_pf + 1,
	e_Spr_Lev14_DoorR_fg = e_Spr_Lev14_DoorR + 1,

	e_Spr_Lev16_ToolShed = e_Spr_Lev14_DoorR_fg + 1,
	e_Spr_Lev16_ToolShed_pf,

	e_Spr_Itm_AmmoBox1 = e_Spr_Lev16_ToolShed_pf + 1,
	e_Spr_Itm_WeaponCapsuleH = e_Spr_Itm_AmmoBox1 + 6,
	e_Spr_Itm_WeaponCapsuleF = e_Spr_Itm_WeaponCapsuleH + 4,
	e_Spr_Itm_WeaponCapsuleS = e_Spr_Itm_WeaponCapsuleF + 4,
	e_Spr_Itm_WeaponCapsuleR = e_Spr_Itm_WeaponCapsuleS + 4,
	e_Spr_Itm_WeaponCapsulePickUp = e_Spr_Itm_WeaponCapsuleR + 4,
	e_Spr_Itm_GasTank1 = e_Spr_Itm_WeaponCapsulePickUp + 6,
	e_Spr_Itm_Container1 = e_Spr_Itm_GasTank1 + 6,
	e_Spr_Itm_Container1_HardSpr = e_Spr_Itm_Container1 + 1,
	e_Spr_Itm_Container1_Dust = e_Spr_Itm_Container1_HardSpr + 1,

	e_Spr_Debris_Metal0 = e_Spr_Itm_Container1_Dust + 10,
	e_Spr_Debris_Wood0 = e_Spr_Debris_Metal0 + 8,

	e_Spr_JellyfishSmall_Swim = e_Spr_Debris_Wood0 + 10,
	e_Spr_JellyfishSmall_Death = e_Spr_JellyfishSmall_Swim + 10,

	e_Spr_Squid_Swim = e_Spr_JellyfishSmall_Death + 13,
	e_Spr_Squid_Death = e_Spr_Squid_Swim + 15,

	e_Spr_Whale_Mouth = e_Spr_Squid_Death + 12,		// L/R => 2 * 4 spr.
	e_Spr_Whale_BodyMid = e_Spr_Whale_Mouth + 8,
	e_Spr_Whale_BodyBotL,
	e_Spr_Whale_BodyBotR,
	e_Spr_Whale_Bubbles_Dust,

	e_Spr_JellyfishBig_Swim = e_Spr_Whale_Bubbles_Dust + 10,
	e_Spr_JellyfishBig_Death = e_Spr_JellyfishBig_Swim + 10,

	e_Spr_Piranha_Swim = e_Spr_JellyfishBig_Death + 12,
	e_Spr_Piranha_Death = e_Spr_Piranha_Swim + 12,

	e_Spr_SensorMine = e_Spr_Piranha_Death + 17,

	e_Spr_RebSoldier_DeathBullet = e_Spr_SensorMine + 8,
	e_Spr_RebSoldier_DeathShotgun = e_Spr_RebSoldier_DeathBullet + 22,
	e_Spr_RebSoldier_DeathFire = e_Spr_RebSoldier_DeathShotgun + 12,	// 8 (loop) + 14 (disparition).

	e_Spr_RebSoldier_Std_Run = e_Spr_RebSoldier_DeathFire + 8 + 14,
	e_Spr_RebSoldier_Std_GunShot = e_Spr_RebSoldier_Std_Run + 12,
	e_Spr_RebSoldier_Std_Idle = e_Spr_RebSoldier_Std_GunShot + 9,
	e_Spr_RebSoldier_Std_Flee = e_Spr_RebSoldier_Std_Idle + 4,		// 2 init + 12 loop course fuite.
	e_Spr_RebSoldier_Std_Jump = e_Spr_RebSoldier_Std_Flee + 14,
	e_Spr_RebSoldier_Std_HandgrenadeShot = e_Spr_RebSoldier_Std_Jump + 6,
	e_Spr_RebSoldier_Handgrenade_Bullet = e_Spr_RebSoldier_Std_HandgrenadeShot + 15,
	e_Spr_RebSoldier_Std_JumpDive = e_Spr_RebSoldier_Handgrenade_Bullet + 16,

	e_Spr_RebSoldier_Rifle_Shot = e_Spr_RebSoldier_Std_JumpDive + 12,
	e_Spr_RebSoldier_Rifle_Shot_Dust = e_Spr_RebSoldier_Rifle_Shot + 7,
	e_Spr_RebSoldier_Bullet = e_Spr_RebSoldier_Rifle_Shot_Dust + 8,
	e_Spr_RebSoldier_Weapon_Rifle = e_Spr_RebSoldier_Bullet + 2,		// Fusil seul pour dust.
	e_Spr_RebSoldier_Rifle_UpDown,
	e_Spr_RebSoldier_Rifle_CrouchWait = e_Spr_RebSoldier_Rifle_UpDown + 4,
	e_Spr_RebSoldier_Rifle_WaitUp = e_Spr_RebSoldier_Rifle_CrouchWait + 3,
	e_Spr_RebSoldier_Rifle_Run = e_Spr_RebSoldier_Rifle_WaitUp + 3,
	e_Spr_RebSoldier_Rifle_Strike = e_Spr_RebSoldier_Rifle_Run + 12,

	e_Spr_RebSoldier_Mortar_CrouchWait = e_Spr_RebSoldier_Rifle_Strike + 5,
	e_Spr_RebSoldier_Weapon_Mortar = e_Spr_RebSoldier_Mortar_CrouchWait + 4,
	e_Spr_RebSoldier_Mortar_Shot,
	e_Spr_RebSoldier_Mortar_Shot_Dust = e_Spr_RebSoldier_Mortar_Shot + 14,
	e_Spr_RebSoldier_Rocket = e_Spr_RebSoldier_Mortar_Shot_Dust + 8,

	e_Spr_RebSoldier_LRAC_Run = e_Spr_RebSoldier_Rocket + 32,
	e_Spr_RebSoldier_LRAC_Idle = e_Spr_RebSoldier_LRAC_Run + 12,
	e_Spr_RebSoldier_Weapon_LRAC = e_Spr_RebSoldier_LRAC_Idle + 4,
	e_Spr_RebSoldier_LRAC_Jump,
	e_Spr_RebSoldier_LRAC_Fright = e_Spr_RebSoldier_LRAC_Jump + 6,
	e_Spr_RebSoldier_LRAC_Aim,
	e_Spr_RebSoldier_LRAC_Shot = e_Spr_RebSoldier_LRAC_Aim + 9,
	e_Spr_RebSoldier_LRAC_Shot_Dust = e_Spr_RebSoldier_LRAC_Shot + 9,

	e_Spr_RebSoldier_Shield_Run = e_Spr_RebSoldier_LRAC_Shot_Dust + 15,
	e_Spr_RebSoldier_Shield_Idle = e_Spr_RebSoldier_Shield_Run + 12,
	e_Spr_RebSoldier_Shield_Block = e_Spr_RebSoldier_Shield_Idle + 4,
	e_Spr_RebSoldier_Shield_Jump = e_Spr_RebSoldier_Shield_Block + 4,
	e_Spr_RebSoldier_Shield_Strike = e_Spr_RebSoldier_Shield_Jump + 6,
	e_Spr_RebSoldier_Weapon_Shield = e_Spr_RebSoldier_Shield_Strike + 13,

	e_Spr_RebSoldier_Sunbath0_Idle = e_Spr_RebSoldier_Weapon_Shield + 8,
	e_Spr_RebSoldier_Sunbath0_Death = e_Spr_RebSoldier_Sunbath0_Idle + 4,

	e_Spr_RebSoldier_Biker_Drive = e_Spr_RebSoldier_Sunbath0_Death + 19,
	e_Spr_RebSoldier_Biker_Jump = e_Spr_RebSoldier_Biker_Drive + 3,
	e_Spr_RebSoldier_Biker_Land_Dust = e_Spr_RebSoldier_Biker_Jump + 3,
	e_Spr_Lev8_Cache1 = e_Spr_RebSoldier_Biker_Land_Dust + 7,

	e_Spr_RocketDiver_Jump = e_Spr_Lev8_Cache1 + 1,
	e_Spr_RocketDiver_Rocket_Rot = e_Spr_RocketDiver_Jump + 3,
	e_Spr_RocketDiver_Rocket_Bullet,
	e_Spr_RocketDiver_Shot = e_Spr_RocketDiver_Rocket_Bullet + 4,
	e_Spr_RocketDiver_Death = e_Spr_RocketDiver_Shot + 8,

	e_Spr_Girida0_Turret_Shot = e_Spr_RocketDiver_Death + 8,	// 4 images, 5 angles. Image 0 = Idle.
	e_Spr_Girida0_Bullet = e_Spr_Girida0_Turret_Shot + 20,
	e_Spr_Girida0_Body_Idle = e_Spr_Girida0_Bullet + 19,
	e_Spr_Girida0_ShotFx = e_Spr_Girida0_Body_Idle + 2,
	e_Spr_Girida0_Body_Shot = e_Spr_Girida0_ShotFx + 3,			// A coller sur la tourelle.
	e_Spr_Girida0_Body_Drive = e_Spr_Girida0_Body_Shot + 4,
	e_Spr_Girida0_Body_Destroyed = e_Spr_Girida0_Body_Drive + 10,

	e_Spr_BrainBot_Walk = e_Spr_Girida0_Body_Destroyed + 20,
	e_Spr_BrainBot_Flip = e_Spr_BrainBot_Walk + 6,
	e_Spr_BrainBot_Death = e_Spr_BrainBot_Flip + 12,
	e_Spr_BrainBot_BrainDust = e_Spr_BrainBot_Death + 13,
	e_Spr_BrainBot_ShotLow = e_Spr_BrainBot_BrainDust + 9,
	e_Spr_BrainBot_ShotHigh = e_Spr_BrainBot_ShotLow + 18,

	e_Spr_MarsPeople_Idle = e_Spr_BrainBot_ShotHigh + 18,
	e_Spr_MarsPeople_Shot = e_Spr_MarsPeople_Idle + 16,
	e_Spr_MarsPeople_Flip = e_Spr_MarsPeople_Shot + 11,
	e_Spr_MarsPeople_DeathGen = e_Spr_MarsPeople_Flip + 10,
	e_Spr_MarsPeople_DeathFire = e_Spr_MarsPeople_DeathGen + 43,

	e_Spr_MarsTIE_Fly = e_Spr_MarsPeople_DeathFire + 43,
	e_Spr_MarsTIE_Flip = e_Spr_MarsTIE_Fly + 16,
	e_Spr_MarsEye_Fly = e_Spr_MarsTIE_Flip + 6,
	e_Spr_MarsEye_Flip = e_Spr_MarsEye_Fly + 16,
	e_Spr_MarsUFO_Fly = e_Spr_MarsEye_Flip + 6,
	e_Spr_MarsUFO_ShotFx = e_Spr_MarsUFO_Fly + 12,
	e_Spr_MarsUFO_ShotDust = e_Spr_MarsUFO_ShotFx + 8,
	e_Spr_MarsUFO_ShotBullet = e_Spr_MarsUFO_ShotDust + 8,
	e_Spr_MarsUFO_ShotBullet_Dust = e_Spr_MarsUFO_ShotBullet + 8,

	e_Spr_MiniUFO_Fly = e_Spr_MarsUFO_ShotBullet_Dust + 3,
	e_Spr_MiniUFO_Explode = e_Spr_MiniUFO_Fly + 16,

	e_Spr_SpaceRock_Medium = e_Spr_MiniUFO_Explode + 24,
	e_Spr_SpaceRock_Small = e_Spr_SpaceRock_Medium + 8,
	e_Spr_Asteroid_Rotate = e_Spr_SpaceRock_Small + 8,

	e_Spr_BigAsteroid = e_Spr_Asteroid_Rotate + 15,

	e_Spr_AlienMothership = e_Spr_BigAsteroid + 16,		// (Top Left, Top Right, Bottom) x3.

	e_Spr_Pow_SittingWait = e_Spr_AlienMothership + 9,
	e_Spr_Pow_SittingFree = e_Spr_Pow_SittingWait + 5,
	e_Spr_Pow_TotemWait = e_Spr_Pow_SittingFree + 4,
	e_Spr_Pow_TotemAlt = e_Spr_Pow_TotemWait + 3,
	e_Spr_Pow_TotemFree = e_Spr_Pow_TotemAlt + 5,
	e_Spr_Pow_TotemDust = e_Spr_Pow_TotemFree + 3,
	e_Spr_Pow_Idle = e_Spr_Pow_TotemDust + 7,
	e_Spr_Pow_BigPants = e_Spr_Pow_Idle + 1,
	e_Spr_Pow_Salute = e_Spr_Pow_BigPants + 10,
	e_Spr_Pow_Fall = e_Spr_Pow_Salute + 11,
	e_Spr_Pow_Run = e_Spr_Pow_Fall + 5,
	e_Spr_Pow_HangingWait = e_Spr_Pow_Run + 8,
	e_Spr_Pow_HangingRopeEnd_Dust = e_Spr_Pow_HangingWait + 5,

	e_Spr_AikawaRumi_Walk = e_Spr_Pow_HangingRopeEnd_Dust + 7,
	e_Spr_AikawaRumi_GiveItem = e_Spr_AikawaRumi_Walk + 16,
	e_Spr_AikawaRumi_Flee = e_Spr_AikawaRumi_GiveItem + 15,

	e_Spr_RShobu_Flying = e_Spr_AikawaRumi_Flee + 9,
	e_Spr_RShobu_Flip = e_Spr_RShobu_Flying + 7,
	e_Spr_RShobu_Blades = e_Spr_RShobu_Flip + 7,
	e_Spr_Shot_RShobu_Bomb = e_Spr_RShobu_Blades + (5 * 7),
	e_Spr_RShobu_BombDrop_Dust = e_Spr_Shot_RShobu_Bomb + 7,
	e_Spr_Explo0_Big = e_Spr_RShobu_BombDrop_Dust + 8,
	e_Spr_Explo0_Medium = e_Spr_Explo0_Big + 28,
	e_Spr_Explo0_Medium_LargeTouch = e_Spr_Explo0_Medium + 28,

	e_Spr_Masknell_Body = e_Spr_Explo0_Medium_LargeTouch + 6,
	e_Spr_Masknell_Blades = e_Spr_Masknell_Body + 8,
	e_Spr_Masknell_ShotFx = e_Spr_Masknell_Blades + (8*6),

	e_Spr_FlyingTara_Flying = e_Spr_Masknell_ShotFx + (8*2),
	e_Spr_FlyingTara_BodyBomb = e_Spr_FlyingTara_Flying + 5,
	e_Spr_FlyingTara_Propeller = e_Spr_FlyingTara_BodyBomb + 5,
	e_Spr_FlyingTara_ShotFx = e_Spr_FlyingTara_Propeller + 9,
	e_Spr_FlyingTara_DebrisBody1 = e_Spr_FlyingTara_ShotFx + 4,
	e_Spr_FlyingTara_DebrisBody2 = e_Spr_FlyingTara_DebrisBody1 + 4,
	e_Spr_FlyingTara_DebrisWing = e_Spr_FlyingTara_DebrisBody2 + 4,
	e_Spr_FlyingTara_DebrisEngine = e_Spr_FlyingTara_DebrisWing + 8,

	e_Spr_Ribert_P0_0 = e_Spr_FlyingTara_DebrisEngine + 8,
	e_Spr_Ribert_P2_0,
	e_Spr_Ribert_P0_1,
	e_Spr_Ribert_P2_1,
	e_Spr_Ribert_P0_2,
	e_Spr_Ribert_P2_2,
	e_Spr_Ribert_P0_3,
	e_Spr_Ribert_P2_3,
	e_Spr_Ribert_P1_012,
	e_Spr_Ribert_P1_3,
	e_Spr_Ribert_EnginePropeller,
	e_Spr_Ribert_EngineExhaust = e_Spr_Ribert_EnginePropeller + 18,
	e_Spr_Ribert_InTheDistance = e_Spr_Ribert_EngineExhaust + 7,
	e_Spr_Ribert_Mine = e_Spr_Ribert_InTheDistance + 2,
	e_Spr_Ribert_DoorOpening = e_Spr_Ribert_Mine + 8,
	e_Spr_Ribert_DoorDust = e_Spr_Ribert_DoorOpening + 21,

	e_Spr_ZombieTeen_Idle = e_Spr_Ribert_DoorDust + 6,
	e_Spr_ZombieTeen_Walk = e_Spr_ZombieTeen_Idle + 7,
	e_Spr_ZombieTeen_Fall = e_Spr_ZombieTeen_Walk + 16,
	e_Spr_ZombieTeen_Rise = e_Spr_ZombieTeen_Fall + 13,
	e_Spr_ZombieTeen_Flip = e_Spr_ZombieTeen_Rise + 12,
	e_Spr_ZombieTeen_Shot = e_Spr_ZombieTeen_Flip + 9,
	e_Spr_ZombieTeen_DeathBurnWalk = e_Spr_ZombieTeen_Shot + 20,
	e_Spr_ZombieTeen_DeathBurn_Dust = e_Spr_ZombieTeen_DeathBurnWalk + 24,
	e_Spr_ZombieTeen_ShotFX_Dust = e_Spr_ZombieTeen_DeathBurn_Dust + 17,
	e_Spr_Blood_FX0_Dust = e_Spr_ZombieTeen_ShotFX_Dust + 12,
	e_Spr_Zombie_DeathShotgunGen = e_Spr_Blood_FX0_Dust + 8,		// Mort générique des zombies au shotgun.
	e_Spr_Shot_ZombieTeen_Bullet = e_Spr_Zombie_DeathShotgunGen + 15,
	e_Spr_Shot_ZombieTeen_DustAir = e_Spr_Shot_ZombieTeen_Bullet + 13,
	e_Spr_Shot_ZombieTeen_DustGnd = e_Spr_Shot_ZombieTeen_DustAir + 16,

	e_Spr_ZombieFat_Idle = e_Spr_Shot_ZombieTeen_DustGnd + 19,
	e_Spr_ZombieFat_Walk = e_Spr_ZombieFat_Idle + 11,
	e_Spr_ZombieFat_Fall = e_Spr_ZombieFat_Walk + 14,
	e_Spr_ZombieFat_Rise = e_Spr_ZombieFat_Fall + 13,
	e_Spr_ZombieFat_Flip = e_Spr_ZombieFat_Rise + 12,
	e_Spr_ZombieFat_Shot = e_Spr_ZombieFat_Flip + 9,
	e_Spr_ZombieFat_ShotFX_Dust = e_Spr_ZombieFat_Shot + 14,
	e_Spr_ZombieFat_DeathBurnWalk = e_Spr_ZombieFat_ShotFX_Dust + 12,
	e_Spr_ZombieFat_DeathBurn_Dust = e_Spr_ZombieFat_DeathBurnWalk + 24,

	e_Spr_ZombieBits_Arm1 = e_Spr_ZombieFat_DeathBurn_Dust + 17,
	e_Spr_ZombieBits_Arm2 = e_Spr_ZombieBits_Arm1 + 16,
	e_Spr_ZombieBits_Leg1 = e_Spr_ZombieBits_Arm2 + 16,
	e_Spr_ZombieBits_Leg2 = e_Spr_ZombieBits_Leg1 + 16,

	e_Spr_VulcanCannon_Still = e_Spr_ZombieBits_Leg2 + 16,	// 32 frames de rotation.
	e_Spr_VulcanCannon_Shooting = e_Spr_VulcanCannon_Still + 32,		// 4 frames de tir, x32 frames.
	e_Spr_Shot_VulcanCannon_Bullet = e_Spr_VulcanCannon_Shooting + (32 * 4),	// 32 frames.
	e_Spr_Shot_VulcanCannon_Bullet_Dust = e_Spr_Shot_VulcanCannon_Bullet + 32,

	e_Spr_SlugSubmarine_Body = e_Spr_Shot_VulcanCannon_Bullet_Dust + 3,
	e_Spr_SlugSubmarine_Close = e_Spr_SlugSubmarine_Body + 30,
	e_Spr_Shot_SubmarineRocket_Bullet = e_Spr_SlugSubmarine_Close + 6,
	e_Spr_SubmarineBubbles_Dust,
	e_Spr_Shot_SubmarineRocket_Trail_Dust = e_Spr_SubmarineBubbles_Dust + 14,
	e_Spr_SlugSubmarine_WaitOpen = e_Spr_Shot_SubmarineRocket_Trail_Dust + 8,
	e_Spr_SlugSubmarine_WaitOpenSplash,

	e_Spr_SlugTank_Close = e_Spr_SlugSubmarine_WaitOpenSplash + 4,
	e_Spr_SlugTank_Idle = e_Spr_SlugTank_Close + 4,
	e_Spr_SlugTank_Drive = e_Spr_SlugTank_Idle + 2,
	e_Spr_SlugTank_DriveStop = e_Spr_SlugTank_Drive + 20,
	e_Spr_SlugTank_DownUp = e_Spr_SlugTank_DriveStop + 4,
	e_Spr_SlugTankCrouch_Idle = e_Spr_SlugTank_DownUp + 8,
	e_Spr_SlugTankCrouch_Drive = e_Spr_SlugTankCrouch_Idle + 2,
	e_Spr_SlugTankCrouch_DriveStop = e_Spr_SlugTankCrouch_Drive + 20,
	e_Spr_SlugTank_Jump = e_Spr_SlugTankCrouch_DriveStop + 4,
	e_Spr_SlugTank_ShotAnm = e_Spr_SlugTank_Jump + 8,
	e_Spr_Slug_WaterPlouf = e_Spr_SlugTank_ShotAnm + 3,

	e_Spr_SlugCopter_WaitPlayer = e_Spr_Slug_WaterPlouf + 17,
	e_Spr_SlugCopter_Close_Body = e_Spr_SlugCopter_WaitPlayer + 6,
	e_Spr_SlugCopter_Close_Blades = e_Spr_SlugCopter_Close_Body + 6,
	e_Spr_SlugCopter_Body = e_Spr_SlugCopter_Close_Blades + 6,		// 5 inclinaisons * 5 angles.
	e_Spr_SlugCopter_Blades = e_Spr_SlugCopter_Body + 25,		// 9 images * 5 angles * 5 inclinaisons.
	e_Spr_Shot_Helicopter_Bomb = e_Spr_SlugCopter_Blades + 225,

	e_Spr_SlugRocket_Idle = e_Spr_Shot_Helicopter_Bomb + 9,
	e_Spr_SlugRocket_MainThrustersV = e_Spr_SlugRocket_Idle + 4,
	e_Spr_SlugRocket_MainThrustersR = e_Spr_SlugRocket_MainThrustersV + 5,
	e_Spr_SlugRocket_MainThrustersTrail_Dust = e_Spr_SlugRocket_MainThrustersR + 5,
	e_Spr_SlugRocket_ThrustPushUp = e_Spr_SlugRocket_MainThrustersTrail_Dust + 5,
	e_Spr_SlugRocket_ThrustPushDown = e_Spr_SlugRocket_ThrustPushUp + 3,
	e_Spr_SlugRocket_ThrustPushLR = e_Spr_SlugRocket_ThrustPushDown + 3,
	e_Spr_SlugRocket_GunL = e_Spr_SlugRocket_ThrustPushLR + 6,
	e_Spr_SlugRocket_GunR = e_Spr_SlugRocket_GunL + 12,
	e_Spr_SlugRocket_ShotSparkMain = e_Spr_SlugRocket_GunR + 12,
	e_Spr_SlugRocket_ShotSparkSide = e_Spr_SlugRocket_ShotSparkMain + 4,
	e_Spr_SlugRocket_WaitPlayer = e_Spr_SlugRocket_ShotSparkSide + 4,
	e_Spr_SlugRocket_Close_Body,
	e_Spr_SlugRocket_Close_Door,
	e_Spr_SlugRocket_TakeOffTrail = e_Spr_SlugRocket_Close_Door + 9,
	e_Spr_Shot_Rocket_Bomb,

	e_Spr_Truck0_Drive = e_Spr_Shot_Rocket_Bomb + 4,
	e_Spr_Truck0_TrunkOver = e_Spr_Truck0_Drive + 4,
	e_Spr_Truck0_TrunkOpen = e_Spr_Truck0_TrunkOver + 1,
	e_Spr_Truck0_Destroyed = e_Spr_Truck0_TrunkOpen + 12,
	e_Spr_Truck0_Destroyed_Pf = e_Spr_Truck0_Destroyed + 1,
	e_Spr_Truck0_DestroyedAnm = e_Spr_Truck0_Destroyed_Pf + 1,

	e_Spr_SuspendedSub0 = e_Spr_Truck0_DestroyedAnm + 6,
	e_Spr_SuspendedSub0_fg = e_Spr_SuspendedSub0 + 4,

	e_Spr_Boss1_P1_1 = e_Spr_SuspendedSub0_fg + 4,
	e_Spr_Boss1_P1_2345,
	e_Spr_Boss1_P2_12,
	e_Spr_Boss1_P2_345,
	e_Spr_Boss1_P3_12345,
	e_Spr_Boss1_P4_12,
	e_Spr_Boss1_P4_345,
	e_Spr_Boss1_P5_12,
	e_Spr_Boss1_P5_3,
	e_Spr_Boss1_P5_4,
	e_Spr_Boss1_P5_5,
	e_Spr_Boss1_P10_1,
	e_Spr_Boss1_P10_2345,
	e_Spr_Boss1_P6_12,
	e_Spr_Boss1_P6_3,
	e_Spr_Boss1_P7_123,
	e_Spr_Boss1_P7_4,
	e_Spr_Boss1_P7_5,
	e_Spr_Boss1_P8_12,
	e_Spr_Boss1_P8_3,
	e_Spr_Boss1_P8_4,
	e_Spr_Boss1_P9_123,
	e_Spr_Boss1_P9_45,
	e_Spr_Boss1_CannonUp_Shot,
	e_Spr_Boss1_CannonUp_Destroyed = e_Spr_Boss1_CannonUp_Shot + 21,
	e_Spr_Boss1_BombeParachute,
	e_Spr_Boss1_CannonFront_Idle = e_Spr_Boss1_BombeParachute + 5,
	e_Spr_Boss1_CannonFront_Shot,
	e_Spr_Boss1_CannonFront_Bullet = e_Spr_Boss1_CannonFront_Shot + 14,
	e_Spr_Boss1_Tarp_Idle = e_Spr_Boss1_CannonFront_Bullet + 4,
	e_Spr_Boss1_Tarp_Dust,

	e_Spr_Boss2Front_P0_0 = e_Spr_Boss1_Tarp_Dust + 15,
	e_Spr_Boss2Front_P1_0,
	e_Spr_Boss2Back_P0_0,
	e_Spr_Boss2Back_P1_0,
	e_Spr_Boss2Front_P2_0,
	e_Spr_Boss2Front_P3_0,
	e_Spr_Boss2Back_P2_0,
	e_Spr_Boss2Back_P3_0,
	e_Spr_Boss2Front_P0_1,
	e_Spr_Boss2Front_P1_1,
	e_Spr_Boss2Back_P0_1,
	e_Spr_Boss2Back_P1_1,
	e_Spr_Boss2Front_P2_1,
	e_Spr_Boss2Front_P3_1,
	e_Spr_Boss2Back_P2_1,
	e_Spr_Boss2Back_P3_1,
	e_Spr_Boss2Front_PilotTop,
	e_Spr_Boss2Front_PilotBottom = e_Spr_Boss2Front_PilotTop + 10,
	e_Spr_Boss2Back_Propeller = e_Spr_Boss2Front_PilotBottom + 10,
	e_Spr_Boss2Back_TailTop = e_Spr_Boss2Back_Propeller +4+1+1,		// Propeller : Loop 4 / ? 1 / Destroyed 1.
	e_Spr_Boss2Back_TailBottom = e_Spr_Boss2Back_TailTop + 9,
	e_Spr_Boss2Back_BodyTailBottomPiece2 = e_Spr_Boss2Back_TailBottom + 9,
	e_Spr_Boss2Back_WingTop,
	e_Spr_Boss2Back_WingBottom,
	e_Spr_Boss2Front_Turret,
	e_Spr_Boss2Back_P0_ShotMask = e_Spr_Boss2Front_Turret + 17,
	e_Spr_Boss2Front_FrontCannonTop_GetUp,
	e_Spr_Boss2Front_FrontCannonTop_Destroyed = e_Spr_Boss2Front_FrontCannonTop_GetUp + 13,
	e_Spr_Boss2Front_FrontCannonBottom_Destroyed,
	e_Spr_Boss2Front_FrontCannonBottom_GetUp,
	e_Spr_Boss2Front_Turret_ShotSpark = e_Spr_Boss2Front_FrontCannonBottom_GetUp + 13,	// 17 * 3
	e_Spr_Boss2Front_FrontCannon_ShotSpark = e_Spr_Boss2Front_Turret_ShotSpark + (17 * 3),
	e_Spr_Boss2Back_FlameShot = e_Spr_Boss2Front_FrontCannon_ShotSpark + 8,
	e_Spr_Boss2_InTheDistance_HalfHealth = e_Spr_Boss2Back_FlameShot + 52,
	e_Spr_Boss2_InTheDistance_FullHealth = e_Spr_Boss2_InTheDistance_HalfHealth + 4,
	e_Spr_Boss2_InTheDistance_HalfHealth_Smoke1 = e_Spr_Boss2_InTheDistance_FullHealth + 3,
	e_Spr_Boss2_InTheDistance_HalfHealth_Smoke2 = e_Spr_Boss2_InTheDistance_HalfHealth_Smoke1 + 4,

	e_Spr_Core_Base = e_Spr_Boss2_InTheDistance_HalfHealth_Smoke2 + 4,
	e_Spr_Core_BaseDestroyed_fg,
	e_Spr_Core_BaseDestroyed_bg,
	e_Spr_Core_CoreDestroyed,
	e_Spr_Core_CoreGlow,
	e_Spr_CoreCannon_Idle = e_Spr_Core_CoreGlow + 5,
	e_Spr_CoreCannon_Shot,
	e_Spr_CoreCannon_Bullet = e_Spr_CoreCannon_Shot + 12,
	e_Spr_CoreCannon_SparkDust = e_Spr_CoreCannon_Bullet + 4,

	e_Spr_Monoeye_Appear = e_Spr_CoreCannon_SparkDust + 12,
	e_Spr_Monoeye_Idle = e_Spr_Monoeye_Appear + 6,

	e_Spr_SlugInSign = e_Spr_Monoeye_Idle + 1,
	e_Spr_SlugOutSign = e_Spr_SlugInSign + 16,
	e_Spr_HUD_SlugGauge = e_Spr_SlugOutSign + 16,	// 1 spr jauge / 8 spr de remplissage de 1 à 8.
	e_Spr_HUD_ArmsFrameTopLeft = e_Spr_HUD_SlugGauge + 1 + 8,
	e_Spr_HUD_ArmsFrameTopRight = e_Spr_HUD_ArmsFrameTopLeft + 1,
	e_Spr_HUD_ArmsFrameBottom = e_Spr_HUD_ArmsFrameTopRight + 3,
	e_Spr_HUD_Prisoner = e_Spr_HUD_ArmsFrameBottom + 1,
	e_Spr_HUD_HeroHeads = e_Spr_HUD_Prisoner + 1,

	e_Spr_MSE_BarTop = e_Spr_HUD_HeroHeads + 1,
	e_Spr_MSE_Plate,
	e_Spr_MSE_BarBottom,

	e_Spr_Tstrct_Cross = e_Spr_MSE_BarBottom + 1,
	e_Spr_Tstrct_CornerUL,
	e_Spr_Tstrct_CornerUR,
	e_Spr_Tstrct_CornerDL,
	e_Spr_Tstrct_CornerDR,

	e_Spr_HowToPlay_Stick = e_Spr_Tstrct_CornerDR + 1,
	e_Spr_HowToPlay_ButtonA = e_Spr_HowToPlay_Stick + 5,
	e_Spr_HowToPlay_ButtonB = e_Spr_HowToPlay_ButtonA + 3,
	e_Spr_HowToPlay_ButtonC = e_Spr_HowToPlay_ButtonB + 3,

	e_Spr_MS_Logo = e_Spr_HowToPlay_ButtonC + 3,		// En dernier, comme ça on voit tout de suite s'il y a un problème.

	e_Spr_NEXT = e_Spr_MS_Logo + 1,

};

