#include "includes.h"


// 1 u64 : 1 clef.
// Ensuite :
// 1 u64 : Nb de frames d'affichage
//  Si b31, code de contrôle : Voir liste en haut de anims.h.
// 1 u64 : N° du sprite.


// Macro pour le clignotement de mort.
#define	ANIM_CLIGNO(sprno)	\
6, SPR_NoSprite,	\
6, sprno,	\
6, SPR_NoSprite,	\
6, sprno,	\
6, SPR_NoSprite,	\
6, sprno,	\
6, SPR_NoSprite,	\
6, sprno,

// Macro pour jouer un son ou l'autre.
#define	ANIM_SFXRND(proba, sfx1no, sfx1prio, sfx2no, sfx2prio)	\
e_Anm_RndJump, proba, 4+0, 4+5,	\
e_Anm_Sfx, sfx1no, sfx1prio,	\
e_Anm_Jump, 5,					\
e_Anm_Sfx, sfx2no, sfx2prio,


// Nage - Repos (legs).
u64	gAnm_HeroSub_Stance[] =
{
e_AnmKey_HeroSub_Stance,
12, e_Spr_HeroSubLegs_Run,
12, e_Spr_HeroSubLegs_Run + 1,
12, e_Spr_HeroSubLegs_Run,
//12, e_Spr_HeroSubLegs_Run + 2,
//12, e_Spr_HeroSubLegs_Run + 1,
12, e_Spr_HeroSubLegs_Run + 5,
12, e_Spr_HeroSubLegs_Run + 6,
//12, e_Spr_HeroSubLegs_Run + 7,
//12, e_Spr_HeroSubLegs_Run + 6,
12, e_Spr_HeroSubLegs_Run + 5,
e_Anm_Goto, (u64)gAnm_HeroSub_Stance
};
// Nage - Avance (legs).
u64	gAnm_HeroSub_Run[] =
{
e_AnmKey_HeroSub_Run,
4, e_Spr_HeroSubLegs_Run,
4, e_Spr_HeroSubLegs_Run + 1,
4, e_Spr_HeroSubLegs_Run + 2,
4, e_Spr_HeroSubLegs_Run + 3,
4, e_Spr_HeroSubLegs_Run + 4,
//4, e_Spr_HeroSubLegs_Run + 3,
4, e_Spr_HeroSubLegs_Run + 2,
4, e_Spr_HeroSubLegs_Run + 1,
4, e_Spr_HeroSubLegs_Run,
4, e_Spr_HeroSubLegs_Run + 5,
4, e_Spr_HeroSubLegs_Run + 6,
4, e_Spr_HeroSubLegs_Run + 7,
4, e_Spr_HeroSubLegs_Run + 8,
4, e_Spr_HeroSubLegs_Run + 9,
//4, e_Spr_HeroSubLegs_Run + 8,
4, e_Spr_HeroSubLegs_Run + 7,
4, e_Spr_HeroSubLegs_Run + 6,
4, e_Spr_HeroSubLegs_Run + 5,
e_Anm_Goto, (u64)gAnm_HeroSub_Run
};
// Nage - Mort générique.
u64	gAnm_HeroSub_DeathGen[] =
{
e_AnmKey_Hero_Death,
e_Anm_Sfx, e_Sfx_Death_Scream1, e_SfxPrio_Hero_Shot + 10,
4, e_Spr_HeroSub_DeathGen,      4, e_Spr_HeroSub_DeathGen + 1,  4, e_Spr_HeroSub_DeathGen + 2,
4, e_Spr_HeroSub_DeathGen + 3,  4, e_Spr_HeroSub_DeathGen + 4,  4, e_Spr_HeroSub_DeathGen + 5,
4, e_Spr_HeroSub_DeathGen + 6,  4, e_Spr_HeroSub_DeathGen + 7,  4, e_Spr_HeroSub_DeathGen + 8,
4, e_Spr_HeroSub_DeathGen + 9,
4, e_Spr_HeroSub_DeathGen + 10, 4, e_Spr_HeroSub_DeathGen + 11, 4, e_Spr_HeroSub_DeathGen + 12,
4, e_Spr_HeroSub_DeathGen + 13, 4, e_Spr_HeroSub_DeathGen + 14, 4, e_Spr_HeroSub_DeathGen + 15,
4, e_Spr_HeroSub_DeathGen + 16, 4, e_Spr_HeroSub_DeathGen + 17, 4, e_Spr_HeroSub_DeathGen + 18,
4, e_Spr_HeroSub_DeathGen + 19,
4, e_Spr_HeroSub_DeathGen + 20, 4, e_Spr_HeroSub_DeathGen + 21, 4, e_Spr_HeroSub_DeathGen + 22,
ANIM_CLIGNO(e_Spr_HeroSub_DeathGen + 22)
e_Anm_End
};
// Nage - Mort générique - Casque en dust.
u64	gAnm_HeroSub_HelmetDeath_Dust[] =
{
e_AnmKey_Null,
8, e_Spr_HeroSub_HelmetDeath,
8, e_Spr_HeroSub_HelmetDeath + 1,
8, e_Spr_HeroSub_HelmetDeath + 2,
8, e_Spr_HeroSub_HelmetDeath + 3,
8, e_Spr_HeroSub_HelmetDeath + 4,
8, e_Spr_HeroSub_HelmetDeath + 5,
8, e_Spr_HeroSub_HelmetDeath + 6,
8, e_Spr_HeroSub_HelmetDeath + 7,
8, e_Spr_HeroSub_HelmetDeath + 8,
8, e_Spr_HeroSub_HelmetDeath + 9,
8, e_Spr_HeroSub_HelmetDeath + 10,
4, e_Spr_HeroSub_HelmetDeath + 11,
ANIM_CLIGNO(e_Spr_HeroSub_HelmetDeath + 11)
e_Anm_Kill
};
// Nage - Sortie de slug générique.
u64	gAnm_HeroSub_SlugOutGen[] =
{
e_AnmKey_Hero_SlugOut,
3, e_Spr_HeroSub_SlugOutGen,
3, e_Spr_HeroSub_SlugOutGen + 1,
3, e_Spr_HeroSub_SlugOutGen + 2,
3, e_Spr_HeroSub_SlugOutGen + 3,
3, e_Spr_HeroSub_SlugOutGen + 4,
3, e_Spr_HeroSub_SlugOutGen + 5,
3, (e_Spr_HeroSub_SlugOutGen) | SPR_Flip_X | SPR_Flip_Y,
3, (e_Spr_HeroSub_SlugOutGen + 1) | SPR_Flip_X | SPR_Flip_Y,
3, (e_Spr_HeroSub_SlugOutGen + 2) | SPR_Flip_X | SPR_Flip_Y,
3, (e_Spr_HeroSub_SlugOutGen + 3) | SPR_Flip_X | SPR_Flip_Y,
3, (e_Spr_HeroSub_SlugOutGen + 4) | SPR_Flip_X | SPR_Flip_Y,
3, (e_Spr_HeroSub_SlugOutGen + 5) | SPR_Flip_X | SPR_Flip_Y,
3, e_Spr_HeroSub_SlugOutGen,
3, e_Spr_HeroSub_SlugOutGen + 1,
e_Anm_Goto, (u64)gAnm_HeroSub_Stance
};

// Naked gun - Bored 0 - Boit dans sa gourde.
u64	gAnm_HeroTop_Bored0_Gun[] =
{
e_AnmKey_Hero_Bored,
4, e_Spr_HeroTop_Bored0_Gun,       4, e_Spr_HeroTop_Bored0_Gun + 1,   4, e_Spr_HeroTop_Bored0_Gun + 2,
4, e_Spr_HeroTop_Bored0_Gun + 3,   4, e_Spr_HeroTop_Bored0_Gun + 4,   4, e_Spr_HeroTop_Bored0_Gun + 5,
4, e_Spr_HeroTop_Bored0_Gun + 6,
8, e_Spr_HeroTop_Bored0_Gun + 7,
8, e_Spr_HeroTop_Bored0_Gun + 8,   8, e_Spr_HeroTop_Bored0_Gun + 9,
8, e_Spr_HeroTop_Bored0_Gun + 8,   8, e_Spr_HeroTop_Bored0_Gun + 9,
8, e_Spr_HeroTop_Bored0_Gun + 8,   8, e_Spr_HeroTop_Bored0_Gun + 9,
8, e_Spr_HeroTop_Bored0_Gun + 7,
4, e_Spr_HeroTop_Bored0_Gun + 6,
4, e_Spr_HeroTop_Bored0_Gun + 5,   4, e_Spr_HeroTop_Bored0_Gun + 4,   4, e_Spr_HeroTop_Bored0_Gun + 3,
4, e_Spr_HeroTop_Bored0_Gun + 2,   4, e_Spr_HeroTop_Bored0_Gun + 1,   4, e_Spr_HeroTop_Bored0_Gun,
e_Anm_Goto, (u64)gAnm_Hero_Stance
};
// Naked gun - Bored 1 - Recharge le pistolet.
u64	gAnm_HeroTop_Bored1_Gun[] =
{
e_AnmKey_Hero_Bored,
8, e_Spr_HeroTop_Bored1_Gun,
8, e_Spr_HeroTop_Bored1_Gun + 1,
8, e_Spr_HeroTop_Bored1_Gun + 2,
8, e_Spr_HeroTop_Bored1_Gun + 3,
8, e_Spr_HeroTop_Bored1_Gun + 4,
8, e_Spr_HeroTop_Bored1_Gun + 5,
8, e_Spr_HeroTop_Bored1_Gun + 6,
8, e_Spr_HeroTop_Bored1_Gun + 7,
8, e_Spr_HeroTop_Bored1_Gun + 8,
8, e_Spr_HeroTop_Bored1_Gun + 9,
8, e_Spr_HeroTop_Bored1_Gun + 10,
8, e_Spr_HeroTop_Bored1_Gun + 11,
8, e_Spr_HeroTop_Bored1_Gun + 12,
8, e_Spr_HeroTop_Bored1_Gun + 13,
8, e_Spr_HeroTop_Bored1_Gun + 14,
8, e_Spr_HeroTop_Bored1_Gun + 15,
8, e_Spr_HeroTop_Bored1_Gun + 16,
8, e_Spr_HeroTop_Bored1_Gun + 17,
e_Anm_Goto, (u64)gAnm_Hero_Stance
};

// Naked shotgun - Bored 0 - Talkie walkie.
u64	gAnm_HeroTop_Bored0_Shotgun[] =
{
e_AnmKey_Hero_Bored,
5, e_Spr_HeroTop_Bored0_Shotgun,
5, e_Spr_HeroTop_Bored0_Shotgun + 1,
5, e_Spr_HeroTop_Bored0_Shotgun + 2,
16, e_Spr_HeroTop_Bored0_Shotgun + 3,
5, e_Spr_HeroTop_Bored0_Shotgun + 4,
5, e_Spr_HeroTop_Bored0_Shotgun + 5,
5, e_Spr_HeroTop_Bored0_Shotgun + 6,
5, e_Spr_HeroTop_Bored0_Shotgun + 7,
5, e_Spr_HeroTop_Bored0_Shotgun + 8,
4, e_Spr_HeroTop_Bored0_Shotgun + 9,   4, e_Spr_HeroTop_Bored0_Shotgun + 10,   4, e_Spr_HeroTop_Bored0_Shotgun + 11,
4, e_Spr_HeroTop_Bored0_Shotgun + 9,   4, e_Spr_HeroTop_Bored0_Shotgun + 10,   4, e_Spr_HeroTop_Bored0_Shotgun + 11,
4, e_Spr_HeroTop_Bored0_Shotgun + 9,   4, e_Spr_HeroTop_Bored0_Shotgun + 10,   4, e_Spr_HeroTop_Bored0_Shotgun + 11,
5, e_Spr_HeroTop_Bored0_Shotgun + 8,
5, e_Spr_HeroTop_Bored0_Shotgun + 7,
5, e_Spr_HeroTop_Bored0_Shotgun + 6,
e_Anm_Goto, (u64)gAnm_Hero_Stance
};
// Naked shotgun - Bored 1 - S'endort.
u64	gAnm_HeroTop_Bored1_Shotgun[] =
{
e_AnmKey_Hero_Bored,
8, e_Spr_HeroTop_Bored1_Shotgun,
8, e_Spr_HeroTop_Bored1_Shotgun + 1,
8, e_Spr_HeroTop_Bored1_Shotgun + 2,
8, e_Spr_HeroTop_Bored1_Shotgun + 3,   8, e_Spr_HeroTop_Bored1_Shotgun + 4,   8, e_Spr_HeroTop_Bored1_Shotgun + 5,
8, e_Spr_HeroTop_Bored1_Shotgun + 6,  16, e_Spr_HeroTop_Bored1_Shotgun + 7,   8, e_Spr_HeroTop_Bored1_Shotgun + 6,
8, e_Spr_HeroTop_Bored1_Shotgun + 5,   8, e_Spr_HeroTop_Bored1_Shotgun + 4,
8, e_Spr_HeroTop_Bored1_Shotgun + 3,   8, e_Spr_HeroTop_Bored1_Shotgun + 4,   8, e_Spr_HeroTop_Bored1_Shotgun + 5,
8, e_Spr_HeroTop_Bored1_Shotgun + 6,  16, e_Spr_HeroTop_Bored1_Shotgun + 7,   8, e_Spr_HeroTop_Bored1_Shotgun + 6,
8, e_Spr_HeroTop_Bored1_Shotgun + 5,   8, e_Spr_HeroTop_Bored1_Shotgun + 4,
8, e_Spr_HeroTop_Bored1_Shotgun + 3,   8, e_Spr_HeroTop_Bored1_Shotgun + 4,   8, e_Spr_HeroTop_Bored1_Shotgun + 5,
8, e_Spr_HeroTop_Bored1_Shotgun + 6,  16, e_Spr_HeroTop_Bored1_Shotgun + 7,   8, e_Spr_HeroTop_Bored1_Shotgun + 6,
8, e_Spr_HeroTop_Bored1_Shotgun + 5,   8, e_Spr_HeroTop_Bored1_Shotgun + 4,
8, e_Spr_HeroTop_Bored1_Shotgun + 3,
8, e_Spr_HeroTop_Bored1_Shotgun + 2,
8, e_Spr_HeroTop_Bored1_Shotgun + 1,
8, e_Spr_HeroTop_Bored1_Shotgun,
e_Anm_Goto, (u64)gAnm_Hero_Stance
};

// Naked machinegun - Bored 0 - S'endort.
u64	gAnm_HeroTop_Bored0_Machinegun[] =
{
e_AnmKey_Hero_Bored,
8, e_Spr_HeroTop_Bored0_Machinegun,
8, e_Spr_HeroTop_Bored0_Machinegun + 1,
8, e_Spr_HeroTop_Bored0_Machinegun + 2,
8, e_Spr_HeroTop_Bored0_Machinegun + 3,   8, e_Spr_HeroTop_Bored0_Machinegun + 4,   8, e_Spr_HeroTop_Bored0_Machinegun + 5,
8, e_Spr_HeroTop_Bored0_Machinegun + 6,  16, e_Spr_HeroTop_Bored0_Machinegun + 7,   8, e_Spr_HeroTop_Bored0_Machinegun + 6,
8, e_Spr_HeroTop_Bored0_Machinegun + 5,   8, e_Spr_HeroTop_Bored0_Machinegun + 4,
8, e_Spr_HeroTop_Bored0_Machinegun + 3,   8, e_Spr_HeroTop_Bored0_Machinegun + 4,   8, e_Spr_HeroTop_Bored0_Machinegun + 5,
8, e_Spr_HeroTop_Bored0_Machinegun + 6,  16, e_Spr_HeroTop_Bored0_Machinegun + 7,   8, e_Spr_HeroTop_Bored0_Machinegun + 6,
8, e_Spr_HeroTop_Bored0_Machinegun + 5,   8, e_Spr_HeroTop_Bored0_Machinegun + 4,
8, e_Spr_HeroTop_Bored0_Machinegun + 3,   8, e_Spr_HeroTop_Bored0_Machinegun + 4,   8, e_Spr_HeroTop_Bored0_Machinegun + 5,
8, e_Spr_HeroTop_Bored0_Machinegun + 6,  16, e_Spr_HeroTop_Bored0_Machinegun + 7,   8, e_Spr_HeroTop_Bored0_Machinegun + 6,
8, e_Spr_HeroTop_Bored0_Machinegun + 5,   8, e_Spr_HeroTop_Bored0_Machinegun + 4,
8, e_Spr_HeroTop_Bored0_Machinegun + 3,
8, e_Spr_HeroTop_Bored0_Machinegun + 2,
8, e_Spr_HeroTop_Bored0_Machinegun + 1,
8, e_Spr_HeroTop_Bored0_Machinegun,
e_Anm_Goto, (u64)gAnm_Hero_Stance
};


// Repos (legs).
u64	gAnm_Hero_Stance[] =
{
e_AnmKey_Hero_Stance,
8, e_Spr_HeroLegs_Stance,
8, e_Spr_HeroLegs_Stance+1,
8, e_Spr_HeroLegs_Stance+2,
8, e_Spr_HeroLegs_Stance+3,
8, e_Spr_HeroLegs_Stance+2,
8, e_Spr_HeroLegs_Stance+1,
e_Anm_Goto, (u64)gAnm_Hero_Stance
};
// Course (legs).
u64	gAnm_Hero_Run[] =
{
e_AnmKey_Hero_Run,
4, e_Spr_HeroLegs_Run,
e_Anm_StepFlag,				// Dust pas sur images 1 et 7.
4, e_Spr_HeroLegs_Run+1,
4, e_Spr_HeroLegs_Run+2,
4, e_Spr_HeroLegs_Run+3,
	e_Anm_StepFlag,
4, e_Spr_HeroLegs_Run+4,
4, e_Spr_HeroLegs_Run+5,
4, e_Spr_HeroLegs_Run+6,
e_Anm_StepFlag,
4, e_Spr_HeroLegs_Run+7,
4, e_Spr_HeroLegs_Run+8,
4, e_Spr_HeroLegs_Run+9,
	e_Anm_StepFlag,
4, e_Spr_HeroLegs_Run+10,
4, e_Spr_HeroLegs_Run+11,
e_Anm_Goto, (u64)gAnm_Hero_Run
};
// Saut montant (legs).
u64	gAnm_Hero_Jump[] =
{
e_AnmKey_Hero_Jump,
3, e_Spr_HeroLegs_Jump,
3, e_Spr_HeroLegs_Jump+1,
3, e_Spr_HeroLegs_Jump+2,
3, e_Spr_HeroLegs_Jump+3,
3, e_Spr_HeroLegs_Jump+4,
3, e_Spr_HeroLegs_Jump+5,
e_Anm_Jump, (u64)-2
};
// Saut descendant (legs).
u64	gAnm_Hero_Fall[] =
{
e_AnmKey_Hero_Jump,
3, e_Spr_HeroLegs_Jump+5,
3, e_Spr_HeroLegs_Jump+4,
3, e_Spr_HeroLegs_Jump+3,
3, e_Spr_HeroLegs_Jump+2,
e_Anm_Jump, (u64)-2
};
// Naked - Sortie de slug générique.
u64	gAnm_Hero_SlugOutGen[] =
{
e_AnmKey_Hero_SlugOut,
3, e_Spr_Hero_SlugOutGen,
3, e_Spr_Hero_SlugOutGen + 1,
3, e_Spr_Hero_SlugOutGen + 2,
3, e_Spr_Hero_SlugOutGen + 3,
3, e_Spr_Hero_SlugOutGen + 4,
3, e_Spr_Hero_SlugOutGen + 5,
3, e_Spr_Hero_SlugOutGen + 6,
3, e_Spr_Hero_SlugOutGen + 7,
3, e_Spr_Hero_SlugOutGen + 8,
3, e_Spr_Hero_SlugOutGen + 9,
3, e_Spr_Hero_SlugOutGen + 10,
3, e_Spr_Hero_SlugOutGen + 11,
e_Anm_Goto, (u64)gAnm_Hero_SlugOutGen
};

// Repos accroupi.
u64	gAnm_Hero_CrouchStance[] =
{
e_AnmKey_Hero_CrouchStance,
8, 0,
8, 1,
8, 2,
8, 3,
8, 2,
8, 1,
e_Anm_Goto, (u64)gAnm_Hero_CrouchStance
};
// Marche accroupie.
u64	gAnm_Hero_CrouchWalk[] =
{
e_AnmKey_Hero_CrouchWalk,
8, 0,
//	e_Anm_StepFlag,
8, 1,
8, 2,
8, 3,
//4, 4,
//	e_Anm_StepFlag,
//4, 4,
8, 4,
8, 5,
8, 6,
e_Anm_Goto, (u64)gAnm_Hero_CrouchWalk
};

// Lancé de grenade (anim identique pour toutes les armes).
u64	gAnm_Hero_ThrowHG_Gen[] =
{
e_AnmKey_Hero_ThrowHG,
e_Anm_Sfx, e_Sfx_Fx_Swoosh, e_SfxPrio_Hero_Shot - 1,
3, 0,
3, 1,
4, 2,
4, 3,
3, 4,
e_Anm_Kill
};

// Fin du coup de couteau 1.
u64	gAnm_Hero_ShotEnd_Knife1[] =
{
e_AnmKey_Hero_KnifeEnd,
2, 4,
2, 3,
2, 2,
2, 1,
2, 0,
e_Anm_Kill
};
// Coup de couteau 1.
u64	gAnm_Hero_Shot_Knife1[] =
{
e_AnmKey_Hero_Knife,
e_Anm_Sfx, e_Sfx_Fx_Swoosh, e_SfxPrio_Hero_Shot - 1,
2, 0,
2, 1,
2, 2,
2, 3,
4, 4,
4, 5,
e_Anm_Goto, (u64)gAnm_Hero_ShotEnd_Knife1
};

// Fin du tir pistolet.
u64	gAnm_Hero_ShotEnd_Gun[] =
{
e_AnmKey_Hero_ShotEnd,
4, 2,
4, 3,
4, 4,
4, 5,
4, 6,
e_Anm_Kill
};
// Tir pistolet.
u64	gAnm_Hero_Shot_Gun[] =
{
e_AnmKey_Hero_Shot,
e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Hero_Shot,		//e_Sfx_Shot
4, 0,
4, 1,
e_Anm_Goto, (u64)gAnm_Hero_ShotEnd_Gun
};

// Fin du tir fusil à pompe.
u64	gAnm_Hero_ShotEnd_Shotgun[] =
{
e_AnmKey_Hero_ShotEnd,
6, 4,
e_Anm_Sfx, e_Sfx_Fx_GunReload, e_SfxPrio_0+5,
6, 5,
6, 6,
6, 7,
6, 8,
6, 9,
e_Anm_Kill
};
// Tir fusil à pompe.
u64	gAnm_Hero_Shot_Shotgun[] =
{
e_AnmKey_Hero_Shot,
e_Anm_SfxStop, e_Sfx_Fx_GunReload,
e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Hero_Shot,
8, 0,
6, 1,
6, 2,
6, 3,
e_Anm_Goto, (u64)gAnm_Hero_ShotEnd_Shotgun
};

// Tir au lance roquettes. = Tir au shotgun, mais son différent.		// *** SFX
u64	gAnm_Hero_Shot_RocketLauncher[] =
{
e_AnmKey_Hero_Shot,
e_Anm_SfxStop, e_Sfx_Fx_GunReload,
e_Anm_Sfx, e_Sfx_Shot_MiniRocket, e_SfxPrio_Hero_Shot,
8, 0,
6, 1,
6, 2,
6, 3,
e_Anm_Goto, (u64)gAnm_Hero_ShotEnd_Shotgun
};

// Tir mitrailleuse.
u64	gAnm_Hero_Shot_Machinegun[] =
{
e_AnmKey_Hero_Shot,
//e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Hero_Shot,	// Mitrailleuse et lance flammes : Sfx dans la balle (pb de l'arrêt de l'anim de tir avec getimage, son suivant joué).
6, 0,
//e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Hero_Shot,
6, 1,
//e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Hero_Shot,
6, 2,
//e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Hero_Shot,
6, 3,
e_Anm_Jump, (u64)-8//-20//-8
};
// Fin du tir mitrailleuse.
u64	gAnm_Hero_ShotEnd_Machinegun[] =
{
e_AnmKey_Hero_ShotEnd,
5, 4,
5, 5,
5, 6,
5, 7,
e_Anm_Kill
};

// Tir flame.
u64	gAnm_Hero_Shot_Flamethrower[] =
{
e_AnmKey_Hero_Shot,
//e_Anm_Sfx, e_Sfx_Shot_Flame, e_SfxPrio_Hero_Shot,
6, 10,
//e_Anm_Sfx, e_Sfx_Shot_Flame, e_SfxPrio_Hero_Shot,
6, 11,
//e_Anm_Sfx, e_Sfx_Shot_Flame, e_SfxPrio_Hero_Shot,
6, 12,
//e_Anm_Sfx, e_Sfx_Shot_Flame, e_SfxPrio_Hero_Shot,
6, 13,
e_Anm_Jump, (u64)-8//-20//-8
};
// Fin du tir flame.
u64	gAnm_Hero_ShotEnd_Flamethrower[] =
{
e_AnmKey_Hero_ShotEnd,
5, 4,
5, 5,
5, 6,
5, 7,
e_Anm_Kill
};

// Tir Machinegun sous marin = Anim du lance flammes + son de la mitrailleuse.	// *** SFX
u64	gAnm_Hero_Shot_SubMachinegun[] =
{
e_AnmKey_Hero_Shot,
//e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Hero_Shot,
6, 10,
//e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Hero_Shot,
6, 11,
//e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Hero_Shot,
6, 12,
//e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Hero_Shot,
6, 13,
e_Anm_Jump, (u64)-8//-20//-8
};

// Victoire.
u64	gAnm_Hero_Victory[] =
{
e_AnmKey_Hero_Victory,
8, 0,
8, 1,
8, 2,
8, 3,
8, 2,
8, 1,
e_Anm_Goto, (u64)gAnm_Hero_Victory
};

// Mort générique, debout.
u64	gAnm_Hero_DeathGen_Gnd[] =
{
e_AnmKey_Hero_Death,
e_Anm_Sfx, e_Sfx_Death_Scream1, e_SfxPrio_Hero_Shot + 10,
3, e_Spr_Hero_DeathGen_Gnd,
3, e_Spr_Hero_DeathGen_Gnd + 1,
3, e_Spr_Hero_DeathGen_Gnd + 2,
3, e_Spr_Hero_DeathGen_Gnd + 3,
3, e_Spr_Hero_DeathGen_Gnd + 4,
3, e_Spr_Hero_DeathGen_Gnd + 5,
3, e_Spr_Hero_DeathGen_Gnd + 6,
3, e_Spr_Hero_DeathGen_Gnd + 7,
3, e_Spr_Hero_DeathGen_Gnd + 8,
3, e_Spr_Hero_DeathGen_Gnd + 9,
16, e_Spr_Hero_DeathGen_Gnd + 10,
4, e_Spr_Hero_DeathGen_Gnd + 11,
4, e_Spr_Hero_DeathGen_Gnd + 12,
4, e_Spr_Hero_DeathGen_Gnd + 13,
4, e_Spr_Hero_DeathGen_Gnd + 14,
4, e_Spr_Hero_DeathGen_Gnd + 15,
4, e_Spr_Hero_DeathGen_Gnd + 16,
6, e_Spr_Hero_DeathGen_Gnd + 17,
12, e_Spr_Hero_DeathGen_Gnd + 18,
ANIM_CLIGNO(e_Spr_Hero_DeathGen_Gnd + 18)
e_Anm_End
};
// Mort générique, touché en l'air.
u64	gAnm_Hero_DeathGen_Air[] =
{
e_AnmKey_Hero_DeathAir,
e_Anm_Sfx, e_Sfx_Death_Scream1, e_SfxPrio_Hero_Shot + 10,
6, e_Spr_Hero_DeathGen_Air,
6, e_Spr_Hero_DeathGen_Air + 1,
6, e_Spr_Hero_DeathGen_Air + 2,
4, e_Spr_Hero_DeathGen_Air + 3,
4, e_Spr_Hero_DeathGen_Air + 4,
4, e_Spr_Hero_DeathGen_Air + 5,
4, e_Spr_Hero_DeathGen_Air + 6,
// Loop.
4, e_Spr_Hero_DeathGen_Air + 7,
4, e_Spr_Hero_DeathGen_Air + 8,
4, e_Spr_Hero_DeathGen_Air + 9,
e_Anm_Jump, (u64)-6
};
// Mort générique, atterrissage.
u64	gAnm_Hero_DeathGen_Land[] =
{
e_AnmKey_Hero_Death,
2, e_Spr_Hero_DeathGen_Land,
2, e_Spr_Hero_DeathGen_Land + 1,
2, e_Spr_Hero_DeathGen_Land + 2,
2, e_Spr_Hero_DeathGen_Land + 3,
2, e_Spr_Hero_DeathGen_Land + 4,
2, e_Spr_Hero_DeathGen_Land + 5,
2, e_Spr_Hero_DeathGen_Land + 6,
2, e_Spr_Hero_DeathGen_Land + 7,
2, e_Spr_Hero_DeathGen_Land + 8,
2, e_Spr_Hero_DeathGen_Land + 9,
2, e_Spr_Hero_DeathGen_Land + 10,
2, e_Spr_Hero_DeathGen_Land + 11,
2, e_Spr_Hero_DeathGen_Land + 12,
12, e_Spr_Hero_DeathGen_Land + 13,
ANIM_CLIGNO(e_Spr_Hero_DeathGen_Land + 13)
e_Anm_End
};

// Armes seules pour dust.
u64	gAnm_Weapon_Gun_Dust[] =
{
e_AnmKey_Null,
128, e_Spr_Weapon_Gun,
e_Anm_Jump, (u64)-2
};
u64	gAnm_Weapon_Shotgun_Dust[] =
{
e_AnmKey_Null,
128, e_Spr_Weapon_Shotgun,
e_Anm_Jump, (u64)-2
};
u64	gAnm_Weapon_Machinegun_Dust[] =
{
e_AnmKey_Null,
128, e_Spr_Weapon_Machinegun,
e_Anm_Jump, (u64)-2
};

// Trace de pas : Sable.
u64	gAnm_HumanStep_Sand_Dust[] =
{
e_AnmKey_Null,
32, e_Spr_Steps_Sand,
12, e_Spr_Steps_Sand + 1,
12, e_Spr_Steps_Sand + 2,
12, e_Spr_Steps_Sand + 3,
12, e_Spr_Steps_Sand + 4,
12, e_Spr_Steps_Sand + 5,
12, e_Spr_Steps_Sand + 6,
12, e_Spr_Steps_Sand + 7,
e_Anm_Kill
};
// Step dans l'eau (tous humains) - Dust.
u64	gAnm_HumanStep_Water_Dust[] =
{
e_AnmKey_Null,
/*
3, e_Spr_Human_WaterStep,
*/
e_Anm_Sfx, e_Sfx_Fx_WaterSplash, e_SfxPrio_0 + 5,
3, e_Spr_Human_WaterStep + 1,
3, e_Spr_Human_WaterStep + 2,
3, e_Spr_Human_WaterStep + 3,
3, e_Spr_Human_WaterStep + 4,
3, e_Spr_Human_WaterStep + 5,
3, e_Spr_Human_WaterStep + 6,
3, e_Spr_Human_WaterStep + 7,
3, e_Spr_Human_WaterStep + 8,
3, e_Spr_Human_WaterStep + 9,
e_Anm_Kill
};
// Atterrissage dans l'eau - Dust.
u64	gAnm_Land_Water_Dust[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Fx_WaterSplash2, e_SfxPrio_0 + 5,
3, e_Spr_Hero_DeathGen_Plouf + 17,
3, e_Spr_Hero_DeathGen_Plouf + 18,
3, e_Spr_Hero_DeathGen_Plouf + 19,
3, e_Spr_Hero_DeathGen_Plouf + 20,
3, e_Spr_Hero_DeathGen_Plouf + 21,
3, e_Spr_Hero_DeathGen_Plouf + 22,
3, e_Spr_Hero_DeathGen_Plouf + 23,
3, e_Spr_Hero_DeathGen_Plouf + 24,
3, e_Spr_Hero_DeathGen_Plouf + 25,
3, e_Spr_Hero_DeathGen_Plouf + 26,
e_Anm_Kill
};
// Atterrissage dans l'eau avec un slug - Dust.
u64	gAnm_Slug_Land_Water_Dust[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Fx_WaterSplash2, e_SfxPrio_0 + 5,
3, e_Spr_Slug_WaterPlouf,      3, e_Spr_Slug_WaterPlouf + 1,  3, e_Spr_Slug_WaterPlouf + 2,
3, e_Spr_Slug_WaterPlouf + 3,  3, e_Spr_Slug_WaterPlouf + 4,  3, e_Spr_Slug_WaterPlouf + 5,
3, e_Spr_Slug_WaterPlouf + 6,  3, e_Spr_Slug_WaterPlouf + 7,  3, e_Spr_Slug_WaterPlouf + 8,
3, e_Spr_Slug_WaterPlouf + 9,
3, e_Spr_Slug_WaterPlouf + 10, 3, e_Spr_Slug_WaterPlouf + 11, 3, e_Spr_Slug_WaterPlouf + 12,
3, e_Spr_Slug_WaterPlouf + 13, 3, e_Spr_Slug_WaterPlouf + 14, 3, e_Spr_Slug_WaterPlouf + 15,
3, e_Spr_Slug_WaterPlouf + 16,
e_Anm_Kill
};

// Tir héros - Pistolet H.
u64	gAnm_ShotHero_PistolH[] =
{
e_AnmKey_Null,
//e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Hero_Shot,
128, e_Spr_Shot_BulletH,
e_Anm_Jump, (u64)-2
};
// Tir héros - Pistolet H - Dust.
u64	gAnm_ShotHero_PistolH_Dust[] =
{
e_AnmKey_Null,
4, e_Spr_Shot_BulletH_Dust,
4, e_Spr_Shot_BulletH_Dust + 1,
4, e_Spr_Shot_BulletH_Dust + 2,
e_Anm_Kill
};
// Tir héros - Pistolet V.
u64	gAnm_ShotHero_PistolV[] =
{
e_AnmKey_Null,
//e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Hero_Shot,
128, e_Spr_Shot_BulletV,
e_Anm_Jump, (u64)-2
};
// Tir héros - Pistolet V - Dust.
u64	gAnm_ShotHero_PistolV_Dust[] =
{
e_AnmKey_Null,
4, e_Spr_Shot_BulletV_Dust,
4, e_Spr_Shot_BulletV_Dust + 1,
4, e_Spr_Shot_BulletV_Dust + 2,
e_Anm_Kill
};

// Tir héros - Mitrailleuse H.
u64	gAnm_ShotHero_MachinegunH[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Hero_Shot,
128, e_Spr_Shot_MachgunH,
e_Anm_Jump, (u64)-2
};
// Tir héros - Mitrailleuse V.
u64	gAnm_ShotHero_MachinegunV[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Hero_Shot,
128, e_Spr_Shot_MachgunV,
e_Anm_Jump, (u64)-2
};

// Tir héros - Slug Rocket - Mitrailleuse V (sans son).
u64	gAnm_ShotHero_SlugRocket_MachinegunV[] =
{
e_AnmKey_Null,
128, e_Spr_Shot_MachgunV,
e_Anm_Jump, (u64)-2
};

// Tir héros - Fusil H.
u64	gAnm_ShotHero_ShotgunH[] =
{
e_AnmKey_Null,
2, e_Spr_Shot_ShotgunH,
2, e_Spr_Shot_ShotgunH + 1,
2, e_Spr_Shot_ShotgunH + 2,
2, e_Spr_Shot_ShotgunH + 3,
2, e_Spr_Shot_ShotgunH + 4,
2, e_Spr_Shot_ShotgunH + 5,
2, e_Spr_Shot_ShotgunH + 6,
2, e_Spr_Shot_ShotgunH + 7,
2, e_Spr_Shot_ShotgunH + 8,
2, e_Spr_Shot_ShotgunH + 9,
2, e_Spr_Shot_ShotgunH + 10,
2, e_Spr_Shot_ShotgunH + 11,
e_Anm_End
};
// Tir héros - Fusil V.
u64	gAnm_ShotHero_ShotgunV[] =
{
e_AnmKey_Null,
2, e_Spr_Shot_ShotgunV,
2, e_Spr_Shot_ShotgunV + 1,
2, e_Spr_Shot_ShotgunV + 2,
2, e_Spr_Shot_ShotgunV + 3,
2, e_Spr_Shot_ShotgunV + 4,
2, e_Spr_Shot_ShotgunV + 5,
2, e_Spr_Shot_ShotgunV + 6,
2, e_Spr_Shot_ShotgunV + 7,
2, e_Spr_Shot_ShotgunV + 8,
2, e_Spr_Shot_ShotgunV + 9,
2, e_Spr_Shot_ShotgunV + 10,
2, e_Spr_Shot_ShotgunV + 11,
e_Anm_End
};

// Tir héros - Flame H.
u64	gAnm_ShotHero_FlameH[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Shot_Flame, e_SfxPrio_Hero_Shot,
2, e_Spr_Shot_FlameH,
2, e_Spr_Shot_FlameH + 1,
2, e_Spr_Shot_FlameH + 2,
2, e_Spr_Shot_FlameH + 3,
2, e_Spr_Shot_FlameH + 4,
2, e_Spr_Shot_FlameH + 5,
2, e_Spr_Shot_FlameH + 6,
2, e_Spr_Shot_FlameH + 7,
2, e_Spr_Shot_FlameH + 8,
2, e_Spr_Shot_FlameH + 9,
2, e_Spr_Shot_FlameH + 10,
2, e_Spr_Shot_FlameH + 11,
2, e_Spr_Shot_FlameH + 12,
2, e_Spr_Shot_FlameH + 13,
e_Anm_End
};
// Tir héros - Flame H - Dust de fin de tir.
u64	gAnm_ShotHero_FlameH_Dust[] =
{
e_AnmKey_Null,
2, e_Spr_Shot_FlameH + 9,
2, e_Spr_Shot_FlameH + 10,
2, e_Spr_Shot_FlameH + 11,
2, e_Spr_Shot_FlameH + 12,
2, e_Spr_Shot_FlameH + 13,
e_Anm_Kill
};
// Tir héros - Flame V.
u64	gAnm_ShotHero_FlameV[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Shot_Flame, e_SfxPrio_Hero_Shot,
2, e_Spr_Shot_FlameV,
2, e_Spr_Shot_FlameV + 1,
2, e_Spr_Shot_FlameV + 2,
2, e_Spr_Shot_FlameV + 3,
2, e_Spr_Shot_FlameV + 4,
2, e_Spr_Shot_FlameV + 5,
2, e_Spr_Shot_FlameV + 6,
2, e_Spr_Shot_FlameV + 7,
2, e_Spr_Shot_FlameV + 8,
2, e_Spr_Shot_FlameV + 9,
2, e_Spr_Shot_FlameV + 10,
2, e_Spr_Shot_FlameV + 11,
2, e_Spr_Shot_FlameV + 12,
2, e_Spr_Shot_FlameV + 13,
e_Anm_End
};
// Tir héros - Flame V - Dust de fin de tir.
u64	gAnm_ShotHero_FlameV_Dust[] =
{
e_AnmKey_Null,
2, e_Spr_Shot_FlameV + 9,
2, e_Spr_Shot_FlameV + 10,
2, e_Spr_Shot_FlameV + 11,
2, e_Spr_Shot_FlameV + 12,
2, e_Spr_Shot_FlameV + 13,
e_Anm_Kill
};

// Tir héros - Grenade (rotation : 32 sprites).
u64	gAnm_ShotHero_HandGrenade_Air[] =
{
e_AnmKey_Null,
1, e_Spr_Shot_Grenade_Rotate,       1, e_Spr_Shot_Grenade_Rotate + 1,   1, e_Spr_Shot_Grenade_Rotate + 2,
1, e_Spr_Shot_Grenade_Rotate + 3,   1, e_Spr_Shot_Grenade_Rotate + 4,   1, e_Spr_Shot_Grenade_Rotate + 5,
1, e_Spr_Shot_Grenade_Rotate + 6,   1, e_Spr_Shot_Grenade_Rotate + 7,   1, e_Spr_Shot_Grenade_Rotate + 8,
1, e_Spr_Shot_Grenade_Rotate + 9,
1, e_Spr_Shot_Grenade_Rotate + 10,  1, e_Spr_Shot_Grenade_Rotate + 11,  1, e_Spr_Shot_Grenade_Rotate + 12,
1, e_Spr_Shot_Grenade_Rotate + 13,  1, e_Spr_Shot_Grenade_Rotate + 14,  1, e_Spr_Shot_Grenade_Rotate + 15,
1, e_Spr_Shot_Grenade_Rotate + 16,  1, e_Spr_Shot_Grenade_Rotate + 17,  1, e_Spr_Shot_Grenade_Rotate + 18,
1, e_Spr_Shot_Grenade_Rotate + 19,
1, e_Spr_Shot_Grenade_Rotate + 20,  1, e_Spr_Shot_Grenade_Rotate + 21,  1, e_Spr_Shot_Grenade_Rotate + 22,
1, e_Spr_Shot_Grenade_Rotate + 23,  1, e_Spr_Shot_Grenade_Rotate + 24,  1, e_Spr_Shot_Grenade_Rotate + 25,
1, e_Spr_Shot_Grenade_Rotate + 26,  1, e_Spr_Shot_Grenade_Rotate + 27,  1, e_Spr_Shot_Grenade_Rotate + 28,
1, e_Spr_Shot_Grenade_Rotate + 29,
1, e_Spr_Shot_Grenade_Rotate + 30,  1, e_Spr_Shot_Grenade_Rotate + 31,
e_Anm_Goto, (u64)gAnm_ShotHero_HandGrenade_Air
};
// Tir héros - Explosion grenade.
u64	gAnm_ShotHero_HandGrenade_Explo[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Fx_Explosion2, e_SfxPrio_Explosion,
3, e_Spr_Shot_Grenade_Explo,      3, e_Spr_Shot_Grenade_Explo + 1,  3, e_Spr_Shot_Grenade_Explo + 2,
3, e_Spr_Shot_Grenade_Explo + 3,  3, e_Spr_Shot_Grenade_Explo + 4,  3, e_Spr_Shot_Grenade_Explo + 5,
3, e_Spr_Shot_Grenade_Explo + 6,  3, e_Spr_Shot_Grenade_Explo + 7,  3, e_Spr_Shot_Grenade_Explo + 8,
3, e_Spr_Shot_Grenade_Explo + 9,
3, e_Spr_Shot_Grenade_Explo + 10, 3, e_Spr_Shot_Grenade_Explo + 11, 3, e_Spr_Shot_Grenade_Explo + 12,
3, e_Spr_Shot_Grenade_Explo + 13, 3, e_Spr_Shot_Grenade_Explo + 14, 3, e_Spr_Shot_Grenade_Explo + 15,
3, e_Spr_Shot_Grenade_Explo + 16, 3, e_Spr_Shot_Grenade_Explo + 17, 3, e_Spr_Shot_Grenade_Explo + 18,
3, e_Spr_Shot_Grenade_Explo + 19,
3, e_Spr_Shot_Grenade_Explo + 20,
e_Anm_End
};


// Parachute (héros + rebel soldiers) - Plane.
u64	gAnm_Parachute_Glide[] =
{
e_AnmKey_Null,
12, e_Spr_Parachute_Glide,
12, e_Spr_Parachute_Glide + 1,
12, e_Spr_Parachute_Glide + 2,
12, e_Spr_Parachute_Glide + 3,
12, e_Spr_Parachute_Glide + 4,
12, e_Spr_Parachute_Glide + 3,
12, e_Spr_Parachute_Glide + 2,
12, e_Spr_Parachute_Glide + 1,
e_Anm_Goto, (u64)gAnm_Parachute_Glide
};
// Parachute (héros + rebel soldiers) - Se replie dans le sac à dos.
u64	gAnm_Parachute_Fold[] =
{
e_AnmKey_Null,
2, e_Spr_Parachute_Fold,       2, e_Spr_Parachute_Fold + 1,   2, e_Spr_Parachute_Fold + 2,
2, e_Spr_Parachute_Fold + 3,   2, e_Spr_Parachute_Fold + 4,   2, e_Spr_Parachute_Fold + 5,
2, e_Spr_Parachute_Fold + 6,   2, e_Spr_Parachute_Fold + 7,   2, e_Spr_Parachute_Fold + 8,
2, e_Spr_Parachute_Fold + 9,
2, e_Spr_Parachute_Fold + 10,  2, e_Spr_Parachute_Fold + 11,  2, e_Spr_Parachute_Fold + 12,
2, e_Spr_Parachute_Fold + 13,  2, e_Spr_Parachute_Fold + 14,  2, e_Spr_Parachute_Fold + 15,
2, e_Spr_Parachute_Fold + 16,
e_Anm_Kill	//e_Anm_End
};

// Brain Bot - Idle.
u64	gAnm_BrainBot_Idle[] =
{
e_AnmKey_Null,
128, e_Spr_BrainBot_Walk,
e_Anm_Jump, (u64)-2
};
// Brain Bot - Shot.
u64	gAnm_BrainBot_Shot[] =
{
e_AnmKey_Enemy_Shot,
32, e_Spr_BrainBot_Walk,
e_Anm_Goto, (u64)gAnm_BrainBot_Idle
};
// Brain Bot - "Walk".
#define	BRAINBOT_T0	3
u64	gAnm_BrainBot_Walk[] =
{
e_AnmKey_Null,
BRAINBOT_T0, e_Spr_BrainBot_Walk,
BRAINBOT_T0, e_Spr_BrainBot_Walk + 1,
BRAINBOT_T0, e_Spr_BrainBot_Walk + 2,
BRAINBOT_T0, e_Spr_BrainBot_Walk + 1,
BRAINBOT_T0, e_Spr_BrainBot_Walk,
BRAINBOT_T0, e_Spr_BrainBot_Walk + 3,
BRAINBOT_T0, e_Spr_BrainBot_Walk + 4,
BRAINBOT_T0, e_Spr_BrainBot_Walk + 5,
BRAINBOT_T0, e_Spr_BrainBot_Walk + 4,
BRAINBOT_T0, e_Spr_BrainBot_Walk + 3,
e_Anm_Goto, (u64)gAnm_BrainBot_Walk
};
// Brain Bot - Flip.
#define	BRAINBOT_T1	3
u64	gAnm_BrainBot_Flip[] =
{
e_AnmKey_Enemy_Flip,
BRAINBOT_T1, e_Spr_BrainBot_Flip,
BRAINBOT_T1, e_Spr_BrainBot_Flip + 1,
BRAINBOT_T1, e_Spr_BrainBot_Flip + 2,
BRAINBOT_T1, e_Spr_BrainBot_Flip + 3,
BRAINBOT_T1, e_Spr_BrainBot_Flip + 4,
BRAINBOT_T1, e_Spr_BrainBot_Flip + 5,
e_Anm_StepFlag,					// Signal pour placer le bit de flip.
BRAINBOT_T1, (e_Spr_BrainBot_Flip + 6) | SPR_Flip_X,
BRAINBOT_T1, (e_Spr_BrainBot_Flip + 7) | SPR_Flip_X,
BRAINBOT_T1, (e_Spr_BrainBot_Flip + 8) | SPR_Flip_X,
BRAINBOT_T1, (e_Spr_BrainBot_Flip + 9) | SPR_Flip_X,
BRAINBOT_T1, (e_Spr_BrainBot_Flip + 10) | SPR_Flip_X,
BRAINBOT_T1, (e_Spr_BrainBot_Flip + 11) | SPR_Flip_X,
e_Anm_Goto, (u64)gAnm_BrainBot_Idle
};
// Brain Bot - Mort - Dust. => 1ère anim en mvt.
#define	BRAINBOT_T2	4
u64	gAnm_BrainBot_Death_Dust[] =
{
e_AnmKey_Null,
BRAINBOT_T2, e_Spr_BrainBot_Death,
BRAINBOT_T2, e_Spr_BrainBot_Death + 1,
BRAINBOT_T2, e_Spr_BrainBot_Death + 2,
BRAINBOT_T2, e_Spr_BrainBot_Death + 3,
BRAINBOT_T2, e_Spr_BrainBot_Death + 4,
BRAINBOT_T2, e_Spr_BrainBot_Death + 5,
e_Anm_Kill
};
// Brain Bot - Mort, atterissage - Dust. => 2ème anim arrêtée. Lancée en même temps que la première.
#define	BRAINBOT_T2	4
u64	gAnm_BrainBot_DeathLand_Dust[] =
{
e_AnmKey_Null,
BRAINBOT_T2 * 6, SPR_NoSprite,			// = Temps de la première anim.
BRAINBOT_T2, e_Spr_BrainBot_Death + 6,
BRAINBOT_T2, e_Spr_BrainBot_Death + 7,
BRAINBOT_T2, e_Spr_BrainBot_Death + 8,
BRAINBOT_T2, e_Spr_BrainBot_Death + 9,
BRAINBOT_T2, e_Spr_BrainBot_Death + 10,
BRAINBOT_T2, e_Spr_BrainBot_Death + 11,
BRAINBOT_T2, e_Spr_BrainBot_Death + 12,
ANIM_CLIGNO(e_Spr_BrainBot_Death + 12)
e_Anm_Kill
};
// Brain Bot - Explosion du cerveau - Dust.
#define	BRAINBOT_T3	3
u64	gAnm_BrainBot_BrainExplo_Dust[] =
{
e_AnmKey_Null,
BRAINBOT_T3, e_Spr_BrainBot_BrainDust,
BRAINBOT_T3, e_Spr_BrainBot_BrainDust + 1,
BRAINBOT_T3, e_Spr_BrainBot_BrainDust + 2,
BRAINBOT_T3, e_Spr_BrainBot_BrainDust + 3,
BRAINBOT_T3, e_Spr_BrainBot_BrainDust + 4,
BRAINBOT_T3, e_Spr_BrainBot_BrainDust + 5,
BRAINBOT_T3, e_Spr_BrainBot_BrainDust + 6,
BRAINBOT_T3, e_Spr_BrainBot_BrainDust + 7,
BRAINBOT_T3, e_Spr_BrainBot_BrainDust + 8,
e_Anm_Kill
};
// Tir ennemi - Brain Bot > Ring, partie basse.
#define	BRAINBOT_T4	2
u64	gAnm_ShotEnemy_BrainBot_RingLow[] =
{
e_AnmKey_Null,
BRAINBOT_T4, e_Spr_BrainBot_ShotLow,
BRAINBOT_T4, e_Spr_BrainBot_ShotLow + 1,
BRAINBOT_T4, e_Spr_BrainBot_ShotLow + 2,
BRAINBOT_T4, e_Spr_BrainBot_ShotLow + 3,
BRAINBOT_T4, e_Spr_BrainBot_ShotLow + 4,
BRAINBOT_T4, e_Spr_BrainBot_ShotLow + 5,
BRAINBOT_T4, e_Spr_BrainBot_ShotLow + 6,
BRAINBOT_T4, e_Spr_BrainBot_ShotLow + 7,
BRAINBOT_T4+8, e_Spr_BrainBot_ShotLow + 8,
BRAINBOT_T4+8, e_Spr_BrainBot_ShotLow + 9,
BRAINBOT_T4+8, e_Spr_BrainBot_ShotLow + 10,
BRAINBOT_T4+8, e_Spr_BrainBot_ShotLow + 11,
BRAINBOT_T4, e_Spr_BrainBot_ShotLow + 12,
BRAINBOT_T4, e_Spr_BrainBot_ShotLow + 13,
BRAINBOT_T4, e_Spr_BrainBot_ShotLow + 14,
BRAINBOT_T4, e_Spr_BrainBot_ShotLow + 15,
BRAINBOT_T4, e_Spr_BrainBot_ShotLow + 16,
BRAINBOT_T4, e_Spr_BrainBot_ShotLow + 17,
e_Anm_End
};
// Tir ennemi - Brain Bot > Ring, partie haute (petit sprite superposé pour avoir 2 zones de col).
u64	gAnm_ShotEnemy_BrainBot_RingHigh[] =
{
e_AnmKey_Null,
BRAINBOT_T4, e_Spr_BrainBot_ShotHigh,
BRAINBOT_T4, e_Spr_BrainBot_ShotHigh + 1,
BRAINBOT_T4, e_Spr_BrainBot_ShotHigh + 2,
BRAINBOT_T4, e_Spr_BrainBot_ShotHigh + 3,
BRAINBOT_T4, e_Spr_BrainBot_ShotHigh + 4,
BRAINBOT_T4, e_Spr_BrainBot_ShotHigh + 5,
BRAINBOT_T4, e_Spr_BrainBot_ShotHigh + 6,
BRAINBOT_T4, e_Spr_BrainBot_ShotHigh + 7,
BRAINBOT_T4+8, e_Spr_BrainBot_ShotHigh + 8,
BRAINBOT_T4+8, e_Spr_BrainBot_ShotHigh + 9,
BRAINBOT_T4+8, e_Spr_BrainBot_ShotHigh + 10,
BRAINBOT_T4+8, e_Spr_BrainBot_ShotHigh + 11,
BRAINBOT_T4, e_Spr_BrainBot_ShotHigh + 12,
BRAINBOT_T4, e_Spr_BrainBot_ShotHigh + 13,
BRAINBOT_T4, e_Spr_BrainBot_ShotHigh + 14,
BRAINBOT_T4, e_Spr_BrainBot_ShotHigh + 15,
BRAINBOT_T4, e_Spr_BrainBot_ShotHigh + 16,
BRAINBOT_T4, e_Spr_BrainBot_ShotHigh + 17,
e_Anm_End
};

// Mars people - Idle.
u64	gAnm_MarsPeople_Idle[] =
{
e_AnmKey_Null,
3, e_Spr_MarsPeople_Idle,      3, e_Spr_MarsPeople_Idle + 1,  3, e_Spr_MarsPeople_Idle + 2,
3, e_Spr_MarsPeople_Idle + 3,  3, e_Spr_MarsPeople_Idle + 4,  3, e_Spr_MarsPeople_Idle + 5,
3, e_Spr_MarsPeople_Idle + 6,  3, e_Spr_MarsPeople_Idle + 7,  3, e_Spr_MarsPeople_Idle + 8,
3, e_Spr_MarsPeople_Idle + 9,
3, e_Spr_MarsPeople_Idle + 10, 3, e_Spr_MarsPeople_Idle + 11, 3, e_Spr_MarsPeople_Idle + 12,
3, e_Spr_MarsPeople_Idle + 13, 3, e_Spr_MarsPeople_Idle + 14,
3, e_Spr_MarsPeople_Idle + 15,
e_Anm_Goto, (u64)gAnm_MarsPeople_Idle
};
// Mars people - Shot.
u64	gAnm_MarsPeople_Shot[] =
{
e_AnmKey_Enemy_Shot,
3, e_Spr_MarsPeople_Shot,
3, e_Spr_MarsPeople_Shot + 1,
3, e_Spr_MarsPeople_Shot + 2,
e_Anm_Sfx, e_Sfx_Shot_Alien, e_SfxPrio_Enemy_Shot,
3, e_Spr_MarsPeople_Shot + 3,
3, e_Spr_MarsPeople_Shot + 4,
3, e_Spr_MarsPeople_Shot + 5,
3, e_Spr_MarsPeople_Shot + 6,
3, e_Spr_MarsPeople_Shot + 7,
3, e_Spr_MarsPeople_Shot + 8,
3, e_Spr_MarsPeople_Shot + 9,
3, e_Spr_MarsPeople_Shot + 10,
e_Anm_Goto, (u64)gAnm_MarsPeople_Idle
};
// Mars people - Flip (se tourne d'un côté à l'autre).
u64	gAnm_MarsPeople_Flip[] =
{
e_AnmKey_Enemy_Flip,
3, e_Spr_MarsPeople_Flip,
3, e_Spr_MarsPeople_Flip + 1,
3, e_Spr_MarsPeople_Flip + 2,
3, e_Spr_MarsPeople_Flip + 3,
3, e_Spr_MarsPeople_Flip + 4,
e_Anm_StepFlag,					// Signal pour placer le bit de flip.
3, (e_Spr_MarsPeople_Flip + 5) | SPR_Flip_X,
3, (e_Spr_MarsPeople_Flip + 6) | SPR_Flip_X,
3, (e_Spr_MarsPeople_Flip + 7) | SPR_Flip_X,
3, (e_Spr_MarsPeople_Flip + 8) | SPR_Flip_X,
3, (e_Spr_MarsPeople_Flip + 9) | SPR_Flip_X,
e_Anm_Goto, (u64)gAnm_MarsPeople_Idle
};
// Mars people - Mort générique - Dust.
u64	gAnm_MarsPeople_DeathGen_Dust[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Death_MarsPeople, e_SfxPrio_Enemy_Death,
4, e_Spr_MarsPeople_DeathGen,      4, e_Spr_MarsPeople_DeathGen + 1,  4, e_Spr_MarsPeople_DeathGen + 2,  4, e_Spr_MarsPeople_DeathGen + 3,
4, e_Spr_MarsPeople_DeathGen + 4,  4, e_Spr_MarsPeople_DeathGen + 5,  4, e_Spr_MarsPeople_DeathGen + 6,  4, e_Spr_MarsPeople_DeathGen + 7,
4, e_Spr_MarsPeople_DeathGen + 8,  4, e_Spr_MarsPeople_DeathGen + 9,
4, e_Spr_MarsPeople_DeathGen + 10, 4, e_Spr_MarsPeople_DeathGen + 11, 4, e_Spr_MarsPeople_DeathGen + 12, 4, e_Spr_MarsPeople_DeathGen + 13,
4, e_Spr_MarsPeople_DeathGen + 14, 4, e_Spr_MarsPeople_DeathGen + 15, 4, e_Spr_MarsPeople_DeathGen + 16, 4, e_Spr_MarsPeople_DeathGen + 17,
4, e_Spr_MarsPeople_DeathGen + 18, 4, e_Spr_MarsPeople_DeathGen + 19,
4, e_Spr_MarsPeople_DeathGen + 20, 4, e_Spr_MarsPeople_DeathGen + 21, 4, e_Spr_MarsPeople_DeathGen + 22, 4, e_Spr_MarsPeople_DeathGen + 23,
4, e_Spr_MarsPeople_DeathGen + 24, 4, e_Spr_MarsPeople_DeathGen + 25, 4, e_Spr_MarsPeople_DeathGen + 26, 4, e_Spr_MarsPeople_DeathGen + 27,
4, e_Spr_MarsPeople_DeathGen + 28, 4, e_Spr_MarsPeople_DeathGen + 29,
4, e_Spr_MarsPeople_DeathGen + 30, 4, e_Spr_MarsPeople_DeathGen + 31, 4, e_Spr_MarsPeople_DeathGen + 32, 4, e_Spr_MarsPeople_DeathGen + 33,
4, e_Spr_MarsPeople_DeathGen + 34, 4, e_Spr_MarsPeople_DeathGen + 35, 4, e_Spr_MarsPeople_DeathGen + 36, 4, e_Spr_MarsPeople_DeathGen + 37,
4, e_Spr_MarsPeople_DeathGen + 38, 4, e_Spr_MarsPeople_DeathGen + 39,
4, e_Spr_MarsPeople_DeathGen + 40, 4, e_Spr_MarsPeople_DeathGen + 41, 4, e_Spr_MarsPeople_DeathGen + 42,
e_Anm_Kill
};
// Mars people - Mort par le feu - Dust.
u64	gAnm_MarsPeople_DeathFire_Dust[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Death_MarsPeople, e_SfxPrio_Enemy_Death,
4, e_Spr_MarsPeople_DeathFire,      4, e_Spr_MarsPeople_DeathFire + 1,  4, e_Spr_MarsPeople_DeathFire + 2,  4, e_Spr_MarsPeople_DeathFire + 3,
4, e_Spr_MarsPeople_DeathFire + 4,  4, e_Spr_MarsPeople_DeathFire + 5,  4, e_Spr_MarsPeople_DeathFire + 6,  4, e_Spr_MarsPeople_DeathFire + 7,
4, e_Spr_MarsPeople_DeathFire + 8,  4, e_Spr_MarsPeople_DeathFire + 9,
4, e_Spr_MarsPeople_DeathFire + 10, 4, e_Spr_MarsPeople_DeathFire + 11, 4, e_Spr_MarsPeople_DeathFire + 12, 4, e_Spr_MarsPeople_DeathFire + 13,
4, e_Spr_MarsPeople_DeathFire + 14, 4, e_Spr_MarsPeople_DeathFire + 15, 4, e_Spr_MarsPeople_DeathFire + 16, 4, e_Spr_MarsPeople_DeathFire + 17,
4, e_Spr_MarsPeople_DeathFire + 18, 4, e_Spr_MarsPeople_DeathFire + 19,
4, e_Spr_MarsPeople_DeathFire + 20, 4, e_Spr_MarsPeople_DeathFire + 21, 4, e_Spr_MarsPeople_DeathFire + 22, 4, e_Spr_MarsPeople_DeathFire + 23,
4, e_Spr_MarsPeople_DeathFire + 24, 4, e_Spr_MarsPeople_DeathFire + 25, 4, e_Spr_MarsPeople_DeathFire + 26, 4, e_Spr_MarsPeople_DeathFire + 27,
4, e_Spr_MarsPeople_DeathFire + 28, 4, e_Spr_MarsPeople_DeathFire + 29,
4, e_Spr_MarsPeople_DeathFire + 30, 4, e_Spr_MarsPeople_DeathFire + 31, 4, e_Spr_MarsPeople_DeathFire + 32, 4, e_Spr_MarsPeople_DeathFire + 33,
4, e_Spr_MarsPeople_DeathFire + 34, 4, e_Spr_MarsPeople_DeathFire + 35, 4, e_Spr_MarsPeople_DeathFire + 36, 4, e_Spr_MarsPeople_DeathFire + 37,
4, e_Spr_MarsPeople_DeathFire + 38, 4, e_Spr_MarsPeople_DeathFire + 39,
4, e_Spr_MarsPeople_DeathFire + 40, 4, e_Spr_MarsPeople_DeathFire + 41, 4, e_Spr_MarsPeople_DeathFire + 42,
e_Anm_Kill
};

// Mars TIE - Fly.
#define	MARSTIE_T0	4
u64	gAnm_MarsTIE_Fly[] =
{
e_AnmKey_Null,
MARSTIE_T0, e_Spr_MarsTIE_Fly,
MARSTIE_T0, e_Spr_MarsTIE_Fly + 1,
MARSTIE_T0, e_Spr_MarsTIE_Fly + 2,
MARSTIE_T0, e_Spr_MarsTIE_Fly + 3,
MARSTIE_T0, e_Spr_MarsTIE_Fly + 4,
MARSTIE_T0, e_Spr_MarsTIE_Fly + 5,
MARSTIE_T0, e_Spr_MarsTIE_Fly + 6,
MARSTIE_T0, e_Spr_MarsTIE_Fly + 7,
MARSTIE_T0, e_Spr_MarsTIE_Fly + 8,
MARSTIE_T0, e_Spr_MarsTIE_Fly + 9,
MARSTIE_T0, e_Spr_MarsTIE_Fly + 10,
MARSTIE_T0, e_Spr_MarsTIE_Fly + 11,
MARSTIE_T0, e_Spr_MarsTIE_Fly + 12,
MARSTIE_T0, e_Spr_MarsTIE_Fly + 13,
MARSTIE_T0, e_Spr_MarsTIE_Fly + 14,
MARSTIE_T0, e_Spr_MarsTIE_Fly + 15,
e_Anm_Goto, (u64)gAnm_MarsTIE_Fly
};
// Mars TIE - Flip x.
u64	gAnm_MarsTIE_Flip[] =
{
e_AnmKey_Enemy_Key3,
MARSTIE_T0, e_Spr_MarsTIE_Flip + 2,
MARSTIE_T0, e_Spr_MarsTIE_Flip + 3,
e_Anm_StepFlag,					// Signal pour placer le bit de flip.
MARSTIE_T0, e_Spr_MarsTIE_Flip + 1,
MARSTIE_T0, e_Spr_MarsTIE_Flip,
e_Anm_Goto, (u64)gAnm_MarsTIE_Fly
};

// Mars Eye - Fly.
#define	MARSEYE_T0	4
u64	gAnm_MarsEye_Fly[] =
{
e_AnmKey_Null,
MARSEYE_T0, e_Spr_MarsEye_Fly,
MARSEYE_T0, e_Spr_MarsEye_Fly + 1,
MARSEYE_T0, e_Spr_MarsEye_Fly + 2,
MARSEYE_T0, e_Spr_MarsEye_Fly + 3,
MARSEYE_T0, e_Spr_MarsEye_Fly + 4,
MARSEYE_T0, e_Spr_MarsEye_Fly + 5,
MARSEYE_T0, e_Spr_MarsEye_Fly + 6,
MARSEYE_T0, e_Spr_MarsEye_Fly + 7,
MARSEYE_T0, e_Spr_MarsEye_Fly + 8,
MARSEYE_T0, e_Spr_MarsEye_Fly + 9,
MARSEYE_T0, e_Spr_MarsEye_Fly + 10,
MARSEYE_T0, e_Spr_MarsEye_Fly + 11,
MARSEYE_T0, e_Spr_MarsEye_Fly + 12,
MARSEYE_T0, e_Spr_MarsEye_Fly + 13,
MARSEYE_T0, e_Spr_MarsEye_Fly + 14,
MARSEYE_T0, e_Spr_MarsEye_Fly + 15,
e_Anm_Goto, (u64)gAnm_MarsEye_Fly
};
// Mars Eye - Flip x.
u64	gAnm_MarsEye_Flip[] =
{
e_AnmKey_Enemy_Key3,
MARSEYE_T0, e_Spr_MarsEye_Flip + 2,
MARSEYE_T0, e_Spr_MarsEye_Flip + 3,
e_Anm_StepFlag,					// Signal pour placer le bit de flip.
MARSEYE_T0, e_Spr_MarsEye_Flip + 1,
MARSEYE_T0, e_Spr_MarsEye_Flip,
e_Anm_Goto, (u64)gAnm_MarsEye_Fly
};

// Mars UFO - Fly.
#define	MARSUFO_T0	4
u64	gAnm_MarsUFO_Fly[] =
{
e_AnmKey_Null,
MARSUFO_T0, e_Spr_MarsUFO_Fly,
MARSUFO_T0, e_Spr_MarsUFO_Fly + 1,
MARSUFO_T0, e_Spr_MarsUFO_Fly + 2,
MARSUFO_T0, e_Spr_MarsUFO_Fly + 3,
MARSUFO_T0, e_Spr_MarsUFO_Fly + 4,
MARSUFO_T0, e_Spr_MarsUFO_Fly + 5,
MARSUFO_T0, e_Spr_MarsUFO_Fly + 6,
MARSUFO_T0, e_Spr_MarsUFO_Fly + 7,
MARSUFO_T0, e_Spr_MarsUFO_Fly + 8,
MARSUFO_T0, e_Spr_MarsUFO_Fly + 9,
MARSUFO_T0, e_Spr_MarsUFO_Fly + 10,
MARSUFO_T0, e_Spr_MarsUFO_Fly + 11,
e_Anm_Goto, (u64)gAnm_MarsUFO_Fly
};
// Mars UFO - Anim avant le tir.
u64	gAnm_MarsUFO_ShotFx[] =
{
e_AnmKey_Null,
3, e_Spr_MarsUFO_ShotFx,
3, e_Spr_MarsUFO_ShotFx + 1,
3, e_Spr_MarsUFO_ShotFx + 2,
3, e_Spr_MarsUFO_ShotFx + 3,
3, e_Spr_MarsUFO_ShotFx + 4,
3, e_Spr_MarsUFO_ShotFx + 5,
3, e_Spr_MarsUFO_ShotFx + 6,
3, e_Spr_MarsUFO_ShotFx + 7,
e_Anm_StepFlag,					// Signal pour tirer.
e_Anm_Sfx, e_Sfx_Shot_Alien, e_SfxPrio_Enemy_Shot,
3, e_Spr_MarsUFO_ShotDust,
3, e_Spr_MarsUFO_ShotDust + 1,
3, e_Spr_MarsUFO_ShotDust + 2,
3, e_Spr_MarsUFO_ShotDust + 3,
3, e_Spr_MarsUFO_ShotDust + 4,
3, e_Spr_MarsUFO_ShotDust + 5,
3, e_Spr_MarsUFO_ShotDust + 6,
3, e_Spr_MarsUFO_ShotDust + 7,
e_Anm_End
};
// Tir ennemi - Mars UFO > Green Bullet.
u64	gAnm_ShotEnemy_MarsUFO_GreenBullet0[] =
{
e_AnmKey_Null,
3, e_Spr_MarsUFO_ShotBullet,
3, e_Spr_MarsUFO_ShotBullet + 1,
3, e_Spr_MarsUFO_ShotBullet + 2,
3, e_Spr_MarsUFO_ShotBullet + 3,
3, e_Spr_MarsUFO_ShotBullet + 4,
3, e_Spr_MarsUFO_ShotBullet + 5,
3, e_Spr_MarsUFO_ShotBullet + 6,
3, e_Spr_MarsUFO_ShotBullet + 7,
e_Anm_Jump, (u64)-16
};
// Tir ennemi - Mars UFO > Green Bullet - Dust.
u64	gAnm_ShotEnemy_MarsUFO_GreenBullet0_Dust[] =
{
e_AnmKey_Null,
4, e_Spr_MarsUFO_ShotBullet_Dust,
4, e_Spr_MarsUFO_ShotBullet_Dust + 1,
4, e_Spr_MarsUFO_ShotBullet_Dust + 2,
e_Anm_Kill
};

// Mini UFO - Fly.
#define	MINIUFO_T0	4
u64	gAnm_MiniUFO_Fly[] =
{
e_AnmKey_Null,
MINIUFO_T0, e_Spr_MiniUFO_Fly,       MINIUFO_T0, e_Spr_MiniUFO_Fly + 1,   MINIUFO_T0, e_Spr_MiniUFO_Fly + 2,
MINIUFO_T0, e_Spr_MiniUFO_Fly + 3,   MINIUFO_T0, e_Spr_MiniUFO_Fly + 4,   MINIUFO_T0, e_Spr_MiniUFO_Fly + 5,
MINIUFO_T0, e_Spr_MiniUFO_Fly + 6,   MINIUFO_T0, e_Spr_MiniUFO_Fly + 7,   MINIUFO_T0, e_Spr_MiniUFO_Fly + 8,
MINIUFO_T0, e_Spr_MiniUFO_Fly + 9,
MINIUFO_T0, e_Spr_MiniUFO_Fly + 10,  MINIUFO_T0, e_Spr_MiniUFO_Fly + 11,  MINIUFO_T0, e_Spr_MiniUFO_Fly + 12,
MINIUFO_T0, e_Spr_MiniUFO_Fly + 13,  MINIUFO_T0, e_Spr_MiniUFO_Fly + 14,  MINIUFO_T0, e_Spr_MiniUFO_Fly + 15,
e_Anm_Goto, (u64)gAnm_MiniUFO_Fly
};
// Mini UFO - Explosion - Dust.
#define	MINIUFO_T1	4
u64	gAnm_MiniUFO_Explode_Dust[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Death_MarsPeople, e_SfxPrio_Enemy_Death,
MINIUFO_T1, e_Spr_MiniUFO_Explode,       MINIUFO_T1, e_Spr_MiniUFO_Explode + 1,   MINIUFO_T1, e_Spr_MiniUFO_Explode + 2,
MINIUFO_T1, e_Spr_MiniUFO_Explode + 3,   MINIUFO_T1, e_Spr_MiniUFO_Explode + 4,   MINIUFO_T1, e_Spr_MiniUFO_Explode + 5,
MINIUFO_T1, e_Spr_MiniUFO_Explode + 6,   MINIUFO_T1, e_Spr_MiniUFO_Explode + 7,   MINIUFO_T1, e_Spr_MiniUFO_Explode + 8,
MINIUFO_T1, e_Spr_MiniUFO_Explode + 9,
MINIUFO_T1, e_Spr_MiniUFO_Explode + 10,  MINIUFO_T1, e_Spr_MiniUFO_Explode + 11,  MINIUFO_T1, e_Spr_MiniUFO_Explode + 12,
MINIUFO_T1, e_Spr_MiniUFO_Explode + 13,  MINIUFO_T1, e_Spr_MiniUFO_Explode + 14,  MINIUFO_T1, e_Spr_MiniUFO_Explode + 15,
MINIUFO_T1, e_Spr_MiniUFO_Explode + 16,  MINIUFO_T1, e_Spr_MiniUFO_Explode + 17,  MINIUFO_T1, e_Spr_MiniUFO_Explode + 18,
MINIUFO_T1, e_Spr_MiniUFO_Explode + 19,
MINIUFO_T1, e_Spr_MiniUFO_Explode + 20,  MINIUFO_T1, e_Spr_MiniUFO_Explode + 21,  MINIUFO_T1, e_Spr_MiniUFO_Explode + 22,
MINIUFO_T1, e_Spr_MiniUFO_Explode + 23,
e_Anm_Kill
};

// Astéroïde.
#define	ASTEROID_T0	4
u64	gAnm_AsteroidMedium_Fly[] =
{
e_AnmKey_Null,
ASTEROID_T0, e_Spr_Asteroid_Rotate,      ASTEROID_T0, e_Spr_Asteroid_Rotate + 1,  ASTEROID_T0, e_Spr_Asteroid_Rotate + 2,
ASTEROID_T0, e_Spr_Asteroid_Rotate + 3,  ASTEROID_T0, e_Spr_Asteroid_Rotate + 4,  ASTEROID_T0, e_Spr_Asteroid_Rotate + 5,
ASTEROID_T0, e_Spr_Asteroid_Rotate + 6,  ASTEROID_T0, e_Spr_Asteroid_Rotate + 7,  ASTEROID_T0, e_Spr_Asteroid_Rotate + 8,
ASTEROID_T0, e_Spr_Asteroid_Rotate + 9,
ASTEROID_T0, e_Spr_Asteroid_Rotate + 10, ASTEROID_T0, e_Spr_Asteroid_Rotate + 11, ASTEROID_T0, e_Spr_Asteroid_Rotate + 12,
ASTEROID_T0, e_Spr_Asteroid_Rotate + 13, ASTEROID_T0, e_Spr_Asteroid_Rotate + 14,
e_Anm_Goto, (u64)gAnm_AsteroidMedium_Fly
};

// Gros Astéroïde - Se fissure.
#define	BIGASTEROID_T0	4
u64	gAnm_BigAsteroid_Cracks[] =
{
e_AnmKey_Null,
BIGASTEROID_T0, e_Spr_BigAsteroid + 2,
BIGASTEROID_T0, e_Spr_BigAsteroid + 4,
BIGASTEROID_T0, e_Spr_BigAsteroid + 6,
BIGASTEROID_T0, e_Spr_BigAsteroid + 8,
BIGASTEROID_T0, e_Spr_BigAsteroid + 10,
e_Anm_StepFlag,					// Flag pour repérer quand lancer les dusts.
BIGASTEROID_T0, e_Spr_BigAsteroid + 12,
4, e_Spr_BigAsteroid + 0,
e_Anm_StepFlag,
8, e_Spr_BigAsteroid + 0,
e_Anm_StepFlag,
8, e_Spr_BigAsteroid + 0,
e_Anm_StepFlag,
8, e_Spr_BigAsteroid + 0,		// On rallonge la dernière frame pour laisser le temps aux dusts de se jouer.
e_Anm_End
};
// Gros Astéroïde - Se fissure - Dust.
#define	BIGASTEROID_T1	3
u64	gAnm_BigAsteroid_Cracks_Dust[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Fx_Explosion2, e_SfxPrio_Explosion,
BIGASTEROID_T1, e_Spr_RebSoldier_LRAC_Shot_Dust + 2,
BIGASTEROID_T1, e_Spr_RebSoldier_LRAC_Shot_Dust + 3,
BIGASTEROID_T1, e_Spr_RebSoldier_LRAC_Shot_Dust + 4,
BIGASTEROID_T1, e_Spr_RebSoldier_LRAC_Shot_Dust + 5,
BIGASTEROID_T1, e_Spr_RebSoldier_LRAC_Shot_Dust + 6,
BIGASTEROID_T1, e_Spr_RebSoldier_LRAC_Shot_Dust + 7,
BIGASTEROID_T1, e_Spr_RebSoldier_LRAC_Shot_Dust + 8,
BIGASTEROID_T1, e_Spr_RebSoldier_LRAC_Shot_Dust + 9,
BIGASTEROID_T1, e_Spr_RebSoldier_LRAC_Shot_Dust + 10,
BIGASTEROID_T1, e_Spr_RebSoldier_LRAC_Shot_Dust + 11,
BIGASTEROID_T1, e_Spr_RebSoldier_LRAC_Shot_Dust + 12,
BIGASTEROID_T1, e_Spr_RebSoldier_LRAC_Shot_Dust + 13,
BIGASTEROID_T1, e_Spr_RebSoldier_LRAC_Shot_Dust + 14,
e_Anm_Kill
};

// Alien Mothership, clignotement des lampes (offset).
#define	ALIENMOTHERSHIP_T0	9
u64	gAnm_AlienMothershipBlink[] =
{
e_AnmKey_Null,
ALIENMOTHERSHIP_T0, 0,
ALIENMOTHERSHIP_T0, 3,
ALIENMOTHERSHIP_T0, 6,
e_Anm_Jump, (u64)-6
};

// Weapon Capsule - H (Heavy machine gun).
#define	WEAPONCAPSULE_T1	5
u64	gAnm_Itm_WeaponCapsuleH[] =
{
e_AnmKey_Null,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleH,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleH + 1,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleH + 2,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleH + 3,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleH + 2,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleH + 1,
e_Anm_Jump, (u64)-12
};
// Weapon Capsule - F (Flamethrower).
u64	gAnm_Itm_WeaponCapsuleF[] =
{
e_AnmKey_Null,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleF,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleF + 1,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleF + 2,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleF + 3,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleF + 2,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleF + 1,
e_Anm_Jump, (u64)-12
};
// Weapon Capsule - S (Shotgun).
u64	gAnm_Itm_WeaponCapsuleS[] =
{
e_AnmKey_Null,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleS,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleS + 1,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleS + 2,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleS + 3,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleS + 2,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleS + 1,
e_Anm_Jump, (u64)-12
};
// Weapon Capsule - R (Rocket).
u64	gAnm_Itm_WeaponCapsuleR[] =
{
e_AnmKey_Null,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleR,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleR + 1,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleR + 2,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleR + 3,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleR + 2,
WEAPONCAPSULE_T1, e_Spr_Itm_WeaponCapsuleR + 1,
e_Anm_Jump, (u64)-12
};
// Weapon Capsule - Effet quand ramassage - Dust.
u64	gAnm_Itm_WeaponCapsulePickUp_Dust[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Fx_GunReload, e_SfxPrio_0+5,
3, e_Spr_Itm_WeaponCapsulePickUp,
3, e_Spr_Itm_WeaponCapsulePickUp + 1,
3, e_Spr_Itm_WeaponCapsulePickUp + 2,
3, e_Spr_Itm_WeaponCapsulePickUp + 3,
3, e_Spr_Itm_WeaponCapsulePickUp + 4,
3, e_Spr_Itm_WeaponCapsulePickUp + 5,
e_Anm_Kill
};

// Item - Caisse de munitions.
u64	gAnm_Itm_AmmoBox1[] =
{
e_AnmKey_Null,
60, e_Spr_Itm_AmmoBox1,
3, e_Spr_Itm_AmmoBox1 + 1,
3, e_Spr_Itm_AmmoBox1 + 2,
3, e_Spr_Itm_AmmoBox1 + 3,
3, e_Spr_Itm_AmmoBox1 + 4,
3, e_Spr_Itm_AmmoBox1 + 5,
e_Anm_Jump, (u64)-12
};
// Item - Jerrican d'essence.
u64	gAnm_Itm_GasTank1[] =
{
e_AnmKey_Null,
60, e_Spr_Itm_GasTank1,
3, e_Spr_Itm_GasTank1 + 1,
3, e_Spr_Itm_GasTank1 + 2,
3, e_Spr_Itm_GasTank1 + 3,
3, e_Spr_Itm_GasTank1 + 4,
3, e_Spr_Itm_GasTank1 + 5,
e_Anm_Jump, (u64)-12
};
// Item - Caisse container.
u64	gAnm_Itm_Container1[] =
{
e_AnmKey_Null,
128, e_Spr_Itm_Container1,
e_Anm_Jump, (u64)-2
};
// Item - Caisse container - Dust.
u64	gAnm_Itm_Container1_Dust[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Fx_Wood, e_SfxPrio_Enemy_Shot - 5,
3, e_Spr_Itm_Container1_Dust,
3, e_Spr_Itm_Container1_Dust + 1,
3, e_Spr_Itm_Container1_Dust + 2,
3, e_Spr_Itm_Container1_Dust + 3,
3, e_Spr_Itm_Container1_Dust + 4,
3, e_Spr_Itm_Container1_Dust + 5,
3, e_Spr_Itm_Container1_Dust + 6,
3, e_Spr_Itm_Container1_Dust + 7,
3, e_Spr_Itm_Container1_Dust + 8,
3, e_Spr_Itm_Container1_Dust + 9,
e_Anm_Kill
};

// Debris - Metal 0 - Dust.
u64	gAnm_Debris_Metal0_Dust[] =
{
e_AnmKey_Null,
3, e_Spr_Debris_Metal0,
3, e_Spr_Debris_Metal0 + 1,
3, e_Spr_Debris_Metal0 + 2,
3, e_Spr_Debris_Metal0 + 3,
3, e_Spr_Debris_Metal0 + 4,
3, e_Spr_Debris_Metal0 + 5,
3, e_Spr_Debris_Metal0 + 6,
3, e_Spr_Debris_Metal0 + 7,
e_Anm_Goto, (u64)gAnm_Debris_Metal0_Dust
};
// Debris - Bois 0 - Dust.
u64	gAnm_Debris_Wood0_Dust[] =
{
e_AnmKey_Null,
3, e_Spr_Debris_Wood0,
3, e_Spr_Debris_Wood0 + 1,
3, e_Spr_Debris_Wood0 + 2,
3, e_Spr_Debris_Wood0 + 3,
3, e_Spr_Debris_Wood0 + 4,
3, e_Spr_Debris_Wood0 + 5,
3, e_Spr_Debris_Wood0 + 6,
3, e_Spr_Debris_Wood0 + 7,
3, e_Spr_Debris_Wood0 + 8,
3, e_Spr_Debris_Wood0 + 9,
e_Anm_Goto, (u64)gAnm_Debris_Wood0_Dust
};
// Debris - Rocher petit - Dust.
u64	gAnm_Debris_RockSmall0_Dust[] =
{
e_AnmKey_Null,
3, e_Spr_SpaceRock_Small,
3, e_Spr_SpaceRock_Small + 1,
3, e_Spr_SpaceRock_Small + 2,
3, e_Spr_SpaceRock_Small + 3,
3, e_Spr_SpaceRock_Small + 4,
3, e_Spr_SpaceRock_Small + 5,
3, e_Spr_SpaceRock_Small + 6,
3, e_Spr_SpaceRock_Small + 7,
e_Anm_Goto, (u64)gAnm_Debris_RockSmall0_Dust
};
// Debris - Rocher moyen - Dust.
u64	gAnm_Debris_RockMedium0_Dust[] =
{
e_AnmKey_Null,
3, e_Spr_SpaceRock_Medium,
3, e_Spr_SpaceRock_Medium + 1,
3, e_Spr_SpaceRock_Medium + 2,
3, e_Spr_SpaceRock_Medium + 3,
3, e_Spr_SpaceRock_Medium + 4,
3, e_Spr_SpaceRock_Medium + 5,
3, e_Spr_SpaceRock_Medium + 6,
3, e_Spr_SpaceRock_Medium + 7,
e_Anm_Goto, (u64)gAnm_Debris_RockMedium0_Dust
};

// POW - Attente assis.
u64	gAnm_POW_SittingWait[] =
{
e_AnmKey_Null,
6, e_Spr_Pow_SittingWait,
6, e_Spr_Pow_SittingWait + 1,
6, e_Spr_Pow_SittingWait + 2,
6, e_Spr_Pow_SittingWait + 3,
6, e_Spr_Pow_SittingWait + 4,
6, e_Spr_Pow_SittingWait + 3,
6, e_Spr_Pow_SittingWait + 2,
6, e_Spr_Pow_SittingWait + 1,
e_Anm_Jump, (u64)-16
};
// POW - Idle.
u64	gAnm_POW_Idle[] =
{
e_AnmKey_Null,
3, e_Spr_Pow_Idle,
e_Anm_Jump, (u64)-2
};
// POW - Assis libéré.
u64	gAnm_POW_SittingFree[] =
{
e_AnmKey_Null + 1,	// Il faut juste que ce ne soit pas la même clef que gAnm_POW_Idle.
3, e_Spr_Pow_SittingFree,
3, e_Spr_Pow_SittingFree + 1,
3, e_Spr_Pow_SittingFree + 2,
3, e_Spr_Pow_SittingFree + 3,
e_Anm_Goto, (u64)gAnm_POW_Idle
};
// POW - Totem libéré.
u64	gAnm_POW_TotemFree[] =
{
e_AnmKey_Null + 1,	// Il faut juste que ce ne soit pas la même clef que gAnm_POW_Idle.
3, e_Spr_Pow_TotemFree,
3, e_Spr_Pow_TotemFree + 1,
3, e_Spr_Pow_TotemFree + 2,
e_Anm_Goto, (u64)gAnm_POW_Idle
};
// POW - Attente sur totem.
u64	gAnm_POW_TotemWait[] =
{
e_AnmKey_Null,
8, e_Spr_Pow_TotemWait + 2,
8, e_Spr_Pow_TotemWait + 1,
8, e_Spr_Pow_TotemWait,
8, e_Spr_Pow_TotemWait + 1,
e_Anm_RndGoto, 64, (u64)gAnm_POW_TotemAlt, (u64)gAnm_POW_TotemWait
};
// POW - Attente sur totem, anim alternative.
u64	gAnm_POW_TotemAlt[] =
{
e_AnmKey_Null,
8, e_Spr_Pow_TotemAlt,
8, e_Spr_Pow_TotemAlt + 1,
8, e_Spr_Pow_TotemAlt + 2,
8, e_Spr_Pow_TotemAlt + 3,
8, e_Spr_Pow_TotemAlt + 4,
8, e_Spr_Pow_TotemAlt + 3,
8, e_Spr_Pow_TotemAlt + 4,
8, e_Spr_Pow_TotemAlt + 3,
8, e_Spr_Pow_TotemAlt + 2,
8, e_Spr_Pow_TotemAlt + 1,
8, e_Spr_Pow_TotemAlt,
e_Anm_Goto, (u64)gAnm_POW_TotemWait
};
// POW totem - poteau disparait - Dust.
u64	gAnm_POW_TotemDisappear_Dust[] =
{
e_AnmKey_Null,
9, e_Spr_Pow_TotemDust,			// Durée de gAnm_POW_TotemFree.
3, e_Spr_Pow_TotemDust + 1,
3, e_Spr_Pow_TotemDust + 2,
3, e_Spr_Pow_TotemDust + 3,
3, e_Spr_Pow_TotemDust + 4,
3, e_Spr_Pow_TotemDust + 5,
3, e_Spr_Pow_TotemDust + 6,
e_Anm_Kill
};
// POW - Pendu par les mains.
u64	gAnm_POW_HangingWait[] =
{
e_AnmKey_Null,
8, e_Spr_Pow_HangingWait,
8, e_Spr_Pow_HangingWait + 1,
8, e_Spr_Pow_HangingWait + 2,
8, e_Spr_Pow_HangingWait + 3,
8, e_Spr_Pow_HangingWait + 4,
8, e_Spr_Pow_HangingWait + 3,
8, e_Spr_Pow_HangingWait + 2,
8, e_Spr_Pow_HangingWait + 1,
e_Anm_Goto, (u64)gAnm_POW_HangingWait
};
// POW - Hanging - Disparition du bout de la corde - Dust.
u64	gAnm_POW_Hanging_RopeEndDisappear_Dust[] =
{
e_AnmKey_Null,
3, e_Spr_Pow_HangingRopeEnd_Dust,
3, e_Spr_Pow_HangingRopeEnd_Dust + 1,
3, e_Spr_Pow_HangingRopeEnd_Dust + 2,
3, e_Spr_Pow_HangingRopeEnd_Dust + 3,
3, e_Spr_Pow_HangingRopeEnd_Dust + 4,
3, e_Spr_Pow_HangingRopeEnd_Dust + 5,
3, e_Spr_Pow_HangingRopeEnd_Dust + 6,
e_Anm_Kill
};
// POW - Gros caleçon qui donne un bonus.
u64	gAnm_POW_BigPants[] =
{
e_AnmKey_Null + 1,	// Il faut juste que ce ne soit pas la même clef que gAnm_POW_Idle.
6, e_Spr_Pow_Idle,
6, e_Spr_Pow_BigPants,
6, e_Spr_Pow_BigPants + 1,
6, e_Spr_Pow_BigPants + 2,
6, e_Spr_Pow_BigPants + 3,
6, e_Spr_Pow_BigPants + 4,
6, e_Spr_Pow_BigPants + 5,
6, e_Spr_Pow_BigPants + 6,
6, e_Spr_Pow_BigPants + 7,
6, e_Spr_Pow_BigPants + 8,
e_Anm_StepFlag,					// Pour larguer le bonus.
30, e_Spr_Pow_BigPants + 9,
e_Anm_Goto, (u64)gAnm_POW_Idle
};
// POW - Salut et disparition - Dust.
u64	gAnm_POW_SaluteDisappear_Dust[] =
{
e_AnmKey_Null,
3, e_Spr_Pow_Idle,
3, e_Spr_Pow_Salute,
3, e_Spr_Pow_Salute + 1,
3, e_Spr_Pow_Salute + 2,
3, e_Spr_Pow_Salute + 3,
3, e_Spr_Pow_Salute + 4,
3, e_Spr_Pow_Salute + 5,
3, e_Spr_Pow_Salute + 6,
3, e_Spr_Pow_Salute + 7,
e_Anm_Sfx, e_Sfx_Fx_ThankYou, e_SfxPrio_0 + 5,
3, e_Spr_Pow_Salute + 8,
3, e_Spr_Pow_Salute + 9,
30, e_Spr_Pow_Salute + 10,
ANIM_CLIGNO(e_Spr_Pow_Salute + 10)
e_Anm_Kill
};
// POW - Salut.
u64	gAnm_POW_Salute[] =
{
e_AnmKey_Null + 1,	// Il faut juste que ce ne soit pas la même clef que gAnm_POW_Idle.
3, e_Spr_Pow_Idle,
3, e_Spr_Pow_Salute,
3, e_Spr_Pow_Salute + 1,
3, e_Spr_Pow_Salute + 2,
3, e_Spr_Pow_Salute + 3,
3, e_Spr_Pow_Salute + 4,
3, e_Spr_Pow_Salute + 5,
3, e_Spr_Pow_Salute + 6,
3, e_Spr_Pow_Salute + 7,
e_Anm_Sfx, e_Sfx_Fx_ThankYou, e_SfxPrio_0 + 5,
3, e_Spr_Pow_Salute + 8,
3, e_Spr_Pow_Salute + 9,
30, e_Spr_Pow_Salute + 10,
e_Anm_Goto, (u64)gAnm_POW_Idle
};
// POW - Chute (depuis une marche).
u64	gAnm_POW_Fall[] =
{
e_AnmKey_Null,
3, e_Spr_Pow_Fall,
3, e_Spr_Pow_Fall + 1,
3, e_Spr_Pow_Fall + 2,
3, e_Spr_Pow_Fall + 3,
3, e_Spr_Pow_Fall + 4,
3, e_Spr_Pow_Fall + 3,
3, e_Spr_Pow_Fall + 2,
3, e_Spr_Pow_Fall + 1,
e_Anm_Goto, (u64)gAnm_POW_Fall
};
// POW - Court.
u64	gAnm_POW_Run[] =
{
e_AnmKey_Null,
3, e_Spr_Pow_Run,
3, e_Spr_Pow_Run + 1,
3, e_Spr_Pow_Run + 2,
3, e_Spr_Pow_Run + 3,
3, e_Spr_Pow_Run + 4,
3, e_Spr_Pow_Run + 5,
3, e_Spr_Pow_Run + 6,
3, e_Spr_Pow_Run + 7,
e_Anm_Goto, (u64)gAnm_POW_Run
};

// Aikawa Rumi - Marche. Avance de 2 pixels à chaque nouvelle image, et 3 si FlagStep.
#define	RUMI_T0	5
u64	gAnm_AikawaRumi_Walk[] =
{
e_AnmKey_Null,
RUMI_T0, e_Spr_AikawaRumi_Walk,
RUMI_T0, e_Spr_AikawaRumi_Walk + 1,
RUMI_T0, e_Spr_AikawaRumi_Walk + 2,
e_Anm_StepFlag,
RUMI_T0, e_Spr_AikawaRumi_Walk + 3,
RUMI_T0, e_Spr_AikawaRumi_Walk + 4,
e_Anm_StepFlag,
RUMI_T0, e_Spr_AikawaRumi_Walk + 5,
RUMI_T0, e_Spr_AikawaRumi_Walk + 6,
e_Anm_StepFlag,
RUMI_T0, e_Spr_AikawaRumi_Walk + 7,
RUMI_T0, e_Spr_AikawaRumi_Walk + 8,
RUMI_T0, e_Spr_AikawaRumi_Walk + 9,
RUMI_T0, e_Spr_AikawaRumi_Walk + 10,
e_Anm_StepFlag,
RUMI_T0, e_Spr_AikawaRumi_Walk + 11,
RUMI_T0, e_Spr_AikawaRumi_Walk + 12,
e_Anm_StepFlag,
RUMI_T0, e_Spr_AikawaRumi_Walk + 13,
RUMI_T0, e_Spr_AikawaRumi_Walk + 14,
e_Anm_StepFlag,
RUMI_T0, e_Spr_AikawaRumi_Walk + 15,
e_Anm_Goto, (u64)gAnm_AikawaRumi_Walk
};
// Aikawa Rumi - Tombe, donne l'item, salue.
#define	RUMI_T1	5
u64	gAnm_AikawaRumi_GiveItem[] =
{
e_AnmKey_Enemy_Key3,
RUMI_T1-2, e_Spr_AikawaRumi_GiveItem,
RUMI_T1-2, e_Spr_AikawaRumi_GiveItem + 1,
RUMI_T1-2, e_Spr_AikawaRumi_GiveItem + 2,
RUMI_T1-2, e_Spr_AikawaRumi_GiveItem + 3,
RUMI_T1-2, e_Spr_AikawaRumi_GiveItem + 4,
e_Anm_Sfx, e_Sfx_Fx_Wood, e_SfxPrio_0 + 5,	//e_Sfx_MenuClic2
RUMI_T1+16, e_Spr_AikawaRumi_GiveItem + 5,
RUMI_T1, e_Spr_AikawaRumi_GiveItem + 6,
RUMI_T1, e_Spr_AikawaRumi_GiveItem + 7,
RUMI_T1, e_Spr_AikawaRumi_GiveItem + 8,
RUMI_T1, e_Spr_AikawaRumi_GiveItem + 9,
RUMI_T1, e_Spr_AikawaRumi_GiveItem + 10,
RUMI_T1, e_Spr_AikawaRumi_GiveItem + 11,
RUMI_T1, e_Spr_AikawaRumi_GiveItem + 12,
RUMI_T1, e_Spr_AikawaRumi_GiveItem + 13,
RUMI_T1+16, e_Spr_AikawaRumi_GiveItem + 14,
e_Anm_Goto, (u64)gAnm_AikawaRumi_Flee
};
// Aikawa Rumi - Fuite.
#define	RUMI_T2	3
u64	gAnm_AikawaRumi_Flee[] =
{
e_AnmKey_Null,
RUMI_T2, e_Spr_AikawaRumi_Flee,
RUMI_T2, e_Spr_AikawaRumi_Flee + 1,
RUMI_T2, e_Spr_AikawaRumi_Flee + 2,
RUMI_T2, e_Spr_AikawaRumi_Flee + 3,
RUMI_T2, e_Spr_AikawaRumi_Flee + 4,
RUMI_T2, e_Spr_AikawaRumi_Flee + 5,
RUMI_T2, e_Spr_AikawaRumi_Flee + 6,
RUMI_T2, e_Spr_AikawaRumi_Flee + 7,
RUMI_T2, e_Spr_AikawaRumi_Flee + 8,
e_Anm_Jump, (u64)-16
};

// R-Shobu - Flying (inutilisée, juste pour sortie du flip).
u64	gAnm_RShobu_Flying[] =
{
e_AnmKey_Null,
128, e_Spr_RShobu_Flying + 3,	// Position médiane.
e_Anm_Jump, (u64)-2
};
// R-Shobu - Flip.
u64	gAnm_RShobu_Flip[] =
{
e_AnmKey_Enemy_Flip,
3, e_Spr_RShobu_Flip,
3, e_Spr_RShobu_Flip + 1,
3, e_Spr_RShobu_Flip + 2,
3, e_Spr_RShobu_Flip + 3,
3, e_Spr_RShobu_Flip + 4,
3, e_Spr_RShobu_Flip + 5,
3, e_Spr_RShobu_Flip + 6,
e_Anm_StepFlag,					// Signal pour placer le bit de flip.
3, e_Spr_RShobu_Flip + 6,
3, e_Spr_RShobu_Flip + 5,
3, e_Spr_RShobu_Flip + 4,
3, e_Spr_RShobu_Flip + 3,
3, e_Spr_RShobu_Flip + 2,
3, e_Spr_RShobu_Flip + 1,
3, e_Spr_RShobu_Flip,
e_Anm_Goto, (u64)gAnm_RShobu_Flying
};
// Tir ennemi - RShobu > Bombe.
u64	gAnm_ShotEnemy_RShobu_Bomb[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Shot_BombDrop, e_SfxPrio_Enemy_Shot,
2, e_Spr_Shot_RShobu_Bomb,
2, e_Spr_Shot_RShobu_Bomb + 1,
2, e_Spr_Shot_RShobu_Bomb + 2,
2, e_Spr_Shot_RShobu_Bomb + 3,
2, e_Spr_Shot_RShobu_Bomb + 4,
2, e_Spr_Shot_RShobu_Bomb + 5,
2, e_Spr_Shot_RShobu_Bomb + 6,
2, e_Spr_Shot_RShobu_Bomb + 5,
2, e_Spr_Shot_RShobu_Bomb + 4,
2, e_Spr_Shot_RShobu_Bomb + 3,
2, e_Spr_Shot_RShobu_Bomb + 2,
2, e_Spr_Shot_RShobu_Bomb + 1,
2, e_Spr_Shot_RShobu_Bomb + 2,
2, e_Spr_Shot_RShobu_Bomb + 3,
2, e_Spr_Shot_RShobu_Bomb + 4,
2, e_Spr_Shot_RShobu_Bomb + 5,
2, e_Spr_Shot_RShobu_Bomb + 4,
2, e_Spr_Shot_RShobu_Bomb + 3,
2, e_Spr_Shot_RShobu_Bomb + 2,
2, e_Spr_Shot_RShobu_Bomb + 3,
2, e_Spr_Shot_RShobu_Bomb + 4,
30, e_Spr_Shot_RShobu_Bomb + 3,
e_Anm_Jump, (u64)-2
};
// RShobu - Dust quand on lache uine bombe - Dust.
u64	gAnm_RShobu_BombDrop_Dust[] =
{
e_AnmKey_Null,
3, e_Spr_RShobu_BombDrop_Dust,
3, e_Spr_RShobu_BombDrop_Dust + 1,
3, e_Spr_RShobu_BombDrop_Dust + 2,
3, e_Spr_RShobu_BombDrop_Dust + 3,
3, e_Spr_RShobu_BombDrop_Dust + 4,
3, e_Spr_RShobu_BombDrop_Dust + 5,
3, e_Spr_RShobu_BombDrop_Dust + 6,
3, e_Spr_RShobu_BombDrop_Dust + 7,
e_Anm_Kill
};

// Masknell - Rotor.
u64	gAnm_Masknell_Blades[] =
{
e_AnmKey_Null,
2, 0,
2, (5 * 8),
2, (1 * 8),
2, (5 * 8),
2, (2 * 8),
2, (5 * 8),
2, (3 * 8),
2, (5 * 8),
2, (4 * 8),
2, (5 * 8),
e_Anm_Goto, (u64)gAnm_Masknell_Blades
};
// Masknell - Fx tir.
u64	gAnm_Masknell_ShotFx[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Enemy_Shot,
e_Anm_StepFlag,					// Signal pour tirer.
4, 0,
4, (1 * 8),
e_Anm_End
};

// Zombie Teen - Idle.
u64	gAnm_ZombieTeen_Idle[] =
{
e_AnmKey_Null,
8, e_Spr_ZombieTeen_Idle,
8, e_Spr_ZombieTeen_Idle + 1,
8, e_Spr_ZombieTeen_Idle + 2,
8, e_Spr_ZombieTeen_Idle + 3,
8, e_Spr_ZombieTeen_Idle + 4,
8, e_Spr_ZombieTeen_Idle + 5,
8, e_Spr_ZombieTeen_Idle + 6,
8, e_Spr_ZombieTeen_Idle + 5,
8, e_Spr_ZombieTeen_Idle + 4,
8, e_Spr_ZombieTeen_Idle + 3,
8, e_Spr_ZombieTeen_Idle + 2,
8, e_Spr_ZombieTeen_Idle + 1,
e_Anm_Jump, (u64)-24
};
// Zombie Teen - Walk.
#define	ZMB_TEEN_WLK	4
u64	gAnm_ZombieTeen_Walk[] =
{
e_AnmKey_Null,
//ZMB_TEEN_WLK+3, e_Spr_ZombieTeen_Walk,
e_Anm_StepFlag,
ZMB_TEEN_WLK+3, e_Spr_ZombieTeen_Walk + 1,
e_Anm_StepFlag,
ZMB_TEEN_WLK, e_Spr_ZombieTeen_Walk + 2,
e_Anm_StepFlag,
ZMB_TEEN_WLK, e_Spr_ZombieTeen_Walk + 3,
ZMB_TEEN_WLK, e_Spr_ZombieTeen_Walk + 4,
ZMB_TEEN_WLK, e_Spr_ZombieTeen_Walk + 5,
ZMB_TEEN_WLK, e_Spr_ZombieTeen_Walk + 6,
ZMB_TEEN_WLK, e_Spr_ZombieTeen_Walk + 7,
ZMB_TEEN_WLK, e_Spr_ZombieTeen_Walk + 8,
ZMB_TEEN_WLK, e_Spr_ZombieTeen_Walk + 9,
ZMB_TEEN_WLK, e_Spr_ZombieTeen_Walk + 10,
ZMB_TEEN_WLK, e_Spr_ZombieTeen_Walk + 11,
ZMB_TEEN_WLK, e_Spr_ZombieTeen_Walk + 12,
ZMB_TEEN_WLK, e_Spr_ZombieTeen_Walk + 13,
ZMB_TEEN_WLK, e_Spr_ZombieTeen_Walk + 14,
ZMB_TEEN_WLK+3, e_Spr_ZombieTeen_Walk + 15,
ZMB_TEEN_WLK+3, e_Spr_ZombieTeen_Walk,
e_Anm_Jump, (u64)-32
};
// Zombie Teen - Fall.
u64	gAnm_ZombieTeen_Fall[] =
{
ANMPRIO(1)+1,
3, e_Spr_ZombieTeen_Fall,
3, e_Spr_ZombieTeen_Fall + 1,
3, e_Spr_ZombieTeen_Fall + 2,
3, e_Spr_ZombieTeen_Fall + 3,
3, e_Spr_ZombieTeen_Fall + 4,
3, e_Spr_ZombieTeen_Fall + 5,
3, e_Spr_ZombieTeen_Fall + 6,
3, e_Spr_ZombieTeen_Fall + 7,
3, e_Spr_ZombieTeen_Fall + 8,
3, e_Spr_ZombieTeen_Fall + 9,
3, e_Spr_ZombieTeen_Fall + 10,
3, e_Spr_ZombieTeen_Fall + 11,
//
60, e_Spr_ZombieTeen_Fall + 12,
8, e_Spr_ZombieTeen_Rise,
8, e_Spr_ZombieTeen_Rise + 1,
8, e_Spr_ZombieTeen_Rise,
//
60, e_Spr_ZombieTeen_Fall + 12,
8, e_Spr_ZombieTeen_Rise,
8, e_Spr_ZombieTeen_Rise + 1,
8, e_Spr_ZombieTeen_Rise,
//
60, e_Spr_ZombieTeen_Fall + 12,
8, e_Spr_ZombieTeen_Rise,
8, e_Spr_ZombieTeen_Rise + 1,
8, e_Spr_ZombieTeen_Rise,
//
e_Anm_Goto, (u64)gAnm_ZombieTeen_Rise
};
// Zombie Teen - Rise.
u64	gAnm_ZombieTeen_Rise[] =
{
ANMPRIO(2)+2,
6, e_Spr_ZombieTeen_Rise,
6, e_Spr_ZombieTeen_Rise + 1,
6, e_Spr_ZombieTeen_Rise + 2,
6, e_Spr_ZombieTeen_Rise + 3,
6, e_Spr_ZombieTeen_Rise + 4,
6, e_Spr_ZombieTeen_Rise + 5,
6, e_Spr_ZombieTeen_Rise + 6,
6, e_Spr_ZombieTeen_Rise + 7,
6, e_Spr_ZombieTeen_Rise + 8,
6, e_Spr_ZombieTeen_Rise + 9,
6, e_Spr_ZombieTeen_Rise + 10,
6, e_Spr_ZombieTeen_Rise + 11,
e_Anm_Goto, (u64)gAnm_ZombieTeen_Idle
};
// Zombie Teen - Flip.
u64	gAnm_ZombieTeen_Flip2[] =	// Fin du flip avec même sprite mais clef Null.
{
e_AnmKey_Null,
1, e_Spr_ZombieTeen_Idle + 1,
e_Anm_Goto, (u64)gAnm_ZombieTeen_Idle
};
u64	gAnm_ZombieTeen_Flip[] =
{
e_AnmKey_Enemy_Flip,
5, e_Spr_ZombieTeen_Flip,
5, e_Spr_ZombieTeen_Flip + 1,
5, e_Spr_ZombieTeen_Flip + 2,
5, e_Spr_ZombieTeen_Flip + 3,
5, e_Spr_ZombieTeen_Flip + 4,
e_Anm_StepFlag,					// Signal pour placer le bit de flip.
5, (e_Spr_ZombieTeen_Flip + 5) | SPR_Flip_X,
5, (e_Spr_ZombieTeen_Flip + 6) | SPR_Flip_X,
5, (e_Spr_ZombieTeen_Flip + 7) | SPR_Flip_X,
5, (e_Spr_ZombieTeen_Flip + 8) | SPR_Flip_X,
4, e_Spr_ZombieTeen_Idle + 1,
//e_Anm_Goto, (u64)gAnm_ZombieTeen_Idle
e_Anm_Goto, (u64)gAnm_ZombieTeen_Flip2
};
// Zombie Teen - Tir.
u64	gAnm_ZombieTeen_Shot[] =
{
e_AnmKey_Enemy_Shot,
4, e_Spr_ZombieTeen_Shot,
4, e_Spr_ZombieTeen_Shot + 1,
4, e_Spr_ZombieTeen_Shot + 2,
4, e_Spr_ZombieTeen_Shot + 3,
4, e_Spr_ZombieTeen_Shot + 4,
4, e_Spr_ZombieTeen_Shot + 5,
4, e_Spr_ZombieTeen_Shot + 6,
4, e_Spr_ZombieTeen_Shot + 7,
4, e_Spr_ZombieTeen_Shot + 8,
e_Anm_Sfx, e_Sfx_Shot_Zombie, e_SfxPrio_Enemy_Shot,
18, e_Spr_ZombieTeen_Shot + 9,		// Durée du dust de fx qui va avec (à peu près).
4, e_Spr_ZombieTeen_Shot + 10,
4, e_Spr_ZombieTeen_Shot + 11,
4, e_Spr_ZombieTeen_Shot + 12,
4, e_Spr_ZombieTeen_Shot + 13,
4, e_Spr_ZombieTeen_Shot + 14,
4, e_Spr_ZombieTeen_Shot + 15,
4, e_Spr_ZombieTeen_Shot + 16,
4, e_Spr_ZombieTeen_Shot + 17,
4, e_Spr_ZombieTeen_Shot + 18,
4, e_Spr_ZombieTeen_Shot + 19,
e_Anm_Goto, (u64)gAnm_ZombieTeen_Idle
};
// Zombie Teen - Tir - Dust d'effet spécial.
u64	gAnm_ZombieTeen_ShotFX_Dust[] =
{
e_AnmKey_Null,
2, e_Spr_ZombieTeen_ShotFX_Dust,
2, e_Spr_ZombieTeen_ShotFX_Dust + 1,
2, e_Spr_ZombieTeen_ShotFX_Dust + 2,
2, e_Spr_ZombieTeen_ShotFX_Dust + 3,
2, e_Spr_ZombieTeen_ShotFX_Dust + 4,
2, e_Spr_ZombieTeen_ShotFX_Dust + 5,
2, e_Spr_ZombieTeen_ShotFX_Dust + 6,
2, e_Spr_ZombieTeen_ShotFX_Dust + 7,
2, e_Spr_ZombieTeen_ShotFX_Dust + 8,
2, e_Spr_ZombieTeen_ShotFX_Dust + 9,
2, e_Spr_ZombieTeen_ShotFX_Dust + 10,
2, e_Spr_ZombieTeen_ShotFX_Dust + 11,
e_Anm_Kill
};
// Giclée de sang - Dust d'effet spécial.
u64	gAnm_Blood0_FX_Dust[] =
{
e_AnmKey_Null,
2, e_Spr_Blood_FX0_Dust,
2, e_Spr_Blood_FX0_Dust + 1,
2, e_Spr_Blood_FX0_Dust + 2,
2, e_Spr_Blood_FX0_Dust + 3,
2, e_Spr_Blood_FX0_Dust + 4,
2, e_Spr_Blood_FX0_Dust + 5,
2, e_Spr_Blood_FX0_Dust + 6,
2, e_Spr_Blood_FX0_Dust + 7,
e_Anm_Kill
};
// Zombies - Mort générique au shotgun - Dust.
u64	gAnm_Zombie_DeathShotgunGen_Dust[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Death_Zombie, e_SfxPrio_Enemy_Death,
3, e_Spr_Zombie_DeathShotgunGen,      3, e_Spr_Zombie_DeathShotgunGen + 1,  3, e_Spr_Zombie_DeathShotgunGen + 2,
3, e_Spr_Zombie_DeathShotgunGen + 3,  3, e_Spr_Zombie_DeathShotgunGen + 4,  3, e_Spr_Zombie_DeathShotgunGen + 5,
3, e_Spr_Zombie_DeathShotgunGen + 6,  3, e_Spr_Zombie_DeathShotgunGen + 7,  3, e_Spr_Zombie_DeathShotgunGen + 8,
3, e_Spr_Zombie_DeathShotgunGen + 9,
3, e_Spr_Zombie_DeathShotgunGen + 10, 3, e_Spr_Zombie_DeathShotgunGen + 11, 3, e_Spr_Zombie_DeathShotgunGen + 12,
3, e_Spr_Zombie_DeathShotgunGen + 13, 3, e_Spr_Zombie_DeathShotgunGen + 14,
e_Anm_Kill
};
// Zombie Teen - Marche en brûlant.
u64	gAnm_ZombieTeen_DeathBurnWalk[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Death_Zombie, e_SfxPrio_Enemy_Death,
4, e_Spr_ZombieTeen_DeathBurnWalk,      4, e_Spr_ZombieTeen_DeathBurnWalk + 1,  4, e_Spr_ZombieTeen_DeathBurnWalk + 2,
4, e_Spr_ZombieTeen_DeathBurnWalk + 3,  4, e_Spr_ZombieTeen_DeathBurnWalk + 4,  4, e_Spr_ZombieTeen_DeathBurnWalk + 5,
4, e_Spr_ZombieTeen_DeathBurnWalk + 6,  4, e_Spr_ZombieTeen_DeathBurnWalk + 7,  4, e_Spr_ZombieTeen_DeathBurnWalk + 8,
4, e_Spr_ZombieTeen_DeathBurnWalk + 9,
4, e_Spr_ZombieTeen_DeathBurnWalk + 10, 4, e_Spr_ZombieTeen_DeathBurnWalk + 11, 4, e_Spr_ZombieTeen_DeathBurnWalk + 12,
4, e_Spr_ZombieTeen_DeathBurnWalk + 13, 4, e_Spr_ZombieTeen_DeathBurnWalk + 14, 4, e_Spr_ZombieTeen_DeathBurnWalk + 15,
4, e_Spr_ZombieTeen_DeathBurnWalk + 16, 4, e_Spr_ZombieTeen_DeathBurnWalk + 17, 4, e_Spr_ZombieTeen_DeathBurnWalk + 18,
4, e_Spr_ZombieTeen_DeathBurnWalk + 19,
4, e_Spr_ZombieTeen_DeathBurnWalk + 20, 4, e_Spr_ZombieTeen_DeathBurnWalk + 21, 4, e_Spr_ZombieTeen_DeathBurnWalk + 22,
4, e_Spr_ZombieTeen_DeathBurnWalk + 23,
e_Anm_StepFlag,					// Signal pour fin d'anim.
e_Anm_Jump, -3
};
// Zombie Teen - Mort brulé - Dust.
u64	gAnm_ZombieTeen_DeathBurn_Dust[] =
{
e_AnmKey_Null,
4, e_Spr_ZombieTeen_DeathBurn_Dust,      4, e_Spr_ZombieTeen_DeathBurn_Dust + 1,  4, e_Spr_ZombieTeen_DeathBurn_Dust + 2,
4, e_Spr_ZombieTeen_DeathBurn_Dust + 3,  4, e_Spr_ZombieTeen_DeathBurn_Dust + 4,  4, e_Spr_ZombieTeen_DeathBurn_Dust + 5,
4, e_Spr_ZombieTeen_DeathBurn_Dust + 6,  4, e_Spr_ZombieTeen_DeathBurn_Dust + 7,  4, e_Spr_ZombieTeen_DeathBurn_Dust + 8,
4, e_Spr_ZombieTeen_DeathBurn_Dust + 9,
4, e_Spr_ZombieTeen_DeathBurn_Dust + 10, 4, e_Spr_ZombieTeen_DeathBurn_Dust + 11, 4, e_Spr_ZombieTeen_DeathBurn_Dust + 12,
4, e_Spr_ZombieTeen_DeathBurn_Dust + 13, 4, e_Spr_ZombieTeen_DeathBurn_Dust + 14, 4, e_Spr_ZombieTeen_DeathBurn_Dust + 15,
4, e_Spr_ZombieTeen_DeathBurn_Dust + 16,
e_Anm_Kill
};
// Tir ennemi - Zombie Brain > cerveau.
u64	gAnm_ShotEnemy_ZombieBrain[] =
{
e_AnmKey_Null,
2, (e_Spr_Shot_ZombieTeen_Bullet) | SPR_Flip_X,
2, (e_Spr_Shot_ZombieTeen_Bullet + 1) | SPR_Flip_X,
2, (e_Spr_Shot_ZombieTeen_Bullet + 2) | SPR_Flip_X,
2, (e_Spr_Shot_ZombieTeen_Bullet + 3) | SPR_Flip_X,
2, (e_Spr_Shot_ZombieTeen_Bullet + 4) | SPR_Flip_X,
2, (e_Spr_Shot_ZombieTeen_Bullet + 5) | SPR_Flip_X,
2, (e_Spr_Shot_ZombieTeen_Bullet + 6) | SPR_Flip_X,
2, (e_Spr_Shot_ZombieTeen_Bullet + 7) | SPR_Flip_X,
2, (e_Spr_Shot_ZombieTeen_Bullet + 8) | SPR_Flip_X,
2, (e_Spr_Shot_ZombieTeen_Bullet + 9) | SPR_Flip_X,
2, (e_Spr_Shot_ZombieTeen_Bullet + 10) | SPR_Flip_X,
2, (e_Spr_Shot_ZombieTeen_Bullet + 11) | SPR_Flip_X,
30, (e_Spr_Shot_ZombieTeen_Bullet + 12) | SPR_Flip_X,
e_Anm_Jump, (u64)-2
};
// Tir ennemi - Zombie Brain > cerveau - Dust touché en l'air.
u64	gAnm_ShotEnemy_ZombieBrain_DustAir[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Fx_ZombieAcid, e_SfxPrio_Enemy_Shot+1,
3, e_Spr_Shot_ZombieTeen_DustAir,      3, e_Spr_Shot_ZombieTeen_DustAir + 1,  3, e_Spr_Shot_ZombieTeen_DustAir + 2,
3, e_Spr_Shot_ZombieTeen_DustAir + 3,  3, e_Spr_Shot_ZombieTeen_DustAir + 4,  3, e_Spr_Shot_ZombieTeen_DustAir + 5,
3, e_Spr_Shot_ZombieTeen_DustAir + 6,  3, e_Spr_Shot_ZombieTeen_DustAir + 7,  3, e_Spr_Shot_ZombieTeen_DustAir + 8,
3, e_Spr_Shot_ZombieTeen_DustAir + 9,
3, e_Spr_Shot_ZombieTeen_DustAir + 10, 3, e_Spr_Shot_ZombieTeen_DustAir + 11, 3, e_Spr_Shot_ZombieTeen_DustAir + 12,
3, e_Spr_Shot_ZombieTeen_DustAir + 13, 3, e_Spr_Shot_ZombieTeen_DustAir + 14,
3, e_Spr_Shot_ZombieTeen_DustAir + 15,
e_Anm_Kill
};
// Tir ennemi - Zombie Brain > cerveau - Dust touché au sol.
u64	gAnm_ShotEnemy_ZombieBrain_DustGnd[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Fx_ZombieAcid, e_SfxPrio_Enemy_Shot+1,
3, e_Spr_Shot_ZombieTeen_DustGnd,      3, e_Spr_Shot_ZombieTeen_DustGnd + 1,  3, e_Spr_Shot_ZombieTeen_DustGnd + 2,
3, e_Spr_Shot_ZombieTeen_DustGnd + 3,  3, e_Spr_Shot_ZombieTeen_DustGnd + 4,  3, e_Spr_Shot_ZombieTeen_DustGnd + 5,
3, e_Spr_Shot_ZombieTeen_DustGnd + 6,  3, e_Spr_Shot_ZombieTeen_DustGnd + 7,  3, e_Spr_Shot_ZombieTeen_DustGnd + 8,
3, e_Spr_Shot_ZombieTeen_DustGnd + 9,
3, e_Spr_Shot_ZombieTeen_DustGnd + 10, 3, e_Spr_Shot_ZombieTeen_DustGnd + 11, 3, e_Spr_Shot_ZombieTeen_DustGnd + 12,
3, e_Spr_Shot_ZombieTeen_DustGnd + 13, 3, e_Spr_Shot_ZombieTeen_DustGnd + 14, 3, e_Spr_Shot_ZombieTeen_DustGnd + 15,
3, e_Spr_Shot_ZombieTeen_DustGnd + 16, 3, e_Spr_Shot_ZombieTeen_DustGnd + 17,
3, e_Spr_Shot_ZombieTeen_DustGnd + 18,
e_Anm_Kill
};

// Zombie Fat - Idle.
u64	gAnm_ZombieFat_Idle[] =
{
e_AnmKey_Null,
8, e_Spr_ZombieFat_Idle,      8, e_Spr_ZombieFat_Idle + 1,  8, e_Spr_ZombieFat_Idle + 2,
8, e_Spr_ZombieFat_Idle + 3,  8, e_Spr_ZombieFat_Idle + 4,  8, e_Spr_ZombieFat_Idle + 5,
8, e_Spr_ZombieFat_Idle + 6,
8, e_Spr_ZombieFat_Idle + 5,
8, e_Spr_ZombieFat_Idle + 4,  8, e_Spr_ZombieFat_Idle + 3,  8, e_Spr_ZombieFat_Idle + 2,
8, e_Spr_ZombieFat_Idle + 1,
e_Anm_Jump, (u64)-24
};
// Zombie Fat - Walk.
#define	ZMB_FAT_WLK	5
u64	gAnm_ZombieFat_Walk[] =
{
e_AnmKey_Null,
ZMB_FAT_WLK, e_Spr_ZombieFat_Walk,
ZMB_FAT_WLK, e_Spr_ZombieFat_Walk + 1,
ZMB_FAT_WLK, e_Spr_ZombieFat_Walk + 2,
ZMB_FAT_WLK, e_Spr_ZombieFat_Walk + 3,
ZMB_FAT_WLK, e_Spr_ZombieFat_Walk + 4,
ZMB_FAT_WLK, e_Spr_ZombieFat_Walk + 5,
ZMB_FAT_WLK, e_Spr_ZombieFat_Walk + 6,
ZMB_FAT_WLK, e_Spr_ZombieFat_Walk + 7,
ZMB_FAT_WLK, e_Spr_ZombieFat_Walk + 8,
ZMB_FAT_WLK, e_Spr_ZombieFat_Walk + 9,
ZMB_FAT_WLK, e_Spr_ZombieFat_Walk + 10,
ZMB_FAT_WLK, e_Spr_ZombieFat_Walk + 11,
ZMB_FAT_WLK, e_Spr_ZombieFat_Walk + 12,
ZMB_FAT_WLK, e_Spr_ZombieFat_Walk + 13,
e_Anm_Jump, (u64)-28
};
// Zombie Fat - Fall.
u64	gAnm_ZombieFat_Fall[] =
{
ANMPRIO(1)+1,
3, e_Spr_ZombieFat_Fall,
3, e_Spr_ZombieFat_Fall + 1,
3, e_Spr_ZombieFat_Fall + 2,
3, e_Spr_ZombieFat_Fall + 3,
3, e_Spr_ZombieFat_Fall + 4,
3, e_Spr_ZombieFat_Fall + 5,
3, e_Spr_ZombieFat_Fall + 6,
3, e_Spr_ZombieFat_Fall + 7,
3, e_Spr_ZombieFat_Fall + 8,
3, e_Spr_ZombieFat_Fall + 9,
3, e_Spr_ZombieFat_Fall + 10,
3, e_Spr_ZombieFat_Fall + 11,
//
60, e_Spr_ZombieFat_Fall + 12,
8, e_Spr_ZombieFat_Rise,
8, e_Spr_ZombieFat_Rise + 1,
8, e_Spr_ZombieFat_Rise,
//
60, e_Spr_ZombieFat_Fall + 12,
8, e_Spr_ZombieFat_Rise,
8, e_Spr_ZombieFat_Rise + 1,
8, e_Spr_ZombieFat_Rise,
//
60, e_Spr_ZombieFat_Fall + 12,
8, e_Spr_ZombieFat_Rise,
8, e_Spr_ZombieFat_Rise + 1,
8, e_Spr_ZombieFat_Rise,
//
e_Anm_Goto, (u64)gAnm_ZombieFat_Rise
};
// Zombie Fat - Rise.
u64	gAnm_ZombieFat_Rise[] =
{
ANMPRIO(2)+2,
6, e_Spr_ZombieFat_Rise,
6, e_Spr_ZombieFat_Rise + 1,
6, e_Spr_ZombieFat_Rise + 2,
6, e_Spr_ZombieFat_Rise + 3,
6, e_Spr_ZombieFat_Rise + 4,
6, e_Spr_ZombieFat_Rise + 5,
6, e_Spr_ZombieFat_Rise + 6,
6, e_Spr_ZombieFat_Rise + 7,
6, e_Spr_ZombieFat_Rise + 8,
6, e_Spr_ZombieFat_Rise + 9,
6, e_Spr_ZombieFat_Rise + 10,
6, e_Spr_ZombieFat_Rise + 11,
e_Anm_Goto, (u64)gAnm_ZombieFat_Idle
};
// Zombie Fat - Flip.
u64	gAnm_ZombieFat_Flip2[] =	// Fin du flip avec même sprite mais clef Null.
{
e_AnmKey_Null,
1, (e_Spr_ZombieFat_Flip + 8) | SPR_Flip_X,
e_Anm_Goto, (u64)gAnm_ZombieFat_Idle
};
u64	gAnm_ZombieFat_Flip[] =
{
e_AnmKey_Enemy_Flip,
5, e_Spr_ZombieFat_Flip,
5, e_Spr_ZombieFat_Flip + 1,
5, e_Spr_ZombieFat_Flip + 2,
5, e_Spr_ZombieFat_Flip + 3,
e_Anm_StepFlag,					// Signal pour placer le bit de flip.
5, (e_Spr_ZombieFat_Flip + 4) | SPR_Flip_X,
5, (e_Spr_ZombieFat_Flip + 5) | SPR_Flip_X,
5, (e_Spr_ZombieFat_Flip + 6) | SPR_Flip_X,
5, (e_Spr_ZombieFat_Flip + 7) | SPR_Flip_X,
4, (e_Spr_ZombieFat_Flip + 8) | SPR_Flip_X,
//e_Anm_Goto, (u64)gAnm_ZombieFat_Idle
e_Anm_Goto, (u64)gAnm_ZombieFat_Flip2
};
// Zombie Fat - Tir.
u64	gAnm_ZombieFat_Shot[] =
{
e_AnmKey_Enemy_Shot,
4, e_Spr_ZombieFat_Shot,
4, e_Spr_ZombieFat_Shot + 1,
4, e_Spr_ZombieFat_Shot + 2,
4, e_Spr_ZombieFat_Shot + 3,
4, e_Spr_ZombieFat_Shot + 4,
4, e_Spr_ZombieFat_Shot + 5,
4, e_Spr_ZombieFat_Shot + 6,
4, e_Spr_ZombieFat_Shot + 7,
4, e_Spr_ZombieFat_Shot + 8,
4, e_Spr_ZombieFat_Shot + 9,
e_Anm_Sfx, e_Sfx_Shot_Zombie, e_SfxPrio_Enemy_Shot,
18, e_Spr_ZombieFat_Shot + 10,			// Durée du dust de fx qui va avec (à peu près).
4, e_Spr_ZombieFat_Shot + 11,
7, e_Spr_ZombieFat_Shot + 12,
7, e_Spr_ZombieFat_Shot + 13,
e_Anm_Goto, (u64)gAnm_ZombieFat_Idle
};
// Zombie Fat - Tir - Dust d'effet spécial.
u64	gAnm_ZombieFat_ShotFX_Dust[] =
{
e_AnmKey_Null,
2, e_Spr_ZombieFat_ShotFX_Dust,
2, e_Spr_ZombieFat_ShotFX_Dust + 1,
2, e_Spr_ZombieFat_ShotFX_Dust + 2,
2, e_Spr_ZombieFat_ShotFX_Dust + 3,
2, e_Spr_ZombieFat_ShotFX_Dust + 4,
2, e_Spr_ZombieFat_ShotFX_Dust + 5,
2, e_Spr_ZombieFat_ShotFX_Dust + 6,
2, e_Spr_ZombieFat_ShotFX_Dust + 7,
2, e_Spr_ZombieFat_ShotFX_Dust + 8,
2, e_Spr_ZombieFat_ShotFX_Dust + 9,
2, e_Spr_ZombieFat_ShotFX_Dust + 10,
2, e_Spr_ZombieFat_ShotFX_Dust + 11,
e_Anm_Kill
};
// Zombie Fat - Marche en brûlant.
u64	gAnm_ZombieFat_DeathBurnWalk[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Death_Zombie, e_SfxPrio_Enemy_Death,
4, e_Spr_ZombieFat_DeathBurnWalk,      4, e_Spr_ZombieFat_DeathBurnWalk + 1,  4, e_Spr_ZombieFat_DeathBurnWalk + 2,
4, e_Spr_ZombieFat_DeathBurnWalk + 3,  4, e_Spr_ZombieFat_DeathBurnWalk + 4,  4, e_Spr_ZombieFat_DeathBurnWalk + 5,
4, e_Spr_ZombieFat_DeathBurnWalk + 6,  4, e_Spr_ZombieFat_DeathBurnWalk + 7,  4, e_Spr_ZombieFat_DeathBurnWalk + 8,
4, e_Spr_ZombieFat_DeathBurnWalk + 9,
4, e_Spr_ZombieFat_DeathBurnWalk + 10, 4, e_Spr_ZombieFat_DeathBurnWalk + 11, 4, e_Spr_ZombieFat_DeathBurnWalk + 12,
4, e_Spr_ZombieFat_DeathBurnWalk + 13, 4, e_Spr_ZombieFat_DeathBurnWalk + 14, 4, e_Spr_ZombieFat_DeathBurnWalk + 15,
4, e_Spr_ZombieFat_DeathBurnWalk + 16, 4, e_Spr_ZombieFat_DeathBurnWalk + 17, 4, e_Spr_ZombieFat_DeathBurnWalk + 18,
4, e_Spr_ZombieFat_DeathBurnWalk + 19,
4, e_Spr_ZombieFat_DeathBurnWalk + 20, 4, e_Spr_ZombieFat_DeathBurnWalk + 21, 4, e_Spr_ZombieFat_DeathBurnWalk + 22,
4, e_Spr_ZombieFat_DeathBurnWalk + 23,
e_Anm_StepFlag,					// Signal pour fin d'anim.
e_Anm_Jump, -3
};
// Zombie Fat - Mort brulé - Dust.
u64	gAnm_ZombieFat_DeathBurn_Dust[] =
{
e_AnmKey_Null,
4, e_Spr_ZombieFat_DeathBurn_Dust,      4, e_Spr_ZombieFat_DeathBurn_Dust + 1,  4, e_Spr_ZombieFat_DeathBurn_Dust + 2,
4, e_Spr_ZombieFat_DeathBurn_Dust + 3,  4, e_Spr_ZombieFat_DeathBurn_Dust + 4,  4, e_Spr_ZombieFat_DeathBurn_Dust + 5,
4, e_Spr_ZombieFat_DeathBurn_Dust + 6,  4, e_Spr_ZombieFat_DeathBurn_Dust + 7,  4, e_Spr_ZombieFat_DeathBurn_Dust + 8,
4, e_Spr_ZombieFat_DeathBurn_Dust + 9,
4, e_Spr_ZombieFat_DeathBurn_Dust + 10, 4, e_Spr_ZombieFat_DeathBurn_Dust + 11, 4, e_Spr_ZombieFat_DeathBurn_Dust + 12,
4, e_Spr_ZombieFat_DeathBurn_Dust + 13, 4, e_Spr_ZombieFat_DeathBurn_Dust + 14, 4, e_Spr_ZombieFat_DeathBurn_Dust + 15,
4, e_Spr_ZombieFat_DeathBurn_Dust + 16,
e_Anm_Kill
};

// Zombie Bits - Bras 1 - Dust.
u64	gAnm_ZombieBits_Arm1_Dust[] =
{
e_AnmKey_Null,
3, e_Spr_ZombieBits_Arm1,       3, e_Spr_ZombieBits_Arm1 + 1,   3, e_Spr_ZombieBits_Arm1 + 2,
3, e_Spr_ZombieBits_Arm1 + 3,   3, e_Spr_ZombieBits_Arm1 + 4,   3, e_Spr_ZombieBits_Arm1 + 5,
3, e_Spr_ZombieBits_Arm1 + 6,   3, e_Spr_ZombieBits_Arm1 + 7,   3, e_Spr_ZombieBits_Arm1 + 8,
3, e_Spr_ZombieBits_Arm1 + 9,
3, e_Spr_ZombieBits_Arm1 + 10,  3, e_Spr_ZombieBits_Arm1 + 11,  3, e_Spr_ZombieBits_Arm1 + 12,
3, e_Spr_ZombieBits_Arm1 + 13,  3, e_Spr_ZombieBits_Arm1 + 14,  3, e_Spr_ZombieBits_Arm1 + 15,
e_Anm_Goto, (u64)gAnm_ZombieBits_Arm1_Dust
};
// Zombie Bits - Bras 2 - Dust.
u64	gAnm_ZombieBits_Arm2_Dust[] =
{
e_AnmKey_Null,
3, e_Spr_ZombieBits_Arm2,       3, e_Spr_ZombieBits_Arm2 + 1,   3, e_Spr_ZombieBits_Arm2 + 2,
3, e_Spr_ZombieBits_Arm2 + 3,   3, e_Spr_ZombieBits_Arm2 + 4,   3, e_Spr_ZombieBits_Arm2 + 5,
3, e_Spr_ZombieBits_Arm2 + 6,   3, e_Spr_ZombieBits_Arm2 + 7,   3, e_Spr_ZombieBits_Arm2 + 8,
3, e_Spr_ZombieBits_Arm2 + 9,
3, e_Spr_ZombieBits_Arm2 + 10,  3, e_Spr_ZombieBits_Arm2 + 11,  3, e_Spr_ZombieBits_Arm2 + 12,
3, e_Spr_ZombieBits_Arm2 + 13,  3, e_Spr_ZombieBits_Arm2 + 14,  3, e_Spr_ZombieBits_Arm2 + 15,
e_Anm_Goto, (u64)gAnm_ZombieBits_Arm2_Dust
};
// Zombie Bits - Jambe 1 - Dust.
u64	gAnm_ZombieBits_Leg1_Dust[] =
{
e_AnmKey_Null,
3, e_Spr_ZombieBits_Leg1,       3, e_Spr_ZombieBits_Leg1 + 1,   3, e_Spr_ZombieBits_Leg1 + 2,
3, e_Spr_ZombieBits_Leg1 + 3,   3, e_Spr_ZombieBits_Leg1 + 4,   3, e_Spr_ZombieBits_Leg1 + 5,
3, e_Spr_ZombieBits_Leg1 + 6,   3, e_Spr_ZombieBits_Leg1 + 7,   3, e_Spr_ZombieBits_Leg1 + 8,
3, e_Spr_ZombieBits_Leg1 + 9,
3, e_Spr_ZombieBits_Leg1 + 10,  3, e_Spr_ZombieBits_Leg1 + 11,  3, e_Spr_ZombieBits_Leg1 + 12,
3, e_Spr_ZombieBits_Leg1 + 13,  3, e_Spr_ZombieBits_Leg1 + 14,  3, e_Spr_ZombieBits_Leg1 + 15,
e_Anm_Goto, (u64)gAnm_ZombieBits_Leg1_Dust
};
// Zombie Bits - Jambe 2 - Dust.
u64	gAnm_ZombieBits_Leg2_Dust[] =
{
e_AnmKey_Null,
3, e_Spr_ZombieBits_Leg2,       3, e_Spr_ZombieBits_Leg2 + 1,   3, e_Spr_ZombieBits_Leg2 + 2,
3, e_Spr_ZombieBits_Leg2 + 3,   3, e_Spr_ZombieBits_Leg2 + 4,   3, e_Spr_ZombieBits_Leg2 + 5,
3, e_Spr_ZombieBits_Leg2 + 6,   3, e_Spr_ZombieBits_Leg2 + 7,   3, e_Spr_ZombieBits_Leg2 + 8,
3, e_Spr_ZombieBits_Leg2 + 9,
3, e_Spr_ZombieBits_Leg2 + 10,  3, e_Spr_ZombieBits_Leg2 + 11,  3, e_Spr_ZombieBits_Leg2 + 12,
3, e_Spr_ZombieBits_Leg2 + 13,  3, e_Spr_ZombieBits_Leg2 + 14,  3, e_Spr_ZombieBits_Leg2 + 15,
e_Anm_Goto, (u64)gAnm_ZombieBits_Leg2_Dust
};

// Mort du héros dans l'espace (+ hélico no exit), on affiche rien et ça dure le temps de la grosse explosion.
u64	gAnm_HeroSlugNoExit_DeathGen[] =
{
e_AnmKey_Hero_Death,
28*2, SPR_NoSprite,
e_Anm_End
};
// Explosion générique - Big - Dust.
u64	gAnm_Explosion0_Big_Dust[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Fx_Explosion2, e_SfxPrio_Explosion,
2, e_Spr_Explo0_Big,      2, e_Spr_Explo0_Big + 1,  2, e_Spr_Explo0_Big + 2,  2, e_Spr_Explo0_Big + 3,
2, e_Spr_Explo0_Big + 4,  2, e_Spr_Explo0_Big + 5,  2, e_Spr_Explo0_Big + 6,  2, e_Spr_Explo0_Big + 7,
2, e_Spr_Explo0_Big + 8,  2, e_Spr_Explo0_Big + 9,
2, e_Spr_Explo0_Big + 10, 2, e_Spr_Explo0_Big + 11, 2, e_Spr_Explo0_Big + 12, 2, e_Spr_Explo0_Big + 13,
2, e_Spr_Explo0_Big + 14, 2, e_Spr_Explo0_Big + 15, 2, e_Spr_Explo0_Big + 16, 2, e_Spr_Explo0_Big + 17,
2, e_Spr_Explo0_Big + 18, 2, e_Spr_Explo0_Big + 19,
2, e_Spr_Explo0_Big + 20, 2, e_Spr_Explo0_Big + 21, 2, e_Spr_Explo0_Big + 22, 2, e_Spr_Explo0_Big + 23,
2, e_Spr_Explo0_Big + 24, 2, e_Spr_Explo0_Big + 25, 2, e_Spr_Explo0_Big + 26, 2, e_Spr_Explo0_Big + 27,
e_Anm_Kill
};
// Explosion générique - Moyenne - Dust.
u64	gAnm_Explosion0_Medium_Dust[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Fx_Explosion2, e_SfxPrio_Explosion,
2, e_Spr_Explo0_Medium,      2, e_Spr_Explo0_Medium + 1,  2, e_Spr_Explo0_Medium + 2,  2, e_Spr_Explo0_Medium + 3,
2, e_Spr_Explo0_Medium + 4,  2, e_Spr_Explo0_Medium + 5,  2, e_Spr_Explo0_Medium + 6,  2, e_Spr_Explo0_Medium + 7,
2, e_Spr_Explo0_Medium + 8,  2, e_Spr_Explo0_Medium + 9,
2, e_Spr_Explo0_Medium + 10, 2, e_Spr_Explo0_Medium + 11, 2, e_Spr_Explo0_Medium + 12, 2, e_Spr_Explo0_Medium + 13,
2, e_Spr_Explo0_Medium + 14, 2, e_Spr_Explo0_Medium + 15, 2, e_Spr_Explo0_Medium + 16, 2, e_Spr_Explo0_Medium + 17,
2, e_Spr_Explo0_Medium + 18, 2, e_Spr_Explo0_Medium + 19,
2, e_Spr_Explo0_Medium + 20, 2, e_Spr_Explo0_Medium + 21, 2, e_Spr_Explo0_Medium + 22, 2, e_Spr_Explo0_Medium + 23,
2, e_Spr_Explo0_Medium + 24, 2, e_Spr_Explo0_Medium + 25, 2, e_Spr_Explo0_Medium + 26, 2, e_Spr_Explo0_Medium + 27,
e_Anm_Kill
};
// Explosion générique - Moyenne - Dust (sans son).
u64	gAnm_Explosion0_Medium_NoSfx_Dust[] =
{
e_AnmKey_Null,
2, e_Spr_Explo0_Medium,      2, e_Spr_Explo0_Medium + 1,  2, e_Spr_Explo0_Medium + 2,  2, e_Spr_Explo0_Medium + 3,
2, e_Spr_Explo0_Medium + 4,  2, e_Spr_Explo0_Medium + 5,  2, e_Spr_Explo0_Medium + 6,  2, e_Spr_Explo0_Medium + 7,
2, e_Spr_Explo0_Medium + 8,  2, e_Spr_Explo0_Medium + 9,
2, e_Spr_Explo0_Medium + 10, 2, e_Spr_Explo0_Medium + 11, 2, e_Spr_Explo0_Medium + 12, 2, e_Spr_Explo0_Medium + 13,
2, e_Spr_Explo0_Medium + 14, 2, e_Spr_Explo0_Medium + 15, 2, e_Spr_Explo0_Medium + 16, 2, e_Spr_Explo0_Medium + 17,
2, e_Spr_Explo0_Medium + 18, 2, e_Spr_Explo0_Medium + 19,
2, e_Spr_Explo0_Medium + 20, 2, e_Spr_Explo0_Medium + 21, 2, e_Spr_Explo0_Medium + 22, 2, e_Spr_Explo0_Medium + 23,
2, e_Spr_Explo0_Medium + 24, 2, e_Spr_Explo0_Medium + 25, 2, e_Spr_Explo0_Medium + 26, 2, e_Spr_Explo0_Medium + 27,
e_Anm_Kill
};

// Tir ennemi - Explosion moyenne.
u64	gAnm_ShotEnemy_Explosion0_Medium[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Fx_Explosion2, e_SfxPrio_Explosion,
2, e_Spr_Explo0_Medium,      2, e_Spr_Explo0_Medium + 1,  2, e_Spr_Explo0_Medium + 2,  2, e_Spr_Explo0_Medium + 3,
2, e_Spr_Explo0_Medium + 4,  2, e_Spr_Explo0_Medium + 5,  2, e_Spr_Explo0_Medium + 6,  2, e_Spr_Explo0_Medium + 7,
2, e_Spr_Explo0_Medium + 8,  2, e_Spr_Explo0_Medium + 9,
2, e_Spr_Explo0_Medium + 10, 2, e_Spr_Explo0_Medium + 11, 2, e_Spr_Explo0_Medium + 12, 2, e_Spr_Explo0_Medium + 13,
2, e_Spr_Explo0_Medium + 14, 2, e_Spr_Explo0_Medium + 15, 2, e_Spr_Explo0_Medium + 16, 2, e_Spr_Explo0_Medium + 17,
2, e_Spr_Explo0_Medium + 18, 2, e_Spr_Explo0_Medium + 19,
2, e_Spr_Explo0_Medium + 20, 2, e_Spr_Explo0_Medium + 21, 2, e_Spr_Explo0_Medium + 22, 2, e_Spr_Explo0_Medium + 23,
2, e_Spr_Explo0_Medium + 24, 2, e_Spr_Explo0_Medium + 25, 2, e_Spr_Explo0_Medium + 26, 2, e_Spr_Explo0_Medium + 27,
e_Anm_End
};
// Tir - Explosion moyenne, large zone de touché.
u64	gAnm_ShotHero_Explosion0_Medium_LargeTouch[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Fx_Explosion2, e_SfxPrio_Explosion,
2, e_Spr_Explo0_Medium_LargeTouch,      2, e_Spr_Explo0_Medium_LargeTouch + 1,  2, e_Spr_Explo0_Medium_LargeTouch + 2,
2, e_Spr_Explo0_Medium_LargeTouch + 3,  2, e_Spr_Explo0_Medium_LargeTouch + 4,  2, e_Spr_Explo0_Medium_LargeTouch + 5,
2, e_Spr_Explo0_Medium + 6,  2, e_Spr_Explo0_Medium + 7,
2, e_Spr_Explo0_Medium + 8,  2, e_Spr_Explo0_Medium + 9,
2, e_Spr_Explo0_Medium + 10, 2, e_Spr_Explo0_Medium + 11, 2, e_Spr_Explo0_Medium + 12, 2, e_Spr_Explo0_Medium + 13,
2, e_Spr_Explo0_Medium + 14, 2, e_Spr_Explo0_Medium + 15, 2, e_Spr_Explo0_Medium + 16, 2, e_Spr_Explo0_Medium + 17,
2, e_Spr_Explo0_Medium + 18, 2, e_Spr_Explo0_Medium + 19,
2, e_Spr_Explo0_Medium + 20, 2, e_Spr_Explo0_Medium + 21, 2, e_Spr_Explo0_Medium + 22, 2, e_Spr_Explo0_Medium + 23,
2, e_Spr_Explo0_Medium + 24, 2, e_Spr_Explo0_Medium + 25, 2, e_Spr_Explo0_Medium + 26, 2, e_Spr_Explo0_Medium + 27,
e_Anm_End
};

// Lev 7 - Anim du tube foreground - Partie haute.
u64	gAnm_Lev7_TubeUp_fg[] =
{
e_AnmKey_Null,
16, e_Spr_Lev7_TubeUp_fg,
16, e_Spr_Lev7_TubeUp_fg + 1,
16, e_Spr_Lev7_TubeUp_fg + 2,
16, e_Spr_Lev7_TubeUp_fg + 3,
16, e_Spr_Lev7_TubeUp_fg + 4,
16, e_Spr_Lev7_TubeUp_fg + 5,
16, e_Spr_Lev7_TubeUp_fg + 6,
16, e_Spr_Lev7_TubeUp_fg + 7,
e_Anm_Jump, (u64)-16
};
// Lev 7 - Anim du tube foreground - Partie basse.
u64	gAnm_Lev7_TubeDown_fg[] =
{
e_AnmKey_Null,
16, e_Spr_Lev7_TubeDown_fg,
16, e_Spr_Lev7_TubeDown_fg + 1,
16, e_Spr_Lev7_TubeDown_fg + 2,
16, e_Spr_Lev7_TubeDown_fg + 3,
16, e_Spr_Lev7_TubeDown_fg + 4,
16, e_Spr_Lev7_TubeDown_fg + 5,
16, e_Spr_Lev7_TubeDown_fg + 6,
16, e_Spr_Lev7_TubeDown_fg + 7,
e_Anm_Jump, (u64)-16
};

// Lev 8 - Blocs - Ouverture de la PF 1.
u64	gAnm_Lev8_PF1_Opening[] =
{
e_AnmKey_Null,
3, e_Spr_Lev8_PF1Desc,
3, e_Spr_Lev8_PF1Desc + 1,
3, e_Spr_Lev8_PF1Desc + 2,
//e_Anm_Sfx, e_Sfx_MenuClic2, e_SfxPrio_0 + 5,		// Inaudible avec le levier en même temps.
3, e_Spr_Lev8_PF1Desc + 3,
e_Anm_End
};
// Lev 8 - Blocs - Ouverture de la porte 1.
u64	gAnm_Lev8_Door1_Opening[] =
{
e_AnmKey_Null,
3, e_Spr_Lev8_Door1Open,
3, e_Spr_Lev8_Door1Open + 1,
//e_Anm_Sfx, e_Sfx_MenuClic2, e_SfxPrio_0 + 5,		// Inaudible avec le coup de feu nécessaire pour ouvrir le cadenas.
3, e_Spr_Lev8_Door1Open + 2,
e_Anm_End
};

// Lev8 - Levier - Attente contact joueur.
u64	gAnm_Lev8_Lever_Wait[] =
{
e_AnmKey_Null,
128, e_Spr_Lev8_Lever1,
e_Anm_Jump, (u64)-2
};
// Lev8 - Levier - Inactif.
u64	gAnm_Lev8_Lever_Dead[] =
{
e_AnmKey_Null,
128, e_Spr_Lev8_Lever1 + 2,
e_Anm_Jump, (u64)-2
};
// Lev8 - Levier - Anim quand contact.
u64	gAnm_Lev8_Lever_CtcAnm[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Fx_Wood, e_SfxPrio_0 + 5,
3, e_Spr_Lev8_Lever1 + 1,
e_Anm_Goto, (u64)gAnm_Lev8_Lever_Dead
};

// Lev8 - Cadenas - Attente tir joueur.
u64	gAnm_Lev8_Lock_Wait[] =
{
e_AnmKey_Null,
24, e_Spr_Lev8_Lock1,
8, e_Spr_Lev8_Lock1 + 1,
8, e_Spr_Lev8_Lock1 + 2,
8, e_Spr_Lev8_Lock1 + 1,
e_Anm_Goto, (u64)gAnm_Lev8_Lock_Wait
};

// Lev8 - Porte du wagon à bestiaux - Ouverture part 2, rebond.
u64	gAnm_Lev8_WagonDoor_Opening2[] =
{
e_AnmKey_Null,
20, e_Spr_Lev8_WagonDoorOpening + 5,
32, e_Spr_Lev8_WagonDoorOpening + 6,
e_Anm_Jump, (u64)-2
};
// Lev8 - Porte du wagon à bestiaux - Ouverture.
u64	gAnm_Lev8_WagonDoor_Opening[] =
{
e_AnmKey_Enemy_Key3,
24, e_Spr_Lev8_WagonDoorOpening,
12, e_Spr_Lev8_WagonDoorOpening + 1,
10, e_Spr_Lev8_WagonDoorOpening + 2,
8, e_Spr_Lev8_WagonDoorOpening + 3,
6, e_Spr_Lev8_WagonDoorOpening + 4,
6, e_Spr_Lev8_WagonDoorOpening + 5,
6, e_Spr_Lev8_WagonDoorOpening + 6,
e_Anm_Sfx, e_Sfx_MenuClic2, e_SfxPrio_0 + 5,
e_Anm_Goto, (u64)gAnm_Lev8_WagonDoor_Opening2
};

// Lev13 - Hanging Rock - Wait.
u64	gAnm_Lev13_HangingRock_Wait[] =
{
e_AnmKey_Null,
12, e_Spr_Lev13_Rock1_Hanging,
12, e_Spr_Lev13_Rock1_Hanging + 1,
12, e_Spr_Lev13_Rock1_Hanging + 2,
12, e_Spr_Lev13_Rock1_Hanging + 3,
12, e_Spr_Lev13_Rock1_Hanging + 4,
12, e_Spr_Lev13_Rock1_Hanging + 3,
12, e_Spr_Lev13_Rock1_Hanging + 2,
12, e_Spr_Lev13_Rock1_Hanging + 1,
e_Anm_Goto, (u64)gAnm_Lev13_HangingRock_Wait
};
// Lev13 - Hanging Rock - Fall - Anim pour le tir.
u64	gAnm_Shot_Lev13_HangingRock_Fall[] =
{
e_AnmKey_Null,
128, e_Spr_Lev13_Rock1_Falling_Shot,
e_Anm_Goto, (u64)gAnm_Shot_Lev13_HangingRock_Fall
};
// Lev13 - Hanging Rock - Fall.
u64	gAnm_Lev13_HangingRock_Fall[] =
{
e_AnmKey_Null,
128, e_Spr_Lev13_Rock1_Falling,
e_Anm_Goto, (u64)gAnm_Lev13_HangingRock_Fall
};
// Lev13 - Hanging Rock - Landing.
u64	gAnm_Lev13_HangingRock_Land[] =
{
e_AnmKey_Null,
4, e_Spr_Lev13_Rock1_Landing,
4, e_Spr_Lev13_Rock1_Landing + 1,
4, e_Spr_Lev13_Rock1_Landing + 2,
4, e_Spr_Lev13_Rock1_Landing + 3,
128, e_Spr_Lev13_Rock1_Landing + 4,
e_Anm_Jump, (u64)-2
};


// Slug - Flèche IN.
u64	gAnm_Slug_InSign[] =
{
e_AnmKey_Null,
8, e_Spr_SlugInSign,
8, e_Spr_SlugInSign + 1,
8, e_Spr_SlugInSign + 2,
8, e_Spr_SlugInSign + 3,
8, e_Spr_SlugInSign + 4,
8, e_Spr_SlugInSign + 5,
8, e_Spr_SlugInSign + 6,
8, e_Spr_SlugInSign + 7,
8, e_Spr_SlugInSign + 8,
8, e_Spr_SlugInSign + 9,
8, e_Spr_SlugInSign + 10,
8, e_Spr_SlugInSign + 11,
8, e_Spr_SlugInSign + 12,
8, e_Spr_SlugInSign + 13,
8, e_Spr_SlugInSign + 14,
8, e_Spr_SlugInSign + 15,
e_Anm_Goto, (u64)gAnm_Slug_InSign
};
// Slug - Flèche OUT.
u64	gAnm_Slug_OutSign[] =
{
e_AnmKey_Null,
8, e_Spr_SlugOutSign,
8, e_Spr_SlugOutSign + 1,
8, e_Spr_SlugOutSign + 2,
8, e_Spr_SlugOutSign + 3,
8, e_Spr_SlugOutSign + 4,
8, e_Spr_SlugOutSign + 5,
8, e_Spr_SlugOutSign + 6,
8, e_Spr_SlugOutSign + 7,
8, e_Spr_SlugOutSign + 8,
8, e_Spr_SlugOutSign + 9,
8, e_Spr_SlugOutSign + 10,
8, e_Spr_SlugOutSign + 11,
8, e_Spr_SlugOutSign + 12,
8, e_Spr_SlugOutSign + 13,
8, e_Spr_SlugOutSign + 14,
8, e_Spr_SlugOutSign + 15,
e_Anm_Goto, (u64)gAnm_Slug_OutSign
};


// Vulcan cannon - Idle - Rotation par prg.
u64	gAnm_VulcanCannon_Idle[] =
{
e_AnmKey_Hero_Stance,
30, e_Spr_VulcanCannon_Still,
e_Anm_Jump, (u64)-2
};
// Vulcan cannon - Tir - Rotation par prg.
u64	gAnm_VulcanCannon_Shot[] =
{
e_AnmKey_Hero_Shot,
e_Anm_Sfx, e_Sfx_Shot_MiniRocket, e_SfxPrio_Hero_Shot,
3, e_Spr_VulcanCannon_Shooting,
3, e_Spr_VulcanCannon_Shooting + 32,
3, e_Spr_VulcanCannon_Shooting + 64,
3, e_Spr_VulcanCannon_Shooting + 96,
e_Anm_Goto, (u64)gAnm_VulcanCannon_Idle
};
// Tir héros - Vulcan cannon - Rotation par prg.
u64	gAnm_ShotHero_VulcanCannon[] =
{
e_AnmKey_Null,
128, e_Spr_Shot_VulcanCannon_Bullet,
e_Anm_Jump, (u64)-2
};
// Tir héros - Vulcan cannon - Dust.
u64	gAnm_ShotHero_VulcanCannon_Dust[] =
{
e_AnmKey_Null,
4, e_Spr_Shot_VulcanCannon_Bullet_Dust,
4, e_Spr_Shot_VulcanCannon_Bullet_Dust + 1,
4, e_Spr_Shot_VulcanCannon_Bullet_Dust + 2,
e_Anm_Kill
};

// Tir héros - Rocket Small - 32 frames, rotation par prg.
u64	gAnm_ShotHero_RocketSmall[] =
{
e_AnmKey_Null,
3, e_Spr_Shot_RocketSmall,
3, e_Spr_Shot_RocketSmall + 32,
e_Anm_Jump, (u64)-4
};
// Tir héros - Rocket Small - Trail - Dust.
u64	gAnm_RocketSmall_Trail_Dust[] =
{
e_AnmKey_Null,
2, e_Spr_Shot_RocketSmall_Trail,      2, e_Spr_Shot_RocketSmall_Trail + 1,  2, e_Spr_Shot_RocketSmall_Trail + 2,
2, e_Spr_Shot_RocketSmall_Trail + 3,  2, e_Spr_Shot_RocketSmall_Trail + 4,  2, e_Spr_Shot_RocketSmall_Trail + 5,
2, e_Spr_Shot_RocketSmall_Trail + 6,  2, e_Spr_Shot_RocketSmall_Trail + 7,  2, e_Spr_Shot_RocketSmall_Trail + 8,
2, e_Spr_Shot_RocketSmall_Trail + 9,
2, e_Spr_Shot_RocketSmall_Trail + 10, 2, e_Spr_Shot_RocketSmall_Trail + 11, 2, e_Spr_Shot_RocketSmall_Trail + 12,
2, e_Spr_Shot_RocketSmall_Trail + 13, 2, e_Spr_Shot_RocketSmall_Trail + 14, 2, e_Spr_Shot_RocketSmall_Trail + 15,
e_Anm_Kill
};

// Faire un truc générique ! avec 0 1 2 3... Sinon pour les héros, on va être embêtés.
// Slug In.
u64	gAnm_Hero_SlugGetIn[] =
{
e_AnmKey_Null,
4, 0,// e_Spr_HeroSub_SlugIn_Gun,
4, 1,// e_Spr_HeroSub_SlugIn_Gun + 1,
4, 2,// e_Spr_HeroSub_SlugIn_Gun + 2,
4, 3,// e_Spr_HeroSub_SlugIn_Gun + 3,
4, 4,// e_Spr_HeroSub_SlugIn_Gun + 4,
4, 5,// e_Spr_HeroSub_SlugIn_Gun + 5,
e_Anm_End
};

// Slug - Tank - Idle.
u64	gAnm_SlugTank_Idle[] =
{
e_AnmKey_Hero_Stance,
8, e_Spr_SlugTank_Idle,
8, e_Spr_SlugTank_Idle + 1,
e_Anm_Jump, (u64)-4
};
// Slug - Tank - Drive - Marche avant.
u64	gAnm_SlugTank_DriveForward[] =
{
e_AnmKey_Hero_Run,
//3, e_Spr_SlugTank_Drive,
3, e_Spr_SlugTank_Drive + 1,  3, e_Spr_SlugTank_Drive + 2,
3, e_Spr_SlugTank_Drive + 3,  3, e_Spr_SlugTank_Drive + 4,  3, e_Spr_SlugTank_Drive + 5,
3, e_Spr_SlugTank_Drive + 6,  3, e_Spr_SlugTank_Drive + 7,  3, e_Spr_SlugTank_Drive + 8,
3, e_Spr_SlugTank_Drive + 9,
//3, e_Spr_SlugTank_Drive + 10,
3, e_Spr_SlugTank_Drive + 11, 3, e_Spr_SlugTank_Drive + 12,
3, e_Spr_SlugTank_Drive + 13, 3, e_Spr_SlugTank_Drive + 14, 3, e_Spr_SlugTank_Drive + 15,
3, e_Spr_SlugTank_Drive + 16, 3, e_Spr_SlugTank_Drive + 17, 3, e_Spr_SlugTank_Drive + 18,
3, e_Spr_SlugTank_Drive + 19,
e_Anm_Goto, (u64)gAnm_SlugTank_DriveForward
};
// Slug - Tank - Drive - Marche arrière.
u64	gAnm_SlugTank_DriveBackward[] =
{
e_AnmKey_Hero_Run,
3, e_Spr_SlugTank_Drive + 19, 3, e_Spr_SlugTank_Drive + 18, 3, e_Spr_SlugTank_Drive + 17,
3, e_Spr_SlugTank_Drive + 16, 3, e_Spr_SlugTank_Drive + 15, 3, e_Spr_SlugTank_Drive + 14,
3, e_Spr_SlugTank_Drive + 13, 3, e_Spr_SlugTank_Drive + 12, 3, e_Spr_SlugTank_Drive + 11,
//3, e_Spr_SlugTank_Drive + 10,
3, e_Spr_SlugTank_Drive + 9,  3, e_Spr_SlugTank_Drive + 8,  3, e_Spr_SlugTank_Drive + 7,
3, e_Spr_SlugTank_Drive + 6,  3, e_Spr_SlugTank_Drive + 5,  3, e_Spr_SlugTank_Drive + 4,
3, e_Spr_SlugTank_Drive + 3,  3, e_Spr_SlugTank_Drive + 2,  3, e_Spr_SlugTank_Drive + 1,
//3, e_Spr_SlugTank_Drive,
e_Anm_Goto, (u64)gAnm_SlugTank_DriveBackward
};
// Slug - Tank - Drive - Arrêt.
u64	gAnm_SlugTank_DriveStop[] =
{
e_AnmKey_Hero_RunStop,
3, e_Spr_SlugTank_DriveStop,
3, e_Spr_SlugTank_DriveStop + 1,
4, e_Spr_SlugTank_DriveStop + 2,
5, e_Spr_SlugTank_DriveStop + 3,
4, e_Spr_SlugTank_DriveStop + 2,
3, e_Spr_SlugTank_DriveStop + 1,
3, e_Spr_SlugTank_DriveStop,
e_Anm_Goto, (u64)gAnm_SlugTank_Idle
};
// Slug - Tank - Idle crouch.
u64	gAnm_SlugTank_CrouchIdle[] =
{
e_AnmKey_Hero_CrouchStance,
8, e_Spr_SlugTankCrouch_Idle,
8, e_Spr_SlugTankCrouch_Idle + 1,
e_Anm_Jump, (u64)-4
};
// Slug - Tank - Drive - Marche avant.
u64	gAnm_SlugTank_CrouchDriveForward[] =
{
e_AnmKey_Hero_CrouchWalk,
//3, e_Spr_SlugTankCrouch_Drive,
3, e_Spr_SlugTankCrouch_Drive + 1,  3, e_Spr_SlugTankCrouch_Drive + 2,
3, e_Spr_SlugTankCrouch_Drive + 3,  3, e_Spr_SlugTankCrouch_Drive + 4,  3, e_Spr_SlugTankCrouch_Drive + 5,
3, e_Spr_SlugTankCrouch_Drive + 6,  3, e_Spr_SlugTankCrouch_Drive + 7,  3, e_Spr_SlugTankCrouch_Drive + 8,
3, e_Spr_SlugTankCrouch_Drive + 9,
//3, e_Spr_SlugTankCrouch_Drive + 10,
3, e_Spr_SlugTankCrouch_Drive + 11, 3, e_Spr_SlugTankCrouch_Drive + 12,
3, e_Spr_SlugTankCrouch_Drive + 13, 3, e_Spr_SlugTankCrouch_Drive + 14, 3, e_Spr_SlugTankCrouch_Drive + 15,
3, e_Spr_SlugTankCrouch_Drive + 16, 3, e_Spr_SlugTankCrouch_Drive + 17, 3, e_Spr_SlugTankCrouch_Drive + 18,
3, e_Spr_SlugTankCrouch_Drive + 19,
e_Anm_Goto, (u64)gAnm_SlugTank_CrouchDriveForward
};
// Slug - Tank - Drive - Marche arrière.
u64	gAnm_SlugTank_CrouchDriveBackward[] =
{
e_AnmKey_Hero_CrouchWalk,
3, e_Spr_SlugTankCrouch_Drive + 19, 3, e_Spr_SlugTankCrouch_Drive + 18, 3, e_Spr_SlugTankCrouch_Drive + 17,
3, e_Spr_SlugTankCrouch_Drive + 16, 3, e_Spr_SlugTankCrouch_Drive + 15, 3, e_Spr_SlugTankCrouch_Drive + 14,
3, e_Spr_SlugTankCrouch_Drive + 13, 3, e_Spr_SlugTankCrouch_Drive + 12, 3, e_Spr_SlugTankCrouch_Drive + 11,
//3, e_Spr_SlugTankCrouch_Drive + 10,
3, e_Spr_SlugTankCrouch_Drive + 9,  3, e_Spr_SlugTankCrouch_Drive + 8,  3, e_Spr_SlugTankCrouch_Drive + 7,
3, e_Spr_SlugTankCrouch_Drive + 6,  3, e_Spr_SlugTankCrouch_Drive + 5,  3, e_Spr_SlugTankCrouch_Drive + 4,
3, e_Spr_SlugTankCrouch_Drive + 3,  3, e_Spr_SlugTankCrouch_Drive + 2,  3, e_Spr_SlugTankCrouch_Drive + 1,
//3, e_Spr_SlugTankCrouch_Drive,
e_Anm_Goto, (u64)gAnm_SlugTank_CrouchDriveBackward
};
// Slug - Tank - Drive - Arrêt.
u64	gAnm_SlugTank_CrouchDriveStop[] =
{
e_AnmKey_Hero_CrouchWalkStop,
3, e_Spr_SlugTankCrouch_DriveStop,
3, e_Spr_SlugTankCrouch_DriveStop + 1,
4, e_Spr_SlugTankCrouch_DriveStop + 2,
5, e_Spr_SlugTankCrouch_DriveStop + 3,
4, e_Spr_SlugTankCrouch_DriveStop + 2,
3, e_Spr_SlugTankCrouch_DriveStop + 1,
3, e_Spr_SlugTankCrouch_DriveStop,
e_Anm_Goto, (u64)gAnm_SlugTank_CrouchIdle
};
// Slug - Tank - Entrée.
u64	gAnm_SlugTank_GetIn[] =
{
e_AnmKey_Null,
22, e_Spr_SlugTank_Close,
4, e_Spr_SlugTank_Close + 1,
4, e_Spr_SlugTank_Close + 2,
4, e_Spr_SlugTank_Close + 3,
e_Anm_End
};
// Slug - Tank - Sortie.
u64	gAnm_SlugTank_GetOut[] =
{
e_AnmKey_Null,
4, e_Spr_SlugTank_Close + 3,
4, e_Spr_SlugTank_Close + 2,
4, e_Spr_SlugTank_Close + 1,
4, e_Spr_SlugTank_Close,
e_Anm_End
};
// Slug - Tank - Tir.
u64	gAnm_SlugTank_Shot[] =
{
e_AnmKey_Hero_Shot,
3, e_Spr_SlugTank_ShotAnm,
3, e_Spr_SlugTank_ShotAnm + 1,
3, e_Spr_SlugTank_ShotAnm + 2,
e_Anm_Kill
};
// Saut montant (legs).
u64	gAnm_SlugTank_Jump[] =
{
e_AnmKey_Hero_Jump,
2, e_Spr_SlugTank_Jump,
2, e_Spr_SlugTank_Jump + 1,
2, e_Spr_SlugTank_Jump + 2,
2, e_Spr_SlugTank_Jump + 3,
2, e_Spr_SlugTank_Jump + 4,
2, e_Spr_SlugTank_Jump + 5,
2, e_Spr_SlugTank_Jump + 6,
2, e_Spr_SlugTank_Jump + 7,
e_Anm_Jump, (u64)-2
};
// Saut descendant (legs).
u64	gAnm_SlugTank_Fall[] =
{
e_AnmKey_Hero_Jump,
2, e_Spr_SlugTank_Jump + 7,
2, e_Spr_SlugTank_Jump + 6,
2, e_Spr_SlugTank_Jump + 5,
2, e_Spr_SlugTank_Jump + 4,
2, e_Spr_SlugTank_Jump + 3,
2, e_Spr_SlugTank_Jump + 2,
//2, e_Spr_SlugTank_Jump + 1,
//2, e_Spr_SlugTank_Jump,
e_Anm_Jump, (u64)-2
};
// Slug - Tank - Up 2 Down.
#define	SLUGTANK_DOWNUP_T0	2
u64	gAnm_SlugTank_Up2Down[] =
{
e_AnmKey_Hero_UpDown,
SLUGTANK_DOWNUP_T0, e_Spr_SlugTank_DownUp,
SLUGTANK_DOWNUP_T0, e_Spr_SlugTank_DownUp + 1,
SLUGTANK_DOWNUP_T0, e_Spr_SlugTank_DownUp + 2,
SLUGTANK_DOWNUP_T0, e_Spr_SlugTank_DownUp + 3,
SLUGTANK_DOWNUP_T0, e_Spr_SlugTank_DownUp + 4,
SLUGTANK_DOWNUP_T0, e_Spr_SlugTank_DownUp + 5,
SLUGTANK_DOWNUP_T0, e_Spr_SlugTank_DownUp + 6,
SLUGTANK_DOWNUP_T0, e_Spr_SlugTank_DownUp + 7,
e_Anm_Goto, (u64)gAnm_SlugTank_CrouchIdle
};
// Slug - Tank - Down 2 Up.
u64	gAnm_SlugTank_Down2Up[] =
{
e_AnmKey_Hero_UpDown,
SLUGTANK_DOWNUP_T0, e_Spr_SlugTank_DownUp + 7,
SLUGTANK_DOWNUP_T0, e_Spr_SlugTank_DownUp + 6,
SLUGTANK_DOWNUP_T0, e_Spr_SlugTank_DownUp + 5,
SLUGTANK_DOWNUP_T0, e_Spr_SlugTank_DownUp + 4,
SLUGTANK_DOWNUP_T0, e_Spr_SlugTank_DownUp + 3,
SLUGTANK_DOWNUP_T0, e_Spr_SlugTank_DownUp + 2,
SLUGTANK_DOWNUP_T0, e_Spr_SlugTank_DownUp + 1,
SLUGTANK_DOWNUP_T0, e_Spr_SlugTank_DownUp,
e_Anm_Goto, (u64)gAnm_SlugTank_Idle
};

// Slug - Sous-marin - Idle.
u64	gAnm_SlugSubmarine_Idle[] =
{
e_AnmKey_Null,
128, 0,
e_Anm_Jump, (u64)-2
};
// Slug - Sous-marin - Entrée.
u64	gAnm_SlugSubmarine_GetIn[] =
{
e_AnmKey_Null,
22, e_Spr_SlugSubmarine_Close,
2, e_Spr_SlugSubmarine_Close + 1,
2, e_Spr_SlugSubmarine_Close + 2,
2, e_Spr_SlugSubmarine_Close + 3,
2, e_Spr_SlugSubmarine_Close + 4,
2, e_Spr_SlugSubmarine_Close + 5,
e_Anm_End
};
// Slug - Sous-marin - Sortie.
u64	gAnm_SlugSubmarine_GetOut[] =
{
e_AnmKey_Null,
2, e_Spr_SlugSubmarine_Close + 5,
2, e_Spr_SlugSubmarine_Close + 4,
2, e_Spr_SlugSubmarine_Close + 3,
2, e_Spr_SlugSubmarine_Close + 2,
2, e_Spr_SlugSubmarine_Close + 1,
2, e_Spr_SlugSubmarine_Close,
e_Anm_End
};
// Tir héros - Roquette sous-marine - Rotation par prg.
u64	gAnm_ShotHero_SubmarineRocket[] =
{
e_AnmKey_Null,
128, e_Spr_Shot_SubmarineRocket_Bullet,
e_Anm_Jump, (u64)-2
};
// Tir héros - Roquette sous-marine - Trail - Dust.
u64	gAnm_SubmarineRocket_Trail_Dust[] =
{
e_AnmKey_Null,
5, e_Spr_Shot_SubmarineRocket_Trail_Dust,
5, e_Spr_Shot_SubmarineRocket_Trail_Dust + 1,
5, e_Spr_Shot_SubmarineRocket_Trail_Dust + 2,
5, e_Spr_Shot_SubmarineRocket_Trail_Dust + 3,
5, e_Spr_Shot_SubmarineRocket_Trail_Dust + 4,
5, e_Spr_Shot_SubmarineRocket_Trail_Dust + 5,
5, e_Spr_Shot_SubmarineRocket_Trail_Dust + 6,
5, e_Spr_Shot_SubmarineRocket_Trail_Dust + 7,
e_Anm_Kill
};

// Bulle sous marine - Dust.
u64	gAnm_SubmarineBubble_Dust[] =
{
e_AnmKey_Null,
3, e_Spr_SubmarineBubbles_Dust,
3, e_Spr_SubmarineBubbles_Dust + 1,
3, e_Spr_SubmarineBubbles_Dust + 2,
3, e_Spr_SubmarineBubbles_Dust + 3,
3, e_Spr_SubmarineBubbles_Dust + 4,
3, e_Spr_SubmarineBubbles_Dust + 5,
3, e_Spr_SubmarineBubbles_Dust + 6,
3, e_Spr_SubmarineBubbles_Dust + 7,
3, e_Spr_SubmarineBubbles_Dust + 8,
3, e_Spr_SubmarineBubbles_Dust + 9,
3, e_Spr_SubmarineBubbles_Dust + 10,
3, e_Spr_SubmarineBubbles_Dust + 11,
3, e_Spr_SubmarineBubbles_Dust + 12,
3, e_Spr_SubmarineBubbles_Dust + 13,
e_Anm_Kill
};

// Slug - Hélicoptère - Idle.
u64	gAnm_SlugCopter_Idle[] =
{
e_AnmKey_Null,
128, 0,
e_Anm_Jump, (u64)-2
};
// Slug - Hélicoptère - Entrée.
u64	gAnm_SlugCopter_GetIn[] =
{
e_AnmKey_Null,
2, e_Spr_SlugCopter_Close_Body,
2, e_Spr_SlugCopter_Close_Body + 1,
2, e_Spr_SlugCopter_Close_Body + 2,
2, e_Spr_SlugCopter_Close_Body + 3,
2, e_Spr_SlugCopter_Close_Body + 4,
2, e_Spr_SlugCopter_Close_Body + 5,
e_Anm_End
};
// Slug - Hélicoptère - Sortie.
u64	gAnm_SlugCopter_GetOut[] =
{
e_AnmKey_Null,
2, e_Spr_SlugCopter_Close_Body + 5,
2, e_Spr_SlugCopter_Close_Body + 4,
2, e_Spr_SlugCopter_Close_Body + 3,
2, e_Spr_SlugCopter_Close_Body + 2,
2, e_Spr_SlugCopter_Close_Body + 1,
2, e_Spr_SlugCopter_Close_Body,
e_Anm_End
};
// Slug - Hélicoptère - Attente du joueur (monstre).
u64	gAnm_SlugCopter_WaitPlayer[] =
{
e_AnmKey_Null,
16, e_Spr_SlugCopter_WaitPlayer,
16, e_Spr_SlugCopter_WaitPlayer + 1,
16, e_Spr_SlugCopter_WaitPlayer + 2,
16, e_Spr_SlugCopter_WaitPlayer + 3,
16, e_Spr_SlugCopter_WaitPlayer + 4,
16, e_Spr_SlugCopter_WaitPlayer + 5,
e_Anm_Goto, (u64)gAnm_SlugCopter_WaitPlayer
};
// Tir héros - Bombe de l'hélicoptère.
u64	gAnm_ShotHero_HelicopterBomb[] =
{
e_AnmKey_Null,
3, e_Spr_Shot_Helicopter_Bomb,
3, e_Spr_Shot_Helicopter_Bomb + 1,
3, e_Spr_Shot_Helicopter_Bomb + 2,
3, e_Spr_Shot_Helicopter_Bomb + 3,
3, e_Spr_Shot_Helicopter_Bomb + 4,
3, e_Spr_Shot_Helicopter_Bomb + 5,
3, e_Spr_Shot_Helicopter_Bomb + 6,
3, e_Spr_Shot_Helicopter_Bomb + 7,
128, e_Spr_Shot_Helicopter_Bomb + 8,
e_Anm_Jump, (u64)-2
};

// Slug - Rocket - Idle.
u64	gAnm_SlugRocket_Idle[] =
{
e_AnmKey_Null,
8, e_Spr_SlugRocket_Idle,
8, e_Spr_SlugRocket_Idle + 1,
8, e_Spr_SlugRocket_Idle + 2,
8, e_Spr_SlugRocket_Idle + 3,
e_Anm_Jump, (u64)-8
};
// Héros dans la fusée - Regarde vers la droite.
u64	gAnm_HeroInSlug_Rocket_LookR[] =
{
e_AnmKey_Null,
4, e_Spr_Hero_InSlug_Rocket,
4, e_Spr_Hero_InSlug_Rocket + 1,
8, e_Spr_Hero_InSlug_Rocket + 2,
8, e_Spr_Hero_InSlug_Rocket + 3,
8, e_Spr_Hero_InSlug_Rocket + 4,
8, e_Spr_Hero_InSlug_Rocket + 5,
e_Anm_Jump, (u64)-8
};
// Héros dans la fusée - Regarde vers la gauche.
u64	gAnm_HeroInSlug_Rocket_LookL[] =
{
e_AnmKey_Null,
4, e_Spr_Hero_InSlug_Rocket + 1,
4, e_Spr_Hero_InSlug_Rocket,
8, e_Spr_Hero_InSlug_Rocket + 6,
8, e_Spr_Hero_InSlug_Rocket + 7,
8, e_Spr_Hero_InSlug_Rocket + 8,
8, e_Spr_Hero_InSlug_Rocket + 9,
e_Anm_Jump, (u64)-8
};
// Slug - Rocket - Réacteur principal.
u64	gAnm_SlugRocket_MainThruster[] =
{
e_AnmKey_Null,
4, 0,
4, 1,
4, 2,
4, 3,
4, 4,
e_Anm_Jump, (u64)-10
};
// Slug - Rocket - Réacteur principal - Dust.
u64	gAnm_SlugRocket_MainThruster_Dust[] =
{
e_AnmKey_Null,
4, e_Spr_SlugRocket_MainThrustersTrail_Dust,
4, e_Spr_SlugRocket_MainThrustersTrail_Dust + 1,
4, e_Spr_SlugRocket_MainThrustersTrail_Dust + 2,
4, e_Spr_SlugRocket_MainThrustersTrail_Dust + 3,
4, e_Spr_SlugRocket_MainThrustersTrail_Dust + 4,
e_Anm_Kill
};
// Slug - Rocket - Arme Gauche - Sort.
u64	gAnm_SlugRocket_WeaponL_Out[] =
{
e_AnmKey_Null,
6, e_Spr_SlugRocket_GunL+1,
6, e_Spr_SlugRocket_GunL+2,
6, e_Spr_SlugRocket_GunL+3,
6, e_Spr_SlugRocket_GunL+4,
6, e_Spr_SlugRocket_GunL+5,
6, e_Spr_SlugRocket_GunL+6,
6, e_Spr_SlugRocket_GunL+7,
8, e_Spr_SlugRocket_GunL+8,
8, e_Spr_SlugRocket_GunL+9,
8, e_Spr_SlugRocket_GunL+10,
8, e_Spr_SlugRocket_GunL+11,
e_Anm_Jump, (u64)-8
};
// Slug - Rocket - Arme Gauche - Rentre.
u64	gAnm_SlugRocket_WeaponL_In[] =
{
e_AnmKey_Null,
6, e_Spr_SlugRocket_GunL+8,
6, e_Spr_SlugRocket_GunL+7,
6, e_Spr_SlugRocket_GunL+6,
6, e_Spr_SlugRocket_GunL+5,
6, e_Spr_SlugRocket_GunL+4,
6, e_Spr_SlugRocket_GunL+3,
6, e_Spr_SlugRocket_GunL+2,
6, e_Spr_SlugRocket_GunL+1,
e_Anm_End
};
// Slug - Rocket - Arme Droite - Sort.
u64	gAnm_SlugRocket_WeaponR_Out[] =
{
e_AnmKey_Null,
6, e_Spr_SlugRocket_GunR+1,
6, e_Spr_SlugRocket_GunR+2,
6, e_Spr_SlugRocket_GunR+3,
6, e_Spr_SlugRocket_GunR+4,
6, e_Spr_SlugRocket_GunR+5,
6, e_Spr_SlugRocket_GunR+6,
6, e_Spr_SlugRocket_GunR+7,
8, e_Spr_SlugRocket_GunR+8,
8, e_Spr_SlugRocket_GunR+9,
8, e_Spr_SlugRocket_GunR+10,
8, e_Spr_SlugRocket_GunR+11,
e_Anm_Jump, (u64)-8
};
// Slug - Rocket - Arme Droite - Rentre.
u64	gAnm_SlugRocket_WeaponR_In[] =
{
e_AnmKey_Null,
6, e_Spr_SlugRocket_GunR+8,
6, e_Spr_SlugRocket_GunR+7,
6, e_Spr_SlugRocket_GunR+6,
6, e_Spr_SlugRocket_GunR+5,
6, e_Spr_SlugRocket_GunR+4,
6, e_Spr_SlugRocket_GunR+3,
6, e_Spr_SlugRocket_GunR+2,
6, e_Spr_SlugRocket_GunR+1,
e_Anm_End
};
// Fusée : Etincelles de tir.
u64	gAnm_SlugRocket_NoSprite[] =
{
e_AnmKey_Null,
128, SPR_NoSprite,
e_Anm_Jump, (u64)-2
};
u64	gAnm_SlugRocket_ShotSparkMain[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Shot_MiniRocket, e_SfxPrio_Hero_Shot,
3, e_Spr_SlugRocket_ShotSparkMain,
3, e_Spr_SlugRocket_ShotSparkMain+1,
3, e_Spr_SlugRocket_ShotSparkMain+2,
3, e_Spr_SlugRocket_ShotSparkMain+3,
e_Anm_Goto, (u64)gAnm_SlugRocket_NoSprite
};
u64	gAnm_SlugRocket_ShotSparkSideL[] =
{
e_AnmKey_Null,
//e_Anm_Sfx, e_Sfx_Shot_MiniRocket, e_SfxPrio_Hero_Shot,	// Inutile, anim de tir latérale synchro avec le tir central.
3, e_Spr_SlugRocket_ShotSparkSide,
3, e_Spr_SlugRocket_ShotSparkSide+1,
3, e_Spr_SlugRocket_ShotSparkSide+2,
3, e_Spr_SlugRocket_ShotSparkSide+3,
e_Anm_Goto, (u64)gAnm_SlugRocket_NoSprite
};
u64	gAnm_SlugRocket_ShotSparkSideR[] =
{
e_AnmKey_Null,
//e_Anm_Sfx, e_Sfx_Shot_MiniRocket, e_SfxPrio_Hero_Shot,	// Inutile, anim de tir latérale synchro avec le tir central.
3, (e_Spr_SlugRocket_ShotSparkSide) | SPR_Flip_X,
3, (e_Spr_SlugRocket_ShotSparkSide+1) | SPR_Flip_X,
3, (e_Spr_SlugRocket_ShotSparkSide+2) | SPR_Flip_X,
3, (e_Spr_SlugRocket_ShotSparkSide+3) | SPR_Flip_X,
e_Anm_Goto, (u64)gAnm_SlugRocket_NoSprite
};
// Slug - Fusée - Entrée.
u64	gAnm_SlugRocket_GetIn[] =
{
e_AnmKey_Null,
22, e_Spr_SlugRocket_Close_Door+8,
3, e_Spr_SlugRocket_Close_Door+7,
3, e_Spr_SlugRocket_Close_Door+6,
3, e_Spr_SlugRocket_Close_Door+5,
3, e_Spr_SlugRocket_Close_Door+4,
3, e_Spr_SlugRocket_Close_Door+3,
3, e_Spr_SlugRocket_Close_Door+2,
3, e_Spr_SlugRocket_Close_Door+1,
3, e_Spr_SlugRocket_Close_Door,
e_Anm_End
};
// Slug - Thrust up.
#define	ROCKET_THRUST_T0	2
u64	gAnm_SlugRocket_ThrustUp[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Rocket1, e_SfxPrio_0 + 5,
ROCKET_THRUST_T0, e_Spr_SlugRocket_ThrustPushUp,
ROCKET_THRUST_T0, e_Spr_SlugRocket_ThrustPushUp + 1,
ROCKET_THRUST_T0, e_Spr_SlugRocket_ThrustPushUp + 2,
e_Anm_Goto, (u64)gAnm_SlugRocket_ThrustUp
};
// Slug - Thrust down.
u64	gAnm_SlugRocket_ThrustDown[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Rocket1, e_SfxPrio_0 + 5,
ROCKET_THRUST_T0, e_Spr_SlugRocket_ThrustPushDown,
ROCKET_THRUST_T0, e_Spr_SlugRocket_ThrustPushDown + 1,
ROCKET_THRUST_T0, e_Spr_SlugRocket_ThrustPushDown + 2,
e_Anm_Goto, (u64)gAnm_SlugRocket_ThrustDown
};
// Slug - Thrust left.
u64	gAnm_SlugRocket_ThrustLeft[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Rocket1, e_SfxPrio_0 + 5,
ROCKET_THRUST_T0, e_Spr_SlugRocket_ThrustPushLR,
ROCKET_THRUST_T0, e_Spr_SlugRocket_ThrustPushLR + 2,
ROCKET_THRUST_T0, e_Spr_SlugRocket_ThrustPushLR + 4,
e_Anm_Goto, (u64)gAnm_SlugRocket_ThrustLeft
};
// Slug - Thrust right.
u64	gAnm_SlugRocket_ThrustRight[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Rocket1, e_SfxPrio_0 + 5,
ROCKET_THRUST_T0, e_Spr_SlugRocket_ThrustPushLR + 1,
ROCKET_THRUST_T0, e_Spr_SlugRocket_ThrustPushLR + 3,
ROCKET_THRUST_T0, e_Spr_SlugRocket_ThrustPushLR + 5,
e_Anm_Goto, (u64)gAnm_SlugRocket_ThrustRight
};
// Slug - Thrust empty.
u64	gAnm_SlugRocket_ThrustEmpty[] =
{
e_AnmKey_Null,
128, SPR_NoSprite,
e_Anm_Jump, (u64)-2
};
// Tir héros - Bombe de la fusée.
u64	gAnm_ShotHero_RocketBomb[] =
{
e_AnmKey_Null,
3, e_Spr_Shot_Rocket_Bomb,
3, e_Spr_Shot_Rocket_Bomb + 1,
3, e_Spr_Shot_Rocket_Bomb + 2,
3, e_Spr_Shot_Rocket_Bomb + 3,
e_Anm_Jump, (u64)-8
};

// Rebel Soldier - Mort générique classique - Dust.
u64	gAnm_RebSoldier_DeathStd_Dust[] =
{
e_AnmKey_Null,
1, e_Spr_RebSoldier_DeathBullet,
ANIM_SFXRND(128, e_Sfx_Death_Scream1, e_SfxPrio_Enemy_Death, e_Sfx_Death_Scream2, e_SfxPrio_Enemy_Death)
2, e_Spr_RebSoldier_DeathBullet,      3, e_Spr_RebSoldier_DeathBullet + 1,  3, e_Spr_RebSoldier_DeathBullet + 2,
3, e_Spr_RebSoldier_DeathBullet + 3,  3, e_Spr_RebSoldier_DeathBullet + 4,  3, e_Spr_RebSoldier_DeathBullet + 5,
3, e_Spr_RebSoldier_DeathBullet + 6,  3, e_Spr_RebSoldier_DeathBullet + 7,  3, e_Spr_RebSoldier_DeathBullet + 8,
3, e_Spr_RebSoldier_DeathBullet + 9,
3, e_Spr_RebSoldier_DeathBullet + 10, 3, e_Spr_RebSoldier_DeathBullet + 11, 3, e_Spr_RebSoldier_DeathBullet + 12,
3, e_Spr_RebSoldier_DeathBullet + 13, 3, e_Spr_RebSoldier_DeathBullet + 14, 3, e_Spr_RebSoldier_DeathBullet + 15,
3, e_Spr_RebSoldier_DeathBullet + 16, 3, e_Spr_RebSoldier_DeathBullet + 17, 3, e_Spr_RebSoldier_DeathBullet + 18,
3, e_Spr_RebSoldier_DeathBullet + 19,
3, e_Spr_RebSoldier_DeathBullet + 20, 3, e_Spr_RebSoldier_DeathBullet + 21,
ANIM_CLIGNO(e_Spr_RebSoldier_DeathBullet + 21)
e_Anm_Kill
};
// Rebel Soldier - Mort générique au shotgun - Dust.
u64	gAnm_RebSoldier_DeathBlow_Dust[] =
{
e_AnmKey_Null,
1, e_Spr_RebSoldier_DeathShotgun,
ANIM_SFXRND(128, e_Sfx_Death_Scream1, e_SfxPrio_Enemy_Death, e_Sfx_Death_Scream2, e_SfxPrio_Enemy_Death)
2, e_Spr_RebSoldier_DeathShotgun,      3, e_Spr_RebSoldier_DeathShotgun + 1,  3, e_Spr_RebSoldier_DeathShotgun + 2,
3, e_Spr_RebSoldier_DeathShotgun + 3,  3, e_Spr_RebSoldier_DeathShotgun + 4,  3, e_Spr_RebSoldier_DeathShotgun + 5,
3, e_Spr_RebSoldier_DeathShotgun + 6,  3, e_Spr_RebSoldier_DeathShotgun + 7,  3, e_Spr_RebSoldier_DeathShotgun + 8,
3, e_Spr_RebSoldier_DeathShotgun + 9,
3, e_Spr_RebSoldier_DeathShotgun + 10, 3, e_Spr_RebSoldier_DeathShotgun + 11,
e_Anm_Kill
};
// Rebel Soldier - Mort générique brulé - Dust.
u64	gAnm_RebSoldier_DeathBurn_Dust[] =
{
e_AnmKey_Null,
1, e_Spr_RebSoldier_DeathFire,
ANIM_SFXRND(128, e_Sfx_Death_Scream1, e_SfxPrio_Enemy_Death, e_Sfx_Death_Scream2, e_SfxPrio_Enemy_Death)
3, e_Spr_RebSoldier_DeathFire,
4, e_Spr_RebSoldier_DeathFire + 1,
4, e_Spr_RebSoldier_DeathFire + 2,
4, e_Spr_RebSoldier_DeathFire + 3,
4, e_Spr_RebSoldier_DeathFire + 4,
4, e_Spr_RebSoldier_DeathFire + 5,
4, e_Spr_RebSoldier_DeathFire + 6,
4, e_Spr_RebSoldier_DeathFire + 7,
// Loop [0-7] possible.
4, e_Spr_RebSoldier_DeathFire + 8,
4, e_Spr_RebSoldier_DeathFire + 9,
4, e_Spr_RebSoldier_DeathFire + 10, 4, e_Spr_RebSoldier_DeathFire + 11, 4, e_Spr_RebSoldier_DeathFire + 12,
4, e_Spr_RebSoldier_DeathFire + 13, 4, e_Spr_RebSoldier_DeathFire + 14, 4, e_Spr_RebSoldier_DeathFire + 15,
4, e_Spr_RebSoldier_DeathFire + 16, 4, e_Spr_RebSoldier_DeathFire + 17, 4, e_Spr_RebSoldier_DeathFire + 18,
4, e_Spr_RebSoldier_DeathFire + 19,
4, e_Spr_RebSoldier_DeathFire + 20, 4, e_Spr_RebSoldier_DeathFire + 21,
ANIM_CLIGNO(e_Spr_RebSoldier_DeathFire + 21)
e_Anm_Kill
};

// Rebel soldier - Rifle - Attente accroupi.
u64	gAnm_RebSoldier_Rifle_CrouchWait[] =
{
e_AnmKey_Null,
8, e_Spr_RebSoldier_Rifle_CrouchWait,
8, e_Spr_RebSoldier_Rifle_CrouchWait + 1,
8, e_Spr_RebSoldier_Rifle_CrouchWait + 2,
8, e_Spr_RebSoldier_Rifle_CrouchWait + 1,
e_Anm_Goto, (u64)gAnm_RebSoldier_Rifle_CrouchWait
};
// Anim pour transition à la fin du tir avec clef NULL.
u64	gAnm_RebSoldier_Rifle_ShotEnd[] =
{
e_AnmKey_Null,
3, e_Spr_RebSoldier_Rifle_UpDown,
e_Anm_Goto, (u64)gAnm_RebSoldier_Rifle_WaitUp
};
// Rebel soldier - Rifle - Tir.
#define	RBSLDR_T0	3
u64	gAnm_RebSoldier_Rifle_Shot[] =
{
e_AnmKey_Enemy_Shot,
RBSLDR_T0+10, e_Spr_RebSoldier_Rifle_Shot,		// < On rallonge la pause ici.
e_Anm_Sfx, e_Sfx_Shot_Rifle, e_SfxPrio_Enemy_Shot,
RBSLDR_T0, e_Spr_RebSoldier_Rifle_Shot + 1,
RBSLDR_T0, e_Spr_RebSoldier_Rifle_Shot + 2,
RBSLDR_T0*2, e_Spr_RebSoldier_Rifle_Shot + 3,
RBSLDR_T0*2, e_Spr_RebSoldier_Rifle_Shot + 4,
RBSLDR_T0*2, e_Spr_RebSoldier_Rifle_Shot + 5,
RBSLDR_T0*2, e_Spr_RebSoldier_Rifle_Shot + 6,
RBSLDR_T0*3, e_Spr_RebSoldier_Rifle_Shot + 1,
e_Anm_Goto, (u64)gAnm_RebSoldier_Rifle_ShotEnd	//gAnm_RebSoldier_Rifle_WaitUp	//gAnm_RebSoldier_Rifle_GetDown
};
u64	gAnm_RebSoldier_Rifle_Shot_Dust[] =
{
e_AnmKey_Null,
RBSLDR_T0*2, e_Spr_RebSoldier_Rifle_Shot_Dust,
RBSLDR_T0, e_Spr_RebSoldier_Rifle_Shot_Dust + 1,
RBSLDR_T0, e_Spr_RebSoldier_Rifle_Shot_Dust + 2,
RBSLDR_T0, e_Spr_RebSoldier_Rifle_Shot_Dust + 3,
RBSLDR_T0*2, e_Spr_RebSoldier_Rifle_Shot_Dust + 4,
RBSLDR_T0*2, e_Spr_RebSoldier_Rifle_Shot_Dust + 5,
RBSLDR_T0*2, e_Spr_RebSoldier_Rifle_Shot_Dust + 6,
RBSLDR_T0*2, e_Spr_RebSoldier_Rifle_Shot_Dust + 7,
e_Anm_Kill
};
// Anim pour transition à la fin du tir avec clef NULL.
u64	gAnm_RebSoldier_Rifle_GetUpEnd[] =
{
e_AnmKey_Enemy_Shot,//_Null,
2, e_Spr_RebSoldier_Rifle_UpDown + 1,
e_Anm_Goto, (u64)gAnm_RebSoldier_Rifle_WaitUp
};
// Rebel soldier - Rifle - Se lève et passe en pos de tir.
u64	gAnm_RebSoldier_Rifle_GetUp[] =
{
e_AnmKey_Enemy_Key3,	//ANMPRIO(1) + 2 = shot
4, e_Spr_RebSoldier_Rifle_UpDown + 3,
4, e_Spr_RebSoldier_Rifle_UpDown + 2,
3, e_Spr_RebSoldier_Rifle_UpDown + 1,
//4, e_Spr_RebSoldier_Rifle_UpDown,
//e_Anm_Goto, (u64)gAnm_RebSoldier_Rifle_Shot
e_Anm_Goto, (u64)gAnm_RebSoldier_Rifle_GetUpEnd
};
// Tir rebel soldier - Balle.
u64	gAnm_ShotEnemy_Bullet[] =
{
e_AnmKey_Null,
4, e_Spr_RebSoldier_Bullet,
4, e_Spr_RebSoldier_Bullet + 1,
e_Anm_Jump, (u64)-4
};
// Rebel soldier - Rifle - Attente debout.
u64	gAnm_RebSoldier_Rifle_WaitUp[] =
{
e_AnmKey_Null,
8, e_Spr_RebSoldier_Rifle_WaitUp,
8, e_Spr_RebSoldier_Rifle_WaitUp + 1,
8, e_Spr_RebSoldier_Rifle_WaitUp + 2,
8, e_Spr_RebSoldier_Rifle_WaitUp + 1,
e_Anm_Goto, (u64)gAnm_RebSoldier_Rifle_WaitUp
};
// Rebel soldier - Rifle - Course.
u64	gAnm_RebSoldier_Rifle_Run[] =
{
e_AnmKey_Null,
4, e_Spr_RebSoldier_Rifle_Run,
4, e_Spr_RebSoldier_Rifle_Run + 1,
4, e_Spr_RebSoldier_Rifle_Run + 2,
4, e_Spr_RebSoldier_Rifle_Run + 3,
4, e_Spr_RebSoldier_Rifle_Run + 4,
4, e_Spr_RebSoldier_Rifle_Run + 5,
4, e_Spr_RebSoldier_Rifle_Run + 6,
4, e_Spr_RebSoldier_Rifle_Run + 7,
4, e_Spr_RebSoldier_Rifle_Run + 8,
4, e_Spr_RebSoldier_Rifle_Run + 9,
4, e_Spr_RebSoldier_Rifle_Run + 10,
4, e_Spr_RebSoldier_Rifle_Run + 11,
e_Anm_Goto, (u64)gAnm_RebSoldier_Rifle_Run
};
// Rebel soldier - Rifle - Se rasseoit.
u64	gAnm_RebSoldier_Rifle_GetDown[] =
{
e_AnmKey_Enemy_Key3,
4, e_Spr_RebSoldier_Rifle_UpDown + 1,
4, e_Spr_RebSoldier_Rifle_UpDown + 2,
4, e_Spr_RebSoldier_Rifle_UpDown + 3,
e_Anm_Goto, (u64)gAnm_RebSoldier_Rifle_CrouchWait
};
// Rebel soldier - Rifle - Frappe avec la crosse.
u64	gAnm_RebSoldier_Rifle_Strike[] =
{
e_AnmKey_Enemy_Strike,
e_Anm_Sfx, e_Sfx_Fx_Swoosh, e_SfxPrio_Enemy_Shot - 1,
4, e_Spr_RebSoldier_Rifle_Strike,
4, e_Spr_RebSoldier_Rifle_Strike + 1,
4, e_Spr_RebSoldier_Rifle_Strike + 2,
6, e_Spr_RebSoldier_Rifle_Strike + 3,
18, e_Spr_RebSoldier_Rifle_Strike + 4,
e_Anm_Goto, (u64)gAnm_RebSoldier_Rifle_WaitUp
};
// Rebel soldier - Rifle qui dégage - Dust.
u64	gAnm_RebSoldier_WeaponRifle_Dust[] =
{
e_AnmKey_Null,
128, e_Spr_RebSoldier_Weapon_Rifle,
e_Anm_Jump, (u64)-2
};

// Rebel soldier - Mortier - Attente accroupi.
u64	gAnm_RebSoldier_Mortar_CrouchWait[] =
{
e_AnmKey_Null,
6, e_Spr_RebSoldier_Mortar_CrouchWait,
6, e_Spr_RebSoldier_Mortar_CrouchWait + 1,
6, e_Spr_RebSoldier_Mortar_CrouchWait + 2,
6, e_Spr_RebSoldier_Mortar_CrouchWait + 3,
6, e_Spr_RebSoldier_Mortar_CrouchWait + 2,
6, e_Spr_RebSoldier_Mortar_CrouchWait + 1,
e_Anm_Goto, (u64)gAnm_RebSoldier_Mortar_CrouchWait
};
// Rebel soldier - Mortier - Tir.
#define	RBSLDR_T1	4
u64	gAnm_RebSoldier_Mortar_Shot[] =
{
e_AnmKey_Enemy_Shot,
RBSLDR_T1, e_Spr_RebSoldier_Mortar_Shot,
RBSLDR_T1, e_Spr_RebSoldier_Mortar_Shot + 1,
RBSLDR_T1, e_Spr_RebSoldier_Mortar_Shot + 2,
RBSLDR_T1, e_Spr_RebSoldier_Mortar_Shot + 3,
RBSLDR_T1, e_Spr_RebSoldier_Mortar_Shot + 4,
RBSLDR_T1, e_Spr_RebSoldier_Mortar_Shot + 5,
RBSLDR_T1, e_Spr_RebSoldier_Mortar_Shot + 6,
RBSLDR_T1, e_Spr_RebSoldier_Mortar_Shot + 7,
RBSLDR_T1, e_Spr_RebSoldier_Mortar_Shot + 8,
RBSLDR_T1, e_Spr_RebSoldier_Mortar_Shot + 9,
RBSLDR_T1, e_Spr_RebSoldier_Mortar_Shot + 10,
RBSLDR_T1, e_Spr_RebSoldier_Mortar_Shot + 11,
e_Anm_Sfx, e_Sfx_Shot_Mortar, e_SfxPrio_Enemy_Shot,
RBSLDR_T1*3, e_Spr_RebSoldier_Mortar_Shot + 12,
RBSLDR_T1*2, e_Spr_RebSoldier_Mortar_Shot + 13,
e_Anm_Goto, (u64)gAnm_RebSoldier_Mortar_CrouchWait
};
u64	gAnm_RebSoldier_Mortar_Shot_Dust[] =
{
e_AnmKey_Null,
RBSLDR_T1, e_Spr_RebSoldier_Mortar_Shot_Dust,
RBSLDR_T1, e_Spr_RebSoldier_Mortar_Shot_Dust + 1,
RBSLDR_T1, e_Spr_RebSoldier_Mortar_Shot_Dust + 2,
RBSLDR_T1, e_Spr_RebSoldier_Mortar_Shot_Dust + 3,
RBSLDR_T1, e_Spr_RebSoldier_Mortar_Shot_Dust + 4,
RBSLDR_T1, e_Spr_RebSoldier_Mortar_Shot_Dust + 5,
RBSLDR_T1, e_Spr_RebSoldier_Mortar_Shot_Dust + 6,
RBSLDR_T1, e_Spr_RebSoldier_Mortar_Shot_Dust + 7,
e_Anm_Kill
};
// Tir rebel soldier - Roquette du mortier.
#define	RBSLDR_T2	3
u64	gAnm_ShotEnemy_MortarRocket[] =
{
e_AnmKey_Null,
RBSLDR_T2, (e_Spr_RebSoldier_Rocket + 2+8) | SPR_Flip_X,
RBSLDR_T2, (e_Spr_RebSoldier_Rocket + 3+8) | SPR_Flip_X,
RBSLDR_T2, (e_Spr_RebSoldier_Rocket + 4+8) | SPR_Flip_X,
RBSLDR_T2, (e_Spr_RebSoldier_Rocket + 5+8) | SPR_Flip_X,
RBSLDR_T2-1, (e_Spr_RebSoldier_Rocket + 6+8) | SPR_Flip_X,
RBSLDR_T2-1, (e_Spr_RebSoldier_Rocket + 7+8) | SPR_Flip_X,
RBSLDR_T2-1, (e_Spr_RebSoldier_Rocket + 8+8) | SPR_Flip_X,
RBSLDR_T2, (e_Spr_RebSoldier_Rocket + 9+8) | SPR_Flip_X,
RBSLDR_T2, (e_Spr_RebSoldier_Rocket + 10+8) | SPR_Flip_X,
RBSLDR_T2, (e_Spr_RebSoldier_Rocket + 11+8) | SPR_Flip_X,
RBSLDR_T2, (e_Spr_RebSoldier_Rocket + 12+8) | SPR_Flip_X,
RBSLDR_T2, (e_Spr_RebSoldier_Rocket + 13+8) | SPR_Flip_X,
128, (e_Spr_RebSoldier_Rocket + 14+8) | SPR_Flip_X,
e_Anm_Jump, (u64)-2
};

// Rebel soldier std - Idle.
u64	gAnm_RebSoldier_Std_Idle[] =
{
e_AnmKey_Null,
8, e_Spr_RebSoldier_Std_Idle,
8, e_Spr_RebSoldier_Std_Idle + 1,
8, e_Spr_RebSoldier_Std_Idle + 2,
8, e_Spr_RebSoldier_Std_Idle + 3,
8, e_Spr_RebSoldier_Std_Idle + 2,
8, e_Spr_RebSoldier_Std_Idle + 1,
e_Anm_Goto, (u64)gAnm_RebSoldier_Std_Idle
};
// Rebel soldier std - Run.
u64	gAnm_RebSoldier_Std_Run[] =
{
e_AnmKey_Null,
4, e_Spr_RebSoldier_Std_Run,
4, e_Spr_RebSoldier_Std_Run + 1,
4, e_Spr_RebSoldier_Std_Run + 2,
4, e_Spr_RebSoldier_Std_Run + 3,
4, e_Spr_RebSoldier_Std_Run + 4,
4, e_Spr_RebSoldier_Std_Run + 5,
4, e_Spr_RebSoldier_Std_Run + 6,
4, e_Spr_RebSoldier_Std_Run + 7,
4, e_Spr_RebSoldier_Std_Run + 8,
4, e_Spr_RebSoldier_Std_Run + 9,
4, e_Spr_RebSoldier_Std_Run + 10,
4, e_Spr_RebSoldier_Std_Run + 11,
e_Anm_Goto, (u64)gAnm_RebSoldier_Std_Run
};
// Rebel soldier std - Jump.
u64	gAnm_RebSoldier_Std_Jump[] =
{
e_AnmKey_Null,
3, e_Spr_RebSoldier_Std_Jump,
3, e_Spr_RebSoldier_Std_Jump + 1,
3, e_Spr_RebSoldier_Std_Jump + 2,
3, e_Spr_RebSoldier_Std_Jump + 3,
3, e_Spr_RebSoldier_Std_Jump + 4,
16, e_Spr_RebSoldier_Std_Jump + 5,
e_Anm_Jump, (u64)-2
};
// Rebel soldier std - Fall.
u64	gAnm_RebSoldier_Std_Fall[] =
{
e_AnmKey_Null,
3, e_Spr_RebSoldier_Std_Jump + 5,
3, e_Spr_RebSoldier_Std_Jump + 4,
3, e_Spr_RebSoldier_Std_Jump + 3,
3, e_Spr_RebSoldier_Std_Jump + 2,
3, e_Spr_RebSoldier_Std_Jump + 1,
16, e_Spr_RebSoldier_Std_Jump,
e_Anm_Jump, (u64)-2
};
// Rebel soldier std - Descente en parachute.
u64	gAnm_RebSoldier_Std_Parachuting[] =
{
e_AnmKey_Null,
128, e_Spr_RebSoldier_Std_Jump+1,
e_Anm_Goto, (u64)gAnm_RebSoldier_Std_Parachuting
};
// Rebel soldier Std - Tir pistolet.
#define	RBSLDR_T5	3
u64	gAnm_RebSoldier_Std_Shot_Gun[] =
{
e_AnmKey_Enemy_Shot,
RBSLDR_T5, e_Spr_RebSoldier_Std_GunShot,
RBSLDR_T5, e_Spr_RebSoldier_Std_GunShot + 1,
RBSLDR_T5, e_Spr_RebSoldier_Std_GunShot + 2,
RBSLDR_T5, e_Spr_RebSoldier_Std_GunShot + 3,
RBSLDR_T5, e_Spr_RebSoldier_Std_GunShot + 4,
RBSLDR_T5, e_Spr_RebSoldier_Std_GunShot + 5,
e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Enemy_Shot,
RBSLDR_T5, e_Spr_RebSoldier_Std_GunShot + 6,
RBSLDR_T5, e_Spr_RebSoldier_Std_GunShot + 7,
RBSLDR_T5, e_Spr_RebSoldier_Std_GunShot + 8,
e_Anm_Goto, (u64)gAnm_RebSoldier_Std_Idle
};
// Rebel soldier Std - Lance une grenade.
#define	RBSLDR_T6	3
u64	gAnm_RebSoldier_Std_Shot_Handgrenade[] =
{
e_AnmKey_Enemy_Shot,
RBSLDR_T6, e_Spr_RebSoldier_Std_HandgrenadeShot,       RBSLDR_T6, e_Spr_RebSoldier_Std_HandgrenadeShot + 1,   RBSLDR_T6, e_Spr_RebSoldier_Std_HandgrenadeShot + 2,
RBSLDR_T6, e_Spr_RebSoldier_Std_HandgrenadeShot + 3,   RBSLDR_T6, e_Spr_RebSoldier_Std_HandgrenadeShot + 4,   RBSLDR_T6, e_Spr_RebSoldier_Std_HandgrenadeShot + 5,
RBSLDR_T6, e_Spr_RebSoldier_Std_HandgrenadeShot + 6,   RBSLDR_T6, e_Spr_RebSoldier_Std_HandgrenadeShot + 7,   RBSLDR_T6, e_Spr_RebSoldier_Std_HandgrenadeShot + 8,
e_Anm_Sfx, e_Sfx_Fx_Swoosh, e_SfxPrio_Enemy_Shot - 1,
RBSLDR_T6-2, e_Spr_RebSoldier_Std_HandgrenadeShot + 9,         2, e_Spr_RebSoldier_Std_HandgrenadeShot + 10,	// 2 frames pour l'image de tir, car getlastimage pour test du tir, puis getimage pour l'affichage de l'image en cours.
RBSLDR_T6, e_Spr_RebSoldier_Std_HandgrenadeShot + 11,  RBSLDR_T6, e_Spr_RebSoldier_Std_HandgrenadeShot + 12,
RBSLDR_T6, e_Spr_RebSoldier_Std_HandgrenadeShot + 13,  RBSLDR_T6, e_Spr_RebSoldier_Std_HandgrenadeShot + 14,
e_Anm_Goto, (u64)gAnm_RebSoldier_Std_Idle
};
// Tir rebel soldier - Std - Grenade.
u64	gAnm_ShotEnemy_StdHandgrenade[] =
{
e_AnmKey_Null,
2, e_Spr_RebSoldier_Handgrenade_Bullet,
2, e_Spr_RebSoldier_Handgrenade_Bullet + 1,
2, e_Spr_RebSoldier_Handgrenade_Bullet + 2,
2, e_Spr_RebSoldier_Handgrenade_Bullet + 3,
2, e_Spr_RebSoldier_Handgrenade_Bullet + 4,
2, e_Spr_RebSoldier_Handgrenade_Bullet + 5,
2, e_Spr_RebSoldier_Handgrenade_Bullet + 6,
2, e_Spr_RebSoldier_Handgrenade_Bullet + 7,
2, e_Spr_RebSoldier_Handgrenade_Bullet + 8,
2, e_Spr_RebSoldier_Handgrenade_Bullet + 9,
2, e_Spr_RebSoldier_Handgrenade_Bullet + 10,
2, e_Spr_RebSoldier_Handgrenade_Bullet + 11,
2, e_Spr_RebSoldier_Handgrenade_Bullet + 12,
2, e_Spr_RebSoldier_Handgrenade_Bullet + 13,
2, e_Spr_RebSoldier_Handgrenade_Bullet + 14,
2, e_Spr_RebSoldier_Handgrenade_Bullet + 15,
e_Anm_Goto, (u64)gAnm_ShotEnemy_StdHandgrenade
};

// Rebel soldier LRAC - Idle.
u64	gAnm_RebSoldier_LRAC_Idle[] =
{
e_AnmKey_Null,
8, e_Spr_RebSoldier_LRAC_Idle,
8, e_Spr_RebSoldier_LRAC_Idle + 1,
8, e_Spr_RebSoldier_LRAC_Idle + 2,
8, e_Spr_RebSoldier_LRAC_Idle + 3,
8, e_Spr_RebSoldier_LRAC_Idle + 2,
8, e_Spr_RebSoldier_LRAC_Idle + 1,
e_Anm_Goto, (u64)gAnm_RebSoldier_LRAC_Idle
};
// Rebel soldier LRAC - Run.
u64	gAnm_RebSoldier_LRAC_Run[] =
{
e_AnmKey_Null,
4, e_Spr_RebSoldier_LRAC_Run,
4, e_Spr_RebSoldier_LRAC_Run + 1,
4, e_Spr_RebSoldier_LRAC_Run + 2,
4, e_Spr_RebSoldier_LRAC_Run + 3,
4, e_Spr_RebSoldier_LRAC_Run + 4,
4, e_Spr_RebSoldier_LRAC_Run + 5,
4, e_Spr_RebSoldier_LRAC_Run + 6,
4, e_Spr_RebSoldier_LRAC_Run + 7,
4, e_Spr_RebSoldier_LRAC_Run + 8,
4, e_Spr_RebSoldier_LRAC_Run + 9,
4, e_Spr_RebSoldier_LRAC_Run + 10,
4, e_Spr_RebSoldier_LRAC_Run + 11,
e_Anm_Goto, (u64)gAnm_RebSoldier_LRAC_Run
};
// Rebel soldier LRAC - Jump.
u64	gAnm_RebSoldier_LRAC_Jump[] =
{
e_AnmKey_Null,
3, e_Spr_RebSoldier_LRAC_Jump,
3, e_Spr_RebSoldier_LRAC_Jump + 1,
3, e_Spr_RebSoldier_LRAC_Jump + 2,
3, e_Spr_RebSoldier_LRAC_Jump + 3,
3, e_Spr_RebSoldier_LRAC_Jump + 4,
16, e_Spr_RebSoldier_LRAC_Jump + 5,
e_Anm_Jump, (u64)-2
};
// Rebel soldier LRAC - Fall.
u64	gAnm_RebSoldier_LRAC_Fall[] =
{
e_AnmKey_Null,
3, e_Spr_RebSoldier_LRAC_Jump + 5,
3, e_Spr_RebSoldier_LRAC_Jump + 4,
3, e_Spr_RebSoldier_LRAC_Jump + 3,
3, e_Spr_RebSoldier_LRAC_Jump + 2,
3, e_Spr_RebSoldier_LRAC_Jump + 1,
16, e_Spr_RebSoldier_LRAC_Jump,
e_Anm_Jump, (u64)-2
};
// Rebel soldier LRAC - Tir.
u64	gAnm_RebSoldier_LRAC_Shot[] =
{
e_AnmKey_Enemy_Shot,
e_Anm_Sfx, e_Sfx_Shot_Mortar, e_SfxPrio_Enemy_Shot,
8, 9,			// Offset à ajouter à (e_Spr_RebSoldier_LRAC_Shot + angle).
8, 0,
e_Anm_Goto, (u64)gAnm_RebSoldier_LRAC_Idle
//e_Anm_End
};
// Fumée noire (fin du dust du LRAC, mais qui sert ailleurs) - Dust.
u64	gAnm_BlackSmoke_Dust[] =
{
e_AnmKey_Null,
2, e_Spr_RebSoldier_LRAC_Shot_Dust + 2,
2, e_Spr_RebSoldier_LRAC_Shot_Dust + 3,   2, e_Spr_RebSoldier_LRAC_Shot_Dust + 4,   2, e_Spr_RebSoldier_LRAC_Shot_Dust + 5,
2, e_Spr_RebSoldier_LRAC_Shot_Dust + 6,   2, e_Spr_RebSoldier_LRAC_Shot_Dust + 7,   2, e_Spr_RebSoldier_LRAC_Shot_Dust + 8,
2, e_Spr_RebSoldier_LRAC_Shot_Dust + 9,
2, e_Spr_RebSoldier_LRAC_Shot_Dust + 10,  2, e_Spr_RebSoldier_LRAC_Shot_Dust + 11,  2, e_Spr_RebSoldier_LRAC_Shot_Dust + 12,
2, e_Spr_RebSoldier_LRAC_Shot_Dust + 13,  2, e_Spr_RebSoldier_LRAC_Shot_Dust + 14,
e_Anm_Kill
};
// Rebel soldier LRAC - Tir - Dust.
u64	gAnm_RebSoldier_LRAC_Shot_Dust[] =
{
e_AnmKey_Null,
2, e_Spr_RebSoldier_LRAC_Shot_Dust,       2, e_Spr_RebSoldier_LRAC_Shot_Dust + 1,
e_Anm_Goto, (u64)gAnm_BlackSmoke_Dust
/*
2, e_Spr_RebSoldier_LRAC_Shot_Dust,       2, e_Spr_RebSoldier_LRAC_Shot_Dust + 1,   2, e_Spr_RebSoldier_LRAC_Shot_Dust + 2,
2, e_Spr_RebSoldier_LRAC_Shot_Dust + 3,   2, e_Spr_RebSoldier_LRAC_Shot_Dust + 4,   2, e_Spr_RebSoldier_LRAC_Shot_Dust + 5,
2, e_Spr_RebSoldier_LRAC_Shot_Dust + 6,   2, e_Spr_RebSoldier_LRAC_Shot_Dust + 7,   2, e_Spr_RebSoldier_LRAC_Shot_Dust + 8,
2, e_Spr_RebSoldier_LRAC_Shot_Dust + 9,
2, e_Spr_RebSoldier_LRAC_Shot_Dust + 10,  2, e_Spr_RebSoldier_LRAC_Shot_Dust + 11,  2, e_Spr_RebSoldier_LRAC_Shot_Dust + 12,
2, e_Spr_RebSoldier_LRAC_Shot_Dust + 13,  2, e_Spr_RebSoldier_LRAC_Shot_Dust + 14,
e_Anm_Kill
*/
};
// Tir rebel soldier LRAC - Roquette (32 frames de rotation).
u64	gAnm_ShotEnemy_LRAC_Rocket[] =
{
e_AnmKey_Null,
128, e_Spr_RebSoldier_Rocket,
e_Anm_Jump, (u64)-2
};
// Rebel soldier - LRAC qui dégage - Dust.
u64	gAnm_RebSoldier_WeaponLRAC_Dust[] =
{
e_AnmKey_Null,
128, e_Spr_RebSoldier_Weapon_LRAC,
e_Anm_Jump, (u64)-2
};

// Rebel soldier shield - Idle.
u64	gAnm_RebSoldier_Shield_Idle[] =
{
e_AnmKey_Null,
8, e_Spr_RebSoldier_Shield_Idle,
8, e_Spr_RebSoldier_Shield_Idle + 1,
8, e_Spr_RebSoldier_Shield_Idle + 2,
8, e_Spr_RebSoldier_Shield_Idle + 3,
8, e_Spr_RebSoldier_Shield_Idle + 2,
8, e_Spr_RebSoldier_Shield_Idle + 1,
e_Anm_Goto, (u64)gAnm_RebSoldier_Shield_Idle
};
// Rebel soldier shield - Bloque un tir.
#define	RBSLDR_T6b	4
u64	gAnm_RebSoldier_Shield_Block2[] =	// Block2 pour reblocage.
{
e_AnmKey_Enemy_Key3,
RBSLDR_T6b, e_Spr_RebSoldier_Shield_Block + 1,
RBSLDR_T6b, e_Spr_RebSoldier_Shield_Block + 2,
RBSLDR_T6b, e_Spr_RebSoldier_Shield_Block + 3,
e_Anm_Goto, (u64)gAnm_RebSoldier_Shield_Idle
};
u64	gAnm_RebSoldier_Shield_Block[] =
{
e_AnmKey_Enemy_Key3,
RBSLDR_T6b, e_Spr_RebSoldier_Shield_Block + 3,
RBSLDR_T6b, e_Spr_RebSoldier_Shield_Block + 2,
e_Anm_Goto, (u64)gAnm_RebSoldier_Shield_Block2
};
// Rebel soldier shield - Run.
u64	gAnm_RebSoldier_Shield_Run[] =
{
e_AnmKey_Null,
4, e_Spr_RebSoldier_Shield_Run,
4, e_Spr_RebSoldier_Shield_Run + 1,
4, e_Spr_RebSoldier_Shield_Run + 2,
4, e_Spr_RebSoldier_Shield_Run + 3,
4, e_Spr_RebSoldier_Shield_Run + 4,
4, e_Spr_RebSoldier_Shield_Run + 5,
4, e_Spr_RebSoldier_Shield_Run + 6,
4, e_Spr_RebSoldier_Shield_Run + 7,
4, e_Spr_RebSoldier_Shield_Run + 8,
4, e_Spr_RebSoldier_Shield_Run + 9,
4, e_Spr_RebSoldier_Shield_Run + 10,
4, e_Spr_RebSoldier_Shield_Run + 11,
e_Anm_Goto, (u64)gAnm_RebSoldier_Shield_Run
};
// Rebel soldier shield - Jump.
u64	gAnm_RebSoldier_Shield_Jump[] =
{
e_AnmKey_Null,
3, e_Spr_RebSoldier_Shield_Jump,
3, e_Spr_RebSoldier_Shield_Jump + 1,
3, e_Spr_RebSoldier_Shield_Jump + 2,
3, e_Spr_RebSoldier_Shield_Jump + 3,
3, e_Spr_RebSoldier_Shield_Jump + 4,
16, e_Spr_RebSoldier_Shield_Jump + 5,
e_Anm_Jump, (u64)-2
};
// Rebel soldier shield - Fall.
u64	gAnm_RebSoldier_Shield_Fall[] =
{
e_AnmKey_Null,
3, e_Spr_RebSoldier_Shield_Jump + 5,
3, e_Spr_RebSoldier_Shield_Jump + 4,
3, e_Spr_RebSoldier_Shield_Jump + 3,
3, e_Spr_RebSoldier_Shield_Jump + 2,
3, e_Spr_RebSoldier_Shield_Jump + 1,
16, e_Spr_RebSoldier_Shield_Jump,
e_Anm_Jump, (u64)-2
};
// Rebel soldier - Shield - Au contact, frappe avec une machette.
#define	RBSLDR_T6a	3
u64	gAnm_RebSoldier_Shield_Strike[] =
{
e_AnmKey_Enemy_Strike,
RBSLDR_T6a-1, e_Spr_RebSoldier_Shield_Strike,
RBSLDR_T6a-1, e_Spr_RebSoldier_Shield_Strike + 1,
RBSLDR_T6a+4-1, e_Spr_RebSoldier_Shield_Strike + 2,
RBSLDR_T6a, e_Spr_RebSoldier_Shield_Strike + 3,
RBSLDR_T6a, e_Spr_RebSoldier_Shield_Strike + 4,
RBSLDR_T6a, e_Spr_RebSoldier_Shield_Strike + 5,
e_Anm_Sfx, e_Sfx_Fx_Swoosh, e_SfxPrio_Enemy_Shot - 1,
RBSLDR_T6a, e_Spr_RebSoldier_Shield_Strike + 6,
RBSLDR_T6a, e_Spr_RebSoldier_Shield_Strike + 7,
RBSLDR_T6a, e_Spr_RebSoldier_Shield_Strike + 8,
RBSLDR_T6a, e_Spr_RebSoldier_Shield_Strike + 9,
RBSLDR_T6a+8, e_Spr_RebSoldier_Shield_Strike + 10,
RBSLDR_T6a, e_Spr_RebSoldier_Shield_Strike + 11,
RBSLDR_T6a, e_Spr_RebSoldier_Shield_Strike + 12,
RBSLDR_T6a+4, e_Spr_RebSoldier_Shield_Strike + 2,
RBSLDR_T6a, e_Spr_RebSoldier_Shield_Strike + 1,
RBSLDR_T6a, e_Spr_RebSoldier_Shield_Strike,
e_Anm_Goto, (u64)gAnm_RebSoldier_Shield_Idle
};
// Rebel soldier shield - Shield qui dégage - Dust.
#define	RBSLDR_T9	4
u64	gAnm_RebSoldier_WeaponShield_Dust[] =
{
e_AnmKey_Null,
RBSLDR_T9, e_Spr_RebSoldier_Weapon_Shield,
RBSLDR_T9, e_Spr_RebSoldier_Weapon_Shield + 1,
RBSLDR_T9, e_Spr_RebSoldier_Weapon_Shield + 2,
RBSLDR_T9, e_Spr_RebSoldier_Weapon_Shield + 3,
RBSLDR_T9, e_Spr_RebSoldier_Weapon_Shield + 4,
RBSLDR_T9, e_Spr_RebSoldier_Weapon_Shield + 5,
RBSLDR_T9, e_Spr_RebSoldier_Weapon_Shield + 6,
RBSLDR_T9, e_Spr_RebSoldier_Weapon_Shield + 7,
e_Anm_Goto, (u64)gAnm_RebSoldier_WeaponShield_Dust
};

// Rebel Soldier Sunbathing - Idle.
u64	gAnm_RebSoldier_Sunbath0_Idle[] =
{
e_AnmKey_Null,
12, e_Spr_RebSoldier_Sunbath0_Idle,
12, e_Spr_RebSoldier_Sunbath0_Idle + 1,
12, e_Spr_RebSoldier_Sunbath0_Idle + 2,
12, e_Spr_RebSoldier_Sunbath0_Idle + 3,
12, e_Spr_RebSoldier_Sunbath0_Idle + 2,
12, e_Spr_RebSoldier_Sunbath0_Idle + 1,
e_Anm_Goto, (u64)gAnm_RebSoldier_Sunbath0_Idle
};
// Rebel Soldier Sunbathing - Mort générique classique - Dust.
u64	gAnm_RebSoldier_Sunbath0_DeathStd_Dust[] =
{
e_AnmKey_Null,
1, e_Spr_RebSoldier_Sunbath0_Death,
ANIM_SFXRND(128, e_Sfx_Death_Scream1, e_SfxPrio_Enemy_Death, e_Sfx_Death_Scream2, e_SfxPrio_Enemy_Death)
2, e_Spr_RebSoldier_Sunbath0_Death,      3, e_Spr_RebSoldier_Sunbath0_Death + 1,  3, e_Spr_RebSoldier_Sunbath0_Death + 2,
3, e_Spr_RebSoldier_Sunbath0_Death + 3,  3, e_Spr_RebSoldier_Sunbath0_Death + 4,  3, e_Spr_RebSoldier_Sunbath0_Death + 5,
3, e_Spr_RebSoldier_Sunbath0_Death + 6,  3, e_Spr_RebSoldier_Sunbath0_Death + 7,  3, e_Spr_RebSoldier_Sunbath0_Death + 8,
3, e_Spr_RebSoldier_Sunbath0_Death + 9,
3, e_Spr_RebSoldier_Sunbath0_Death + 10, 3, e_Spr_RebSoldier_Sunbath0_Death + 11, 3, e_Spr_RebSoldier_Sunbath0_Death + 12,
3, e_Spr_RebSoldier_Sunbath0_Death + 13, 3, e_Spr_RebSoldier_Sunbath0_Death + 14, 3, e_Spr_RebSoldier_Sunbath0_Death + 15,
3, e_Spr_RebSoldier_Sunbath0_Death + 16, 3, e_Spr_RebSoldier_Sunbath0_Death + 17, 3, e_Spr_RebSoldier_Sunbath0_Death + 18,
ANIM_CLIGNO(e_Spr_RebSoldier_Sunbath0_Death + 18)
e_Anm_Kill
};

// Rebel Soldier Biker - Idle.
u64	gAnm_RebSoldier_Biker_Idle[] =
{
e_AnmKey_Null,
128, e_Spr_RebSoldier_Biker_Drive,
e_Anm_Goto, (u64)gAnm_RebSoldier_Biker_Idle
};
// Rebel Soldier Biker - Drive.
#define	RBSLDR_BIKER0	4
u64	gAnm_RebSoldier_Biker_Drive[] =
{
e_AnmKey_Null,
RBSLDR_BIKER0, e_Spr_RebSoldier_Biker_Drive,
RBSLDR_BIKER0, e_Spr_RebSoldier_Biker_Drive + 1,
RBSLDR_BIKER0, e_Spr_RebSoldier_Biker_Drive + 2,
RBSLDR_BIKER0, e_Spr_RebSoldier_Biker_Drive + 1,
e_Anm_Goto, (u64)gAnm_RebSoldier_Biker_Drive
};
// Rebel Soldier Biker - Jump.
u64	gAnm_RebSoldier_Biker_Jump[] =
{
e_AnmKey_Enemy_Shot,
3, e_Spr_RebSoldier_Biker_Jump,
3, e_Spr_RebSoldier_Biker_Jump + 1,
3, e_Spr_RebSoldier_Biker_Jump + 2,
e_Anm_Jump, (u64)-2
};
// Rebel Soldier Biker - Atterrissage.
u64	gAnm_RebSoldier_Biker_Land[] =
{
e_AnmKey_Enemy_Shot,
3, e_Spr_RebSoldier_Biker_Jump,
e_Anm_Goto, (u64)gAnm_RebSoldier_Biker_Drive
};
// Rebel Soldier Biker - Dust d'atterrissage.
u64	gAnm_RebSoldier_Biker_Land_Dust[] =
{
e_AnmKey_Null,
3, e_Spr_RebSoldier_Biker_Land_Dust,
3, e_Spr_RebSoldier_Biker_Land_Dust + 1,
3, e_Spr_RebSoldier_Biker_Land_Dust + 2,
3, e_Spr_RebSoldier_Biker_Land_Dust + 3,
3, e_Spr_RebSoldier_Biker_Land_Dust + 4,
3, e_Spr_RebSoldier_Biker_Land_Dust + 5,
3, e_Spr_RebSoldier_Biker_Land_Dust + 6,
e_Anm_Kill
};

// Truck Trooper - Idle.
u64	gAnm_TruckTrooper_Idle[] =
{
e_AnmKey_Null,
e_Anm_SfxStop, e_Sfx_Submarine0,
128, e_Spr_Truck0_Drive,
e_Anm_Jump, (u64)-2
};
// Truck Trooper - Avance/recule.
u64	gAnm_TruckTrooper_Drive[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Submarine0, e_SfxPrio_Explosion + 1,
8, e_Spr_Truck0_Drive,
8, e_Spr_Truck0_Drive + 1,
e_Anm_Sfx, e_Sfx_Submarine0, e_SfxPrio_Explosion + 1,
8, e_Spr_Truck0_Drive + 2,
8, e_Spr_Truck0_Drive + 3,
e_Anm_Goto, (u64)gAnm_TruckTrooper_Drive
};
// Truck Trooper - Déploiement de la planche.
u64	gAnm_TruckTrooper_TrunkOpen[] =
{
e_AnmKey_Null,
3, e_Spr_Truck0_TrunkOpen,
3, e_Spr_Truck0_TrunkOpen + 1,
3, e_Spr_Truck0_TrunkOpen + 2,
3, e_Spr_Truck0_TrunkOpen + 3,
3, e_Spr_Truck0_TrunkOpen + 4,
3, e_Spr_Truck0_TrunkOpen + 5,
3, e_Spr_Truck0_TrunkOpen + 6,
3, e_Spr_Truck0_TrunkOpen + 7,
3, e_Spr_Truck0_TrunkOpen + 8,
3, e_Spr_Truck0_TrunkOpen + 9,
3, e_Spr_Truck0_TrunkOpen + 10,
e_Anm_Sfx, e_Sfx_MenuClic2, e_SfxPrio_0 + 5,
e_Anm_StepFlag,
128, e_Spr_Truck0_TrunkOpen + 11,
e_Anm_Jump, (u64)-2
};
// Truck Trooper - Aim du drapeau quand détruit.
u64	gAnm_TruckTrooper_DestroyedAnm[] =
{
e_AnmKey_Null,
6, e_Spr_Truck0_DestroyedAnm,
6, e_Spr_Truck0_DestroyedAnm + 1,
6, e_Spr_Truck0_DestroyedAnm + 2,
6, e_Spr_Truck0_DestroyedAnm + 3,
6, e_Spr_Truck0_DestroyedAnm + 4,
6, e_Spr_Truck0_DestroyedAnm + 5,
e_Anm_Goto, (u64)gAnm_TruckTrooper_DestroyedAnm
};

#define	FTARA_T0	3
// Flying Tara - Debris Body 1 - Dust.
u64	gAnm_FlyingTara_DebrisBody1_Dust[] =
{
e_AnmKey_Null,
FTARA_T0, e_Spr_FlyingTara_DebrisBody1,
FTARA_T0, e_Spr_FlyingTara_DebrisBody1 + 1,
FTARA_T0, e_Spr_FlyingTara_DebrisBody1 + 2,
FTARA_T0, e_Spr_FlyingTara_DebrisBody1 + 3,
e_Anm_Goto, (u64)gAnm_FlyingTara_DebrisBody1_Dust
//e_Anm_Kill
};
// Flying Tara - Debris Body 2 - Dust.
u64	gAnm_FlyingTara_DebrisBody2_Dust[] =
{
e_AnmKey_Null,
FTARA_T0, e_Spr_FlyingTara_DebrisBody2,
FTARA_T0, e_Spr_FlyingTara_DebrisBody2 + 1,
FTARA_T0, e_Spr_FlyingTara_DebrisBody2 + 2,
FTARA_T0, e_Spr_FlyingTara_DebrisBody2 + 3,
e_Anm_Goto, (u64)gAnm_FlyingTara_DebrisBody2_Dust
//e_Anm_Kill
};
// Flying Tara - Debris Aile - Dust.
u64	gAnm_FlyingTara_DebrisWing_Dust[] =
{
e_AnmKey_Null,
FTARA_T0, e_Spr_FlyingTara_DebrisWing,
FTARA_T0, e_Spr_FlyingTara_DebrisWing + 1,
FTARA_T0, e_Spr_FlyingTara_DebrisWing + 2,
FTARA_T0, e_Spr_FlyingTara_DebrisWing + 3,
FTARA_T0, e_Spr_FlyingTara_DebrisWing + 4,
FTARA_T0, e_Spr_FlyingTara_DebrisWing + 5,
FTARA_T0, e_Spr_FlyingTara_DebrisWing + 6,
FTARA_T0, e_Spr_FlyingTara_DebrisWing + 7,
e_Anm_Goto, (u64)gAnm_FlyingTara_DebrisWing_Dust
//e_Anm_Kill
};
// Flying Tara - Debris Moteur - Dust.
u64	gAnm_FlyingTara_DebrisEngine_Dust[] =
{
e_AnmKey_Null,
FTARA_T0, e_Spr_FlyingTara_DebrisEngine,
FTARA_T0, e_Spr_FlyingTara_DebrisEngine + 1,
FTARA_T0, e_Spr_FlyingTara_DebrisEngine + 2,
FTARA_T0, e_Spr_FlyingTara_DebrisEngine + 3,
FTARA_T0, e_Spr_FlyingTara_DebrisEngine + 4,
FTARA_T0, e_Spr_FlyingTara_DebrisEngine + 5,
FTARA_T0, e_Spr_FlyingTara_DebrisEngine + 6,
FTARA_T0, e_Spr_FlyingTara_DebrisEngine + 7,
e_Anm_Goto, (u64)gAnm_FlyingTara_DebrisEngine_Dust
//e_Anm_Kill
};
// Flying Tara - Tir frontal.
u64	gAnm_FlyingTara_ShotFx[] =
{
e_AnmKey_Null,
e_Anm_StepFlag,					// Signal pour tirer.
e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Explosion + 1,	//e_SfxPrio_Enemy_Shot,		// Spécial L2, sinon le son des explosions mange celui des tirs. (Slug engine 30, tir heros 20, tir C heros 20, tir ennemi 10, explo 15).
3, e_Spr_FlyingTara_ShotFx,
3, e_Spr_FlyingTara_ShotFx + 1,
3, e_Spr_FlyingTara_ShotFx + 2,
3, e_Spr_FlyingTara_ShotFx + 3,
e_Anm_StepFlag,					// Signal pour tirer.
e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Explosion + 1,	//e_SfxPrio_Enemy_Shot,		// Spécial L2, sinon le son des explosions mange celui des tirs. (Slug engine 30, tir heros 20, tir C heros 20, tir ennemi 10, explo 15).
3, e_Spr_FlyingTara_ShotFx,
3, e_Spr_FlyingTara_ShotFx + 1,
3, e_Spr_FlyingTara_ShotFx + 2,
3, e_Spr_FlyingTara_ShotFx + 3,
e_Anm_StepFlag,					// Signal pour tirer.
e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Explosion + 1,	//e_SfxPrio_Enemy_Shot,		// Spécial L2, sinon le son des explosions mange celui des tirs. (Slug engine 30, tir heros 20, tir C heros 20, tir ennemi 10, explo 15).
3, e_Spr_FlyingTara_ShotFx,
3, e_Spr_FlyingTara_ShotFx + 1,
3, e_Spr_FlyingTara_ShotFx + 2,
3, e_Spr_FlyingTara_ShotFx + 3,
e_Anm_End
};

// Jellyfish small - Nage.
u64	gAnm_JellyfishSmall_Swim[] =
{
e_AnmKey_Null,
e_Anm_StepFlag,		// Flag step = Reset vitesse.
4, e_Spr_JellyfishSmall_Swim,       4, e_Spr_JellyfishSmall_Swim + 1,   4, e_Spr_JellyfishSmall_Swim + 2,
4, e_Spr_JellyfishSmall_Swim + 3,   4, e_Spr_JellyfishSmall_Swim + 4,   4, e_Spr_JellyfishSmall_Swim + 5,
4, e_Spr_JellyfishSmall_Swim + 6,   4, e_Spr_JellyfishSmall_Swim + 7,   4, e_Spr_JellyfishSmall_Swim + 8,
4, e_Spr_JellyfishSmall_Swim + 9,
e_Anm_Goto, (u64)gAnm_JellyfishSmall_Swim
};
// Jellyfish small - Mort - Dust.
u64	gAnm_JellyfishSmall_Death_Dust[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Death_Jellyfish, e_SfxPrio_Enemy_Death,
3, e_Spr_JellyfishSmall_Death,      3, e_Spr_JellyfishSmall_Death + 1,  3, e_Spr_JellyfishSmall_Death + 2,
3, e_Spr_JellyfishSmall_Death + 3,  3, e_Spr_JellyfishSmall_Death + 4,  3, e_Spr_JellyfishSmall_Death + 5,
3, e_Spr_JellyfishSmall_Death + 6,  3, e_Spr_JellyfishSmall_Death + 7,  3, e_Spr_JellyfishSmall_Death + 8,
3, e_Spr_JellyfishSmall_Death + 9,
3, e_Spr_JellyfishSmall_Death + 10, 3, e_Spr_JellyfishSmall_Death + 11, 3, e_Spr_JellyfishSmall_Death + 12,
e_Anm_Kill
};

// Squid big - Nage.
u64	gAnm_Squid_Swim[] =
{
e_AnmKey_Null,
e_Anm_StepFlag,		// Flag step = Reset vitesse.
4, e_Spr_Squid_Swim + 7,   4, e_Spr_Squid_Swim + 8,
4, e_Spr_Squid_Swim + 9,
4, e_Spr_Squid_Swim + 10,  4, e_Spr_Squid_Swim + 11,  4, e_Spr_Squid_Swim + 12,
4, e_Spr_Squid_Swim + 13,  4, e_Spr_Squid_Swim + 14,
4, e_Spr_Squid_Swim,       4, e_Spr_Squid_Swim + 1,   4, e_Spr_Squid_Swim + 2,
4, e_Spr_Squid_Swim + 3,   4, e_Spr_Squid_Swim + 4,   4, e_Spr_Squid_Swim + 5,
4, e_Spr_Squid_Swim + 6,
e_Anm_Goto, (u64)gAnm_Squid_Swim
};
// Squid big - Mort - Dust.
u64	gAnm_Squid_Death_Dust[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Death_MarsPeople, e_SfxPrio_Enemy_Death,	//e_Sfx_Death_Jellyfish
3, e_Spr_Squid_Death,      3, e_Spr_Squid_Death + 1,  3, e_Spr_Squid_Death + 2,
3, e_Spr_Squid_Death + 3,  3, e_Spr_Squid_Death + 4,  3, e_Spr_Squid_Death + 5,
3, e_Spr_Squid_Death + 6,  3, e_Spr_Squid_Death + 7,  3, e_Spr_Squid_Death + 8,
3, e_Spr_Squid_Death + 9,
3, e_Spr_Squid_Death + 10, 3, e_Spr_Squid_Death + 11,
e_Anm_Kill
};

// Whale - Attaque (anim de la bouche).
u64	gAnm_Whale_Attack[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Fx_WaterSplash2, e_SfxPrio_20,
12, 0,
8, 2,
8, 4,
128, 6,
e_Anm_Jump, (u64)-2
};
// Whale - Bulles - Dust.
u64	gAnm_Whale_Bubbles_Dust[] =
{
e_AnmKey_Null,
8, e_Spr_Whale_Bubbles_Dust,      8, e_Spr_Whale_Bubbles_Dust + 1,  8, e_Spr_Whale_Bubbles_Dust + 2,
8, e_Spr_Whale_Bubbles_Dust + 3,  8, e_Spr_Whale_Bubbles_Dust + 4,  8, e_Spr_Whale_Bubbles_Dust + 5,
8, e_Spr_Whale_Bubbles_Dust + 6,  8, e_Spr_Whale_Bubbles_Dust + 7,  8, e_Spr_Whale_Bubbles_Dust + 8,
8, e_Spr_Whale_Bubbles_Dust + 9,
e_Anm_Kill
};

// Jellyfish big - Nage.
u64	gAnm_JellyfishBig_Swim[] =
{
e_AnmKey_Null,
e_Anm_StepFlag,		// Flag step = Reset vitesse.
4, e_Spr_JellyfishBig_Swim,       4, e_Spr_JellyfishBig_Swim + 1,   4, e_Spr_JellyfishBig_Swim + 2,
4, e_Spr_JellyfishBig_Swim + 3,   4, e_Spr_JellyfishBig_Swim + 4,   4, e_Spr_JellyfishBig_Swim + 5,
4, e_Spr_JellyfishBig_Swim + 6,   4, e_Spr_JellyfishBig_Swim + 7,   4, e_Spr_JellyfishBig_Swim + 8,
4, e_Spr_JellyfishBig_Swim + 9,
e_Anm_Goto, (u64)gAnm_JellyfishBig_Swim
};
// Jellyfish big - Mort - Dust.
u64	gAnm_JellyfishBig_Death_Dust[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Death_Jellyfish, e_SfxPrio_Enemy_Death,
3, e_Spr_JellyfishBig_Death,      3, e_Spr_JellyfishBig_Death + 1,  3, e_Spr_JellyfishBig_Death + 2,
3, e_Spr_JellyfishBig_Death + 3,  3, e_Spr_JellyfishBig_Death + 4,  3, e_Spr_JellyfishBig_Death + 5,
3, e_Spr_JellyfishBig_Death + 6,  3, e_Spr_JellyfishBig_Death + 7,  3, e_Spr_JellyfishBig_Death + 8,
3, e_Spr_JellyfishBig_Death + 9,
3, e_Spr_JellyfishBig_Death + 10, 3, e_Spr_JellyfishBig_Death + 11,
e_Anm_Kill
};

// Piranha small - Nage.
u64	gAnm_Piranha_Swim[] =
{
e_AnmKey_Null,
e_Anm_StepFlag,		// Flag step = Reset vitesse.
4, e_Spr_Piranha_Swim + 4,   4, e_Spr_Piranha_Swim + 5,
10, e_Spr_Piranha_Swim + 6,   4, e_Spr_Piranha_Swim + 7,   4, e_Spr_Piranha_Swim + 8,
4, e_Spr_Piranha_Swim + 9,   4, e_Spr_Piranha_Swim + 10,  4, e_Spr_Piranha_Swim + 11,
4, e_Spr_Piranha_Swim,       4, e_Spr_Piranha_Swim + 1,   4, e_Spr_Piranha_Swim + 2,
4, e_Spr_Piranha_Swim + 3,
e_Anm_Goto, (u64)gAnm_Piranha_Swim
};
// Piranha small - Mort - Dust.
u64	gAnm_Piranha_Death_Dust[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Shot_Zombie, e_SfxPrio_Enemy_Death,	//e_Sfx_Death_Jellyfish
3, e_Spr_Piranha_Death,      3, e_Spr_Piranha_Death + 1,  3, e_Spr_Piranha_Death + 2,
3, e_Spr_Piranha_Death + 3,  3, e_Spr_Piranha_Death + 4,  3, e_Spr_Piranha_Death + 5,
3, e_Spr_Piranha_Death + 6,  3, e_Spr_Piranha_Death + 7,  3, e_Spr_Piranha_Death + 8,
3, e_Spr_Piranha_Death + 9,
3, e_Spr_Piranha_Death + 10, 3, e_Spr_Piranha_Death + 11, 3, e_Spr_Piranha_Death + 12,
3, e_Spr_Piranha_Death + 13, 3, e_Spr_Piranha_Death + 14, 3, e_Spr_Piranha_Death + 15,
3, e_Spr_Piranha_Death + 16,
e_Anm_Kill
};

// Sensor mine - Idle.
#define	SENSOR_T0	8
u64	gAnm_SensorMine_Idle[] =
{
e_AnmKey_Null,
SENSOR_T0, e_Spr_SensorMine,
SENSOR_T0, e_Spr_SensorMine + 1,
SENSOR_T0, e_Spr_SensorMine + 2,
SENSOR_T0, e_Spr_SensorMine + 3,
SENSOR_T0, e_Spr_SensorMine + 4,
SENSOR_T0, e_Spr_SensorMine + 5,
SENSOR_T0, e_Spr_SensorMine + 6,
SENSOR_T0, e_Spr_SensorMine + 7,
e_Anm_RndJump, 128, -16, 4,
SENSOR_T0, e_Spr_SensorMine + 6,
SENSOR_T0, e_Spr_SensorMine + 5,
SENSOR_T0, e_Spr_SensorMine + 4,
SENSOR_T0, e_Spr_SensorMine + 3,
SENSOR_T0, e_Spr_SensorMine + 2,
SENSOR_T0, e_Spr_SensorMine + 1,
e_Anm_Goto, (u64)gAnm_SensorMine_Idle
};

// Rocket diver - Saute.
u64	gAnm_RocketDiver_Jump[] =
{
e_AnmKey_Null,
6, e_Spr_RocketDiver_Jump,
6, e_Spr_RocketDiver_Jump + 1,
128, e_Spr_RocketDiver_Jump + 2,
e_Anm_Jump, (u64)-2
};
// Rocket diver - Chute après l'attaque.
u64	gAnm_RocketDiver_Fall[] =
{
e_AnmKey_Null,
128, e_Spr_RocketDiver_Shot + 7,
e_Anm_Jump, (u64)-2
};
// Rocket diver - Attaque.
u64	gAnm_RocketDiver_Attack[] =
{
e_AnmKey_Enemy_Shot,
3, e_Spr_RocketDiver_Shot,
3, e_Spr_RocketDiver_Shot + 1,
3, e_Spr_RocketDiver_Shot + 2,
3, e_Spr_RocketDiver_Shot + 3,
3, e_Spr_RocketDiver_Shot + 4,
3, e_Spr_RocketDiver_Shot + 5,
3, e_Spr_RocketDiver_Shot + 6,
3, e_Spr_RocketDiver_Shot + 7,
e_Anm_Goto, (u64)gAnm_RocketDiver_Fall
};
// Rocket Diver - Roquette (tir). (Un peu lame, mais faire un flag exprès juste pour ça n'est pas rentable).
#define	RDVR_T0	3
u64	gAnm_ShotEnemy_RocketDiverRocket[] =
{
e_AnmKey_Null,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
//
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
//
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
//
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 1,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 2,   RDVR_T0, e_Spr_RocketDiver_Rocket_Bullet + 3,
//
e_Anm_End
};
// Rocket diver - Mort - Chute.
u64	gAnm_RocketDiver_DeathFall[] =
{
e_AnmKey_Enemy_Shot + 1,	// +1, juste pour avoir le même niveau de priorité que l'anim de tir, pour anmsetifnew de la mort.
3, e_Spr_RocketDiver_Death,
3, e_Spr_RocketDiver_Death + 1,
3, e_Spr_RocketDiver_Death + 2,
3, e_Spr_RocketDiver_Death + 3,
3, e_Spr_RocketDiver_Death + 4,
128, e_Spr_RocketDiver_Death + 5,
e_Anm_Jump, (u64)-2
};
// Rocket diver - Mort - Atterrissage + disparition.
u64	gAnm_RocketDiver_DeathLand_Dust[] =
{
e_AnmKey_Null,
3, e_Spr_RocketDiver_Death + 6,
3, e_Spr_RocketDiver_Death + 7,
3, e_Spr_RocketDiver_Death + 6,
ANIM_CLIGNO(e_Spr_RocketDiver_Death + 6)
e_Anm_Kill
};
// Rocket diver - Ploouf de décollage/atterrissage dans l'eau - Dust.
u64	gAnm_RocketDiver_WaterSplash_Dust[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Fx_WaterSplash2, e_SfxPrio_0 + 5,
3, e_Spr_Hero_DeathGen_Plouf,       3, e_Spr_Hero_DeathGen_Plouf + 1,   3, e_Spr_Hero_DeathGen_Plouf + 2,
3, e_Spr_Hero_DeathGen_Plouf + 3,   3, e_Spr_Hero_DeathGen_Plouf + 4,   3, e_Spr_Hero_DeathGen_Plouf + 5,
3, e_Spr_Hero_DeathGen_Plouf + 6,   3, e_Spr_Hero_DeathGen_Plouf + 7,   3, e_Spr_Hero_DeathGen_Plouf + 8,
3, e_Spr_Hero_DeathGen_Plouf + 9,
3, e_Spr_Hero_DeathGen_Plouf + 10,  3, e_Spr_Hero_DeathGen_Plouf + 11,  3, e_Spr_Hero_DeathGen_Plouf + 12,
3, e_Spr_Hero_DeathGen_Plouf + 13,  3, e_Spr_Hero_DeathGen_Plouf + 14,  3, e_Spr_Hero_DeathGen_Plouf + 15,
3, e_Spr_Hero_DeathGen_Plouf + 16,  3, e_Spr_Hero_DeathGen_Plouf + 17,  3, e_Spr_Hero_DeathGen_Plouf + 18,
3, e_Spr_Hero_DeathGen_Plouf + 19,
3, e_Spr_Hero_DeathGen_Plouf + 20,  3, e_Spr_Hero_DeathGen_Plouf + 21,  3, e_Spr_Hero_DeathGen_Plouf + 22,
3, e_Spr_Hero_DeathGen_Plouf + 23,  3, e_Spr_Hero_DeathGen_Plouf + 24,  3, e_Spr_Hero_DeathGen_Plouf + 25,
3, e_Spr_Hero_DeathGen_Plouf + 26,
e_Anm_Kill
};
// Rocket diver - Roquette qui dégage en tournoyant - Dust.
u64	gAnm_RocketDiver_RocketRot_Dust[] =
{
e_AnmKey_Null,
128, e_Spr_RocketDiver_Rocket_Rot,
e_Anm_Jump, (u64)-2
};

// Girida0 - Turret - Shot fx.
u64	gAnm_Girida0_Turret_ShotFx_Dust[] =
{
e_AnmKey_Null,
3, e_Spr_Girida0_ShotFx,
3, e_Spr_Girida0_ShotFx + 1,
3, e_Spr_Girida0_ShotFx + 2,
e_Anm_Kill,
};
// Girida0 - Turret - Idle.
u64	gAnm_Girida0_Turret_Idle[] =
{
e_AnmKey_Null,
128, 0,
e_Anm_Jump, (u64)-2
};
// Girida0 - Turret - Tir.
u64	gAnm_Girida0_Turret_Shot[] =
{
e_AnmKey_Enemy_Strike,
e_Anm_Sfx, e_Sfx_Shot_Mortar, e_SfxPrio_Enemy_Shot,
3, 0,
3, 1,
3, 2,
3, 3,
3, 2,
3, 1,
3, 0,
e_Anm_Goto, (u64)gAnm_Girida0_Turret_Idle
};
// Girida0 - Body - Idle - Anm d'init sans son, au cas ou on soit d'abord en phase d'attente.
u64	gAnm_Girida0_Body_Idle_NoFx[] =
{
e_AnmKey_Null,
16, e_Spr_Girida0_Body_Idle,
16, e_Spr_Girida0_Body_Idle + 1,
e_Anm_Jump, (u64)-4
};
// Girida0 - Body - Idle.
u64	gAnm_Girida0_Body_Idle[] =
{
e_AnmKey_Null,
e_Anm_SfxStop, e_Sfx_Submarine1,
e_Anm_Sfx, e_Sfx_Submarine0, e_SfxPrio_Enemy_Shot + 2,
16, e_Spr_Girida0_Body_Idle,
e_Anm_Sfx, e_Sfx_Submarine0, e_SfxPrio_Enemy_Shot + 2,
16, e_Spr_Girida0_Body_Idle + 1,
e_Anm_Jump, (u64)-10//-4
};
// Girida0 - Body - Tir.
u64	gAnm_Girida0_Body_Shot[] =
{
e_AnmKey_Enemy_Strike,
3, e_Spr_Girida0_Body_Shot,
3, e_Spr_Girida0_Body_Shot + 1,
e_Anm_SfxStop, e_Sfx_Submarine1,
e_Anm_Sfx, e_Sfx_Submarine0, e_SfxPrio_Enemy_Shot + 2,
3, e_Spr_Girida0_Body_Shot + 2,
3, e_Spr_Girida0_Body_Shot + 3,
3, e_Spr_Girida0_Body_Shot + 2,
3, e_Spr_Girida0_Body_Shot + 1,
3, e_Spr_Girida0_Body_Shot,
e_Anm_Goto, (u64)gAnm_Girida0_Body_Idle
};
// Girida0 - Body - Drive forward.
u64	gAnm_Girida0_Body_DriveForward[] =
{
e_AnmKey_Enemy_Key3,	//e_AnmKey_Null,
e_Anm_SfxStop, e_Sfx_Submarine0,
e_Anm_Sfx, e_Sfx_Submarine1, e_SfxPrio_Enemy_Shot + 2,
3, e_Spr_Girida0_Body_Drive,
3, e_Spr_Girida0_Body_Drive + 1,
3, e_Spr_Girida0_Body_Drive + 2,
3, e_Spr_Girida0_Body_Drive + 3,
3, e_Spr_Girida0_Body_Drive + 4,
e_Anm_Sfx, e_Sfx_Submarine1, e_SfxPrio_Enemy_Shot + 2,
3, e_Spr_Girida0_Body_Drive + 5,
3, e_Spr_Girida0_Body_Drive + 6,
3, e_Spr_Girida0_Body_Drive + 7,
3, e_Spr_Girida0_Body_Drive + 8,
3, e_Spr_Girida0_Body_Drive + 9,
e_Anm_Goto, (u64)gAnm_Girida0_Body_DriveForward
};
// Girida0 - Body - Drive backwards.
u64	gAnm_Girida0_Body_DriveBackwards[] =
{
e_AnmKey_Enemy_Key3,	//e_AnmKey_Null,
e_Anm_SfxStop, e_Sfx_Submarine0,
e_Anm_Sfx, e_Sfx_Submarine1, e_SfxPrio_Enemy_Shot + 2,
3, e_Spr_Girida0_Body_Drive + 9,
3, e_Spr_Girida0_Body_Drive + 8,
3, e_Spr_Girida0_Body_Drive + 7,
3, e_Spr_Girida0_Body_Drive + 6,
3, e_Spr_Girida0_Body_Drive + 5,
e_Anm_Sfx, e_Sfx_Submarine1, e_SfxPrio_Enemy_Shot + 2,
3, e_Spr_Girida0_Body_Drive + 4,
3, e_Spr_Girida0_Body_Drive + 3,
3, e_Spr_Girida0_Body_Drive + 2,
3, e_Spr_Girida0_Body_Drive + 1,
3, e_Spr_Girida0_Body_Drive,
e_Anm_Goto, (u64)gAnm_Girida0_Body_DriveBackwards
};
// Girida0 - Body - Destroyed.
u64	gAnm_Girida0_Body_Destroyed[] =
{
e_AnmKey_Null,
e_Anm_SfxStop, e_Sfx_Submarine0,
e_Anm_SfxStop, e_Sfx_Submarine1,
3, e_Spr_Girida0_Body_Destroyed,       3, e_Spr_Girida0_Body_Destroyed + 1,   3, e_Spr_Girida0_Body_Destroyed + 2,
3, e_Spr_Girida0_Body_Destroyed + 3,   3, e_Spr_Girida0_Body_Destroyed + 4,   3, e_Spr_Girida0_Body_Destroyed + 5,
3, e_Spr_Girida0_Body_Destroyed + 6,   3, e_Spr_Girida0_Body_Destroyed + 7,   3, e_Spr_Girida0_Body_Destroyed + 8,
3, e_Spr_Girida0_Body_Destroyed + 9,
3, e_Spr_Girida0_Body_Destroyed + 10,  3, e_Spr_Girida0_Body_Destroyed + 11,  3, e_Spr_Girida0_Body_Destroyed + 12,
3, e_Spr_Girida0_Body_Destroyed + 13,  3, e_Spr_Girida0_Body_Destroyed + 14,  3, e_Spr_Girida0_Body_Destroyed + 15,
3, e_Spr_Girida0_Body_Destroyed + 16,  3, e_Spr_Girida0_Body_Destroyed + 17,  3, e_Spr_Girida0_Body_Destroyed + 18,
16, e_Spr_Girida0_Body_Destroyed + 19,
e_Anm_End
};
// Girida0 - Boulet.
#define	GRD0_T0	3
u64	gAnm_ShotEnemy_Girida0Bullet[] =
{
e_AnmKey_Null,
GRD0_T0, e_Spr_Girida0_Bullet,       GRD0_T0, e_Spr_Girida0_Bullet + 1,   GRD0_T0, e_Spr_Girida0_Bullet + 2,
GRD0_T0, e_Spr_Girida0_Bullet + 3,   GRD0_T0, e_Spr_Girida0_Bullet + 4,   GRD0_T0, e_Spr_Girida0_Bullet + 5,
GRD0_T0, e_Spr_Girida0_Bullet + 6,   GRD0_T0, e_Spr_Girida0_Bullet + 7,   GRD0_T0, e_Spr_Girida0_Bullet + 8,
GRD0_T0, e_Spr_Girida0_Bullet + 9,
GRD0_T0, e_Spr_Girida0_Bullet + 10,  GRD0_T0, e_Spr_Girida0_Bullet + 11,  GRD0_T0, e_Spr_Girida0_Bullet + 12,
GRD0_T0, e_Spr_Girida0_Bullet + 13,  GRD0_T0, e_Spr_Girida0_Bullet + 14,  GRD0_T0, e_Spr_Girida0_Bullet + 15,
GRD0_T0, e_Spr_Girida0_Bullet + 16,  GRD0_T0, e_Spr_Girida0_Bullet + 17,  GRD0_T0, e_Spr_Girida0_Bullet + 18,
e_Anm_Goto, (u64)gAnm_ShotEnemy_Girida0Bullet
};

// Boss 1 - Canon qui tire en l'air - Idle.
u64	gAnm_Boss1_CannonUp_Idle[] =
{
e_AnmKey_Null,
128, e_Spr_Boss1_CannonUp_Shot + 20,
e_Anm_Jump, (u64)-2
};
// Boss 1 - Canon qui tire en l'air - Tir.
u64	gAnm_Boss1_CannonUp_Shot[] =
{
e_AnmKey_Enemy_Shot,
2, e_Spr_Boss1_CannonUp_Shot,       2, e_Spr_Boss1_CannonUp_Shot + 1,   2, e_Spr_Boss1_CannonUp_Shot + 2,
2, e_Spr_Boss1_CannonUp_Shot + 3,
e_Anm_Sfx, e_Sfx_Shot_MiniRocket, e_SfxPrio_Enemy_Shot,
e_Anm_StepFlag,
// Loop
2, e_Spr_Boss1_CannonUp_Shot + 4,   2, e_Spr_Boss1_CannonUp_Shot + 5,   2, e_Spr_Boss1_CannonUp_Shot + 6,
2, e_Spr_Boss1_CannonUp_Shot + 7,
e_Anm_Sfx, e_Sfx_Shot_MiniRocket, e_SfxPrio_Enemy_Shot,
e_Anm_StepFlag,
2, e_Spr_Boss1_CannonUp_Shot + 8,   2, e_Spr_Boss1_CannonUp_Shot + 9,
2, e_Spr_Boss1_CannonUp_Shot + 10,  2, e_Spr_Boss1_CannonUp_Shot + 11,
e_Anm_Sfx, e_Sfx_Shot_MiniRocket, e_SfxPrio_Enemy_Shot,
e_Anm_StepFlag,
// Loop
2, e_Spr_Boss1_CannonUp_Shot + 4,   2, e_Spr_Boss1_CannonUp_Shot + 5,   2, e_Spr_Boss1_CannonUp_Shot + 6,
2, e_Spr_Boss1_CannonUp_Shot + 7,
e_Anm_Sfx, e_Sfx_Shot_MiniRocket, e_SfxPrio_Enemy_Shot,
e_Anm_StepFlag,
2, e_Spr_Boss1_CannonUp_Shot + 8,   2, e_Spr_Boss1_CannonUp_Shot + 9,
2, e_Spr_Boss1_CannonUp_Shot + 10,  2, e_Spr_Boss1_CannonUp_Shot + 11,
e_Anm_Sfx, e_Sfx_Shot_MiniRocket, e_SfxPrio_Enemy_Shot,
e_Anm_StepFlag,
// Loop
2, e_Spr_Boss1_CannonUp_Shot + 12,
2, e_Spr_Boss1_CannonUp_Shot + 13,  2, e_Spr_Boss1_CannonUp_Shot + 14,  2, e_Spr_Boss1_CannonUp_Shot + 15,
e_Anm_Sfx, e_Sfx_Shot_MiniRocket, e_SfxPrio_Enemy_Shot,
e_Anm_StepFlag,
2, e_Spr_Boss1_CannonUp_Shot + 16,  2, e_Spr_Boss1_CannonUp_Shot + 17,  2, e_Spr_Boss1_CannonUp_Shot + 18,
2, e_Spr_Boss1_CannonUp_Shot + 19,
e_Anm_Goto, (u64)gAnm_Boss1_CannonUp_Idle
};
// Boss 1 - Canon qui tire en l'air - Détruit.
u64	gAnm_Boss1_CannonUp_Destroyed[] =
{
e_AnmKey_Null,
128, e_Spr_Boss1_CannonUp_Destroyed,
e_Anm_Jump, (u64)-2
};
// Boss 1 - Bombe parachute.
u64	gAnm_ShotEnemy_Boss1_ParachuteBomb[] =
{
e_AnmKey_Null,
8, e_Spr_Boss1_BombeParachute,
8, e_Spr_Boss1_BombeParachute + 1,
8, e_Spr_Boss1_BombeParachute + 2,
8, e_Spr_Boss1_BombeParachute + 3,
8, e_Spr_Boss1_BombeParachute + 4,
8, e_Spr_Boss1_BombeParachute + 3,
8, e_Spr_Boss1_BombeParachute + 2,
8, e_Spr_Boss1_BombeParachute + 1,
e_Anm_Goto, (u64)gAnm_ShotEnemy_Boss1_ParachuteBomb
};
// Boss 1 - Canon frontal - Idle.
u64	gAnm_Boss1_CannonFront_Idle[] =
{
e_AnmKey_Null,
128, e_Spr_Boss1_CannonFront_Idle,
e_Anm_Jump, (u64)-2
};
// Boss 1 - Canon frontal - Tir.
u64	gAnm_Boss1_CannonFront_Shot[] =
{
e_AnmKey_Enemy_Shot,
3, e_Spr_Boss1_CannonFront_Shot,
3, e_Spr_Boss1_CannonFront_Shot + 1,
3, e_Spr_Boss1_CannonFront_Shot + 2,
3, e_Spr_Boss1_CannonFront_Shot + 3,
3, e_Spr_Boss1_CannonFront_Shot + 4,
e_Anm_Sfx, e_Sfx_Shot_Mortar, e_SfxPrio_Enemy_Shot,
e_Anm_StepFlag,
3, e_Spr_Boss1_CannonFront_Shot + 5,
3, e_Spr_Boss1_CannonFront_Shot + 6,
3, e_Spr_Boss1_CannonFront_Shot + 7,
3, e_Spr_Boss1_CannonFront_Shot + 8,
3, e_Spr_Boss1_CannonFront_Shot + 9,
3, e_Spr_Boss1_CannonFront_Shot + 10,
3, e_Spr_Boss1_CannonFront_Shot + 11,
3, e_Spr_Boss1_CannonFront_Shot + 12,
3, e_Spr_Boss1_CannonFront_Shot + 13,
e_Anm_Goto, (u64)gAnm_Boss1_CannonFront_Idle
};
// Boss 1 - Obus du canon.
u64	gAnm_ShotEnemy_Boss1_BulletFront[] =
{
e_AnmKey_Null,
2, (e_Spr_Boss1_CannonFront_Bullet) | SPR_Flip_X,
2, (e_Spr_Boss1_CannonFront_Bullet + 1) | SPR_Flip_X,
2, (e_Spr_Boss1_CannonFront_Bullet + 2) | SPR_Flip_X,
2, (e_Spr_Boss1_CannonFront_Bullet + 3) | SPR_Flip_X,
e_Anm_Goto, (u64)gAnm_ShotEnemy_Boss1_BulletFront
};
// Boss 1 - Bâche couvrant le canon qui dégagge - Dust.
u64	gAnm_Boss1_Tarp_Dust[] =
{
e_AnmKey_Null,
4, e_Spr_Boss1_Tarp_Dust,
4, e_Spr_Boss1_Tarp_Dust + 1,
4, e_Spr_Boss1_Tarp_Dust + 2,
4, e_Spr_Boss1_Tarp_Dust + 3,
4, e_Spr_Boss1_Tarp_Dust + 4,
4, e_Spr_Boss1_Tarp_Dust + 5,
4, e_Spr_Boss1_Tarp_Dust + 6,
4, e_Spr_Boss1_Tarp_Dust + 7,
4, e_Spr_Boss1_Tarp_Dust + 8,
4, e_Spr_Boss1_Tarp_Dust + 9,
4, e_Spr_Boss1_Tarp_Dust + 10,
4, e_Spr_Boss1_Tarp_Dust + 11,
4, e_Spr_Boss1_Tarp_Dust + 12,
4, e_Spr_Boss1_Tarp_Dust + 13,
4, e_Spr_Boss1_Tarp_Dust + 14,
e_Anm_Kill,
};

// Boss 2A - Montée des canons frontaux (offset pour anim qui sert aux deux).
#define	BOSS2A_T1	3
u64	gAnm_Boss2A_FrontCannonGetUp[] =
{
e_AnmKey_Null,
1, SPR_NoSprite,
BOSS2A_T1, 0,   BOSS2A_T1, 1,   BOSS2A_T1, 2,
BOSS2A_T1, 3,   BOSS2A_T1, 4,   BOSS2A_T1, 5,
BOSS2A_T1, 6,   BOSS2A_T1, 7,   BOSS2A_T1, 8,
BOSS2A_T1, 9,
BOSS2A_T1, 10,  BOSS2A_T1, 11,
BOSS2A_T1, 12,
e_Anm_Jump, (u64)-2
};
// Boss 2A - Etincelle des tirs des tourelles.
u64	gAnm_Boss2A_Turret_ShotSpark[] =
{
e_AnmKey_Null,
e_Anm_StepFlag,					// Signal pour tirer.
e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Explosion + 1,	//e_SfxPrio_Enemy_Shot,		// Spécial L2, sinon le son des explosions mange celui des tirs. (Slug engine 30, tir heros 20, tir C heros 20, tir ennemi 10, explo 15).
3, 0,
3, 17,
3, 34,
e_Anm_Kill
//e_Anm_End
};
// Boss 2A - Etincelle des tirs des canons frontaux.
#define	BOSS2A_T2	3
u64	gAnm_Boss2A_FrontCannon_ShotSpark[] =
{
e_AnmKey_Null,
e_Anm_StepFlag,					// Signal pour tirer.
e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Explosion + 1,	//e_SfxPrio_Enemy_Shot,		// Spécial L2, sinon le son des explosions mange celui des tirs. (Slug engine 30, tir heros 20, tir C heros 20, tir ennemi 10, explo 15).
BOSS2A_T2, e_Spr_Boss2Front_FrontCannon_ShotSpark,
BOSS2A_T2, e_Spr_Boss2Front_FrontCannon_ShotSpark + 1,
// > Loop
e_Anm_StepFlag,
e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Explosion + 1,	//e_SfxPrio_Enemy_Shot,		// Spécial L2, sinon le son des explosions mange celui des tirs. (Slug engine 30, tir heros 20, tir C heros 20, tir ennemi 10, explo 15).
BOSS2A_T2, e_Spr_Boss2Front_FrontCannon_ShotSpark + 2,
BOSS2A_T2, e_Spr_Boss2Front_FrontCannon_ShotSpark + 3,
e_Anm_StepFlag,
e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Explosion + 1,	//e_SfxPrio_Enemy_Shot,		// Spécial L2, sinon le son des explosions mange celui des tirs. (Slug engine 30, tir heros 20, tir C heros 20, tir ennemi 10, explo 15).
BOSS2A_T2, e_Spr_Boss2Front_FrontCannon_ShotSpark + 4,
BOSS2A_T2, e_Spr_Boss2Front_FrontCannon_ShotSpark + 5,
// < Loop
e_Anm_StepFlag,
e_Anm_Sfx, e_Sfx_Shot_Gun, e_SfxPrio_Explosion + 1,	//e_SfxPrio_Enemy_Shot,		// Spécial L2, sinon le son des explosions mange celui des tirs. (Slug engine 30, tir heros 20, tir C heros 20, tir ennemi 10, explo 15).
BOSS2A_T2, e_Spr_Boss2Front_FrontCannon_ShotSpark + 2,
BOSS2A_T2, e_Spr_Boss2Front_FrontCannon_ShotSpark + 3,
BOSS2A_T2, e_Spr_Boss2Front_FrontCannon_ShotSpark + 6,
BOSS2A_T2, e_Spr_Boss2Front_FrontCannon_ShotSpark + 7,
e_Anm_Kill
//e_Anm_End
};

// Boss 2B - Trainée de flames.
#define	BOSS2B_T0	4
u64	gAnm_Boss2B_FlameShot[] =
{
e_AnmKey_Null,
	e_Anm_Sfx, e_Sfx_Shot_Flame, e_SfxPrio_Hero_Shot - 1,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 1,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 2,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 3,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 4,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 5,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 6,
	e_Anm_Sfx, e_Sfx_Rocket0, e_SfxPrio_Hero_Shot - 1,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 7,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 8,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 9,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 10,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 11,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 12,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 13,

	e_Anm_Sfx, e_Sfx_Rocket0, e_SfxPrio_Hero_Shot - 1,
BOSS2B_T0/2, e_Spr_Boss2Back_FlameShot + 14,
BOSS2B_T0/2, e_Spr_Boss2Back_FlameShot + 15,
BOSS2B_T0/2, e_Spr_Boss2Back_FlameShot + 16,
BOSS2B_T0/2, e_Spr_Boss2Back_FlameShot + 17,
BOSS2B_T0/2, e_Spr_Boss2Back_FlameShot + 18,
BOSS2B_T0/2, e_Spr_Boss2Back_FlameShot + 19,
BOSS2B_T0/2, e_Spr_Boss2Back_FlameShot + 20,
BOSS2B_T0/2, e_Spr_Boss2Back_FlameShot + 21,
BOSS2B_T0/2, e_Spr_Boss2Back_FlameShot + 22,
BOSS2B_T0/2, e_Spr_Boss2Back_FlameShot + 23,
BOSS2B_T0/2, e_Spr_Boss2Back_FlameShot + 24,
BOSS2B_T0/2, e_Spr_Boss2Back_FlameShot + 25,
BOSS2B_T0/2, e_Spr_Boss2Back_FlameShot + 26,
BOSS2B_T0/2, e_Spr_Boss2Back_FlameShot + 27,
BOSS2B_T0/2, e_Spr_Boss2Back_FlameShot + 28,
BOSS2B_T0/2, e_Spr_Boss2Back_FlameShot + 29,

BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 30,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 31,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 32,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 33,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 34,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 35,
	e_Anm_SfxStop, e_Sfx_Rocket0,
	e_Anm_Sfx, e_Sfx_Shot_Flame, e_SfxPrio_Hero_Shot - 1,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 36,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 37,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 38,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 39,

BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 40,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 41,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 42,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 43,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 44,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 45,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 46,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 47,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 48,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 49,

BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 50,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 51,
e_Anm_End
};
// Boss 2B - Trainée de flames - Fin pour coupure éventuelle.
u64	gAnm_Boss2B_FlameShot_Cut[] =
{
e_AnmKey_Null,
	e_Anm_SfxStop, e_Sfx_Rocket0,
	e_Anm_Sfx, e_Sfx_Shot_Flame, e_SfxPrio_Hero_Shot - 1,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 36,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 37,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 38,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 39,

BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 40,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 41,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 42,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 43,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 44,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 45,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 46,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 47,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 48,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 49,

BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 50,
BOSS2B_T0, e_Spr_Boss2Back_FlameShot + 51,
e_Anm_End
};

// Hair Buster Ribert - Mine.
u64	gAnm_ShotEnemy_HairBusterRibert_Mine[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Shot_BombDrop, e_SfxPrio_Enemy_Shot,
3, e_Spr_Ribert_Mine + 7,
3, e_Spr_Ribert_Mine + 6,
3, e_Spr_Ribert_Mine + 5,
3, e_Spr_Ribert_Mine + 4,
3, e_Spr_Ribert_Mine + 3,
3, e_Spr_Ribert_Mine + 2,
3, e_Spr_Ribert_Mine + 1,
3, e_Spr_Ribert_Mine,
//e_Anm_Goto, (u64)gAnm_ShotEnemy_HairBusterRibert_Mine
e_Anm_Jump, (u64)-16
};
// Hair Buster Ribert - Heat Exhaust.
u64	gAnm_HairBusterRibert_HeatExhaust[] =
{
e_AnmKey_Null,
3, e_Spr_Ribert_EngineExhaust,
3, e_Spr_Ribert_EngineExhaust + 1,
3, e_Spr_Ribert_EngineExhaust + 2,
3, e_Spr_Ribert_EngineExhaust + 3,
3, e_Spr_Ribert_EngineExhaust + 4,
3, e_Spr_Ribert_EngineExhaust + 5,
3, e_Spr_Ribert_EngineExhaust + 6,
128, SPR_NoSprite,
e_Anm_Goto, (u64)gAnm_HairBusterRibert_HeatExhaust
};
// Hair Buster Ribert - Door opening.
u64	gAnm_HairBusterRibert_DoorOpening[] =
{
e_AnmKey_Enemy_Key3,
3, e_Spr_Ribert_DoorOpening,
3, e_Spr_Ribert_DoorOpening + 1,
3, e_Spr_Ribert_DoorOpening + 2,
3, e_Spr_Ribert_DoorOpening + 3,
3, e_Spr_Ribert_DoorOpening + 4,
3, e_Spr_Ribert_DoorOpening + 5,
3, e_Spr_Ribert_DoorOpening + 6,
3, e_Spr_Ribert_DoorOpening + 7,
3, e_Spr_Ribert_DoorOpening + 8,
3, e_Spr_Ribert_DoorOpening + 9,
3, e_Spr_Ribert_DoorOpening + 10,
3, e_Spr_Ribert_DoorOpening + 11,
3, e_Spr_Ribert_DoorOpening + 12,
16, e_Spr_Ribert_DoorOpening + 13,
3, e_Spr_Ribert_DoorOpening + 14,
3, e_Spr_Ribert_DoorOpening + 15,
3, e_Spr_Ribert_DoorOpening + 16,
3, e_Spr_Ribert_DoorOpening + 17,
e_Anm_StepFlag,						// Signal pour afficher le LRAC.
3, e_Spr_Ribert_DoorOpening + 18,
4, e_Spr_Ribert_DoorOpening + 19,
4, e_Spr_Ribert_DoorOpening + 20,
4, e_Spr_Ribert_DoorOpening + 19,
4, e_Spr_Ribert_DoorOpening + 20,
128, e_Spr_Ribert_DoorOpening + 19,
e_Anm_Jump, (u64)-2
};
// Hair Buster Ribert - Door closed.
u64	gAnm_HairBusterRibert_DoorClosed[] =
{
e_AnmKey_Null,
128, e_Spr_Ribert_P1_012,
e_Anm_Jump, (u64)-2
};
// Hair Buster Ribert - Door closing.
u64	gAnm_HairBusterRibert_DoorClosing[] =
{
e_AnmKey_Enemy_Key3,
3 + (7*3), e_Spr_Ribert_DoorOpening + 19,
3, e_Spr_Ribert_DoorOpening + 20,
3, e_Spr_Ribert_DoorOpening + 19,
3, e_Spr_Ribert_DoorOpening + 18,
	3, e_Spr_Ribert_DoorOpening + 20,
	32+3, e_Spr_Ribert_DoorOpening + 19,
	3, e_Spr_Ribert_DoorOpening + 18,
e_Anm_StepFlag,					// Signal pour ne plus afficher le LRAC.
3, e_Spr_Ribert_DoorOpening + 17,
3, e_Spr_Ribert_DoorOpening + 16,
3, e_Spr_Ribert_DoorOpening + 15,
3, e_Spr_Ribert_DoorOpening + 14,
3, e_Spr_Ribert_DoorOpening + 13,
3, e_Spr_Ribert_DoorOpening + 12,
3, e_Spr_Ribert_DoorOpening + 11,
3, e_Spr_Ribert_DoorOpening + 10,
3, e_Spr_Ribert_DoorOpening + 9,
3, e_Spr_Ribert_DoorOpening + 8,
3, e_Spr_Ribert_DoorOpening + 7,
3, e_Spr_Ribert_DoorOpening + 6,
3, e_Spr_Ribert_DoorOpening + 5,
3, e_Spr_Ribert_DoorOpening + 4,
3, e_Spr_Ribert_DoorOpening + 3,
3, e_Spr_Ribert_DoorOpening + 2,
3, e_Spr_Ribert_DoorOpening + 1,
3, e_Spr_Ribert_DoorOpening,
18, e_Spr_Ribert_P1_012,	// Attente pour laisser le temps au dust de claquement de porte de se jouer.
e_Anm_End
};
// Hair Buster Ribert - Door closed, clap - Dust.
u64	gAnm_HairBusterRibert_DoorClose_Dust[] =
{
e_AnmKey_Null,
(22*3) + (7*3) + (32+9), SPR_NoSprite,
e_Anm_Sfx, e_Sfx_MenuClic2, e_SfxPrio_0 + 5,
3, e_Spr_Ribert_DoorDust,
3, e_Spr_Ribert_DoorDust + 1,
3, e_Spr_Ribert_DoorDust + 2,
3, e_Spr_Ribert_DoorDust + 3,
3, e_Spr_Ribert_DoorDust + 4,
3, e_Spr_Ribert_DoorDust + 5,
e_Anm_Kill
};
// Hair Buster Ribert - Soldat qui saute - Prise d'élan.
u64	gAnm_HairBusterRibert_SoldierJumpDive[] =
{
e_AnmKey_Null,
3, e_Spr_RebSoldier_Std_JumpDive,
3, e_Spr_RebSoldier_Std_JumpDive + 1,
3, e_Spr_RebSoldier_Std_JumpDive + 2,
3, e_Spr_RebSoldier_Std_JumpDive + 3,
3, e_Spr_RebSoldier_Std_JumpDive + 4,
3, e_Spr_RebSoldier_Std_JumpDive + 5,
3, e_Spr_RebSoldier_Std_JumpDive + 6,
e_Anm_End
};
// Hair Buster Ribert - Soldat qui saute - Dust.
u64	gAnm_HairBusterRibert_SoldierJumpDive_Dust[] =
{
e_AnmKey_Null,
3, e_Spr_RebSoldier_Std_JumpDive + 7,
3, e_Spr_RebSoldier_Std_JumpDive + 8,
3, e_Spr_RebSoldier_Std_JumpDive + 9,
3, e_Spr_RebSoldier_Std_JumpDive + 10,
36, e_Spr_RebSoldier_Std_JumpDive + 11,		// < Ca laisse le temps au soldat de sortir de l'écran.
e_Anm_Kill
};

// Monoeye - Idle.
u64	gAnm_Monoeye_Idle[] =
{
e_AnmKey_Null,
128, e_Spr_Monoeye_Idle,
e_Anm_Jump, (u64)-2
};
// Monoeye - Apparition.
#define	MONOEYE_T0	4
u64	gAnm_Monoeye_Appear[] =
{
e_AnmKey_Enemy_Key3,
e_Anm_Sfx, e_Sfx_Death_Jellyfish, e_SfxPrio_Enemy_Shot,
MONOEYE_T0, e_Spr_Monoeye_Appear,
MONOEYE_T0, e_Spr_Monoeye_Appear + 1,
e_Anm_Sfx, e_Sfx_Death_Jellyfish, e_SfxPrio_Enemy_Shot,
MONOEYE_T0, e_Spr_Monoeye_Appear + 2,
MONOEYE_T0, e_Spr_Monoeye_Appear + 3,
e_Anm_Sfx, e_Sfx_Death_Jellyfish, e_SfxPrio_Enemy_Shot,
MONOEYE_T0, e_Spr_Monoeye_Appear + 4,
MONOEYE_T0, e_Spr_Monoeye_Appear + 5,
e_Anm_Goto, (u64)gAnm_Monoeye_Idle
};

// Core - Glow.
#define	CORE_T0	8//4
u64	gAnm_Core_Glow[] =
{
e_AnmKey_Null,
CORE_T0, e_Spr_Core_CoreGlow,
CORE_T0, e_Spr_Core_CoreGlow + 1,
CORE_T0, e_Spr_Core_CoreGlow + 2,
CORE_T0, e_Spr_Core_CoreGlow + 3,
CORE_T0, e_Spr_Core_CoreGlow + 4,
e_Anm_Goto, (u64)gAnm_Core_Glow
};

// Core Cannon - Idle.
u64	gAnm_CoreCannon_Idle[] =
{
e_AnmKey_Null,
128, e_Spr_CoreCannon_Idle,
e_Anm_Jump, (u64)-2
};
// Core Cannon - Shot.
#define	CORECANNON_T0	3
u64	gAnm_CoreCannon_Shot[] =
{
e_AnmKey_Enemy_Key3,
CORECANNON_T0, e_Spr_CoreCannon_Shot,
CORECANNON_T0, e_Spr_CoreCannon_Shot + 1,
CORECANNON_T0, e_Spr_CoreCannon_Shot + 2,
CORECANNON_T0, e_Spr_CoreCannon_Shot + 3,
CORECANNON_T0, e_Spr_CoreCannon_Shot + 4,
1, e_Spr_CoreCannon_Shot + 5,				// Shot.
CORECANNON_T0, e_Spr_CoreCannon_Shot + 6,
CORECANNON_T0, e_Spr_CoreCannon_Shot + 7,
CORECANNON_T0, e_Spr_CoreCannon_Shot + 8,
CORECANNON_T0, e_Spr_CoreCannon_Shot + 9,
CORECANNON_T0, e_Spr_CoreCannon_Shot + 10,
CORECANNON_T0, e_Spr_CoreCannon_Shot + 11,
e_Anm_Goto, (u64)gAnm_CoreCannon_Idle
};
// Core Cannon - Bullet.
u64	gAnm_CoreCannon_Bullet[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Shot_Alien, e_SfxPrio_Enemy_Shot,
4, (e_Spr_CoreCannon_Bullet) ^ SPR_Flip_Y,
4, (e_Spr_CoreCannon_Bullet + 1) ^ SPR_Flip_Y,
4, (e_Spr_CoreCannon_Bullet + 2) ^ SPR_Flip_Y,
4, (e_Spr_CoreCannon_Bullet + 3) ^ SPR_Flip_Y,
e_Anm_Jump, (u64)-8
};
// Core Cannon - Bullet Spark Dust.
#define	CORECANNON_T1	3
u64	gAnm_CoreCannon_BulletSparkDust[] =
{
e_AnmKey_Null,
e_Anm_Sfx, e_Sfx_Death_Jellyfish, e_SfxPrio_0 + 5,
CORECANNON_T1, e_Spr_CoreCannon_SparkDust,
CORECANNON_T1, e_Spr_CoreCannon_SparkDust + 1,
CORECANNON_T1, e_Spr_CoreCannon_SparkDust + 2,
CORECANNON_T1, e_Spr_CoreCannon_SparkDust + 3,
CORECANNON_T1, e_Spr_CoreCannon_SparkDust + 4,
CORECANNON_T1, e_Spr_CoreCannon_SparkDust + 5,
CORECANNON_T1, e_Spr_CoreCannon_SparkDust + 6,
CORECANNON_T1, e_Spr_CoreCannon_SparkDust + 7,
CORECANNON_T1, e_Spr_CoreCannon_SparkDust + 8,
CORECANNON_T1, e_Spr_CoreCannon_SparkDust + 9,
CORECANNON_T1, e_Spr_CoreCannon_SparkDust + 10,
CORECANNON_T1, e_Spr_CoreCannon_SparkDust + 11,
e_Anm_Kill
};



