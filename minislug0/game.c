
#include "includes.h"

// Tank : Restriction : Le tank n'est pas fait pour les "marches" verticales.
// 		J'ai fait un truc qui fonctionne pas mal et qui dans ce cas garde le tank dans son inclinaison.
//		MAIS, il faut qu'en cas de "marche", la pente en haut et en bas de la "marche" soient les mêmes,
//		à cause du décalage dû au rotozoom.

//#define DEBUG_KEYS	1		// A commenter.

#define	PLAYER_Lives_Start	3
#define	PLAYER_Lives_Max	9

// En mettre au moins un des deux !
#define	POW_RESET_EACH_CREDIT	1		// Reset du nombre de POW à chaque crédit.
//#define	POW_RESET_EACH_LIFE		1		// Reset du nombre de POW à chaque vie (mode MS, mais c'est dur...).
#if !defined(POW_RESET_EACH_CREDIT) && !defined(POW_RESET_EACH_LIFE)
#error POW: POW_RESET_EACH_CREDIT & POW_RESET_EACH_LIFE not defined!
#endif
#if defined(POW_RESET_EACH_CREDIT) && defined(POW_RESET_EACH_LIFE)
#error POW: POW_RESET_EACH_CREDIT & POW_RESET_EACH_LIFE both defined!
#endif

#define	PLYR_INVULNERABILTY_CNT	120
#define	PLYR_INVL_BLINK	2//8			// Masque pour clignotement.

#define	PLYR_NAKED_BOMBS_DEFAULT	10//50
#define	PLYR_NAKED_BOMBS_MAX		20//75

#define	PLYR_NAKED_SPDX_MAX	0x280
#define PLYR_NAKED_SPDY_MAX	SPDY_MAX
#define	PLYR_NAKED_ACCX		0x30
#define	PLYR_NAKED_JUMP_VAL	0x600
#define	PLYR_NAKED_REF_OFFSETX	0x700	// Décalage dte/gauche par rapport au point de ref pour collisions.
#define	PLYR_NAKED_HEIGHT	40//48

#define	PLYR_SWIM_SPDX_MAX	0x100
#define PLYR_SWIM_SPDY_MAX	0x100
#define	PLYR_SWIM_ACC		0x08
#define	PLYR_SWIM_REF_OFFSETX	0x1000
#define	PLYR_SWIM_HEIGHT	24
#define	PLYR_SWIM_HEIGHT_B	8
#define	PLYR_SWIM_DEFAULTSPRADD	2

#define PLYR_PARACHUTE_SPDY_MAX	0x100

#define	SLUG_ENERGY_ALERT	(SLUG_ENERGY_MAX / 8)

#define	SLUG_SUBMARINE_SPDX_MAX	0x100
#define SLUG_SUBMARINE_SPDY_MAX	0x100
#define	SLUG_SUBMARINE_ACC		0x08 // 0x20
#define	SLUG_SUBMARINE_REF_OFFSETX	0x1000
#define	SLUG_SUBMARINE_HEIGHT	40//48
#define	SLUG_SUBMARINE_HEIGHT_B	8
#define	SLUG_SUBMARINE_DEFAULTSPRADD	15

#define	SLUG_TANK_SPDX_MAX	0x280
#define SLUG_TANK_SPDY_MAX	SPDY_MAX
#define	SLUG_TANK_ACCX		0x30
#define	SLUG_TANK_JUMP_VAL	0x600
#define	SLUG_TANK_REF_OFFSETX	0x0F00//0x1000
#define	SLUG_TANK_HEIGHT	40//48

#define	SLUG_HELICOPTER_SPDX_MAX	0x200
#define SLUG_HELICOPTER_SPDY_MAX	0x200
#define	SLUG_HELICOPTER_ACC		0x10
#define	SLUG_HELICOPTER_REF_OFFSETX	0x1000
#define	SLUG_HELICOPTER_HEIGHT	40//48
#define	SLUG_HELICOPTER_HEIGHT_B	8
#define	SLUG_HELICOPTER_DEFAULTSPRADD	0x2828

#define	SLUG_ROCKET_SPDX_MAX	0x180
#define SLUG_ROCKET_SPDY_MAX	0x180
#define	SLUG_ROCKET_ACC		0x10
#define	SLUG_ROCKET_REF_OFFSETX	0x1000
#define	SLUG_ROCKET_HEIGHT	64//40//48

enum
{
	e_SlugPhase_GetIn = 0,
	e_SlugPhase_Normal,
	e_SlugPhase_GetOut,
};

//=============================================================================

// Tableau des missions.
struct SLevRecord
{
	s8	nLevelNo;		// N° du levXX à charger. -1 si terminé.
	s8	nMissionNo;		// N° de mission a afficher. -1 si rien a afficher. 0 pour cas particuliers how to play + credits.
	u8	nVehicleType;
	u8	nScrollType;
	char	*pMissionName;	// Nom de la mission.

	s8	nMusicNo;
};	// + Pour changer de niveau, on attend l'item de quête 255.

//	e_ScrollType_Free / e_ScrollType_RightOnly,
struct SLevRecord	gMissionTb[] =
{
// How to play. !!! Mission n°0 !!!
{  4, 0, e_HeroVehicle_None_Naked, e_ScrollType_RightOnly, "", e_YmMusic_NoMusic },
{ -1, 0, e_HeroVehicle_None_Naked, e_ScrollType_Free, "", e_YmMusic_NoMusic },

// Credits. !!! Mission n°0 !!!
{  2, 0, e_HeroVehicle_HelicopterNoExit, e_ScrollType_RightOnly, "", e_YmMusic_Credits },
{ -1, 0, e_HeroVehicle_None_Naked, e_ScrollType_Free, "", e_YmMusic_NoMusic },

// Niveaux normaux.
/*
Mission 1:
Lev 01 : Désert.
Lev 13 : Jungle + Boss. (Arrivée en parachute).

Mission 2:
Lev 07 : Underwater.
Lev 15 : Transit Underwater > Base sous marine.
Lev 05 : Base sous marine. (+slug tank ?)
Lev 06 : Plateformes.
Lev 16 : Transit vers hélico.
Lev 02 : Hélico + Boss.

Mission 3:
Lev 08 : Train. (Arrivée en parachute). (+slug tank & slug helico ?)
Lev 09 : Transit Train > Fusée.
Lev 11 : Fusée.
Lev 14 : Mothership.
Lev 17 : Mothership core room. (Faux boss).

How to play:
Lev 04 : Ville détruite.

Unused:
Lev 10 : Test.
Lev 12 : Transit Fusée > Factory.
Lev 03 : Factory.

*/



//{  1,  1, e_HeroVehicle_None_Parachute, e_ScrollType_Free, "M1-1 Desert" },
/*
{ 16, 1, e_HeroVehicle_None_Naked, e_ScrollType_RightOnly, "M2-5 On The Roof" },
{  2, 1, e_HeroVehicle_HelicopterNoExit, e_ScrollType_Free, "M2-6 Sky Battle" },
{  5, -1, e_HeroVehicle_None_Naked, e_ScrollType_RightOnly, "M2-3 Docks" },
{  7,  2, e_HeroVehicle_None_Swim, e_ScrollType_RightOnly, "M2-1 Underwater" },
*/


// Mission 1.
{  1,  1, e_HeroVehicle_None_Parachute, e_ScrollType_RightOnly, "M1-1 Desert", e_YmMusic_Mission1 },
{ 13, -1, e_HeroVehicle_None_Naked, e_ScrollType_RightOnly, "M1-2 Jungle", e_YmMusic_Mission1 },

//	{ -1,  0, e_HeroVehicle_None_Naked, e_ScrollType_Free, "", e_YmMusic_NoMusic },	// < Fin de la preview.

// Mission 2.
{  7,  2, e_HeroVehicle_None_Swim, e_ScrollType_RightOnly, "M2-1 Underwater", e_YmMusic_Mission2 },
{ 15, -1, e_HeroVehicle_None_Naked, e_ScrollType_RightOnly, "M2-2 Docks Entrance", e_YmMusic_Mission2 },
{  5, -1, e_HeroVehicle_None_Naked, e_ScrollType_RightOnly, "M2-3 Docks", e_YmMusic_Mission2 },
{  6, -1, e_HeroVehicle_None_Naked, e_ScrollType_Free, "M2-4 Factory", e_YmMusic_Mission2 },
{ 16, -1, e_HeroVehicle_None_Naked, e_ScrollType_RightOnly, "M2-5 On The Roof", e_YmMusic_Mission2 },
{  2, -1, e_HeroVehicle_HelicopterNoExit, e_ScrollType_Free, "M2-6 Sky Battle", e_YmMusic_Mission2 },

// Mission 3.
{  8,  3, e_HeroVehicle_None_Parachute, e_ScrollType_RightOnly, "M3-1 Rebel Train", e_YmMusic_Mission3 },
{  9, -1, e_HeroVehicle_None_Naked, e_ScrollType_RightOnly, "M3-2 Spaceport", e_YmMusic_Mission3 },
{ 11, -1, e_HeroVehicle_Rocket, e_ScrollType_Free, "M3-3 To Infinity...", e_YmMusic_Mission3 },
{ 14, -1, e_HeroVehicle_None_Naked, e_ScrollType_RightOnly, "M3-4 Mothership", e_YmMusic_Mission3 },
{ 17, -1, e_HeroVehicle_None_Naked, e_ScrollType_Free, "M3-5 Final Room", e_YmMusic_Mission3 },

{ -1,  0, e_HeroVehicle_None_Naked, e_ScrollType_Free, "", e_YmMusic_NoMusic },	// Fin.

};

// Renvoie le nom du niveau (pour sélecteur de niveau).
char * Level_NameGet(u32 nLevelNo)
{
	return (gMissionTb[nLevelNo].pMissionName);
}
// Renvoie le n° réel d'un niveau (pour checksum).
s32 Level_RealNumber(u32 nLevelNo)
{
	return (gMissionTb[nLevelNo].nLevelNo);
}

struct SGameVar	gGameVar;
struct SPlayer	gShoot;

//=============================================================================
typedef void (*pFctCtrl) (void);
typedef void (*pFctDisp) (void);

void PlayerControl_Naked(void);
void PlayerDisplay_Naked(void);
void PlayerControl_Swim(void);
void PlayerDisplay_Swim(void);
void PlayerControl_Parachute(void);
void PlayerDisplay_Parachute(void);
void PlayerControl_SlugNoExit_Temp(void);
void PlayerDisplay_SlugNoExit_Temp(void);
void SlugControl_Submarine(void);
void SlugDisplay_Submarine(void);
void SlugControl_Tank(void);
void SlugDisplay_Tank(void);
void SlugControl_Helicopter(void);
void SlugDisplay_Helicopter(void);
void SlugControl_Rocket(void);
void SlugDisplay_Rocket(void);

// Fonctions de contrôle et d'affichage en fonction du véhicule.
pFctCtrl gpFctCtrlTb[e_HeroVehicle_MAX] =
	{ PlayerControl_Naked, PlayerControl_Swim, PlayerControl_Parachute, PlayerControl_SlugNoExit_Temp, SlugControl_Submarine, SlugControl_Tank, SlugControl_Helicopter, SlugControl_Helicopter, SlugControl_Rocket };
pFctDisp gpFctDispTb[e_HeroVehicle_MAX] =
	{ PlayerDisplay_Naked, PlayerDisplay_Swim, PlayerDisplay_Parachute, PlayerDisplay_SlugNoExit_Temp, SlugDisplay_Submarine, SlugDisplay_Tank, SlugDisplay_Helicopter, SlugDisplay_Helicopter, SlugDisplay_Rocket };

void Player_Control(void)
{
	gpFctCtrlTb[gShoot.nVehicleType]();	// La routine de contrôle du nouveau véhicule.
}

void Game_Player_DeathSet(void);
void Player_DeathManage(void);
u32 Player_VictoryManage(void);
void Player_ShotHitCheck(void);
u32 Gen_GroundCheck(u32 nPosX);
u32 Gen_GroundManage(s32 nSpdXMax, s32 nSpdYMax, s32 nRefOffset, u32 nPfTest);
void Gen_SideCheck(s32 nTestOffsX, s32 nSpdMaxX);
void Gen_KbRestore(void);
void Gen_KbNoControl(void);
void Gen_ButtonB_Check(s32 nJumpSpdY);
void Slug_AutoPilot_NewDir(s32 nBlkX, s32 nBlkY);

//=============================================================================
struct SPlayerSav		// Structure pour sauvegarde de certaines données lors du passage d'un level à l'autre.
{
/*
	s32	nAmmo_Save;			// Sauvegarde pour changement naked > vehicule > naked.
	u32	nBombAmmo_Save;

	// Special Slug.
	u32	nVehicleType;
	u8	nVehicleEnergy;
	u32	nVehiclePhase;

	u32	nVehicleCurAdd;
	u32	nVehicleReqAdd;
	s32	nVehicleOffsY;
	u8	nVehicleFrmCnt0;

	s32	nVehicleAmmo;			// Munitions pour l'arme en cours.
	u32	nVehicleBombAmmo;		// Nombre de bombes.

*/
	u32	nWeapon;		// Arme en cours.
	s32	nAmmo;			// Munitions pour l'arme de poing en cours.
	u32	nBombAmmo;		// Nombre de grenades.

	u32	nFreedPrisoners;	// Nb de prisonniers libérés.

	u32	nVehicleType;	// Véhicule pour Mst44.
};
struct SPlayerSav	gShootSav;

// Sauvegarde les variables du joueur (inter-level).
// !!! Attention, les datas peuvent aussi être initialisés dans ExgPlatformerInit() !!!
void Player_LvlDataSave(void)
{
	gShootSav.nWeapon = gShoot.nWeapon;
	gShootSav.nAmmo = gShoot.nAmmo;
	gShootSav.nBombAmmo = gShoot.nBombAmmo;

	gShootSav.nFreedPrisoners = gShoot.nFreedPrisoners;

	gShootSav.nVehicleType = gShoot.nVehicleType;	// Svg véhicule pour Mst44.

}

// Restore les variables du joueur (inter-level).
// !!! Attention, les datas peuvent aussi être initialisés dans ExgPlatformerInit() !!!
void Player_LvlDataRestore(void)
{
	Player_WeaponSet(gShootSav.nWeapon);
	gShoot.nWeapon = gShootSav.nWeapon;			// Au cas ou passage dans slug. (A voir...)
	gShoot.nAmmo = gShootSav.nAmmo;
	gShoot.nBombAmmo = gShootSav.nBombAmmo;
//	gShoot.nAmmo_Save = gShoot.nAmmo;			// Pour premier passage dans VehicleSet (appelée plus loin dans la fonction !).
//	gShoot.nBombAmmo_Save = gShoot.nBombAmmo;

	gShoot.nFreedPrisoners = gShootSav.nFreedPrisoners;

}

// Pour Mst 44.
u32 LvlDataSave_GetVehicle(void) { return (gShootSav.nVehicleType); }

//=============================================================================

// On remet de l'énergie au slug.
void Player_SlugRefuel(u32 nEnergy)
{
	u32	nEnergyTemp;

	if (gShoot.nVehicleType < e_HeroVehicle_SlugBase) return;	// Seulement si en slug.

	nEnergyTemp = gShoot.nVehicleEnergy;
	nEnergyTemp += nEnergy;
	if (nEnergyTemp > SLUG_ENERGY_MAX) nEnergyTemp = SLUG_ENERGY_MAX;
	gShoot.nVehicleEnergy = nEnergyTemp;
}

//s32	gpnArmsLoadMax[e_Player_Weapon_Max] = { -1, 100, 350, 350, 50 };
//s32	gpnArmsLoadMax[e_Player_Weapon_Max] = { -1, 10, 35, 35, 5 };
s32	gpnArmsLoadMax[e_Player_Weapon_Max] = { -1, 25, 100, 100, 15 };
u8 gpHeroShotClearKb[e_Player_Weapon_Max] = { 1, 1, 0, 0, 1 };	// Doit-on clearer la touche de tir, en fct de l'arme.

// On recharge l'arme en cours.
// (externalisé pour caisse de munitions).
void Player_WeaponReload(u32 nBombsAdd)
{
//	static	s32	gpnArmsLoadMax[e_Player_Weapon_Max] = { -1, 100, 350, 350, 50 };
	// Arme de poing.
	gShoot.nAmmo = gpnArmsLoadMax[gShoot.nWeapon];

	// En slug ?
	if (gShoot.nVehicleType < e_HeroVehicle_SlugBase)
	{	// Non.
		// Grenades.
		gShoot.nBombAmmo += nBombsAdd;
		if (gShoot.nBombAmmo > PLYR_NAKED_BOMBS_MAX) gShoot.nBombAmmo = PLYR_NAKED_BOMBS_MAX;
	}
	else
	{	// Oui.
		// Bombes.
		gShoot.nVehicleBombAmmo += nBombsAdd;
		if (gShoot.nVehicleBombAmmo > SLUG_BOMBS_MAX) gShoot.nVehicleBombAmmo = SLUG_BOMBS_MAX;
	}
}

// Changement d'arme. Si dans un véhicule on change l'arme de poing.
void Player_WeaponSet(u32 nWeaponNo)
{
	assert(nWeaponNo < e_Player_Weapon_Max);

	// Coupe une éventuelle anim de tir.
	if (gShoot.nPlayerAnmTop != -1)
	{
		AnmReleaseSlot(gShoot.nPlayerAnmTop);
		gShoot.nPlayerAnmTop = -1;
	}
	// Nouvelle arme.
	gShoot.nWeapon = nWeaponNo;

	// On recharge.
	Player_WeaponReload(0);

	// Si on n'est pas dans un véhicule, on regarde s'il faut clearer ou pas le bouton de tir.
	if (gShoot.nVehicleType < e_HeroVehicle_SlugBase)
//	if (gVar.pKeys[KB_BUTTONA]) gVar.pKeys[KB_BUTTONA] = gpHeroShotClearKb[gShoot.nWeapon] ^ 1;
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]]) gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] = gpHeroShotClearKb[gShoot.nWeapon] ^ 1;

}


// Sortie/rentrée des canons.
// In: 1 = out / 0 = in.
void Rocket_CannonsInOut(u32 nInOut)
{
	if (nInOut)
	{
		// Sortie.
		if (gShoot.pnRocketSideWeaponsAnmLR[0] == -1) AnmSet(gAnm_SlugRocket_NoSprite, gShoot.pnRocketSideWeaponsSparkAnmLR[0]);	// RAZ étincelle.
		gShoot.pnRocketSideWeaponsAnmLR[0] = AnmSetIfNew(gAnm_SlugRocket_WeaponL_Out, gShoot.pnRocketSideWeaponsAnmLR[0]);
		if (gShoot.pnRocketSideWeaponsAnmLR[1] == -1) AnmSet(gAnm_SlugRocket_NoSprite, gShoot.pnRocketSideWeaponsSparkAnmLR[1]);	// RAZ étincelle.
		gShoot.pnRocketSideWeaponsAnmLR[1] = AnmSetIfNew(gAnm_SlugRocket_WeaponR_Out, gShoot.pnRocketSideWeaponsAnmLR[1]);
	}
	else
	{
		// Rentrée.
		if (gShoot.pnRocketSideWeaponsAnmLR[0] != -1) AnmSetIfNew(gAnm_SlugRocket_WeaponL_In, gShoot.pnRocketSideWeaponsAnmLR[0]);
		if (gShoot.pnRocketSideWeaponsAnmLR[1] != -1) AnmSetIfNew(gAnm_SlugRocket_WeaponR_In, gShoot.pnRocketSideWeaponsAnmLR[1]);
	}

}

// Weapon set pour le slug fusée.
void Rocket_WeaponSet(u32 nWeaponNo)
{
	gShoot.nRocketWeapon = nWeaponNo;
	gShoot.nVehicleAmmo = gpnArmsLoadMax[nWeaponNo];
	Rocket_CannonsInOut((nWeaponNo == e_Player_Weapon_Gun ? 0 : 1));	// Rentre/sort les canons.

}


// Changement de véhicule.
void Player_VehicleSet(u32 nVehicle, u32 nPosX, u32 nPosY, u32 nEnergy, u32 nVulcanAngle, u32 nBombs)
{

	// Position : On passe à celle du véhicule.
	gShoot.nPlayerPosX = nPosX;
	gShoot.nPlayerPosY = nPosY;
	// RAZ vitesse.
	gShoot.nPlayerSpdX = 0;
	gShoot.nPlayerSpdY = 0;
	// RAZ sprite de col.
	gShoot.nPlayerSprCol = SPR_NoSprite;
	gShoot.nPlayerKnifeSprCol = SPR_NoSprite;
	//
	gShoot.nPfAddress = 0;			// RAZ plateforme.
	gShoot.nPlayerGndAng = 0;		// RAZ angle au sol.
	gShoot.nInvulnerability = 0;	// RAZ invulnérabilité.
	gShoot.nDeathFlag = 0;			// RAZ flag mort.
	gShoot.nVehicleHitCnt = 0;		// RAZ Compteur de hit.
	gShoot.nBoredCnt = 0;			// RAZ compteur d'ennui.

	// Clear anim top s'il y en a une.
	if (gShoot.nPlayerAnmTop != -1)
	{
		AnmReleaseSlot(gShoot.nPlayerAnmTop);
		gShoot.nPlayerAnmTop = -1;
	}
	// Clear anim parachute s'il y en a une.
	if (gShoot.nParachuteAnm != -1)
	{
		AnmReleaseSlot(gShoot.nParachuteAnm);
		gShoot.nParachuteAnm = -1;
	}

	// Fusée : Clear des anims si nécessaire.
	u32	i;
	if (gShoot.nRocketMainThrusterAnm != -1) { AnmReleaseSlot(gShoot.nRocketMainThrusterAnm); gShoot.nRocketMainThrusterAnm = -1; }
	if (gShoot.nRocketBodyWeaponSparkAnm != -1) { AnmReleaseSlot(gShoot.nRocketBodyWeaponSparkAnm); gShoot.nRocketBodyWeaponSparkAnm = -1; }
	for (i = 0; i < 2; i++)
	{
		if (gShoot.pnRocketSideWeaponsAnmLR[i] != -1) { AnmReleaseSlot(gShoot.pnRocketSideWeaponsAnmLR[i]); gShoot.pnRocketSideWeaponsAnmLR[i] = -1; }
		if (gShoot.pnRocketSideWeaponsSparkAnmLR[i] != -1) { AnmReleaseSlot(gShoot.pnRocketSideWeaponsSparkAnmLR[i]); gShoot.pnRocketSideWeaponsSparkAnmLR[i] = -1; }
	}
	for (i = 0; i < 4; i++)
	if (gShoot.pnRocketThrustersAnm[i] != -1) { AnmReleaseSlot(gShoot.pnRocketThrustersAnm[i]); gShoot.pnRocketThrustersAnm[i] = -1; }

	// Slug.
	gShoot.nVehicleAutoPilot = 0;
	if (gGameVar.nGenLevel == MISSIONOFFS_HOWTOPLAY) gShoot.nVehicleAutoPilot = 0x80;	// Un bon patch dégueulasse pour le how to play...
	if (nVehicle >= e_HeroVehicle_SlugBase)
	{
		gShoot.nVehicleAmmo = -1;			// Vulcan cannon, munitions infinies.
		gShoot.nVehicleBombAmmo = nBombs;
		// Auto-pilot ?
		Slug_AutoPilot_NewDir(nPosX >> 12, (nPosY >> 12) - 1);
	}
	gShoot.nEngineCnt = -2;		// Pour bruit de moteur.

	// Contrôle sur l'énergie max.
	if (nEnergy > SLUG_ENERGY_MAX) nEnergy = SLUG_ENERGY_MAX;

	gShoot.nVehicleType = nVehicle;
	gShoot.nVehiclePhase = e_SlugPhase_GetIn;
	AnmSet(gAnm_VulcanCannon_Idle, gShoot.nVulcanAnm);	// RAZ anim du Vulcan.
	switch (nVehicle)
	{
	case e_HeroVehicle_None_Naked:
	case e_HeroVehicle_None_SlugNoExit_Temp:
		break;

	case e_HeroVehicle_None_Swim:
		gShoot.nVehicleCurAdd = gShoot.nVehicleReqAdd = PLYR_SWIM_DEFAULTSPRADD;
		gShoot.nVehicleFrmCnt0 = 0;
		break;

	case e_HeroVehicle_None_Parachute:
		gShoot.nParachuteAnm = AnmSetIfNew(gAnm_Parachute_Glide, gShoot.nParachuteAnm);		// Parachute.
		gShoot.nVehicleFrmCnt0 = 0;
		break;

	case e_HeroVehicle_Tank:
		AnmSet(gAnm_SlugTank_GetIn, gShoot.nVehicleAnim);			// Anim de fermeture pour le vehicule.
		AnmSet(gAnm_Hero_SlugGetIn, gShoot.nPlayerAnimInOut);		// Anim du héros en train de rentrer dans le slug.
		//
		gShoot.nPlayerDir = 0;
		gShoot.nVehicleOffsY = 0;
		//gShoot.nVehicleFrmCnt0 = 0;	// inutile, mais pas grave.
		//gShoot.nVehicleCurAdd = gShoot.nVehicleReqAdd = 0;	// inutile, mais pas grave.
		gShoot.nVulcanAngle = nVulcanAngle;
		gShoot.nVehicleEnergy = nEnergy;

		gShoot.nPlayerLastGnd = gShoot.nPlayerGnd = 1;		// Pour éviter le "plouf" quand on monte dans le tank.
		break;

	case e_HeroVehicle_Submarine:
		AnmSet(gAnm_SlugSubmarine_GetIn, gShoot.nVehicleAnim);		// Anim de fermeture pour le vehicule.
		AnmSet(gAnm_Hero_SlugGetIn, gShoot.nPlayerAnimInOut);		// Anim du héros en train de rentrer dans le slug.
		//
		gShoot.nPlayerDir = 0;
		gShoot.nVehicleOffsY = 0;
		gShoot.nVehicleFrmCnt0 = 0;
		gShoot.nVehicleCurAdd = gShoot.nVehicleReqAdd = SLUG_SUBMARINE_DEFAULTSPRADD;
		gShoot.nVulcanAngle = nVulcanAngle;
		gShoot.nVehicleEnergy = nEnergy;
		break;

	case e_HeroVehicle_Helicopter:
	case e_HeroVehicle_HelicopterNoExit:
		AnmSet(gAnm_SlugCopter_GetIn, gShoot.nVehicleAnim);			// Anim de fermeture pour le vehicule.
		AnmSet(gAnm_Hero_SlugGetIn, gShoot.nPlayerAnimInOut);		// Anim du héros en train de rentrer dans le slug.
		//
		gShoot.nPlayerDir = 0;
		gShoot.nVehicleOffsY = 0;
		gShoot.nVehicleFrmCnt0 = 0;
		gShoot.nVehicleCurAdd = gShoot.nVehicleReqAdd = SLUG_HELICOPTER_DEFAULTSPRADD;
		gShoot.nVulcanAngle = nVulcanAngle;
		gShoot.nVehicleEnergy = nEnergy;
		break;

	case e_HeroVehicle_Rocket:
		gShoot.nRocketWeapon = e_Player_Weapon_Gun;

		AnmSet(gAnm_SlugRocket_GetIn, gShoot.nVehicleAnim);			// Anim de fermeture pour le vehicule.
		AnmSet(gAnm_Hero_SlugGetIn, gShoot.nPlayerAnimInOut);		// Anim du héros en train de rentrer dans le slug.
		AnmSet(gAnm_HeroInSlug_Rocket_LookR, gShoot.nPlayerAnm);	// Anim du joueur dans le slug.

		gShoot.nRocketMainThrusterAnm = AnmSet(gAnm_SlugRocket_MainThruster, gShoot.nRocketMainThrusterAnm);	// Anim du réacteur.

		for (i = 0; i < 4; i++)
			gShoot.pnRocketThrustersAnm[i] = AnmSet(gAnm_SlugRocket_ThrustEmpty, gShoot.pnRocketThrustersAnm[i]);	// Anims des thrusters.

		// On réserve les slots pour les étincelles des tirs.
		gShoot.nRocketBodyWeaponSparkAnm = AnmSet(gAnm_SlugRocket_NoSprite, gShoot.nRocketBodyWeaponSparkAnm);
		gShoot.pnRocketSideWeaponsSparkAnmLR[0] = AnmSet(gAnm_SlugRocket_NoSprite, gShoot.pnRocketSideWeaponsSparkAnmLR[0]);
		gShoot.pnRocketSideWeaponsSparkAnmLR[1] = AnmSet(gAnm_SlugRocket_NoSprite, gShoot.pnRocketSideWeaponsSparkAnmLR[1]);

		//
		gShoot.nPlayerDir = 0;
		gShoot.nVehicleOffsY = 0;
		gShoot.nVehicleFrmCnt0 = 0;
		gShoot.nVehicleCurAdd = gShoot.nVehicleReqAdd = 0;
		gShoot.nVulcanAngle = nVulcanAngle;
		gShoot.nVehicleEnergy = nEnergy;
		break;

	}

}

// Inits au niveau du crédit.
void Player_InitCredit(void)
{
	gShoot.nPlayerLives = PLAYER_Lives_Start - 1;	// -1, car on utilise la première vie !
	gShoot.nHUDPlayerLivesBlink = 0;		// RAZ clignotement.
	gShoot.nPlayerScore = 0;
#ifdef POW_RESET_EACH_CREDIT
	gShoot.nFreedPrisoners = 0;	// Nb de prisonniers libérés.
#endif

}

// Init pour une partie, récupère/initialise les paramètres de gExg.
void ExgPlatformerInit(s32 nCreditsNb, u32 nMissionTbOffset)
{
	gGameVar.nGenLevel = nMissionTbOffset;
	gGameVar.nLevel = gMissionTb[gGameVar.nGenLevel].nLevelNo;	// Init nLevel pour premier passage dans ShootGame().

	gGameVar.nCreditsNb = nCreditsNb;		// Nb de crédits pour la partie.
	if (gGameVar.nCreditsNb > 0) gGameVar.nCreditsNb--;		// On utilise le premier.
	Player_InitCredit();
	gGameVar.nBestScore = 0;		// Meilleur score de tous les crédits.
	gGameVar.nContinueUsed = 0;		// Nb de continue utilisés.
	gGameVar.nMissionNo = 0;
	//
	gGameVar.nExitCode = 0;
	gGameVar.nPhase = e_Game_LoadLevel;	// !!!

	srand(time(NULL));		// Init hasard.


	// Level sélecteur activé ? (et jeu ? i.e. pas crédits ou how to play).
	if ((gCCodes.nCheat & e_Cheat_LevelSelect) && nMissionTbOffset >= MISSIONOFFS_LEVELS)
	{
		// Sur un début de mission ?
		if (gMissionTb[gGameVar.nGenLevel].nMissionNo == -1)
		{
			// Non, on remonte dans la liste pour trouver le n° de mission qui va bien.
			u32	i = gGameVar.nGenLevel;
			while (gMissionTb[--i].nMissionNo == -1);
			gGameVar.nMissionNo = gMissionTb[i].nMissionNo;

			// Init des données qui seront restaurées dans Player_LvlDataRestore(),
			// lors du e_Game_LoadLevel. (Normalement, datas sauvegardées en sortant du niveau précédent).
			gShootSav.nWeapon = e_Player_Weapon_Gun;
			gShootSav.nAmmo = gpnArmsLoadMax[gShootSav.nWeapon];
			gShootSav.nBombAmmo = PLYR_NAKED_BOMBS_DEFAULT;
			gShootSav.nFreedPrisoners = 0;
			gShootSav.nVehicleType = e_HeroVehicle_None_Naked;	// Pour Mst44.
		}
	}

}

void Inactivity_WriterInit(void);
extern s32	gnScrollLimitXMin;
// Réinitialisation du bit de flip quand nécessaire, après le Player_InitLife.
void Player_ReinitFlipX(u32 nPlayerLastDir)
{
	// Si scroll libre OU scroll bloqué, on restaure le bit de flip.
	if (gMissionTb[gGameVar.nGenLevel].nScrollType == e_ScrollType_Free || gnScrollLimitXMin != -1)
		gShoot.nPlayerDir = nPlayerLastDir;
}

// Init joueur à chaque vie.
void Player_InitLife(s32 nPosX, s32 nPosY, u32 nVehicleType)
{
	gShoot.nPlayerPosX = nPosX;
	gShoot.nPlayerPosY = nPosY;
	gShoot.nPlayerLastGndPosY = gShoot.nPlayerPosY;
	gShoot.nPlayerSpdX = 0;
	gShoot.nPlayerSpdY = 0;

	Player_WeaponSet(e_Player_Weapon_Gun);
	gShoot.nBombAmmo = PLYR_NAKED_BOMBS_DEFAULT;

	gShoot.nVehicleType = nVehicleType;

	gShoot.nVehicleAnim = AnmSet(gAnm_Hero_Stance, gShoot.nVehicleAnim);	// On réserve un slot.
	gShoot.nPlayerAnimInOut = AnmSet(gAnm_Hero_Stance, gShoot.nPlayerAnimInOut);	// On réserve un slot.

#ifdef POW_RESET_EACH_LIFE
	gShoot.nFreedPrisoners = 0;	// Nb de prisonniers libérés.
#endif

	gShoot.nPlayerAnm = AnmSet(gAnm_Hero_Stance, gShoot.nPlayerAnm);	// Anim du bas.
	gShoot.nPlayerAnmTop = -1;	// Anim du haut pour cas spéciaux (tirs...). => Normalement, pas de "fuite" ici, quand mort, le slot est cleané.
	gShoot.nPlayerDir = 0;		// 0 = droite / 1 = gauche.
	gShoot.nParachuteAnm = -1;
	gShoot.nInvulnerability = 0;
	gShoot.nDeathFlag = 0;		// RAZ flag mort.
	gShoot.nBoredCnt = 0;		// RAZ compteur d'ennui.
	gShoot.nInactivityCnt = 0;	// Compteur d'inactivité.
	Inactivity_WriterInit();	// (On pourrait aussi l'initialiser une fois pour toutes, mais bon...).

	gShoot.nPfAddress = 0;		// Pour plateformes mobiles.

	gShoot.nMstProximity = 0;

	Player_VehicleSet(gShoot.nVehicleType, nPosX, nPosY, SLUG_ENERGY_MAX, 0, SLUG_BOMBS_DEFAULT);	// Nb de bombes par défaut, au cas où on commence un niveau en slug.

	// Init vulcan cannon.
	gShoot.nVulcanAngle = 0;
	gShoot.nVulcanAnm = AnmSet(gAnm_VulcanCannon_Idle, gShoot.nVulcanAnm);	// On réserve un slot.

}

// Initialisation à l'init du niveau.
void GameInitLevel(void)
{
	u32	nPosX, nPosY;
	u32	i;

	MstInitEngine();
	AnmInitEngine();
	FireInitEngine();
	DustInitEngine();
	HardSpr_Init();

	// Position du joueur (relue à la lecture du niveau).
	nPosX = (gMap.nPlayerStartPosX << 12) + (8 << 8);
	nPosY = (gMap.nPlayerStartPosY << 12) + (16 << 8);

	// Solution de facilité un peu crade, car ça ne sert que dans un niveau. Sinon on pourrait faire un monstre qui initialise la variable, voire avec un niveau côté droit et un niveau côté gauche et en fct de la pos du joueur.
	static s16 gnSplashTb[LEVEL_MAX] = { -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, (20*16)-8, -1, (13*16), -1, -1 };
	gShoot.nSplashLevel = (s32)gnSplashTb[gGameVar.nLevel];

	// Init joueur.
	gShoot.nVehicleAnim = -1;		// RAZ des n° de slots d'anim à l'init du niveau, pour réinit de vie sans perte de slots.
	gShoot.nPlayerAnimInOut = -1;
	gShoot.nPlayerAnm = -1;
	gShoot.nVulcanAnm = -1;

	gShoot.nRocketMainThrusterAnm = -1;
	gShoot.pnRocketSideWeaponsAnmLR[0] = gShoot.pnRocketSideWeaponsAnmLR[1] = -1;
	gShoot.nRocketBodyWeaponSparkAnm = -1;
	gShoot.pnRocketSideWeaponsSparkAnmLR[0] = gShoot.pnRocketSideWeaponsSparkAnmLR[1] = -1;
	for (i = 0; i < 4; i++) gShoot.pnRocketThrustersAnm[i] = -1;

	Player_InitLife(nPosX, nPosY, gMissionTb[gGameVar.nGenLevel].nVehicleType);
	// Au cas ou on commence dans un véhicule, on force la phase "normale" pour ne pas voir le 'get in'.
	gShoot.nVehiclePhase = e_SlugPhase_Normal;

//<<< Récupérer le "mission start" ici. (????? Ah, peut-être le n° de la mission !)

	ScrollInitScreen(gMissionTb[gGameVar.nGenLevel].nScrollType);

}


//=============================================================================
// Routines générales.

u64 *gpHeroAnm[] =
{
	gAnm_Hero_Stance,
	gAnm_Hero_CrouchStance,
	gAnm_Hero_Run,
	gAnm_Hero_CrouchWalk,

	gAnm_Hero_Jump,
	gAnm_Hero_Fall,

	gAnm_HeroSub_Stance,
	gAnm_HeroSub_Run,
	gAnm_HeroSub_DeathGen,

	gAnm_HeroSlugNoExit_DeathGen,

	gAnm_Hero_DeathGen_Gnd,
	gAnm_Hero_DeathGen_Air,
	gAnm_Hero_DeathGen_Land,

	gAnm_Hero_Victory,

};
enum
{
	e_Anm_Hero_Stance = 0,		// !!! Attention à l'ordre !!! Pour Stance et Run, on fait un anm+1 dans control_naked quand crouch.
	e_Anm_Hero_CrouchStance,
	e_Anm_Hero_Run,
	e_Anm_Hero_CrouchWalk,

	e_Anm_Hero_Jump,
	e_Anm_Hero_Fall,

	e_Anm_HeroSub_Stance,
	e_Anm_HeroSub_Run,
	e_Anm_HeroSub_DeathGen,

	e_Anm_HeroSlugNoExit_DeathGen,

	e_Anm_Hero_DeathGen_Gnd,
	e_Anm_Hero_DeathGen_Air,
	e_Anm_Hero_DeathGen_Land,

	e_Anm_Hero_Victory,

};


// Avance l'anim du bas et en déduit le sprite qui va bien en haut (en fct de l'arme, etc...).
void Player_GetTopBottom(u32 *pnLegs, u32 *pnTop)
{
	u32	nAnmKey;
	//*** 1) On regarde l'anim du bas. On en déduit le sprite qui va bien pour le haut (en fct de l'arme, etc...).

	*pnLegs = AnmGetImage(gShoot.nPlayerAnm);		// Anim du bas.
	*pnTop = 0;

	static u32 gpHeroTopImg[][e_Player_Weapon_Max] =
	{
		{ e_Spr_HeroTop_Stance_Gun, e_Spr_HeroTop_Stance_Shotgun, e_Spr_HeroTop_Stance_Machinegun, e_Spr_HeroTop_Stance_Shotgun, e_Spr_HeroTop_Stance_Shotgun },
		{ e_Spr_HeroTop_Run_Gun, e_Spr_HeroTop_Run_Shotgun, e_Spr_HeroTop_Run_Machinegun, e_Spr_HeroTop_Run_Shotgun, e_Spr_HeroTop_Run_Shotgun },
		{ e_Spr_HeroTop_Jump_Gun, e_Spr_HeroTop_Jump_Shotgun, e_Spr_HeroTop_Jump_Machinegun, e_Spr_HeroTop_Jump_Shotgun, e_Spr_HeroTop_Jump_Shotgun },
		{ e_Spr_HeroCrouch_Stance_Gun, e_Spr_HeroCrouch_Stance_Shotgun, e_Spr_HeroCrouch_Stance_Machinegun, e_Spr_HeroCrouch_Stance_Shotgun, e_Spr_HeroCrouch_Stance_Shotgun },
		{ e_Spr_HeroCrouch_Walk_Gun, e_Spr_HeroCrouch_Walk_Shotgun, e_Spr_HeroCrouch_Walk_Machinegun, e_Spr_HeroCrouch_Walk_Shotgun, e_Spr_HeroCrouch_Walk_Shotgun },
		{ e_Spr_HeroTop_StanceUp_Gun, e_Spr_HeroTop_StanceUp_Shotgun, e_Spr_HeroTop_StanceUp_Machinegun, e_Spr_HeroTop_StanceUp_Shotgun, e_Spr_HeroTop_StanceUp_Shotgun },
		{ e_Spr_Hero_Victory_Gun, e_Spr_Hero_Victory_Shotgun, e_Spr_Hero_Victory_Machinegun, e_Spr_Hero_Victory_Shotgun, e_Spr_Hero_Victory_Shotgun },
		{ e_Spr_HeroSubTop_Run_Gun, e_Spr_HeroSubTop_Run_Shotgun, e_Spr_HeroSubTop_Run_Shotgun, e_Spr_HeroSubTop_Run_Shotgun, e_Spr_HeroSubTop_Run_Shotgun },
	};
	enum
	{
		e_gpHeroTopImg_StandStance = 0,
		e_gpHeroTopImg_StandRun,
		e_gpHeroTopImg_Jump,
		e_gpHeroTopImg_CrouchStance,
		e_gpHeroTopImg_CrouchWalk,
		e_gpHeroTopImg_StandStanceUp,
		e_gpHeroTopImg_Victory,
		e_gpHeroTopImg_SubRun,
	};

	nAnmKey = AnmGetKey(gShoot.nPlayerAnm);
	switch (nAnmKey)
	{
	case e_AnmKey_Hero_Stance:
		*pnTop = *pnLegs - e_Spr_HeroLegs_Stance;
//		*pnTop += gpHeroTopImg[(gVar.pKeys[SDLK_UP] ? e_gpHeroTopImg_StandStanceUp : e_gpHeroTopImg_StandStance)][gShoot.nWeapon];	// Test du haut...
		*pnTop += gpHeroTopImg[(gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Up]] ? e_gpHeroTopImg_StandStanceUp : e_gpHeroTopImg_StandStance)][gShoot.nWeapon];	// Test du haut...
		break;
	case e_AnmKey_Hero_Run:
		*pnTop = *pnLegs - e_Spr_HeroLegs_Run;
		*pnTop += gpHeroTopImg[e_gpHeroTopImg_StandRun][gShoot.nWeapon];
		break;
	case e_AnmKey_Hero_Jump:
		*pnTop = *pnLegs - e_Spr_HeroLegs_Jump;
		*pnTop += gpHeroTopImg[e_gpHeroTopImg_Jump][gShoot.nWeapon];
		break;
	case e_AnmKey_Hero_CrouchStance:
		*pnLegs += gpHeroTopImg[e_gpHeroTopImg_CrouchStance][gShoot.nWeapon];
		break;
	case e_AnmKey_Hero_CrouchWalk:
		*pnLegs += gpHeroTopImg[e_gpHeroTopImg_CrouchWalk][gShoot.nWeapon];
		break;

	case e_AnmKey_Hero_Victory:
		*pnLegs += gpHeroTopImg[e_gpHeroTopImg_Victory][gShoot.nWeapon];
		break;

	case e_AnmKey_HeroSub_Stance:
	case e_AnmKey_HeroSub_Run:
		*pnTop = *pnLegs - e_Spr_HeroSubLegs_Run;
		*pnTop += gpHeroTopImg[e_gpHeroTopImg_SubRun][gShoot.nWeapon];
		break;

	case e_AnmKey_Hero_Bored:		// Cas très spécial des anims "bored".
		*pnTop = *pnLegs;
		*pnLegs = e_Spr_HeroLegs_Stance;	// Jambes fixes. L'anim donne l'image du haut.
		break;

	}

}

// Accélération/ralentissement.
// Out : 1 = On a demandé a bouger / 0 = Plus de direction.
u32 PlyrMove_Spd(s32 *pSpd, s32 nSpdMax, s32 nAcc, const u32 nKeyMinus, const u32 nKeyPlus)
{
	if (gVar.pKeys[nKeyMinus] || gVar.pKeys[nKeyPlus])
	{
		// Acceleration.
		if (gVar.pKeys[nKeyMinus])
		{
			if (*pSpd > -nSpdMax) *pSpd -= nAcc;
			if (*pSpd < -nSpdMax) *pSpd = -nSpdMax;
		}
		if (gVar.pKeys[nKeyPlus])
		{
			if (*pSpd < nSpdMax) *pSpd += nAcc;
			if (*pSpd > nSpdMax) *pSpd = nSpdMax;
		}
		return (1);
	}
	else
	{
		// Ralentissement.
		if (*pSpd)
		{
			*pSpd += (*pSpd < 0 ? nAcc : -nAcc);
			if (ABS(*pSpd) < nAcc) *pSpd = 0;
		}
	}
	return (0);
}

// Empèche de sortir de la map par le haut.
void PlayerMove_LimitY(s32 nHeight)
{
	if (gShoot.nPlayerPosY - (nHeight << 8) <= 0)
	{
		gShoot.nPlayerPosY = nHeight << 8;
		if (gShoot.nPlayerSpdY < 0) gShoot.nPlayerSpdY = 0;
	}
}

// Empèche de sortir de la map par le bas (peut arriver dans le niveau sous-marin).
void PlayerMove_LimitY_Bottom(s32 nHeight)
{
	if (gShoot.nPlayerPosY + (nHeight << 8) >= (gMap.pPlanesHt[gMap.nHeroPlane] << 12) - 0x100)
	{
		gShoot.nPlayerPosY = (gMap.pPlanesHt[gMap.nHeroPlane] << 12) - 0x100 - (nHeight << 8);
		if (gShoot.nPlayerSpdY > 0) gShoot.nPlayerSpdY = 0;
	}
}

// Limites gauche et droite. Map ou fenêtre.
void PlayerMove_LimitX(s32 nOffsetX)
{
	s32	nLimitMinX, nLimitMaxX;

	nLimitMinX = gScrollPos.nPosX;
	nLimitMaxX = gScrollPos.nPosX + (SCR_Width << 8) - 0x100;

	if (gShoot.nPlayerPosX + gShoot.nPlayerSpdX - nOffsetX <= nLimitMinX) gShoot.nPlayerSpdX = 0;
	if (gShoot.nPlayerPosX - nOffsetX <= nLimitMinX) gShoot.nPlayerPosX = nLimitMinX + nOffsetX;	// Sécurité, mais ne devrait pas arriver.
	if (gShoot.nPlayerPosX + gShoot.nPlayerSpdX + nOffsetX >= nLimitMaxX) gShoot.nPlayerSpdX = 0;
	if (gShoot.nPlayerPosX + nOffsetX >= nLimitMaxX) gShoot.nPlayerPosX = nLimitMaxX - nOffsetX;	// Sécurité, mais ne devrait pas arriver.

}

//=============================================================================
// Gestion du canon vulcain accroché aux slugs.

// In : Le n° de sprite sur lequel attacher le canon.
void VulcanCannon_Manage(u32 nSlugSpr)
{
	static	u8	nCntS = 0;
	u32	nSpr;
	u8	nKbDir;

	// Angle.
	static	u8	pAngKb[16] = { 0, 64, 0, 32, 192, 0, 224, 0, 128, 96, 0, 64, 160, 64, 192, 0  };	// Direction en fonction des touches de direction enfoncées.
	nKbDir = 0;
//	nKbDir |= (gVar.pKeys[SDLK_UP] ? e_KbDir_Up : 0) | (gVar.pKeys[SDLK_RIGHT] ? e_KbDir_Right : 0) |
//				(gVar.pKeys[SDLK_DOWN] ? e_KbDir_Down : 0) | (gVar.pKeys[SDLK_LEFT] ? e_KbDir_Left : 0);
	nKbDir |= (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Up]] ? e_KbDir_Up : 0) | (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Right]] ? e_KbDir_Right : 0) |
				(gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]] ? e_KbDir_Down : 0) | (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Left]] ? e_KbDir_Left : 0);
	if ( (nKbDir & (e_KbDir_Up | e_KbDir_Down)) == (e_KbDir_Up | e_KbDir_Down)) nKbDir &= ~(e_KbDir_Up | e_KbDir_Down);
	if ( (nKbDir & (e_KbDir_Left | e_KbDir_Right)) == (e_KbDir_Left | e_KbDir_Right)) nKbDir &= ~(e_KbDir_Left | e_KbDir_Right);
	nKbDir &= 0x0F;
	if (gShoot.nVulcanAngle == 64 && nKbDir == e_KbDir_Down) nKbDir |= e_KbDir_Right;	// Pour forcer la rotation Haut > Bas en passant par l'avant.
	if (nKbDir)
	{
		gShoot.nVulcanReqAngle = pAngKb[nKbDir];
		// Rotation.
		if (gShoot.nVulcanAngle != gShoot.nVulcanReqAngle)
		{
			if ((gShoot.nVulcanAngle - gShoot.nVulcanReqAngle) & (1 << 7))
				gShoot.nVulcanAngle += 2;
			else
				gShoot.nVulcanAngle -= 2;
		}
	}

	// On récupère le point d'attache du canon sur le sprite du slug.
	s32	nOffsX = 0, nOffsY = 0;
	struct SSprRect sSlugRect;
	if (SprGetRect(nSlugSpr, e_SprRectZone_ShotOrg, &sSlugRect))
	{
		if (sSlugRect.nType == e_SprRect_Rect)
		{
			nOffsX = sSlugRect.nX1;		// Rect, le canon est au point inférieur gauche. (Supérieur droit = origine du tir du slug (-16)).
			nOffsY = sSlugRect.nY2;
		}
		else
		{
			nOffsX = sSlugRect.nX1;		// Point.
			nOffsY = sSlugRect.nY1;
		}
	}

	// Slugs en rotation, décalage du point de ref du cannon.
	if (gShoot.nVehicleType == e_HeroVehicle_Tank)
	{
		Rot2D_RotatePoint(&nOffsX, &nOffsY, gShoot.nVehicleCurAdd);
	}

	// Tir ?
//	if (gVar.pKeys[KB_BUTTONA] && nCntS == 0)
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] && nCntS == 0)
	{
		// Offset du tir.
		struct SSprRect sCannonRect;
		s32	nCannonOffsX, nCannonOffsY;
		if (SprGetRect(e_Spr_VulcanCannon_Still + (gShoot.nVulcanAngle >> 3), e_SprRectZone_ShotOrg, &sCannonRect))
		{
			nCannonOffsX = sCannonRect.nX1;
			nCannonOffsY = sCannonRect.nY1;
		}
		else
		{
			nCannonOffsX = nCannonOffsY = 0;
		}
		// Tir. (Note : On cleare les 3 derniers bits pour avoir un affichage qui est nickel du tir par rapport au canon).
		FireAdd(e_Shot_Player_VulcanCannon, gShoot.nPlayerPosX + ((nOffsX + nCannonOffsX) << 8), gShoot.nPlayerPosY + ((nOffsY + nCannonOffsY) << 8), gShoot.nVulcanAngle & ~7);
		// Anim de tir.
		AnmSet(gAnm_VulcanCannon_Shot, gShoot.nVulcanAnm);
		nCntS = 6;//8;
	}
//	if (gVar.pKeys[KB_BUTTONA] == 0) nCntS = 0;
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] == 0) nCntS = 0;
	if (nCntS) nCntS--;

	// Affichage.
	u32	nFlagHit;
	nFlagHit = (gShoot.nVehicleHitCnt ? SPR_Flag_HitPal : 0);		// Palette hit.
	nSpr = AnmGetImage(gShoot.nVulcanAnm);
	SprDisplay((nSpr + (gShoot.nVulcanAngle >> 3)) | nFlagHit, (gShoot.nPlayerPosX >> 8) + nOffsX, (gShoot.nPlayerPosY >> 8) + nOffsY, e_Prio_Joueur + 1);

}

//=============================================================================
// Gestion des bruits de moteur des slugs.

#define	SLUGENGINENOISE_MAX	2
struct sSlugEngineNoise
{
	s8	pnFx[SLUGENGINENOISE_MAX];		// 1 son et éventuellement 1 son alternatif.
};
struct sSlugEngineNoise	gpSlugEngineNoises[e_HeroVehicle_MAX - e_HeroVehicle_SlugBase] =
{
	{ { e_Sfx_Submarine0, e_Sfx_Submarine1 } },	// e_HeroVehicle_Submarine
	{ { e_Sfx_Tank0, e_Sfx_Tank1 } },	// e_HeroVehicle_Tank
	{ { e_Sfx_Heli0, -1 } },	// e_HeroVehicle_Helicopter
	{ { e_Sfx_Heli0, -1 } },	// e_HeroVehicle_HelicopterNoExit
	{ { e_Sfx_Rocket0, e_Sfx_Rocket1 } },	// e_HeroVehicle_Rocket
};

// Bruit.
void SlugEngine_Play(u32 nSfxNo, u32 nEngineCntMax)
{
	if (++gShoot.nEngineCnt >= nEngineCntMax)
	{
		gShoot.nEngineCnt = 0;
		Sfx_PlaySfx(nSfxNo, e_SfxPrio_30);	// Sfx.
	}
}

// Stoppe les bruits de moteur.
void SlugEngine_Stop(u32 nVehicle)
{
	u32	i;

	nVehicle -= e_HeroVehicle_SlugBase;
	if (nVehicle >= e_HeroVehicle_MAX - e_HeroVehicle_SlugBase) return;

	for (i = 0; i < SLUGENGINENOISE_MAX; i++)
		if (gpSlugEngineNoises[nVehicle].pnFx[i] != -1)
			Sfx_StopSfx(gpSlugEngineNoises[nVehicle].pnFx[i]);
}

//=============================================================================
// Gestion des slugs.

// Génération d'un monstre slug quand le joueur quitte le véhicule.
void Slug_GenerateMst(u32 nVehicleType, s32 nPosX, s32 nPosY, u32 nSlugBombsSave)
{
	// On génère le monstre slug.

	//[mst9] Slug
	//Type = 0:3: Submarine - Tank
	//State = 4:7 : Wait - Dies(reserved)
	//Energy(192) = 8:15:
	//Vulcan = 16:23:			; Angle du vulcan cannon.
	//Bombs(50) = 24:31:
	//Slug Angle(0) = 32:39:		; Angle du slug (Tank).

	u8	pData[5];
	pData[0] = nVehicleType - e_HeroVehicle_SlugBase;
	pData[0] |= (gShoot.nVehicleEnergy > SLUG_ENERGY_ALERT ? 0 : 1) << 4;
	pData[1] = gShoot.nVehicleEnergy;
	pData[2] = gShoot.nVulcanAngle;
	pData[3] = nSlugBombsSave;
	pData[4] = (nVehicleType == e_HeroVehicle_Tank ? gShoot.nVehicleCurAdd : 0);
	MstAdd(e_Mst9_Slug, (nPosX >> 8), (nPosY >> 8), pData, -1);
}

// Panneau "Out".
void Slug_OutSign(void)
{
	u32	nSpr;
	s32	nOffsX, nOffsY;

	if (gShoot.nVehicleEnergy > SLUG_ENERGY_ALERT) return;

	AnmSetIfNew(gAnm_Slug_OutSign, gShoot.nPlayerAnimInOut);
	nSpr = AnmGetImage(gShoot.nPlayerAnimInOut);
	MstSlug_EntrancePtGet(e_HeroVehicle_Submarine - e_HeroVehicle_SlugBase, &nOffsX, &nOffsY);
	// Slugs en rotation, décalage du point de ref de la flèche.
	if (gShoot.nVehicleType == e_HeroVehicle_Tank)
	{
		Rot2D_RotatePoint(&nOffsX, &nOffsY, gShoot.nVehicleCurAdd);
	}
	SprDisplay(nSpr, (gShoot.nPlayerPosX >> 8) + nOffsX, (gShoot.nPlayerPosY >> 8) + nOffsY - 8, e_Prio_Joueur + 2);

}

// Teste le clavier pour sortie de slug.
u32 Slug_CheckKbGetOut(u64 *pDoorOpeningAnm)
{
//	if (gVar.pKeys[SDLK_DOWN] && gVar.pKeys[KB_BUTTONB])
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]] && gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonB]])
	{
		gShoot.nVehiclePhase = e_SlugPhase_GetOut;
		AnmSet(pDoorOpeningAnm, gShoot.nVehicleAnim);
		// Coupe le controle.
		Gen_KbNoControl();
		// Coupe l'anim de tir s'il y en a une.
		if (gShoot.nPlayerAnmTop != -1)
		{
			AnmReleaseSlot(gShoot.nPlayerAnmTop);
			gShoot.nPlayerAnmTop = -1;
		}
		return (1);
	}
	return (0);
}

// Sous-routine de sortie de slug.
void Slug_GetOut(u32 nSlugType, u32 nNakedType, u64 *pExitAnm, s32 nPlyrSpdY)
{
	s32	nOffsX, nOffsY;
	s32	nPosX, nPosY;
	u32 nSlugBombsSave = gShoot.nVehicleBombAmmo;

//	if (nNakedType == e_HeroVehicle_None_SlugNoExit_Temp) gShoot.nVehicleNoExitTypeToReset = nSlugType;	// NoExit => On note le véhicule qu'on avait.
	if (nNakedType == e_HeroVehicle_None_SlugNoExit_Temp)
	{
		gShoot.nVehicleNoExitTypeToReset = nSlugType;	// NoExit => On note le véhicule qu'on avait.
		gShoot.nBombAmmo = gShoot.nVehicleBombAmmo;		// Pour pb affichage quand on sort d'un véhicule "no exit", pour ne pas afficher le nombre de bombes du mode "naked".
		gShoot.nAmmo = gShoot.nVehicleAmmo;				// idem.
	}

	nPosX = gShoot.nPlayerPosX;		// Sauvegarde des pos du héros AVANT le VehicleSet.
	nPosY = gShoot.nPlayerPosY + (gShoot.nVehicleOffsY << 8);
	MstSlug_EntrancePtGet(nSlugType - e_HeroVehicle_SlugBase, &nOffsX, &nOffsY);
	// Slugs en rotation, décalage du point de ref.
	if (gShoot.nVehicleType == e_HeroVehicle_Tank)
	{
		Rot2D_RotatePoint(&nOffsX, &nOffsY, gShoot.nVehicleCurAdd);
	}

	// Passe en naked.
	if (nNakedType == e_HeroVehicle_None_Swim) nOffsY -= 8;		// Décalage pour le slug out sous-marin.
	Player_VehicleSet(nNakedType, nPosX + (nOffsX << 8), nPosY + (nOffsY << 8), SLUG_ENERGY_MAX, 0, 0);
	AnmSet(pExitAnm, gShoot.nPlayerAnm);	// Force l'anim de slug out.
	gShoot.nPlayerSpdY = -nPlyrSpdY;
	gShoot.nPlayerGnd = gShoot.nPlayerLastGnd = 0;
	gpFctCtrlTb[gShoot.nVehicleType]();	// La routine de contrôle du nouveau véhicule.

	// Génère le monstre.
	Slug_GenerateMst(nSlugType, nPosX, nPosY, nSlugBombsSave);

	SlugEngine_Stop(nSlugType);	// Stoppe les bruits de moteur.

}

// Gestion des phases In/Out.
// Renvoie 1 quand il ne faut pas continuer la gestion du slug derrière (le héros est sorti). 0 sinon.
u32 Slug_CtrlInOut(u32 nSlugType, u32 nNakedType, u64 *pExitAnm, s32 nPlyrSpdY, u64 *pVehicleIdleAnim)
{
	if (gShoot.nVehiclePhase != e_SlugPhase_Normal)
	{
		// Pas de contrôle pendant les In/Out.
		Gen_KbNoControl();

		// Avance des anims In/Out.
		AnmGetImage(gShoot.nVehicleAnim);			// La porte qui se ferme.
		AnmGetImage(gShoot.nPlayerAnimInOut);		// Le joueur en train d'entrer.
		if (AnmCheckEnd(gShoot.nVehicleAnim))
		{
			// Anim de sortie finie ?
			if (gShoot.nVehiclePhase == e_SlugPhase_GetOut)
			{
				Slug_GetOut(nSlugType, nNakedType, pExitAnm, nPlyrSpdY);
				return (1);
			}
			// On était en GetIn, on passe en mode normal.
			gShoot.nVehiclePhase = e_SlugPhase_Normal;
			// Anim du vehicule "idle".
			AnmSet(pVehicleIdleAnim, gShoot.nVehicleAnim);

			if (gShoot.nVehicleAutoPilot == 0) Gen_KbRestore();	// Remis uniquement pour gérer le cas ou on monte dans le slug tank et qu'on appuie vers le bas.
		}
	}
	else
	if (gShoot.nVehicleEnergy == 0)
	{
		// Plus d'énergie ?
		Slug_GetOut(nSlugType, nNakedType, pExitAnm, nPlyrSpdY);
		Game_Player_DeathSet();
		return (1);
	}
	return (0);
}

//=================

// Auto Pilot : Nouvelle direction ?
void Slug_AutoPilot_NewDir(s32 nBlkX, s32 nBlkY)
{
	s32	nRes;

	// Update position du test.
	gShoot.nVehicleAutoPilotLastBlkX = nBlkX;
	gShoot.nVehicleAutoPilotLastBlkY = nBlkY;
	// Un bloc de chemin ?
	if ((nRes = Map_PathAirGetBlock(nBlkX, nBlkY)) != -1)
	{
		if (nRes < 16)
		{
			// Oui.
			static	u8 pDir[16] =
			{ e_KbDir_Right,e_KbDir_Right|e_KbDir_Up,e_KbDir_Right|e_KbDir_Up,e_KbDir_Right|e_KbDir_Up,
			  e_KbDir_Up,e_KbDir_Up|e_KbDir_Left,e_KbDir_Up|e_KbDir_Left,e_KbDir_Up|e_KbDir_Left,
			  e_KbDir_Left,e_KbDir_Left|e_KbDir_Down,e_KbDir_Left|e_KbDir_Down,e_KbDir_Left|e_KbDir_Down,
			  e_KbDir_Down,e_KbDir_Down|e_KbDir_Right,e_KbDir_Down|e_KbDir_Right,e_KbDir_Down|e_KbDir_Right };
			gShoot.nVehicleAutoPilot = pDir[nRes];
		}
		else
			gShoot.nVehicleAutoPilot = 0;		// Coupe l'auto pilot.
	}
}

// Gestion du pilote automatique.
void Slug_AutoPilot_Manage(void)
{
	u32	i;
	s32	nNewBlkX, nNewBlkY;

	if (gShoot.nVehicleAutoPilot == 0) return;	// Pas de pilote automatique.
	if (gShoot.nInvulnerability == 0) gShoot.nInvulnerability = PLYR_INVL_BLINK - 1;	// Invulnérable pendant l'auto pilot.

	// Auto pilote de base : Direction automatique.
	if ((gShoot.nVehicleAutoPilot & 0x80) == 0)
	{
		// On a changé de bloc ?
		nNewBlkX = ((gShoot.nPlayerPosX & ~0xFFF) | 0x800) >> 12;
		nNewBlkY = ((gShoot.nPlayerPosY & ~0xFFF) | 0x800) >> 12;
		if (nNewBlkX != gShoot.nVehicleAutoPilotLastBlkX || nNewBlkY != gShoot.nVehicleAutoPilotLastBlkY)
			Slug_AutoPilot_NewDir(nNewBlkX, nNewBlkY);
	}
	// Sinon on tombe dans la version pilotée par un monstre (how to play + credits) : Le monstre place les bits de gShoot.nVehicleAutoPilot.

	// Coupe le contrôle.
	Gen_KbNoControl();
	// Taggue les touches qui vont bien.
//	for (i = 0; i < 7; i++)
	for (i = 0; i < e_CfgKey_LAST; i++)
	{
//		static	u32	pSDLKeys[7] = { SDLK_UP, SDLK_RIGHT, SDLK_DOWN, SDLK_LEFT, KB_BUTTONA, KB_BUTTONB, KB_BUTTONC };
//		if (gShoot.nVehicleAutoPilot & (1 << i)) gVar.pKeys[pSDLKeys[i]] = 1;
		static	u32	pSDLKeys[e_CfgKey_LAST] = { e_CfgKey_Up, e_CfgKey_Right, e_CfgKey_Down, e_CfgKey_Left, e_CfgKey_ButtonA, e_CfgKey_ButtonB, e_CfgKey_ButtonC };
		if (gShoot.nVehicleAutoPilot & (1 << i)) gVar.pKeys[gMSCfg.pKeys[pSDLKeys[i]]] = 1;
	}

}

//=================

u32 Gen_CeilingCheck(u32 nPosX, s32 nHeroHt);
void Gen_CeilingSideCheck(s32 nTestOffsX, s32 nHeroHt, s32 nSpdMaxX);

// Contrôle du sous-marin.
void SlugControl_Submarine(void)
{

#ifdef DEBUG_KEYS
if (gVar.pKeys[SDLK_r])
{
	u32	r = (gShoot.nVehicleEnergy >= 10 ? 10 : gShoot.nVehicleEnergy);
	gShoot.nVehicleEnergy -= r;
	gVar.pKeys[SDLK_r] = 0;
}
#endif

	// Gestion de la victoire.
	Player_VictoryManage();

	// Gestion des phases In/Out.
	if (Slug_CtrlInOut(e_HeroVehicle_Submarine, e_HeroVehicle_None_Swim, gAnm_HeroSub_SlugOutGen, PLYR_SWIM_SPDY_MAX, gAnm_SlugSubmarine_Idle)) return;

	// *** Déplacement du sub.
	// Vitesse.
//	PlyrMove_Spd(&gShoot.nPlayerSpdX, SLUG_SUBMARINE_SPDX_MAX, SLUG_SUBMARINE_ACC, SDLK_LEFT, SDLK_RIGHT);
//	PlyrMove_Spd(&gShoot.nPlayerSpdY, SLUG_SUBMARINE_SPDY_MAX, SLUG_SUBMARINE_ACC, SDLK_UP, SDLK_DOWN);
	PlyrMove_Spd(&gShoot.nPlayerSpdX, SLUG_SUBMARINE_SPDX_MAX, SLUG_SUBMARINE_ACC, gMSCfg.pKeys[e_CfgKey_Left], gMSCfg.pKeys[e_CfgKey_Right]);
	PlyrMove_Spd(&gShoot.nPlayerSpdY, SLUG_SUBMARINE_SPDY_MAX, SLUG_SUBMARINE_ACC, gMSCfg.pKeys[e_CfgKey_Up], gMSCfg.pKeys[e_CfgKey_Down]);
	// Bords de map ou scroll bloqué.
	PlayerMove_LimitX(SLUG_SUBMARINE_REF_OFFSETX);
	PlayerMove_LimitY(SLUG_SUBMARINE_HEIGHT);
	PlayerMove_LimitY_Bottom(SLUG_SUBMARINE_HEIGHT_B);
	// Sol dte/gche.
	Gen_SideCheck(SLUG_SUBMARINE_REF_OFFSETX, SLUG_SUBMARINE_SPDX_MAX);	// Mettra la vitesse à 0 si mouvement pas possible.
	// Plafond dte/gche.
	Gen_CeilingSideCheck(SLUG_SUBMARINE_REF_OFFSETX, (SLUG_SUBMARINE_HEIGHT - 4) << 8, SLUG_SUBMARINE_SPDX_MAX);
	// Add.
	gShoot.nPlayerPosX += gShoot.nPlayerSpdX;
	gShoot.nPlayerPosY += gShoot.nPlayerSpdY;

	// *** Test du sol.
	gShoot.nPlayerLastGnd = gShoot.nPlayerGnd = 0;		// RAZ gnd.
	Gen_GroundCheck(gShoot.nPlayerPosX + SLUG_SUBMARINE_REF_OFFSETX);
	Gen_GroundCheck(gShoot.nPlayerPosX - SLUG_SUBMARINE_REF_OFFSETX);
	// Test du plafond.
	Gen_CeilingCheck(gShoot.nPlayerPosX + SLUG_SUBMARINE_REF_OFFSETX, (SLUG_SUBMARINE_HEIGHT - 4) << 8);
	Gen_CeilingCheck(gShoot.nPlayerPosX - SLUG_SUBMARINE_REF_OFFSETX, (SLUG_SUBMARINE_HEIGHT - 4) << 8);

	// Inclinaison.
	gShoot.nVehicleReqAdd = SLUG_SUBMARINE_DEFAULTSPRADD;
//	if (gVar.pKeys[SDLK_UP]) gShoot.nVehicleReqAdd = 29 - 4;	// Le sinus sur le add spr varie de -4 à +4.
//	if (gVar.pKeys[SDLK_DOWN]) gShoot.nVehicleReqAdd = 0 + 4;
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Up]]) gShoot.nVehicleReqAdd = 29 - 4;	// Le sinus sur le add spr varie de -4 à +4.
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]]) gShoot.nVehicleReqAdd = 0 + 4;
	if (gShoot.nVehicleCurAdd != gShoot.nVehicleReqAdd)
		gShoot.nVehicleCurAdd += (gShoot.nVehicleCurAdd < gShoot.nVehicleReqAdd ? 1 : -1);

	// Sortie du slug ?
	Slug_CheckKbGetOut(gAnm_SlugSubmarine_GetOut);

	// Bruit du moteur.
	if (gShoot.nVehiclePhase != e_SlugPhase_GetOut)
	{
		u32	nEngineCntMax = 18;
		u32	nKeysMvt = !!(gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Left]] | gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Right]] | gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Up]] | gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]]);
		u32	nSfxToTest = gpSlugEngineNoises[gShoot.nVehicleType - e_HeroVehicle_SlugBase].pnFx[!nKeysMvt];
		if (Sfx_IsPlaying(nSfxToTest))
		{
			Sfx_StopSfx(nSfxToTest);
			nEngineCntMax = 0;
		}
		SlugEngine_Play(gpSlugEngineNoises[gShoot.nVehicleType - e_HeroVehicle_SlugBase].pnFx[nKeysMvt], nEngineCntMax);
	}

}

// Affichage du sous-marin.
void SlugDisplay_Submarine(void)
{
	u32	nSpr;
	s32	nAdd;
	s32	nOffsX, nOffsY;
	u32	nFlagHit;

	nFlagHit = (gShoot.nVehicleHitCnt ? SPR_Flag_HitPal : 0);		// Palette hit.

	switch (gShoot.nVehiclePhase)
	{
	case e_SlugPhase_GetIn:
		// Affiche le héros en train d'entrer dans le slug.
		if (AnmCheckEnd(gShoot.nPlayerAnimInOut) == 0)
		{
			nSpr = AnmGetLastImage(gShoot.nPlayerAnimInOut);
			nSpr += e_Spr_HeroSub_SlugIn_Gun;	// Si plusieurs tables, faire une table en static.
			MstSlug_EntrancePtGet(e_HeroVehicle_Submarine - e_HeroVehicle_SlugBase, &nOffsX, &nOffsY);
			SprDisplay(nSpr, (gShoot.nPlayerPosX >> 8) + nOffsX, (gShoot.nPlayerPosY >> 8) + nOffsY, e_Prio_Joueur + 2);
		}
		// Pas de break !
	case e_SlugPhase_GetOut:
		// L'anim d'ouverture/fermeture.
		nSpr = AnmGetLastImage(gShoot.nVehicleAnim);
		SprDisplay(nSpr | nFlagHit, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8, e_Prio_Joueur-1);
		break;

	//case e_SlugPhase_Normal:
	default:
		Slug_OutSign();		// Panneau "Out" si plus assez de vie.

		// Slug, + petit sinus pour faire une petite oscillation.
		nSpr = e_Spr_SlugSubmarine_Body + gShoot.nVehicleCurAdd;
		gShoot.nVehicleFrmCnt0 += 3;
		nAdd = (gVar.pSin[gShoot.nVehicleFrmCnt0] + 31) >> 6;	// +31 : On divise par 64, avec le +31 ça répartit équitablement entre -4 et +4, au lieu d'avoir plein de -1 et un seul +4.
		SprDisplay((nSpr + nAdd) | nFlagHit, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8, e_Prio_Joueur-1);
		// Perso dans le sous-marin.
		SprDisplay(e_Spr_Hero_InSlug_Sub + gShoot.nVehicleCurAdd + nAdd, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8, e_Prio_Joueur);

		// Paillettes : Bulles sur le déplacement avant.
		if ((gnFrame & 15) == 0)
		if (gShoot.nPlayerSpdX > SLUG_SUBMARINE_ACC)
			DustSet(gAnm_SubmarineBubble_Dust,
				gShoot.nPlayerPosX - (24 << 8), gShoot.nPlayerPosY - (8 << 8) + ((gShoot.nVehicleCurAdd - SLUG_SUBMARINE_DEFAULTSPRADD) << 7), e_Prio_DustUnder, 0);

		break;
	}

	// Bombe ?
//	if (gVar.pKeys[KB_BUTTONC] && gShoot.nVehicleBombAmmo)
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonC]] && gShoot.nVehicleBombAmmo)
	{
		struct SSprRect sSlugRect;
		if (SprGetRect(nSpr, e_SprRectZone_ShotOrg, &sSlugRect))
		if (sSlugRect.nType == e_SprRect_Rect)
		{
			// Origine du tir : Point supérieur droit (+16).
//			FireAdd(e_Shot_Player_SlugSubmarine_Rocket_Launch, gShoot.nPlayerPosX + (sSlugRect.nX2 << 8), gShoot.nPlayerPosY  + ((sSlugRect.nY1 + 16) << 8), 0);	// Tout droit.
			FireAdd(e_Shot_Player_SlugSubmarine_Rocket_Launch, gShoot.nPlayerPosX + (sSlugRect.nX2 << 8), gShoot.nPlayerPosY  + ((sSlugRect.nY1 + 16) << 8), gShoot.nVehicleCurAdd - 15);	// Avec angle su slug.
			Sfx_PlaySfx(e_Sfx_Shot_MiniRocket, e_SfxPrio_Hero_Shot);	// !!changer!!
//			gVar.pKeys[KB_BUTTONC] = 0;
			gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonC]] = 0;
			gShoot.nVehicleBombAmmo--;
		}
	}

	// Sprite de col.
	gShoot.nPlayerSprCol = e_Spr_SlugSubmarine_Body + 15;	// Pas de flip pour les slugs.
	Player_ShotHitCheck();	// Teste si on se fait toucher par un tir.

	// Vulcan.
	VulcanCannon_Manage(nSpr);

}

//=================

u32	gpSlugInBaseImg[e_Player_Weapon_Max] =
	{ e_Spr_Hero_SlugIn_Gun, e_Spr_Hero_SlugIn_Shotgun, e_Spr_Hero_SlugIn_Machinegun, e_Spr_Hero_SlugIn_Shotgun, e_Spr_Hero_SlugIn_Shotgun };

// Test pour le Mst37, a-t'on poussé le joueur hors de l'écran ?
u32 Rocket_EnemyCheckScreenOut(void)
{
	u32	nRetVal = 0;
	if (gShoot.nPlayerPosX - SLUG_ROCKET_REF_OFFSETX < gScrollPos.nPosX)
	{
		gShoot.nPlayerPosX = gScrollPos.nPosX + SLUG_ROCKET_REF_OFFSETX;
		nRetVal++;
	}
	else if (gShoot.nPlayerPosX + SLUG_ROCKET_REF_OFFSETX > gScrollPos.nPosX + (SCR_Width << 8) - 0x100)
	{
		gShoot.nPlayerPosX = gScrollPos.nPosX + (SCR_Width << 8) - 0x100 - SLUG_ROCKET_REF_OFFSETX;
		nRetVal++;
	}
	if (gShoot.nPlayerPosY < gScrollPos.nPosY + (SLUG_ROCKET_HEIGHT << 8))
	{
		gShoot.nPlayerPosY = gScrollPos.nPosY + (SLUG_ROCKET_HEIGHT << 8);
		nRetVal++;
	}
	else if (gShoot.nPlayerPosY > gScrollPos.nPosY + (SCR_Height << 8))
	{
		gShoot.nPlayerPosY = gScrollPos.nPosY + (SCR_Height << 8);
		nRetVal++;
	}
	return (nRetVal);
}

// Contrôle de la fusée.
void SlugControl_Rocket(void)
{
	s32	nPosX_sav, nPosY_sav;	// Juste pour explosion en dust over.
	nPosX_sav = gShoot.nPlayerPosX;
	nPosY_sav = gShoot.nPlayerPosY;

#ifdef DEBUG_KEYS
if (gVar.pKeys[SDLK_r])
{
	u32	r = (gShoot.nVehicleEnergy >= 10 ? 10 : gShoot.nVehicleEnergy);
	gShoot.nVehicleEnergy -= r;
	gVar.pKeys[SDLK_r] = 0;
}
#endif

	// Gestion du pilote automatique.
	Slug_AutoPilot_Manage();

	// Gestion de la victoire.
	Player_VictoryManage();

	// Gestion des phases In/Out.
	if (Slug_CtrlInOut(e_HeroVehicle_Rocket, e_HeroVehicle_None_SlugNoExit_Temp, gAnm_HeroSlugNoExit_DeathGen, 3 * PLYR_NAKED_JUMP_VAL / 4, gAnm_SlugRocket_Idle))// return;
	{
		// Explosion (en dust over, exactement superposée à celle d'en dessous).
		DustSet(gAnm_Explosion0_Big_Dust, nPosX_sav, nPosY_sav, e_Prio_DustOver, 0);
		return;
	}
	// !!! Attention : du fait de la gestion simplifiée à l'extème dans l'espace, il faut passer gAnm_HeroSlugNoExit_DeathGen
	// !!! à Slug_CtrlInOut() et pas une anim de sortie.
	// !!! Sinon, Slug_GetOut() appelle la fonction de contrôle du joueur, qui vérifie que l'anim n'est pas celle de mort
	// !!! (le Game_Player_DeathSet() est appelé juste après Slug_GetOut()), et comme c'est le cas repasse tout de suite en fusée.
	// !!! Avec un pb supplémentaire : Comme on repasse en fusée entre temps, on génère un slug.

	// *** Déplacement de la fusée.
	// Vitesse. Note : Avec autopilot, spdy * 2 : C'est pour le décollage, car une fois la transition lancée le kb est forcé à 0.
//	PlyrMove_Spd(&gShoot.nPlayerSpdX, SLUG_ROCKET_SPDX_MAX, SLUG_ROCKET_ACC, SDLK_LEFT, SDLK_RIGHT);
//	PlyrMove_Spd(&gShoot.nPlayerSpdY, (gShoot.nVehicleAutoPilot ? 2*SLUG_ROCKET_SPDY_MAX : SLUG_ROCKET_SPDY_MAX), SLUG_ROCKET_ACC, SDLK_UP, SDLK_DOWN);
	PlyrMove_Spd(&gShoot.nPlayerSpdX, SLUG_ROCKET_SPDX_MAX, SLUG_ROCKET_ACC, gMSCfg.pKeys[e_CfgKey_Left], gMSCfg.pKeys[e_CfgKey_Right]);
	PlyrMove_Spd(&gShoot.nPlayerSpdY, (gShoot.nVehicleAutoPilot ? 2*SLUG_ROCKET_SPDY_MAX : SLUG_ROCKET_SPDY_MAX), SLUG_ROCKET_ACC, gMSCfg.pKeys[e_CfgKey_Up], gMSCfg.pKeys[e_CfgKey_Down]);
	if (gShoot.nPlayerSpdX)
		AnmSetIfNew((gShoot.nPlayerSpdX < 0 ? gAnm_HeroInSlug_Rocket_LookL : gAnm_HeroInSlug_Rocket_LookR), gShoot.nPlayerAnm);	// Anim du joueur dans le slug.
	// Bords de map ou scroll bloqué.
	if (gShoot.nVehicleAutoPilot == 0)	// Si pilote automatique, pas de limites : Permet de faire sortir la fusée de la map au décollage.
	{
		PlayerMove_LimitX(SLUG_ROCKET_REF_OFFSETX);
		// En Y, cas spécifique au niveau de la roquette.
		s32	nLimitMinY, nLimitMaxY;
		nLimitMinY = gScrollPos.nPosY + (SLUG_ROCKET_HEIGHT << 8);
		nLimitMaxY = gScrollPos.nPosY + (SCR_Height << 8);
		if (gShoot.nPlayerPosY + gShoot.nPlayerSpdY <= nLimitMinY)
		{
			gShoot.nPlayerPosY = nLimitMinY;
			gShoot.nPlayerSpdY = 0;
		}
		if (gShoot.nPlayerPosY + gShoot.nPlayerSpdY >= nLimitMaxY)
		{
			gShoot.nPlayerPosY = nLimitMaxY;
			gShoot.nPlayerSpdY = 0;
		}
	}
	// Add.
	gShoot.nPlayerPosX += gShoot.nPlayerSpdX;
	gShoot.nPlayerPosY += gShoot.nPlayerSpdY;

/*
// On ne sort pas de la fusée !
	// Sortie du slug ?
	Slug_CheckKbGetOut(gAnm_SlugSubmarine_GetOut);
*/

}

// Sous routine de déclenchement d'un tir.
void SlugRocket_sub_Shot(u32 nShotNo, u32 nSpr, s32 nPosX, s32 nPosY, s32 nSparkAnmSlot, u64 *pSparkAnm)
{
	// Offset du tir.
	struct SSprRect sShotRect;
	s32	nShotOffsX, nShotOffsY;
	if (SprGetRect(nSpr, e_SprRectZone_ShotOrg, &sShotRect))
	{	// Si on a trouvé un point, tir.
		nShotOffsX = sShotRect.nX1;
		nShotOffsY = sShotRect.nY1;
		FireAdd(nShotNo, nPosX + (nShotOffsX << 8), nPosY + ((nShotOffsY - 16) << 8), 64);
		// Anim de tir.
		AnmSet(pSparkAnm, nSparkAnmSlot);
		// Dec.
		if (gShoot.nVehicleAmmo > 0) gShoot.nVehicleAmmo--;
	}
}

extern u32	gnSprSto;	// Pour supprimer les ajouts lors du clignotement.

// Affichage de la fusée.
void SlugDisplay_Rocket(void)
{
	u32	nSpr;
	u32	i;
	s32	nOffsX, nOffsY;
	u32	nFlagHit;
	u32	nSprSto_sav;

	nFlagHit = (gShoot.nVehicleHitCnt ? SPR_Flag_HitPal : 0);		// Palette hit.

	switch (gShoot.nVehiclePhase)
	{
	case e_SlugPhase_GetIn:
		// Affiche le héros en train d'entrer dans le slug.
		if (AnmCheckEnd(gShoot.nPlayerAnimInOut) == 0)
		{
			nSpr = AnmGetLastImage(gShoot.nPlayerAnimInOut);
			nSpr += gpSlugInBaseImg[gShoot.nWeapon];
			MstSlug_EntrancePtGet(e_HeroVehicle_Rocket - e_HeroVehicle_SlugBase, &nOffsX, &nOffsY);
			SprDisplay(nSpr, (gShoot.nPlayerPosX >> 8) + nOffsX, (gShoot.nPlayerPosY >> 8) + nOffsY, e_Prio_Joueur + 2);
		}
		else
		{
			// Perso dans la fusée.
			SprDisplay(AnmGetImage(gShoot.nPlayerAnm) | nFlagHit, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8, e_Prio_Joueur);
		}
		// Pas de break !
	case e_SlugPhase_GetOut:
		// L'anim d'ouverture/fermeture.
		nSpr = AnmGetLastImage(gShoot.nVehicleAnim);
		SprDisplay(nSpr | nFlagHit, gShoot.nPlayerPosX >> 8, (gShoot.nPlayerPosY >> 8) - 50, e_Prio_Joueur);
		SprDisplay(e_Spr_SlugRocket_Close_Body | nFlagHit, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8, e_Prio_Joueur-1);	// Corps de la fusée.
		break;

	//case e_SlugPhase_Normal:
	default:
//		Slug_OutSign();		// Panneau "Out" si plus assez de vie.

		AnmSetIfNew(gAnm_SlugRocket_Idle, gShoot.nVehicleAnim);		// Anim du corps de la roquette. (Obligatoire ici, cas des niveaux où on commence dans la roquette).

		nSprSto_sav = gnSprSto;		// Sauvegarde du nombre de sprites actuel dans la pile, pour un éventuel clignotement.

		// Affichage de la fusée.
		nSpr = AnmGetImage(gShoot.nVehicleAnim);
		SprDisplay(nSpr | nFlagHit, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8, e_Prio_Joueur-1);
		// Perso dans la fusée.
		SprDisplay(AnmGetImage(gShoot.nPlayerAnm) | nFlagHit, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8, e_Prio_Joueur);
		// Etincelles du tir sur le corps de la fusée.
		if ((nSpr = AnmGetImage(gShoot.nRocketBodyWeaponSparkAnm)) != SPR_NoSprite)
			SprDisplay(nSpr | nFlagHit, gShoot.nPlayerPosX >> 8, (gShoot.nPlayerPosY >> 8) - 40, e_Prio_Joueur+1);

		// Armes sur les côtés.
		for (i = 0; i < 2; i++)
		if (gShoot.pnRocketSideWeaponsAnmLR[i] != -1)
		{
			u32	nSpr2;
			nSpr = AnmGetImage(gShoot.pnRocketSideWeaponsAnmLR[i]);
			if (AnmCheckEnd(gShoot.pnRocketSideWeaponsAnmLR[i]))	// Si anim de rentrée des canons.
			{
				AnmReleaseSlot(gShoot.pnRocketSideWeaponsAnmLR[i]);
				gShoot.pnRocketSideWeaponsAnmLR[i] = -1;
				continue;
			}
			SprDisplay(nSpr | nFlagHit, gShoot.nPlayerPosX >> 8, (gShoot.nPlayerPosY >> 8) - 20, e_Prio_Joueur);
			// Etincelles des tirs.
			if ((nSpr2 = AnmGetImage(gShoot.pnRocketSideWeaponsSparkAnmLR[i])) != SPR_NoSprite)
			{
				// Offset du tir.
				struct SSprRect sShotRect;
				s32	nShotOffsX, nShotOffsY;
				if (SprGetRect(nSpr, e_SprRectZone_ShotOrg, &sShotRect))
				{	// Si on a trouvé un point, affichage de l'étincelle.
					nShotOffsX = sShotRect.nX1;
					nShotOffsY = sShotRect.nY1;
					SprDisplay(nSpr2 | nFlagHit, (gShoot.nPlayerPosX >> 8) + nShotOffsX, (gShoot.nPlayerPosY >> 8) + nShotOffsY - 20, e_Prio_Joueur+1);
				}
			}
		}

		// Réacteur + thrusters.
		if (gShoot.nVehicleAutoPilot)
		{
			// Si autopilot, trainée différente.
//			SprDisplay((e_Spr_SlugRocket_MainThrustersV + 3 + ((gnFrame >> 2) & 1)) | nFlagHit, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8, e_Prio_Joueur-2);
			SprDisplayZoom(e_Spr_SlugRocket_TakeOffTrail | nFlagHit, gShoot.nPlayerPosX >> 8, (gShoot.nPlayerPosY >> 8) - 1, e_Prio_Joueur-2, 0x100  + (gnFrame & 4 ? 0 : 0x20), 0x100 + (gnFrame & 4 ? 0x20 : 0) );
			// Bruit du réacteur.
			u32	nEngineCntMax = 64;
			SlugEngine_Play(gpSlugEngineNoises[gShoot.nVehicleType - e_HeroVehicle_SlugBase].pnFx[0], nEngineCntMax);
		}
		else
		{
			// Cas normal.

		// Réacteur principal.
		nSpr = AnmGetImage(gShoot.nRocketMainThrusterAnm);
		nSpr += (ABS(gShoot.nPlayerSpdX) >= SLUG_ROCKET_SPDX_MAX / 2 ? e_Spr_SlugRocket_MainThrustersR | (gShoot.nPlayerSpdX > 0 ? SPR_Flip_X : 0) : e_Spr_SlugRocket_MainThrustersV);
		SprDisplay(nSpr | nFlagHit, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8, e_Prio_Joueur-2);
		// Trail dust (un peu de la bidouille mais bon...).
		if ((gnFrame & 15) == 0)
			DustSetMvt(gAnm_SlugRocket_MainThruster_Dust, gShoot.nPlayerPosX - (gShoot.nPlayerSpdX * 2), gShoot.nPlayerPosY + (16 << 8), gShoot.nPlayerSpdX / 2, gShoot.nPlayerSpdY + 0x200, e_Prio_Joueur - 1, 0);

		// Small thrusters.
		for (i = 0; i < 4; i++)
		{
//			static	u32	pKeysToTest[4] = { SDLK_RIGHT, SDLK_LEFT, SDLK_UP, SDLK_DOWN };
			static	u32	pKeysToTest[4] = { e_CfgKey_Right, e_CfgKey_Left, e_CfgKey_Up, e_CfgKey_Down };
			static	u64	*pThrAnmTb[4] = { gAnm_SlugRocket_ThrustLeft, gAnm_SlugRocket_ThrustRight, gAnm_SlugRocket_ThrustUp, gAnm_SlugRocket_ThrustDown };
			static	s8	pThrOffsY[4] = { -30, -30, 0, -50 };
			static	u32	pThrLRNoWeapon[2] = { e_Spr_SlugRocket_GunL, e_Spr_SlugRocket_GunR };
			s32	nThrOffsX;

			// Anims en fonction des touches enfoncées.
//			gShoot.pnRocketThrustersAnm[i] = AnmSetIfNew((gVar.pKeys[pKeysToTest[i]] ? pThrAnmTb[i] : gAnm_SlugRocket_ThrustEmpty), gShoot.pnRocketThrustersAnm[i]);
			gShoot.pnRocketThrustersAnm[i] = AnmSetIfNew((gVar.pKeys[gMSCfg.pKeys[pKeysToTest[i]]] ? pThrAnmTb[i] : gAnm_SlugRocket_ThrustEmpty), gShoot.pnRocketThrustersAnm[i]);
			// Décalage sur les côtés (à cause des side weapons).
			nThrOffsX = 0;
			if (i < 2)
			{
				struct SSprRect sRect2;
				u32	nSpr2;
				nSpr2 = (gShoot.pnRocketSideWeaponsAnmLR[i] != -1 ? AnmGetImage(gShoot.pnRocketSideWeaponsAnmLR[i]) : pThrLRNoWeapon[i]);
				if (SprGetRect(nSpr2, e_SprRectZone_RectCol, &sRect2))
				if (sRect2.nType == e_SprRect_Rect)
					nThrOffsX = (i == 0 ? sRect2.nX1 - 7 : sRect2.nX2 + 7);
			}
			// Display.
			nSpr = AnmGetImage(gShoot.pnRocketThrustersAnm[i]);
			if (nSpr != SPR_NoSprite)
				SprDisplay(nSpr | nFlagHit, (gShoot.nPlayerPosX >> 8) + nThrOffsX, (gShoot.nPlayerPosY >> 8) + (s32)pThrOffsY[i], e_Prio_Joueur);
		}

		}

		if ((gShoot.nInvulnerability & PLYR_INVL_BLINK) != 0)	// Clignotement quand invulnérable ?
			gnSprSto = nSprSto_sav;		// On "annule" l'affichage des sprites de la fusée.

		break;
	}


	// Bombe ?
//	if (gVar.pKeys[KB_BUTTONC] && gShoot.nVehicleBombAmmo)
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonC]] && gShoot.nVehicleBombAmmo)
	{
		struct SSprRect sSlugRect;
		if (SprGetRect(e_Spr_SlugRocket_Idle, e_SprRectZone_ShotOrg, &sSlugRect))
		if (sSlugRect.nType == e_SprRect_Point)
		{
			FireAdd(e_Shot_Player_SlugRocket_Rocket_Launch, gShoot.nPlayerPosX + (sSlugRect.nX1 << 8), gShoot.nPlayerPosY  + (sSlugRect.nY1 << 8), 64);
			Sfx_PlaySfx(e_Sfx_Shot_MiniRocket, e_SfxPrio_Hero_Shot);	// !!changer!!
//			gVar.pKeys[KB_BUTTONC] = 0;
			gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonC]] = 0;
			gShoot.nVehicleBombAmmo--;
		}
	}


	// Tir ?
	static u8	nCntS = 0;
//	static u32 gpHeroShotClearKb[e_Player_Weapon_Max] = { 1, 1, 0, 0, 1 };	// Doit-on clearer la touche de tir, en fct de l'arme.
//	if (gVar.pKeys[KB_BUTTONA] && nCntS == 0)
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] && nCntS == 0)
	{
		// Rocket's body.
		SlugRocket_sub_Shot(e_Shot_Player_SlugRocket_MachinegunV, e_Spr_SlugRocket_Idle, gShoot.nPlayerPosX, gShoot.nPlayerPosY, gShoot.nRocketBodyWeaponSparkAnm, gAnm_SlugRocket_ShotSparkMain);
		// Armes sur les côtés.
		for (i = 0; i < 2; i++)
		if (gShoot.nVehicleAmmo > 0)
		if (gShoot.pnRocketSideWeaponsAnmLR[i] != -1)
		{
			nSpr = AnmGetLastImage(gShoot.pnRocketSideWeaponsAnmLR[i]);
			SlugRocket_sub_Shot(e_Shot_Player_SlugRocket_MachinegunV, nSpr, gShoot.nPlayerPosX, gShoot.nPlayerPosY - (20 << 8), gShoot.pnRocketSideWeaponsSparkAnmLR[i], (i == 0 ? gAnm_SlugRocket_ShotSparkSideL : gAnm_SlugRocket_ShotSparkSideR));
		}
		nCntS = 8;
		// Plus de balles ? => Repasse en Gun.
		if (gShoot.nVehicleAmmo == 0) Rocket_WeaponSet(e_Player_Weapon_Gun);
	}
//	if (gpHeroShotClearKb[gShoot.nRocketWeapon]) gVar.pKeys[KB_BUTTONA] = 0;	// Clear touche de tir si nécessaire.
//	if (gVar.pKeys[KB_BUTTONA] == 0) nCntS = 0;
	if (gpHeroShotClearKb[gShoot.nRocketWeapon]) gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] = 0;	// Clear touche de tir si nécessaire.
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] == 0) nCntS = 0;
	if (nCntS) nCntS--;

	// Sprite de col.
	gShoot.nPlayerSprCol = e_Spr_SlugRocket_Idle;	// Pas de flip pour les slugs.
	Player_ShotHitCheck();	// Teste si on se fait toucher par un tir.

	// Vulcan.
//	VulcanCannon_Manage(nSpr);

}

//=================

// Contrôle de l'hélico.
void SlugControl_Helicopter(void)
{
	s32	nPosX_sav, nPosY_sav;	// Juste pour explosion en dust over.
	nPosX_sav = gShoot.nPlayerPosX;
	nPosY_sav = gShoot.nPlayerPosY;

#ifdef DEBUG_KEYS
if (gVar.pKeys[SDLK_r])
{
	u32	r = (gShoot.nVehicleEnergy >= 10 ? 10 : gShoot.nVehicleEnergy);
	gShoot.nVehicleEnergy -= r;
	gVar.pKeys[SDLK_r] = 0;
}
#endif

	// Gestion du pilote automatique.
	Slug_AutoPilot_Manage();

	// Gestion de la victoire.
	Player_VictoryManage();

/*
	// Gestion des phases In/Out.
//	if (Slug_CtrlInOut(e_HeroVehicle_Helicopter, e_HeroVehicle_None_Naked, gAnm_Hero_SlugOutGen, 2 * PLYR_NAKED_JUMP_VAL / 3, gAnm_SlugCopter_Idle)) return;
	if (Slug_CtrlInOut(e_HeroVehicle_Helicopter, e_HeroVehicle_None_Naked, gAnm_Hero_SlugOutGen, 3 * PLYR_NAKED_JUMP_VAL / 4, gAnm_SlugCopter_Idle)) return;
*/

	// Gestion des phases In/Out.
	if (gShoot.nVehicleType == e_HeroVehicle_HelicopterNoExit)
	{	// Cas de l'hélico "no exit" (lev2).
		if (Slug_CtrlInOut(e_HeroVehicle_HelicopterNoExit, e_HeroVehicle_None_SlugNoExit_Temp, gAnm_HeroSlugNoExit_DeathGen, 3 * PLYR_NAKED_JUMP_VAL / 4, gAnm_SlugRocket_Idle))// return;
		{
			// Explosion (en dust over, exactement superposée à celle d'en dessous).
			DustSet(gAnm_Explosion0_Big_Dust, nPosX_sav, nPosY_sav, e_Prio_DustOver, 0);
			return;
		}
		// !!! Voir explication dans SlugControl_Rocket() !!!
	}
	else
	{	// Cas hélico normal.
		if (Slug_CtrlInOut(e_HeroVehicle_Helicopter, e_HeroVehicle_None_Naked, gAnm_Hero_SlugOutGen, 3 * PLYR_NAKED_JUMP_VAL / 4, gAnm_SlugCopter_Idle)) return;
	}


	// *** Déplacement du slug.
	// Vitesse.
//	PlyrMove_Spd(&gShoot.nPlayerSpdX, SLUG_HELICOPTER_SPDX_MAX, SLUG_HELICOPTER_ACC, SDLK_LEFT, SDLK_RIGHT);
//	PlyrMove_Spd(&gShoot.nPlayerSpdY, SLUG_HELICOPTER_SPDY_MAX, SLUG_HELICOPTER_ACC, SDLK_UP, SDLK_DOWN);
	PlyrMove_Spd(&gShoot.nPlayerSpdX, SLUG_HELICOPTER_SPDX_MAX, SLUG_HELICOPTER_ACC, gMSCfg.pKeys[e_CfgKey_Left], gMSCfg.pKeys[e_CfgKey_Right]);
	PlyrMove_Spd(&gShoot.nPlayerSpdY, SLUG_HELICOPTER_SPDY_MAX, SLUG_HELICOPTER_ACC, gMSCfg.pKeys[e_CfgKey_Up], gMSCfg.pKeys[e_CfgKey_Down]);
	// Bords de map ou scroll bloqué.
	PlayerMove_LimitX(SLUG_HELICOPTER_REF_OFFSETX);
	PlayerMove_LimitY(SLUG_HELICOPTER_HEIGHT);
	PlayerMove_LimitY_Bottom(SLUG_HELICOPTER_HEIGHT_B);
	// Sol dte/gche.
	Gen_SideCheck(SLUG_HELICOPTER_REF_OFFSETX, SLUG_HELICOPTER_SPDX_MAX);	// Mettra la vitesse à 0 si mouvement pas possible.
	// Add.
	gShoot.nPlayerPosX += gShoot.nPlayerSpdX;
	gShoot.nPlayerPosY += gShoot.nPlayerSpdY;

	// *** Test du sol.
	gShoot.nPlayerLastGnd = gShoot.nPlayerGnd = 0;		// RAZ gnd.
	Gen_GroundCheck(gShoot.nPlayerPosX);
	Gen_GroundCheck(gShoot.nPlayerPosX + SLUG_HELICOPTER_REF_OFFSETX);
	Gen_GroundCheck(gShoot.nPlayerPosX - SLUG_HELICOPTER_REF_OFFSETX);
	// Si pas de sol, on met à jour le LastY.
	//if (gShoot.nPlayerGnd == 0) gShoot.nPlayerLastGndPosY = gShoot.nPlayerPosY + 0x1800;		// Hmmm... Le recalage du scroll est très violent.
	gShoot.nPlayerLastGndPosY = gShoot.nPlayerPosY;

	// Inclinaison : b0-b7 = X / b8-b15 = Y.
	gShoot.nVehicleReqAdd = SLUG_HELICOPTER_DEFAULTSPRADD;
/*
	if (gVar.pKeys[SDLK_UP]) gShoot.nVehicleReqAdd = (gShoot.nVehicleReqAdd & ~0xFF00) | 0x0800;
	if (gVar.pKeys[SDLK_DOWN]) gShoot.nVehicleReqAdd = (gShoot.nVehicleReqAdd & ~0xFF00) | 0x4800;
	if (gVar.pKeys[SDLK_LEFT]) gShoot.nVehicleReqAdd = (gShoot.nVehicleReqAdd & ~0xFF) | 0x08;
	if (gVar.pKeys[SDLK_RIGHT]) gShoot.nVehicleReqAdd = (gShoot.nVehicleReqAdd & ~0xFF) | 0x48;
*/
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Up]]) gShoot.nVehicleReqAdd = (gShoot.nVehicleReqAdd & ~0xFF00) | 0x0800;
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]]) gShoot.nVehicleReqAdd = (gShoot.nVehicleReqAdd & ~0xFF00) | 0x4800;
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Left]]) gShoot.nVehicleReqAdd = (gShoot.nVehicleReqAdd & ~0xFF) | 0x08;
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Right]]) gShoot.nVehicleReqAdd = (gShoot.nVehicleReqAdd & ~0xFF) | 0x48;
	if (gShoot.nPlayerGnd)
	{	// Si au sol, RAZ inclinaison Y.
		gShoot.nVehicleCurAdd = (gShoot.nVehicleCurAdd & ~0xFF00) | (SLUG_HELICOPTER_DEFAULTSPRADD & 0xFF00);
		gShoot.nVehicleReqAdd = (gShoot.nVehicleReqAdd & ~0xFF00) | (SLUG_HELICOPTER_DEFAULTSPRADD & 0xFF00);
	}
	if ((gShoot.nVehicleCurAdd & 0xFF) != (gShoot.nVehicleReqAdd & 0xFF))
		gShoot.nVehicleCurAdd += ((gShoot.nVehicleCurAdd & 0xFF) < (gShoot.nVehicleReqAdd & 0xFF) ? 1 : -1);
	if ((gShoot.nVehicleCurAdd & 0xFF00) != (gShoot.nVehicleReqAdd & 0xFF00))
		gShoot.nVehicleCurAdd += ((gShoot.nVehicleCurAdd  & 0xFF00) < (gShoot.nVehicleReqAdd & 0xFF00) ? 0x100 : -0x100);

	// Sortie du slug ?
	if (gShoot.nPlayerGnd) Slug_CheckKbGetOut(gAnm_SlugCopter_GetOut);

	// Bruit de l'hélice.
	if (gShoot.nVehiclePhase != e_SlugPhase_GetOut)
	{
		u32	nEngineCntMax = (ABS(gShoot.nPlayerSpdX) > SLUG_HELICOPTER_SPDX_MAX / 2 ||
							 ABS(gShoot.nPlayerSpdY) > SLUG_HELICOPTER_SPDY_MAX / 2 ? 8 : 10);
		SlugEngine_Play(gpSlugEngineNoises[gShoot.nVehicleType - e_HeroVehicle_SlugBase].pnFx[0], nEngineCntMax);
	}

}

// Affichage de l'hélico.
void SlugDisplay_Helicopter(void)
{
	u32	nSpr, nSpr2;
	s32	nAdd;
//	s32	nOffsX, nOffsY;
	u32	nFlagHit;

	nFlagHit = (gShoot.nVehicleHitCnt ? SPR_Flag_HitPal : 0);		// Palette hit.

	switch (gShoot.nVehiclePhase)
	{
	case e_SlugPhase_GetIn:
/*
//!!! Pas pour l'hélico !!!
		// Affiche le héros en train d'entrer dans le slug.
		if (AnmCheckEnd(gShoot.nPlayerAnimInOut) == 0)
		{
			nSpr = AnmGetLastImage(gShoot.nPlayerAnimInOut);
			nSpr += e_Spr_HeroSub_SlugIn_Gun;	// Si plusieurs tables, faire une table en static.
			MstSlug_EntrancePtGet(e_HeroVehicle_Helicopter - e_HeroVehicle_SlugBase, &nOffsX, &nOffsY);
			SprDisplay(nSpr, (gShoot.nPlayerPosX >> 8) + nOffsX, (gShoot.nPlayerPosY >> 8) + nOffsY, e_Prio_Joueur + 2);
		}
*/
		// Pas de break !
	case e_SlugPhase_GetOut:
		nSpr = AnmGetLastImage(gShoot.nVehicleAnim);
		// L'hélice.
		SprDisplay(((nSpr - e_Spr_SlugCopter_Close_Body) + e_Spr_SlugCopter_Close_Blades) | nFlagHit, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8, e_Prio_Joueur - 2);
		// L'anim d'ouverture/fermeture (body).
		SprDisplay(nSpr | nFlagHit, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8, e_Prio_Joueur - 1);
		// Le perso assis pendant l'ouverture/fermeture.
		SprDisplay((nSpr - e_Spr_SlugCopter_Close_Body) + e_Spr_Hero_InSlug_Copter_GetIn, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8, e_Prio_Joueur);
		break;

	//case e_SlugPhase_Normal:
	default:
		if (gShoot.nVehicleType != e_HeroVehicle_HelicopterNoExit)
			Slug_OutSign();		// Panneau "Out" si plus assez de vie.

		// Hélice.
		static u8	pAnmBlades[] = { 0,0, 1, 2,2, 3, 4,4, 5, 0,0, 6, 2,2, 7, 4,4, 8 };
		if (++gShoot.nVehicleFrmCnt0 >= 18*2) gShoot.nVehicleFrmCnt0 = 0;
		nSpr2 = e_Spr_SlugCopter_Blades + (((gShoot.nVehicleCurAdd >> 4) & 0x0F) * 9 * 5);
		nSpr2 += ((gShoot.nVehicleCurAdd >> 12) & 0x0F) * 9;
		nSpr2 += pAnmBlades[gShoot.nVehicleFrmCnt0 >> 1];
		// Body.
		nAdd = (gShoot.nVehicleCurAdd >> 4) & 0x0F;				// Inclinaison droite/gauche.
		nAdd += ((gShoot.nVehicleCurAdd >> 12) & 0x0F) * 5;		// Inclinaison haut/bas.
		nSpr = e_Spr_SlugCopter_Body + nAdd;

		if ((gShoot.nInvulnerability & PLYR_INVL_BLINK) == 0)	// Clignotement quand invulnérable ?
		{
			// Hélice.
			SprDisplay(nSpr2 | nFlagHit, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8, e_Prio_Joueur - 2);
			// Body.
			SprDisplay(nSpr | nFlagHit, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8, e_Prio_Joueur - 1);
			// Perso dans l'hélico.
			SprDisplay(e_Spr_Hero_InSlug_Copter + nAdd, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8, e_Prio_Joueur);
		}
		break;
	}

	// Bombe ?
//	if (gVar.pKeys[KB_BUTTONC] && gShoot.nVehicleBombAmmo)
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonC]] && gShoot.nVehicleBombAmmo)
	{
		struct SSprRect sSlugRect;
		if (SprGetRect(nSpr, e_SprRectZone_ShotOrg, &sSlugRect))
		if (sSlugRect.nType == e_SprRect_Rect)
		{
			// Origine du tir : Point supérieur droit (+16).
//			FireAdd(e_Shot_Player_SlugCopter_Bomb_Drop, gShoot.nPlayerPosX + (sSlugRect.nX2 << 8), gShoot.nPlayerPosY  + ((sSlugRect.nY1 + 16) << 8), 0);

			if (gShoot.nVehicleType == e_HeroVehicle_HelicopterNoExit)
			{	// Hélico no exit : Missile.
//				FireAdd(e_Shot_Player_RocketSmall_Launch, gShoot.nPlayerPosX + (sSlugRect.nX2 << 8), gShoot.nPlayerPosY  + ((sSlugRect.nY1 + 16) << 8), -gShoot.nPlayerSpdY >> 7);
				FireAdd(e_Shot_Player_RocketSmall_Launch, gShoot.nPlayerPosX + (sSlugRect.nX2 << 8) + (8<<8), gShoot.nPlayerPosY  + ((sSlugRect.nY1 + 16) << 8) - (8<<8) + (gShoot.nPlayerSpdY * 4), 0);
				Sfx_PlaySfx(e_Sfx_Shot_MiniRocket, e_SfxPrio_Hero_Shot);	// !!changer!!
			}
			else
			{	// Hélico normal : Bombe.
				// Tir + Modification de la vitesse X du tir.
				s32	nFireSlot;
				nFireSlot = FireAdd(e_Shot_Player_SlugCopter_Bomb_Drop, gShoot.nPlayerPosX + (sSlugRect.nX2 << 8), gShoot.nPlayerPosY  + ((sSlugRect.nY1 + 16) << 8), 0);
				Sfx_PlaySfx(e_Sfx_Shot_Gun, e_SfxPrio_Hero_Shot);	// !!changer!!
				if (nFireSlot != -1)
				{
					s16	*pSpdX;
					pSpdX = FireGetSpdXPtr(nFireSlot);
					*pSpdX = gShoot.nPlayerSpdX;
				}
			}

//			gVar.pKeys[KB_BUTTONC] = 0;
			gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonC]] = 0;
			gShoot.nVehicleBombAmmo--;
		}
	}

	// Sprite de col.
	gShoot.nPlayerSprCol = e_Spr_SlugCopter_Body + 12;	// Pas de flip pour les slugs.
	Player_ShotHitCheck();	// Teste si on se fait toucher par un tir.

	// Vulcan.
	VulcanCannon_Manage(nSpr);

}

//=================

u64 *gpSlugTankAnm[] =
{
	gAnm_SlugTank_Idle,
	gAnm_SlugTank_CrouchIdle,
	gAnm_SlugTank_DriveForward,
	gAnm_SlugTank_CrouchDriveForward,
	gAnm_SlugTank_DriveBackward,
	gAnm_SlugTank_CrouchDriveBackward,
	gAnm_SlugTank_DriveStop,
	gAnm_SlugTank_CrouchDriveStop,
	gAnm_SlugTank_Jump,
	gAnm_SlugTank_Fall,
	gAnm_SlugTank_Up2Down,
	gAnm_SlugTank_Down2Up,

};
enum
{
	e_Anm_SlugTank_Stance = 0,				// !!! Attention à l'ordre !!! +1 pour passer de normal à accroupi.
	e_Anm_SlugTank_CrouchStance,
	e_Anm_SlugTank_DriveForward,
	e_Anm_SlugTank_CrouchDriveForward,
	e_Anm_SlugTank_DriveBackward,
	e_Anm_SlugTank_CrouchDriveBackward,
	e_Anm_SlugTank_DriveStop,
	e_Anm_SlugTank_CrouchDriveStop,

	e_Anm_SlugTank_Jump,
	e_Anm_SlugTank_Fall,

	e_Anm_SlugTank_Up2Down,
	e_Anm_SlugTank_Down2Up,

};


// Contrôle du tank.
void SlugControl_Tank(void)
{
	s32	nAnmNo;
	u32	nAnmKey;

#ifdef DEBUG_KEYS
if (gVar.pKeys[SDLK_r])
{
	u32	r = (gShoot.nVehicleEnergy >= 10 ? 10 : gShoot.nVehicleEnergy);
	gShoot.nVehicleEnergy -= r;
	gVar.pKeys[SDLK_r] = 0;
}
#endif

	// Gestion du pilote automatique. (How to play).
	Slug_AutoPilot_Manage();

	// Gestion de la victoire.
	if (Player_VictoryManage()) return;

	nAnmNo = e_Anm_SlugTank_Stance;

	// Gestion des phases In/Out.
//	if (Slug_CtrlInOut(e_HeroVehicle_Tank, e_HeroVehicle_None_Naked, gAnm_Hero_SlugOutGen, 2 * PLYR_NAKED_JUMP_VAL / 3, gAnm_SlugTank_Idle)) return;
	if (Slug_CtrlInOut(e_HeroVehicle_Tank, e_HeroVehicle_None_Naked, gAnm_Hero_SlugOutGen, 2 * PLYR_NAKED_JUMP_VAL / 3, gAnm_SlugTank_CrouchIdle)) return;

	// *** Déplacement du tank.
	// Vitesse. (Spd Max X = cos(angle du tank)).
	s32	nSpdXMax = (gShoot.nPlayerGnd ? (SLUG_TANK_SPDX_MAX * ABS(gVar.pCos[gShoot.nVehicleCurAdd & 0xFF])) / 256 : SLUG_TANK_SPDX_MAX);
//(a voir, mais plutôt non)	if (AnmGetKey(gShoot.nVehicleAnim) == e_AnmKey_Hero_UpDown) nSpdXMax = 0;	// Cas de l'anim UpDown.
//	if (PlyrMove_Spd(&gShoot.nPlayerSpdX, nSpdXMax, SLUG_TANK_ACCX, SDLK_LEFT, SDLK_RIGHT)) nAnmNo = (gShoot.nPlayerSpdX >= 0 ? e_Anm_SlugTank_DriveForward : e_Anm_SlugTank_DriveBackward);
	if (PlyrMove_Spd(&gShoot.nPlayerSpdX, nSpdXMax, SLUG_TANK_ACCX, gMSCfg.pKeys[e_CfgKey_Left], gMSCfg.pKeys[e_CfgKey_Right])) nAnmNo = (gShoot.nPlayerSpdX >= 0 ? e_Anm_SlugTank_DriveForward : e_Anm_SlugTank_DriveBackward);
	// Anim d'arrêt ?
	if (nAnmNo == e_Anm_SlugTank_Stance && gShoot.nPlayerSpdX > 0) nAnmNo = e_Anm_SlugTank_DriveStop;

	// Bords de map ou scroll bloqué.
	PlayerMove_LimitX(SLUG_TANK_REF_OFFSETX);
	PlayerMove_LimitY(SLUG_TANK_HEIGHT);

/*
	// Gestion de l'accroupi. (v1 : sans anims de UpDown).
//	if (gVar.pKeys[KB_BUTTONA] == 0)
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] == 0)
	{
		// Pas de tir (vulcan), on regarde si le joueur veut se baisser.
//		if (gVar.pKeys[SDLK_DOWN]) nAnmNo++;	// Ok car à ce moment là, seulement Idle ou Run.
		if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]]) nAnmNo++;	// Ok car à ce moment là, seulement Idle ou Run.
	}
	else
	{
		// Tir en cours (vulcan), si anm == crouch, on reste baissé.
		nAnmKey = AnmGetKey(gShoot.nVehicleAnim);
		if (nAnmKey == e_AnmKey_Hero_CrouchStance || nAnmKey == e_AnmKey_Hero_CrouchWalk ||
			nAnmKey == e_AnmKey_Hero_CrouchWalkStop) nAnmNo++;
	}
*/

	// Gestion de l'accroupi. (v2 : avec anims de UpDown).
	nAnmKey = AnmGetKey(gShoot.nVehicleAnim);
	u8	nCrouch = 0;
	if (nAnmKey == e_AnmKey_Hero_CrouchStance || nAnmKey == e_AnmKey_Hero_CrouchWalk ||
		nAnmKey == e_AnmKey_Hero_CrouchWalkStop) nCrouch = 1;
//	if (gVar.pKeys[KB_BUTTONA] == 0)
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] == 0)
	{
		// Pas de tir (vulcan), on regarde si le joueur veut se baisser.
//		if (gVar.pKeys[SDLK_DOWN])
		if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]])
		{
			nAnmNo++;	// Ok car à ce moment là, seulement Idle ou Run.
			if (nCrouch == 0 || nAnmKey == e_AnmKey_Hero_UpDown) nAnmNo = e_Anm_SlugTank_Up2Down;
		}
		else
		{
			if (nCrouch || nAnmKey == e_AnmKey_Hero_UpDown) nAnmNo = e_Anm_SlugTank_Down2Up;
		}
	}
	else
	{
		// Tir en cours (vulcan), si crouch, on reste baissé.
		if (nCrouch) nAnmNo++;
		if (nAnmKey == e_AnmKey_Hero_UpDown) nAnmNo = -1;	// Et si UpDown, rien.
	}

	// Sol dte/gche.
	Gen_SideCheck(SLUG_TANK_REF_OFFSETX, SLUG_TANK_SPDX_MAX);	// Mettra la vitesse à 0 si mouvement pas possible.
	// Add.
	gShoot.nPlayerPosX += gShoot.nPlayerSpdX;

	// Sortie du slug ?
//	if (gShoot.nPlayerGnd) Slug_CheckKbGetOut(gAnm_SlugTank_GetOut);
	if (gShoot.nPlayerGnd)
		if (Slug_CheckKbGetOut(gAnm_SlugTank_GetOut)) gShoot.nPlayerSpdX = 0;	// RAZ spd x pour le tank, sinon au bord des pf on peut tomber pendant l'ouverture.

	// Jump ?
	Gen_ButtonB_Check(SLUG_TANK_JUMP_VAL);

	// *** Test du sol.
	Gen_GroundManage(SLUG_TANK_SPDX_MAX, SLUG_TANK_SPDY_MAX, SLUG_TANK_REF_OFFSETX, 0);

	// * Inclinaison en fonction de la pente. On prend l'angle entre les deux points extremes (sauf si "marches").
	s32	nHtL, nHtR;
	nHtL = BlockGetGroundLevel((gShoot.nPlayerPosX - SLUG_TANK_REF_OFFSETX) >> 8, gShoot.nPlayerPosY >> 8);
	nHtR = BlockGetGroundLevel((gShoot.nPlayerPosX + SLUG_TANK_REF_OFFSETX) >> 8, gShoot.nPlayerPosY >> 8);
//>> marches
	s32	nHtM, nDiffX, nShift;
	nHtM = BlockGetGroundLevel((gShoot.nPlayerPosX) >> 8, gShoot.nPlayerPosY >> 8);

	nDiffX = SLUG_TANK_REF_OFFSETX * 2;		// Pour fatan2.
	nShift = 1;								// Pour calcul offset y.

	// Gestion des "marches" verticales. Test sur le point central.
	// (Bien garder en tête que le point qui touche est obligatoirement le point le plus haut).
	if (nHtL - nHtM > SLUG_TANK_REF_OFFSETX >> 8)	// Pt à gauche en l'air.
	{
		nHtL = nHtM;
		nDiffX = SLUG_TANK_REF_OFFSETX;
		nShift = 0;
	}
	else
	if (nHtR - nHtM > SLUG_TANK_REF_OFFSETX >> 8)	// Pt à droite en l'air.
	{
		nHtR = nHtM;
		nDiffX = SLUG_TANK_REF_OFFSETX;
		nShift = 0;
	}
	if (nHtL - nHtM < -SLUG_TANK_REF_OFFSETX >> 8)	// Pts à droite et central en l'air, seul le pt gauche touche.
	{
		if (nHtM > nHtR) nHtM = nHtR;	// Cas foireux ou il y a des pf les unes au dessus des autres. => 'if ' testé sur le boss du l13.
		//
		nHtR = nHtL + (nHtR - nHtM);
		nDiffX = SLUG_TANK_REF_OFFSETX;
		nShift = 0;
	}
	else
	if (nHtR - nHtM < -SLUG_TANK_REF_OFFSETX >> 8)	// Pts à gauche et central en l'air, seul le pt droit touche.
	{
		if (nHtM > nHtL) nHtM = nHtL;	// Cas foireux ou il y a des pf les unes au dessus des autres. => 'if ' non testé.
		//
		nHtL = nHtR + (nHtL - nHtM);
		nDiffX = SLUG_TANK_REF_OFFSETX;
		nShift = 0;
	}
//<< marches

//m	gShoot.nVehicleOffsY = ABS(nHtL - nHtR) / 2;	// Décalage pour l'affichage.
	gShoot.nVehicleOffsY = ABS(nHtL - nHtR) >> nShift;	// Décalage Y pour l'affichage.

	if (gShoot.nPlayerGnd)
	{
		// Au sol.
//m		gShoot.nVehicleCurAdd = fatan2((nHtL - nHtR) << 8, SLUG_TANK_REF_OFFSETX * 2);
		gShoot.nVehicleCurAdd = fatan2((nHtL - nHtR) << 8, nDiffX);
	}
	else if (gShoot.nPlayerSpdY > 0)
	{
		// En l'air, on tourne un peu juste avant de retoucher le sol pour éviter un gros accoup.
		if (BlockGetGroundLevel(gShoot.nPlayerPosX >> 8, (gShoot.nPlayerPosY >> 8) + gShoot.nVehicleOffsY) <= 16)
		{
//m			s32 nAng = fatan2((nHtL - nHtR) << 8, SLUG_TANK_REF_OFFSETX * 2);
			s32 nAng = fatan2((nHtL - nHtR) << 8, nDiffX);
			gShoot.nVehicleCurAdd += (nAng - (s32)gShoot.nVehicleCurAdd) / 4;
		}
	}
	gShoot.nVehicleCurAdd &= 0xFF;	// Fix 11/10/2012. Sinon pb quand accès à table de sinus, ligne 2462. 

	// Anim de jump/fall.
	if (gShoot.nPlayerGnd == 0) nAnmNo = (gShoot.nPlayerSpdY < 0 ? e_Anm_SlugTank_Jump : e_Anm_SlugTank_Fall);

	// Anim.
	if (gShoot.nVehiclePhase == e_SlugPhase_Normal)
	if (nAnmNo != -1)
	{
		nAnmKey = AnmGetKey(gShoot.nVehicleAnim);
		// Cas particulier pour laisser l'anim de drive stop.
		if ( !((nAnmNo == e_Anm_SlugTank_Stance || nAnmNo == e_Anm_SlugTank_CrouchStance) && (nAnmKey == e_AnmKey_Hero_RunStop || nAnmKey == e_AnmKey_Hero_CrouchWalkStop)) )
			AnmSetIfNew(gpSlugTankAnm[nAnmNo], gShoot.nVehicleAnim);
	}

	// Sortie du slug ?
//	if (gShoot.nPlayerGnd) Slug_CheckKbGetOut(gAnm_SlugTank_GetOut);	// Mis plus haut.

	// Bruit du moteur.
	if (gShoot.nVehiclePhase != e_SlugPhase_GetOut)
	{
		u32	nEngineCntMax = 18;
		u32	nKeysMvt = !!(gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Left]] | gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Right]]);
		u32	nSfxToTest = gpSlugEngineNoises[gShoot.nVehicleType - e_HeroVehicle_SlugBase].pnFx[!nKeysMvt];
		if (Sfx_IsPlaying(nSfxToTest))
		{
			Sfx_StopSfx(nSfxToTest);
			nEngineCntMax = 0;
		}
		SlugEngine_Play(gpSlugEngineNoises[gShoot.nVehicleType - e_HeroVehicle_SlugBase].pnFx[nKeysMvt], nEngineCntMax);
	}

}

void Gen_StepDust(u32 nVehicle, u32 nGndCol, s32 nOffset, u32 nStepFlag, u32 nPosX, u32 nPosY);
void Gen_LandDust(u32 nVehicle, u32 nGndCol, u32 nPosX, u32 nPosY);

// Affichage du tank.
void SlugDisplay_Tank(void)
{
	u32	nSpr;
	s32	nOffsX, nOffsY;
	u32	nFlagHit;

	nFlagHit = (gShoot.nVehicleHitCnt ? SPR_Flag_HitPal : 0);		// Palette hit.

	// Le décalage - Add.
	gShoot.nPlayerPosY += gShoot.nVehicleOffsY << 8;

	switch (gShoot.nVehiclePhase)
	{
	case e_SlugPhase_GetIn:
		// Affiche le héros en train d'entrer dans le slug.
		if (AnmCheckEnd(gShoot.nPlayerAnimInOut) == 0)
		{
			nSpr = AnmGetLastImage(gShoot.nPlayerAnimInOut);
			nSpr += gpSlugInBaseImg[gShoot.nWeapon];
			MstSlug_EntrancePtGet(e_HeroVehicle_Tank - e_HeroVehicle_SlugBase, &nOffsX, &nOffsY);
			// Rotation de l'offset d'entrée.
			Rot2D_RotatePoint(&nOffsX, &nOffsY, gShoot.nVehicleCurAdd);
			SprDisplay(nSpr, (gShoot.nPlayerPosX >> 8) + nOffsX, (gShoot.nPlayerPosY >> 8) + nOffsY, e_Prio_Joueur + 2);
		}
		// Pas de break !
	case e_SlugPhase_GetOut:
		// L'anim d'ouverture/fermeture.
		nSpr = AnmGetLastImage(gShoot.nVehicleAnim);
		SprDisplayRotoZoom(nSpr | nFlagHit, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8, e_Prio_Joueur, gShoot.nVehicleCurAdd, 0x0100);
		break;

	//case e_SlugPhase_Normal:
	default:
		Slug_OutSign();		// Panneau "Out" si plus assez de vie.

		// Slug.
		nSpr = AnmGetImage(gShoot.nVehicleAnim);
		SprDisplayRotoZoom(nSpr | nFlagHit, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8, e_Prio_Joueur, gShoot.nVehicleCurAdd, 0x0100);
//SprDisplay(e_Spr_Tstrct_Cross, (gShoot.nPlayerPosX >> 8), (gShoot.nPlayerPosY >> 8), 255);
		break;
	}

	// Point de lancer du tir.
	struct SSprRect sSlugRect;
	nOffsX = nOffsY = 0;	// Par défaut.
	if (SprGetRect(nSpr, e_SprRectZone_ShotOrg, &sSlugRect))
	if (sSlugRect.nType == e_SprRect_Rect)
	{
		// Origine du tir : Point supérieur droit (+16).
		nOffsX = sSlugRect.nX2;
		nOffsY = sSlugRect.nY1 + 16;
		Rot2D_RotatePoint(&nOffsX, &nOffsY, gShoot.nVehicleCurAdd);
	}

	// Bombe ?
//	if (gVar.pKeys[KB_BUTTONC] && gShoot.nVehicleBombAmmo)
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonC]] && gShoot.nVehicleBombAmmo)
	{
		gShoot.nPlayerAnmTop = AnmSet(gAnm_SlugTank_Shot, gShoot.nPlayerAnmTop);

		// Tir.
		FireAdd(e_Shot_Player_RocketSmall_Launch, gShoot.nPlayerPosX + (nOffsX << 8), gShoot.nPlayerPosY + (nOffsY << 8), gShoot.nVehicleCurAdd);
		Sfx_PlaySfx(e_Sfx_Shot_Mortar, e_SfxPrio_Hero_Shot);

//		gVar.pKeys[KB_BUTTONC] = 0;
		gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonC]] = 0;
		gShoot.nVehicleBombAmmo--;
	}

	// Anim de tir ?
	if (gShoot.nPlayerAnmTop != -1)
	{
		u32	i;

		i = AnmGetImage(gShoot.nPlayerAnmTop);
		if (i == (u32)-1)
		{
			gShoot.nPlayerAnmTop = -1;		// Anim du haut finie.
		}
		else
		{
//			SprDisplayRotoZoom(i, (gShoot.nPlayerPosX >> 8) + sPtRot.nX, (gShoot.nPlayerPosY >> 8) + sPtRot.nY, e_Prio_Joueur - 1, gShoot.nVehicleCurAdd, 0x0100);
			SprDisplayRotoZoom(i, (gShoot.nPlayerPosX >> 8) + nOffsX, (gShoot.nPlayerPosY >> 8) + nOffsY, e_Prio_Joueur - 1, gShoot.nVehicleCurAdd, 0x0100);
		}
	}

	// Vulcan.
	VulcanCannon_Manage(nSpr);

	// Sprite de col.
	// e_Spr_SlugTank_Idle ou e_Spr_SlugTankCrouch_Idle en fct de la clef d'anim.
	u32 nAnmKey = AnmGetKey(gShoot.nVehicleAnim);
	gShoot.nPlayerSprCol = 			// Pas besoin du flip pour les slugs.
		(nAnmKey == e_AnmKey_Hero_CrouchStance || nAnmKey == e_AnmKey_Hero_CrouchWalk ||
		 nAnmKey == e_AnmKey_Hero_CrouchWalkStop || nAnmKey == e_AnmKey_Hero_UpDown ? e_Spr_SlugTankCrouch_Idle : e_Spr_SlugTank_Idle);

	Player_ShotHitCheck();	// Teste si on se fait toucher par un tir.

	// Dust d'atterrissage.
	if (gShoot.nPlayerGnd && gShoot.nPlayerLastGnd == 0) Gen_LandDust(e_HeroVehicle_Tank, gShoot.nPlayerGnd, gShoot.nPlayerPosX, gShoot.nPlayerPosY);
	// Splash dans l'eau.
	Gen_StepDust(e_HeroVehicle_Tank, gShoot.nPlayerGnd, SLUG_TANK_REF_OFFSETX, 0, gShoot.nPlayerPosX, gShoot.nPlayerPosY);
//	Slug_StepDust(gShoot.nPlayerGnd, SLUG_TANK_REF_OFFSETX, gShoot.nPlayerPosX, gShoot.nPlayerPosY);

	// Le décalage - Sub.
	gShoot.nPlayerPosY -= gShoot.nVehicleOffsY << 8;

}

//=============================================================================

// Tir, appui sur le bouton ?
void Player_ButtonA_Check(void)
{
	u32	nAnmKey;

	// Tir, appui sur le bouton ?
// le test nAmmo doit être virable, on repasse en gun si ammo == 0 (à moins de laisser l'anim de fin de tir ?).
//	if (gVar.pKeys[KB_BUTTONA] && gShoot.nAmmo)
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] && gShoot.nAmmo)
	{
/*
		static u64 *gpHeroShotAnm[e_HeroVehicle_SlugBase][e_Player_Weapon_Max] =	// Anim de tir en fct de l'arme.
		{
			{ gAnm_Hero_Shot_Gun, gAnm_Hero_Shot_Shotgun, gAnm_Hero_Shot_Machinegun, gAnm_Hero_Shot_Flamethrower, gAnm_Hero_Shot_Shotgun },	// Naked.
			{ gAnm_Hero_Shot_Gun, gAnm_Hero_Shot_Gun, gAnm_Hero_Shot_Gun, gAnm_Hero_Shot_Gun, gAnm_Hero_Shot_Gun },							// Swim (un peu inutile, mais c'est par sécurité car on n'a pas toutes les anims).
		};
*/
//+		static u64 *gpHeroShotAnm[e_Player_Weapon_Max] =	// Anim de tir en fct de l'arme.
//+			{ gAnm_Hero_Shot_Gun, gAnm_Hero_Shot_Shotgun, gAnm_Hero_Shot_Machinegun, gAnm_Hero_Shot_Flamethrower, gAnm_Hero_Shot_Shotgun };

		static u64 *gpHeroShotAnmStd[e_Player_Weapon_Max] =	// Anim de tir en fct de l'arme.
			{ gAnm_Hero_Shot_Gun, gAnm_Hero_Shot_Shotgun, gAnm_Hero_Shot_Machinegun, gAnm_Hero_Shot_Flamethrower, gAnm_Hero_Shot_RocketLauncher };
		static u64 *gpHeroShotAnmSub[e_Player_Weapon_Max] =	// Anim de tir en fct de l'arme, spécial sub car pas de gfx de mitrailleuse.
			{ gAnm_Hero_Shot_Gun, gAnm_Hero_Shot_Shotgun, gAnm_Hero_Shot_SubMachinegun, gAnm_Hero_Shot_Flamethrower, gAnm_Hero_Shot_RocketLauncher };

		static u64 **gpHeroShotAnmTb[e_HeroVehicle_SlugBase] =
			{ gpHeroShotAnmStd, gpHeroShotAnmSub, gpHeroShotAnmStd, gpHeroShotAnmStd };

//		static u32 gpHeroShotClearKb[e_Player_Weapon_Max] = { 1, 1, 0, 0, 1 };	// Doit-on clearer la touche de tir, en fct de l'arme.
		//
//sans couteau		gShoot.nPlayerAnmTop = AnmSetIfNew(gpHeroShotAnm[gShoot.nWeapon], gShoot.nPlayerAnmTop);
//sans couteau		if (gpHeroShotClearKb[gShoot.nWeapon]) gVar.pKeys[KB_BUTTONA] = 0;	// Clear touche de tir si nécessaire.

		nAnmKey = (gShoot.nPlayerAnmTop == -1 ? e_AnmKey_Null : AnmGetKey(gShoot.nPlayerAnmTop));
		if (gShoot.nMstProximity && nAnmKey != e_AnmKey_Hero_Shot)
		{
			gShoot.nPlayerAnmTop = AnmSetIfNew(gAnm_Hero_Shot_Knife1, gShoot.nPlayerAnmTop);	// Coup de couteau.
//			gVar.pKeys[KB_BUTTONA] = 0;
			gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] = 0;
		}
		else
		{
//			gShoot.nPlayerAnmTop = AnmSetIfNew(gpHeroShotAnm[gShoot.nVehicleType][gShoot.nWeapon], gShoot.nPlayerAnmTop);	// Tir normal.
//+			gShoot.nPlayerAnmTop = AnmSetIfNew(gpHeroShotAnm[gShoot.nWeapon], gShoot.nPlayerAnmTop);	// Tir normal.
			gShoot.nPlayerAnmTop = AnmSetIfNew( (gpHeroShotAnmTb[gShoot.nVehicleType])[gShoot.nWeapon], gShoot.nPlayerAnmTop);	// Tir normal.
//			if (gpHeroShotClearKb[gShoot.nWeapon]) gVar.pKeys[KB_BUTTONA] = 0;	// Clear touche de tir si nécessaire.
			if (gpHeroShotClearKb[gShoot.nWeapon]) gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] = 0;	// Clear touche de tir si nécessaire.
		}
	}
}

// Avance de l'anim du haut, placement de l'anim de fin de tir si nécessaire.
void Player_TopAnmAdvance(void)
{
	u32	i;

	// Repasse en "gun" si plus de balles, ET fin de l'anim de tir (pas celle de shot end !).
	if (gShoot.nAmmo == 0)
	if ((gShoot.nPlayerAnmTop == -1 ? 1 : (AnmGetKey(gShoot.nPlayerAnmTop) != e_AnmKey_Hero_Shot ? 1 : 0)))
	{
		Player_WeaponSet(e_Player_Weapon_Gun);
	}

	// Avance de l'anim du haut.
	if (gShoot.nPlayerAnmTop != -1)
	{
		i = AnmGetImage(gShoot.nPlayerAnmTop);
		if (i == (u32)-1)
		{
			gShoot.nPlayerAnmTop = -1;		// Anim du haut finie.
		}
		else
		{
			// Cas particulier des tirs qui bouclent, il faut mettre l'anim de fin de tir à la main.
			if (AnmGetKey(gShoot.nPlayerAnmTop) == e_AnmKey_Hero_Shot)
			{
//+				static u32 *gpHeroShotEndAnm[e_Player_Weapon_Max] = { 0, 0, gAnm_Hero_ShotEnd_Machinegun, gAnm_Hero_ShotEnd_Flamethrower, 0 };	// Anim de fin de tir en fct de l'arme.

				static u64 *gpHeroShotEndAnmStd[e_Player_Weapon_Max] = { 0, 0, gAnm_Hero_ShotEnd_Machinegun, gAnm_Hero_ShotEnd_Flamethrower, 0 };	// Anim de fin de tir en fct de l'arme.
				static u64 *gpHeroShotEndAnmSub[e_Player_Weapon_Max] = { 0, 0, gAnm_Hero_ShotEnd_Machinegun, gAnm_Hero_ShotEnd_Flamethrower, 0 };	// Anim de fin de tir en fct de l'arme, spécial sub car pas de gfx de mitrailleuse.

				static u64 **gpHeroShotEndAnmTb[e_HeroVehicle_SlugBase] =
					{ gpHeroShotEndAnmStd, gpHeroShotEndAnmSub, gpHeroShotEndAnmStd, gpHeroShotEndAnmStd };

//+				if (gpHeroShotEndAnm[gShoot.nWeapon])	// Il y a une anim de fin de tir ?
				if ((gpHeroShotEndAnmTb[gShoot.nVehicleType])[gShoot.nWeapon])	// Il y a une anim de fin de tir ?
				if (AnmCheckNewImgFlag(gShoot.nPlayerAnmTop))	// Nouvelle image ?
				{
					// On change d'image, on a toujours le doigt sur le tir et il reste des balles ?
//					if (gVar.pKeys[KB_BUTTONA] == 0 || gShoot.nAmmo == 0)
					if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] == 0 || gShoot.nAmmo == 0)
					{
						// Non, mettre l'anim de fin de tir.
//+						gShoot.nPlayerAnmTop = AnmSetIfNew(gpHeroShotEndAnm[gShoot.nWeapon], gShoot.nPlayerAnmTop);
						gShoot.nPlayerAnmTop = AnmSetIfNew((gpHeroShotEndAnmTb[gShoot.nVehicleType])[gShoot.nWeapon], gShoot.nPlayerAnmTop);
					}
				}

			} // AnmKey == e_AnmKey_Hero_Shot
		} // if (i == (u32)-1)
	}

}

// Déclenchement d'un tir ?
void Player_ShotCheck(u32 nSpr, u32 nUp)
{
	// Frame de hit (couteau).
	if (AnmGetKey(gShoot.nPlayerAnmTop) == e_AnmKey_Hero_Knife) gShoot.nPlayerKnifeSprCol = nSpr | (gShoot.nPlayerDir ? SPR_Flip_X : 0);

	// Déclenchement d'un tir ?
	if (AnmGetKey(gShoot.nPlayerAnmTop) == e_AnmKey_Hero_Shot)	// Est-on dans une anim de tir ?
	if (AnmCheckNewImgFlag(gShoot.nPlayerAnmTop))	// Nouvelle image ?
	{
		// Est-ce qu'on a une coordonnée d'origine de tir ?
		struct SSprRect sRect1;
		if (SprGetRect(nSpr | (gShoot.nPlayerDir ? SPR_Flip_X : 0), e_SprRectZone_ShotOrg, &sRect1))
		if (sRect1.nType == e_SprRect_Point)
		{
			// Oui, tir.
			static	u32	gpPlyrShotH[e_Player_Weapon_Max] = { e_Shot_Player_PistolH, e_Shot_Player_ShotgunH, e_Shot_Player_MachinegunH, e_Shot_Player_FlameH, e_Shot_Player_RocketSmall_Launch };
			static	u32	gpPlyrShotV[e_Player_Weapon_Max] = { e_Shot_Player_PistolV, e_Shot_Player_ShotgunV, e_Shot_Player_MachinegunV, e_Shot_Player_FlameV, e_Shot_Player_RocketSmall_Launch };
			s32	nAddAng = 0;

			// Ajout petit random sur les tirs de la mitrailleuse et du lance flames.
			if (gShoot.nWeapon == e_Player_Weapon_Machinegun)
			{
				static	s8	gpShotAngRand[] = { 0, 1, 0, -1 };
				nAddAng = gpShotAngRand[rand() & 3];
			}
			else if (gShoot.nWeapon == e_Player_Weapon_Flamethrower)
			{
				nAddAng = (rand() & 15) - 7;
			}

			if (nUp)
			FireAdd(gpPlyrShotV[gShoot.nWeapon], gShoot.nPlayerPosX + (sRect1.nX1 << 8), gShoot.nPlayerPosY + (sRect1.nY1 << 8), (nUp == 1 ? 64 : 192) + nAddAng);
			else
			FireAdd(gpPlyrShotH[gShoot.nWeapon], gShoot.nPlayerPosX + (sRect1.nX1 << 8), gShoot.nPlayerPosY + (sRect1.nY1 << 8), (gShoot.nPlayerDir ? 128 : 0) + nAddAng);

			// Décrémentation du nombre de balles, sauf si négatif (pistolet => munitions infinies).
			if (gShoot.nAmmo > 0) gShoot.nAmmo--;
		}
	}

}


// Le joueur se prend des dégats.
// On peut appeler la routine directement pour un hit avec un coup de poing par exemple.
void Player_Damage(u32 nDamage)
{
	u32	nDamagePts;

	if (gShoot.nInvulnerability) return;
	if (gShoot.nDeathFlag) return;

	nDamagePts = nDamage & 0xFFFF;
	if (nDamagePts == 0) return;
	nDamage >>= 16;		// = Damage Type.

	if (gShoot.nVehicleType < e_HeroVehicle_SlugBase)
	{	// Joueur pas dans un véhicule. Metal Slug => 1 touchette = mort.
		Game_Player_DeathSet();
	}
	else
	{	// Joueur dans un véhicule.
		gShoot.nVehicleHitCnt = MST_HIT_CNT;		// Compteur de hit pour palette alt.
		if (gShoot.nVehicleEnergy > nDamagePts)
			gShoot.nVehicleEnergy -= nDamagePts;
		else
			gShoot.nVehicleEnergy = 0;
	}


//	if (nEnergyTemp > SLUG_ENERGY_MAX) nEnergyTemp = SLUG_ENERGY_MAX;
//	gShoot.nVehicleEnergy = nEnergyTemp;

/* pour les tests des types de damage :
		// Giclée de sang quand il se prend une balle.
		if (nDamage >> 16 == e_ShotDamageType_Gen)
		// Souffle.
		else if (nDamage >> 16 == e_ShotDamageType_Blow)
		// Brule.
		else
*/

}

// Teste si on se fait toucher par un tir ennemi.
void Player_ShotHitCheck(void)
{
	s32	nOffsX, nOffsY;

	// Offsets du rect de col pour le tank en rotozoom.
	if (gShoot.nVehicleType == e_HeroVehicle_Tank)
	{
//		nOffsX = (SLUG_TANK_REF_OFFSETX >> 8) * gVar.pSin[gShoot.nVehicleCurAdd];
		nOffsX = (SLUG_TANK_REF_OFFSETX >> 8) * gVar.pSin[gShoot.nVehicleCurAdd & 0xFF];	// Fix 11/10/2012. Normalement AND fait ligne 2115. Sinon problème négatif en u32 et ptr en 64 bits...
		nOffsY = ABS(nOffsX);
	}
	else
	{
		nOffsX = 0;
		nOffsY = 0;
	}
	// FireHitCheck.
	u32	nDamage;
	nDamage = FireHitCheck(gShoot.nPlayerSprCol, gShoot.nPlayerPosX + nOffsX, gShoot.nPlayerPosY + nOffsY, e_ShotOrg_Enemy, 1);
	if (nDamage & 0xFFFF) Player_Damage(nDamage);

}

// Teste une collision entre 2 rectangles.
// Out : 1 = Col / 0 = Pas col.
u32 RectColCheck(struct SSprRect *pRect1, s32 nPosX1, s32 nPosY1, struct SSprRect *pRect2, s32 nPosX2, s32 nPosY2)
{
	s32	nXMin1, nXMax1, nYMin1, nYMax1;
	s32	nXMin2, nXMax2, nYMin2, nYMax2;

	// Rect 1.
	nXMin1 = nPosX1 + (pRect1->nX1 << 8);
	nXMax1 = nPosX1 + (pRect1->nX2 << 8);
	nYMin1 = nPosY1 + (pRect1->nY1 << 8);
	nYMax1 = nPosY1 + (pRect1->nY2 << 8);
	// Rect 2.
	nXMin2 = nPosX2 + (pRect2->nX1 << 8);
	nXMax2 = nPosX2 + (pRect2->nX2 << 8);
	nYMin2 = nPosY2 + (pRect2->nY1 << 8);
	nYMax2 = nPosY2 + (pRect2->nY2 << 8);

/*
//>> tst / affichage du sprite de col
{
	SprDisplay(e_Spr_Tstrct_CornerUL, (nXMin1 >> 8), (nYMin1 >> 8), e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerUR, (nXMax1 >> 8), (nYMin1 >> 8), e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerDL, (nXMin1 >> 8), (nYMax1 >> 8), e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerDR, (nXMax1 >> 8), (nYMax1 >> 8), e_Prio_Joueur + 3);

	SprDisplay(e_Spr_Tstrct_CornerUL, (nXMin2 >> 8), (nYMin2 >> 8), e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerUR, (nXMax2 >> 8), (nYMin2 >> 8), e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerDL, (nXMin2 >> 8), (nYMax2 >> 8), e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerDR, (nXMax2 >> 8), (nYMax2 >> 8), e_Prio_Joueur + 3);
}
//<< tst / affichage du sprite de col
*/

	// Collisions entre les rectangles ?
	if (nXMax1 >= nXMin2 && nXMin1 <= nXMax2 && nYMax1 >= nYMin2 && nYMin1 <= nYMax2) return (1);
	return (0);

}

// Test d'une frappe sur les canons latéraux de la fusée. (Sub-routine de Enemy_PlayerBlowRect()).
// Out: 0 = Pas de col / 1 = Il y a eu col.
u32 Enemy_sub_RocketBlowRect(struct SSprRect *pRect1, s32 nPosX1, s32 nPosY1)
{
	u32	i;
	struct SSprRect sRect2;

	for (i = 0; i < 2; i++)
	{
		if (gShoot.pnRocketSideWeaponsAnmLR[i] != -1)
		if (SprGetRect(AnmGetLastImage(gShoot.pnRocketSideWeaponsAnmLR[i]), e_SprRectZone_RectCol, &sRect2))
		if (sRect2.nType == e_SprRect_Rect)
		if (RectColCheck(pRect1, nPosX1, nPosY1, &sRect2, gShoot.nPlayerPosX, gShoot.nPlayerPosY - (20 << 8)))
		{
			// Explosion du canon.
			DustSet(gAnm_Explosion0_Medium_Dust, gShoot.nPlayerPosX + ((i & 1 ? 32 : -32) << 8), gShoot.nPlayerPosY - (20 << 8), e_Prio_Joueur + 3, 0);	//e_Prio_DustOver
			// Libère le slot d'anim.
			AnmReleaseSlot(gShoot.pnRocketSideWeaponsAnmLR[i]);
			gShoot.pnRocketSideWeaponsAnmLR[i] = -1;
			return (1);
		}
	}
	return (0);

}

// Un ennemi frappe le joueur.
// Out: 0 = Pas de col / 1 = Il y a eu col.
u32 Enemy_PlayerBlowRect(struct SSprRect *pRect1, s32 nPosX, s32 nPosY, u32 nDamage)
{
	s32	nXMin1, nXMax1, nYMin1, nYMax1;
	s32	nXMin2, nXMax2, nYMin2, nYMax2;
	struct SSprRect sRect2;

	if (gShoot.nPlayerSprCol == SPR_NoSprite) return (0);

	// Cas spécial pour la fusée, test des canons latéraux.
	if (gShoot.nVehicleType == e_HeroVehicle_Rocket)
	if (Enemy_sub_RocketBlowRect(pRect1, nPosX, nPosY))
	{
		if (gShoot.pnRocketSideWeaponsAnmLR[0] == -1 &&
			gShoot.pnRocketSideWeaponsAnmLR[1] == -1 &&
			gShoot.nRocketWeapon != e_Player_Weapon_Gun)
			Rocket_WeaponSet(e_Player_Weapon_Gun);			// Si plus de canons, on repasse en arme de base.
		return (1);
	}

	// On récupère la zone de col du joueur.
	if (SprGetRect(gShoot.nPlayerSprCol, e_SprRectZone_RectCol, &sRect2) == 0) return (0);
	if (sRect2.nType != e_SprRect_Rect) return (0);
	// Ok.
	nXMin2 = gShoot.nPlayerPosX + (sRect2.nX1 << 8);
	nXMax2 = gShoot.nPlayerPosX + (sRect2.nX2 << 8);
	nYMin2 = gShoot.nPlayerPosY + (sRect2.nY1 << 8);
	nYMax2 = gShoot.nPlayerPosY + (sRect2.nY2 << 8);
	// Rectangle de col de l'ennemi.
	nXMin1 = nPosX + (pRect1->nX1 << 8);
	nXMax1 = nPosX + (pRect1->nX2 << 8);
	nYMin1 = nPosY + (pRect1->nY1 << 8);
	nYMax1 = nPosY + (pRect1->nY2 << 8);

/*
//>> tst / affichage du sprite de col
{
	SprDisplay(e_Spr_Tstrct_CornerUL, (nXMin1 >> 8), (nYMin1 >> 8), e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerUR, (nXMax1 >> 8), (nYMin1 >> 8), e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerDL, (nXMin1 >> 8), (nYMax1 >> 8), e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerDR, (nXMax1 >> 8), (nYMax1 >> 8), e_Prio_Joueur + 3);

	SprDisplay(e_Spr_Tstrct_CornerUL, (nXMin2 >> 8), (nYMin2 >> 8), e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerUR, (nXMax2 >> 8), (nYMin2 >> 8), e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerDL, (nXMin2 >> 8), (nYMax2 >> 8), e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerDR, (nXMax2 >> 8), (nYMax2 >> 8), e_Prio_Joueur + 3);
}
//<< tst / affichage du sprite de col
*/

	// Collisions entre les rectangles ?
	if (nXMax1 >= nXMin2 && nXMin1 <= nXMax2 && nYMax1 >= nYMin2 && nYMin1 <= nYMax2)
	{
		Player_Damage(nDamage);
		return (1);
	}
	return (0);

}

//=============================================================================
// Gestion du héros en train de nager.

// Test du plafond.
// Out : Si on touche, le code du bloc sur lequel on touche.
u32 Gen_CeilingCheck(u32 nPosX, s32 nHeroHt)
{
	s32	*pBlocks;
	u32	nBlockNo;
	u32	nHt, nHt2;
	u32	nCol;

	if (gShoot.nPlayerSpdY > 0) return (0);

	gShoot.nPlayerPosY -= nHeroHt;	// *** Ajoute le décalage.

//SprDisplay(e_Spr_Tstrct_Cross, nPosX>>8, gShoot.nPlayerPosY>>8, 250);
	pBlocks = gMap.ppPlanesBlocks[gMap.nHeroPlane];	// Blocs.

	nBlockNo = *(pBlocks + ((gShoot.nPlayerPosY >> 12) * gMap.nMapLg) + (nPosX >> 12) );
	nHt = BlockCeilingGetHeight(nBlockNo, (nPosX >> 8));
	if (nHt)
	{
		nHt--;
		if (((gShoot.nPlayerPosY >> 8) & 0x0F) <= nHt)	// Est-on dans le dur ?
		{
			nHt2 = nHt;
			nCol = (gMap.ppColCodes[gMap.nHeroPlane] + nBlockNo)->nCol;

			// Si on pose sur la ligne inférieure, on regarde aussi le bloc du dessous.
			if (nHt == 15)
			{
				nBlockNo = *(pBlocks + (((gShoot.nPlayerPosY >> 12) + 1) * gMap.nMapLg) + (nPosX >> 12) );
				nHt = BlockCeilingGetHeight(nBlockNo, (nPosX >> 8));
				if (nHt)
				{
					nHt--;
					nHt2 = 16 + nHt;
					nCol = (gMap.ppColCodes[gMap.nHeroPlane] + nBlockNo)->nCol;
				}
			}

			// Plafond touché.
			gShoot.nPlayerSpdY = 0;
			gShoot.nPlayerPosY &= ~0xFFF;	// FFF => On cleane la virgule avec.
			gShoot.nPlayerPosY += nHt2 << 8;

//SprDisplay(e_Spr_Tstrct_Cross, nPosX>>8, gShoot.nPlayerPosY>>8, 250);

			gShoot.nPlayerPosY += nHeroHt;	// *** Retire le décalage.
			return (nCol);
		}
	}

	gShoot.nPlayerPosY += nHeroHt;	// *** Retire le décalage.
	return (0);
}

// Test sur les côtés au niveau du plafond (nage + slug sous-marin).
void Gen_CeilingSideCheck(s32 nTestOffsX, s32 nHeroHt, s32 nSpdMaxX)
{
	s32	*pBlocks;
	u32	nBlockNo;
	u32	nHt;
	s32 nPosX, nPosY;

	nPosX = gShoot.nPlayerPosX + gShoot.nPlayerSpdX + (gShoot.nPlayerSpdX >= 0 ? nTestOffsX : -nTestOffsX);
	pBlocks = gMap.ppPlanesBlocks[gMap.nHeroPlane];	// Blocs.

	// Au niveau du plafond.
	nPosY = gShoot.nPlayerPosY - nHeroHt + (nSpdMaxX * 2);
	nBlockNo = *(pBlocks + ((nPosY >> 12) * gMap.nMapLg) + (nPosX >> 12) );
	if (((gMap.ppColCodes[gMap.nHeroPlane] + nBlockNo)->nCol & 0x0F) == e_BlockCode_Ceiling)	// Sur les blocs de plafond.
	{
		nHt = BlockCeilingGetHeight(nBlockNo, (nPosX >> 8));
		if (nHt)
		{
			nHt--;
			if (((nPosY >> 8) & 0x0F) <= nHt)
			{
				gShoot.nPlayerSpdX = 0;
				return;
			}
		}
	}

}

void PlayerControl_Swim(void)
{
	u32	nAnmNo;
	u32	i;

	nAnmNo = e_Anm_HeroSub_Stance;

	// Gestion de la mort.
	Player_DeathManage();
	// Gestion de la victoire.
	Player_VictoryManage();

	// Slug out : Couper le contrôle.
	if (AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_SlugOut) Gen_KbNoControl();


	// *** Déplacement du perso.
	// Vitesse.
//	i = PlyrMove_Spd(&gShoot.nPlayerSpdX, PLYR_SWIM_SPDX_MAX, PLYR_SWIM_ACC, SDLK_LEFT, SDLK_RIGHT);
//	i += PlyrMove_Spd(&gShoot.nPlayerSpdY, PLYR_SWIM_SPDY_MAX, PLYR_SWIM_ACC, SDLK_UP, SDLK_DOWN);
	i = PlyrMove_Spd(&gShoot.nPlayerSpdX, PLYR_SWIM_SPDX_MAX, PLYR_SWIM_ACC, gMSCfg.pKeys[e_CfgKey_Left], gMSCfg.pKeys[e_CfgKey_Right]);
	i += PlyrMove_Spd(&gShoot.nPlayerSpdY, PLYR_SWIM_SPDY_MAX, PLYR_SWIM_ACC, gMSCfg.pKeys[e_CfgKey_Up], gMSCfg.pKeys[e_CfgKey_Down]);
	if (i) nAnmNo = e_Anm_HeroSub_Run;
	if (gShoot.nPlayerSpdX) gShoot.nPlayerDir = (gShoot.nPlayerSpdX >= 0 ? 0 : 1);
	// Bords de map ou scroll bloqué.
	PlayerMove_LimitX(PLYR_SWIM_REF_OFFSETX);
	PlayerMove_LimitY(PLYR_SWIM_HEIGHT);
	PlayerMove_LimitY_Bottom(PLYR_SWIM_HEIGHT_B);
	// Sol dte/gche.
	Gen_SideCheck(PLYR_SWIM_REF_OFFSETX, PLYR_SWIM_SPDX_MAX);	// Mettra la vitesse à 0 si mouvement pas possible.
	// Plafond dte/gche.
	Gen_CeilingSideCheck(PLYR_SWIM_REF_OFFSETX, (PLYR_SWIM_HEIGHT - 4) << 8, PLYR_SWIM_SPDX_MAX);
	// Add.
	gShoot.nPlayerPosX += gShoot.nPlayerSpdX;
	gShoot.nPlayerPosY += gShoot.nPlayerSpdY;

	// Inclinaison.
	gShoot.nVehicleReqAdd = PLYR_SWIM_DEFAULTSPRADD;
//	if (gVar.pKeys[SDLK_UP]) gShoot.nVehicleReqAdd = 0;
//	if (gVar.pKeys[SDLK_DOWN]) gShoot.nVehicleReqAdd = 4;
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Up]]) gShoot.nVehicleReqAdd = 0;
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]]) gShoot.nVehicleReqAdd = 4;
	if (gShoot.nVehicleCurAdd != gShoot.nVehicleReqAdd)
	{
		if ((++gShoot.nVehicleFrmCnt0 & 7) == 0)
			gShoot.nVehicleCurAdd += (gShoot.nVehicleCurAdd < gShoot.nVehicleReqAdd ? 1 : -1);
	}

	// *** Test du sol.
	gShoot.nPlayerLastGnd = gShoot.nPlayerGnd = 0;		// RAZ gnd.
	Gen_GroundCheck(gShoot.nPlayerPosX + PLYR_SWIM_REF_OFFSETX);
	Gen_GroundCheck(gShoot.nPlayerPosX - PLYR_SWIM_REF_OFFSETX);
	// Test du plafond.
	Gen_CeilingCheck(gShoot.nPlayerPosX + PLYR_SWIM_REF_OFFSETX, (PLYR_SWIM_HEIGHT - 4) << 8);
	Gen_CeilingCheck(gShoot.nPlayerPosX - PLYR_SWIM_REF_OFFSETX, (PLYR_SWIM_HEIGHT - 4) << 8);

	// Anim.
	AnmSetIfNew(gpHeroAnm[nAnmNo], gShoot.nPlayerAnm);

}

void PlayerDisplay_Swim(void)
{
	u32	nLegs, nTop;
	u32	i;

	//>> Cheat : Changement d'arme.
	if (gCCodes.nCheat & e_Cheat_WeaponSelect)
	if (gVar.pKeys[SDLK_F5])
	{
		if (++gShoot.nWeapon >= e_Player_Weapon_Max) gShoot.nWeapon = 0;
		Player_WeaponSet(gShoot.nWeapon);
		gVar.pKeys[SDLK_F5] = 0;
	}
	//<< Cheat : Changement d'arme.

	// Tir, appui sur le bouton ?
	Player_ButtonA_Check();

	// Bombe ?
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonC]] && gShoot.nBombAmmo)
	{
		// Inclinaison.
		s32	nShotAng = (s32)gShoot.nVehicleCurAdd - PLYR_SWIM_DEFAULTSPRADD;
		nShotAng <<= 3;
		// Tir.
		FireAdd(e_Shot_Player_SwimGrenade_Throw, gShoot.nPlayerPosX + ((gShoot.nPlayerDir ? -16 : 16) << 8), gShoot.nPlayerPosY - (6 << 8) + (nShotAng << 7), (gShoot.nPlayerDir ? 128 + nShotAng : 0 - nShotAng));
		Sfx_PlaySfx(e_Sfx_Shot_MiniRocket, e_SfxPrio_Hero_Shot);	// !!changer!!
		gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonC]] = 0;
		gShoot.nBombAmmo--;
	}


	// *** Tirs, à cause des tirs qui bouclent, on est obligé de faire un test d'image ici, AVANT l'affichage réel.
	Player_TopAnmAdvance();
/*
	// Avance de l'anim du haut.
	if (gShoot.nPlayerAnmTop != -1)
	{
		i = AnmGetImage(gShoot.nPlayerAnmTop);
		if (i == (u32)-1)
		{
			gShoot.nPlayerAnmTop = -1;		// Anim du haut finie.
		}
	}
*/


	// Affichage du héros.

	// *** 1) Récupère les sprites bottom et top basiques.
	Player_GetTopBottom(&nLegs, &nTop);
	// Inclinaison (sauf anim de mort ou slug out).
	if (AnmGetKey(gShoot.nPlayerAnm) != e_AnmKey_Hero_Death &&
		AnmGetKey(gShoot.nPlayerAnm) != e_AnmKey_Hero_SlugOut)
	{
		i = gShoot.nVehicleCurAdd * 10;	// Anim de repos/avance : 10 sprites x 5 angles.
		nLegs += i;
		nTop += i;

		// Les paillettes : Des p'tites bulles...
//		static	u32	nBubbles = 32;
//		if (((++nBubbles) & 63) == 0)
		if ((gnFrame & 63) == 0)
			DustSet(gAnm_SubmarineBubble_Dust,
				gShoot.nPlayerPosX + ((gShoot.nPlayerDir ? -8 : 8) << 8), gShoot.nPlayerPosY - (12 << 8), e_Prio_DustUnder, 0);
	}

	// *** 2) Ensuite, y-a-t'il une anim spéciale en haut ? Si oui, on met le sprite qui va bien. Si nécessaire, écrase celui du bas (accroupi...).
	// Anim de tir ?
	gShoot.nPlayerKnifeSprCol = SPR_NoSprite;		// RAZ frame de hit (couteau).
	if (gShoot.nPlayerAnmTop != -1)
	{
		static u32 gpHeroTopShotImg[][e_Player_Weapon_Max] =
		{
			{ e_Spr_HeroSubTop_Shoot_Gun, e_Spr_HeroSubTop_Shoot_Shotgun, e_Spr_HeroSubTop_Shoot_Shotgun, e_Spr_HeroSubTop_Shoot_Shotgun, e_Spr_HeroSubTop_Shoot_Shotgun },
			{ e_Spr_HeroSubTop_Knife1_Gun, e_Spr_HeroSubTop_Knife1_Shotgun, e_Spr_HeroSubTop_Knife1_Shotgun, e_Spr_HeroSubTop_Knife1_Shotgun, e_Spr_HeroSubTop_Knife1_Shotgun },
		};
		static u32 gpHeroTopShotMultiplier[][e_Player_Weapon_Max] = {
			{ 7, 14, 14, 14, 14 },
			{ 6, 6, 6, 6, 6 },
		};
		u32	nTbIdx;

		// Index dans les anims pour tir classique, lancer de grenade ou coup de couteau.
		switch (AnmGetKey(gShoot.nPlayerAnmTop))
		{
		case e_AnmKey_Hero_Knife:
		case e_AnmKey_Hero_KnifeEnd:
			nTbIdx = 1;
			break;
		default:
			nTbIdx = 0;
			break;
		}

		i = AnmGetLastImage(gShoot.nPlayerAnmTop);
		nTop = i + gpHeroTopShotImg[nTbIdx][gShoot.nWeapon];
		nTop += gShoot.nVehicleCurAdd * gpHeroTopShotMultiplier[nTbIdx][gShoot.nWeapon];	// Inclinaison.
		i = nTop;	// En cas de tir, on note le sprite.

		// Déclenchement d'un tir ?
		Player_ShotCheck(i, 0);

	} //if (gShoot.nPlayerAnmTop != -1)

	if ((gShoot.nInvulnerability & PLYR_INVL_BLINK) == 0)	// Clignotement quand invulnérable.
	{
		// Affichage haut et bas.
		SprDisplay(nLegs ^ (gShoot.nPlayerDir ? SPR_Flip_X : 0), (gShoot.nPlayerPosX >> 8), (gShoot.nPlayerPosY >> 8), e_Prio_Joueur);
		if (nTop)
		SprDisplay(nTop ^ (gShoot.nPlayerDir ? SPR_Flip_X : 0), (gShoot.nPlayerPosX >> 8), (gShoot.nPlayerPosY >> 8), e_Prio_Joueur + 1);
	}

	// Sprite de col du joueur (parce que je ne veux pas avoir d'erreur de rectangles).
	i = e_Spr_HeroSubLegs_Run + (gShoot.nVehicleCurAdd * 10);
	gShoot.nPlayerSprCol = i | (gShoot.nPlayerDir ? SPR_Flip_X : 0);
	// Mort => Plus de sprite de collision.
	if (gShoot.nDeathFlag) gShoot.nPlayerSprCol = SPR_NoSprite;

	Player_ShotHitCheck();		// Teste si on se fait toucher par un tir ennemi.

}

//=============================================================================
// Gestion "tampon" du héros quand mort dans un slug duquel on ne doit pas descendre.

void PlayerControl_SlugNoExit_Temp(void)
{
	// Gestion de la mort.
	Player_DeathManage();
	// Gestion de la victoire.
//	Player_VictoryManage();

	// Si pas (plus) en mort, on repasse dans le véhicule qui va bien.
//	if (AnmGetKey(gShoot.nPlayerAnm) != e_AnmKey_Hero_Death)
	if (gShoot.nDeathFlag == 0)
	{
		s32	nOffsX, nOffsY;
//		MstSlug_EntrancePtGet(e_HeroVehicle_Rocket - e_HeroVehicle_SlugBase, &nOffsX, &nOffsY);		// Pour compenser le décalage de la sortie.
//		Player_VehicleSet(e_HeroVehicle_Rocket, gShoot.nPlayerPosX - (nOffsX << 8), gShoot.nPlayerPosY - (nOffsY << 8), SLUG_ENERGY_MAX, 0, SLUG_BOMBS_DEFAULT);
		MstSlug_EntrancePtGet(gShoot.nVehicleNoExitTypeToReset - e_HeroVehicle_SlugBase, &nOffsX, &nOffsY);		// Pour compenser le décalage de la sortie.
		Player_VehicleSet(gShoot.nVehicleNoExitTypeToReset, gShoot.nPlayerPosX - (nOffsX << 8), gShoot.nPlayerPosY - (nOffsY << 8), SLUG_ENERGY_MAX, 0, SLUG_BOMBS_DEFAULT);

		gShoot.nInvulnerability = PLYR_INVULNERABILTY_CNT;	// Petit temps d'invulnérabilité. (Cas spécial espace + hélico no exit).
		gShoot.nVehiclePhase = e_SlugPhase_Normal;			// On force la phase "normale" pour ne pas voir le 'get in'.
		Player_Control();	// La routine de contrôle du nouveau véhicule.
	}

}

void PlayerDisplay_SlugNoExit_Temp(void)
{
	u32	nSpr;
	nSpr = AnmGetImage(gShoot.nPlayerAnm);		// Très important, pour faire avancer l'anim de mort et arriver sur le AnmEnd !

	// Pas de sprite de collision.
	gShoot.nPlayerSprCol = SPR_NoSprite;

}

//=============================================================================
// Gestion du héros en parachute.

// Contrôle du joueur.
void PlayerControl_Parachute(void)
{
	u32	nAnmNo;
	s32	nPosX_sav;

	// Gestion de la victoire.
	Player_VictoryManage();

	// Oscillation X : Ajout et retrait de l'offset pour ne pas perturber le scroll.
	nPosX_sav = gShoot.nPlayerPosX;	// Sauvegarde de la pos X.
	gShoot.nPlayerPosX += gVar.pSin[gShoot.nVehicleFrmCnt0] << 2;
	gShoot.nVehicleFrmCnt0 += 3;	// Incrémentation de l'index pour oscillation.

	nAnmNo = e_Anm_Hero_Fall;

	// Jump ? => On lâche le parachute.
	u32 nHt;
	nHt = BlockGetGroundLevel(gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8);
//	if (gVar.pKeys[KB_BUTTONB] && nHt < 64)
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonB]] && nHt < 64)
	{
		Player_VehicleSet(e_HeroVehicle_None_Naked, gShoot.nPlayerPosX, gShoot.nPlayerPosY, SLUG_ENERGY_MAX, 0, 0);
		gShoot.nParachuteAnm = AnmSetIfNew(gAnm_Parachute_Fold, gShoot.nParachuteAnm);	// Repli du parachute.
		gShoot.nPlayerGnd = 0;
		gShoot.nPlayerSpdY = PLYR_PARACHUTE_SPDY_MAX;
//		gVar.pKeys[KB_BUTTONB] = 0;
		gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonB]] = 0;
		gpFctCtrlTb[gShoot.nVehicleType]();	// La routine de contrôle du nouveau véhicule.
		return;
	}

	// *** Test du sol.
	u32	nTouch;
	nTouch = Gen_GroundManage(PLYR_NAKED_SPDX_MAX, PLYR_PARACHUTE_SPDY_MAX, PLYR_NAKED_REF_OFFSETX, 1);
	if (nTouch)
	{
		// Atterrissage.
		Player_VehicleSet(e_HeroVehicle_None_Naked, gShoot.nPlayerPosX, gShoot.nPlayerPosY, SLUG_ENERGY_MAX, 0, 0);
		gShoot.nParachuteAnm = AnmSetIfNew(gAnm_Parachute_Fold, gShoot.nParachuteAnm);	// Repli du parachute.
		gpFctCtrlTb[gShoot.nVehicleType]();	// La routine de contrôle du nouveau véhicule.
		return;
	}

	// Anim.
	AnmSetIfNew(gpHeroAnm[nAnmNo], gShoot.nPlayerAnm);

	gShoot.nPlayerPosX = nPosX_sav;	// Restore pos X.

}

// Affichage "naked", avec ajout/retrait de l'oscillation.
void PlayerDisplay_Parachute(void)
{
	s32	nPosX_sav;

	nPosX_sav = gShoot.nPlayerPosX;	// Sauvegarde de la pos X.
	gShoot.nPlayerPosX += gVar.pSin[gShoot.nVehicleFrmCnt0] << 2;

	PlayerDisplay_Naked();		// Affichage "naked".

	gShoot.nPlayerPosX = nPosX_sav;	// Restore pos X.

}

//=============================================================================
// Gestion du héros à pied.

// Teste le déclenchement d'un saut.
void Gen_ButtonB_Check(s32 nJumpSpdY)
{
	// Jump.
//	if (gShoot.nPlayerGnd && gVar.pKeys[KB_BUTTONB])
	if (gShoot.nPlayerGnd && gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonB]])
	{
		gShoot.nPlayerGnd = 0;
		gShoot.nPlayerSpdY = -nJumpSpdY;
		gShoot.nPfAddress = 0;	// Si on était sur une plateforme mobile, RAZ (sinon la plateforme recale !).
//		gVar.pKeys[KB_BUTTONB] = 0;
		gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonB]] = 0;
	}

}

// Replace les touches qui servent dans l'état du buffer SDL.
void Gen_KbRestore(void)
{
/*
	gVar.pKeys[SDLK_LEFT] = gVar.pKeysSDL[SDLK_LEFT];
	gVar.pKeys[SDLK_RIGHT] = gVar.pKeysSDL[SDLK_RIGHT];
	gVar.pKeys[SDLK_UP] = gVar.pKeysSDL[SDLK_UP];
	gVar.pKeys[SDLK_DOWN] = gVar.pKeysSDL[SDLK_DOWN];
//... mais pas les boutons.
//	gVar.pKeys[KB_BUTTONA] = gVar.pKeysSDL[KB_BUTTONA];
//	gVar.pKeys[KB_BUTTONB] = gVar.pKeysSDL[KB_BUTTONB];
//	gVar.pKeys[KB_BUTTONC] = gVar.pKeysSDL[KB_BUTTONC];
*/
	gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Left]] = gVar.pKeysSDL[gMSCfg.pKeys[e_CfgKey_Left]];
	gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Right]] = gVar.pKeysSDL[gMSCfg.pKeys[e_CfgKey_Right]];
	gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Up]] = gVar.pKeysSDL[gMSCfg.pKeys[e_CfgKey_Up]];
	gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]] = gVar.pKeysSDL[gMSCfg.pKeys[e_CfgKey_Down]];
//... mais pas les boutons.
//	gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] = gVar.pKeysSDL[gMSCfg.pKeys[e_CfgKey_ButtonA]];
//	gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonB]] = gVar.pKeysSDL[gMSCfg.pKeys[e_CfgKey_ButtonB]];
//	gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonC]] = gVar.pKeysSDL[gMSCfg.pKeys[e_CfgKey_ButtonC]];
}

// Plus de contrôle, place toutes les touches de contrôle du joueur à 0.
void Gen_KbNoControl(void)
{
//	gVar.pKeys[SDLK_LEFT] = gVar.pKeys[SDLK_RIGHT] = gVar.pKeys[SDLK_UP] = gVar.pKeys[SDLK_DOWN] =
//		gVar.pKeys[KB_BUTTONA] = gVar.pKeys[KB_BUTTONB] = gVar.pKeys[KB_BUTTONC] = 0;
	gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Left]] = gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Right]] = gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Up]] = gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]] =
		gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] = gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonB]] = gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonC]] = 0;
}

// Test du sol.
// Out : Si on pose, le code du bloc sur lequel on pose.
u32 Gen_GroundCheck(u32 nPosX)
{
	s32	*pBlocks;
	u32	nBlockNo;
	u32	nHt, nHt2, nCmpY;
	u32	nCol;

	if (gShoot.nPlayerSpdY < 0) return (0);

	pBlocks = gMap.ppPlanesBlocks[gMap.nHeroPlane];	// Blocs.

	nBlockNo = *(pBlocks + ((gShoot.nPlayerPosY >> 12) * gMap.nMapLg) + (nPosX >> 12) );
	nHt = BlockGetHeight(nBlockNo, (nPosX >> 8));
	if (nHt)
	{
		nHt = 16 - nHt;
		if (((gShoot.nPlayerPosY >> 8) & 0x0F) >= nHt)	// Est-on dans le dur ?
		{
			nHt2 = nHt;
			nCol = (gMap.ppColCodes[gMap.nHeroPlane] + nBlockNo)->nCol;

			// Si on pose sur la ligne supérieure, on regarde aussi le bloc du dessus.
			if (nHt == 0)
			{
				nBlockNo = *(pBlocks + (((gShoot.nPlayerPosY >> 12) - 1) * gMap.nMapLg) + (nPosX >> 12) );
				nHt = BlockGetHeight(nBlockNo, (nPosX >> 8));
				if (nHt)
				{
					nHt = 16 - nHt;
					nHt2 = -16 + nHt;
					nCol = (gMap.ppColCodes[gMap.nHeroPlane] + nBlockNo)->nCol;
				}
			}

			// On doit monter sur une plateforme transparente ? (Pentes déconseillées, recalage merdique avec le double point).
			if (gShoot.nPlayerLastGnd == 0 && (nCol & 0x0F) == e_BlockCode_Soft)
			{
				nCmpY = (gShoot.nPlayerPosY & ~0xF00) + (nHt2 << 8);
				if (gShoot.nPlayerPosY - nCmpY > PLYR_NAKED_SPDY_MAX) return (0);	// On est trop bas, on ne monte pas.
			}

			// Sol touché.
			gShoot.nPlayerGnd = nCol;//1;
			gShoot.nPlayerSpdY = 0;
			gShoot.nPlayerPosY &= ~0xFFF;//~0xF00;	// FFF => On cleane la virgule avec.
			gShoot.nPlayerPosY += (nHt2) << 8;
			gShoot.nPlayerLastGndPosY = gShoot.nPlayerPosY;

//>
			// Hard sprite ?
			nHt = HardSpr_TestHt(nPosX >> 8, gShoot.nPlayerPosY >> 8, 0);
			if ((s32)nHt < 0)
			{
				nCol = e_BlockCode_Hard;
				gShoot.nPlayerGnd = nCol;
				gShoot.nPlayerPosY += nHt << 8;
				gShoot.nPlayerLastGndPosY = gShoot.nPlayerPosY;
			}
//<

			return (nCol);
		}
	}

//>
	// Hard sprite ?
	nHt = HardSpr_TestHt(nPosX >> 8, gShoot.nPlayerPosY >> 8, 0);
	if ((s32)nHt < 0)
	{
		nCol = e_BlockCode_Hard;
		gShoot.nPlayerGnd = nCol;
		gShoot.nPlayerSpdY = 0;
		gShoot.nPlayerPosY &= ~0xFF;
		gShoot.nPlayerPosY += nHt << 8;
		gShoot.nPlayerLastGndPosY = gShoot.nPlayerPosY;
		return (nCol);
	}
//<

	return (0);
}

/*
>> version 1, sans test du bloc de col.
void GroundCheck(u32 nPosX)
{
	s32	*pBlocks;
	u32	nBlockNo;
	u32	nHt;

	if (gShoot.nPlayerSpdY < 0) return;

	pBlocks = gMap.ppPlanesBlocks[gMap.nHeroPlane];	// Blocs.

	nBlockNo = *(pBlocks + ((gShoot.nPlayerPosY >> 12) * gMap.nMapLg) + (nPosX >> 12) );
	nHt = BlockGetHeight(nBlockNo, (nPosX >> 8));
	if (nHt)
	{
		nHt = 16 - nHt;
		if (((gShoot.nPlayerPosY >> 8) & 0x0F) >= nHt)
		{
			gShoot.nPlayerGnd = 1;
			gShoot.nPlayerSpdY = 0;
			gShoot.nPlayerPosY &= ~0xF00;
			gShoot.nPlayerPosY += (nHt) << 8;

			// Si on pose sur la ligne supérieure, on regarde aussi le bloc du dessus.
			if (nHt == 0)
			{
				nBlockNo = *(pBlocks + (((gShoot.nPlayerPosY >> 12) - 1) * gMap.nMapLg) + (nPosX >> 12) );
				nHt = BlockGetHeight(nBlockNo, (nPosX >> 8));
				if (nHt)
				{
					nHt = 16 - nHt;
					gShoot.nPlayerPosY -= 0x100;
					gShoot.nPlayerPosY &= ~0xF00;
					gShoot.nPlayerPosY += (nHt) << 8;
				}
			}

			return;
		}
	}

}
<<
*/

// Gestion du sol.
// Out : b0 = Touche à droite / b1 = Touche à gauche / Tester avec & car le résultat peut être 3.
u32 Gen_GroundManage(s32 nSpdXMax, s32 nSpdYMax, s32 nRefOffset, u32 nPfTest)
{
	u32	nCol0, nCol1;
	u32	nTouch = 0;
	s32	nPosYSav = 0;

	gShoot.nPlayerLastGnd = gShoot.nPlayerGnd;
	gShoot.nPlayerGnd = 0;		// RAZ gnd.

	gShoot.nPlayerSpdY += GRAVITY;
	if (gShoot.nPlayerSpdY > nSpdYMax) gShoot.nPlayerSpdY = nSpdYMax;
	gShoot.nPlayerPosY += gShoot.nPlayerSpdY;

	// Pour coller au sol dans les pentes, quand nPlayerLastGnd, on rajoute spdmax_x au y.
	if (gShoot.nPlayerLastGnd) gShoot.nPlayerPosY += nSpdXMax + 0x100;
	// Test du sol.
	nCol0 = Gen_GroundCheck(gShoot.nPlayerPosX + nRefOffset);
	if (nCol0)
	{
		nTouch |= 1;
		nPosYSav = gShoot.nPlayerPosY;
	}
	nCol1 = Gen_GroundCheck(gShoot.nPlayerPosX - nRefOffset);
	if (nCol1)
	{
		nTouch |= 2;
		if (nPosYSav != gShoot.nPlayerPosY) nTouch &= ~1;
	}
	// Dernier recalage si on vient d'atterrir sur une pf.
	if (gShoot.nPlayerLastGnd == 0 && gShoot.nPlayerGnd && ((nCol0 & 0x0F) == e_BlockCode_Soft || (nCol1 & 0x0F) == e_BlockCode_Soft))
	{
		gShoot.nPlayerLastGnd = 1;
		Gen_GroundCheck(gShoot.nPlayerPosX + nRefOffset);
		Gen_GroundCheck(gShoot.nPlayerPosX - nRefOffset);
	}
// dust...

	// Plateforme mobile ?
	if (nPfTest)
	if (gShoot.nPfAddress)
	{
		if (gShoot.nPlayerGnd == 0)
		{
			gShoot.nPlayerGnd = 1;	// Sur pf et pas de contact avec le sol => on fait comme si on était par terre.
			gShoot.nPlayerSpdY = 0;
			gShoot.nPlayerLastGndPosY = gShoot.nPlayerPosY;
			nTouch = 0;
		}
		else
		{
			gShoot.nPfAddress = 0;	// Si sur pf on touche le sol, priorité au sol.
		}
	}

	// Remise au y précédent si on n'a pas touché le sol (une vraie chute commence).
	if (gShoot.nPlayerLastGnd && gShoot.nPlayerGnd == 0) gShoot.nPlayerPosY -= nSpdXMax + 0x100;

	return (nTouch);
}

// Test sur les côtés.
void Gen_SideCheck(s32 nTestOffsX, s32 nSpdMaxX)
{
	s32	*pBlocks;
	u32	nBlockNo;
	u32	nHt;
	s32 nPosX, nPosY;
	s32	nSprPosY;

	nPosX = gShoot.nPlayerPosX + gShoot.nPlayerSpdX + (gShoot.nPlayerSpdX >= 0 ? nTestOffsX : -nTestOffsX);
	pBlocks = gMap.ppPlanesBlocks[gMap.nHeroPlane];	// Blocs.

	// Au niveau du sol.
	nPosY = gShoot.nPlayerPosY - (nSpdMaxX * 2);
//SprDisplay(e_Spr_Tstrct_Cross, nPosX>>8, nPosY>>8, 250);
	nBlockNo = *(pBlocks + ((nPosY >> 12) * gMap.nMapLg) + (nPosX >> 12) );
	if (((gMap.ppColCodes[gMap.nHeroPlane] + nBlockNo)->nCol & 0x0F) == e_BlockCode_Hard)	// On ne teste que sur du dur.
	{
		nHt = BlockGetHeight(nBlockNo, (nPosX >> 8));
		if (nHt)
		{
			nHt = 16 - nHt;
			if (((nPosY >> 8) & 0x0F) >= nHt)
			{
				gShoot.nPlayerSpdX = 0;
				return;
			}
		}
	}
	// Hard sprite ?
	nSprPosY = (gShoot.nPlayerPosY >> 8) + HardSpr_TestHt(nPosX >> 8, gShoot.nPlayerPosY >> 8, 0);
	if ((nPosY >> 8) >= nSprPosY)	// Le test du hard sprite suffit à ce niveau. Les bords doivent être au pire verticaux, si ça ne bloque pas ici, ça ne bloquera pas plus haut.
	{
		gShoot.nPlayerSpdX = 0;
		return;
	}

//	// Si on est accroupi, on arrête les tests ici.
//	if (AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_CrouchStance ||
//		AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_CrouchWalk) return;
	// Si on est accroupi ou en train de nager, on arrête les tests ici.
	if (AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_CrouchStance ||
		AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_CrouchWalk ||
//		AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_HeroSub_Stance ||
//		AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_HeroSub_Run ||
//		(AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_SlugOut && gShoot.nVehicleType == e_HeroVehicle_None_Swim) ) return;
////todo: pas besoin du && vehicletype ?
		AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_SlugOut ||
		gShoot.nVehicleType == e_HeroVehicle_None_Swim) return;

	// Au niveau du torse.
	nPosY -= 1 << 12;
//SprDisplay(e_Spr_Tstrct_Cross, nPosX>>8, nPosY>>8, 250);
	nBlockNo = *(pBlocks + ((nPosY >> 12) * gMap.nMapLg) + (nPosX >> 12) );
	if (((gMap.ppColCodes[gMap.nHeroPlane] + nBlockNo)->nCol & 0x0F) == e_BlockCode_Hard)	// On ne teste que sur du dur.
	{
		nHt = BlockGetHeight(nBlockNo, (nPosX >> 8));
		if (nHt)
		{
			nHt = 16 - nHt;
			if (((nPosY >> 8) & 0x0F) >= nHt)
			{
				gShoot.nPlayerSpdX = 0;
				return;
			}
		}
	}


// tst
//if (AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_HeroSub_Stance ||
//	AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_HeroSub_Run) return;


	// Et enfin au niveau de la tête.
	nPosY -= 10 << 8;
//SprDisplay(e_Spr_Tstrct_Cross, nPosX>>8, nPosY>>8, 250);
//SprDisplay(e_Spr_Tstrct_CornerUL, (nPosX>>12)<<4, (nPosY>>12)<<4, 250);
	nBlockNo = *(pBlocks + ((nPosY >> 12) * gMap.nMapLg) + (nPosX >> 12) );
	if (((gMap.ppColCodes[gMap.nHeroPlane] + nBlockNo)->nCol & 0x0F) == e_BlockCode_Hard)	// On ne teste que sur du dur.
	{
		nHt = BlockGetHeight(nBlockNo, (nPosX >> 8));
		if (nHt)
		{
			nHt = 16 - nHt;
			if (((nPosY >> 8) & 0x0F) >= nHt)
			{
				gShoot.nPlayerSpdX = 0;
				return;
			}
		}
	}

}

/* - version avant de la rendre générique.
// Test sur les côtés.
void SideCheck(void)
{
	s32	*pBlocks;
	u32	nBlockNo;
	u32	nHt;
	s32 nPosX, nPosY;

	nPosX = gShoot.nPlayerPosX + gShoot.nPlayerSpdX + (gShoot.nPlayerSpdX >= 0 ? PLYR_NAKED_REF_OFFSETX : -PLYR_NAKED_REF_OFFSETX);

	pBlocks = gMap.ppPlanesBlocks[gMap.nHeroPlane];	// Blocs.

	// Au niveau du sol.
	nPosY = gShoot.nPlayerPosY - (PLYR_NAKED_SPDX_MAX * 2);
//SprDisplay(e_Spr_Ball, nPosX>>8, nPosY>>8, 250);
	nBlockNo = *(pBlocks + ((nPosY >> 12) * gMap.nMapLg) + (nPosX >> 12) );
	if (((gMap.ppColCodes[gMap.nHeroPlane] + nBlockNo)->nCol & 0x0F) == e_BlockCode_Hard)	// On ne teste que sur du dur.
	{
		nHt = BlockGetHeight(nBlockNo, (nPosX >> 8));
		if (nHt)
		{
			nHt = 16 - nHt;
			if (((nPosY >> 8) & 0x0F) >= nHt)
			{
				gShoot.nPlayerSpdX = 0;
				return;
			}
		}
	}

	// Si on est accroupi, on arrête les tests ici.
	if (AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_CrouchStance ||
		AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_CrouchWalk) return;

	// Au niveau du torse.
	nPosY -= 1 << 12;
//SprDisplay(e_Spr_Ball, nPosX>>8, nPosY>>8, 250);
	nBlockNo = *(pBlocks + ((nPosY >> 12) * gMap.nMapLg) + (nPosX >> 12) );
	if (((gMap.ppColCodes[gMap.nHeroPlane] + nBlockNo)->nCol & 0x0F) == e_BlockCode_Hard)	// On ne teste que sur du dur.
	{
		nHt = BlockGetHeight(nBlockNo, (nPosX >> 8));
		if (nHt)
		{
			nHt = 16 - nHt;
			if (((nPosY >> 8) & 0x0F) >= nHt)
			{
				gShoot.nPlayerSpdX = 0;
				return;
			}
		}
	}

	// Et enfin au niveau de la tête.
	nPosY -= 10 << 8;
//SprDisplay(e_Spr_Ball, nPosX>>8, nPosY>>8, 250);
	nBlockNo = *(pBlocks + ((nPosY >> 12) * gMap.nMapLg) + (nPosX >> 12) );
	if (((gMap.ppColCodes[gMap.nHeroPlane] + nBlockNo)->nCol & 0x0F) == e_BlockCode_Hard)	// On ne teste que sur du dur.
	{
		nHt = BlockGetHeight(nBlockNo, (nPosX >> 8));
		if (nHt)
		{
			nHt = 16 - nHt;
			if (((nPosY >> 8) & 0x0F) >= nHt)
			{
				gShoot.nPlayerSpdX = 0;
				return;
			}
		}
	}

}
*/

// Pendant un saut, la tête touche ?
void HeadCheck(u32 nPosX)
{
	s32	*pBlocks;
	u32	nBlockNo;
	u32	nHt;
	s32 nPosY;

	if (gShoot.nPlayerSpdY >= 0) return;

	pBlocks = gMap.ppPlanesBlocks[gMap.nHeroPlane];	// Blocs.

	nPosY = gShoot.nPlayerPosY + gShoot.nPlayerSpdY;

////	nPosY -= 24 << 8;
//	nPosY -= (PLYR_NAKED_SPDX_MAX * 2) + (1 << 12);
	nPosY -= (PLYR_NAKED_SPDX_MAX * 2) + (1 << 12) + (10 << 8);		// = Même calcul que dans SideCheck, mais vitesse en dur car fct utlisée seulement dans 'naked'.
//SprDisplay(e_Spr_Tstrct_Cross, nPosX>>8, nPosY>>8, 250);
//SprDisplay(e_Spr_Tstrct_CornerUL, (nPosX>>12)<<4, (nPosY>>12)<<4, 250);
	nBlockNo = *(pBlocks + ((nPosY >> 12) * gMap.nMapLg) + (nPosX >> 12) );
	if (((gMap.ppColCodes[gMap.nHeroPlane] + nBlockNo)->nCol & 0x0F) == e_BlockCode_Hard)	// On ne teste que sur du dur.
	{
		nHt = BlockGetHeight(nBlockNo, (nPosX >> 8));
		if (nHt)
		{
			gShoot.nPlayerSpdY = 0;
		}
	}

}

// Si on est accroupi, on teste au dessus du perso pour voir si on permet de se relever.
u32 CrouchCheck(u32 nPosX)
{
	s32	*pBlocks;
	u32	nBlockNo;
	u32	nHt;
	s32 nPosY;

	if (AnmGetKey(gShoot.nPlayerAnm) != e_AnmKey_Hero_CrouchStance &&
		AnmGetKey(gShoot.nPlayerAnm) != e_AnmKey_Hero_CrouchWalk) return (0);

	pBlocks = gMap.ppPlanesBlocks[gMap.nHeroPlane];	// Blocs.

	nPosY = gShoot.nPlayerPosY;
	nPosY -= (PLYR_NAKED_SPDX_MAX * 2) + (1 << 12);// + (10 << 8);
	nBlockNo = *(pBlocks + ((nPosY >> 12) * gMap.nMapLg) + (nPosX >> 12) );

	if (((gMap.ppColCodes[gMap.nHeroPlane] + nBlockNo)->nCol & 0x0F) == e_BlockCode_Hard)	// On ne teste que sur du dur.
	{
		nHt = BlockGetHeight(nBlockNo, (nPosX >> 8));
		if (nHt)
		{
//SprDisplay(e_Spr_Tstrct_Cross, nPosX>>8, nPosY>>8, 250);
			return (1);
		}
	}
	return (0);
}



// Mort, inits particulières - naked.
void Player_DeathSet_Naked(void)
{
	u32	nAnm;

	gShoot.nPlayerSpdX = 0;
	nAnm = e_Anm_Hero_DeathGen_Gnd;
	if (gShoot.nPlayerGnd == 0)
	{
		nAnm = e_Anm_Hero_DeathGen_Air;
		//if (gShoot.nPlayerSpdY < 0) gShoot.nPlayerSpdY = 0;
		gShoot.nPlayerSpdY = -0x400;	// Petit saut quand touché en l'air.
	}
	AnmSetIfNew(gpHeroAnm[nAnm], gShoot.nPlayerAnm);

	// Laisse tomber l'arme en cours.
	static u64 *pHeroWeaponDust[e_Player_Weapon_Max] =
		{ gAnm_Weapon_Gun_Dust, gAnm_Weapon_Shotgun_Dust, gAnm_Weapon_Machinegun_Dust, gAnm_Weapon_Shotgun_Dust, gAnm_Weapon_Shotgun_Dust };
	gDustExg.nRotInc = 1;
	DustSetMvt(pHeroWeaponDust[gShoot.nWeapon], gShoot.nPlayerPosX + ((gShoot.nPlayerDir ? -4 : 4) << 8), gShoot.nPlayerPosY - (16 << 8), (gShoot.nPlayerDir ? -0x100 : 0x100), gShoot.nPlayerSpdY / 2, e_Prio_Joueur + 2, (gShoot.nPlayerDir ? e_DustFlag_FlipX : 0) | e_DustFlag_Gravity | e_DustFlag_Rotation);
	if (gShoot.nWeapon == e_Player_Weapon_Machinegun)
	{
		gDustExg.nRotInc = -1;
		DustSetMvt(pHeroWeaponDust[gShoot.nWeapon], gShoot.nPlayerPosX + ((gShoot.nPlayerDir ? -4 : 4) << 8), gShoot.nPlayerPosY - (16 << 8), (gShoot.nPlayerDir ? 0x100 : -0x100), gShoot.nPlayerSpdY / 2, e_Prio_Joueur - 2, (gShoot.nPlayerDir ? e_DustFlag_FlipX : 0) | e_DustFlag_Gravity | e_DustFlag_Rotation);
	}

}

// Mort, inits particulières - swim.
void Player_DeathSet_Swim(void)
{
	u32	nAnm;

	gShoot.nPlayerSpdX = 0;
	gShoot.nPlayerSpdY = 0;
	nAnm = e_Anm_HeroSub_DeathGen;
	AnmSetIfNew(gpHeroAnm[nAnm], gShoot.nPlayerAnm);

	// Le casque.
	s32	nOffsX, nOffsY;
	nOffsX = (gShoot.nPlayerDir ? -15 : 15);
	nOffsY = -9;
	DustSetMvt(gAnm_HeroSub_HelmetDeath_Dust, gShoot.nPlayerPosX + (nOffsX << 8), gShoot.nPlayerPosY + (nOffsY << 8), (gShoot.nPlayerDir ? -0x28 : 0x28), -0x20, e_Prio_Joueur + 2, (gShoot.nPlayerDir ? e_DustFlag_FlipX : 0));

}

// Mort, inits particulières - Cas des véhicules desquels on ne doit pas descendre.
void Player_DeathSet_SlugNoExit_Temp(void)
{
	u32	nAnm;

	gShoot.nPlayerSpdX = 0;
	gShoot.nPlayerSpdY = 0;
	nAnm = e_Anm_HeroSlugNoExit_DeathGen;
	AnmSetIfNew(gpHeroAnm[nAnm], gShoot.nPlayerAnm);

}

// Place le joueur en mort, en fct du véhicule.
typedef void (*pFctDeathSet) (void);
void Game_Player_DeathSet(void)
{
	static pFctDeathSet pFctDeathTb[e_HeroVehicle_MAX] =
	{ Player_DeathSet_Naked, Player_DeathSet_Swim, NULL, Player_DeathSet_SlugNoExit_Temp, NULL, NULL, NULL, NULL, NULL };

	if (AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_Death ||
		AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_DeathAir) return;

	// Si parachute, repasse en naked.
	if (gShoot.nVehicleType == e_HeroVehicle_None_Parachute)
		Player_VehicleSet(e_HeroVehicle_None_Naked, gShoot.nPlayerPosX, gShoot.nPlayerPosY, SLUG_ENERGY_MAX, 0, 0);

	if (pFctDeathTb[gShoot.nVehicleType] == NULL) return;	// Au cas ou...
	pFctDeathTb[gShoot.nVehicleType]();

	if (gShoot.nPlayerAnmTop != -1)		// Libère l'anim top.
	{
		AnmReleaseSlot(gShoot.nPlayerAnmTop);
		gShoot.nPlayerAnmTop = -1;
	}
	if (gShoot.nParachuteAnm != -1)			// Libère l'anim du parachute.
	{
		AnmReleaseSlot(gShoot.nParachuteAnm);
		gShoot.nParachuteAnm = -1;
	}
	Gen_KbNoControl();
	gShoot.nDeathFlag = 1;		// < Je pense que c'est une bonne idée de le placer ici aussi.

}

// Si le joueur est en anim de mort, coupe le contrôle.
void Player_DeathManage(void)
{
	gShoot.nDeathFlag = 0;

	// Si mort, on coupe le contrôle.
	if (AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_Death ||
		AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_DeathAir)
	{
		gShoot.nDeathFlag = 1;

		// Si mort + chute, met l'anim d'arrivée au sol quand on touche le sol.
		if (AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_DeathAir && gShoot.nPlayerGnd)
			AnmSetIfNew(gpHeroAnm[e_Anm_Hero_DeathGen_Land], gShoot.nPlayerAnm);

		// RAZ kb.
		Gen_KbNoControl();

		// Anim terminée ?
		if (AnmCheckEnd(gShoot.nPlayerAnm))
		{
			//>> Cheat : Vies infinies (ici poour le cas de l'activation du flag avec 0 vies).
//			if (gCCodes.nCheat & e_Cheat_InfiniteLives)
			if (gCCodes.nCheat & e_Cheat_InfiniteLives || gpMstQuestItems[MST_QUEST_ITEM_NEXT_LEVEL])
				gShoot.nPlayerLives++;
			//<< Cheat : Vies infinies.
			// OR : Pour prise en compte des cas à la con du style le joueur et le boss s'entretuent (sur la dernière vie du joueur ou pas).
			// Si on bat le boss, le joueur ne perd pas de vie. (On est sympa !).

			// Il reste des vies ?
			if (gShoot.nPlayerLives)
			{
				gShoot.nPlayerLives--;
				gShoot.nHUDPlayerLivesBlink = 0;		// RAZ clignotement du HUD quand vie--.
				// Re-init joueur.
				u32	nPlayerLastDir = gShoot.nPlayerDir;
				Player_InitLife(gShoot.nPlayerPosX, gShoot.nPlayerPosY, gShoot.nVehicleType);
				Player_ReinitFlipX(nPlayerLastDir);
				gShoot.nInvulnerability = PLYR_INVULNERABILTY_CNT;	// Petit temps d'invulnérabilité.
			}
			else
			if (gGameVar.nPhase == e_Game_Normal)	// Pour ne faire les inits qu'une fois.
			{
				// Joueur mort, phase tmp pour sélection du Game Over ou du Continue.
				gGameVar.nPhase = e_Game_PlayerDead;
			}
		}
	}

#ifdef DEBUG_KEYS
if (gVar.pKeys[SDLK_e]) Game_Player_DeathSet();  // test de mort.
#endif

}

// Gestion de la victoire.
// Renvoie tjs 0 sauf s'il faut indiquer à un véhicule qu'on a forcé la sortie du héros.
u32 Player_VictoryManage(void)
{
	// Si victoire OU changement de level :
	if (gpMstQuestItems[MST_QUEST_ITEM_NEXT_LEVEL])
	{
		Gen_KbNoControl();		// Coupe le contrôle.
		gShoot.nInvulnerability = PLYR_INVL_BLINK - 1;	// Invulnérable.
	}

	// On quitte ici si pas de victoire.
	if ((gpMstQuestItems[MST_QUEST_ITEM_NEXT_LEVEL] & 2) == 0) return (0);

	// Victoire.
	switch (gShoot.nVehicleType)
	{
	case e_HeroVehicle_None_Naked:
		// Cas improbable mais au cas ou : Le perso est en victoire, mais le sol disparait sous ses pieds.
		if (gShoot.nPlayerGnd == 0 && AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_Victory)
		{
			AnmSet(gpHeroAnm[e_Anm_Hero_Stance], gShoot.nPlayerAnm);		// Passe au repos, le reste de la gestion du héros le mettra en chute.
		}
		// Au sol ? => Anim de victoire.
		if (gShoot.nPlayerGnd)
		{
			if (gShoot.nPlayerAnmTop != -1)		// Libère l'anim top.
			{
				AnmReleaseSlot(gShoot.nPlayerAnmTop);
				gShoot.nPlayerAnmTop = -1;
			}
			AnmSetIfNew(gpHeroAnm[e_Anm_Hero_Victory], gShoot.nPlayerAnm);	// Anim de victoire.
		}
		break;

	case e_HeroVehicle_Tank:
		// Si le tank est au sol, éjecter le perso.
		if (gShoot.nPlayerGnd)
		{
			Slug_GetOut(e_HeroVehicle_Tank, e_HeroVehicle_None_Naked, gAnm_Hero_SlugOutGen, 2 * PLYR_NAKED_JUMP_VAL / 3);
			return (1);
		}
		break;

	case e_HeroVehicle_None_Swim:
	case e_HeroVehicle_None_Parachute:
	case e_HeroVehicle_None_SlugNoExit_Temp:
	case e_HeroVehicle_Submarine:
	case e_HeroVehicle_Helicopter:
	case e_HeroVehicle_HelicopterNoExit:
	case e_HeroVehicle_Rocket:
		break;
	}

	return (0);
}

// Contrôle du joueur.
void PlayerControl_Naked(void)
{
	u32	i;
	u32	nAnmNo;

	nAnmNo = e_Anm_Hero_Stance;

	// Gestion du pilote automatique. (How to play + credits).
	Slug_AutoPilot_Manage();

	// Gestion de la mort.
	Player_DeathManage();
	// Gestion de la victoire.
	Player_VictoryManage();

	// Slug out : Couper le contrôle.
	if (AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_SlugOut)
	{
		// On touche le sol ? => Passe en wait.
		if (gShoot.nPlayerGnd)
			AnmSet(gpHeroAnm[e_Anm_Hero_Stance], gShoot.nPlayerAnm);
		else
			Gen_KbNoControl();
	}

	// *** Test du déplacement latéral.

//	// Vitesse. (v.1)
////	if (PlyrMove_Spd(&gShoot.nPlayerSpdX, (AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_CrouchWalk ? PLYR_NAKED_SPDX_MAX / 2 : PLYR_NAKED_SPDX_MAX), PLYR_NAKED_ACCX, SDLK_LEFT, SDLK_RIGHT)) nAnmNo = e_Anm_Hero_Run;
//	if (PlyrMove_Spd(&gShoot.nPlayerSpdX, (AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_CrouchWalk ? PLYR_NAKED_SPDX_MAX / 2 : PLYR_NAKED_SPDX_MAX), PLYR_NAKED_ACCX, gMSCfg.pKeys[e_CfgKey_Left], gMSCfg.pKeys[e_CfgKey_Right])) nAnmNo = e_Anm_Hero_Run;

	// Vitesse. (Spd Max X = cos(angle)).
	s32	nSpdXMax = (AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_CrouchWalk ? PLYR_NAKED_SPDX_MAX / 2 : PLYR_NAKED_SPDX_MAX);
	nSpdXMax = (nSpdXMax * ABS(gVar.pCos[(u8)gShoot.nPlayerGndAng])) / 256;
//	if (PlyrMove_Spd(&gShoot.nPlayerSpdX, nSpdXMax, PLYR_NAKED_ACCX, SDLK_LEFT, SDLK_RIGHT)) nAnmNo = e_Anm_Hero_Run;
	if (PlyrMove_Spd(&gShoot.nPlayerSpdX, nSpdXMax, PLYR_NAKED_ACCX, gMSCfg.pKeys[e_CfgKey_Left], gMSCfg.pKeys[e_CfgKey_Right])) nAnmNo = e_Anm_Hero_Run;

	if (gShoot.nPlayerSpdX) gShoot.nPlayerDir = (gShoot.nPlayerSpdX >= 0 ? 0 : 1);
	// Blocage quand crouch + tir.
	if ((AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_CrouchWalk ||
		 AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_CrouchStance) && gShoot.nPlayerAnmTop != -1)
	{
		gShoot.nPlayerSpdX = 0;
		// !!! Ne pas s'occuper de l'anim !!!
		// On laisse stance ou run (ça dépend si le joueur appuie ou pas sur une direction) => Ca corrige le flick de 1 frame entre le tir et le move quand mode enchainé.
	}
	// Limites gauche et droite. Map ou fenêtre.
	PlayerMove_LimitX(PLYR_NAKED_REF_OFFSETX);
	PlayerMove_LimitY(PLYR_NAKED_HEIGHT);

	// Si on est accroupi, on teste au dessus du perso pour voir si on permet de se relever.
	i = CrouchCheck(gShoot.nPlayerPosX + PLYR_NAKED_REF_OFFSETX);
	i += CrouchCheck(gShoot.nPlayerPosX - PLYR_NAKED_REF_OFFSETX);
	// On s'accroupit ?
//	if (gVar.pKeys[SDLK_DOWN] || i) nAnmNo++;	// Ok car à ce moment là, seulement Idle ou Run.
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]] || i) nAnmNo++;	// Ok car à ce moment là, seulement Idle ou Run.
	// Si accroupi forcé, bloque le saut.
//	if (i) gVar.pKeys[KB_BUTTONB] = 0;
	if (i) gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonB]] = 0;

	Gen_SideCheck(PLYR_NAKED_REF_OFFSETX, PLYR_NAKED_SPDX_MAX);	// Mettra la vitesse à 0 si mouvement pas possible.

	gShoot.nPlayerPosX += gShoot.nPlayerSpdX;

	// Jump ?
	Gen_ButtonB_Check(PLYR_NAKED_JUMP_VAL);

	// Pendant un saut, la tête touche ?
	HeadCheck(gShoot.nPlayerPosX + PLYR_NAKED_REF_OFFSETX);
	HeadCheck(gShoot.nPlayerPosX - PLYR_NAKED_REF_OFFSETX);

	// *** Test du sol.
	u32	nTouch;
	nTouch = Gen_GroundManage(PLYR_NAKED_SPDX_MAX, PLYR_NAKED_SPDY_MAX, PLYR_NAKED_REF_OFFSETX, 1);

	// Calcul de l'angle au niveau du sol (calcul entre le point central et le point du côté qui touche).
	gShoot.nPlayerGndAng = 0;	// Par défaut, RAZ.
	if (nTouch && nTouch != 3)
	{
		s32	nHt;
		nHt = BlockGetGroundLevel((gShoot.nPlayerPosX) >> 8, (gShoot.nPlayerPosY) >> 8);
		if (nHt <= PLYR_NAKED_REF_OFFSETX >> 8)
		{
			// Ok, le point qui touche et le point au centre sont sur une pente.
			gShoot.nPlayerGndAng = fatan2((nTouch & 1 ? nHt : -nHt) << 8, PLYR_NAKED_REF_OFFSETX);
		}
	}

	// Anim de jump/fall.
	if (gShoot.nPlayerGnd == 0) nAnmNo = (gShoot.nPlayerSpdY < 0 ? e_Anm_Hero_Jump : e_Anm_Hero_Fall);

	// Anims d'ennui.
	if (AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_Stance && gShoot.nPlayerAnmTop == -1)
	{
		// Toutes les 2 sec, un rnd.
		if (++gShoot.nBoredCnt > 128)
		{
			u32	nRnd = rand();
			if ((nRnd & 0xFF) < 0x40)
			{
				static u64 *pBoredAnm_Gun[e_Player_Weapon_Max][2] =
				{
					{ gAnm_HeroTop_Bored0_Gun, gAnm_HeroTop_Bored1_Gun, },
					{ gAnm_HeroTop_Bored0_Shotgun, gAnm_HeroTop_Bored1_Shotgun, },
					{ gAnm_HeroTop_Bored0_Machinegun, gAnm_HeroTop_Bored0_Machinegun, },
					{ gAnm_HeroTop_Bored0_Shotgun, gAnm_HeroTop_Bored1_Shotgun, },
					{ gAnm_HeroTop_Bored0_Shotgun, gAnm_HeroTop_Bored1_Shotgun, },
				};
				AnmSetIfNew(pBoredAnm_Gun[gShoot.nWeapon][nRnd & 1], gShoot.nPlayerAnm);
			}
			gShoot.nBoredCnt = 0;	// Que l'anim ait été mise ou pas, on repart à 0.
		}
	}
	else
		gShoot.nBoredCnt = 0;

	// Anim. (if uniquement pour ne pas dégager l'anim "bored").
	if ((nAnmNo == e_Anm_Hero_Stance && AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_Bored && gShoot.nPlayerAnmTop == -1) == 0)
	AnmSetIfNew(gpHeroAnm[nAnmNo], gShoot.nPlayerAnm);

	// *** Fin des jambes.

}

// Affichage du joueur + lancement des tirs.
void PlayerDisplay_Naked(void)
{
	u32	i;
	u32	nLegs, nTop, nAnmKey;


//	// Dust d'atterrissage.
//	if (gShoot.nPlayerGnd && gShoot.nPlayerLastGnd == 0) Gen_LandDust(gShoot.nPlayerGnd, gShoot.nPlayerPosX, gShoot.nPlayerPosY);

	//>> Cheat : Changement d'arme.
	if (gCCodes.nCheat & e_Cheat_WeaponSelect)
	if (gVar.pKeys[SDLK_F5])
	{
		if (++gShoot.nWeapon >= e_Player_Weapon_Max) gShoot.nWeapon = 0;
		Player_WeaponSet(gShoot.nWeapon);
		gVar.pKeys[SDLK_F5] = 0;
	}
	//<< Cheat : Changement d'arme.

//Font_Print(10,180, (gShoot.nMstProximity ? "1" : "0"), 0);

	// Tir, appui sur le bouton ?
	Player_ButtonA_Check();

	// Lancer de grenade ?
//	if (gVar.pKeys[KB_BUTTONC] && gShoot.nBombAmmo)
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonC]] && gShoot.nBombAmmo)
	{
		// Anim.
		nAnmKey = (gShoot.nPlayerAnmTop == -1 ? e_AnmKey_Null : AnmGetKey(gShoot.nPlayerAnmTop));
		if (nAnmKey == e_AnmKey_Hero_ThrowHG)
			AnmSet(gAnm_Hero_ThrowHG_Gen, gShoot.nPlayerAnmTop);	// Reset de l'anim de lancé.
		else
			gShoot.nPlayerAnmTop = AnmSetIfNew(gAnm_Hero_ThrowHG_Gen, gShoot.nPlayerAnmTop);
		// Décalage en hauteur (debout / accroupi).
		nAnmKey = AnmGetKey(gShoot.nPlayerAnm);
		i = (nAnmKey == e_AnmKey_Hero_CrouchStance || nAnmKey == e_AnmKey_Hero_CrouchWalk ? 16 : 30);
		// Tir.
		FireAdd(e_Shot_Player_HandGrenade_Throw, gShoot.nPlayerPosX + (0 << 8), gShoot.nPlayerPosY - (i << 8), (gShoot.nPlayerDir ? 128 : 0));
//		gVar.pKeys[KB_BUTTONC] = 0;
		gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonC]] = 0;
		gShoot.nBombAmmo--;
	}

	// *** Tirs, à cause des tirs qui bouclent, on est obligé de faire un test d'image ici, AVANT l'affichage réel.
	Player_TopAnmAdvance();
/*
	// Avance de l'anim du haut.
	if (gShoot.nPlayerAnmTop != -1)
	{
		i = AnmGetImage(gShoot.nPlayerAnmTop);
		if (i == (u32)-1)
		{
			gShoot.nPlayerAnmTop = -1;		// Anim du haut finie.
		}
		else
		{
			// Gestion des tirs en fonction des armes.
			if (AnmGetKey(gShoot.nPlayerAnmTop) == e_AnmKey_Hero_Shot)
			{

				// Nouvelle image ?
				if (AnmCheckNewImgFlag(gShoot.nPlayerAnmTop))
				{
					// Cas particulier de la mitrailleuse/flamethrower, loop + il faut mettre l'anim de fin de tir à la main.
// ou simplifier le test en testant que gpHeroShotEndAnm[] != 0 au lieu de l'arme ? (tb 7 lignes plus bas).
					if (gShoot.nWeapon == e_Player_Weapon_Machinegun ||
						gShoot.nWeapon == e_Player_Weapon_Flamethrower)
					{
						// On change d'image, on a toujours le doigt sur le tir et il reste des balles ?
//						if (gVar.pKeys[KB_BUTTONA] == 0 || gShoot.nAmmo == 0)	// || plus de balles
						if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] == 0 || gShoot.nAmmo == 0)	// || plus de balles
						{
							static u32 *gpHeroShotEndAnm[e_Player_Weapon_Max] = { 0, 0, gAnm_Hero_ShotEnd_Machinegun, gAnm_Hero_ShotEnd_Flamethrower, 0 };	// Anim de tir en fct de l'arme.
							// Non, mettre l'anim de fin de tir.
//							gShoot.nPlayerAnmTop = AnmSetIfNew(gAnm_Hero_ShotEnd_Machinegun, gShoot.nPlayerAnmTop);
							gShoot.nPlayerAnmTop = AnmSetIfNew(gpHeroShotEndAnm[gShoot.nWeapon], gShoot.nPlayerAnmTop);
						}
					}

				}

			} // AnmKey == e_AnmKey_Hero_Shot
		} // if (i == (u32)-1)
	}
*/



	// Affichage du héros.

	// *** 1) Récupère les sprites bottom et top basiques.
	Player_GetTopBottom(&nLegs, &nTop);
	nAnmKey = AnmGetKey(gShoot.nPlayerAnm);

	// *** 2) Ensuite, y-a-t'il une anim spéciale en haut ? Si oui, on met le sprite qui va bien. Si nécessaire, écrase celui du bas (accroupi...).
	// Anim de tir ?
	gShoot.nPlayerKnifeSprCol = SPR_NoSprite;		// RAZ frame de hit (couteau).
	if (gShoot.nPlayerAnmTop != -1)
	{
		static u32 gpHeroTopShotImg[][e_Player_Weapon_Max] =
		{
			{ e_Spr_HeroTop_Shoot_Gun, e_Spr_HeroTop_Shoot_Shotgun, e_Spr_HeroTop_Shoot_Machinegun, e_Spr_HeroTop_Shoot_Shotgun, e_Spr_HeroTop_Shoot_Shotgun },
			{ e_Spr_HeroTop_ShootUp_Gun, e_Spr_HeroTop_ShootUp_Shotgun, e_Spr_HeroTop_ShootUp_Machinegun, e_Spr_HeroTop_ShootUp_Shotgun, e_Spr_HeroTop_ShootUp_Shotgun },
			{ e_Spr_HeroTop_ShootDown_Gun, e_Spr_HeroTop_ShootDown_Shotgun, e_Spr_HeroTop_ShootDown_Machinegun, e_Spr_HeroTop_ShootDown_Shotgun, e_Spr_HeroTop_ShootDown_Shotgun },
			{ e_Spr_HeroCrouch_Shoot_Gun, e_Spr_HeroCrouch_Shoot_Shotgun, e_Spr_HeroCrouch_Shoot_Machinegun, e_Spr_HeroCrouch_Shoot_Shotgun, e_Spr_HeroCrouch_Shoot_Shotgun },
			//
			{ e_Spr_HeroTop_ThrowHG_Gun, e_Spr_HeroTop_ThrowHG_Shotgun, e_Spr_HeroTop_ThrowHG_Machinegun, e_Spr_HeroTop_ThrowHG_Shotgun, e_Spr_HeroTop_ThrowHG_Shotgun },
			{ e_Spr_HeroTop_ThrowHG_Gun, e_Spr_HeroTop_ThrowHG_Shotgun, e_Spr_HeroTop_ThrowHG_Machinegun, e_Spr_HeroTop_ThrowHG_Shotgun, e_Spr_HeroTop_ThrowHG_Shotgun },	// Pas de up, mais évite un cas particulier.
			{ e_Spr_HeroTop_ThrowHG_Gun, e_Spr_HeroTop_ThrowHG_Shotgun, e_Spr_HeroTop_ThrowHG_Machinegun, e_Spr_HeroTop_ThrowHG_Shotgun, e_Spr_HeroTop_ThrowHG_Shotgun },	// Pas de down, mais évite un cas particulier.
			{ e_Spr_HeroCrouch_ThrowHG_Gun, e_Spr_HeroCrouch_ThrowHG_Shotgun, e_Spr_HeroCrouch_ThrowHG_Machinegun, e_Spr_HeroCrouch_ThrowHG_Shotgun, e_Spr_HeroCrouch_ThrowHG_Shotgun },
			//
			{ e_Spr_HeroTop_Knife1_Gun, e_Spr_HeroTop_Knife1_Shotgun, e_Spr_HeroTop_Knife1_Machinegun, e_Spr_HeroTop_Knife1_Shotgun, e_Spr_HeroTop_Knife1_Shotgun },
			{ e_Spr_HeroTop_Knife1_Gun, e_Spr_HeroTop_Knife1_Shotgun, e_Spr_HeroTop_Knife1_Machinegun, e_Spr_HeroTop_Knife1_Shotgun, e_Spr_HeroTop_Knife1_Shotgun },	// Pas de up, mais évite un cas particulier.
			{ e_Spr_HeroTop_Knife1_Gun, e_Spr_HeroTop_Knife1_Shotgun, e_Spr_HeroTop_Knife1_Machinegun, e_Spr_HeroTop_Knife1_Shotgun, e_Spr_HeroTop_Knife1_Shotgun },	// Pas de down, mais évite un cas particulier.
			{ e_Spr_HeroCrouch_Knife1_Gun, e_Spr_HeroCrouch_Knife1_Shotgun, e_Spr_HeroCrouch_Knife1_Machinegun, e_Spr_HeroCrouch_Knife1_Shotgun, e_Spr_HeroCrouch_Knife1_Shotgun },
		};
		u32	nUp = 0;
		u32	nTbIdx;
		//nTbIdx = (AnmGetKey(gShoot.nPlayerAnmTop) == e_AnmKey_Hero_ThrowHG ? 3 : 0);	// Index dans les anims pour lancer de grenade ou tir classique.

		// Index dans les anims pour tir classique, lancer de grenade ou coup de couteau.
		switch (AnmGetKey(gShoot.nPlayerAnmTop))
		{
		case e_AnmKey_Hero_ThrowHG:
			nTbIdx = 4;
			break;
		case e_AnmKey_Hero_Knife:
		case e_AnmKey_Hero_KnifeEnd:
			nTbIdx = 8;
			break;
		default:
			nTbIdx = 0;
			break;
		}

		i = AnmGetLastImage(gShoot.nPlayerAnmTop);
		if (nAnmKey == e_AnmKey_Hero_CrouchStance || nAnmKey == e_AnmKey_Hero_CrouchWalk)
		{
			nLegs = i + gpHeroTopShotImg[nTbIdx + 3][gShoot.nWeapon];
			i = nLegs;	// En cas de tir, on note le sprite.
		}
		else
		{
//			nUp = gVar.pKeys[SDLK_UP];		// nUp servira au déclenchement du tir.
//			nTop = i + gpHeroTopShotImg[nTbIdx + (nUp ? 1 : 0)][gShoot.nWeapon];		// Test du haut...

			// nUp servira au déclenchement du tir. 1 = Haut / 2 = Bas.
//			if (gVar.pKeys[SDLK_UP])
			if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Up]])
			{
				nUp = 1;
			}
			else
//			if (gShoot.nPlayerGnd == 0 && gVar.pKeys[SDLK_DOWN])
			if (gShoot.nPlayerGnd == 0 && gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]])
			{
				nUp = 2;
			}
			nTop = i + gpHeroTopShotImg[nTbIdx + nUp][gShoot.nWeapon];		// Test du haut/bas...

			i = nTop;	// En cas de tir, on note le sprite.
		}

		// Déclenchement d'un tir ?
		Player_ShotCheck(i, nUp);

	} //if (gShoot.nPlayerAnmTop != -1)


	if ((gShoot.nInvulnerability & PLYR_INVL_BLINK) == 0)	// Clignotement quand invulnérable.
	{
		// Affichage haut et bas.
		SprDisplay(nLegs ^ (gShoot.nPlayerDir ? SPR_Flip_X : 0), (gShoot.nPlayerPosX >> 8), (gShoot.nPlayerPosY >> 8), e_Prio_Joueur);
		if (nTop)
		SprDisplay(nTop ^ (gShoot.nPlayerDir ? SPR_Flip_X : 0), (gShoot.nPlayerPosX >> 8), (gShoot.nPlayerPosY >> 8), e_Prio_Joueur + 1);
		// Parachute en plus ?
		if (gShoot.nParachuteAnm != -1)
		{
			if ((i = AnmGetImage(gShoot.nParachuteAnm)) == (u32)-1)
				gShoot.nParachuteAnm = -1;	// Anim terminée.
			else
				SprDisplay(i ^ (gShoot.nPlayerDir ? SPR_Flip_X : 0), (gShoot.nPlayerPosX >> 8), (gShoot.nPlayerPosY >> 8), e_Prio_Joueur - 1);
		}
	}

	// Sprite de col du joueur (parce que je ne veux pas avoir d'erreur de rectangles).
	switch (nAnmKey)
	{
	case e_AnmKey_Hero_Run:
		i = e_Spr_HeroLegs_Run;
		break;
	case e_AnmKey_Hero_Jump:
		i = e_Spr_HeroLegs_Jump;
		break;
	case e_AnmKey_Hero_CrouchStance:
	case e_AnmKey_Hero_CrouchWalk:
		i = e_Spr_HeroCrouch_Stance_Gun;
		break;
	default:
		i = e_Spr_HeroLegs_Stance;	// Par défaut, stance.
		break;
	}
	gShoot.nPlayerSprCol = i ^ (gShoot.nPlayerDir ? SPR_Flip_X : 0);
	// Mort => Plus de sprite de collision (et pas de bit de flip).
	if (gShoot.nDeathFlag) gShoot.nPlayerSprCol = SPR_NoSprite;

	Player_ShotHitCheck();		// Teste si on se fait toucher par un tir ennemi.


/*
//>> tst / affichage du sprite de col
{
	struct SSprRect sRectC;
	if (SprGetRect(gShoot.nPlayerSprCol, e_SprRectZone_RectCol, &sRectC))
	{
	SprDisplay(e_Spr_Tstrct_CornerUL, (gShoot.nPlayerPosX >> 8) + sRectC.nX1, (gShoot.nPlayerPosY >> 8) + sRectC.nY1, e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerUR, (gShoot.nPlayerPosX >> 8) + sRectC.nX2, (gShoot.nPlayerPosY >> 8) + sRectC.nY1, e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerDL, (gShoot.nPlayerPosX >> 8) + sRectC.nX1, (gShoot.nPlayerPosY >> 8) + sRectC.nY2, e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerDR, (gShoot.nPlayerPosX >> 8) + sRectC.nX2, (gShoot.nPlayerPosY >> 8) + sRectC.nY2, e_Prio_Joueur + 3);
	}
}
//<< tst / affichage du sprite de col
*/

	// Dust d'atterrissage.
	if (gShoot.nPlayerGnd && gShoot.nPlayerLastGnd == 0) Gen_LandDust(e_HeroVehicle_None_Naked, gShoot.nPlayerGnd, gShoot.nPlayerPosX, gShoot.nPlayerPosY);
	// Dust de trace de pas, splash dans l'eau.
	if (!(gShoot.nDeathFlag && AnmCheckEnd(gShoot.nPlayerAnm)))	// Sauf quand mort.
	Gen_StepDust(e_HeroVehicle_None_Naked, gShoot.nPlayerGnd, 0, AnmCheckStepFlag(gShoot.nPlayerAnm), gShoot.nPlayerPosX, gShoot.nPlayerPosY);

}

// Traces de pas, splash, etc...
void Gen_StepDust(u32 nVehicle, u32 nGndCol, s32 nOffset, u32 nStepFlag, u32 nPosX, u32 nPosY)
{
	if ((nGndCol & 0xF0) == 0) return;		// Pas de col ?

	// Cas particulier de l'eau du lev13, au dessus du perso et pas un dust.
	if ((nGndCol & 0xF0) == 0x20)
	{
		SprDisplay(e_Spr_Steps_WaterL13 + ((gnFrame >> 3) & 3), (nPosX + nOffset) >> 8, (gShoot.nSplashLevel == -1 ? nPosY >> 8 : gShoot.nSplashLevel), 0x100 + 1);
		if (nOffset)	// Si offset, on met un second splash.
		SprDisplay(e_Spr_Steps_WaterL13 + (((gnFrame >> 3) + 1) & 3), (nPosX - nOffset) >> 8, (gShoot.nSplashLevel == -1 ? nPosY >> 8 : gShoot.nSplashLevel), 0x100 + 2);

		if (gShoot.nSplashLevel != -1) nPosY = (gShoot.nSplashLevel - 8) << 8;	// On profite du if pour décaler le Y pour le step dans l'eau.
	}

	// Traces de pas. (Seulement naked, sinon faire une table comme dans Gen_LandDust()).
	if (nStepFlag)
	{
//		static	u64	*gpStepAnmTb[] = { NULL, gAnm_Step_Sand_Dust, NULL, NULL };
		static	u64	*gpStepAnmTb[] = { NULL, gAnm_HumanStep_Sand_Dust, gAnm_HumanStep_Water_Dust, gAnm_HumanStep_Water_Dust };
		static	u8	gnStepPrio[] = { e_Prio_DustUnder, e_Prio_DustUnder, e_Prio_Joueur + 10, e_Prio_Joueur + 10 };

		if (gpStepAnmTb[(nGndCol >> 4) & 0x0F] != NULL)
			DustSet(gpStepAnmTb[(nGndCol >> 4) & 0x0F], nPosX, nPosY, gnStepPrio[(nGndCol >> 4) & 0x0F], 0);
	}

}

// Dust d'atterrissage.
void Gen_LandDust(u32 nVehicle, u32 nGndCol, u32 nPosX, u32 nPosY)
{
	if ((nGndCol & 0xF0) == 0) return;		// Pas de col ?

	static	u64	*gpLandAnmTb_Naked[] = { NULL, NULL, gAnm_Land_Water_Dust, gAnm_Land_Water_Dust };
	static	u64	*gpLandAnmTb_SlugTank[] = { NULL, NULL, gAnm_Slug_Land_Water_Dust, gAnm_Slug_Land_Water_Dust };
	static	u64	**gpLandAnmTb[e_HeroVehicle_MAX] = { gpLandAnmTb_Naked, NULL, NULL, NULL, NULL, gpLandAnmTb_SlugTank, NULL, NULL, NULL };
	u64	**pCurTb;

	if ((pCurTb = gpLandAnmTb[nVehicle]) != NULL)
	if (pCurTb[(nGndCol >> 4) & 0x0F] != NULL)
	{
		if ((nGndCol & 0xF0) == 0x20 && gShoot.nSplashLevel != -1) nPosY = (gShoot.nSplashLevel - 8) << 8;
		DustSet(pCurTb[(nGndCol >> 4) & 0x0F], nPosX, nPosY, e_Prio_Joueur + 10, 0);
	}

}

// Game.
// Out : Si pas 0, ne pas faire le reste des traitements.
u32 ShootGame(void)
{
	u32	nRet = 0;

	switch (gGameVar.nPhase)
	{
	case e_Game_LoadLevel:		// Load level.
		Transit2D_Reset();		// Au cas ou, reset transitions.
		LevelLoad(gGameVar.nLevel);
		AnmBlkInit(gGameVar.nLevel);
		GameInitLevel();
		FrameInit();
		Music_Start(gMissionTb[gGameVar.nGenLevel].nMusicNo, 0);
		gGameVar.nPhase = e_Game_Normal;				// Phase normale.
		// Mission n° == 0 => Cas spéciaux du how to play et des crédits.
		if (gMissionTb[gGameVar.nGenLevel].nMissionNo == 0)
		{
			Transit2D_InitOpening(e_Transit_HowToPlay);
		}
		else
		// "Mission x start" a afficher ? (En début de mission).
		if (gMissionTb[gGameVar.nGenLevel].nMissionNo != -1)
		{
			gGameVar.nMissionNo = gMissionTb[gGameVar.nGenLevel].nMissionNo;
			Transit2D_InitOpening(e_Transit_Mission);	// Ouverture début de mission.
			gGameVar.nPhase = e_Game_MissionStart;		// Affichage du mission start.
			MSE_MissionStartReset(0);
		}
		else
		{
//			Transit2D_InitOpening(e_Transit_InterLvl_H);	// Ouverture inter level.
			Transit2D_InitOpening(gMissionTb[gGameVar.nGenLevel].nVehicleType == e_HeroVehicle_Rocket ? e_Transit_InterLvl_V : e_Transit_InterLvl_H);	// Ouverture inter level.
			Player_LvlDataRestore();	// Restore les variables du joueur.
		}
		break;

	case e_Game_MissionStart:	// "Mission x Start".
		if (MSE_MissionStartDisplay())
			gGameVar.nPhase = e_Game_Normal;	// Passage en phase normale une fois l'affichage terminé.
		// !! Pas de break !!
	case e_Game_Normal:			// Jeu.

/*
#ifdef DEBUG_KEYS
if (gVar.pKeys[SDLK_F6])
{
	gpMstQuestItems[MST_QUEST_ITEM_NEXT_LEVEL] |= 1;	// Item de quête : Fin de niveau.
	gVar.pKeys[SDLK_F6] = 0;
}
#endif
*/
		// Cheat : Next level ?
		if (gCCodes.nCheat & e_Cheat_LevelNext)
		if (gVar.pKeys[SDLK_F6])
		{
			gpMstQuestItems[MST_QUEST_ITEM_NEXT_LEVEL] |= 1;	// Item de quête : Fin de niveau.
			gVar.pKeys[SDLK_F6] = 0;
		}

		// Fin de niveau ?
		if (gpMstQuestItems[MST_QUEST_ITEM_NEXT_LEVEL] & 1)
		{
			// Mission n° == 0 => Cas spéciaux du how to play et des crédits.
			if (gMissionTb[gGameVar.nGenLevel].nMissionNo == 0)
			{
				Transit2D_InitClosing(e_Transit_HowToPlay);
				gGameVar.nPhase = e_Game_LevelCompleted;
			}
			else
			if (gMissionTb[gGameVar.nGenLevel + 1].nMissionNo != -1)
			{
				// Fin de mission.
				gpMstQuestItems[MST_QUEST_ITEM_NEXT_LEVEL] |= 2;	// Signal pour que le héros passe en victory (s'il n'y est pas déjà).
				MSE_MissionStartReset(1);	// Init du "Mission x Complete".
				MSE_EndMissionStatusReset();
				gGameVar.nPhase = e_Game_MissionEnd;
				Music_Start(e_YmMusic_MissionComplete, 1);
			}
			else
			{
				Transit2D_InitClosing(gShoot.nVehicleType == e_HeroVehicle_Rocket ? e_Transit_InterLvl_V : e_Transit_InterLvl_H);	// Fermeture inter level.
				Player_LvlDataSave();	// Sauvegarde les variables du joueur.
				gGameVar.nPhase = e_Game_LevelCompleted;
			}
		}
		break;

	case e_Game_PlayerDead:		// Joueur mort, sélection du Game Over ou du Continue.
		// Meilleur score de tous les crédits.
		if (gShoot.nPlayerScore > gGameVar.nBestScore) gGameVar.nBestScore = gShoot.nPlayerScore;
		//
		if (gGameVar.nCreditsNb)
		{
			// Phase "continue ?".
			MSE_ContinueReset();
			gGameVar.nPhase = e_Game_Continue;
		}
		else
		{
			// Game Over.
			gGameVar.nPhase = e_Game_GameOver;
			Transit2D_InitClosing(e_Transit_Mission);
		}
		break;

	case e_Game_Continue:			// Continue ?
		// "Continue?"
		if (MSE_ContinueDisplay())
		{
			// Game Over.
			gGameVar.nPhase = e_Game_GameOver;
			Transit2D_InitClosing(e_Transit_Mission);
			break;
		}

		// Bouton A (restart) ? Bouton B (accélère le countdown) ?
		if (MSE_ContinueTxtInPos())		// Seulement une fois le texte en place !
		{
			if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]])
			{
				// Credit -- / Continue ++.
				gGameVar.nContinueUsed++;
				if (gGameVar.nCreditsNb > 0) gGameVar.nCreditsNb--;
				// Re-init joueur.
				Player_InitCredit();
				u32	nPlayerLastDir = gShoot.nPlayerDir;
				Player_InitLife(gShoot.nPlayerPosX, gShoot.nPlayerPosY, gShoot.nVehicleType);
				Player_ReinitFlipX(nPlayerLastDir);
				gShoot.nInvulnerability = PLYR_INVULNERABILTY_CNT;	// Petit temps d'invulnérabilité.
				// Phase.
				gGameVar.nPhase = e_Game_Normal;
	//			gVar.pKeys[KB_BUTTONA] = 0;
				gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] = 0;
				break;
			}
			else
			if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonB]])
			{
				MSE_ContinueCountDown_Faster();
				gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonB]] = 0;
				break;
			}
		}
		break;

	case e_Game_Aborted:			// Partie abandonnée (Inactivité).
		gShoot.nInvulnerability = PLYR_INVL_BLINK - 1;		// On ajoute l'invulnérabilité. Pendant la transition, il ne faut pas que le joueur meure (pas testé, mais ça pourrait foutre la merde).
	case e_Game_GameOver:			// Game over.
		Gen_KbNoControl();						// Coupe le contrôle du joueur. (Pour la sortie sur inactivité).
		if (Transit2D_CheckEnd())
		{
			LevelRelease();
			gGameVar.nExitCode = gGameVar.nPhase;	// Code de sortie (Game Over ou Aborted).
			nRet = 1;	// Plus rien de chargé, il ne faut pas continuer dans PlatformerGame().
		}
		break;

	case e_Game_MissionEnd:			// Fin de mission.
		Gen_KbNoControl();						// Coupe le contrôle du joueur.
		if (MSE_EndMissionStatusDisplay())		// Affichage des bonus de fin de mission.
		if (MSE_MissionStartDisplay())			// Affichage "Mission Complete".
		if (Music_IsOver())						// Attente de la fin du jingle de fin de mission.
		{
			Transit2D_InitClosing(e_Transit_Mission);		// Fermeture de mission.
			gGameVar.nPhase = e_Game_MissionEnd_2;
		}

/*
// test pour victory : ok, on peut faire 'l' et attendre un appui sur 'a'.
//		if (gVar.pKeys[KB_BUTTONA] || gVar.pKeys[KB_BUTTONB] || gVar.pKeys[KB_BUTTONC])
		if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] || gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonB]] || gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonC]])
		{
			Transit2D_InitClosing(e_Transit_Mission);		// Fermeture de mission.
			gGameVar.nPhase = e_Game_LevelCompleted;
		}
*/
		break;

	case e_Game_MissionEnd_2:		// Fin de mission, fermeture en laissant l'affichage du "mission complete".
		if (Transit2D_CheckEnd() == 0)			// 'if' en plus pour éviter le dernier affichage quand on doit quitter (sinon ça place les sprites dans la file, on quitte, et les sprites sont affichés sur la première frame du Game Over).
			MSE_MissionStartDisplay();			// Affichage "Mission Complete".
		// Pas de break !
	case e_Game_LevelCompleted:		// Niveau terminé.
		Gen_KbNoControl();						// Coupe le contrôle du joueur.
		if (Transit2D_CheckEnd() == 0) break;	// Transition en cours ?

		// Transition terminée, on passe à la suite.
		LevelRelease();
		if (gMissionTb[++gGameVar.nGenLevel].nLevelNo != -1)
		{
			// Niveau suivant.
			gGameVar.nLevel = gMissionTb[gGameVar.nGenLevel].nLevelNo;
			gGameVar.nPhase = e_Game_LoadLevel;
		}
		else
		{
			// Jeu terminé. Sortie.
			gGameVar.nExitCode = e_Game_AllClear;	// Code de sortie.
			// Meilleur score de tous les crédits.
			if (gShoot.nPlayerScore > gGameVar.nBestScore) gGameVar.nBestScore = gShoot.nPlayerScore;
		}
		nRet = 1;	// Plus rien de chargé, il ne faut plus rien faire avant qu'un niveau soit lu !
		break;

	}

	return (nRet);

}

// +1 vie à certains scores.
#define	HUD_PLYR_1UP_BLINK	64
#define	SC_EVERY	5000	//10000
void CheckSpecialScore(void)
{
	static u32	nNextScore = SC_EVERY;

	if (gShoot.nPlayerScore < SC_EVERY) nNextScore = SC_EVERY;	// Re-init.

	// 1 vie tous les x points.
	if (gShoot.nPlayerScore >= nNextScore)
	{
		// 1Up.
		if (gShoot.nPlayerLives < PLAYER_Lives_Max) gShoot.nPlayerLives++;
		gShoot.nHUDPlayerLivesBlink = HUD_PLYR_1UP_BLINK;		// On met le clignotement et le son même si max.
		Sfx_PlaySfx(e_Sfx_Fx_GunReload, e_SfxPrio_0 + 5);	// Sfx.
		// Le score suivant.
		nNextScore += SC_EVERY;
	}

}

#define	INACTIVITY_MIN			(FRM_1S * 10)
#define	INACTIVITY_CNTSTART		(INACTIVITY_MIN + (FRM_1S * 10))
#define	INACTIVITY_CNTEND		(INACTIVITY_CNTSTART + (FRM_1S * 10) - 1)

u32 MSE_sub_CalcTxtPos(char *pStr, s16 *pPosXTb, u32 *pSprTb, u8 *pChrNb);

#define	INACTIVITY_TXTSZMAX	4
struct SInactivityWrt
{
// !!! Ne pas changer les tailles des données, ça matche avec MSE_sub_CalcTxtPos() !!!
	s16	pnPosXTb[INACTIVITY_TXTSZMAX];
	u32	pnSprTb[INACTIVITY_TXTSZMAX];
	u8	nChrNb;
};
struct SInactivityWrt	gInactivityWrt;

// Initialisation du "writer" (pour ne pas calculer les positions des lettres de la grosse fonte à chaque fois).
void Inactivity_WriterInit(void)
{
	u32	i;
	static char	*pStrGo = "GO!";
	u32	nLg;

	assert(strlen(pStrGo) <= INACTIVITY_TXTSZMAX);

	// Nos des sprites, position des lettres.
	nLg = MSE_sub_CalcTxtPos(pStrGo, gInactivityWrt.pnPosXTb, gInactivityWrt.pnSprTb, &gInactivityWrt.nChrNb);
	// Centrage x des lettres.
	for (i = 0; i < gInactivityWrt.nChrNb; i++)
		gInactivityWrt.pnPosXTb[i] += (SCR_Width - nLg) / 2;
}

// Gestion de l'inactivité.
void Inactivity(void)
{
	u32	i;

/*
//>tst
char	pTb[8+1] = "00000000";
MyItoA(gShoot.nInactivityCnt, pTb);
Font_Print(10, 160, pTb, 0);
//Font_Print(10,108, (gShoot.nVehicleAutoPilot == 0 ? "0":"1"), 0);
//<tst
*/

	if (gGameVar.nPhase == e_Game_Normal)
	if (gShoot.nVehicleType != e_HeroVehicle_HelicopterNoExit && gShoot.nVehicleType != e_HeroVehicle_Rocket)
	if (gShoot.nDeathFlag == 0)
	if (gShoot.nVehicleAutoPilot == 0)
	if (gpMstQuestItems[MST_QUEST_ITEM_NEXT_LEVEL] == 0)
	{
		// Des touches en cours ?
//		for (i = 0; i < e_CfgKey_MAX; i++)
		for (i = 0; i < e_CfgKey_LAST; i++)
			if (gVar.pKeysSDL[gMSCfg.pKeys[i]]) goto _EndRAZ;

		// Abort ? (Compteur terminé).
		if (gShoot.nInactivityCnt >= INACTIVITY_CNTEND)
		{
			gGameVar.nPhase = e_Game_Aborted;
			Transit2D_InitClosing(e_Transit_Mission);
			return;
		}

		// Avance compteur.
		gShoot.nInactivityCnt++;
		// "Go!" si compteur assez loin.
		if (gShoot.nInactivityCnt >= INACTIVITY_MIN)
		{
			s32	nOffs;
			u32	nZoom;
			u32	nSprFlag = (gShoot.nInactivityCnt & 8 ? SPR_Flag_HitPal : 0);

			nOffs = (INACTIVITY_MIN + 8) - gShoot.nInactivityCnt;	// Petit truc pour l'arrivée en zoomant.
			if (nOffs < 0) nOffs = 0;
			nZoom = 0x100 - (nOffs * 32);
			nOffs *= nOffs;

			for (i = 0; i < gInactivityWrt.nChrNb; i++)
//				SprDisplayAbsolute(gInactivityWrt.pnSprTb[i] | (gShoot.nInactivityCnt & 8 ? SPR_Flag_HitPal : 0), gInactivityWrt.pnPosXTb[i] - nOffs, (SCR_Height / 2) - 4, 0x100 + e_Prio_HUD);
				SprDisplayZoomAbsolute(gInactivityWrt.pnSprTb[i] | nSprFlag, gInactivityWrt.pnPosXTb[i] - nOffs, (SCR_Height / 2) - 4   - 12 + ((24 * nZoom) / 512), 0x100 + e_Prio_HUD, 0x100, nZoom);
		}
		// Le countdown sur la fin.
		if (gShoot.nInactivityCnt >= INACTIVITY_CNTSTART)
		{
			struct SSprite	*pSpr;

			i = (INACTIVITY_CNTEND - gShoot.nInactivityCnt) / FRM_1S;
			pSpr = SprGetDesc(e_Spr_FontMission_Figures + i);
			SprDisplayAbsolute(e_Spr_FontMission_Figures + i, (SCR_Width / 2) - (pSpr->nLg / 2), (SCR_Height / 2) + 24+4, 0x100 + e_Prio_HUD);
		}

		return;
	}

_EndRAZ:
	gShoot.nInactivityCnt = 0;		// RAZ compteur d'inactivité.

}


// Platformer.
void PlatformerGame(void)
{
	u32	i;

	if (ShootGame()) return;		// !!! Ne pas déplacer !!! (Load level fait dedans !).

	// Deux direction opposées à la fois ? => Clear.
//	if (gVar.pKeys[SDLK_UP] && gVar.pKeys[SDLK_DOWN]) gVar.pKeys[SDLK_UP] = gVar.pKeys[SDLK_DOWN] = 0;
//	if (gVar.pKeys[SDLK_LEFT] && gVar.pKeys[SDLK_RIGHT]) gVar.pKeys[SDLK_LEFT] = gVar.pKeys[SDLK_RIGHT] = 0;
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Up]] && gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]]) gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Up]] = gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Down]] = 0;
	if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Left]] && gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Right]]) gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Left]] = gVar.pKeys[gMSCfg.pKeys[e_CfgKey_Right]] = 0;

	// Contrôle du joueur.
	gpFctCtrlTb[gShoot.nVehicleType]();

	ScrollManage();
	AnmBlkManage();

	FireManage();
	FireDestructibleCheck();
	ChaserTarget_ClearList();		// Cleare les cibles des homing missiles.
	MstManage();

	Inactivity();

	// Affichage du joueur.
	gpFctDispTb[gShoot.nVehicleType]();		// APRES les monstres, car les pf mobiles peuvent déplacer le joueur.
	Gen_KbRestore();	// Après la gestion et l'affichage, on restore systématiquement le clavier ! Il sera cleané à nouveau dans les cas spéciaux.
	// Traitement des variables spéciales.
	gShoot.nMstProximity = 0;				// RAZ du flag de proximité.
	if (gShoot.nInvulnerability) gShoot.nInvulnerability--;		// Décrémentation invulnérabilité.
	if (gShoot.nVehicleHitCnt) gShoot.nVehicleHitCnt--;			// Décrémentation compteur de hit.

	//>> Cheat : Invulnérabilité.
	if (gCCodes.nCheat & e_Cheat_Invulnerability)
		gShoot.nInvulnerability = PLYR_INVL_BLINK - 1;	// Invulnérable.
	//<< Cheat : Invulnérabilité.

	//>> Cheat : Munitions ilimitées.
	if (gCCodes.nCheat & e_Cheat_WeaponUnlimited)
	{
		Player_WeaponReload(0);
		if (gShoot.nVehicleType == e_HeroVehicle_Rocket) gShoot.nVehicleAmmo = gpnArmsLoadMax[gShoot.nRocketWeapon];
		gShoot.nBombAmmo = PLYR_NAKED_BOMBS_DEFAULT;
		gShoot.nVehicleBombAmmo = SLUG_BOMBS_DEFAULT;
	}
	//<< Cheat : Munitions ilimitées.

	CheckSpecialScore();	// +1 vie tous les x points.

	DustManage();

	if (gMissionTb[gGameVar.nGenLevel].nMissionNo != 0)	// Mission n° == 0 => Cas spéciaux du how to play et des crédits. Pas de HUD.
		HUDDisplay();	// Affichage du HUD.

	// Affichage du décor, plans derrière des sprites.
	for (i = 0; i <= gMap.nHeroPlane; i++) ScrollDisplayPlane(i);
	// Les sprites sous l'avant plan (quasiment tout).
	SprDisplayAll_Pass1();
	// Affichage du décor, plans devant les sprites.
	for (i = gMap.nHeroPlane + 1; i < gMap.nPlanesNb; i++) ScrollDisplayPlane(i);
	// Les sprites au dessus de l'avant-plan (hud, ...).
	SprDisplayAll_Pass2();

}



