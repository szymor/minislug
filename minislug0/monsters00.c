
#include "includes.h"
#include "monsters_inc.h"

//=============================================================================
void Mst0_Init_Platform1(struct SMstCommon *pMst, u8 *pData);
s32 Mst0_Main_Platform1(struct SMstCommon *pMst);
void Mst1_Init_ForegroundSpr1(struct SMstCommon *pMst, u8 *pData);
s32 Mst1_Main_ForegroundSpr1(struct SMstCommon *pMst);
void Mst2_Init_MiniUFO0(struct SMstCommon *pMst, u8 *pData);
s32 Mst2_Main_MiniUFO0(struct SMstCommon *pMst);
void Mst3_Init_POW1(struct SMstCommon *pMst, u8 *pData);
s32 Mst3_Main_POW1(struct SMstCommon *pMst);
void Mst4_Init_WeaponCapsule1(struct SMstCommon *pMst, u8 *pData);
s32 Mst4_Main_WeaponCapsule1(struct SMstCommon *pMst);
void Mst5_Init_ScrollLockX1(struct SMstCommon *pMst, u8 *pData);
s32 Mst5_Main_ScrollLockX1(struct SMstCommon *pMst);
void Mst6_Init_RShobu1(struct SMstCommon *pMst, u8 *pData);
s32 Mst6_Main_RShobu1(struct SMstCommon *pMst);
void Mst7_Init_LeftRight0(struct SMstCommon *pMst, u8 *pData);
s32 Mst7_Main_LeftRight0(struct SMstCommon *pMst);
void Mst8_Init_BlkBkg1(struct SMstCommon *pMst, u8 *pData);
s32 Mst8_Main_BlkBkg1(struct SMstCommon *pMst);
void Mst9_Init_Slug(struct SMstCommon *pMst, u8 *pData);
s32 Mst9_Main_Slug(struct SMstCommon *pMst);

void Mst10_Init_Jellyfish(struct SMstCommon *pMst, u8 *pData);
s32 Mst10_Main_Jellyfish(struct SMstCommon *pMst);
void Mst11_Init_ObjItemCtc(struct SMstCommon *pMst, u8 *pData);
s32 Mst11_Main_ObjItemCtc(struct SMstCommon *pMst);
void Mst12_Init_HeroBlocker(struct SMstCommon *pMst, u8 *pData);
s32 Mst12_Main_HeroBlocker(struct SMstCommon *pMst);
void Mst13_Init_NextLevel(struct SMstCommon *pMst, u8 *pData);
s32 Mst13_Main_NextLevel(struct SMstCommon *pMst);
void Mst14_Init_RebelSoldier0(struct SMstCommon *pMst, u8 *pData);
s32 Mst14_Main_RebelSoldier0(struct SMstCommon *pMst);
void Mst15_Init_Truck0(struct SMstCommon *pMst, u8 *pData);
s32 Mst15_Main_Truck0(struct SMstCommon *pMst);
void Mst16_Init_BlkBkg2(struct SMstCommon *pMst, u8 *pData);
s32 Mst16_Main_BlkBkg2(struct SMstCommon *pMst);
void Mst17_Init_Explosions0(struct SMstCommon *pMst, u8 *pData);
s32 Mst17_Main_Explosions0(struct SMstCommon *pMst);
void Mst18_Init_RectShotsCheck0(struct SMstCommon *pMst, u8 *pData);
s32 Mst18_Main_RectShotsCheck0(struct SMstCommon *pMst);
void Mst19_Init_FgSpr2States0(struct SMstCommon *pMst, u8 *pData);
s32 Mst19_Main_FgSpr2States0(struct SMstCommon *pMst);

void Mst20_Init_Boss0(struct SMstCommon *pMst, u8 *pData);
s32 Mst20_Main_Boss0(struct SMstCommon *pMst);
void Mst21_Init_ScrollY1(struct SMstCommon *pMst, u8 *pData);
s32 Mst21_Main_ScrollY1(struct SMstCommon *pMst);
void Mst22_Init_ExplodingBarrel0(struct SMstCommon *pMst, u8 *pData);
s32 Mst22_Main_ExplodingBarrel0(struct SMstCommon *pMst);
void Mst23_Init_FallingObjects0(struct SMstCommon *pMst, u8 *pData);
s32 Mst23_Main_FallingObjects0(struct SMstCommon *pMst);
void Mst24_Init_HardObjects0(struct SMstCommon *pMst, u8 *pData);
s32 Mst24_Main_HardObjects0(struct SMstCommon *pMst);
void Mst25_Init_RocketDiver0(struct SMstCommon *pMst, u8 *pData);
s32 Mst25_Main_RocketDiver0(struct SMstCommon *pMst);
void Mst26_Init_Girida0(struct SMstCommon *pMst, u8 *pData);
s32 Mst26_Main_Girida0(struct SMstCommon *pMst);
void Mst27_Init_HalfBoss0(struct SMstCommon *pMst, u8 *pData);
s32 Mst27_Main_HalfBoss0(struct SMstCommon *pMst);
void Mst28_Init_Masknell0(struct SMstCommon *pMst, u8 *pData);
s32 Mst28_Main_Masknell0(struct SMstCommon *pMst);
void Mst29_Init_Whale0(struct SMstCommon *pMst, u8 *pData);
s32 Mst29_Main_Whale0(struct SMstCommon *pMst);

void Mst30_Init_SquidGenerator0(struct SMstCommon *pMst, u8 *pData);
s32 Mst30_Main_SquidGenerator0(struct SMstCommon *pMst);
void Mst31_Init_Squid0(struct SMstCommon *pMst, u8 *pData);
s32 Mst31_Main_Squid0(struct SMstCommon *pMst);
void Mst32_Init_L11ScrollCtrl(struct SMstCommon *pMst, u8 *pData);
s32 Mst32_Main_L11ScrollCtrl(struct SMstCommon *pMst);
void Mst33_Init_L11Generator0(struct SMstCommon *pMst, u8 *pData);
s32 Mst33_Main_L11Generator0(struct SMstCommon *pMst);
void Mst34_Init_L11MarsEye0(struct SMstCommon *pMst, u8 *pData);
s32 Mst34_Main_L11MarsEye0(struct SMstCommon *pMst);
void Mst35_Init_L11SpaceRocks0(struct SMstCommon *pMst, u8 *pData);
s32 Mst35_Main_L11SpaceRocks0(struct SMstCommon *pMst);
void Mst36_Init_L11Asteroid0(struct SMstCommon *pMst, u8 *pData);
s32 Mst36_Main_L11Asteroid0(struct SMstCommon *pMst);
void Mst37_Init_L11BigAsteroid0(struct SMstCommon *pMst, u8 *pData);
s32 Mst37_Main_L11BigAsteroid0(struct SMstCommon *pMst);
void Mst38_Init_BigJellyfish0(struct SMstCommon *pMst, u8 *pData);
s32 Mst38_Main_BigJellyfish0(struct SMstCommon *pMst);
void Mst39_Init_SensorMine0(struct SMstCommon *pMst, u8 *pData);
s32 Mst39_Main_SensorMine0(struct SMstCommon *pMst);

void Mst40_Init_L11SpaceTrash0(struct SMstCommon *pMst, u8 *pData);
s32 Mst40_Main_L11SpaceTrash0(struct SMstCommon *pMst);
void Mst41_Init_L11MarsUFO0(struct SMstCommon *pMst, u8 *pData);
s32 Mst41_Main_L11MarsUFO0(struct SMstCommon *pMst);
void Mst42_Init_DoorToDestroy0(struct SMstCommon *pMst, u8 *pData);
s32 Mst42_Main_DoorToDestroy0(struct SMstCommon *pMst);
void Mst43_Init_FlyingTara0(struct SMstCommon *pMst, u8 *pData);
s32 Mst43_Main_FlyingTara0(struct SMstCommon *pMst);
void Mst44_Init_SlugWaitTransit0(struct SMstCommon *pMst, u8 *pData);
s32 Mst44_Main_SlugWaitTransit0(struct SMstCommon *pMst);
void Mst45_Init_FlyingTaraBomb0(struct SMstCommon *pMst, u8 *pData);
s32 Mst45_Main_FlyingTaraBomb0(struct SMstCommon *pMst);
void Mst46_Init_HairBusterRibert0(struct SMstCommon *pMst, u8 *pData);
s32 Mst46_Main_HairBusterRibert0(struct SMstCommon *pMst);
void Mst47_Init_Lev9RocketTop0(struct SMstCommon *pMst, u8 *pData);
s32 Mst47_Main_Lev9RocketTop0(struct SMstCommon *pMst);
void Mst48_Init_Joke0(struct SMstCommon *pMst, u8 *pData);
s32 Mst48_Main_Joke0(struct SMstCommon *pMst);
void Mst49_Init_SuspSub0(struct SMstCommon *pMst, u8 *pData);
s32 Mst49_Main_SuspSub0(struct SMstCommon *pMst);

void Mst50_Init_AikawaRumi0(struct SMstCommon *pMst, u8 *pData);
s32 Mst50_Main_AikawaRumi0(struct SMstCommon *pMst);
void Mst51_Init_HowToPlay0(struct SMstCommon *pMst, u8 *pData);
s32 Mst51_Main_HowToPlay0(struct SMstCommon *pMst);
void Mst52_Init_Credits0(struct SMstCommon *pMst, u8 *pData);
s32 Mst52_Main_Credits0(struct SMstCommon *pMst);

struct SMstTb gpMstTb[e_Mst_MAX] =
{
	{ Mst0_Init_Platform1, Mst0_Main_Platform1, NULL, 0 },	// Plateformes mobiles.
	{ Mst1_Init_ForegroundSpr1, Mst1_Main_ForegroundSpr1, NULL, 0 },	// Sprites d'avant plan.
	{ Mst2_Init_MiniUFO0, Mst2_Main_MiniUFO0, gAnm_MiniUFO_Fly,  100 },	// Mini UFO.
	{ Mst3_Init_POW1, Mst3_Main_POW1, gAnm_POW_SittingWait, 100 },	// POW.
	{ Mst4_Init_WeaponCapsule1, Mst4_Main_WeaponCapsule1, gAnm_Itm_WeaponCapsuleH, 50 },	// Weapon Capsule.
	{ Mst5_Init_ScrollLockX1, Mst5_Main_ScrollLockX1, NULL, 0 },	// Scroll lock.
	{ Mst6_Init_RShobu1, Mst6_Main_RShobu1, gAnm_RShobu_Flying, 500 },	// R-Shobu (demi-boss).
	{ Mst7_Init_LeftRight0, Mst7_Main_LeftRight0, gAnm_ZombieTeen_Idle, 50 },	// Gauche droite.
	{ Mst8_Init_BlkBkg1, Mst8_Main_BlkBkg1, gAnm_ZombieTeen_Idle, 0 },	// Update du décor / Anim quelconque, uniquement pour init du slot.
	{ Mst9_Init_Slug, Mst9_Main_Slug, gAnm_Slug_InSign, 0 },	// Slug.

	{ Mst10_Init_Jellyfish, Mst10_Main_Jellyfish, gAnm_JellyfishSmall_Swim, 10 },	// Méduse.
	{ Mst11_Init_ObjItemCtc, Mst11_Main_ObjItemCtc, gAnm_ZombieTeen_Idle, 0 },	// Objet donnant un itm de quête par contact / Anim bidon, uniquement pour init du slot.
	{ Mst12_Init_HeroBlocker, Mst12_Main_HeroBlocker, NULL, 0 },	// Hero blocker.
	{ Mst13_Init_NextLevel, Mst13_Main_NextLevel, NULL, 0 },	// Next level.
	{ Mst14_Init_RebelSoldier0, Mst14_Main_RebelSoldier0, gAnm_RebSoldier_Rifle_CrouchWait, 50 },	// Rebel soldier.
	{ Mst15_Init_Truck0, Mst15_Main_Truck0, gAnm_TruckTrooper_Idle, 250 },	// Transport de troupes.
	{ Mst16_Init_BlkBkg2, Mst16_Main_BlkBkg2, NULL, 0 },	// Update du décor 2.
	{ Mst17_Init_Explosions0, Mst17_Main_Explosions0, NULL, 0 },	// Explosions.
	{ Mst18_Init_RectShotsCheck0, Mst18_Main_RectShotsCheck0, NULL, 50 },	// Test des tirs dans un rectangle.
	{ Mst19_Init_FgSpr2States0, Mst19_Main_FgSpr2States0, NULL, 0 },	// Foreground sprite avec 2 états.

	{ Mst20_Init_Boss0, Mst20_Main_Boss0, NULL, 0 },	// Boss (encapsule les bosses).
	{ Mst21_Init_ScrollY1, Mst21_Main_ScrollY1, NULL, 0 },	// Scroll Y.
	{ Mst22_Init_ExplodingBarrel0, Mst22_Main_ExplodingBarrel0, NULL, 50 },	// Tonneau explosif.
	{ Mst23_Init_FallingObjects0, Mst23_Main_FallingObjects0, gAnm_ZombieTeen_Idle, 100 },	// Objets suspendus.
	{ Mst24_Init_HardObjects0, Mst24_Main_HardObjects0, NULL, 0 },	// Objets durs.
	{ Mst25_Init_RocketDiver0, Mst25_Main_RocketDiver0, gAnm_RocketDiver_Jump, 100 },	// Rocket Diver.
	{ Mst26_Init_Girida0, Mst26_Main_Girida0, gAnm_Girida0_Body_Idle_NoFx, 250 },	// Girida-0.
	{ Mst27_Init_HalfBoss0, Mst27_Main_HalfBoss0, NULL, 0 },	// Container des demi-boss.
	{ Mst28_Init_Masknell0, Mst28_Main_Masknell0, gAnm_Masknell_Blades, 100 },	// Masknell (demi-boss).
	{ Mst29_Init_Whale0, Mst29_Main_Whale0, gAnm_Whale_Attack, 0 },	// Orque.

	{ Mst30_Init_SquidGenerator0, Mst30_Main_SquidGenerator0, NULL, 0 },	// Squid generator.
	{ Mst31_Init_Squid0, Mst31_Main_Squid0, gAnm_Squid_Swim, 50 },	// Squid.
	{ Mst32_Init_L11ScrollCtrl, Mst32_Main_L11ScrollCtrl, NULL, 0 },	// L11 - Scroll control.
	{ Mst33_Init_L11Generator0, Mst33_Main_L11Generator0, NULL, 0 },	// L11 - Générateur.
	{ Mst34_Init_L11MarsEye0, Mst34_Main_L11MarsEye0, gAnm_MarsTIE_Fly, 50 },	// L11 - Mars Eye.
	{ Mst35_Init_L11SpaceRocks0, Mst35_Main_L11SpaceRocks0, NULL, 0 },	// L11 - Space Rocks.
	{ Mst36_Init_L11Asteroid0, Mst36_Main_L11Asteroid0, gAnm_AsteroidMedium_Fly, 50 },	// L11 - Asteroid.
	{ Mst37_Init_L11BigAsteroid0, Mst37_Main_L11BigAsteroid0, gAnm_BigAsteroid_Cracks, 50 },	// L11 - Big Asteroid.
	{ Mst38_Init_BigJellyfish0, Mst38_Main_BigJellyfish0, gAnm_JellyfishBig_Swim, 500 },	// Big Jellyfish  (demi-boss).
	{ Mst39_Init_SensorMine0, Mst39_Main_SensorMine0, gAnm_SensorMine_Idle, 50 },	// Sensor Mine.

	{ Mst40_Init_L11SpaceTrash0, Mst40_Main_L11SpaceTrash0, NULL, 0 },	// L11 - Space trash.
	{ Mst41_Init_L11MarsUFO0, Mst41_Main_L11MarsUFO0, gAnm_MarsUFO_Fly, 100 },	// L11 - Mars UFO.
	{ Mst42_Init_DoorToDestroy0, Mst42_Main_DoorToDestroy0, NULL, 50 },	// Porte à détruire.
	{ Mst43_Init_FlyingTara0, Mst43_Main_FlyingTara0, NULL, 50 },	// Flying Tara.
	{ Mst44_Init_SlugWaitTransit0, Mst44_Main_SlugWaitTransit0, NULL, 0 },	// Slug ouvert pour transition.
	{ Mst45_Init_FlyingTaraBomb0, Mst45_Main_FlyingTaraBomb0, NULL, 0 },	// Bombe du Flying Tara.
	{ Mst46_Init_HairBusterRibert0, Mst46_Main_HairBusterRibert0, gAnm_HairBusterRibert_DoorClosed, 500 },	// Hair Buster Ribert (demi-boss).
	{ Mst47_Init_Lev9RocketTop0, Mst47_Main_Lev9RocketTop0, NULL, 100 },	// L09 - Chapeau de la fusée.
	{ Mst48_Init_Joke0, Mst48_Main_Joke0, gAnm_SensorMine_Idle, 50 },	// Mst pour blagues / Anim bidon, uniquement pour init du slot.
	{ Mst49_Init_SuspSub0, Mst49_Main_SuspSub0, NULL, 500 },	// Suspended submarine (half-boss).

	{ Mst50_Init_AikawaRumi0, Mst50_Main_AikawaRumi0, gAnm_AikawaRumi_Walk, 0 },	// Aikawa Rumi.
	{ Mst51_Init_HowToPlay0, Mst51_Main_HowToPlay0, 0 },	// How to play.
	{ Mst52_Init_Credits0, Mst52_Main_Credits0, 0 },	// Credits.

};

//=============================================================================
// Fonctions communes.

// Lecture d'un champ de bits (phase d'init).
s32 GetBits(u32 nBitStart, u32 nBitEnd, u8 *pData, u32 nSign)
{
	s32	nRet = 0;
	u8	nSrcMsk;
	u32 nDstMsk;
	u32	nCurByte;
	u32	i;

	if (nBitEnd < nBitStart) return (0);
	if (nBitEnd - nBitStart + 1 > 32) return (0);

	nDstMsk = 1;
	nSrcMsk = 1 << (nBitStart & 7);
	i = nBitEnd - nBitStart + 1;
	nCurByte = nBitStart >> 3;

	while (i)
	{
		if (pData[nCurByte] & nSrcMsk) nRet |= nDstMsk;
		nDstMsk <<= 1;
		nSrcMsk <<= 1;
		if (nSrcMsk == 0)
		{
			nSrcMsk = 1;
			nCurByte++;
		}
		i--;
	}

	// Faut-il étendre le signe ?
	if (nSign)
	{
		nDstMsk = ((u32)-1) << (nBitEnd - nBitStart);
		// Comme les bits supérieurs sont à 0, ça ne va matcher que le bit de signe (sur la taille demandée).
		if (nRet & nDstMsk) nRet |= nDstMsk;
	}

	return (nRet);
}

// Décalage d'un monstre : On le place en dehors de l'écran en le décalant à l'opposé de son angle.
void Mst_PutOutOfScreen(struct SMstCommon *pMst, s32 nOffset)
{
	u8	nAng = pMst->nAngle + 128;
	nOffset <<= 8;
	pMst->nPosX += (gVar.pCos[nAng] * nOffset) >> 8;
	pMst->nPosY += (gVar.pSin[nAng] * nOffset) >> 8;
}

// Colle un ennemi au sol. Version simple, pas de gravité.
void Enemy_GroundLevel(struct SMstCommon *pMst)
{
	s32	nHt;

	nHt = BlockGetGroundLevel(pMst->nPosX >> 8, pMst->nPosY >> 8);
	pMst->nPosY += nHt << 8;
	pMst->nGround = 1;
	pMst->nSpdY = 0;

}


// Teste si un monstre est sorti de l'écran.
// Out : 0 = Toujours à l'écran / 1 = Sorti.
u32 Screen_ObjectOut(s32 nPosX, s32 nPosY)
{
	if (nPosX > gScrollPos.nPosX + (SCR_Width << 8) + ((MST_CLIP_VAL + 1) << 12)) return (1);
	if (nPosX < gScrollPos.nPosX - ((MST_CLIP_VAL + 1) << 12)) return (1);
	if (nPosY > gScrollPos.nPosY + (SCR_Height << 8) + ((MST_CLIP_VAL + 1) << 12)) return (1);
	if (nPosY < gScrollPos.nPosY - ((MST_CLIP_VAL + 1) << 12)) return (1);
	return (0);
}
// Idem avec un rectangle.
// Out : 0 = Toujours à l'écran / 1 = Sorti.
u32 Screen_ObjectOutRect(s32 nPosX, s32 nPosY, u32 nBlkLg, u32 nBlkHt)
{
	if (nPosX > gScrollPos.nPosX + (SCR_Width << 8) + ((MST_CLIP_VAL + 1) << 12)) return (1);
	if (nPosX + (s32)(nBlkLg << 12) < gScrollPos.nPosX - ((MST_CLIP_VAL + 1) << 12)) return (1);
	if (nPosY > gScrollPos.nPosY + (SCR_Height << 8) + ((MST_CLIP_VAL + 1) << 12)) return (1);
	if (nPosY + (s32)(nBlkHt << 12) < gScrollPos.nPosY - ((MST_CLIP_VAL + 1) << 12)) return (1);
	return (0);
}

// Teste si le monstre est à proximité du héros pour un coup de couteau.
void Mst_ProximityCheck(struct SMstCommon *pMst)
{
	s32	dx;
	dx = pMst->nPosX - gShoot.nPlayerPosX;
	// Distance.
	if (ABS(dx) > (0x28 << 8)) return;
	if (ABS(pMst->nPosY - gShoot.nPlayerPosY) > (0x18 << 8)) return;
	// Joueur tourné vers le monstre ?
	if (gShoot.nPlayerDir != (dx < 0 ? 1 : 0)) return;
//SprDisplay(e_Spr_Tstrct_Cross, pMst->nPosX>>8, pMst->nPosY>>8, e_Prio_HUD);	// debug
	gShoot.nMstProximity = 1;		// Oui !
}

// Teste le coup de couteau du héros.
u32 Mst_KnifeHitCheck(struct SMstCommon *pMst, u32 nMstSpr)
{
	// Couteau ?
	if (gShoot.nPlayerKnifeSprCol == SPR_NoSprite) return (0);
	// Distance.
	if (ABS(pMst->nPosX - gShoot.nPlayerPosX) > (4 << 12)) return (0);
	if (ABS(pMst->nPosY - gShoot.nPlayerPosY) > (3 << 12)) return (0);
	// On est pas trop loin, on teste le couteau.
	return (SprCheckColBox(nMstSpr, pMst->nPosX >> 8, pMst->nPosY >> 8,
				gShoot.nPlayerKnifeSprCol, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8));
}

// Déclenche un tir de monstre si nécessaire.
// Out : 1 = Un tir a eu lieu. 0 = Pas de tir.
u32 Mst_ShotLaunch(struct SMstCommon *pMst, u32 nSpr, u32 nShotNo, u64 *pDustAnm)
{
	// Anim de tir ?
	if (AnmGetKey(pMst->nAnm) == e_AnmKey_Enemy_Shot)
	if (AnmCheckNewImgFlag(pMst->nAnm))		// Nouvelle image ?
	{
		// Est-ce qu'on a une coordonnée d'origine de tir ?
		struct SSprRect sRect1;
		if (SprGetRect(nSpr | (pMst->nFlipX ? SPR_Flip_X : 0), e_SprRectZone_ShotOrg, &sRect1))
		if (sRect1.nType == e_SprRect_Point)
		{
			FireAdd(nShotNo, pMst->nPosX + (sRect1.nX1 << 8), pMst->nPosY + (sRect1.nY1 << 8), (pMst->nFlipX ? 0 : 128));
			if (pDustAnm != NULL)
				DustSet(pDustAnm, pMst->nPosX + (sRect1.nX1 << 8), pMst->nPosY + (sRect1.nY1 << 8), e_Prio_DustOver, (pMst->nFlipX ? e_DustFlag_FlipX : 0));
			return (1);
		}
	}
	return (0);
}

// Ralentissement.
void Mst_SpeedDown(s32 *pnSpd, s32 nAcc)
{
	if (*pnSpd)
	{
		*pnSpd += (*pnSpd < 0 ? nAcc : -nAcc);
		if (ABS(*pnSpd) < nAcc) *pnSpd = 0;
	}
}

// Déplace un monstre, sauf si trou ou mur devant.
// Utilise les variables standard de la structure : nPosX, nPosY, nFlipX, nGround.
// Out : 0 = Déplacement ok / 1 = On ne peut pas avancer.
enum
{
	e_M_BM_Flag_AllowHole = 1,		// Autorise à avancer dans le vide. => Tester nGround en sortie.
};
u32 Mst_BasicMove(struct SMstCommon *pMst, s32 nSpdX, u32 nFlags)
{
	s32	nHt;
	s32	nOffset = (nSpdX < 0x100 ? 0x100 : nSpdX);

	nHt = BlockGetGroundLevel((pMst->nPosX + (pMst->nFlipX ? nOffset : -nOffset)) >> 8, pMst->nPosY >> 8);
	// Test vers le haut.
	if (nHt > -8 && nHt <= (nOffset >> 8) + 1)	// +1 : Pour les vitesses < 0x100.
	{
		pMst->nSpd = (pMst->nFlipX ? nSpdX : -nSpdX);
		pMst->nPosX += pMst->nSpd;	//(pMst->nFlipX ? nSpdX : -nSpdX);
		pMst->nPosY += nHt << 8;
		pMst->nGround = 1;
		pMst->nSpdY = 0;
		return (0);
	}

	// Test vers le bas.
	if ((nFlags & e_M_BM_Flag_AllowHole) && nHt > (nOffset >> 8) + 1)
	{
		pMst->nPosX += (pMst->nFlipX ? nSpdX : -nSpdX);
		pMst->nGround = 0;
		return (0);
	}

//printf("x=%d y=%d ht=%d\n", pMst->nPosX >> 8, pMst->nPosY >> 8, nHt);
	return (1);
}
/*
u32 Mst_BasicMove(struct SMstCommon *pMst, s32 nSpdX)
{
	s32	nHt;
	s32	nOffset = (nSpdX < 0x100 ? 0x100 : nSpdX);

	nHt = BlockGetGroundLevel((pMst->nPosX + (pMst->nFlipX ? nOffset : -nOffset)) >> 8, pMst->nPosY >> 8);
	if (nHt > -8 && nHt <= (nOffset >> 8) + 1)	// +1 : Pour les vitesses < 0x100.
	{
		pMst->nPosX += (pMst->nFlipX ? nSpdX : -nSpdX);
		pMst->nPosY += nHt << 8;
		pMst->nGround = 1;
		return (0);
	}
//printf("x=%d y=%d ht=%d\n", pMst->nPosX >> 8, pMst->nPosY >> 8, nHt);
	return (1);
}
*/

// Gravité simplifiée pour les ennemis, avec prms.
void Mst_Gravity2(struct SMstCommon *pMst, s32 nGravity, s32 nSpdYMax)
{
	pMst->nSpdY += nGravity;
	if (pMst->nSpdY > nSpdYMax) pMst->nSpdY = nSpdYMax;
	pMst->nPosY += pMst->nSpdY;

	s32	nHt;
	pMst->nGround = 0;
	nHt = BlockGetGroundLevel(pMst->nPosX >> 8, pMst->nPosY >> 8);
	if (nHt <= 0)
	{
		pMst->nPosY += nHt << 8;
		pMst->nPosY &= ~0xFF;	// Pour annuler les bits de virgule ajoutés par la gravité.
		pMst->nGround = 1;
		pMst->nSpdY = 0;
	}
}
// Gravité simplifiée pour les ennemis.
void Mst_Gravity(struct SMstCommon *pMst)
{
	Mst_Gravity2(pMst, GRAVITY/2, SPDY_MAX/2);

/*
	pMst->nSpdY += GRAVITY/2;
	if (pMst->nSpdY > SPDY_MAX/2) pMst->nSpdY = SPDY_MAX/2;
	pMst->nPosY += pMst->nSpdY;

	s32	nHt;
	pMst->nGround = 0;
	nHt = BlockGetGroundLevel(pMst->nPosX >> 8, pMst->nPosY >> 8);
//	if (nHt < 0)
	if (nHt <= 0)
	{
		pMst->nPosY += nHt << 8;
		pMst->nPosY &= ~0xFF;	// Pour annuler les bits de virgule de la gravité.
		pMst->nGround = 1;
		pMst->nSpdY = 0;
	}
*/
}

//=============================================================================

// non > pts de vie
// effets (trace de sang pour balle, bouts qui volent pour le shotgun...).
// voir pour le dégat de type "knife", pas de sang ou sang spécial ?

// En fait pour le moment ne sert qu'au rebel soldier.

// Monstres - Gestion du touché par un tir.
u32 Mst_ShotCheckHit(struct SMstCommon *pMst, u32 nSpr, u64 *pDeathAnmTb[])
{
	// Se prend un tir ?
	u32	nDamage;
	nDamage = FireHitCheck(nSpr, pMst->nPosX, pMst->nPosY, e_ShotOrg_Player, 0);
	nDamage |= Mst_KnifeHitCheck(pMst, nSpr);	// Comme ça, donne la priorité de la mort au tir.
	if (nDamage)
	{
		if (pMst->nMstNo == e_Mst14_RebelSoldier0 && pMst->nGround == 0)
		{
//		nDamage = e_ShotDamageType_Blow << 16;	// Cas spécial du rebel soldier en l'air.
			DustSetMvt(pDeathAnmTb[e_ShotDamageType_Blow], pMst->nPosX, pMst->nPosY, 0, -0x300, e_Prio_DustUnder, (pMst->nFlipX ? e_DustFlag_FlipX : 0) | e_DustFlag_Gravity);
		}
		else
		DustSet(pDeathAnmTb[nDamage >> 16], pMst->nPosX, pMst->nPosY, e_Prio_DustUnder, (pMst->nFlipX ? e_DustFlag_FlipX : 0));

//		return (e_MstState_Dead);

/*
		// Giclée de sang quand il se prend une balle.
		if (nDamage >> 16 == e_ShotDamageType_Gen)
		{
//			AnmSet(gAnm_ZombieTeen_Fall, pMst->nAnm);
			DustSet(gAnm_Blood0_FX_Dust, pMst->nPosX, pMst->nPosY, e_Prio_DustUnder, (pMst->nPosX <= gShoot.nPlayerPosX ? e_DustFlag_FlipX : 0));
//			pMst->nPhase = e_Mst7_Zombie_Fall;
		}
		// Souffle.
		else if (nDamage >> 16 == e_ShotDamageType_Blow)
		{
//			DustSet(gAnm_Zombie_DeathShotgunGen_Dust, pMst->nPosX, pMst->nPosY, e_Prio_Joueur - 8, (pMst->nFlipX ? e_DustFlag_FlipX : 0));
//			return (e_MstState_Dead);
		}
		// Brule.
		else
		{
//			AnmSet(gAnm_ZombieTeen_DeathBurnWalk, pMst->nAnm);
//			pMst->nPhase = e_Mst7_Zombie_BurnWalk;
		}
*/
		// Score.
		gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;
	}
	return (nDamage);
}

// Teste si un monstre se prend un tir, retire les points de vie qui vont bien.
// Prévu pour les monstres qui ont des points de vie. => On peut modifier ça en testant NULL pour le ptr sur Life.
/*
// Out: 0 = Le monstre n'est pas mort / 1 = Le monstre est mort.
u32 Mst_ShotCheckLife(u32 nSpr, s32 nPosX, s32 nPosY, u8 *pnHitCnt, u8 *pnLife)
{
	u32	nDamage;
	if ((nDamage = FireHitCheck(nSpr, nPosX, nPosY, e_ShotOrg_Player, 1)) != 0)
	{
		*pnHitCnt = MST_HIT_CNT;	// Compteur de frames du hit, pour palette 'hit'.

		nDamage &= 0xFFFF;
		if (nDamage > *pnLife) nDamage = *pnLife;
		*pnLife -= nDamage;

		if (*pnLife == 0) { *pnHitCnt = 0; return (1); }	// Mort.
	}
	return (0);
}
*/
// Out: 0 = Le monstre n'est pas mort / !0 = Le dernier dégat (utile pour le type de dégats).
u32 Mst_ShotCheckLife(u32 nSpr, s32 nPosX, s32 nPosY, u8 *pnHitCnt, u8 *pnLife)
{
	u32	nDamage, nPts;
	if ((nDamage = FireHitCheck(nSpr, nPosX, nPosY, e_ShotOrg_Player, 1)) != 0)
	{
//		*pnHitCnt = MST_HIT_CNT;	// Compteur de frames du hit, pour palette 'hit'.

		nPts = nDamage & 0xFFFF;
		if (nPts > *pnLife) nPts = *pnLife;
		*pnLife -= nPts;

		if (*pnLife == 0) { *pnHitCnt = 0; return (nDamage); }	// Mort.

		*pnHitCnt = MST_HIT_CNT;	// Compteur de frames du hit, pour palette 'hit'.
	}
	return (0);
}

// Out: 0 = Le monstre n'est pas mort / !0 = Le dernier dégat (utile pour le type de dégats).
u32 Mst_ShotCheckLife2(u32 nSpr, s32 nPosX, s32 nPosY, u8 *pnHitCnt, u8 *pnLife, u32 nFlags)
{
	u32	nDamage, nPts;
	if ((nDamage = FireHitCheck(nSpr, nPosX, nPosY, e_ShotOrg_Player, 1)) != 0)
	{
//		*pnHitCnt = MST_HIT_CNT;	// Compteur de frames du hit, pour palette 'hit'.

		nPts = nDamage & 0xFFFF;

		if (nFlags & e_ShotCheck_MinimizedDamage)
		{
			static	u8	nMiniCnt = 0;
			if ((++nMiniCnt) & 3) nPts /= 2;
			if (nPts == 0) nPts = 1;
			else if (nPts > 3) nPts = 3;
		}

		if (nPts > *pnLife) nPts = *pnLife;
		*pnLife -= nPts;

		if (*pnLife == 0) { *pnHitCnt = 0; return (nDamage); }	// Mort.

		*pnHitCnt = MST_HIT_CNT;	// Compteur de frames du hit, pour palette 'hit'.
	}
	return (0);
}

//=============================================================================
// Mst 0 : Plateformes mobiles.

// Phases.
enum
{
	e_Mst0_Platform1_WaitForPlayer = 0,
	e_Mst0_Platform1_MoveGo,
	e_Mst0_Platform1_MoveBack,

};

// Structure spécifique.
struct SMst0_Platform1
{
	s8	nOffsY;		// Offset du joueur sur la pf, par rapport au pt de ref.
	u16	nCnt;		// Nombre d'itérations de déplacement.

};

void Mst0_Init_Platform1(struct SMstCommon *pMst, u8 *pData)
{
	//struct SMst0_Platform1	*pSpe = (struct SMst0_Platform1 *)pMst->pData;

	static	u32 pMst0_Spr[] = { e_Spr_Platform_1, e_Spr_Platform_Lev5, e_Spr_Platform_Lev6, e_Spr_Platform_Lev6 | SPR_Flip_X };
	u32	nVal;
	s32	nDir;

	nVal = GetBits(0, 7, pData, 0);	// Sprite no.
	assert(nVal < sizeof(pMst0_Spr) / sizeof(pMst0_Spr[0]));
	pMst->nAnm = pMst0_Spr[nVal];	// On stocke le sprite ici. Si anims, faire un truc avec le bit 31.

	// Direction initiale, se trouve dans le bloc au dessus du monstre.
	nDir = Map_PathGndGetBlock(pMst->nPosX >> 12, (pMst->nPosY >> 12) - 1);
	pMst->nSpd = 0x100;
	if (nDir == 16)		// 'X' => Plateforme immobile.
	{
		pMst->nSpd = 0;	// Vitesse à zéro.
	}
	else if (nDir == -1)
	{
		fprintf(stderr, "Mst0(%d, %d): No direction found.\n", (int)(pMst->nPosX >> 12), (int)(pMst->nPosY >> 12));
		pMst->nSpd = 0;	// Vitesse à zéro.
	}
	pMst->nAngle = (nDir & 15) << 4;

	pMst->nPhase = e_Mst0_Platform1_WaitForPlayer;

}

// Teste si le joueur doit être attrapé par la plateforme.
u32 Mst0_Sub_PfGrab(struct SMstCommon *pMst, s8 *pOffsY)
{
	if (gShoot.nVehicleType >= e_HeroVehicle_SlugBase) return (0);	// Seulement si héros à pied.
 	if (gShoot.nPlayerSpdY < 0) return (0);	// Pas de test si je joueur monte.

	if (SprCheckColBox(pMst->nAnm, pMst->nPosX >> 8, pMst->nPosY >> 8,
		e_Spr_HeroLegs_Stance, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8))	// Il faut toujours tester le même sprite pour tester le même rectangle !
//		gShoot.nPlayerSprCol, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8))
	{
		struct SSprRect sRectP;
		s32	nPfPosY;

		if (SprGetRect(pMst->nAnm, e_SprRectZone_RectCol, &sRectP) == 0) return (0);	// (Normalement, ça n'arrive pas).

		nPfPosY = pMst->nPosY + (sRectP.nY1 << 8);		// Ligne supérieure de la zone de col. C'est là que posera le joueur.
		if (gShoot.nPlayerPosY - nPfPosY > SPDY_MAX) return (0);	// On est trop bas, on ne monte pas.

		// Le joueur pose sur la plateforme, on note l'offset Y.
		*pOffsY = sRectP.nY1;

		return (1);
	}

	return (0);
}

s32 Mst0_Main_Platform1(struct SMstCommon *pMst)
{
	struct SMst0_Platform1	*pSpe = (struct SMst0_Platform1 *)pMst->pData;
	u32	nSpr;
	s32	nOffsDeplX;
	s32	nLastBlkX, nLastBlkY;

	nOffsDeplX = 0;
	// Coord avant déplacement.
	nLastBlkX = pMst->nPosX >> 12;
	nLastBlkY = pMst->nPosY >> 12;

	// Si joueur pas sur une pf, on teste l'accroche.
	if (gShoot.nPfAddress == 0)
	{
		if (Mst0_Sub_PfGrab(pMst, &pSpe->nOffsY))
		{
			gShoot.nPfAddress = (u64)pMst;
		}
	}

	// Phases.
	switch (pMst->nPhase)
	{
	case e_Mst0_Platform1_WaitForPlayer:
		// Le joueur est monté ?
		if (gShoot.nPfAddress == (u64)pMst && pMst->nSpd)	// && nSpd => Si v=0 on ne change pas de phase.
		{
			pMst->nPhase = e_Mst0_Platform1_MoveGo;
			pSpe->nCnt = 0;		// RAZ nombre d'itérations de déplacement.
		}
		else
		{
			// La pf est à son point de départ. Sortie de l'écran ?
			if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY))
			{
				pMst->nAnm = -1;	// !!! Très important !!! Sinon le management va essayer de libérer une anim.
				return (e_MstState_Asleep);
			}
		}
		break;

	case e_Mst0_Platform1_MoveGo:
		// Déplacement.
		nOffsDeplX = (pMst->nSpd * gVar.pCos[pMst->nAngle]) / 256;
		pMst->nPosX += nOffsDeplX;
		pMst->nPosY += (pMst->nSpd * gVar.pSin[pMst->nAngle]) / 256;
		pSpe->nCnt++;
		// On change de bloc ? => On arrive en bout de course ?
		if (nLastBlkX != pMst->nPosX >> 12 || nLastBlkY != pMst->nPosY >> 12)
		{
			s32	nDir;
//printf("go new blk %d %d\n", pMst->nPosX >> 12, pMst->nPosY >> 12);	//debug
			nDir = Map_PathGndGetBlock(pMst->nPosX >> 12, pMst->nPosY >> 12);
			if (nDir == 16)	// Bout du chemin ?
			{
				pMst->nPhase = e_Mst0_Platform1_MoveBack;
				pMst->nAngle += 128;
			}
		}
		break;

	case e_Mst0_Platform1_MoveBack:
		// Déplacement.
		nOffsDeplX = (pMst->nSpd * gVar.pCos[pMst->nAngle]) / 256;
		pMst->nPosX += nOffsDeplX;
		pMst->nPosY += (pMst->nSpd * gVar.pSin[pMst->nAngle]) / 256;
		// On est de retour au point de départ ?
		if (--pSpe->nCnt == 0)
		{
//printf("stop %p %p\n", pMst->nPosX, pMst->nPosY);		// vérifier s'il n'y a jamais de décélage ? (qui viendrait d'une différence entre sin et sin+128)
			pMst->nPhase = e_Mst0_Platform1_WaitForPlayer;
			pMst->nAngle += 128;
		}
		break;

	}

	// Si le joueur est sur cette pf.
	if (gShoot.nPfAddress == (u64)pMst)
	{
		// Recalage du joueur après le mouvement.
		gShoot.nPlayerPosY = pMst->nPosY + ((s32)pSpe->nOffsY << 8);
		gShoot.nPlayerSpdY = 0;
		gShoot.nPlayerPosX += nOffsDeplX;
		// Test de col. Si plus de collision, plus de plateforme, on relache le joueur.
		if (SprCheckColBox(pMst->nAnm, pMst->nPosX >> 8, pMst->nPosY >> 8,
			e_Spr_HeroLegs_Stance, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8) == 0)	// Il faut toujours tester le même sprite pour tester le même rectangle !
//			gShoot.nPlayerSprCol, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8) == 0)
		{
			gShoot.nPfAddress = 0;
		}
	}

	// Affichage.
	nSpr = pMst->nAnm;//AnmGetImage(pMst->nAnm);
	SprDisplay(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_EnnemiesBg);

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 1 : Sprites d'avant plan.

void Mst1_Init_ForegroundSpr1(struct SMstCommon *pMst, u8 *pData)
{
	static	u64 pMst1_Spr[] = { e_Spr_Lev8_WagonL_fg, e_Spr_Lev8_WagonR_fg, e_Spr_Lev8_Loco_fg,
		e_Spr_Lev3_WallR_fg, e_Spr_Lev12_WallR_fg,
		e_Spr_Lev13_SubmarinePart0_fg, e_Spr_Lev13_SubmarinePart1L_fg, e_Spr_Lev13_SubmarinePart1R_fg,
		(u64)gAnm_Lev7_TubeUp_fg, (u64)gAnm_Lev7_TubeDown_fg, e_Spr_Lev13_WaterfallBottom_fg,
	};
	static	u32	nAnmFlg = (1 << 8) | (1 << 9);	// Pour indiquer les anims dans la table du dessus.
	static	u32	nFgFlg = (1 << 10);				// Pour indiquer qu'on passe au dessus de l'avant plan.
	u32	nVal;

	nVal = GetBits(0, 7, pData, 0);	// Sprite no.
	assert(nVal < sizeof(pMst1_Spr) / sizeof(pMst1_Spr[0]));

	// On se sert du b0 de nGround pour savoir si c'est une anim ou un sprite.
	if ((pMst->nGround = (nAnmFlg & (1 << nVal) ? 1 : 0)) != 0)
		pMst->nAnm = AnmSet((u64 *)pMst1_Spr[nVal], -1);	// Anim.
	else
		pMst->nAnm = pMst1_Spr[nVal];	// On stocke le sprite ici.

	// On se sert du b1 de nGround pour savoir s'il faut passer au dessus de l'avant plan.
	pMst->nGround |= (nFgFlg & (1 << nVal) ? 2 : 0);

}

s32 Mst1_Main_ForegroundSpr1(struct SMstCommon *pMst)
{
	u32	nSpr;

	// Sortie de l'écran ?
	if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY))
	{
		if ((pMst->nGround & 1) == 0) pMst->nAnm = -1;	// !!! Très important !!! Sinon le management va essayer de libérer une anim qui est dans ce cas un n° de sprite.
		return (e_MstState_Asleep);
	}

	// Affichage.
	nSpr = ((pMst->nGround & 1) ? AnmGetImage(pMst->nAnm) : pMst->nAnm);
//	SprDisplay(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_HUD-1);//255);
	SprDisplay(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8, ((pMst->nGround & 2) ? 0x100 + 10 : e_Prio_HUD-1));

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 2 : Mini UFO.

/*
[mst2] Ennemy 1
Nb = 0:7:
*/

// Phases.
enum
{
	e_Mst2_MiniUfo_Fly = 0,
	e_Mst2_MiniUfo_Turn,
	e_Mst2_MiniUfo_Shot,

};

// Structure spécifique.
struct SMst2_MiniUFO0
{
	struct SMstPath	sPath;
	u8	nSinIdx;

	s8	nShotAnm;	// Anim supplémentaire pour le tir.
	u8	nNoShotCnt;	// Ne pas tirer pendant x frames.

	u8	nHitCnt, nLife;

};

#define	MST2_MINIUFO_LIFE	7
void Mst2_Init_MiniUFO0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst2_MiniUFO0	*pSpe = (struct SMst2_MiniUFO0 *)pMst->pData;

	// Init path.
	Mst_PathInit(pMst, &pSpe->sPath, 0x180, 0);
	pSpe->nSinIdx = 0;

	pSpe->nShotAnm = -1;
	pSpe->nNoShotCnt = 0;	// Ne pas tirer pendant x frames.

	pSpe->nLife = MST2_MINIUFO_LIFE;
	pSpe->nHitCnt = 0;

	pMst->nPhase = e_Mst2_MiniUfo_Fly;

}

#define	MST2_SHOTXBLK	8
#define	MST2_SHOTYPIX	64//(48+8)
s32 Mst2_Main_MiniUFO0(struct SMstCommon *pMst)
{
	struct SMst2_MiniUFO0	*pSpe = (struct SMst2_MiniUFO0 *)pMst->pData;
	u32	nSpr;

	// Compteur de tir.
	if (pSpe->nNoShotCnt) pSpe->nNoShotCnt--;

	// Phases.
	switch (pMst->nPhase)
	{
	case e_Mst2_MiniUfo_Fly:
		{
			// Pour que les chemins "bouclent" bien, il faut des carrés et des bonnes valeurs.
			// => On ajuste en se recalant au niveau du path sur les verticales/horizontales.
			s32	nDiff;
			if (pMst->nAngle == 0 || pMst->nAngle == 128)
			{
				nDiff = pSpe->sPath.nPathPosY - pMst->nPosY;
				if (ABS(nDiff) > 0x100) pMst->nPosY += SGN(nDiff) * 0x20;
				// Tir ?
				if (MST_SHOT_COND)		// Si le héros n'est pas mort...
				if ((u32)(((pMst->nPosY + 0x2000) - gShoot.nPlayerPosY) >> 8) <= MST2_SHOTYPIX &&
					ABS(pMst->nPosX - gShoot.nPlayerPosX) >> 12 < MST2_SHOTXBLK)
				if (pMst->nPosX >= gScrollPos.nPosX + 0x1000 && pMst->nPosX <= gScrollPos.nPosX + (SCR_Width << 8) - 0x1000)	// On ne tire pas si en dehors de l'écran.
				{
					pMst->nPhase = e_Mst2_MiniUfo_Shot;
				}
			}
			if (pMst->nAngle == 64 || pMst->nAngle == 192)
			{
				nDiff = pSpe->sPath.nPathPosX - pMst->nPosX;
				if (ABS(nDiff) > 0x100) pMst->nPosX += SGN(nDiff) * 0x20;
			}
		}
		break;

	case e_Mst2_MiniUfo_Turn:
		{
			if (pMst->nAngle == pSpe->sPath.nNewAngle)
			{
				pMst->nPhase = e_Mst2_MiniUfo_Fly;
				break;
			}
			pMst->nAngle += pSpe->sPath.nSensRot << 2;
			// Sens.
			//pMst->nFlipX = ((u8)(pMst->nAngle + 64) < 128 ? 1 : 0);
		}
		break;

	case e_Mst2_MiniUfo_Shot:
		if (MST_SHOT_COND == 0) break;		// Si le héros est mort, stop.

		if (pSpe->nNoShotCnt == 0)
		{
			pSpe->nShotAnm = AnmSet(gAnm_MarsUFO_ShotFx, pSpe->nShotAnm);
			pSpe->nNoShotCnt = 63;
		}
		// Si anim de tir finie, on regarde si le joueur est toujours dans la zone et que le monstre est tjs à l'écran.
		if (pSpe->nShotAnm == -1)
		if ((u32)(((pMst->nPosY + 0x2000) - gShoot.nPlayerPosY) >> 8) > MST2_SHOTYPIX ||
			ABS(pMst->nPosX - gShoot.nPlayerPosX) >> 12 >= (MST2_SHOTXBLK + 1) ||
			(pMst->nPosX >= gScrollPos.nPosX + 0x1000 && pMst->nPosX <= gScrollPos.nPosX + (SCR_Width << 8) - 0x1000) == 0)
		{
			pMst->nPhase = e_Mst2_MiniUfo_Fly;
		}
		break;

	}

	// Déplacement sur path.
	if (pMst->nPhase <= e_Mst2_MiniUfo_Turn)
		Mst_PathMove(pMst, &pSpe->sPath, 0x80, e_Mst2_MiniUfo_Turn);

	// Oscillation.
	s32	nMstPosY_sav = pMst->nPosY;
	pMst->nPosY -= gVar.pSin[pSpe->nSinIdx] << 2;
	pSpe->nSinIdx += 2;

	nSpr = AnmGetImage(pMst->nAnm) ^ (pMst->nFlipX ? SPR_Flip_X : 0);

	ChaserTarget_AddToList(pMst->nPosX >> 8, (pMst->nPosY >> 8) - 16);	// Ajout dans la table de visée des missiles.

	// Test des tirs.
	if (Mst_ShotCheckLife(nSpr, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife))
	{
		DustSet(gAnm_MiniUFO_Explode_Dust, pMst->nPosX, pMst->nPosY, e_Prio_DustUnder, 0);	//(pMst->nFlipX ? e_DustFlag_FlipX : 0)
		// Score.
		gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;
		return (e_MstState_Dead);
	}

	// Gestion du tir.
	struct SSprRect sRect1;
	u32	nSprShot = SPR_NoSprite;
	u32	nShotOffsX = 0, nShotOffsY = 0;
	if (pSpe->nShotAnm != -1)
	{
		nSprShot = AnmGetImage(pSpe->nShotAnm);
		// Offset du tir.
		if (SprGetRect(nSpr ^ (pMst->nFlipX ? SPR_Flip_X : 0), e_SprRectZone_ShotOrg, &sRect1))
		if (sRect1.nType == e_SprRect_Point)
		{
			nShotOffsX = sRect1.nX1;
			nShotOffsY = sRect1.nY1;
		}
		// Fin d'anim ?
		if (AnmCheckEnd(pSpe->nShotAnm))
		{
			AnmReleaseSlot(pSpe->nShotAnm);
			pSpe->nShotAnm = -1;
			nSprShot = SPR_NoSprite;
		}
		// Tir ?
		else if (AnmCheckStepFlag(pSpe->nShotAnm))
		{
			FireAdd(e_Shot_Enemy_MarsUFO_Bullet0, pMst->nPosX + (nShotOffsX << 8), pMst->nPosY + (nShotOffsY << 8), (gShoot.nPlayerPosX > pMst->nPosX ? 0 : 128));
		}
	}

	// Affichage.
	if (pSpe->nHitCnt)
	{
		pSpe->nHitCnt--;
		pSpe->nNoShotCnt++;		// Si hit, on incrémente le ShotCnt pour tirer moins souvent.
	}
	SprDisplay(nSpr | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies + gnMstPrio);
	// Affichage de l'éclat du tir.
	if (nSprShot != SPR_NoSprite)
		SprDisplay(nSprShot | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), (pMst->nPosX >> 8) + nShotOffsX, (pMst->nPosY >> 8) + nShotOffsY, e_Prio_Ennemies + 1 + gnMstPrio);

	// Restore pos Y.
	pMst->nPosY = nMstPosY_sav;

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 3 : Prisoner Of War.

/*
[mst3] POW
Type = 0:2: Sitting - Totem - Hanging
Flee(1) = 3:3: Disappear - Run
Drop = 4:7: Nothing - S_Shotgun - H_Machinegun - F_Flamethrower - R_Rocket - Gas_Tank - Ammo_Box1
*/

#define	MST3_SPDX	0x200
#define	MST3_POW_MAX	3

// Phases.
enum
{
	e_Mst3_POW1_Wait = 0,
	e_Mst3_POW1_Free,
	e_Mst3_POW1_Drop,
	e_Mst3_POW1_FleeSelect,
	e_Mst3_POW1_Flee,

};

// Structure spécifique.
struct SMst3_POW1
{
	u8	nPOWNo;			// N° du type de POW.
	u8	nWeaponDrop;	// 0 ne lache rien. Sinon n° pour Mst4 (faire -1 quand on passe le n°).
	u8	nFleeType;		// 0 disparait / 1 court.
};

void Mst3_Init_POW1(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst3_POW1	*pSpe = (struct SMst3_POW1 *)pMst->pData;
	static	u64 *pMst3_AnmInit[MST3_POW_MAX] = { gAnm_POW_SittingWait, gAnm_POW_TotemWait, gAnm_POW_HangingWait, };

	u32	nVal;
	nVal = GetBits(0, 2, pData, 0);	// Sprite no.
	assert(nVal < MST3_POW_MAX);//sizeof(pMst3_AnmInit) / sizeof(pMst3_AnmInit[0]));
	pSpe->nPOWNo = nVal;
	AnmSet(pMst3_AnmInit[nVal], pMst->nAnm);
	if (pSpe->nPOWNo == 2)
		pMst->nPosY += (53 - 16) << 8;	// Décalage pour le POW hanging.

	nVal = GetBits(3, 3, pData, 0);	// Type de fuite.
	pSpe->nFleeType = nVal;

	nVal = GetBits(4, 7, pData, 0);	// Drop weapon no.
	assert(nVal <= 6);//e_Player_Weapon_Max);
	pSpe->nWeaponDrop = nVal;

	if (pSpe->nPOWNo != 2) Enemy_GroundLevel(pMst);	// Au sol, sauf le suspendu.

	pMst->nPhase = e_Mst3_POW1_Wait;

}

s32 Mst3_Main_POW1(struct SMstCommon *pMst)
{
	struct SMst3_POW1	*pSpe = (struct SMst3_POW1 *)pMst->pData;
	u32	nDamage;
	u32	nSpr;

	// Gravité.
	if ((pSpe->nPOWNo == 2 && pMst->nPhase == e_Mst3_POW1_Wait) == 0)	// Sauf suspendu en attente.
		Mst_Gravity(pMst);

	// Phases.
	switch (pMst->nPhase)
	{
	case e_Mst3_POW1_Wait:		// Attend que le joueur le touche.
		// Sortie de l'écran ?
		if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);

		Mst_ProximityCheck(pMst);	// Donne le signal pour un éventuel coup de couteau.

		// Le joueur libère le POW avec un coup de couteau ou un tir ?
		nSpr = AnmGetLastImage(pMst->nAnm);
		nDamage = FireHitCheck(nSpr, pMst->nPosX, pMst->nPosY, e_ShotOrg_Player, 0);
		nDamage |= Mst_KnifeHitCheck(pMst, nSpr);	// Comme ça, donne la priorité de la mort au tir.
		if (nDamage)
		{
			static	u64 *pMst3_AnmFree[MST3_POW_MAX] = { gAnm_POW_SittingFree, gAnm_POW_TotemFree, gAnm_POW_Fall, };
			static	u64 *pMst3_AnmFreeDust[MST3_POW_MAX] = { NULL, gAnm_POW_TotemDisappear_Dust, gAnm_POW_Hanging_RopeEndDisappear_Dust, };
			// Anim de libération.
			AnmSet(pMst3_AnmFree[pSpe->nPOWNo], pMst->nAnm);
			// Dust en plus ?
			if (pMst3_AnmFreeDust[pSpe->nPOWNo] != NULL) DustSet(pMst3_AnmFreeDust[pSpe->nPOWNo], pMst->nPosX, pMst->nPosY, e_Prio_DustUnder - 1, 0);
			// Phase.
			pMst->nPhase = (pMst->nGround ? e_Mst3_POW1_Free : e_Mst3_POW1_Flee);
			// Compteur.
			gShoot.nFreedPrisoners++;	// Nb de prisonniers libérés.
			// Score.
			gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;
		}
		break;

	case e_Mst3_POW1_Free:		// En train de se libérer.
		// A-t-on fini l'anim de libération ? (Key pour idle == e_AnmKey_Null).
		if (AnmGetKey(pMst->nAnm) != e_AnmKey_Null) break;
		// Oui. Doit-on donner une item ?
		if (pSpe->nWeaponDrop)
		{
			AnmSet(gAnm_POW_BigPants, pMst->nAnm);		// Anim de caleçon.
			pMst->nPhase = e_Mst3_POW1_Drop;
			break;
		}
		// !! Pas de break !!
	case e_Mst3_POW1_FleeSelect:	// Selection du type de fuite.
// tourner le salut vers le joueur ?
		// Fuite en courant ?
		if (pSpe->nFleeType)
		{
			AnmSet(gAnm_POW_Salute, pMst->nAnm);		// Anim de salut sans disparition.
			pMst->nPhase = e_Mst3_POW1_Flee;
			break;
		}
		// Défaut : Salut + disparition.
		DustSet(gAnm_POW_SaluteDisappear_Dust, pMst->nPosX, pMst->nPosY, e_Prio_DustUnder, 0);
		return (e_MstState_Dead);
		break;

	case e_Mst3_POW1_Drop:		// Big pants !
		// A-t-on fini l'anim de drop ? (Key pour idle == e_AnmKey_Null).
		if (AnmGetKey(pMst->nAnm) == e_AnmKey_Null)
		{
			// Oui, salut.
			AnmSet(gAnm_POW_Salute, pMst->nAnm);		// Anim de salut sans disparition.
			pMst->nPhase = e_Mst3_POW1_FleeSelect;
			break;
		}
		// Drop ?
		if (AnmCheckStepFlag(pMst->nAnm))
		{
			struct SSprRect rRect;
			if (SprGetRect(AnmGetLastImage(pMst->nAnm), e_SprRectZone_RectCol, &rRect))
			{
				// On génère un monstre Weapon Capsule.
/*
[mst4] Weapon Capsule
Type = 0:3: S_Shotgun - H_Machinegun - F_Flamethrower - R_Rocket - Gas_Tank - Ammo_Box1
Boxed = 4:4: No - Yes		; Une caisse contient l'item ?
Location = 5:6: Normal - Space - Underwater - Air
*/
				u32	nData;
				nData = (pSpe->nWeaponDrop - 1);
				MstAdd(e_Mst4_WeaponCapsule, (pMst->nPosX >> 8) + rRect.nX1, (pMst->nPosY >> 8) + rRect.nY1, (u8 *)&nData, -1);
				//
				pSpe->nWeaponDrop = 0;	// RAZ, sert de flag pour le 'hanging'.
			}
		}
		break;

	case e_Mst3_POW1_Flee:
		// Sortie de l'écran ?
		if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Dead);

		if (AnmGetKey(pMst->nAnm) != e_AnmKey_Null) break;		// Attente de ne plus être en "salut".
		if (pMst->nGround)
		{
			if (pSpe->nWeaponDrop)	// Le 'hanging' peut arriver là avant de lacher son item.
			{
				pMst->nPhase = e_Mst3_POW1_Free;
				break;
			}
			if (Mst_BasicMove(pMst, MST3_SPDX, e_M_BM_Flag_AllowHole)) pMst->nFlipX ^= 1;	// Mur ? => Flip.
			AnmSetIfNew(gAnm_POW_Run, pMst->nAnm);		// Anim de course.
		}
		else
		{
			// Ralentissement X pendant la chute.
			Mst_SpeedDown(&pMst->nSpd, 0x20);
			pMst->nPosX += pMst->nSpd;
//			// Gravité.
//			Mst_Gravity(pMst);
			AnmSetIfNew(gAnm_POW_Fall, pMst->nAnm);		// Anim de chute.
		}
		break;
	}

	nSpr = AnmGetImage(pMst->nAnm);

	// Affichage.
	SprDisplay(nSpr | (pMst->nFlipX ? SPR_Flip_X : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies);// + gnMstPrio);

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 4 : Weapon Capsule.

/*
[mst4] Weapon Capsule
Type = 0:3: S_Shotgun - H_Machinegun - F_Flamethrower - R_Rocket - Gas_Tank - Ammo_Box1
Boxed = 4:4: No - Yes		; Une caisse contient l'item ?
Location = 5:6: Normal - Space - Underwater - Air
*/

// Structure spécifique.
struct SMst4_WeaponCapsule1
{
	u8	nWeaponNo;	// No de l'arme à passer au joueur.
	u8	nAltered;	// Pour flaguer le chgt d'état entre boxed et non boxed.

	u8	nCnt;		// Pour sinus L11.
	s32	nPosXOrg, nPosYOrg;
};

// Ordre : S - H - F - R = Même ordre que les armes du héros (à partir de e_Player_Weapon_Shotgun).
u64 *gpMst4_AnmInit[] = {
	gAnm_Itm_WeaponCapsuleS, gAnm_Itm_WeaponCapsuleH, gAnm_Itm_WeaponCapsuleF, gAnm_Itm_WeaponCapsuleR,
	gAnm_Itm_GasTank1, gAnm_Itm_AmmoBox1 };

#define	MST04_VAL	8
void Mst4_Init_WeaponCapsule1(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst4_WeaponCapsule1	*pSpe = (struct SMst4_WeaponCapsule1 *)pMst->pData;

	s32	nVal;
	nVal = GetBits(0, 3, pData, 0);	// Type d'arme.
	pSpe->nWeaponNo = nVal;
	nVal = GetBits(5, 6, pData, 0);	// Location.
	pSpe->nWeaponNo += nVal * MST04_VAL;	// Offset, +MST04_VAL = espace, +MST04_VAL*2 = underwater.
	nVal = GetBits(4, 4, pData, 0);	// Boxed ?
	AnmSet((nVal ? gAnm_Itm_Container1 : gpMst4_AnmInit[pSpe->nWeaponNo & (MST04_VAL - 1)]), pMst->nAnm);
	pMst->nPhase = nVal;	// 0 = cas normal / 1 = boxed.
	pSpe->nAltered = 0;
	pSpe->nCnt = 0;

	// Mise au niveau du sol, mais seulement si dans du dur.
	nVal = BlockGetGroundLevel(pMst->nPosX >> 8, pMst->nPosY >> 8);
	if (nVal < 0) pMst->nPosY += nVal << 8;

	// On sauvegarde les pos x et y originales.
	pSpe->nPosXOrg = pMst->nPosX;
	pSpe->nPosYOrg = pMst->nPosY;

}

void Rocket_WeaponSet(u32 nWeaponNo);
#define	PLYR_NAKED_BOMBS_MAX	20

#define	MST04_CAPSULE_AREA	5
s32 Mst4_Main_WeaponCapsule1(struct SMstCommon *pMst)
{
	struct SMst4_WeaponCapsule1	*pSpe = (struct SMst4_WeaponCapsule1 *)pMst->pData;
	u32	nSpr;

	// Sortie de l'écran ?
	if (Screen_ObjectOutRect(pMst->nPosX - (MST04_CAPSULE_AREA << 12), pMst->nPosY, MST04_CAPSULE_AREA*2, 1))
		return (pMst->nLoadIdx == -1 || pSpe->nAltered ? e_MstState_Dead : e_MstState_Asleep);

	// Mouvement de la capsule.
	switch (pSpe->nWeaponNo >> 3)
	{
	case 1:		// Espace : Pas de gravité et petit sinus.
		pMst->nPosY += 0x80;
		pMst->nPosX += gVar.pCos[pSpe->nCnt];
		pSpe->nCnt += 2;
		break;

	case 3:		// Air.
		pMst->nPosX -= 0x80;
	case 2:		// Underwater.
		pMst->nPosY = pSpe->nPosYOrg + (gVar.pCos[pSpe->nCnt] << 1);
		pSpe->nCnt += 3; //2
		break;

	case 0:		// Normal : Gravité.
	default:
		Mst_Gravity(pMst);
		break;
	}

	// Boxed ?
	if (pMst->nPhase)
	{
		nSpr = AnmGetImage(pMst->nAnm);
		// Se prend un tir ?
		if (FireHitCheck(nSpr, pMst->nPosX, pMst->nPosY, e_ShotOrg_Player, 0))
		{
			// Dust de disparition de la caisse.
			DustSet(gAnm_Itm_Container1_Dust, pMst->nPosX, pMst->nPosY, e_Prio_DustUnder, 0);
			// Repasse l'anim sur celle de l'item.
			AnmSet(gpMst4_AnmInit[pSpe->nWeaponNo], pMst->nAnm);
			pMst->nPhase = 0;	// Repasse au cas normal.
			pSpe->nAltered = 1;	// Chgt d'état.
		}
		else
		{
			// Affichage.
			SprDisplay(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies - 1);	// On affiche les boites en dessous des autres monstres.
			return (e_MstState_Managed);
		}
	}

	// Contact avec le joueur ?
	nSpr = AnmGetLastImage(pMst->nAnm);
	if (SprCheckColBox(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8,
		gShoot.nPlayerSprCol, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8))
	{
		switch (pSpe->nWeaponNo & (MST04_VAL - 1))
		{
		case 0:		// Armes.
		case 1:
		case 2:
		case 3:
			// Donne l'arme au joueur.
			if ((pSpe->nWeaponNo & ~(MST04_VAL - 1)) == MST04_VAL)
				Rocket_WeaponSet(e_Player_Weapon_Machinegun);	// Fusée, H seulement.
			else
				Player_WeaponSet(e_Player_Weapon_Shotgun + (pSpe->nWeaponNo & (MST04_VAL - 1)));	// Cas normal.
			break;
		case 4:		// Gas tank.
			Player_SlugRefuel(SLUG_ENERGY_MAX / 2);
			break;
		case 5:		// Ammo box 1.
			Player_WeaponReload(PLYR_NAKED_BOMBS_MAX / 2);
			break;
		}
		// Dust de disparition + mort.
		DustSet(gAnm_Itm_WeaponCapsulePickUp_Dust, pMst->nPosX, pMst->nPosY, e_Prio_DustUnder, 0);
		gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;	// Score.
		return (e_MstState_Dead);
	}

	// Affichage.
	nSpr = AnmGetImage(pMst->nAnm);
	SprDisplay(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies + MSTPRIO_AND + 1);	// On affiche les capsules au dessus des autres monstres.

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 5 : Scroll Lock X.

// Phases.
enum
{
	e_Mst5_ScrollLock_WaitPos = 0,
	e_Mst5_ScrollLock_WaitItem,
};

// Structure spécifique.
struct SMst5_ScrollLockX1
{
	u8	nItemGive;	// N° item de quête à donner une fois le scroll locké.
	u8	nItemWait;	// N° item de quête attendu pour libérer le scroll.
	u8	nBlkLg;		// Largeur en blocs à bloquer.
};

void Mst5_Init_ScrollLockX1(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst5_ScrollLockX1	*pSpe = (struct SMst5_ScrollLockX1 *)pMst->pData;
	s32	nVal;

	pMst->nPosX &= ~0xFFF;	// Recalage sur la colonne 0.

	nVal = GetBits(0, 7, pData, 0);		// N° item de quête à donner une fois le scroll locké.
	pSpe->nItemGive = nVal;
	nVal = GetBits(8, 15, pData, 0);	// N° item de quête attendu pour libérer le scroll.
	pSpe->nItemWait = nVal;
	nVal = GetBits(16, 23, pData, 0);	// Largeur en blocs à bloquer.
	if (nVal < SCR_Width / 16) nVal = SCR_Width / 16;
	pSpe->nBlkLg = nVal;

	// Sens pour savoir dans quel sens tester le dépassement : 0 droite / 1 gauche.
	pMst->nFlipX = (pMst->nPosX >= gScrollPos.nPosX ? 0 : 1);

	pMst->nPhase = e_Mst5_ScrollLock_WaitPos;

}

// tst
extern s32	gnScrollLimitXMin;
extern s32	gnScrollLimitXMax;
extern s32	gnScrollLimitYMin;
extern s32	gnScrollLimitYMax;

s32 Mst5_Main_ScrollLockX1(struct SMstCommon *pMst)
{
	struct SMst5_ScrollLockX1	*pSpe = (struct SMst5_ScrollLockX1 *)pMst->pData;

	// Phases.
	switch (pMst->nPhase)
	{
	case e_Mst5_ScrollLock_WaitPos:		// Attend que le scroll arrive à la position.
		// Sortie de l'écran ?
		if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);
		//
		if ((pMst->nFlipX == 0 && gScrollPos.nPosX >= pMst->nPosX) ||
			(pMst->nFlipX == 1 && gScrollPos.nPosX <= pMst->nPosX))
		{
			gnScrollLimitXMin = pMst->nPosX;
			gnScrollLimitXMax = pMst->nPosX + (pSpe->nBlkLg << 12);

			gpMstQuestItems[pSpe->nItemGive] = 1;
			pMst->nPhase = e_Mst5_ScrollLock_WaitItem;

			if (pSpe->nItemWait == 0)return (e_MstState_Dead);		// Si 0, ne libère jamais le scroll (boss).
		}
		break;

	case e_Mst5_ScrollLock_WaitItem:	// Attend une item de quête pour libérer le scroll.
		if (gpMstQuestItems[pSpe->nItemWait])
		{
			// Empêche le retour à gauche dans les scroll forcés vers la droite.
			if (gScrollPos.nScrollType == e_ScrollType_RightOnly)
				gScrollPos.nLastPosX = gnScrollLimitXMin + (((SCR_Width / 16) / 3) << 12);
			// Relâche le scroll.
			gnScrollLimitXMin = gnScrollLimitXMax = -1;
			return (e_MstState_Dead);
		}
		break;
	}

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 6 : R-Shobu (Half-boss).

/*
[mst6] R-Shobu (half-boss)
Wait itm = 0:7:			; N° item de quête attendu pour démarrer.
Give itm = 8:15:		; N° item de quête à donner une fois mort pour libérer le scroll.
*/

// Phases.
enum
{
	e_Mst6_RShobu_WaitItem = 0,
	e_Mst6_RShobu_Arrival,
	e_Mst6_RShobu_Normal,

};

// Structure spécifique.
struct SMst6_RShobu1
{
	u8	nItemWait;	// N° item de quête attendu pour démarrer.
	u8	nItemGive;	// N° item de quête à donner une fois mort pour libérer le scroll.

	u8	nFrmCnt0;	// Compteur incrémenté à chaque frame pour affichage des hélices.
	u8	nFrmCnt1;	// Compteur pour les inclinaisons.
	u8	nSinIdx;	// Compteur pour le sinus.
	u8	nInclin;	// Inclinaison : 0 recule > 3 stationnaire > 6 avance.
	u8	nLife;		// Points de vie.
	u8	nHitCnt;	// Pour affichage rougi.
	u8	nFrmShot;	// Compteur pour les tirs.

};

#define	MST6_LIFE	30
void Mst6_Init_RShobu1(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst6_RShobu1	*pSpe = (struct SMst6_RShobu1 *)pMst->pData;
	u32	nVal;

	nVal = GetBits(0, 7, pData, 0);		// N° item de quête attendu pour démarrer.
	pSpe->nItemWait = nVal;
	nVal = GetBits(8, 15, pData, 0);	// N° item de quête à donner une fois mort pour libérer le scroll.
	pSpe->nItemGive = nVal;

	pSpe->nFrmCnt0 = 0;
	pSpe->nFrmCnt1 = 0;
	pSpe->nSinIdx = 1;		// De 2 en 2, on commence à 1 pour éviter les 256 sur les angles 0, 64, 128 et 256.
	pSpe->nLife = MST6_LIFE;	// Points de vie.
	pSpe->nHitCnt = 0;
	pSpe->nFrmShot = 0;

	pMst->nPhase = e_Mst6_RShobu_WaitItem;

}

// Déplacement vers le joueur.
#define	RSHOBU_ACCX		0x20
#define	RSHOBU_SPDX_MAX	0x200
void Mst6_MoveToPlayer(struct SMstCommon *pMst)
{
	s32	nDist;

	nDist = gShoot.nPlayerPosX - pMst->nPosX;
//	nDist = gShoot.nPlayerPosX - (pMst->nPosX - 0x800);	// tst pour tirer un peu à côté du joueur.
	if (ABS(nDist) > (1 << 12))
	{
		// Accélération.
		if (nDist > 0)
		{
			pMst->nSpd += RSHOBU_ACCX;
			if (pMst->nSpd > RSHOBU_SPDX_MAX) pMst->nSpd = RSHOBU_SPDX_MAX;
		}
		else
		{
			pMst->nSpd -= RSHOBU_ACCX;
			if (pMst->nSpd < -RSHOBU_SPDX_MAX) pMst->nSpd = -RSHOBU_SPDX_MAX;
		}
	}
	else
	{
		// Ralentissement.
		pMst->nSpd += (pMst->nSpd < 0 ? RSHOBU_ACCX : -RSHOBU_ACCX);
		if (ABS(pMst->nSpd) <= RSHOBU_ACCX) pMst->nSpd = 0;
	}
	pMst->nPosX += pMst->nSpd;

}

s32 Mst6_Main_RShobu1(struct SMstCommon *pMst)
{
	struct SMst6_RShobu1	*pSpe = (struct SMst6_RShobu1 *)pMst->pData;
	s32	nDist;

	nDist = gShoot.nPlayerPosX - pMst->nPosX;

	switch (pMst->nPhase)
	{
	case e_Mst6_RShobu_WaitItem:		// Attend une item de quête avant d'apparaître.
		if (gpMstQuestItems[pSpe->nItemWait])
		{
			pMst->nPhase = e_Mst6_RShobu_Arrival;
			// Position. (On pourrait l'initialiser de l'autre côté en fct de la pos du joueur).
			pMst->nPosX = gScrollPos.nPosX + (((SCR_Width * 3) / 4) << 8);
			pMst->nPosY = gScrollPos.nPosY - (1 << 12);
			pMst->nSpd = 0;
			pMst->nFlipX = 0;
			pSpe->nInclin = 3;
		}
		else
		{
			// Sortie de l'écran ?
			if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);
		}
		return (e_MstState_Managed);
		//break;

	case e_Mst6_RShobu_Arrival:			// Descente dans l'écran.
		pMst->nPosY += 1 << 8;
		if (pMst->nPosY >= gScrollPos.nPosY + (5 << 12))
		{
			pMst->nPhase = e_Mst6_RShobu_Normal;
		}
		break;

	case e_Mst6_RShobu_Normal:			// Vol stationnaire.
		pSpe->nSinIdx += 2;		// Sinus du vol stationnaire.
		if (MST_SHOT_COND)	// Si le héros n'est pas mort...
			if (pSpe->nFrmShot == 0 && ABS(nDist) < (3 << 12)) pSpe->nFrmShot = (16 * 3) << 1;	// Tir ?
		break;
	}

	if ((gnFrame & 7) == 0) Sfx_PlaySfx(e_Sfx_Heli0, e_SfxPrio_30);		// Sfx.

	u32	nInclinReq;
	s32	nSensDepl;

	// Déplacement vers le joueur.
	Mst6_MoveToPlayer(pMst);
	// Inclinaison souhaitée en fct de la vitesse.
	nInclinReq = (((6 * ((pMst->nFlipX ? pMst->nSpd : -pMst->nSpd) + RSHOBU_SPDX_MAX)) / 4) >> 8);

	// Doit-on se retourner ?
	nSensDepl = (pMst->nSpd < 0 ? 0 : 1) ^ pMst->nFlipX;
	// Si on recule et que le joueur est trop loin, flip.
	if (nSensDepl && ABS(nDist) > (3 << 12))
	{
		AnmSetIfNew(gAnm_RShobu_Flip, pMst->nAnm);
	}
	// Si flip, repasse en inclinaison 3.
	if (AnmGetKey(pMst->nAnm) == e_AnmKey_Enemy_Flip) nInclinReq = 3;

	// Passe dans l'inclinaison demandée.
	if (pSpe->nInclin != nInclinReq)
	{
		if ((++pSpe->nFrmCnt1 & 3) == 0)
			pSpe->nInclin += (pSpe->nInclin < nInclinReq ? 1 : -1);
	}

	// Ajout dans la table de visée des missiles.
	ChaserTarget_AddToList(pMst->nPosX >> 8, (pMst->nPosY >> 8) - 16);

	// Petite série de largage de bombe ?
	if (pSpe->nFrmShot)
	{
		if ((pSpe->nFrmShot & 15) == 0 && (pSpe->nFrmShot & 64))
		{
			FireAdd(e_Shot_Enemy_RShobu_Bomb_Drop, pMst->nPosX, pMst->nPosY, -1);	// Tir.
			DustSet(gAnm_RShobu_BombDrop_Dust, pMst->nPosX, pMst->nPosY, e_Prio_DustUnder, 0);	// Dust.
		}
		pSpe->nFrmShot--;
	}

	// Préparation de l'affichage.
	static u32	pAnmBlades[] = { 0, 1, 0, 2, 0, 3, 0, 4 };
	u32	nSpr;

	// Flip ?
	if (AnmGetKey(pMst->nAnm) == e_AnmKey_Enemy_Flip && pSpe->nInclin == 3)
	{
		// Gestion du bit de flip lors des retournements.
		nSpr = AnmGetImage(pMst->nAnm);
		if (AnmCheckStepFlag(pMst->nAnm)) pMst->nFlipX ^= 1;
		//pSpe->nInclin = 3;
	}
	else
	{
		nSpr = e_Spr_RShobu_Flying + pSpe->nInclin;
	}

	// Petite oscillation quand vol stationnaire.
	s32	nPosY_sav = pMst->nPosY;
	pMst->nPosY += gVar.pSin[pSpe->nSinIdx] << 2;

/*
{
	// vie.
	char	pTb[8+1] = "00000000";
	MyItoA(pSpe->nLife, pTb);
	Font_Print(10, 60, pTb, 0);
}
*/

	// Se prend un tir ?
	if (pSpe->nLife)
	if (Mst_ShotCheckLife2(nSpr, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife, e_ShotCheck_MinimizedDamage))
	{
		// Mort.
		DustSet(gAnm_Explosion0_Big_Dust, pMst->nPosX, pMst->nPosY, e_Prio_DustUnder, 0);
		DustSetMvt(gAnm_Debris_Metal0_Dust, pMst->nPosX, pMst->nPosY, 0x180, -0x480, e_Prio_DustUnder - 1, e_DustFlag_Gravity);
		DustSetMvt(gAnm_Debris_Metal0_Dust, pMst->nPosX, pMst->nPosY, -0x180, -0x380, e_Prio_DustUnder - 1, e_DustFlag_Gravity);
		gpMstQuestItems[pSpe->nItemGive] = 1;
		gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;	// Score.
		FireRemoveFromList((1<<31) | e_ShotOrg_Enemy);			// Supprime les tirs actifs.
		return (e_MstState_Dead);
	}

	// Affichage.
	if (pSpe->nHitCnt) pSpe->nHitCnt--;
	SprDisplay((nSpr ^ (pMst->nFlipX ? SPR_Flip_X : 0)) | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies);// + gnMstPrio);
	SprDisplay(((e_Spr_RShobu_Blades + (pSpe->nInclin * 5) + pAnmBlades[(pSpe->nFrmCnt0 >> 2) & 7]) ^ (pMst->nFlipX ? SPR_Flip_X : 0)) | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), pMst->nPosX >> 8, (pMst->nPosY >> 8) - 50, e_Prio_Ennemies + 1);// + gnMstPrio);
	pSpe->nFrmCnt0++;	// Pour avance hélice.

	pMst->nPosY = nPosY_sav;	// Restore pos y.

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 7 : Gauche/droite basique.

/*
[mst7] Gauche/Droite
Type = 0:3: Zombie Teen - Zombie Fat - Mars People - Brain Bot -
Zone = 4:7:
*/

#define	MST7_ZOMBIE_SPDX		0		// Modif pour avance image par image.
#define	MST7_ZOMBIE_SHOTX		4
#define	MST7_MARSPEOPLE_SPDX	0x100
#define	MST7_MARSPEOPLE_SHOTX	10
#define	MST7_MARSPEOPLE_LIFE	7
#define	MST7_BRAINBOT_SPDX		0x200
#define	MST7_BRAINBOT_SHOTX		8
#define	MST7_BRAINBOT_LIFE		10

// Phases communes.
enum
{
	e_Mst7_LeftRight_Idle = 0,
	e_Mst7_LeftRight_Walk,
	e_Mst7_LeftRight_CommonLast,
};

// Structure commune.
struct SMst7_LeftRight0
{
//	u16	nZoneMin, nZoneMax;
	s16	nZoneMin, nZoneMax;
	u8	nType;

	u8	nOrgPhase;		// !!! Attention, vrai bug vicieux !!! La phase peut changer dans les traitements communs. Du coup, quand on appelle les traitements spécifiques, on n'est plus dans la même phase ! => Ex : Le rifle qui se lève/descend/se lève...
	u8	nShotCnt;
	u8	nNervousCnt;	// Compteur pour "nervosité".
	u32 nLastAnmKey;
	u16	nCurSpd;		// Vitesse en cours.

	u8	nLife;		// Points de vie (pas zombies).
	u8	nHitCnt;	// Pour affichage rougi (pas zombies).

};

// Anims du Mst7, tables communes.
u64	*gpMST7_AnmWaitTb[] = { gAnm_ZombieTeen_Idle, gAnm_ZombieFat_Idle, gAnm_MarsPeople_Idle, gAnm_BrainBot_Idle, };
u64	*gpMST7_AnmWalkTb[] = { gAnm_ZombieTeen_Walk, gAnm_ZombieFat_Walk, gAnm_MarsPeople_Idle, gAnm_BrainBot_Walk, };
u16	gpMST7_WalkSpdTb[] = { MST7_ZOMBIE_SPDX, MST7_ZOMBIE_SPDX, MST7_MARSPEOPLE_SPDX, MST7_BRAINBOT_SPDX, };
u16	gpMST7_WalkSpdNervousTb[] = { MST7_ZOMBIE_SPDX, MST7_ZOMBIE_SPDX, MST7_MARSPEOPLE_SPDX*2, MST7_BRAINBOT_SPDX, };
u64	*gpMST7_AnmFlipTb[] = { gAnm_ZombieTeen_Flip, gAnm_ZombieFat_Flip, gAnm_MarsPeople_Flip, gAnm_BrainBot_Flip, };
u64	*gpMST7_AnmShotTb[] = { gAnm_ZombieTeen_Shot, gAnm_ZombieFat_Shot, gAnm_MarsPeople_Shot, gAnm_BrainBot_Shot, };
u64	*gpMST7_AnmShotFxTb[] = { gAnm_ZombieTeen_ShotFX_Dust, gAnm_ZombieFat_ShotFX_Dust, NULL, NULL, };
u8	gpMST7_ShotTb[] = { e_Shot_Enemy_ZombieBrain, e_Shot_Enemy_ZombieBrain, e_Shot_Enemy_MarsUFO_Bullet0, e_Shot_Enemy_MarsUFO_Bullet0, };
u8	gpMST7_ShotDstXTb[] = { MST7_ZOMBIE_SHOTX, MST7_ZOMBIE_SHOTX, MST7_MARSPEOPLE_SHOTX, MST7_BRAINBOT_SHOTX, };
u8	gpMST7_ShotDstXMinTb[] = { 0x14, 0x14, 0x28, 0x28, };
u8	gpMST7_ShotDelay[] = { 96, 96, 32, 32, };
u8	gpMST7_LifePtsTb[] = { 0, 0, MST7_MARSPEOPLE_LIFE, MST7_BRAINBOT_LIFE, };

// Zombies.
u64	*gpMst7_Zombie_AnmFallTb[] = { gAnm_ZombieTeen_Fall, gAnm_ZombieFat_Fall, NULL, };
u64	*gpMst7_Zombie_AnmDeathBurnTb[] = { gAnm_ZombieTeen_DeathBurnWalk, gAnm_ZombieFat_DeathBurnWalk, NULL, };
u64	*gpMst7_Zombie_AnmDeathBurnDustTb[] = { gAnm_ZombieTeen_DeathBurn_Dust, gAnm_ZombieFat_DeathBurn_Dust, NULL, };

// Mars People, Anims de mort.
u64 *gpMst7_MarsPeople_DeatAnm[e_ShotDamageType_Max] = { gAnm_MarsPeople_DeathGen_Dust, gAnm_MarsPeople_DeathGen_Dust, gAnm_MarsPeople_DeathFire_Dust };

//== Général.

// Routine de dépacement des gauche/droite.
void Mst7Sub_Move(struct SMstCommon *pMst)
{
	struct SMst7_LeftRight0	*pSpe = (struct SMst7_LeftRight0 *)pMst->pData;

	if (AnmGetKey(pMst->nAnm) == e_AnmKey_Null)
	{
		AnmSetIfNew(gpMST7_AnmWalkTb[pSpe->nType], pMst->nAnm);

		u32	nFlip = 0;
		// Du vide/plein dans le bloc d'à côté ? (Pour s'arrêter un peu avant les bords de pf ou les murs).
		s32	nHt = BlockGetGroundLevel((pMst->nPosX >> 8) + (pMst->nFlipX ? 14 : -14), (pMst->nPosY >> 8));
		if (ABS(nHt) > 14) nFlip = 1;
		// Recalage au sol.
		nFlip |= Mst_BasicMove(pMst, pSpe->nCurSpd, 0);
//		nFlip |= Mst_BasicMove(pMst, nSpd, 0);
//old		nFlip |= Mst_BasicMove(pMst, (pSpe->nHitCnt ? 0 : pSpe->nCurSpd), 0);
//old		nFlip |= Mst_BasicMove(pMst, (pSpe->nHitCnt > 4 ? 0 : pSpe->nCurSpd), 0);

		if ((pMst->nFlipX == 0 && pMst->nPosX <= pSpe->nZoneMin << 12) ||
			(pMst->nFlipX && pMst->nPosX >= ((pSpe->nZoneMax + 1) << 12) - 1) || nFlip)
			AnmSetIfNew(gpMST7_AnmFlipTb[pSpe->nType], pMst->nAnm);

		// Recalage en bord de zone (avec le move, on peut déborder un peu).
		if (pMst->nPosX < pSpe->nZoneMin << 12) pMst->nPosX = pSpe->nZoneMin << 12;
		if (pMst->nPosX > ((pSpe->nZoneMax + 1) << 12) - 1) pMst->nPosX = ((pSpe->nZoneMax + 1) << 12) - 1;
	}
}

//== Brain Bot.

void Mst7Sub_Init_BrainBot0(struct SMstCommon *pMst, u8 *pData)
{
/* exemple
	struct SMst7_LeftRight0	*pSpe = (struct SMst7_LeftRight0 *)pMst->pData;
	u32	nVal;
	nVal = GetBits(0, 3, pData, 0);		// Type.
	pSpe->nType = nVal;
*/
}

#define	M7T3_DUST_SPD	0x100
s32 Mst7Sub_Main_BrainBot0(struct SMstCommon *pMst)
{
	struct SMst7_LeftRight0	*pSpe = (struct SMst7_LeftRight0 *)pMst->pData;

	// Sortie de l'écran ?
	if (Screen_ObjectOutRect(pSpe->nZoneMin << 12, pMst->nPosY, pSpe->nZoneMax - pSpe->nZoneMin + 1, 1)) return (e_MstState_Asleep);

	switch (pSpe->nOrgPhase)
	{
/*
	case e_Mst7_LeftRight_Idle:		// Idle.
		break;
*/
	case e_Mst7_LeftRight_Walk:		// Walks.
		Mst7Sub_Move(pMst);
		break;

	default:
		break;
	}

	// Vitesse, à fond tout le temps pour le Brain Bot.
	pSpe->nCurSpd = gpMST7_WalkSpdTb[pSpe->nType];

	// Affichage.
	u32	nSpr;
	nSpr = AnmGetImage(pMst->nAnm);

	// Anim de tir ?
	if (AnmGetKey(pMst->nAnm) == e_AnmKey_Enemy_Shot)
	if (AnmCheckNewImgFlag(pMst->nAnm))		// Nouvelle image ?
	{
		// Est-ce qu'on a une coordonnée d'origine de tir ?
		struct SSprRect sRect1;
		if (SprGetRect(nSpr | (pMst->nFlipX ? SPR_Flip_X : 0), e_SprRectZone_ShotOrg, &sRect1))
		if (sRect1.nType == e_SprRect_Point)
		{
			FireAdd(e_Shot_Enemy_BrainBot_RingLow, pMst->nPosX + (sRect1.nX1 << 8), pMst->nPosY + (sRect1.nY1 << 8), (pMst->nFlipX ? 0 : 128));
			FireAdd(e_Shot_Enemy_BrainBot_RingHigh, pMst->nPosX + (sRect1.nX1 << 8), pMst->nPosY + (sRect1.nY1 << 8), (pMst->nFlipX ? 0 : 128));
		}
	}

	// Se prend un tir ?
	u32	nDamage;
	// Version avec PV et avec couteau.
	nDamage = Mst_ShotCheckLife(nSpr, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife);
	if (nDamage)
	{
		s32	nDiff = pMst->nPosX - gShoot.nPlayerPosX;
		// Dust de mort.
		DustSetMvt(gAnm_BrainBot_Death_Dust, pMst->nPosX, pMst->nPosY, (nDiff < 0 ? -M7T3_DUST_SPD : M7T3_DUST_SPD), 0, e_Prio_DustUnder, (nDiff < 0 ? e_DustFlag_FlipX : 0));
		DustSet(gAnm_BrainBot_DeathLand_Dust, pMst->nPosX + (24 * (nDiff < 0 ? -M7T3_DUST_SPD : M7T3_DUST_SPD)), pMst->nPosY, e_Prio_DustUnder, (nDiff < 0 ? e_DustFlag_FlipX : 0));
		DustSetMvt(gAnm_BrainBot_BrainExplo_Dust, pMst->nPosX, pMst->nPosY - (50 << 8), (nDiff < 0 ? -M7T3_DUST_SPD/2 : M7T3_DUST_SPD/2), 0, e_Prio_DustUnder + 1, (nDiff < 0 ? e_DustFlag_FlipX : 0));
		// Score.
		gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;
		return (e_MstState_Dead);
	}

	// Touché ? On se tourne vers le joueur.
	if (pSpe->nHitCnt == MST_HIT_CNT)
	{
		pSpe->nNervousCnt = 32;	// Compteur pour "nervosité".
		if (AnmGetKey(pMst->nAnm) == e_AnmKey_Null)
		if (pMst->nFlipX != (gShoot.nPlayerPosX - pMst->nPosX <= 0 ? 0 : 1))
			AnmSetIfNew(gpMST7_AnmFlipTb[pSpe->nType], pMst->nAnm);	// Flip. On se tourne vers le joueur.
	}

	// Gestion du bit de flip lors des retournements (APRES le GetImage).
	if (AnmGetKey(pMst->nAnm) == e_AnmKey_Enemy_Flip)
		if (AnmCheckStepFlag(pMst->nAnm)) pMst->nFlipX ^= 1;

	// Affichage.
	if (pSpe->nHitCnt) pSpe->nHitCnt--;
	SprDisplay((nSpr ^ (pMst->nFlipX ? SPR_Flip_X : 0)) | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), pMst->nPosX >> 8, (pMst->nPosY >> 8), e_Prio_Ennemies + gnMstPrio);

	// Si hit, on incrémente le ShotCnt pour tirer moins souvent.
	if (pSpe->nHitCnt) pSpe->nShotCnt++;

	return (e_MstState_Managed);
}

//== Mars People.

// Phases spécifiques.
enum
{
	e_Mst7_LeftRight_MarsPeople_LeftWait = e_Mst7_LeftRight_CommonLast,
};

void Mst7Sub_Init_MarsPeople0(struct SMstCommon *pMst, u8 *pData)
{
/* exemple
	struct SMst7_LeftRight0	*pSpe = (struct SMst7_LeftRight0 *)pMst->pData;
	u32	nVal;
	nVal = GetBits(0, 3, pData, 0);		// Type.
	pSpe->nType = nVal;
*/
	struct SMst7_LeftRight0	*pSpe = (struct SMst7_LeftRight0 *)pMst->pData;
	// From left ?
	if (pSpe->nType & 8)
	{
		pSpe->nType &= 7;
		pMst->nFlipX = 1;
		pMst->nPhase = e_Mst7_LeftRight_MarsPeople_LeftWait;
	}
}

s32 Mst7Sub_Main_MarsPeople0(struct SMstCommon *pMst)
{
	struct SMst7_LeftRight0	*pSpe = (struct SMst7_LeftRight0 *)pMst->pData;

	// Sortie de l'écran ?
	if (Screen_ObjectOutRect(pSpe->nZoneMin << 12, pMst->nPosY, pSpe->nZoneMax - pSpe->nZoneMin + 1, 1)) return (e_MstState_Asleep);

	switch (pSpe->nOrgPhase)
	{
/*
	case e_Mst7_LeftRight_Idle:		// Idle.
		break;
*/
	case e_Mst7_LeftRight_Walk:		// Walks.
		Mst7Sub_Move(pMst);
		break;

	case e_Mst7_LeftRight_MarsPeople_LeftWait:		// From Left.
		if (pMst->nPosX > gScrollPos.nPosX - 0x2000) return (e_MstState_Managed);
		if (ABS(pMst->nPosY - gShoot.nPlayerPosY) > 0x1000) return (e_MstState_Asleep);
		// Passage en mode normal.
		pMst->nPhase = e_Mst7_LeftRight_Idle;
		break;

	default:
		break;
	}

	// Affichage.
	u32	nSpr;
	nSpr = AnmGetImage(pMst->nAnm);

	Mst_ProximityCheck(pMst);	// Donne le signal au joueur pour un éventuel coup de couteau.

	// Tir ?
	Mst_ShotLaunch(pMst, nSpr, gpMST7_ShotTb[pSpe->nType], gpMST7_AnmShotFxTb[pSpe->nType]);

	// Se prend un tir ?
	u32	nDamage;
	// Version avec PV et avec couteau.
	nDamage = Mst_ShotCheckLife(nSpr, pMst->nPosX, pMst->nPosY, &pSpe->nHitCnt, &pSpe->nLife);
	nDamage |= Mst_KnifeHitCheck(pMst, nSpr);	// Le coup de couteau tue le monstre en un coup.
	if (nDamage)
	{
		// Dust de mort.
		DustSet(gpMst7_MarsPeople_DeatAnm[nDamage >> 16], pMst->nPosX, pMst->nPosY, e_Prio_DustUnder, (pMst->nFlipX ? e_DustFlag_FlipX : 0));
		// Score.
		gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;
		return (e_MstState_Dead);	// < sans respawn.
	}

	// Touché ? On se tourne vers le joueur.
	if (pSpe->nHitCnt == MST_HIT_CNT)
	{
		pSpe->nNervousCnt = 32;	// Compteur pour "nervosité".
		if (AnmGetKey(pMst->nAnm) == e_AnmKey_Null)
		if (pMst->nFlipX != (gShoot.nPlayerPosX - pMst->nPosX <= 0 ? 0 : 1))
			AnmSetIfNew(gpMST7_AnmFlipTb[pSpe->nType], pMst->nAnm);	// Flip. On se tourne vers le joueur.
	}

	// Gestion du bit de flip lors des retournements (APRES le GetImage).
	if (AnmGetKey(pMst->nAnm) == e_AnmKey_Enemy_Flip)
		if (AnmCheckStepFlag(pMst->nAnm)) pMst->nFlipX ^= 1;

	// Affichage.
	if (pSpe->nHitCnt) pSpe->nHitCnt--;
	SprDisplay((nSpr ^ (pMst->nFlipX ? SPR_Flip_X : 0)) | (pSpe->nHitCnt ? SPR_Flag_HitPal : 0), pMst->nPosX >> 8, (pMst->nPosY >> 8), e_Prio_Ennemies + gnMstPrio);

	// Si hit, on incrémente le ShotCnt pour tirer moins souvent.
	if (pSpe->nHitCnt) pSpe->nShotCnt++;

	return (e_MstState_Managed);
}

//== Zombies Teen & Fat.

// Phases spécifiques.
enum
{
	e_Mst7_LeftRight_Zombie_Fall = e_Mst7_LeftRight_CommonLast,
	e_Mst7_LeftRight_Zombie_BurnWalk,
};

void Mst7Sub_Init_Zombie0(struct SMstCommon *pMst, u8 *pData)
{
/* exemple
	struct SMst7_LeftRight0	*pSpe = (struct SMst7_LeftRight0 *)pMst->pData;
	u32	nVal;
	nVal = GetBits(0, 3, pData, 0);		// Type.
	pSpe->nType = nVal;
*/
}

s32 Mst7Sub_Main_Zombie0(struct SMstCommon *pMst)
{
	struct SMst7_LeftRight0	*pSpe = (struct SMst7_LeftRight0 *)pMst->pData;

	// Sortie de l'écran ?
	if (pMst->nPhase != e_Mst7_LeftRight_Zombie_BurnWalk)
	if (Screen_ObjectOutRect(pSpe->nZoneMin << 12, pMst->nPosY, pSpe->nZoneMax - pSpe->nZoneMin + 1, 1)) return (e_MstState_Asleep);

	// GetImage avant, pour les flips... >_<
	u32	nSpr;
	nSpr = AnmGetImage(pMst->nAnm);
	pSpe->nCurSpd = 0;

	switch (pSpe->nOrgPhase)
	{
	case e_Mst7_LeftRight_Walk:		// Walks.
		// Avance au changement d'image, et pas sur la frame en sortie de flip.
		if (AnmGetKey(pMst->nAnm) == e_AnmKey_Null && pSpe->nLastAnmKey != e_AnmKey_Enemy_Flip)
		if (AnmCheckNewImgFlag(pMst->nAnm))
		if (AnmCheckStepFlag(pMst->nAnm) == 0)
			pSpe->nCurSpd = 0x200;
		// Move.
		Mst7Sub_Move(pMst);
		break;

	case e_Mst7_LeftRight_Zombie_Fall:		// Chute + relevé quand touché par balle.
		if (AnmGetKey(pMst->nAnm) == e_AnmKey_Null) pMst->nPhase = e_Mst7_LeftRight_Walk;
		break;

	case e_Mst7_LeftRight_Zombie_BurnWalk:	// Marche en brulant. !!! Attention, il peut sortir de sa zone !!!
		Mst_BasicMove(pMst, 0x40, 0);
		if (AnmCheckStepFlag(pMst->nAnm))
		{
			DustSet(gpMst7_Zombie_AnmDeathBurnDustTb[pSpe->nType], pMst->nPosX, pMst->nPosY, e_Prio_DustUnder, (pMst->nFlipX ? e_DustFlag_FlipX : 0));
			return (e_MstState_Dead);
		}
		break;

	default:
		break;
	}

	// Tir ?
	Mst_ShotLaunch(pMst, nSpr, gpMST7_ShotTb[pSpe->nType], gpMST7_AnmShotFxTb[pSpe->nType]);

	// Se prend un tir ? (Pas de zone de col sur les fall/rise/burn walk).
	u32	nDamage;
	nDamage = FireHitCheck(nSpr, pMst->nPosX, pMst->nPosY, e_ShotOrg_Player, 0);
	nDamage |= Mst_KnifeHitCheck(pMst, nSpr);	// Comme ça, donne la priorité de la mort au tir.
	if (nDamage)
	{
		pSpe->nShotCnt = 0;		// RAZ shot cnt.

		// Giclée de sang quand il se prend une balle.
		if (nDamage >> 16 == e_ShotDamageType_Gen)
		{
			AnmSet(gpMst7_Zombie_AnmFallTb[pSpe->nType], pMst->nAnm);
			DustSet(gAnm_Blood0_FX_Dust, pMst->nPosX, pMst->nPosY, e_Prio_DustUnder, (pMst->nPosX <= gShoot.nPlayerPosX ? e_DustFlag_FlipX : 0));
			pMst->nPhase = e_Mst7_LeftRight_Zombie_Fall;
		}
		// Souffle.
		else if (nDamage >> 16 == e_ShotDamageType_Blow)
		{
			gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;	// Score.
			DustSet(gAnm_Zombie_DeathShotgunGen_Dust, pMst->nPosX, pMst->nPosY, e_Prio_Joueur - 8, (pMst->nFlipX ? e_DustFlag_FlipX : 0));
			// Morceaux qui volent.
			s32	nDiff = pMst->nPosX - gShoot.nPlayerPosX;
			u32	nRnd;
			s32	nAdd;
			nRnd = gnFrame + pMst->nLoadIdx;
			nAdd = nRnd &0x7F;
			DustSetMvt((nRnd & 1 ? gAnm_ZombieBits_Arm1_Dust : gAnm_ZombieBits_Arm2_Dust), pMst->nPosX, (pMst->nPosY - 0x2000), (nDiff < 0 ? -0x120 : 0x120), -(0x400 + nAdd), e_Prio_DustUnder - 1, e_DustFlag_Gravity | (nDiff < 0 ? e_DustFlag_FlipX : 0));
			nAdd = (nAdd + 0x40) &0x7F;
			DustSetMvt((nRnd & 1 ? gAnm_ZombieBits_Leg1_Dust : gAnm_ZombieBits_Leg2_Dust), pMst->nPosX, (pMst->nPosY - 0x2000), (nDiff < 0 ? -0x180 : 0x180), -(0x300 + nAdd), e_Prio_DustUnder - 1, e_DustFlag_Gravity | (nDiff < 0 ? e_DustFlag_FlipX : 0));
			return (e_MstState_Dead);
		}
		// Brûle.
		else
		{
			gShoot.nPlayerScore += gpMstTb[pMst->nMstNo].nPoints;	// Score.
			AnmSet(gpMst7_Zombie_AnmDeathBurnTb[pSpe->nType], pMst->nAnm);
			pMst->nPhase = e_Mst7_LeftRight_Zombie_BurnWalk;
		}
	}

	// Gestion du bit de flip lors des retournements (APRES le GetImage).
	if (AnmGetKey(pMst->nAnm) == e_AnmKey_Enemy_Flip)
		if (AnmCheckStepFlag(pMst->nAnm)) pMst->nFlipX ^= 1;

	// Affichage.
	SprDisplay(nSpr ^ (pMst->nFlipX ? SPR_Flip_X : 0), pMst->nPosX >> 8, (pMst->nPosY >> 8), e_Prio_Ennemies + gnMstPrio);

	return (e_MstState_Managed);
}

//== Container.

#define	MST7_NB	4

// Table de pointeurs de fonctions.
struct SMstFctTb	gpMst7Subroutines[MST7_NB] =
{
	{ Mst7Sub_Init_Zombie0, Mst7Sub_Main_Zombie0 },		// Zombie Teen.
	{ Mst7Sub_Init_Zombie0, Mst7Sub_Main_Zombie0 },		// Zombie Fat.
	{ Mst7Sub_Init_MarsPeople0, Mst7Sub_Main_MarsPeople0 },		// Mars People.
	{ Mst7Sub_Init_BrainBot0, Mst7Sub_Main_BrainBot0 },		// Brain Bot.

};

// Init générale.

void Mst7_Init_LeftRight0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst7_LeftRight0	*pSpe = (struct SMst7_LeftRight0 *)pMst->pData;
	u32	nVal;
	u32	nTypeOrg;

	nTypeOrg = GetBits(0, 3, pData, 0);		// Type.
	pSpe->nType = nTypeOrg & 7;
	AnmSet(gpMST7_AnmWaitTb[pSpe->nType], pMst->nAnm);

	nVal = GetBits(4, 7, pData, 0);		// Zone.
	pSpe->nZoneMin = (pMst->nPosX >> 12) - nVal;
	pSpe->nZoneMax = (pMst->nPosX >> 12) + nVal;

	// Sens, on tourne le monstre vers le joueur.
	pMst->nFlipX = (gShoot.nPlayerPosX - pMst->nPosX >= 0 ? 1 : 0);
	// Mise au niveau du sol.
	Enemy_GroundLevel(pMst);

	pMst->nPhase = e_Mst7_LeftRight_Idle;
	pSpe->nShotCnt = 0;
	pSpe->nNervousCnt = 0;	// Compteur pour "nervosité".
	pSpe->nLastAnmKey = e_AnmKey_Null;
	pSpe->nCurSpd = gpMST7_WalkSpdTb[pSpe->nType];

	pSpe->nLife = gpMST7_LifePtsTb[pSpe->nType];
	pSpe->nHitCnt = 0;

	// Inits spécifiques à chaque type de gauche/droite.
//	gpMst7Subroutines[pSpe->nType].pFctInit(pMst, pData);
	pSpe->nType = nTypeOrg;
	gpMst7Subroutines[nTypeOrg & 7].pFctInit(pMst, pData);

}

// Gestion des phases générales.
s32 Mst7_Main_LeftRight0(struct SMstCommon *pMst)
{
	struct SMst7_LeftRight0	*pSpe = (struct SMst7_LeftRight0 *)pMst->pData;
	s32	nDistX = gShoot.nPlayerPosX - pMst->nPosX;
	s32	nDistY = gShoot.nPlayerPosY - pMst->nPosY;
	u32	nSide = (nDistX < 0 ? 0 : 1);

	// Attaque ? (Avant les zones pour éviter les flipx/flipx/flipx en fin de zone).
	if (pSpe->nShotCnt && AnmGetKey(pMst->nAnm) != e_AnmKey_Enemy_Shot) pSpe->nShotCnt--;
	//
	if (pMst->nPhase < e_Mst7_LeftRight_CommonLast)	//	if (pMst->nPhase == e_Mst7_LeftRight_Idle || pMst->nPhase == e_Mst7_LeftRight_Walk)
	if (AnmGetKey(pMst->nAnm) == e_AnmKey_Null)		// ie pas en flip ou autre.
	{
		if (ABS(nDistX) < ((u32)gpMST7_ShotDstXMinTb[pSpe->nType] << 8)) pSpe->nShotCnt = 0;	// Joueur trop près, stoppe la pause d'après tir.

//		// A distance de tir ET (pas trop près OU qui sort de flip).
//		if (ABS(nDistX) <= (u32)gpMST7_ShotDstXTb[pSpe->nType] << 12 &&
		// A distance de tir ET (pas trop près OU qui sort de flip). + cas spécial pour que le Mars People tire dès qu'il se retourne.
		if ((ABS(nDistX) <= (u32)gpMST7_ShotDstXTb[pSpe->nType] << 12 || (pSpe->nType == 2 && pSpe->nLastAnmKey == e_AnmKey_Enemy_Flip && nSide == pMst->nFlipX)) &&
			(ABS(nDistX) >= (u32)gpMST7_ShotDstXMinTb[pSpe->nType] << 8 || (pSpe->nLastAnmKey == e_AnmKey_Enemy_Flip && nSide == pMst->nFlipX)))
		if (pMst->nPosX >= gScrollPos.nPosX + 0x1000 && pMst->nPosX <= gScrollPos.nPosX + (SCR_Width << 8) - 0x1000)	// Dans l'écran ?
		if (nDistY <= (0x10 << 8) && nDistY >= -(0x28 << 8))
		{
			if (nSide == pMst->nFlipX)
			{
				if (pSpe->nShotCnt == 0)
				{
					if (MST_SHOT_COND)	// Si le héros n'est pas mort...
						AnmSetIfNew(gpMST7_AnmShotTb[pSpe->nType], pMst->nAnm);	// Tir.
					pSpe->nShotCnt = gpMST7_ShotDelay[pSpe->nType];		// On entre dans la boucle que si key == null => le animset passe forcément.
				}
			}
			else
//			if (ABS(nDistX) >= ((u32)gpMST7_ShotDstXMinTb[pSpe->nType] + 8) << 8)
			{
				AnmSetIfNew(gpMST7_AnmFlipTb[pSpe->nType], pMst->nAnm);	// Flip. On se tourne vers le joueur.
				pSpe->nShotCnt = 0;		// On entre dans la boucle que si key == null => le animset passe forcément.
			}
		} // if dy / screen / dx
	}
	//
	if (pSpe->nShotCnt && pMst->nPhase < e_Mst7_LeftRight_CommonLast) pMst->nPhase = e_Mst7_LeftRight_Idle;


	// Gestion de la nervosité.
	if (((ABS(nDistX) <= ((u32)gpMST7_ShotDstXTb[pSpe->nType] << 12) + 0x2000 && nSide == pMst->nFlipX) ||
		(ABS(nDistX) < (u32)gpMST7_ShotDstXMinTb[pSpe->nType] << 8)) &&
		nDistY <= (0x10 << 8) && nDistY >= -(0x28 << 8))
		pSpe->nNervousCnt = 32;	// Compteur pour "nervosité".
	// Accélération/ralentissement.
	u16	nTargetSpd;
	if (pSpe->nNervousCnt)
	{
		if (pSpe->nShotCnt == 0 && AnmGetKey(pMst->nAnm) == e_AnmKey_Null) pSpe->nNervousCnt--;
		nTargetSpd = gpMST7_WalkSpdNervousTb[pSpe->nType];
	}
	else
		nTargetSpd = gpMST7_WalkSpdTb[pSpe->nType];
	if (AnmGetKey(pMst->nAnm) == e_AnmKey_Enemy_Flip) nTargetSpd = 0;
//	if (AnmGetKey(pMst->nAnm) == e_AnmKey_Enemy_Flip) { nTargetSpd = 0; pSpe->nCurSpd = 0; }
	//
	if (pSpe->nCurSpd < nTargetSpd) pSpe->nCurSpd += 0x08;
	else if (pSpe->nCurSpd > nTargetSpd) pSpe->nCurSpd -= 0x08;


	pSpe->nOrgPhase = pMst->nPhase;		// Sauvegarde la phase originale au cas ou la phase change dans les routines communes, pour appel de la phase correcte dans les traitements spécifiques.
	switch (pMst->nPhase)
	{
	case e_Mst7_LeftRight_Idle:		// Idle.
		ChaserTarget_AddToList(pMst->nPosX >> 8, (pMst->nPosY >> 8) - 16);	// Ajout dans la table de visée des missiles.
		if (AnmGetKey(pMst->nAnm) != e_AnmKey_Enemy_Flip)	// On écrase pas le flip.
			AnmSetIfNew(gpMST7_AnmWaitTb[pSpe->nType], pMst->nAnm);
		pMst->nPhase = e_Mst7_LeftRight_Walk;
		break;

	case e_Mst7_LeftRight_Walk:		// Walk.
		ChaserTarget_AddToList(pMst->nPosX >> 8, (pMst->nPosY >> 8) - 16);	// Ajout dans la table de visée des missiles.
/*
		if (AnmGetKey(pMst->nAnm) == e_AnmKey_Null)
		{
			AnmSetIfNew(gpMST7_AnmWalkTb[pSpe->nType], pMst->nAnm);

			u32	nFlip = 0;
			// Du vide/plein dans le bloc d'à côté ? (Pour s'arrêter un peu avant les bords de pf ou les murs).
			s32	nHt = BlockGetGroundLevel((pMst->nPosX >> 8) + (pMst->nFlipX ? 14 : -14), (pMst->nPosY >> 8));
			if (ABS(nHt) > 14) nFlip = 1;
			// Recalage au sol.
			nFlip |= Mst_BasicMove(pMst, pSpe->nCurSpd, 0);
//			nFlip |= Mst_BasicMove(pMst, (pSpe->nHitCnt ? 0 : pSpe->nCurSpd), 0);
//			nFlip |= Mst_BasicMove(pMst, (pSpe->nHitCnt > 4 ? 0 : pSpe->nCurSpd), 0);

			if ((pMst->nFlipX == 0 && pMst->nPosX <= pSpe->nZoneMin << 12) ||
				(pMst->nFlipX && pMst->nPosX >= ((pSpe->nZoneMax + 1) << 12) - 1) || nFlip)
				AnmSetIfNew(gpMST7_AnmFlipTb[pSpe->nType], pMst->nAnm);

			// Recalage en bord de zone (avec le move, on peut déborder un peu).
			if (pMst->nPosX < pSpe->nZoneMin << 12) pMst->nPosX = pSpe->nZoneMin << 12;
			if (pMst->nPosX > ((pSpe->nZoneMax + 1) << 12) - 1) pMst->nPosX = ((pSpe->nZoneMax + 1) << 12) - 1;
		}
*/
		break;

	default:
		break;
	}

	pSpe->nLastAnmKey = AnmGetKey(pMst->nAnm);

	return (gpMst7Subroutines[pSpe->nType].pFctMain(pMst));
}

//=============================================================================
// Mst 8 : Update blocs du décor.

// Phases.
enum
{
	e_Mst8_ItemWait = 0,
	e_Mst8_PhaseAnim,

};

// Structure spécifique.
struct SMst8_BlkBkg1
{
	u8	nItemWait;		// N° item de quête attendu pour démarrer.
	u8	nUpdateBkg;
	u16	nBlkOrg;		// N° du permier bloc en haut à gauche de la zone à copier.
	u8	nBlkLg;			// Lg de la zone à copier.
	u8	nBlkHt;			// Ht de la zone à copier.

};

void Mst8_Init_BlkBkg1(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst8_BlkBkg1	*pSpe = (struct SMst8_BlkBkg1 *)pMst->pData;
	u32	nVal;
	u32	nId;

	static	u64 *pMst8_AnmInit[MST3_POW_MAX] = { gAnm_Lev8_PF1_Opening, gAnm_Lev8_Door1_Opening, };

	nId = GetBits(0, 7, pData, 0);
	nVal = GetBits(8, 14, pData, 0);
	pSpe->nItemWait = nVal;
	nVal = GetBits(15, 15, pData, 0);
	pSpe->nUpdateBkg = nVal;
	nVal = GetBits(16, 31, pData, 0);
	pSpe->nBlkOrg = nVal;
	nVal = GetBits(32, 39, pData, 0);
	pSpe->nBlkLg = nVal;
	nVal = GetBits(40, 47, pData, 0);
	pSpe->nBlkHt = nVal;

	pMst->nPosX &= ~0xFFF;
	pMst->nPosY &= ~0xFFF;

	//if (pMst8_AnmInit[nId] != NULL)
	AnmSet(pMst8_AnmInit[nId], pMst->nAnm);

	pMst->nPhase = e_Mst8_ItemWait;
}

s32 Mst8_Main_BlkBkg1(struct SMstCommon *pMst)
{
	struct SMst8_BlkBkg1	*pSpe = (struct SMst8_BlkBkg1 *)pMst->pData;
	u32	nSpr;

	switch (pMst->nPhase)
	{
	case e_Mst8_ItemWait:		// Attente de l'item qui va bien.
		// Sortie de l'écran ?
		if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);
		// Item de quête ?
		if (gpMstQuestItems[pSpe->nItemWait] == 0) break;
		if (pSpe->nUpdateBkg == 0)
		{
			// Update du décor AVANT l'anim.
			BlkBkg(gMap.nHeroPlane, pMst->nPosX >> 12, pMst->nPosY >> 12, pSpe->nBlkLg, pSpe->nBlkHt, pSpe->nBlkOrg);
		}
		pMst->nPhase = e_Mst8_PhaseAnim;
		//break;		// Pas de break pour avoir l'affichage de la première image tout de suite. (Sinon dans le cas de de l'update AVANT, l'anim démarre une frame plus tard).

	case e_Mst8_PhaseAnim:		// Anim.
		nSpr = AnmGetImage(pMst->nAnm);
		if (AnmCheckEnd(pMst->nAnm))
		{
			if (pSpe->nUpdateBkg)
			{
				// Update du décor APRES l'anim.
				BlkBkg(gMap.nHeroPlane, pMst->nPosX >> 12, pMst->nPosY >> 12, pSpe->nBlkLg, pSpe->nBlkHt, pSpe->nBlkOrg);
			}
			return (e_MstState_Dead);
		}
		// Affichage.
		SprDisplay(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_DustUnder);
		break;
	}

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 9 : Slugs.

u32	gpSlugWaitImgTb[e_HeroVehicle_MAX - e_HeroVehicle_SlugBase] =
	{ e_Spr_SlugSubmarine_Close, e_Spr_SlugTank_Close, e_Spr_SlugCopter_WaitPlayer, e_Spr_SlugCopter_WaitPlayer, e_Spr_SlugRocket_WaitPlayer };

// Renvoie le point d'entrée dans un slug (offset x et y par rapport à un point de ref).
void MstSlug_EntrancePtGet(u32 nSlugType, s32 *pnOffsX, s32 *pnOffsY)
{
	struct SSprRect sSlugRect;
	if (SprGetRect(gpSlugWaitImgTb[nSlugType], e_SprRectZone_RectCol, &sSlugRect))
	{
		if (sSlugRect.nType == e_SprRect_Rect)
		{
			*pnOffsX = sSlugRect.nX1 + ((sSlugRect.nX2 - sSlugRect.nX1) / 2);
			*pnOffsY = sSlugRect.nY1 + 2;
			return;
		}
	}
	*pnOffsX = *pnOffsY = 0;		// Par défaut.
}

// Rotation d'un point.
void Rot2D_RotatePoint(s32 *pnOffsX, s32 *pnOffsY, u8 nAngle)
{
	struct	S2DPoint	sPtOrg, sPtRot;

	if (nAngle == 0) return;

	sPtOrg.nX = *pnOffsX;
	sPtOrg.nY = *pnOffsY;
	Rotation2D(1, &sPtOrg, &sPtRot, nAngle, 0x0100);
	*pnOffsX = sPtRot.nX;
	*pnOffsY = sPtRot.nY;
}


/*
[mst9] Slug
Type = 0:3: Submarine - Tank - Helicopter - Helicopter_No_Exit - Rocket
State = 4:7 : Wait - Dies(reserved)
Energy(192) = 8:15:
Vulcan = 16:23:			; Angle du vulcan cannon.
Bombs(50) = 24:31:
Slug Angle(0) = 32:39:		; Angle du slug (Tank).
*/

// Phases.
enum
{
	e_Mst9_Wait = 0,
	e_Mst9_Explode,

};

// Structure spécifique.
struct SMst9_Slug
{
	u8	nType;
	u8	nEnergy;
	u8	nVulcanAngle;
	u8	nBombs;
	u8	nSlugAngle;
	s8	nAnmWait;		// -1 si pas d'anim.
	s16	nOffsY;			// Décalage pour les slugs en rotation.
};

void Mst9_Init_Slug(struct SMstCommon *pMst, u8 *pData)
{
	static u64	*gpSlugWaitAnmTb[e_HeroVehicle_MAX - e_HeroVehicle_SlugBase] =	// Anim d'attente (si anim il y a).
		{ NULL, NULL, gAnm_SlugCopter_WaitPlayer, gAnm_SlugCopter_WaitPlayer, NULL };

	struct SMst9_Slug	*pSpe = (struct SMst9_Slug *)pMst->pData;
	u32	nVal;

	nVal = GetBits(0, 3, pData, 0);
	pSpe->nType = nVal;

	// Recalage au niveau du sol pour le tank, l'hélico et la fusée.
	// (Pour la fusée dans l'espace, ça fonctionne car la fusée pose en bas de map et l'offset compense la différence).
	pSpe->nOffsY = 0;
	if (pSpe->nType != e_HeroVehicle_Submarine - e_HeroVehicle_SlugBase)
	{
		s32	nOldY = pMst->nPosY;
		Enemy_GroundLevel(pMst);			// Mise au niveau du sol.
		if (pMst->nLoadIdx == -1) pSpe->nOffsY = (nOldY - pMst->nPosY) >> 8;
	}

	nVal = GetBits(8, 15, pData, 0);
	pSpe->nEnergy = nVal;
	nVal = GetBits(16, 23, pData, 0);
	pSpe->nVulcanAngle = nVal;
	nVal = GetBits(24, 31, pData, 0);
	pSpe->nBombs = nVal;
	if (pSpe->nBombs > SLUG_BOMBS_MAX) pSpe->nBombs = SLUG_BOMBS_MAX;
	nVal = GetBits(32, 39, pData, 0);
	pSpe->nSlugAngle = nVal;

	nVal = GetBits(4, 7, pData, 0);		// Phase.
	pMst->nPhase = nVal;
	if (pMst->nPhase != e_Mst9_Wait)
	{
		pSpe->nEnergy <<= 2;	// nEnergy sert de compteur avant l'explosion. Pour ralonger le délai.
	}

	// Anim d'attente ?
	if (gpSlugWaitAnmTb[pSpe->nType] != NULL)
		pSpe->nAnmWait = AnmSet(gpSlugWaitAnmTb[pSpe->nType], -1);
	else
		pSpe->nAnmWait = -1;

}

s32 Mst9_Main_Slug(struct SMstCommon *pMst)
{
	struct SMst9_Slug	*pSpe = (struct SMst9_Slug *)pMst->pData;
	u32	nSpr;
	s32	nOffsX, nOffsY;
	s32	nPosY_sav = pMst->nPosY;	// ***

	switch (pMst->nPhase)
	{
	case e_Mst9_Wait:		// Attente que le joueur monte dans le slug.
//	// Sortie de l'écran ?
//	if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY)) return (e_MstState_Asleep);
//	return (e_MstState_Dead);
//Attention à libérer aussi pSpe->nAnmWait !!!


		// Gravité (sauf sub).
		if (pSpe->nType != e_HeroVehicle_Submarine - e_HeroVehicle_SlugBase)
		{
			Mst_Gravity2(pMst, GRAVITY, SPDY_MAX);
			nPosY_sav = pMst->nPosY;				// *** (Après la gravité, mais avant le break)
			pMst->nPosY += (s32)pSpe->nOffsY << 8;	// ***
			if (pMst->nGround == 0) break;			// On s'arrête là si chute.
		}
		else
			pMst->nPosY += (s32)pSpe->nOffsY << 8;	// ***

		// Si le joueur n'est pas déjà dans un véhicule.
		if (gShoot.nVehicleType < e_HeroVehicle_SlugBase)
		{

		// Contact avec le joueur ?
		if (AnmGetKey(gShoot.nPlayerAnm) == e_AnmKey_Hero_SlugOut) break;

		nSpr = gpSlugWaitImgTb[pSpe->nType];
		if (SprCheckColBox(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8,
			gShoot.nPlayerSprCol, gShoot.nPlayerPosX >> 8, gShoot.nPlayerPosY >> 8))
		{
			u32	nCond = 0;

			// Pour le tank, l'hélico et la fusée, on regarde que les pieds du joueur arrivent dans la zone.
			if (pSpe->nType != e_HeroVehicle_Submarine - e_HeroVehicle_SlugBase)
			{
				struct SSprRect sRect1;
				if (SprGetRect(nSpr, e_SprRectZone_RectCol, &sRect1))
				if (gShoot.nPlayerPosY >= pMst->nPosY + (sRect1.nY1 << 8) &&
					gShoot.nPlayerPosY <= pMst->nPosY + (sRect1.nY2 << 8) &&
					gShoot.nPlayerSpdY > 0)
						nCond = 1;
			}
			else
				nCond = 1;		// Par défaut, contact simple (sous-marin).

			if (nCond)
			{
				Player_VehicleSet(e_HeroVehicle_SlugBase + pSpe->nType, pMst->nPosX, pMst->nPosY, pSpe->nEnergy, pSpe->nVulcanAngle, pSpe->nBombs);
				Player_Control();	// La routine de contrôle du nouveau véhicule.
				if (pSpe->nAnmWait != -1) AnmReleaseSlot(pSpe->nAnmWait);
				return (e_MstState_Dead);
			}
		}

		} // if (gShoot.nVehicleType < e_HeroVehicle_SlugBase)

		// Affichage flèche "In".
		nSpr = AnmGetImage(pMst->nAnm);
		MstSlug_EntrancePtGet(pSpe->nType, &nOffsX, &nOffsY);
		Rot2D_RotatePoint(&nOffsX, &nOffsY, pSpe->nSlugAngle);
		SprDisplay(nSpr, (pMst->nPosX >> 8) + nOffsX, (pMst->nPosY >> 8) + nOffsY - 8, e_Prio_DustOver);
		break;

	case e_Mst9_Explode:	// Le slug va exploser (trop de dégats).
		pMst->nPosY += (s32)pSpe->nOffsY << 8;	// ***
		//
		if (pSpe->nEnergy)
		{
			pSpe->nEnergy--;
			break;
		}
		// Explosion.
		DustSet(gAnm_Explosion0_Big_Dust, pMst->nPosX, pMst->nPosY, e_Prio_DustUnder, 0);
		if (pSpe->nAnmWait != -1) AnmReleaseSlot(pSpe->nAnmWait);
		return (e_MstState_Dead);
		break;
	}

	// Affichage Slug (L'angle vaut toujours 0, sauf pour le tank).
	if (pSpe->nAnmWait != -1)
		nSpr = AnmGetImage(pSpe->nAnmWait);
	else
		nSpr = gpSlugWaitImgTb[pSpe->nType];
	SprDisplayRotoZoom(nSpr, pMst->nPosX >> 8, (pMst->nPosY >> 8), e_Prio_Ennemies-2-1, pSpe->nSlugAngle, 0x0100);
	// Affichage du vulcan cannon.
	if (pSpe->nType != e_HeroVehicle_Rocket - e_HeroVehicle_SlugBase)	// Sauf pour la fusée !
	{
		//- On récupère le point d'attache du canon.
		nOffsX = nOffsY = 0;
		struct SSprRect sSlugRect;
		if (SprGetRect(nSpr, e_SprRectZone_ShotOrg, &sSlugRect))
		{
			nOffsX = sSlugRect.nX1;
			nOffsY = sSlugRect.nY1;
			Rot2D_RotatePoint(&nOffsX, &nOffsY, pSpe->nSlugAngle);
		}
		//- Affichage.
		SprDisplay(e_Spr_VulcanCannon_Still + (pSpe->nVulcanAngle >> 3), (pMst->nPosX >> 8) + nOffsX, (pMst->nPosY >> 8) + nOffsY, e_Prio_Ennemies-2-1 + 1);
	}

	pMst->nPosY = nPosY_sav;	// Restore old posY.
	return (e_MstState_Managed);
}

//=============================================================================

/*
struct SAAA { u8	nTb0[80]; };
assert(sizeof(struct SAAA) < MST_COMMON_DATA_SZ);
*/

#ifndef NDEBUG
// Debug, vérification de la taille des structures.
void Mst00CheckStructSizes(void)
{
	assert(sizeof(struct SMst0_Platform1) < MST_COMMON_DATA_SZ);
	// Mst1: Pas de struct spécifique.
	assert(sizeof(struct SMst2_MiniUFO0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst3_POW1) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst4_WeaponCapsule1) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst5_ScrollLockX1) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst6_RShobu1) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst7_LeftRight0) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst8_BlkBkg1) < MST_COMMON_DATA_SZ);
	assert(sizeof(struct SMst9_Slug) < MST_COMMON_DATA_SZ);
}
#endif

