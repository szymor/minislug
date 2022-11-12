
#include "includes.h"
#include "monsters_inc.h"

//=============================================================================
// Mst 50 : Rumi chan.

u32 MstOnScreenNb(u32 nMstType, s32 nBlkOffset);

#define	RUMI_APPEAR_CNT	(FRM_1S * 10)//5)
struct SRumiGen
{
	u8	nPres;		// Presence.
	u32	nCnt;		// Compteur.
};
struct SRumiGen	gRumiGen;

// Raz des compteurs, etc.
void Rumi_Reset(void)
{
	gRumiGen.nPres = 0;
	gRumiGen.nCnt = 0;
}

// Teste s'il faut générer une Rumi chan.
void Rumi_Check(s32 nPosY, u32 nWeapon)
{
	// Si déjà présente, héros en véhicule, le héros a déjà une arme, une capsule est déjà à l'écran...
	if (gRumiGen.nPres ||
		gShoot.nVehicleType != e_HeroVehicle_None_Naked ||
		gShoot.nWeapon != e_Player_Weapon_Gun ||
		((gnFrame & 31) == 0 && MstOnScreenNb(e_Mst4_WeaponCapsule, 0)) ||
		MST_SHOT_COND == 0)
	{
		gRumiGen.nCnt = 0;
		return;
	}

/*
//>tst
char	pTb[8+1] = "00000000";
MyItoA(gRumiGen.nCnt, pTb);
Font_Print(10, 160, pTb, 0);
//<tst
*/

	if (++gRumiGen.nCnt >= RUMI_APPEAR_CNT)
	{
		// Génération de la Rumi !
		u32	nPrm = nWeapon - 1;
		MstAdd(e_Mst50_AikawaRumi0, (gScrollPos.nPosX >> 8) - 48, nPosY, (u8 *)&nPrm, -1);
	}

}

//=================

/*
[mst50] Aikawa Rumi
Drop = 0:3: S_Shotgun - H_Machinegun - F_Flamethrower - R_Rocket - Gas_Tank - Ammo_Box1
tmp = 4:7:
*/

#define	M50_FLEE_SPD	0x200

// Phases.
enum
{
	e_Mst50_Walk = 0,
	e_Mst50_GiveItem,
	e_Mst50_Flee,
};

struct SMst50_AikawaRumi0
{
	s16	nCurSpd;
	u16	nBlkPosXFin;
	u8	nWeaponDrop;	// N° pour Mst4.
};

void Mst50_Init_AikawaRumi0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst50_AikawaRumi0	*pSpe = (struct SMst50_AikawaRumi0 *)pMst->pData;
	u32	nVal;

	nVal = GetBits(0, 3, pData, 0);	// Drop weapon no.
	assert(nVal <= 5);
	pSpe->nWeaponDrop = nVal;

	Enemy_GroundLevel(pMst);
	pMst->nPhase = e_Mst50_Walk;
	pMst->nFlipX = 1;

	pSpe->nBlkPosXFin = (gScrollPos.nPosX + ((SCR_Width / 3) << 8)) >> 12;	// Position finale.

	gRumiGen.nPres = 1;		// Flag présence.

}

struct SMstCommon * MstGetPtrSlot(u32 nSlot);

s32 Mst50_Main_AikawaRumi0(struct SMstCommon *pMst)
{
	struct SMst50_AikawaRumi0	*pSpe = (struct SMst50_AikawaRumi0 *)pMst->pData;
	u32	nSpr;

	nSpr = AnmGetImage(pMst->nAnm);
	nSpr |= (pMst->nFlipX ? SPR_Flip_X : 0);
	pSpe->nCurSpd = 0;

	// Phases.
	switch (pMst->nPhase)
	{
	case e_Mst50_Walk:		// Marche.
		AnmSetIfNew(gAnm_AikawaRumi_Walk, pMst->nAnm);
		// Avance au changement d'image.
		if (AnmCheckNewImgFlag(pMst->nAnm))
			pSpe->nCurSpd = (AnmCheckStepFlag(pMst->nAnm) ? 0x300 : 0x200);		// 2 pixels de base, 3 si flag.
		// Arrive ?
//		if (pMst->nPosX > gScrollPos.nPosX + ((SCR_Width / 3) << 8))
		if (pMst->nPosX >> 12 > pSpe->nBlkPosXFin)
		{
			pMst->nPhase = e_Mst50_GiveItem;
			AnmSet(gAnm_AikawaRumi_GiveItem, pMst->nAnm);
		}
		break;

	case e_Mst50_GiveItem:	// Tombe, donne l'item, salue.
		if (AnmGetKey(pMst->nAnm) != e_AnmKey_Enemy_Key3)
		{
			pMst->nFlipX ^= 1;
			pMst->nPhase = e_Mst50_Flee;
		}
		else
		// On va tester la frame pour donner l'item.
		if (AnmCheckNewImgFlag(pMst->nAnm))
		{
			struct SSprRect rRect;
			if (SprGetRect(nSpr, e_SprRectZone_ShotOrg, &rRect))
			{
				// On génère un monstre Weapon Capsule.
				/*
				[mst4] Weapon Capsule
				Type = 0:3: S_Shotgun - H_Machinegun - F_Flamethrower - R_Rocket - Gas_Tank - Ammo_Box1
				Boxed = 4:4: No - Yes		; Une caisse contient l'item ?
				Location = 5:6: Normal - Space - Underwater - Air
				*/
				u32	nData;
				s32	nSlot;
				nData = pSpe->nWeaponDrop;
				nSlot = MstAdd(e_Mst4_WeaponCapsule, (pMst->nPosX >> 8) + rRect.nX1, (pMst->nPosY >> 8) + rRect.nY1, (u8 *)&nData, -1);
				// On fait sauter la capsule.
				if (nSlot >= 0)
				{
					struct SMstCommon *pGen;
					pGen = MstGetPtrSlot(nSlot);
					pGen->nSpdY = -0x280;
				}
			}
		}
		break;

	case e_Mst50_Flee:		// Fuite.
		AnmSetIfNew(gAnm_AikawaRumi_Flee, pMst->nAnm);
		pSpe->nCurSpd = M50_FLEE_SPD;

// quand screen out :
//return (e_MstState_Asleep);
//void Rumi_Reset(void)
		// Screen out ?
		if (Screen_ObjectOut(pMst->nPosX, pMst->nPosY))
		{
			Rumi_Reset();
			return (e_MstState_Dead);
		}

		break;
	}

	// Move.
	Mst_BasicMove(pMst, pSpe->nCurSpd, 0);

	// Affichage.
	SprDisplay(nSpr, pMst->nPosX >> 8, pMst->nPosY >> 8, e_Prio_Ennemies - 1);

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 51 : How to play.

/*
[mst51] How To Play
prm = 0:7:			; tmp
*/

struct SHTPRecord
{
	u8	nFrm;		// Nb de frames.
	u8	nStick;		// Valeur du clavier. Si b7, les 6 autres donnent un code d'action spéciale.
	u8	nTxtFrm;	// Nb de frames pendant lesquelles on affiche le texte.
	char *pTxt[2];		// Texte à afficher. NULL si rien.
};

struct SHTPRecord	gpHTPScript[] =
{
	{ 1, 0x80, 0, { NULL, NULL } },
	{ 32, e_KbDir_Right, 128, { "Use the stick to move around.", NULL } },
	{ 64, e_KbDir_Left, 0, { NULL, NULL } },
	{ 32, e_KbDir_Right, 0, { NULL, NULL } },
	{ 32, 0, 0, { NULL, NULL } },

	{ 1, e_KbDir_ButtonA, 128, { "Press Button A to fire", "the current weapon." } },
	{ 30, 0, 0, { NULL, NULL } },
	{ 1, e_KbDir_ButtonA, 0, { NULL, NULL } },
	{ 30, 0, 0, { NULL, NULL } },
	{ 16, e_KbDir_Down, 0, { NULL, NULL } },
	{ 1, e_KbDir_ButtonA | e_KbDir_Down, 0, { NULL, NULL } },
	{ 16, e_KbDir_Down, 0, { NULL, NULL } },
	{ 31, 0, 0, { NULL, NULL } },
	{ 1, e_KbDir_ButtonA, 0, { NULL, NULL } },
	{ 30, 0, 0, { NULL, NULL } },
	{ 32, 0, 0, { NULL, NULL } },

	{ 1, 0x80, 0, { NULL, NULL } },
	{ 4, e_KbDir_Right, 128, { "Press Button B to jump.", NULL } },
	{ 1, e_KbDir_Right | e_KbDir_ButtonB, 0, { NULL, NULL } },
	{ 17, e_KbDir_Right, 0, { NULL, NULL } },
	{ 32, 0, 0, { NULL, NULL } },

	{ 1, e_KbDir_Left, 0, { NULL, NULL } },
	{ 1, e_KbDir_ButtonC, 128, { "Press Button C to throw a grenade.", NULL } },
	{ 63, 0, 0, { NULL, NULL } },
	{ 1, e_KbDir_Right, 0, { NULL, NULL } },
	{ 1, e_KbDir_ButtonC, 0, { NULL, NULL } },
	{ 63, 0, 0, { NULL, NULL } },
	{ 32, 0, 0, { NULL, NULL } },

	{ 1, 0x80, 0, { NULL, NULL } },
	{ 32, 0, 128, { "Jump to get in!", NULL } },
	{ 1, e_KbDir_ButtonB, 0, { NULL, NULL } },
	{ 64, 0, 0, { NULL, NULL } },
	{ 32, 0, 0, { NULL, NULL } },

	{ 32, e_KbDir_ButtonA, 128, { "Press Button A to fire the vulcan", "cannon, use the stick to aim." } },
	{ 32, e_KbDir_Up | e_KbDir_ButtonA, 0, { NULL, NULL } },
	{ 16, e_KbDir_Left | e_KbDir_ButtonA, 0, { NULL, NULL } },
	{ 48, e_KbDir_Right | e_KbDir_Up | e_KbDir_ButtonA, 0, { NULL, NULL } },
	{ 4, e_KbDir_Right | e_KbDir_Up, 0, { NULL, NULL } },

	{ 48, e_KbDir_Right, 0, { NULL, NULL } },
	{ 16, 0, 128, { "Press Button C to fire", "the vehicle's main cannon." } },
	{ 1, e_KbDir_ButtonC, 0, { NULL, NULL } },
	{ 15, 0, 0, { NULL, NULL } },
	{ 15, e_KbDir_Right, 0, { NULL, NULL } },
	{ 1, e_KbDir_Right | e_KbDir_ButtonC, 0, { NULL, NULL } },

	{ 1, 0x80, 0, { NULL, NULL } },
	{ 32, 0, 128, { "Press Down and Button B to get out.", NULL } },
	{ 1, e_KbDir_Down | e_KbDir_ButtonB, 0, { NULL, NULL } },
	{ 96, 0, 0, { NULL, NULL } },

	{ 1, 0x80, 0, { NULL, NULL } },
	{ 16, 0, 0, { NULL, NULL } },
	{ 1, e_KbDir_ButtonA, 128, { "Free the hostages.", "Some might release items." } },
	{ 127+32, 0, 0, { NULL, NULL } },

	{ 1, 0x80, 0, { NULL, NULL } },
	{ 32, 0, 0, { NULL, NULL } },
	{ 32, e_KbDir_ButtonA, 128, { "Enjoy some full automatic weapons!", NULL } },
	{ 32+16, e_KbDir_ButtonA | e_KbDir_Left, 0, { NULL, NULL } },
	{ 32+16, e_KbDir_ButtonA | e_KbDir_Right, 0, { NULL, NULL } },
	{ 48, e_KbDir_Right, 0, { NULL, NULL } },

	{ 1, 0x80, 0, { NULL, NULL } },
	{ 16, 0, 0, { NULL, NULL } },
	{ 16, e_KbDir_Down, 0, { NULL, NULL } },
	{ 1, e_KbDir_Down | e_KbDir_ButtonA, 128, { "Ammo boxes contains useful items too.", NULL } },
	{ 16, e_KbDir_Down, 0, { NULL, NULL } },

	{ 1, 0x80, 0, { NULL, NULL } },
	{ 32, e_KbDir_ButtonA, 0, { NULL, NULL } },
	{ 32, e_KbDir_ButtonA | e_KbDir_Up, 0, { NULL, NULL } },
	{ 32, e_KbDir_ButtonA | e_KbDir_Left, 0, { NULL, NULL } },
	{ 48, e_KbDir_Right, 0, { NULL, NULL } },

	{ 1, 0x80, 0, { NULL, NULL } },
	{ 1, 0, 128, { "That's about it for the basic moves.", "Now let's go and kick some ass!" } },
	{ 127+16, 0, 0, { NULL, NULL } },

	{ 1, 0x80, 0, { NULL, NULL } },	// Vers la sortie.

	{ 0, 0, 0, { NULL, NULL } },

};

#define	HTP_TITLE_SZ	10
#define	HTP_TITLE_POSY	(24+8)//+2
struct SHTPTitle
{
	u32	nSpr;
	s32	nPosX, nPosY;
	s32	nSpd, nSpdMax;
};

#define	HTP_BUTTON_NB	4
struct SHTPButton
{
	u32	nAdd;
	u16	nCnt;
};

#define	HTP_TXT_POSY	80
#define	HTP_TXT_INTER	4
#define	HTP_TXT_FNTHT	8

struct SHTPVar
{
	u32	nIdx;	// Index dans le script.
	u8	nFrm;
	u8	nTxtFrm;	// Nb de frames pendant lesquelles on affiche le texte.
	char	*pTxt[2];
	u8	nTxtLines;	// Nb de lignes.
	s16	pTxtLg[2];	// Largeur de chaque ligne.

	s32	nLastBlkX, nLastBlkY;	// Pour recherche de positions particulières.

	struct SHTPTitle	pTitle[HTP_TITLE_SZ];
	u8	nTitleNb;	// Nb de lettres dans le titre.
	u16	nTitleCnt;

	struct SHTPButton	pButtons[HTP_BUTTON_NB];

};
struct SHTPVar	gHTPVar;

// Initialise l'itération suivante du script.
void HTPScript_NextIter(void)
{
	// Script terminé ?
	if (gpHTPScript[gHTPVar.nIdx].nFrm == 0)
	{
		gHTPVar.nFrm = 1;
		gHTPVar.pTxt[0] = gHTPVar.pTxt[1] = NULL;
		gHTPVar.nTxtFrm = 0;
		gShoot.nVehicleAutoPilot = 0x80;
		return;
	}

	// Stick & 0x80 => Action spéciale.
	if (gpHTPScript[gHTPVar.nIdx].nStick & 0x80)
	{
		// Commande 0 : Avance vers la droite jusqu'au prochain bloc de chemin gnd n°16.
		gShoot.nVehicleAutoPilot = e_KbDir_Right | 0x80;	// Vers la droite.

		s32	nNewBlkX, nNewBlkY, nRes;
		nNewBlkX = gShoot.nPlayerPosX >> 12;
		nNewBlkY = gShoot.nPlayerPosY >> 12;
		if (gHTPVar.nLastBlkX != nNewBlkX || gHTPVar.nLastBlkY != nNewBlkY)
		{
			if ((nRes = Map_PathGndGetBlock(nNewBlkX, nNewBlkY)) != -1)
			if (nRes == 16) gHTPVar.nIdx++;		// Ok, on passera à la suite au prochain tour.
		}
		gHTPVar.nFrm = 1;
		return;		// On quitte sans avancer le ptr.
	}

	// Inits.
	gHTPVar.nFrm = gpHTPScript[gHTPVar.nIdx].nFrm;
	if (gpHTPScript[gHTPVar.nIdx].pTxt[0] != NULL)
	{
		gHTPVar.pTxt[0] = gpHTPScript[gHTPVar.nIdx].pTxt[0];
		gHTPVar.pTxtLg[0] = Font_Print(0, 0, gHTPVar.pTxt[0], FONT_NoDisp);
		gHTPVar.pTxt[1] = gpHTPScript[gHTPVar.nIdx].pTxt[1];
		if (gHTPVar.pTxt[1] != NULL) gHTPVar.pTxtLg[1] = Font_Print(0, 0, gHTPVar.pTxt[1], FONT_NoDisp);
		gHTPVar.nTxtLines = (gHTPVar.pTxt[1] == NULL ? 1 : 2);
		gHTPVar.nTxtFrm = 196;//gpHTPScript[gHTPVar.nIdx].nTxtFrm;
	}
	gShoot.nVehicleAutoPilot = gpHTPScript[gHTPVar.nIdx].nStick | 0x80;
	gHTPVar.nIdx++;

}

void Mst51_Init_HowToPlay0(struct SMstCommon *pMst, u8 *pData)
{
	// Init script.
	gHTPVar.nLastBlkX = gShoot.nPlayerPosX >> 12;
	gHTPVar.nLastBlkY = gShoot.nPlayerPosY >> 12;
	gHTPVar.nIdx = 0;
	gHTPVar.pTxt[0] = gHTPVar.pTxt[1] = NULL;
	HTPScript_NextIter();

	// Init titre.
	struct SSprite	*pSpr;
	char	*pTitle = "HOW TO PLAY";
	char	*pPtr = pTitle;
	gHTPVar.nTitleNb = 0;
	u32	nPosX = 0;
	while (*pPtr && gHTPVar.nTitleNb < HTP_TITLE_SZ)
	{
		if (*pPtr != ' ')
		{
			gHTPVar.pTitle[gHTPVar.nTitleNb].nSpr = e_Spr_FontMission_Letters + (u32)(*pPtr - 'A');
			gHTPVar.pTitle[gHTPVar.nTitleNb].nPosX = nPosX;
			gHTPVar.pTitle[gHTPVar.nTitleNb].nPosY = -(gHTPVar.nTitleNb * 12) - (40 * 2);
			gHTPVar.pTitle[gHTPVar.nTitleNb].nPosY <<= 8;
			gHTPVar.pTitle[gHTPVar.nTitleNb].nSpd = 0x300;
			gHTPVar.pTitle[gHTPVar.nTitleNb].nSpdMax = gHTPVar.pTitle[gHTPVar.nTitleNb].nSpd;
			//
			pSpr = SprGetDesc(gHTPVar.pTitle[gHTPVar.nTitleNb].nSpr);
			nPosX += pSpr->nLg + 1;
			//
			gHTPVar.nTitleNb++;
		}
		else
			nPosX += 10;	// Taille de l'espace.
		pPtr++;
	}
	// Centrage.
	nPosX = (SCR_Width - nPosX) / 2;
	u32	i;
	for (i = 0; i < gHTPVar.nTitleNb; i++) gHTPVar.pTitle[i].nPosX += nPosX;
	// Cnt.
	gHTPVar.nTitleCnt = (-((gHTPVar.pTitle[gHTPVar.nTitleNb - 1].nPosY) >> 8) + 128);

	// Init buttons.
	for (i = 0; i < HTP_BUTTON_NB; i++)
	{
		gHTPVar.pButtons[i].nAdd = 0;
		gHTPVar.pButtons[i].nCnt = 0;
	}

}

#define	M51_TITLE_ACC	0x20
s32 Mst51_Main_HowToPlay0(struct SMstCommon *pMst)
{
	u32	i;
	s32	v;

	// Titre.
	for (i = 0; i < gHTPVar.nTitleNb; i++)
	{
		// Arrivée.
		if (gHTPVar.pTitle[i].nSpdMax)
		{
			// Spd.
			if (gHTPVar.pTitle[i].nPosY >> 8 < 0)
			{
				gHTPVar.pTitle[i].nSpd += M51_TITLE_ACC;
				if (gHTPVar.pTitle[i].nSpd >= gHTPVar.pTitle[i].nSpdMax) gHTPVar.pTitle[i].nSpd = gHTPVar.pTitle[i].nSpdMax;
			}
			else
			{
				gHTPVar.pTitle[i].nSpd -= M51_TITLE_ACC;
				if (gHTPVar.pTitle[i].nSpd <= gHTPVar.pTitle[i].nSpdMax) gHTPVar.pTitle[i].nSpd = gHTPVar.pTitle[i].nSpdMax;
			}
			// Déplacement.
			v = gHTPVar.pTitle[i].nPosY;
			gHTPVar.pTitle[i].nPosY += gHTPVar.pTitle[i].nSpd;
			// Changement de sens ?
			if ((gHTPVar.pTitle[i].nPosY ^ v) & 0x80000000)
			{
				gHTPVar.pTitle[i].nSpdMax = -gHTPVar.pTitle[i].nSpdMax;
				gHTPVar.pTitle[i].nSpdMax += -(SGN(gHTPVar.pTitle[i].nSpdMax) << 8);
				if (gHTPVar.pTitle[i].nSpdMax == 0) gHTPVar.pTitle[i].nPosY = 0;
			}
		}
		// Affichage.
		SprDisplayAbsolute(gHTPVar.pTitle[i].nSpr, gHTPVar.pTitle[i].nPosX, (gHTPVar.pTitle[i].nPosY >> 8) + HTP_TITLE_POSY, e_Prio_HUD);
	}
	// Redémarre le truc de temps en temps.
	gHTPVar.nTitleCnt--;
	if ((gHTPVar.nTitleCnt & 3) == 0)
	if (gHTPVar.nTitleCnt >> 2 <= gHTPVar.nTitleNb)
	{
		if (gHTPVar.nTitleCnt)
		{
			gHTPVar.pTitle[gHTPVar.nTitleNb - (gHTPVar.nTitleCnt >> 2)].nSpd = -0x200;//0x180;
			gHTPVar.pTitle[gHTPVar.nTitleNb - (gHTPVar.nTitleCnt >> 2)].nSpdMax = -0x400;
		}
		else
			gHTPVar.nTitleCnt = 256 + (4 * gHTPVar.nTitleNb);
	}


	// Avance dans le script.
	if (--gHTPVar.nFrm == 0) HTPScript_NextIter();

	gHTPVar.nLastBlkX = gShoot.nPlayerPosX >> 12;
	gHTPVar.nLastBlkY = gShoot.nPlayerPosY >> 12;


	// Affichage du texte.
	if (gHTPVar.pTxt[0] != NULL)
	{
		Font_Print((SCR_Width - gHTPVar.pTxtLg[0]) / 2, HTP_TXT_POSY - ((gHTPVar.nTxtLines - 1) * ((HTP_TXT_INTER + HTP_TXT_FNTHT) / 2)), gHTPVar.pTxt[0], 0);
		if (gHTPVar.pTxt[1] != NULL) Font_Print((SCR_Width - gHTPVar.pTxtLg[1]) / 2, HTP_TXT_POSY + HTP_TXT_INTER, gHTPVar.pTxt[1], 0);
		if (--gHTPVar.nTxtFrm == 0) gHTPVar.pTxt[0] = NULL;
	}


	// Boutons.
	for (i = 0; i < HTP_BUTTON_NB; i++)
	{
		static	s16	pnBtPosX[HTP_BUTTON_NB] = { 70, SCR_Width - 70 - (48 * 2), SCR_Width - 70 - 48, SCR_Width - 70 };
		static	u32 pnBtSpr[HTP_BUTTON_NB] = { e_Spr_HowToPlay_Stick, e_Spr_HowToPlay_ButtonA, e_Spr_HowToPlay_ButtonB, e_Spr_HowToPlay_ButtonC };
		static	u8	pnBtMask[HTP_BUTTON_NB] = { 0, e_KbDir_ButtonA, e_KbDir_ButtonB, e_KbDir_ButtonC };

		// Etat des boutons.
		if (gShoot.nVehicleAutoPilot & pnBtMask[i])
		{
			gHTPVar.pButtons[i].nAdd = 1;
			gHTPVar.pButtons[i].nCnt = 16+16;
		}
		// Etat du stick.
		if (i == 0)
		{
			if (gShoot.nVehicleAutoPilot & (e_KbDir_Up | e_KbDir_Right | e_KbDir_Down | e_KbDir_Left))
			{	// On va donner la priorité à la première direction valide (je n'ai pas les sprites des diagonales).
				static	u8	pnStickMask[4] = { e_KbDir_Right, e_KbDir_Left, e_KbDir_Up, e_KbDir_Down };
				static	u8	pnStickAdd[4] = { 2, 4, 1, 3 };
				u32	j;
				for (j = 0; j < 4; j++)
				{
					if (gShoot.nVehicleAutoPilot & pnStickMask[j])
					{
						gHTPVar.pButtons[i].nAdd = pnStickAdd[j];
						gHTPVar.pButtons[i].nCnt = 16;
						break;
					}
				}
			}
		}
		else
			gHTPVar.pButtons[i].nAdd = (gHTPVar.pButtons[i].nCnt + 15) / 16;	// Pour les boutons.

		// Dec compteurs.
		if (gHTPVar.pButtons[i].nCnt)
		if (--gHTPVar.pButtons[i].nCnt == 0) gHTPVar.pButtons[i].nAdd = 0;

		// Affichage.
		SprDisplayAbsolute(pnBtSpr[i] + gHTPVar.pButtons[i].nAdd, pnBtPosX[i], SCR_Height - 21, 0x100 + e_Prio_HUD);
	}


	// Le joueur appuie sur un bouton ? => On quitte.
	if (gVar.pKeysSDL[SDLK_RETURN] || gVar.pKeysSDL[SDLK_SPACE] ||
//		gVar.pKeysSDL[KB_BUTTONA] || gVar.pKeysSDL[KB_BUTTONB] || gVar.pKeysSDL[KB_BUTTONC])
		gVar.pKeysSDL[gMSCfg.pKeys[e_CfgKey_ButtonA]] || gVar.pKeysSDL[gMSCfg.pKeys[e_CfgKey_ButtonB]] || gVar.pKeysSDL[gMSCfg.pKeys[e_CfgKey_ButtonC]])
	if (Transit2D_CheckEnd())	// Pas pendant l'ouverture !
	{
		gpMstQuestItems[MST_QUEST_ITEM_NEXT_LEVEL] |= 1;	// Item de quête : Fin de niveau.
	}

	return (e_MstState_Managed);
}

//=============================================================================
// Mst 52 : Credits.
// Note : On utilise le lev2. Si c'est pour les crédits, scroll bloqué, crédits. Sinon lev normal.

/*
[mst52] Credits
prm = 0:7:			; tmp
*/

// Phases.
enum
{
	e_Mst52_Death = 0,
	e_Mst52_NextLines,
	e_Mst52_Wait,
	e_Mst52_TxtComeIn,
	e_Mst52_TxtGoOut,
};

#define	MST52_LINES	12
#define	MST52_INTERLN	10
#define	MST52_SINIDX	64
#define	MST52_TIMER	128
//#define	MST52_HELICO_TIMER	128
struct SMst52_Credits0
{
	u8	nCurIdx;
	u8	nSens;
	s16	pnLnSz[MST52_LINES];
	u8	pnSinIdx[MST52_LINES];
	u16	nTimer;

//	u8	nHelicoPhase;
//	u8	nHelicoCnt;
};

void Mst52_Init_Credits0(struct SMstCommon *pMst, u8 *pData)
{
	struct SMst52_Credits0	*pSpe = (struct SMst52_Credits0 *)pMst->pData;

	if (gGameVar.nGenLevel != MISSIONOFFS_CREDITS)
	{	// Ce n'est pas le niveau pour les crédits.
		pMst->nPhase = e_Mst52_Death;
		return;
	}

	// On est bien au moment des crédits.
	pMst->nPhase = e_Mst52_NextLines;
	gpMstQuestItems[MST_QUEST_ITEM_MST32]++;		// Scroll stop.

	gShoot.nVehicleAutoPilot = 0x80;
	gShoot.nPlayerPosX = gScrollPos.nPosX + ((SCR_Width / 2) << 8);

	pSpe->nCurIdx = 0;
	pSpe->nSens = 1;

//	pSpe->nHelicoPhase = 0;
//	pSpe->nHelicoCnt = MST52_HELICO_TIMER;

}

struct SCreditRecord
{
	u8	nLines;
	u16	nTimer;
	char *gpTxt[MST52_LINES];
};

static struct SCreditRecord gpCredits[] =
{
	{
		4, MST52_TIMER, {
"- The Mini Slug Project -",
"",
"A humble tribute to",
"the Metal Slug series",

"",
"",
"",
"",

"",
"",
"",
"",
		}
	},

/*
	{
		10, MST52_TIMER * 2, {
"- All code & everything else -",
"",
"Clement CORDE",
"",

"",
"",

"- Graphics -",
"",
"All graphical content",
"(c) Nazca Corp., SNK and PlayMore.",

"",
"",
		}
	},
*/

	{
		9, MST52_TIMER * 2, {
"- All code & everything else -",
"",
"Clement CORDE",
"",

"",
"",

"- Sound system -",
"",
"ST-Sound by Leonard/OXG",
"",

"",
"",
		}
	},

	{
		10, MST52_TIMER * 2, {
"- Graphics -",
"",
"All graphical content",
"(c) Nazca Corp., SNK and PlayMore.",

"",
"",
"",

"- ST era music -",
"",
"David WHITTAKER",
"",

"",
		}
	},

	{
		12, MST52_TIMER * 4, {
"- Using rips by -",
"",
"Gussprint, Random Rebel Soldier,",
"The Mad Soldier, C2000MC/Futaokuu,",

"Random Talking Bush, Kevin Huff,",
"Grey Wolf Leader, Goemar, @$$h0l3,",
"Magma Dragoon, AzRaezel/Dan, Grim,",
"The_Protagonist, Tk big_mickey_k,",

"Crazy Slug, Xezan,",
"and some unsigned rips.",
"",
"Thank you guys, you did an amazing job.",
		}
	},

	{
		1, MST52_TIMER * 2, {
"Thank you for playing!",
"",
"",
"",

"",
"",
"",
"",

"",
"",
"",
"",
		}
	},
};

// Prepare le prochain affichage, calcule la taille des lignes, etc...
void M52_Prepare(struct SMstCommon *pMst)
{
	struct SMst52_Credits0	*pSpe = (struct SMst52_Credits0 *)pMst->pData;
	u32	i;

	// Largeur des lignes.
	for (i = 0; i < MST52_LINES; i++)
	{
		pSpe->pnLnSz[i] = Font_Print(0, 0, gpCredits[pSpe->nCurIdx].gpTxt[i], FONT_NoDisp);
		pSpe->pnSinIdx[i] = MST52_SINIDX;	// RAZ des index de sin au passage.
	}
	// Changement de sens.
	pSpe->nSens ^= 1;
	// RAZ Timer.
	pSpe->nTimer = gpCredits[pSpe->nCurIdx].nTimer;
}

s32 Mst52_Main_Credits0(struct SMstCommon *pMst)
{
	struct SMst52_Credits0	*pSpe = (struct SMst52_Credits0 *)pMst->pData;
	u32	i;
	u32	nPrint = 0;

	if (pMst->nPhase == e_Mst52_Death) return (e_MstState_Dead);		// Ce n'est pas le niveau des crédits.

	switch (pMst->nPhase)
	{
	case e_Mst52_Wait:		// Attente avec le texte affiché sur l'écran.
		nPrint = 1;

		if (pSpe->nTimer) pSpe->nTimer--;

		if (pSpe->nTimer == 0)
		{
			if (pSpe->nCurIdx + 1 >= NBELEM(gpCredits))
			{	// Fin, avec le dernier texte à l'écran.
				gpMstQuestItems[MST_QUEST_ITEM_NEXT_LEVEL] |= 1;	// Item de quête : Fin de niveau.
			}
			else
			{	// Le texte s'en va.
				pMst->nPhase = e_Mst52_TxtGoOut;
			}
		}
/*
if (gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]])
	gVar.pKeys[gMSCfg.pKeys[e_CfgKey_ButtonA]] = 0;
*/
		break;

	case e_Mst52_NextLines:			// Préparation du bloc suivant.
		if (Transit2D_CheckEnd() == 0) break;	// Pas pendant l'ouverture !

		M52_Prepare(pMst);
		pMst->nPhase = e_Mst52_TxtComeIn;
		break;

	case e_Mst52_TxtComeIn:			// Le texte arrive.
		nPrint = 1;

		// Décrémentation des sin idx des lignes.
		for (i = 0; i < gpCredits[pSpe->nCurIdx].nLines; i++)
		{
			if (i == 0 || pSpe->pnSinIdx[i - 1] < MST52_SINIDX - 4)
				if (pSpe->pnSinIdx[i]) pSpe->pnSinIdx[i]--;
		}

		// Dernière ligne arrivée ?
		if (pSpe->pnSinIdx[gpCredits[pSpe->nCurIdx].nLines - 1] == 0)
		{
			pMst->nPhase = e_Mst52_Wait;
		}
		break;

	case e_Mst52_TxtGoOut:			// Le texte repart.
		// Dernière ligne arrivée ?
		if (pSpe->pnSinIdx[gpCredits[pSpe->nCurIdx].nLines - 1] >= MST52_SINIDX)
		{
			if (pSpe->nCurIdx + 1 >= NBELEM(gpCredits))		// Laissé pour sécurité.
			{
				// Fin.
				gpMstQuestItems[MST_QUEST_ITEM_NEXT_LEVEL] |= 1;	// Item de quête : Fin de niveau.
			}
			else
			{
				// Prochain bloc.
				pSpe->nCurIdx += 1;
				pMst->nPhase = e_Mst52_NextLines;
			}
		}
		else
		{
			// Incrémentation des sin idx des lignes.
			for (i = 0; i < gpCredits[pSpe->nCurIdx].nLines; i++)
			{
				if (i == 0 || pSpe->pnSinIdx[i - 1] > 4)
					if (pSpe->pnSinIdx[i] < MST52_SINIDX) pSpe->pnSinIdx[i]++;
			}

			nPrint = 1;
		}
		break;

	default:
		break;
	}

	// Affichage des lignes en cours.
	if (nPrint)
	{
		s32	nOffset;
		s32	nCtr;
		nCtr = ((MST52_LINES - gpCredits[pSpe->nCurIdx].nLines) * MST52_INTERLN) / 2;
		for (i = 0; i < gpCredits[pSpe->nCurIdx].nLines; i++)
		{
			nOffset = 0x100 - gVar.pCos[pSpe->pnSinIdx[i]];
			nOffset = (nOffset * 0x140) >> 8;
			if (pSpe->nSens) nOffset = -nOffset;

			Font_Print((SCR_Width / 2) - (pSpe->pnLnSz[i] / 2) + nOffset, 10 + nCtr + (MST52_INTERLN * i), gpCredits[pSpe->nCurIdx].gpTxt[i], 0);
		}

		// Position de l'hélico.
		gShoot.nVehicleAutoPilot = 0x80;
		// Y.
		nOffset = gScrollPos.nPosY >> 8;
		nOffset += gpCredits[pSpe->nCurIdx].nLines * MST52_INTERLN;
		nOffset += nCtr + 64 + 16;
		s32	nAbs = ABS((gShoot.nPlayerPosY >> 8) - nOffset);
		if (gShoot.nPlayerPosY >> 8 < nOffset)
		{
			if (nAbs > 20 || (nAbs > 8 && gShoot.nPlayerSpdY <= 0x80))
			gShoot.nVehicleAutoPilot |= e_KbDir_Down;
		}
		if (gShoot.nPlayerPosY >> 8 > nOffset)
		{
			if (nAbs > 20 || (nAbs > 8 && gShoot.nPlayerSpdY >= -0x80))
			gShoot.nVehicleAutoPilot |= e_KbDir_Up;
		}
/*
// Bof...
		// X.
		switch (pSpe->nHelicoPhase)
		{
		case 0:
		case 2:
			if (pMst->nPhase == e_Mst52_TxtComeIn)
//			if (--pSpe->nHelicoCnt == 0)
			{
				pSpe->nHelicoCnt = MST52_HELICO_TIMER;
				pSpe->nHelicoPhase++;
			}
			break;

		case 1:
			if (gShoot.nPlayerPosX < gScrollPos.nPosX + (((SCR_Width * 2) / 3) << 8))
				gShoot.nVehicleAutoPilot |= e_KbDir_Right;
			else
			{
				pSpe->nHelicoCnt = 16;
				pSpe->nHelicoPhase = 3;//++;
			}
			break;

		case 3:
			if (gShoot.nPlayerPosX > gScrollPos.nPosX + (((SCR_Width * 1) / 3) << 8))
				gShoot.nVehicleAutoPilot |= e_KbDir_Left;
			else
				pSpe->nHelicoPhase = 0;
			break;
		}
*/

	}

	return (e_MstState_Managed);
}

//=============================================================================

/*
struct SAAF { u8	nTb0[80]; };
assert(sizeof(struct SAAF) < MST_COMMON_DATA_SZ);
*/

#ifndef NDEBUG
// Debug, vérification de la taille des structures.
void Mst50CheckStructSizes(void)
{
	assert(sizeof(struct SMst50_AikawaRumi0) < MST_COMMON_DATA_SZ);
	// Pas de 51.
	assert(sizeof(struct SMst52_Credits0) < MST_COMMON_DATA_SZ);

}
#endif

