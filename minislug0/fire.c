// Gestion des tirs.

#include "includes.h"


// Dégats des tirs.
enum
{
	e_ShotDamage_Player_Gun = 1,
	e_ShotDamage_Player_Shotgun = 7,
	e_ShotDamage_Player_Machinegun = 1,
	e_ShotDamage_Player_Flamethrower = 1,
	e_ShotDamage_Player_Grenade = 10,
	e_ShotDamage_Player_Vulcan = 2,

	e_ShotDamage_Enemy_Bullet = MST_DAMAGE_BULLET,
	e_ShotDamage_Enemy_Explo = MST_DAMAGE_EXPLO,
};

enum
{
	e_ShotFlag_ChkGnd = 1 << 0,		// Teste le "dur".
	e_ShotFlag_Remain = 1 << 1,		// Le tir ne disparait pas lors d'un touché d'ennemi (explosions).
	e_ShotFlag_NoClip = 1 << 2,		// Pas de clip avec le scroll (grenades, explosions).
	e_ShotFlag_Chaser = 1 << 3,		// Homing missile.
	e_ShotFlag_RotoZoom = 1 << 4,	// Affichage en roto zoom.
	e_ShotFlag_Destructible = 1 << 5,	// Tir destructible en tirant dessus.
	e_ShotFlag_CancelNoCtc = 1 << 6,	// Annule le NoCtc => Un tir avec 'Remain' touchera tout le temps.
};

// Définition des tirs.
struct SFireRecord
{
	u64	*pAnm;
	u64	*pAnmDust;	// Anim de disparition. NULL si pas utilisée.
	u64	*pTrailDust;	// Dust pour la trainée. NULL si pas utilisée.
	s16	nSpeed;		// + b15 pour multiplier par 2 la vitesse Y.
	u8	nAngle;

	u8	nPlyr;		// 1 = Tir du joueur, 0 = Tir d'un monstre.
	u8	nGravity;	// 0 = Sans. x : x << 4 = vitesse de chute max.
	u8	nDamagePts;		// Points de dégats.
	u8	nDamageType;	// Type du dégat.
	s8	nNextShot;	// N° du tir généré (grenades, etc... qui vont faire une explosion). -1 si pas de tir à générer.
	u8	nDiviser;	// 0 si pas de diviseur. Sinon décalage de 256 pour add sur le sprite.
	u8	nPrio;		// 0 pour priorité normale, sinon valeur de la prio.

	u8	nFlags;

};
struct SFireRecord	gpFireTable[] =
{
	{ gAnm_ShotHero_PistolH, gAnm_ShotHero_PistolH_Dust, NULL, 0x800, 0,
		e_ShotOrg_Player, 0, e_ShotDamage_Player_Gun, e_ShotDamageType_Gen, -1, 0, 0,
		e_ShotFlag_ChkGnd },		// Joueur: Pistolet, tir H.
	{ gAnm_ShotHero_PistolV, gAnm_ShotHero_PistolV_Dust, NULL, 0x800, 64,
		e_ShotOrg_Player, 0, e_ShotDamage_Player_Gun, e_ShotDamageType_Gen, -1, 0, 0,
		e_ShotFlag_ChkGnd },		// Joueur: Pistolet, tir V.
	{ gAnm_ShotHero_ShotgunH, NULL, NULL, 0x180, 0,
		e_ShotOrg_Player, 0, e_ShotDamage_Player_Shotgun, e_ShotDamageType_Blow, -1, 0, 0,
		e_ShotFlag_Remain },		// Joueur: Shotgun, tir H.
	{ gAnm_ShotHero_ShotgunV, NULL, NULL, 0x180, 64,
		e_ShotOrg_Player, 0, e_ShotDamage_Player_Shotgun, e_ShotDamageType_Blow, -1, 0, 0,
		e_ShotFlag_Remain },		// Joueur: Shotgun, tir V.
	{ gAnm_ShotHero_MachinegunH, gAnm_ShotHero_PistolH_Dust, NULL, 0x800, 0,
		e_ShotOrg_Player, 0, e_ShotDamage_Player_Machinegun, e_ShotDamageType_Gen, -1, 0, 0,
		e_ShotFlag_ChkGnd },		// Joueur: Mitrailleuse, tir H.
	{ gAnm_ShotHero_MachinegunV, gAnm_ShotHero_PistolV_Dust, NULL, 0x800, 64,
		e_ShotOrg_Player, 0, e_ShotDamage_Player_Machinegun, e_ShotDamageType_Gen, -1, 0, 0,
		e_ShotFlag_ChkGnd },		// Joueur: Mitrailleuse, tir V.
	{ gAnm_ShotHero_FlameH, gAnm_ShotHero_FlameH_Dust, NULL, 0x180, 0,
		e_ShotOrg_Player, 0, e_ShotDamage_Player_Flamethrower, e_ShotDamageType_Flame, -1, 0, 0,
		0 },		// Joueur: Flamethrower, tir H.
	{ gAnm_ShotHero_FlameV, gAnm_ShotHero_FlameV_Dust, NULL, 0x180, 64,
		e_ShotOrg_Player, 0, e_ShotDamage_Player_Flamethrower, e_ShotDamageType_Flame, -1, 0, 0,
		0 },		// Joueur: Flamethrower, tir V.
	{ gAnm_ShotHero_HandGrenade_Air, NULL, NULL, 0x400, 0,
		e_ShotOrg_Player, (SPDY_MAX >> 4), 0, 0, e_Shot_Player_HandGrenade_Explo, 0, 0,
		e_ShotFlag_ChkGnd | e_ShotFlag_NoClip },		// Joueur: Grenade - en vol.
	{ gAnm_ShotHero_HandGrenade_Explo, NULL, NULL, 0, 0,
		e_ShotOrg_Player, 0, e_ShotDamage_Player_Grenade, e_ShotDamageType_Flame, -1, 0, 0,
		e_ShotFlag_Remain | e_ShotFlag_NoClip },		// Joueur: Grenade - explosion.

	{ gAnm_ShotHero_SlugRocket_MachinegunV, gAnm_ShotHero_PistolV_Dust, NULL, 0x800, 64,
		e_ShotOrg_Player, 0, e_ShotDamage_Player_Machinegun, e_ShotDamageType_Gen, -1, 0, 0,
		e_ShotFlag_ChkGnd },		// Joueur: Slug roquette, Mitrailleuse, tir V.

	{ gAnm_ShotHero_VulcanCannon, gAnm_ShotHero_VulcanCannon_Dust, NULL, 0x800, 0,
		e_ShotOrg_Player, 0, e_ShotDamage_Player_Vulcan, e_ShotDamageType_Gen, -1, 3, 0,
		e_ShotFlag_ChkGnd },		// Joueur: Vulcan cannon.
	{ gAnm_ShotHero_SubmarineRocket, NULL, gAnm_SubmarineRocket_Trail_Dust, 0x800, 0,
		e_ShotOrg_Player, 0, 0, 0, e_Shot_Player_SlugSubmarine_Rocket_Explo, 0, e_Prio_Joueur + 3,
		e_ShotFlag_ChkGnd | e_ShotFlag_Chaser | e_ShotFlag_RotoZoom },		// Joueur: Slug Submarine - Roquette.
	{ gAnm_ShotEnemy_Explosion0_Medium, NULL, NULL, 0, 0,
		e_ShotOrg_Player, 0, e_ShotDamage_Player_Grenade, e_ShotDamageType_Gen, -1, 0, 0,
		e_ShotFlag_Remain | e_ShotFlag_NoClip },		// Joueur: Slug Submarine - Explosion de la roquette.

	{ gAnm_ShotHero_RocketSmall, NULL, gAnm_RocketSmall_Trail_Dust, 0x600, 0,
		e_ShotOrg_Player, 0, 0, 0, e_Shot_Player_RocketSmall_Explo, 3, e_Prio_Joueur + 3,
		e_ShotFlag_ChkGnd | e_ShotFlag_Chaser },		// Joueur: Roquette small.
	{ gAnm_ShotEnemy_Explosion0_Medium, NULL, NULL, 0, 0,
		e_ShotOrg_Player, 0, e_ShotDamage_Player_Grenade, e_ShotDamageType_Flame, -1, 0, 0,
		e_ShotFlag_Remain | e_ShotFlag_NoClip },		// Joueur: Rocket small - Explosion de la roquette.

	{ gAnm_ShotHero_HelicopterBomb, NULL, NULL, 0, 0,
		e_ShotOrg_Player, (SPDY_MAX >> 4), 0, 0, e_Shot_Player_SlugCopter_Bomb_Explo, 0, 0,
		e_ShotFlag_ChkGnd },		// Joueur: Slug Copter - Bomb.
	{ gAnm_ShotEnemy_Explosion0_Medium, NULL, NULL, 0, 0,
		e_ShotOrg_Player, 0, e_ShotDamage_Player_Grenade, e_ShotDamageType_Flame, -1, 0, 0,
		e_ShotFlag_Remain | e_ShotFlag_NoClip },		// Joueur: Slug Copter - Bomb - Explosion.

	{ gAnm_ShotHero_RocketBomb, NULL, gAnm_RocketSmall_Trail_Dust, 0x600, 64,
		e_ShotOrg_Player, 0, 0, 0, e_Shot_Player_SlugRocket_Rocket_Explo, 0, 0,
		0 },		// Joueur: Roquette de la fusée.
	{ gAnm_ShotEnemy_Explosion0_Medium, NULL, NULL, 0, 0,
		e_ShotOrg_Player, 0, e_ShotDamage_Player_Grenade, e_ShotDamageType_Flame, -1, 0, 0,
		e_ShotFlag_Remain | e_ShotFlag_NoClip },		// Joueur: Slug Rocket - Roquette - Explosion.
	{ gAnm_ShotHero_Explosion0_Medium_LargeTouch, NULL, NULL, 0, 0,
		e_ShotOrg_Player, 0, e_ShotDamage_Player_Grenade, e_ShotDamageType_Flame, -1, 0, 0,
		e_ShotFlag_Remain | e_ShotFlag_NoClip },		// Explosion medium, large zone de touché (explosion tonneaux lev 13 par exemple).

//gAnm_SubmarineBubble_Dust
//{ gAnm_ShotHero_RocketSmall, NULL, gAnm_SubmarineBubble_Dust, 0x300, 0,	// hmmm, vitesse trop lente, ça génère trop de dusts...
{ gAnm_ShotEnemy_LRAC_Rocket, NULL, NULL, 0x300, 0,
	e_ShotOrg_Player, 0, 0, 0, e_Shot_Player_SwimGrenade_Explo, 0, 0,
	e_ShotFlag_ChkGnd | e_ShotFlag_RotoZoom },		// Joueur : Grenade sous-marine.
{ gAnm_ShotHero_Explosion0_Medium_LargeTouch, NULL, NULL, 0, 0,
	e_ShotOrg_Player, 0, e_ShotDamage_Player_Grenade, e_ShotDamageType_Flame, -1, 0, 0,
	e_ShotFlag_Remain | e_ShotFlag_NoClip },		// Joueur : Grenade sous-marine - explosion.
//#define	MST39_OFFSY	14 / décalage des explo des sensor mines.

	{ gAnm_ShotEnemy_ZombieBrain, gAnm_ShotEnemy_ZombieBrain_DustGnd, NULL, 0x200, 0,
		e_ShotOrg_Enemy, (SPDY_MAX >> 4), e_ShotDamage_Enemy_Bullet, e_ShotDamageType_Gen, -1, 0, 0,
		e_ShotFlag_ChkGnd },		// Zombie Brain.
	{ gAnm_ShotEnemy_RShobu_Bomb, NULL, NULL, 0, 192,
		e_ShotOrg_Enemy, (0x300 >> 4), 0, 0, e_Shot_Enemy_RShobu_Bomb_Explo, 0, e_Prio_Ennemies - 1,
		e_ShotFlag_ChkGnd | e_ShotFlag_NoClip },		// R-Shobu - Bombe.
	{ gAnm_ShotEnemy_Explosion0_Medium, NULL, NULL, 0, 0,
		e_ShotOrg_Enemy, 0, e_ShotDamage_Enemy_Explo, e_ShotDamageType_Flame, -1, 0, e_Prio_Joueur + 10,//e_Prio_DustUnder,
		e_ShotFlag_Remain | e_ShotFlag_NoClip },		// R-Shobu - Explosion de la bombe.
	{ gAnm_ShotEnemy_Bullet, gAnm_ShotHero_PistolH_Dust, NULL, 0x400, 0,
		e_ShotOrg_Enemy, 0, e_ShotDamage_Enemy_Bullet, e_ShotDamageType_Gen, -1, 0, 0,
		e_ShotFlag_ChkGnd },		// Rebel soldier : Balle standard.
	{ gAnm_ShotEnemy_MortarRocket, NULL, NULL, 0x300 | (1 << 15), 64,
		e_ShotOrg_Enemy, (SPDY_MAX >> 4), 0, 0, e_Shot_Enemy_RShobu_Bomb_Explo, 0, 0,
		e_ShotFlag_ChkGnd | e_ShotFlag_NoClip },		// Roquette du mortier.
	{ gAnm_ShotEnemy_RocketDiverRocket, NULL, NULL, 0x180, 64,	//spd 0x280
		e_ShotOrg_Enemy, 0, 0, 0, e_Shot_Enemy_RShobu_Bomb_Explo, 0, 0,
		e_ShotFlag_ChkGnd | e_ShotFlag_Chaser | e_ShotFlag_RotoZoom | e_ShotFlag_Destructible | e_ShotFlag_NoClip },	// Roquette du rocket diver.
	{ gAnm_ShotEnemy_Girida0Bullet, NULL, NULL, 0x300 | (1 << 15), 64,
		e_ShotOrg_Enemy, (SPDY_MAX >> 4), 0, 0, e_Shot_Enemy_RShobu_Bomb_Explo, 0, 0,
		e_ShotFlag_ChkGnd | e_ShotFlag_NoClip },		// Boulet du Girida-0.

	{ gAnm_ShotEnemy_Boss1_ParachuteBomb, NULL, NULL, 0, 192,
		e_ShotOrg_Enemy, (0x80 >> 4), 0, 0, e_Shot_Enemy_RShobu_Bomb_Explo, 0, e_Prio_DustOver - 8,
		e_ShotFlag_ChkGnd | e_ShotFlag_NoClip },		// Boss 1 - Parachute bomb.
	{ gAnm_ShotEnemy_Boss1_BulletFront, NULL, NULL, 0x400, 128,
		e_ShotOrg_Enemy, 0, 0, 0, e_Shot_Enemy_RShobu_Bomb_Explo, 0, 0,
		0 },		// Boss 1 - Obus frontal.

	{ gAnm_ShotEnemy_StdHandgrenade, NULL, NULL, 0x400, 64,
		e_ShotOrg_Enemy, (SPDY_MAX >> 4), 0, 0, e_Shot_Enemy_RShobu_Bomb_Explo, 0, 0,
		e_ShotFlag_ChkGnd | e_ShotFlag_NoClip },		// Rebel soldier : Grenade standard.
	{ gAnm_ShotEnemy_LRAC_Rocket, NULL, NULL /*gAnm_RocketSmall_Trail_Dust*/, 0x400, 0,
		e_ShotOrg_Enemy, 0, 0, 0, e_Shot_Enemy_RShobu_Bomb_Explo, 0, 0,
		e_ShotFlag_ChkGnd | e_ShotFlag_RotoZoom },		// Rebel soldier : Roquette du LRAC.
	{ gAnm_ShotEnemy_MarsUFO_GreenBullet0, gAnm_ShotEnemy_MarsUFO_GreenBullet0_Dust, NULL, 0x400, 0,
		e_ShotOrg_Enemy, 0, e_ShotDamage_Enemy_Bullet, e_ShotDamageType_Gen, -1, 0, 0,
		e_ShotFlag_ChkGnd },		// Mars TIE/Eye/UFO : Green bullet.
	{ gAnm_Shot_Lev13_HangingRock_Fall, NULL, NULL, 0, 0,
		e_ShotOrg_Enemy, 0, SLUG_ENERGY_MAX, e_ShotDamageType_Gen, -1, 0, e_Prio_Ennemies - 2,//e_Prio_DustUnder,
		e_ShotFlag_Remain | e_ShotFlag_CancelNoCtc | e_ShotFlag_NoClip },		// Falling Rock.
	{ gAnm_ShotEnemy_HairBusterRibert_Mine, NULL, NULL, 0x300, 0,
		e_ShotOrg_Enemy, 0, 0, 0, e_Shot_Enemy_RShobu_Bomb_Explo, 0, e_Prio_DustOver - 2,	// Prio pour Boss 2B.
		0 },		// Hair Buster Ribert - Mine.
	{ gAnm_CoreCannon_Bullet, gAnm_CoreCannon_BulletSparkDust, NULL, 0x400, 192,
		e_ShotOrg_Enemy, 0, e_ShotDamage_Enemy_Bullet, e_ShotDamageType_Gen, -1, 0, e_Prio_Ennemies,
		e_ShotFlag_ChkGnd },		// Boss 3 - Core - Bullet.
	{ gAnm_ShotEnemy_BrainBot_RingLow, NULL, NULL, 0x280, 0,
		e_ShotOrg_Enemy, 0, e_ShotDamage_Enemy_Bullet, e_ShotDamageType_Gen, -1, 0, 0,
		e_ShotFlag_Remain | e_ShotFlag_CancelNoCtc | e_ShotFlag_NoClip },		// Brain Bot : Ring partie basse.
	{ gAnm_ShotEnemy_BrainBot_RingHigh, NULL, NULL, 0x280, 0,
		e_ShotOrg_Enemy, 0, e_ShotDamage_Enemy_Bullet, e_ShotDamageType_Gen, -1, 0, e_Prio_Joueur + 10,
		e_ShotFlag_Remain | e_ShotFlag_CancelNoCtc | e_ShotFlag_NoClip },		// Brain Bot : Ring partie haute.

};



// Structure d'un slot du tableau gpFireSlots.
struct SFire
{
	u8	nUsed;			// 0 = slot vide, 1 = slot occupé.

	u8	nAngle;
	u8	nTbIdx;			// Index dans gpFireTable == n° du tir.
	u8	nPlyr;			// On le garde pour éviter la triple indirection dans le FireHitCheck.
	s32	nAnm;			// Anim.
	s32	nPosX, nPosY;	// 8b de virgule fixe.
	s16	nSpeed, nSpeedY;	// nSpeed + nAngle / nSpeedY sur y si nGravity.
	struct SSprRect sColRect;	// Rectangle de col, si type == e_SprRect_Rect.
	u8	nTrailCnt;		// Pour trainée.
	u8	nNoMoreCtc;		// Pour couper les tests sur le tir à la frame suivante (on change la valeur de nPlyr).
};

#define	FIRE_MAX_SLOTS	64
struct SFire	gpFireSlots[FIRE_MAX_SLOTS];
u32	gnFireLastUsed;

u8 ChaserTarget_AcquireTarget(struct SFire *pFire);


// RAZ moteur.
void FireInitEngine(void)
{
	u32	i;

	// RAZ de tous les slots.
	for(i = 0; i < FIRE_MAX_SLOTS; i++)
	{
		gpFireSlots[i].nUsed = 0;
	}
	gnFireLastUsed = 0;

	ChaserTarget_ClearList();		// Cleare les cibles des homing missiles.

}

// Renvoie un ptr sur la vitesse d'un tir pour ajustement après un FireAdd().
s16 * FireGetSpdXPtr(u32 nSlotNo)
{
	return (&gpFireSlots[nSlotNo].nSpeed);
}

// Cherche un slot libre.
// Out : N° d'un slot libre. -1 si erreur.
s32 FireGetSlot(void)
{
	u32	i;

	for (i = gnFireLastUsed; i < FIRE_MAX_SLOTS; i++)
	{
		if (gpFireSlots[i].nUsed == 0)
		{
			gpFireSlots[i].nUsed = 1;
			gnFireLastUsed = i + 1;		// La recherche commencera au suivant.
			return (i);
		}
	}
	return (-1);
}

// Libère un slot.
void FireReleaseSlot(u32 nSlotNo)
{
	// Libère l'anim.
	if (gpFireSlots[nSlotNo].nAnm != -1)
		AnmReleaseSlot(gpFireSlots[nSlotNo].nAnm);
	// Pour accélérer la recherche des slots libres.
	if (nSlotNo < gnFireLastUsed)
	{
		gnFireLastUsed = nSlotNo;
	}
	gpFireSlots[nSlotNo].nUsed = 0;

}
// Idem avec chain reaction.
s32 FireReleaseSlotChain(u32 nSlotNo)
{
	s32	nLinkSlot = -1;

	// Libère le slot.
	FireReleaseSlot(nSlotNo);	// => A ce point, toutes les valeurs dans le slot restent valides !
	// Génère un autre tir si nécessaire.
	if (gpFireTable[gpFireSlots[nSlotNo].nTbIdx].nNextShot != -1)
	{
		nLinkSlot = FireAdd(gpFireTable[gpFireSlots[nSlotNo].nTbIdx].nNextShot, gpFireSlots[nSlotNo].nPosX, gpFireSlots[nSlotNo].nPosY, 0);	//gpFireSlots[nSlotNo].nAngle);	// Angle juste pour la direction => les explosions ne doivent pas clipper.
		// !!! Attention, on doit sauter une frame d'affichage !!!
			// Le nouveau tir doit reprendre le slot (pas sûr => s'il y a un trou avant, non) et skipper l'affichage (et tant mieux).
	}
	return (nLinkSlot);
}

// Init d'un tir.
// In : sAngle = -1 => On prend l'angle par défaut. Sinon val [0;255] => Angle.
// Out : N° du slot. -1 si erreur.
s32 FireAdd(u32 nShot, s32 nPosX, s32 nPosY, s32 nAngle)
{
	s32	nSlotNo;
	s16	nSpeed, nSpeedY;

	if ((nSlotNo = FireGetSlot()) == -1) //return (-1);
	{
		fprintf(stderr, "FireAdd(): Warning, out of shot slots!\n");
		return (-1);
	}
	if ((gpFireSlots[nSlotNo].nAnm = AnmSet(gpFireTable[nShot].pAnm, -1)) == -1) { FireReleaseSlot(nSlotNo); return (-1); }

	gpFireSlots[nSlotNo].nTbIdx = nShot;
	gpFireSlots[nSlotNo].nPosX = nPosX;// << 8;
	gpFireSlots[nSlotNo].nPosY = nPosY;// << 8;
	//
	nSpeed = gpFireTable[nShot].nSpeed & ~(1 << 15);
	nSpeedY = -nSpeed << ((u16)gpFireTable[nShot].nSpeed >> 15);
	gpFireSlots[nSlotNo].nSpeed = nSpeed;
	// Pour le joueur, on ajoute la vitesse initiale aux tirs horizontaux.
	if (gpFireTable[nShot].nPlyr && gpFireTable[nShot].nAngle == 0 && nSpeed)
	{
		gpFireSlots[nSlotNo].nSpeed += ABS(gShoot.nPlayerSpdX);
	}
	gpFireSlots[nSlotNo].nAngle = (nAngle == -1 ? gpFireTable[nShot].nAngle : (nAngle & 0xFF));
	gpFireSlots[nSlotNo].nSpeedY = nSpeedY;	// Gravité.

	gpFireSlots[nSlotNo].nPlyr = gpFireTable[nShot].nPlyr;
	gpFireSlots[nSlotNo].nTrailCnt = 0;
 	gpFireSlots[nSlotNo].sColRect.nType = e_SprRect_NDef;	// RAZ rect de col.

	gpFireSlots[nSlotNo].nNoMoreCtc = 0;

	// Décalage un coup en arrière pour compenser la première avance dans la gestion du tir.
	gpFireSlots[nSlotNo].nPosX -= (gVar.pCos[gpFireSlots[nSlotNo].nAngle] * (s32)nSpeed) >> 8;
	gpFireSlots[nSlotNo].nPosY -= (gVar.pSin[gpFireSlots[nSlotNo].nAngle] * (s32)nSpeed) >> 8;

	return (nSlotNo);
}

#define	SHOT_CLIP_VAL	(32 << 8)
#define	SHOT_PRIO_AND	31
// Gestion des tirs.
void FireManage(void)
{
	u32	i;
	s32	*pBlocks;
	u32	nBlockNo, nHt, nCol;
	u32	nSprPrio = 0;

	pBlocks = gMap.ppPlanesBlocks[gMap.nHeroPlane];	// Blocs.

	for (i = 0; i < FIRE_MAX_SLOTS; i++)
	{
		if (gpFireSlots[i].nUsed)
		{
			s32	nSpr;
			s32	nLastY;

			nSpr = AnmGetImage(gpFireSlots[i].nAnm);
			if (nSpr == SPR_NoSprite)	// Si jamais on a des tirs avec des NoSprite, on peut tester avec AnmCheckEnd().
			{
				// L'anim est finie. On kille le tir (+ éventuelle chain reaction).
				FireReleaseSlotChain(i);
			}
			else
			{

				// Guidage pour les homing missiles.
				if (gpFireTable[gpFireSlots[i].nTbIdx].nFlags & e_ShotFlag_Chaser)
				{
					u8	nAng = ChaserTarget_AcquireTarget(&gpFireSlots[i]);
					s8	nInc = nAng - gpFireSlots[i].nAngle;
					if (ABS(nInc) > 2) nInc = (nInc < 0 ? -2 : 2);
					gpFireSlots[i].nAngle += nInc;
				}

				// Déplacement du tir.
				nLastY = gpFireSlots[i].nPosY;
				gpFireSlots[i].nPosX += (gVar.pCos[gpFireSlots[i].nAngle] * (s32)gpFireSlots[i].nSpeed) >> 8;
				gpFireSlots[i].nPosY += (gVar.pSin[gpFireSlots[i].nAngle] * (s32)gpFireSlots[i].nSpeed) >> 8;
				// Gravité en plus ?
				if (gpFireTable[gpFireSlots[i].nTbIdx].nGravity)
				{
					gpFireSlots[i].nPosY += (s32)gpFireSlots[i].nSpeedY;
					gpFireSlots[i].nSpeedY += GRAVITY;
					if (gpFireSlots[i].nSpeedY > (s32)(gpFireTable[gpFireSlots[i].nTbIdx].nGravity << 4))
						gpFireSlots[i].nSpeedY = (s32)(gpFireTable[gpFireSlots[i].nTbIdx].nGravity << 4);
				}

				// Tests de sortie de l'écran.
				if ((gpFireTable[gpFireSlots[i].nTbIdx].nFlags & e_ShotFlag_NoClip) == 0)
				{
				// Haut/bas.
				if (gpFireSlots[i].nAngle <= 128)	// Inf ou = pour ne pas flipper en Y les tirs horizontaux.
				{
					// Vers le haut.
					if (gpFireSlots[i].nPosY < gScrollPos.nPosY - SHOT_CLIP_VAL)
					{
						FireReleaseSlot(i);
						continue;
					}
				}
				else
				{
					// Vers le bas.
					if (gpFireSlots[i].nPosY > gScrollPos.nPosY + (SCR_Height << 8) + SHOT_CLIP_VAL)
					{
						FireReleaseSlot(i);
						continue;
					}
					// Flip Y sur le sprite.
					nSpr ^= SPR_Flip_Y;
				}
				// Gauche/droite.
				if (gpFireSlots[i].nAngle > 64 && gpFireSlots[i].nAngle < 64+128)
				{
					// Vers la gauche.
					if (gpFireSlots[i].nPosX < gScrollPos.nPosX - SHOT_CLIP_VAL)
					{
						FireReleaseSlot(i);
						continue;
					}
					// Flip X sur le sprite.
					nSpr ^= SPR_Flip_X;
				}
				else
				{
					// Vers la droite.
					if (gpFireSlots[i].nPosX > gScrollPos.nPosX + (SCR_Width << 8) + SHOT_CLIP_VAL)
					{
						FireReleaseSlot(i);
						continue;
					}
				}
				} // if (gpFireTable[gpFireSlots[i].nTbIdx].nNoClip)
				else
				if (gpFireSlots[i].nAngle > 64 && gpFireSlots[i].nAngle < 64+128)
					nSpr ^= SPR_Flip_X;		// Flip X sur le sprite dans le cas ou nNoClip != 0.
				// Je pense que le Flip Y est sans intérêt (le 'else' ne concerne que les explosions et grenades).

				// Clip map.
				if ((u32)(gpFireSlots[i].nPosX >> 12)+2 >= gMap.pPlanesLg[gMap.nHeroPlane]+4 ||
					(u32)(gpFireSlots[i].nPosY >> 12)+2 >= gMap.pPlanesHt[gMap.nHeroPlane]+4)	// Coup des +2/+4 = Pour laisser dépasser les tirs un peu en dehors de la map.
				{
					// Libère le slot.
					FireReleaseSlot(i);
					continue;
				}

				// Doit-on tester le dur ?
				if (gpFireTable[gpFireSlots[i].nTbIdx].nFlags & e_ShotFlag_ChkGnd)
				{
					// Si on est bien dans le plan...
					if ((u32)(gpFireSlots[i].nPosX >> 12) < gMap.pPlanesLg[gMap.nHeroPlane] &&
						(u32)(gpFireSlots[i].nPosY >> 12) < gMap.pPlanesHt[gMap.nHeroPlane])
					{
						// Dans un sprite dur ?
						if (HardSpr_TestIn(gpFireSlots[i].nPosX >> 8, gpFireSlots[i].nPosY >> 8, e_HardSpr_ShotsIgnore)) goto _FireEnd;

						// Dans un bloc ?
						nBlockNo = *(pBlocks + ((gpFireSlots[i].nPosY >> 12) * gMap.nMapLg) + (gpFireSlots[i].nPosX >> 12) );
						// On teste seulement les blocs durs.
						nCol = (gMap.ppColCodes[gMap.nHeroPlane] + nBlockNo)->nCol;
						if ((nCol & 0x0f) == e_BlockCode_Hard)
						{
							if ((nHt = BlockGetHeight(nBlockNo, (gpFireSlots[i].nPosX >> 8))) != 0)
							{
								nHt = 16 - nHt;
								if (((gpFireSlots[i].nPosY >> 8) & 0x0F) >= nHt)	// Est-on dans le dur ?
								{
_FireEnd:
									// Recalage au niveau du sol.
									if (gpFireSlots[i].nSpeedY >= 0)	// (la vitesse y ne bouge que sur les tirs avec gravité !).
									{
										s32	nGndLevel;
										nGndLevel = BlockGetGroundLevel(gpFireSlots[i].nPosX >> 8, gpFireSlots[i].nPosY >> 8);
										if (gpFireSlots[i].nPosY + (nGndLevel << 8) >= nLastY - gpFireSlots[i].nSpeedY)
											gpFireSlots[i].nPosY += nGndLevel << 8;
									}
_FireEnd2:
									// Fin du tir.
									if (gpFireTable[gpFireSlots[i].nTbIdx].pAnmDust != NULL)
//x										DustSet(gpFireTable[gpFireSlots[i].nTbIdx].pAnmDust, gpFireSlots[i].nPosX, gpFireSlots[i].nPosY, e_Prio_Tirs - 1, 0);
										DustSet(gpFireTable[gpFireSlots[i].nTbIdx].pAnmDust, gpFireSlots[i].nPosX, gpFireSlots[i].nPosY, e_Prio_Tirs - 1, ((u8)(gpFireSlots[i].nAngle + 64) > 128 ? e_DustFlag_FlipX : 0) );
									// Libère le slot.
									FireReleaseSlotChain(i);
									continue;
								}
							}

						} // if ((nCol & 0x0f) == e_BlockCode_Hard)
						else if ((nCol & 0x0f) == e_BlockCode_Ceiling)
						{
							if ((nHt = BlockCeilingGetHeight(nBlockNo, (gpFireSlots[i].nPosX >> 8))) != 0)
							{
								nHt--;
								if (((gpFireSlots[i].nPosY >> 8) & 0x0F) <= nHt)	// Est-on dans le dur ?
									goto _FireEnd2;
							}
						} // if ((nCol & 0x0f) == e_BlockCode_Ceiling)

					} // if coordonnées ok
				} // if (gpFireSlots[i].nChkGnd)

				// Si on doit ajouter un offset en fct de l'angle du tir...
				if (gpFireTable[gpFireSlots[i].nTbIdx].nDiviser)
				{
					nSpr &= ~(SPR_Flip_X | SPR_Flip_Y);
					nSpr += gpFireSlots[i].nAngle >> gpFireTable[gpFireSlots[i].nTbIdx].nDiviser;
				}

				// Trainée ? Sur point de l'alpha 3.
				if (gpFireTable[gpFireSlots[i].nTbIdx].pTrailDust != NULL)
				{
					struct SSprRect sTrailRect;
					if (((++gpFireSlots[i].nTrailCnt) & 3)==0)		// Toutes les 4 frames.
					if (SprGetRect(nSpr, e_SprRectZone_ShotOrg, &sTrailRect))
					{
						if (sTrailRect.nType == e_SprRect_Point)	//< On doit pouvoir se passer de ce test.
						{
							s32	nOffsX = sTrailRect.nX1;
							s32 nOffsY = sTrailRect.nY1;
							// Si spr en rotation, on fait tourner le pt de départ du dust.
							if (gpFireTable[gpFireSlots[i].nTbIdx].nFlags & e_ShotFlag_RotoZoom)
								Rot2D_RotatePoint(&nOffsX, &nOffsY, gpFireSlots[i].nAngle);
							// Dust.
							DustSet(gpFireTable[gpFireSlots[i].nTbIdx].pTrailDust, gpFireSlots[i].nPosX + (nOffsX << 8), gpFireSlots[i].nPosY + (nOffsY << 8),
								(gpFireTable[gpFireSlots[i].nTbIdx].nPrio ? gpFireTable[gpFireSlots[i].nTbIdx].nPrio - 1 : e_Prio_DustUnder), 0);
//e_DustFlag_FlipX ? => a voir quand on aura de quoi le faire.
						}
					}
				}

				// Affichage du tir.
				if (gpFireTable[gpFireSlots[i].nTbIdx].nFlags & e_ShotFlag_RotoZoom)
					SprDisplayRotoZoom(nSpr & ~(SPR_Flip_X | SPR_Flip_Y), gpFireSlots[i].nPosX >> 8, gpFireSlots[i].nPosY >> 8,
						(gpFireTable[gpFireSlots[i].nTbIdx].nPrio ? gpFireTable[gpFireSlots[i].nTbIdx].nPrio : e_Prio_Tirs + nSprPrio), gpFireSlots[i].nAngle, 0x100);
				else
					SprDisplay(nSpr, gpFireSlots[i].nPosX >> 8, gpFireSlots[i].nPosY >> 8,
						(gpFireTable[gpFireSlots[i].nTbIdx].nPrio ? gpFireTable[gpFireSlots[i].nTbIdx].nPrio : e_Prio_Tirs + nSprPrio));
				nSprPrio = (nSprPrio + 1) & MSTPRIO_AND;

				// Récupération du rectangle de collision (évite de refaire des appels/tests dans HitCheck).
				if (SprGetRect(nSpr, e_SprRectZone_RectCol, &gpFireSlots[i].sColRect) == 0) gpFireSlots[i].sColRect.nType = e_SprRect_NDef;

				// Coupe les collisions ? (Après le premier contact, et après la frame pour que les grenades puissent toucher plusieurs ennemis dans la frame).
				if (gpFireSlots[i].nNoMoreCtc)
				{
					gpFireSlots[i].nNoMoreCtc = 0;
					gpFireSlots[i].nPlyr = e_ShotOrg_NoMoreCtc;		// Après le premier contact, plus de tests.
				}

			}
		}
	}

}

#ifdef DEBUG_DISP
// debug.
void shot_tst_dispnb(u32 nPosY)
{
	u32	i, k;
	// Compte.
	for (i = 0, k = 0; i < FIRE_MAX_SLOTS; i++) if (gpFireSlots[i].nUsed) k++;
	// Affichage.
	char	pTb[4+8+1] = "SHT:00000000";
	MyItoA(k, pTb);
	Font_Print(10, nPosY, pTb, 0);
}
#endif

//=============================================================================

// Teste si un sprite se prend un tir.
u32 FireHitCheckRect(struct SSprRect *pRect1, s32 nPosX, s32 nPosY, u32 nToCheck, u8 nNoCtc)//, u8 nPrio)
{
	u32	i;
	u32	nDamagePts = 0;
	u32	nDamageType = 0;

	s32	nXMin1, nXMax1, nYMin1, nYMax1;
	s32	nXMin2, nXMax2, nYMin2, nYMax2;

	nXMin1 = nPosX + (pRect1->nX1 << 8);
	nXMax1 = nPosX + (pRect1->nX2 << 8);
	nYMin1 = nPosY + (pRect1->nY1 << 8);
	nYMax1 = nPosY + (pRect1->nY2 << 8);

/*
//>> tst / affichage du sprite de col
//if (nToCheck == e_ShotOrg_Enemy)	// Pour le héros.
{
	SprDisplay(e_Spr_Tstrct_CornerUL, (nXMin1 >> 8), (nYMin1 >> 8), e_Prio_Joueur + 3+0x100);
	SprDisplay(e_Spr_Tstrct_CornerUR, (nXMax1 >> 8), (nYMin1 >> 8), e_Prio_Joueur + 3+0x100);
	SprDisplay(e_Spr_Tstrct_CornerDL, (nXMin1 >> 8), (nYMax1 >> 8), e_Prio_Joueur + 3+0x100);
	SprDisplay(e_Spr_Tstrct_CornerDR, (nXMax1 >> 8), (nYMax1 >> 8), e_Prio_Joueur + 3+0x100);
}
//<< tst / affichage du sprite de col
*/

	for (i = 0; i < FIRE_MAX_SLOTS; i++)
	{
		if (gpFireSlots[i].nUsed)
		{
			if (gpFireSlots[i].nPlyr == nToCheck)
			{
				if (gpFireSlots[i].sColRect.nType != e_SprRect_Rect) continue;
				nXMin2 = gpFireSlots[i].nPosX + (gpFireSlots[i].sColRect.nX1 << 8);
				nXMax2 = gpFireSlots[i].nPosX + (gpFireSlots[i].sColRect.nX2 << 8);
				nYMin2 = gpFireSlots[i].nPosY + (gpFireSlots[i].sColRect.nY1 << 8);
				nYMax2 = gpFireSlots[i].nPosY + (gpFireSlots[i].sColRect.nY2 << 8);

/*
//>> tst / affichage du sprite de col
{
	SprDisplay(e_Spr_Tstrct_CornerUL, (nXMin2 >> 8), (nYMin2 >> 8), 0x100+e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerUR, (nXMax2 >> 8), (nYMin2 >> 8), 0x100+e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerDL, (nXMin2 >> 8), (nYMax2 >> 8), 0x100+e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerDR, (nXMax2 >> 8), (nYMax2 >> 8), 0x100+e_Prio_Joueur + 3);
}
//<< tst / affichage du sprite de col
*/

				// Collisions entre les rectangles ?
				if (nXMax1 >= nXMin2 && nXMin1 <= nXMax2 && nYMax1 >= nYMin2 && nYMin1 <= nYMax2)
				{
					nDamagePts += gpFireTable[gpFireSlots[i].nTbIdx].nDamagePts;		// On continue la boucle : L'ennemi peut se prendre plusieurs balles en 1 fois et/ou arrêter les tirs. / Alternative : return (nDamage);
					nDamageType = gpFireTable[gpFireSlots[i].nTbIdx].nDamageType;	//** revoir, garder le dégat le plus spectaculaire ? (le feu ?)

					// Touché. Le tir doit disparaitre ?
					if ((gpFireTable[gpFireSlots[i].nTbIdx].nFlags & e_ShotFlag_Remain) == 0)
					{
						// Dust de disparition du tir.
						if (gpFireTable[gpFireSlots[i].nTbIdx].pAnmDust != NULL)
//x							DustSet(gpFireTable[gpFireSlots[i].nTbIdx].pAnmDust, gpFireSlots[i].nPosX, gpFireSlots[i].nPosY, (nToCheck == e_ShotOrg_Enemy ? e_Prio_Joueur+10 : e_Prio_DustOver), 0);
							DustSet(gpFireTable[gpFireSlots[i].nTbIdx].pAnmDust, gpFireSlots[i].nPosX, gpFireSlots[i].nPosY, (nToCheck == e_ShotOrg_Enemy ? e_Prio_Joueur+10 : e_Prio_DustOver), ((u8)(gpFireSlots[i].nAngle + 64) > 128 ? e_DustFlag_FlipX : 0) );
						// On libère le slot.
						FireReleaseSlotChain(i);
					}
					else
//						gpFireSlots[i].nNoMoreCtc |= nNoCtc;	// Au prochain FireManage, si pas 0, plus de contact (pour les sprites qui possèdent des points de vie).
					if ((gpFireTable[gpFireSlots[i].nTbIdx].nFlags & e_ShotFlag_CancelNoCtc) == 0)
						gpFireSlots[i].nNoMoreCtc |= nNoCtc;	// Au prochain FireManage, si pas 0, plus de contact (pour les sprites qui possèdent des points de vie).

				}
			}
		}
	}

	return ((nDamageType << 16) | nDamagePts);
}

// Teste si un sprite se prend un tir.
// In : PosX + PosY avec 8 bits de virgule fixe.
// Out: 0 = Pas de choc / 1 = Hit. (Eventuellement, renvoyer le nb de pts de dégats...).
u32 FireHitCheck(u32 nSpr, s32 nPosX, s32 nPosY, u32 nToCheck, u8 nNoCtc)//, u32 pDustAnm = NULL)	// Pour dust particulier en fct du monstre. Si NULL, utiliser dust générique.
{
	struct SSprRect sRect1;

	if (SprGetRect(nSpr, e_SprRectZone_RectCol, &sRect1) == 0) return (0);
	return (FireHitCheckRect(&sRect1, nPosX, nPosY, nToCheck, nNoCtc));
}

//=============================================================================

// Renvoie un ptr sur les infos qui intéressent le Mst 23 (pour mise à jour du slot).
u32 Fire_Mst23Get(u32 nSlotNo, s32 **ppnPosX, s32 **ppnPosY, u8 **ppnPlyr)
{
	if (gpFireSlots[nSlotNo].nUsed)
	{
		*ppnPosX = &gpFireSlots[nSlotNo].nPosX;
		*ppnPosY = &gpFireSlots[nSlotNo].nPosY;
		*ppnPlyr = &gpFireSlots[nSlotNo].nPlyr;
		return (1);
	}
	return (0);
}

//=============================================================================

// Retire des tirs de la liste des tirs actifs (Ex : Destruction d'un boss => on retire ses tirs).
// In: nShotIdx = n° du tir à supprimer.
//     nShotIdx = -1 => Suppression de tous les tirs.
//     nShotIdx = Bit31 | e_ShotOrg_Enemy => Suppression de tous les tirs ennemis.
//     nShotIdx = Bit31 | e_ShotOrg_Player => suppression de tous les tirs du joueur.
void FireRemoveFromList(s32 nShotIdx)
{
	u32	i;
	s32	nSlot;

	for (i = 0; i < FIRE_MAX_SLOTS; i++)
	{
		if (gpFireSlots[i].nUsed)
		if (gpFireSlots[i].nTbIdx == nShotIdx || nShotIdx == -1 ||
			(nShotIdx & (1<<31) && gpFireSlots[i].nPlyr == (nShotIdx & ~(1<<31))) )
		{
			// Dust de disparition du tir.
			if (gpFireTable[gpFireSlots[i].nTbIdx].pAnmDust != NULL)
//x				DustSet(gpFireTable[gpFireSlots[i].nTbIdx].pAnmDust, gpFireSlots[i].nPosX, gpFireSlots[i].nPosY, (gpFireSlots[i].nPlyr ? e_Prio_DustOver : e_Prio_Joueur+10), 0);
				DustSet(gpFireTable[gpFireSlots[i].nTbIdx].pAnmDust, gpFireSlots[i].nPosX, gpFireSlots[i].nPosY, (gpFireSlots[i].nPlyr ? e_Prio_DustOver : e_Prio_Joueur+10), ((u8)(gpFireSlots[i].nAngle + 64) > 128 ? e_DustFlag_FlipX : 0) );
			// On libère le slot + chain secondaire.
			nSlot = FireReleaseSlotChain(i);
			// Si chain, on rend le tir inofensif.
			if (nSlot != -1) gpFireSlots[nSlot].nPlyr = e_ShotOrg_NoMoreCtc;		// e_ShotOrg_NoMoreCtc => plus de tests.
		}
	}

}

//=============================================================================

// Teste les tirs destructibles. Uniquement tirs héros > détruisent les tirs ennemis.
void FireDestructibleCheck(void)
{
	u32	i, j;
	s32	nXMin1, nXMax1, nYMin1, nYMax1;
	s32	nXMin2, nXMax2, nYMin2, nYMax2;

	// Cherche les tirs destructibles.
	for (i = 0; i < FIRE_MAX_SLOTS; i++)
	if (gpFireSlots[i].nUsed)
	if (gpFireTable[gpFireSlots[i].nTbIdx].nFlags & e_ShotFlag_Destructible)
	{
		// Rectangle du tir à détruire.
		if (gpFireSlots[i].sColRect.nType != e_SprRect_Rect) continue;
		nXMin1 = gpFireSlots[i].nPosX + (gpFireSlots[i].sColRect.nX1 << 8);
		nXMax1 = gpFireSlots[i].nPosX + (gpFireSlots[i].sColRect.nX2 << 8);
		nYMin1 = gpFireSlots[i].nPosY + (gpFireSlots[i].sColRect.nY1 << 8);
		nYMax1 = gpFireSlots[i].nPosY + (gpFireSlots[i].sColRect.nY2 << 8);
//printf("Destructible : slot = %d\n",i);

		// Boucle dans les tirs du joueur.
		for (j = 0; j < FIRE_MAX_SLOTS; j++)
		if (gpFireSlots[j].nUsed && gpFireSlots[j].nPlyr == e_ShotOrg_Player && j != i)
		{
			if (gpFireSlots[j].sColRect.nType != e_SprRect_Rect) continue;
			nXMin2 = gpFireSlots[j].nPosX + (gpFireSlots[j].sColRect.nX1 << 8);
			nXMax2 = gpFireSlots[j].nPosX + (gpFireSlots[j].sColRect.nX2 << 8);
			nYMin2 = gpFireSlots[j].nPosY + (gpFireSlots[j].sColRect.nY1 << 8);
			nYMax2 = gpFireSlots[j].nPosY + (gpFireSlots[j].sColRect.nY2 << 8);
//printf("hero : slot = %d\n",j);

			// Collisions entre les rectangles ?
			if (nXMax1 >= nXMin2 && nXMin1 <= nXMax2 && nYMax1 >= nYMin2 && nYMin1 <= nYMax2)
			{

				// Tir joueur.
				if ((gpFireTable[gpFireSlots[j].nTbIdx].nFlags & e_ShotFlag_Remain) == 0)
				{
					// Dust de disparition du tir.
					if (gpFireTable[gpFireSlots[j].nTbIdx].pAnmDust != NULL)
//x						DustSet(gpFireTable[gpFireSlots[j].nTbIdx].pAnmDust, gpFireSlots[j].nPosX, gpFireSlots[j].nPosY, e_Prio_DustOver, 0);
						DustSet(gpFireTable[gpFireSlots[j].nTbIdx].pAnmDust, gpFireSlots[j].nPosX, gpFireSlots[j].nPosY, e_Prio_DustOver, ((u8)(gpFireSlots[j].nAngle + 64) > 128 ? e_DustFlag_FlipX : 0) );
					// On libère le slot.
					FireReleaseSlotChain(j);
				}

				// Tir ennemi (plus simple, car on ne peut détruire que des roquettes).
				// Dust de disparition du tir.
				if (gpFireTable[gpFireSlots[i].nTbIdx].pAnmDust != NULL)
//x					DustSet(gpFireTable[gpFireSlots[i].nTbIdx].pAnmDust, gpFireSlots[i].nPosX, gpFireSlots[i].nPosY, e_Prio_Joueur+10, 0);
					DustSet(gpFireTable[gpFireSlots[i].nTbIdx].pAnmDust, gpFireSlots[i].nPosX, gpFireSlots[i].nPosY, e_Prio_Joueur+10, ((u8)(gpFireSlots[i].nAngle + 64) > 128 ? e_DustFlag_FlipX : 0) );
				// On libère le slot.
				FireReleaseSlotChain(i);

			}

		}
	}

}

//=============================================================================
// Le système de homing missile.

#define	CHASER_MAX_SLOTS	32
struct SChaserTarget
{
	s32	nPosX, nPosY;		// En pixels.
	// on verra pour rajouter un n° d'ordre et un 'locked' pour faire plusieurs cibles pour plusieurs missiles.
};

struct SMstCommon	gpChaserTargetSlots[CHASER_MAX_SLOTS];
u32	gnChaserTargetInList;

// RAZ de la liste (à chaque frame).
void ChaserTarget_ClearList(void)
{
	gnChaserTargetInList = 0;
}

// Ajout d'une cible dans la liste.
void ChaserTarget_AddToList(s32 nPosX, s32 nPosY)
{
	if (gnChaserTargetInList >= CHASER_MAX_SLOTS) return;
	gpChaserTargetSlots[gnChaserTargetInList].nPosX = nPosX;
	gpChaserTargetSlots[gnChaserTargetInList].nPosY = nPosY;
	gnChaserTargetInList++;
}

// Renvoie l'angle pour la cible la plus proche.
// (Conserve l'angle en cours si pas de cible à proximité).
#define	CT_SQUARE_SZ	100
u8 ChaserTarget_AcquireTarget(struct SFire *pFire)
{
	s32	nX1, nY1, nX2, nY2;		// Coord du carré dans lequel chercher.
	static	s8	pnOffsTb[] = {	// Offsets x1 y1 x2 y2 par rapport au missile.
		0,-CT_SQUARE_SZ/2,  CT_SQUARE_SZ,CT_SQUARE_SZ/2,
		0,-CT_SQUARE_SZ,    CT_SQUARE_SZ,0,
		-CT_SQUARE_SZ/2,-CT_SQUARE_SZ,    CT_SQUARE_SZ/2,0,
		-CT_SQUARE_SZ,-CT_SQUARE_SZ,      0,0,
		-CT_SQUARE_SZ,-CT_SQUARE_SZ/2,    0,CT_SQUARE_SZ/2,
		-CT_SQUARE_SZ,0,    0,CT_SQUARE_SZ,
		-CT_SQUARE_SZ/2,0,  CT_SQUARE_SZ/2,CT_SQUARE_SZ,
		0,0,                CT_SQUARE_SZ,CT_SQUARE_SZ,
		};
	u32	i;
	s32	nShotPosX, nShotPosY;


	// Chaser ennemi ? => Target == joueur. Si le joueur est mort, renvoie l'angle en cours.
//	if (pFire->nPlyr == 0)
//		return (gShoot.nDeathFlag ? pFire->nAngle : fatan2(-((gShoot.nPlayerPosY - (12 << 8)) - pFire->nPosY), gShoot.nPlayerPosX - pFire->nPosX) );
	if (pFire->nPlyr == 0)
	{
		u8	j;
		i = fatan2(-((gShoot.nPlayerPosY - (12 << 8)) - pFire->nPosY), gShoot.nPlayerPosX - pFire->nPosX);
		j = i - pFire->nAngle;
		if (j & 0x80) j = -j;
		return (gShoot.nDeathFlag  || j > 64 ? pFire->nAngle : i);		// +- 64 degrés max.
	}

	// Pas de cibles, on renvoie l'angle en cours.
	if (gnChaserTargetInList == 0) return (pFire->nAngle);

	nShotPosX = pFire->nPosX >> 8;
	nShotPosY = pFire->nPosY >> 8;

	// Coord du carré dans lequel chercher.
	i = ((u8)(pFire->nAngle + 16) >> 5) << 2;
	nX1 = (pFire->nPosX >> 8) + pnOffsTb[i];
	nY1 = (pFire->nPosY >> 8) + pnOffsTb[i+1];
	nX2 = (pFire->nPosX >> 8) + pnOffsTb[i+2];
	nY2 = (pFire->nPosY >> 8) + pnOffsTb[i+3];

/*
//>> tst / affichage du sprite de col
{
	SprDisplay(e_Spr_Tstrct_CornerUL, (nX1), (nY1), e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerUR, (nX2), (nY1), e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerDL, (nX1), (nY2), e_Prio_Joueur + 3);
	SprDisplay(e_Spr_Tstrct_CornerDR, (nX2), (nY2), e_Prio_Joueur + 3);
}
//<< tst / affichage du sprite de col
*/

	// Recherche des cibles se trouvant dans le rectangle.
	s32	nBest = -1;
	u32	nBestDist = (u32)-1;
	u32	t;
	for (i = 0; i < gnChaserTargetInList; i++)
	{
		// Dans le rectangle de recherche ?
		if (gpChaserTargetSlots[i].nPosX >= nX1 && gpChaserTargetSlots[i].nPosX <= nX2 &&
			gpChaserTargetSlots[i].nPosY >= nY1 && gpChaserTargetSlots[i].nPosY <= nY2)
		{
//SprDisplay(e_Spr_Tstrct_Cross, (gpChaserTargetSlots[i].nPosX), (gpChaserTargetSlots[i].nPosY), e_Prio_Joueur + 3);
			t = ((gpChaserTargetSlots[i].nPosX - nShotPosX) * (gpChaserTargetSlots[i].nPosX - nShotPosX)) +
				((gpChaserTargetSlots[i].nPosY - nShotPosY) * (gpChaserTargetSlots[i].nPosY - nShotPosY));
			if (t < nBestDist)
			{
				nBestDist = t;
				nBest = i;
			}
		}
	}

	// Si on a une cible, on renvoie l'angle à atteindre pour se diriger vers cette cible.
	u8	nAng;
	if (nBest != -1)
		nAng = fatan2(-(gpChaserTargetSlots[nBest].nPosY - nShotPosY) << 8, (gpChaserTargetSlots[nBest].nPosX - nShotPosX) << 8);
	else
		nAng = pFire->nAngle;

	return (nAng);
}


