

#define	FRM_1S	60		// 1 seconde, en nombre de frames.

#define	SLUG_ENERGY_MAX	48	//192
#define	SLUG_BOMBS_DEFAULT	10//50
#define	SLUG_BOMBS_MAX		25//75

// Offsets de départ dans la table des missions.
#define	MISSIONOFFS_HOWTOPLAY	0
#define	MISSIONOFFS_CREDITS		2
#define	MISSIONOFFS_LEVELS		4

// Phases de jeu.
enum
{
	e_Game_LoadLevel = 0,		// Load level.
	e_Game_MissionStart,		// Mission start.
	e_Game_MissionEnd,			// Mission end.
	e_Game_MissionEnd_2,		// Mission end (phase de transition).
	e_Game_Normal,				// Jeu.
	e_Game_LevelCompleted,		// Niveau terminé.
	e_Game_PlayerDead,			// Joueur mort, sélection du Game Over ou du Continue.
	e_Game_Continue,			// Continue ?
	e_Game_GameOver,			// Game over.		/ Code aussi utilisé comme code de sortie.
	e_Game_AllClear,			// Jeu terminé.		/ Code seulement utilisé comme code de sortie.
	e_Game_Aborted,				// Jeu abandonné.	/ Code aussi utilisé comme code de sortie.

};

// Armes du joueur.
enum
{
	e_Player_Weapon_Gun = 0,
	e_Player_Weapon_Shotgun,
	e_Player_Weapon_Machinegun,
	e_Player_Weapon_Flamethrower,
	e_Player_Weapon_Rocket,

	e_Player_Weapon_Max
};

// Véhicule.
enum
{
	e_HeroVehicle_None_Naked = 0,
	e_HeroVehicle_None_Swim,
	e_HeroVehicle_None_Parachute,
	e_HeroVehicle_None_SlugNoExit_Temp,

	e_HeroVehicle_SlugBase,		// Valeur de base pour les slugs.
	e_HeroVehicle_Submarine = e_HeroVehicle_SlugBase,
	e_HeroVehicle_Tank,
	e_HeroVehicle_Helicopter,
	e_HeroVehicle_HelicopterNoExit,
	e_HeroVehicle_Rocket,

	e_HeroVehicle_MAX
};

struct SPlayer
{
	s32	nPlayerPosX, nPlayerPosY;
	s32	nPlayerSpdX, nPlayerSpdY;

	s32	nPlayerAnm;		// Anim du bas.
	s32	nPlayerAnmTop;	// Anim du haut pour cas spéciaux (tirs...).
	u32	nPlayerSprCol;	// Sprite de col du héros.
	u32	nPlayerDir;		// 0 = droite / 1 = gauche.
	u32	nPlayerGnd;		// 1 = Le joueur est en contact avec le sol.
	u32 nPlayerLastGnd;
	s32	nPlayerLastGndPosY;		// Dernière position au sol (pour scroll).
	s8	nPlayerGndAng;	// Angle au niveau du sol.

	u32	nWeapon;		// Arme en cours.
	s32	nAmmo;			// Munitions pour l'arme de poing en cours.
	u32	nBombAmmo;		// Nombre de grenades.

	u32	nPfAddress;				// Pour plateformes mobiles.

	u32	nPlayerKnifeSprCol;		// Sprite en cours quand coup de couteau.
	u32	nMstProximity;			// Mst à proximité (màj par monstres) pour déclenchement du coup de couteau.

	s8	nParachuteAnm;			// Anim pour le parachute.
	u8	nInvulnerability;		// Compteur d'invulnérabilité. 0 = Pas invulnérable.
	u16	nBoredCnt;				// Compteur pour les anims d'ennui.

	u8	nDeathFlag;			// Flag pour INDIQUER que le joueur est mort (et pas pour l'y placer !).

	// Special Slug.
	u32	nVehicleType;
	u32	nVehicleNoExitTypeToReset;	// Pour les véhicules qu'on ne peut pas quitter, type du slug à rétablir.
	u8	nVehicleEnergy;
	u32	nVehiclePhase;

	s32	nVehicleAmmo;			// Munitions pour l'arme en cours.
	u32	nVehicleBombAmmo;		// Nombre de bombes.

	u32	nVehicleCurAdd;
	u32	nVehicleReqAdd;
	s32	nVehicleOffsY;
	u8	nVehicleFrmCnt0;
	u8	nEngineCnt;			// Variable pour le bruit de moteur.

	s32	nVehicleAnim;
	s32	nPlayerAnimInOut;

	u8	nVehicleHitCnt;		// Compteur de hit pour faire clignoter les slugs quand touché.

	u8	nVehicleAutoPilot;
	u16	nVehicleAutoPilotLastBlkX, nVehicleAutoPilotLastBlkY;

	// Vulcan cannon.
	s32	nVulcanAnm;			// Anim du canon.
	u8	nVulcanAngle;		// Angle du canon.
	u8	nVulcanReqAngle;	// Angle demandé.

	// Spécial fusée.
	s8	nRocketMainThrusterAnm;
	s8	pnRocketSideWeaponsAnmLR[2];
	s8	nRocketBodyWeaponSparkAnm;
	s8	pnRocketSideWeaponsSparkAnmLR[2];
	s8	pnRocketThrustersAnm[4];

	u8	nRocketWeapon;		// Fusée : Arme en cours.

	//

	s32	nSplashLevel;		// Niveau de l'eau. -1 = Pas initialisé.

	u32	nFreedPrisoners;	// Nb de prisonniers libérés.

	u8	nPlayerLives;		// Nb de vies.
	u8	nHUDPlayerLivesBlink;	// Pour clignotement des vies dans le HUD lors des 1UP.

	u32	nPlayerScore;		// Score.

	u32	nInactivityCnt;		// Compteur d'inactivité (général).

};

extern struct SPlayer	gShoot;

//===

struct SGameVar
{
	u32	nExitCode;		// Pour sortie du jeu. Tjs à 0, sauf pour sortie.

	u32 nGenLevel;		// Index dans gMissionTb.
	s32	nLevel;			// Level en cours.
	u32	nPhase;			// Phase de jeu (init, jeu, game over...).

	u32	nMissionNo;		// N° de la mission pour affichage & high-score.
	u32	nContinueUsed;	// 0 si pas de continue utilisé.
	s32	nCreditsNb;		// Nb de crédits.

	u32	nBestScore;		// Meilleur score de tous les crédits.

};

extern struct SGameVar	gGameVar;

//===

enum
{
	e_KbDir_Up = 1,
	e_KbDir_Right = 1 << 1,
	e_KbDir_Down = 1 << 2,
	e_KbDir_Left = 1 << 3,

	e_KbDir_ButtonA = 1 << 4,
	e_KbDir_ButtonB = 1 << 5,
	e_KbDir_ButtonC = 1 << 6,

	// Note : Pour l'autopilot, le bit 7 sert pour test des directions auto ou pas.

};

//===

void ExgPlatformerInit(s32 nCreditsNb, u32 nMissionTbOffset);
void PlatformerGame(void);

void Player_Control(void);
void Player_WeaponSet(u32 nWeaponNo);
void Player_VehicleSet(u32 nVehicle, u32 nPosX, u32 nPosY, u32 nEnergy, u32 nVulcanAngle, u32 nBombs);
void Player_WeaponReload(u32 nBombsAdd);
void Player_SlugRefuel(u32 nEnergy);
u32 Enemy_PlayerBlowRect(struct SSprRect *pRect1, s32 nPosX, s32 nPosY, u32 nDamage);



