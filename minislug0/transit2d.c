
#include "includes.h"

#define	T0_PTS2D_NB	12
#define	T1_PTS2D_NB	48

#define	PTS2D_NB_MAX	T1_PTS2D_NB		// !!!

// Etat de la transition.
enum
{
	e_Transit2D_OpeningEnded = 0,		// Ouverture terminée, plus de tracé.
	e_Transit2D_Opening,
	e_Transit2D_ClosingEnded,			// Fermeture terminée, on continue les tracés, mais plus de "déplacement".
	e_Transit2D_Closing,

};

struct	STransit2D
{
	u8	nTransitNo;
	u8	nAngle;
	u32	nState;
	s32	nOffset;		// Ecartement des points.
	s32	nMultiplier;	// Zoom factor.

	// Pour rotation 2D :
	struct	S2DPoint	pPtsOrg[PTS2D_NB_MAX];		// Les points à faire tourner.
	struct	S2DPoint	pPtsRot[PTS2D_NB_MAX];		// Les points après rotation.

	// Pour tracé des faces :
	s32	pLnBufL[SCR_Height];	// Buffer de lignes.
	s32	pLnBufR[SCR_Height];	// On remplira en x de min à max.
	s32	nLnYMin, nLnYMax;

};

struct	STransit2D	gTransit2D;

//=============================================================================
//=============================================================================
// Routines générales.

// Reset du buffer de lignes.
void LinesBufferClear(void)
{
	// Y min et max.
	gTransit2D.nLnYMin = SCR_Height;
	gTransit2D.nLnYMax = -1;

}

// "Tracé" de la ligne : Remplissage du buffer de lignes.
#define	LN_CLIP_TOP		0
#define	LN_CLIP_BOTTOM	(SCR_Height - 1)
#define	LN_CLIP_LEFT	0
#define	LN_CLIP_RIGHT	(SCR_Width - 1)
void LineBuf(s32 nX1, s32 nY1, s32 nX2, s32 nY2)
{
	s32	*pBuf;
	s32	y;

	// Cas particulier de la ligne horizontale => On ne la trace pas, le cas sera règlé par les autres lignes de la face.
	if (nY1 == nY2) return;

	// La ligne monte ou descend ?
	if (nY2 > nY1)
	{
		// Descend. On trace du côté droit.
		if (nY2 < LN_CLIP_TOP || nY1 > LN_CLIP_BOTTOM) return;	// Complètement en dehors.
		pBuf = gTransit2D.pLnBufR;
	}
	else
	{
		// Monte. On trace du côté gauche.
		// + Pour que les points soient exactement au mêmes endroits si deux lignes sont superposées, on fait descendre la ligne.
		if (nY1 < LN_CLIP_TOP || nY2 > LN_CLIP_BOTTOM) return;	// Complètement en dehors.
		pBuf = gTransit2D.pLnBufL;
		// Swap.
		s32	nTmp;
		nTmp = nX1; nX1 = nX2; nX2 = nTmp;
		nTmp = nY1; nY1 = nY2; nY2 = nTmp;
	}

	s32	nDY;
	s32	nIncX;

	nDY = abs(nY2 - nY1);
	nIncX = ((nX2 - nX1) << 8) / nDY;
	nX1 <<= 8;
	nX1 |= 0x7F;

	// Limites haut et bas.
	if (nY1 < LN_CLIP_TOP)
	{
		nX1 += nIncX * (LN_CLIP_TOP - nY1 + 0);
		nY1 = LN_CLIP_TOP;
	}
	if (nY2 > LN_CLIP_BOTTOM) nY2 = LN_CLIP_BOTTOM;

	// Note les Y min et max.
	if (nY1 < gTransit2D.nLnYMin) gTransit2D.nLnYMin = nY1;
	if (nY2 > gTransit2D.nLnYMax) gTransit2D.nLnYMax = nY2;

	// Tracé (remplissage du buffer).
	for (y = nY1; y <= nY2; y += 1)
	{
		pBuf[y] = nX1 >> 8;
		nX1 += nIncX;
	}

}

// Version avec 2 compare par ligne.
void FaceDraw(void)
{
	u16 *pScr;
	u32 i;
	s32	nMinX, nMaxX, nLg;
	u32	nScrLg = gVar.pScreen->pitch / sizeof(u16);

	if (gTransit2D.nLnYMin > gTransit2D.nLnYMax) return;	// Rien à tracer.

//pitch//	pScr = (u16 *)gVar.pScreen->pixels + (gTransit2D.nLnYMin * SCR_Width);
	pScr = (u16 *)(gVar.pScreen->pixels + (gTransit2D.nLnYMin * gVar.pScreen->pitch));
	for (i = gTransit2D.nLnYMin; i <= gTransit2D.nLnYMax; i++)
	{
		nMinX = gTransit2D.pLnBufL[i];
		if (nMinX < LN_CLIP_LEFT) nMinX = LN_CLIP_LEFT;
		nMaxX = gTransit2D.pLnBufR[i];
		if (nMaxX > LN_CLIP_RIGHT) nMaxX = LN_CLIP_RIGHT;
		nLg = nMaxX - nMinX;

		while (nLg >= 0)
		{
			*(pScr + (nMinX++)) = 0;//255;
			nLg--;
		}
		pScr += nScrLg;
	}

}

//=============================================================================
//=============================================================================
// Transition 0 - Etoile.

#define	T0_UNIT2D	(20 * 256)

struct	SFace3
{
	u32	nPt0, nPt1, nPt2;
};

#define	T0_FACES_NB	4

struct SFace3	gT0_Faces[T0_FACES_NB] =
{
	{ 0, 1, 2 },
	{ 3, 4, 5 },
	{ 6, 7, 8 },
	{ 9, 10, 11 },
};

u8	gT0_Offsets[T0_FACES_NB] =	// Décalages x et y sur les faces.
{
	64, 32, 0, -32,
};

struct S2DPoint	gT0_PtsOrg[T0_PTS2D_NB] =		// Motif original.
{
	{ 0 * T0_UNIT2D, 0 * T0_UNIT2D },
	{ 0 * T0_UNIT2D, -10 * T0_UNIT2D },
	{ 10 * T0_UNIT2D, -10 * T0_UNIT2D },

	{ 0 * T0_UNIT2D, 0 * T0_UNIT2D },
	{ 7 * T0_UNIT2D, -7 * T0_UNIT2D },
	{ 14 * T0_UNIT2D, 0 * T0_UNIT2D },

	{ 0 * T0_UNIT2D, 0 * T0_UNIT2D },
	{ 10 * T0_UNIT2D, 0 * T0_UNIT2D },
	{ 10 * T0_UNIT2D, 10 * T0_UNIT2D },

	{ 0 * T0_UNIT2D, 0 * T0_UNIT2D },
	{ 7 * T0_UNIT2D, 7 * T0_UNIT2D },
	{ 0 * T0_UNIT2D, 14 * T0_UNIT2D },
};

//=============================================================================

// Déplace les points et les place dans le buffer de points à touner.
void T0_MovePoints(void)
{
	u32	i, k;

	k = 0;
	for (i = 0; i < T0_FACES_NB; i++)
	{
		gTransit2D.pPtsOrg[k].nX = (gT0_PtsOrg[gT0_Faces[i].nPt0].nX >> 8) + ((gVar.pCos[(gT0_Offsets[i] + 16) & 0xFF] * gTransit2D.nOffset) >> 8);
		gTransit2D.pPtsOrg[k].nY = (gT0_PtsOrg[gT0_Faces[i].nPt0].nY >> 8) + ((gVar.pSin[(gT0_Offsets[i] + 16) & 0xFF] * gTransit2D.nOffset) >> 8);
		k++;
		gTransit2D.pPtsOrg[k].nX = (gT0_PtsOrg[gT0_Faces[i].nPt1].nX >> 8) + ((gVar.pCos[gT0_Offsets[i]] * gTransit2D.nOffset) >> 8);
		gTransit2D.pPtsOrg[k].nY = (gT0_PtsOrg[gT0_Faces[i].nPt1].nY >> 8) + ((gVar.pSin[gT0_Offsets[i]] * gTransit2D.nOffset) >> 8);
		k++;
		gTransit2D.pPtsOrg[k].nX = (gT0_PtsOrg[gT0_Faces[i].nPt2].nX >> 8) + ((gVar.pCos[gT0_Offsets[i]] * gTransit2D.nOffset) >> 8);
		gTransit2D.pPtsOrg[k].nY = (gT0_PtsOrg[gT0_Faces[i].nPt2].nY >> 8) + ((gVar.pSin[gT0_Offsets[i]] * gTransit2D.nOffset) >> 8);
		k++;
	}

}

// Tracé des faces.
void T0_DrawFaces(void)
{
	u32	i;

	SDL_LockSurface(gVar.pScreen);

	for (i = 0; i < T0_FACES_NB; i++)
	{
		// Face normale.
		LinesBufferClear();
		LineBuf(gTransit2D.pPtsRot[gT0_Faces[i].nPt0].nX + (SCR_Width / 2), gTransit2D.pPtsRot[gT0_Faces[i].nPt0].nY + (SCR_Height / 2), gTransit2D.pPtsRot[gT0_Faces[i].nPt1].nX + (SCR_Width / 2), gTransit2D.pPtsRot[gT0_Faces[i].nPt1].nY + (SCR_Height / 2));
		LineBuf(gTransit2D.pPtsRot[gT0_Faces[i].nPt1].nX + (SCR_Width / 2), gTransit2D.pPtsRot[gT0_Faces[i].nPt1].nY + (SCR_Height / 2), gTransit2D.pPtsRot[gT0_Faces[i].nPt2].nX + (SCR_Width / 2), gTransit2D.pPtsRot[gT0_Faces[i].nPt2].nY + (SCR_Height / 2));
		LineBuf(gTransit2D.pPtsRot[gT0_Faces[i].nPt2].nX + (SCR_Width / 2), gTransit2D.pPtsRot[gT0_Faces[i].nPt2].nY + (SCR_Height / 2), gTransit2D.pPtsRot[gT0_Faces[i].nPt0].nX + (SCR_Width / 2), gTransit2D.pPtsRot[gT0_Faces[i].nPt0].nY + (SCR_Height / 2));
		FaceDraw();
		// Symétrique.
		LinesBufferClear();
		LineBuf(-gTransit2D.pPtsRot[gT0_Faces[i].nPt0].nX + (SCR_Width / 2), -gTransit2D.pPtsRot[gT0_Faces[i].nPt0].nY + (SCR_Height / 2), -gTransit2D.pPtsRot[gT0_Faces[i].nPt1].nX + (SCR_Width / 2), -gTransit2D.pPtsRot[gT0_Faces[i].nPt1].nY + (SCR_Height / 2));
		LineBuf(-gTransit2D.pPtsRot[gT0_Faces[i].nPt1].nX + (SCR_Width / 2), -gTransit2D.pPtsRot[gT0_Faces[i].nPt1].nY + (SCR_Height / 2), -gTransit2D.pPtsRot[gT0_Faces[i].nPt2].nX + (SCR_Width / 2), -gTransit2D.pPtsRot[gT0_Faces[i].nPt2].nY + (SCR_Height / 2));
		LineBuf(-gTransit2D.pPtsRot[gT0_Faces[i].nPt2].nX + (SCR_Width / 2), -gTransit2D.pPtsRot[gT0_Faces[i].nPt2].nY + (SCR_Height / 2), -gTransit2D.pPtsRot[gT0_Faces[i].nPt0].nX + (SCR_Width / 2), -gTransit2D.pPtsRot[gT0_Faces[i].nPt0].nY + (SCR_Height / 2));
		FaceDraw();
	}

	SDL_UnlockSurface(gVar.pScreen);

}

//=============================================================================

// Initialisation de l'ouverture.
void T0_InitOpening(void)
{
	gTransit2D.nState = e_Transit2D_Opening;
	gTransit2D.nAngle = 8;
	gTransit2D.nOffset = 0;
	gTransit2D.nMultiplier = 0x0100;

}

// Initialisation de la fermeture.
void T0_InitClosing(void)
{
	gTransit2D.nState = e_Transit2D_Closing;
	gTransit2D.nAngle = 256-64-10;//256-64;
	gTransit2D.nOffset = 84;
	gTransit2D.nMultiplier = 0x0250;

}

// Gestion de la transition.
void T0_Manage(void)
{
	T0_MovePoints();
	Rotation2D(T0_PTS2D_NB, gTransit2D.pPtsOrg, gTransit2D.pPtsRot, gTransit2D.nAngle, gTransit2D.nMultiplier);
	T0_DrawFaces();

	if (gTransit2D.nState == e_Transit2D_ClosingEnded) return;	// Pour continuer les tracés une fois l'écran fermé (pour éviter le glitch d'une frame dans les menus).

	if (gTransit2D.nState == e_Transit2D_Opening)
	{
		if (gTransit2D.nOffset == 86)
		{
			gTransit2D.nState = e_Transit2D_OpeningEnded;
			return;
		}
		gTransit2D.nAngle += 1;
		gTransit2D.nMultiplier += 0x08;
		gTransit2D.nOffset += 2;
	}
	else
	{
		if (gTransit2D.nOffset == 0)
		{
			gTransit2D.nState = e_Transit2D_ClosingEnded;
			return;
		}
		gTransit2D.nAngle += 2;
		gTransit2D.nMultiplier -= 0x08;
		gTransit2D.nOffset -= 2;
	}

}

//=============================================================================
//=============================================================================
// Transition 1 - Dents.

#define	T1_UNIT2D	(20)

#define	T1_FACES_NB	22

struct S2DPoint	gT1_PtsOrg[T1_PTS2D_NB] =		// Motif original.
{
	// Partie du haut.
	{ -8.5 * T1_UNIT2D, -1 * T1_UNIT2D },
	{ -8.5 * T1_UNIT2D, -7 * T1_UNIT2D },
	{ -6.5 * T1_UNIT2D, -1 * T1_UNIT2D },
	{ -6.5 * T1_UNIT2D, -7 * T1_UNIT2D },
	{ -5.5 * T1_UNIT2D, 1 * T1_UNIT2D },
	{ -5.5 * T1_UNIT2D, -7 * T1_UNIT2D },
	{ -3.5 * T1_UNIT2D, 1 * T1_UNIT2D },
	{ -3.5 * T1_UNIT2D, -7 * T1_UNIT2D },
	{ -2.5 * T1_UNIT2D, -1 * T1_UNIT2D },
	{ -2.5 * T1_UNIT2D, -7 * T1_UNIT2D },
	{ -0.5 * T1_UNIT2D, -1 * T1_UNIT2D },
	{ -0.5 * T1_UNIT2D, -7 * T1_UNIT2D },

	{ 0.5 * T1_UNIT2D, 1 * T1_UNIT2D },
	{ 0.5 * T1_UNIT2D, -7 * T1_UNIT2D },
	{ 2.5 * T1_UNIT2D, 1 * T1_UNIT2D },
	{ 2.5 * T1_UNIT2D, -7 * T1_UNIT2D },
	{ 3.5 * T1_UNIT2D, -1 * T1_UNIT2D },
	{ 3.5 * T1_UNIT2D, -7 * T1_UNIT2D },
	{ 5.5 * T1_UNIT2D, -1 * T1_UNIT2D },
	{ 5.5 * T1_UNIT2D, -7 * T1_UNIT2D },
	{ 6.5 * T1_UNIT2D, 1 * T1_UNIT2D },
	{ 6.5 * T1_UNIT2D, -7 * T1_UNIT2D },
	{ 8.5 * T1_UNIT2D, 1 * T1_UNIT2D },
	{ 8.5 * T1_UNIT2D, -7 * T1_UNIT2D },

	// Partie du bas.
	{ -8.5 * T1_UNIT2D, -1 * T1_UNIT2D },
	{ -8.5 * T1_UNIT2D, 7 * T1_UNIT2D },
	{ -6.5 * T1_UNIT2D, -1 * T1_UNIT2D },
	{ -6.5 * T1_UNIT2D, 7 * T1_UNIT2D },
	{ -5.5 * T1_UNIT2D, 1 * T1_UNIT2D },
	{ -5.5 * T1_UNIT2D, 7 * T1_UNIT2D },
	{ -3.5 * T1_UNIT2D, 1 * T1_UNIT2D },
	{ -3.5 * T1_UNIT2D, 7 * T1_UNIT2D },
	{ -2.5 * T1_UNIT2D, -1 * T1_UNIT2D },
	{ -2.5 * T1_UNIT2D, 7 * T1_UNIT2D },
	{ -0.5 * T1_UNIT2D, -1 * T1_UNIT2D },
	{ -0.5 * T1_UNIT2D, 7 * T1_UNIT2D },

	{ 0.5 * T1_UNIT2D, 1 * T1_UNIT2D },
	{ 0.5 * T1_UNIT2D, 7 * T1_UNIT2D },
	{ 2.5 * T1_UNIT2D, 1 * T1_UNIT2D },
	{ 2.5 * T1_UNIT2D, 7 * T1_UNIT2D },
	{ 3.5 * T1_UNIT2D, -1 * T1_UNIT2D },
	{ 3.5 * T1_UNIT2D, 7 * T1_UNIT2D },
	{ 5.5 * T1_UNIT2D, -1 * T1_UNIT2D },
	{ 5.5 * T1_UNIT2D, 7 * T1_UNIT2D },
	{ 6.5 * T1_UNIT2D, 1 * T1_UNIT2D },
	{ 6.5 * T1_UNIT2D, 7 * T1_UNIT2D },
	{ 8.5 * T1_UNIT2D, 1 * T1_UNIT2D },
	{ 8.5 * T1_UNIT2D, 7 * T1_UNIT2D },

};

//=============================================================================

// Déplace les points et les place dans le buffer de points à touner.
void T1_MovePoints(void)
{
	u32	i;

	// Partie du haut.
	for (i = 0; i < T1_PTS2D_NB/2; i++)
	{
		gTransit2D.pPtsOrg[i].nX = gT1_PtsOrg[i].nX;
		gTransit2D.pPtsOrg[i].nY = gT1_PtsOrg[i].nY - (gTransit2D.nOffset >> 1);
	}
	// Partie du bas.
	for (; i < T1_PTS2D_NB; i++)
	{
		gTransit2D.pPtsOrg[i].nX = gT1_PtsOrg[i].nX;
		gTransit2D.pPtsOrg[i].nY = gT1_PtsOrg[i].nY + (gTransit2D.nOffset >> 1);
	}

}

// Tracé des faces.
void T1_DrawFaces(void)
{
	u32	i;

	SDL_LockSurface(gVar.pScreen);

	for (i = 0; i < T1_FACES_NB/2; i++)
	{
		LinesBufferClear();
		LineBuf(gTransit2D.pPtsRot[(i*2)].nX + (SCR_Width / 2), gTransit2D.pPtsRot[(i*2)].nY + (SCR_Height / 2), gTransit2D.pPtsRot[(i*2) + 1].nX + (SCR_Width / 2), gTransit2D.pPtsRot[(i*2) + 1].nY + (SCR_Height / 2));
		LineBuf(gTransit2D.pPtsRot[(i*2) + 1].nX + (SCR_Width / 2), gTransit2D.pPtsRot[(i*2) + 1].nY + (SCR_Height / 2), gTransit2D.pPtsRot[(i*2) + 3].nX + (SCR_Width / 2), gTransit2D.pPtsRot[(i*2) + 3].nY + (SCR_Height / 2));
		LineBuf(gTransit2D.pPtsRot[(i*2) + 3].nX + (SCR_Width / 2), gTransit2D.pPtsRot[(i*2) + 3].nY + (SCR_Height / 2), gTransit2D.pPtsRot[(i*2) + 2].nX + (SCR_Width / 2), gTransit2D.pPtsRot[(i*2) + 2].nY + (SCR_Height / 2));
		LineBuf(gTransit2D.pPtsRot[(i*2) + 2].nX + (SCR_Width / 2), gTransit2D.pPtsRot[(i*2) + 2].nY + (SCR_Height / 2), gTransit2D.pPtsRot[(i*2)].nX + (SCR_Width / 2), gTransit2D.pPtsRot[(i*2)].nY + (SCR_Height / 2));
		FaceDraw();

		LinesBufferClear();
		LineBuf(gTransit2D.pPtsRot[(i*2) + (T1_PTS2D_NB/2)].nX + (SCR_Width / 2), gTransit2D.pPtsRot[(i*2) + (T1_PTS2D_NB/2)].nY + (SCR_Height / 2), gTransit2D.pPtsRot[(i*2) + 2 + (T1_PTS2D_NB/2)].nX + (SCR_Width / 2), gTransit2D.pPtsRot[(i*2) + 2 + (T1_PTS2D_NB/2)].nY + (SCR_Height / 2));
		LineBuf(gTransit2D.pPtsRot[(i*2) + 2 + (T1_PTS2D_NB/2)].nX + (SCR_Width / 2), gTransit2D.pPtsRot[(i*2) + 2 + (T1_PTS2D_NB/2)].nY + (SCR_Height / 2), gTransit2D.pPtsRot[(i*2) + 3 + (T1_PTS2D_NB/2)].nX + (SCR_Width / 2), gTransit2D.pPtsRot[(i*2) + 3 + (T1_PTS2D_NB/2)].nY + (SCR_Height / 2));
		LineBuf(gTransit2D.pPtsRot[(i*2) + 3 + (T1_PTS2D_NB/2)].nX + (SCR_Width / 2), gTransit2D.pPtsRot[(i*2) + 3 + (T1_PTS2D_NB/2)].nY + (SCR_Height / 2), gTransit2D.pPtsRot[(i*2) + 1 + (T1_PTS2D_NB/2)].nX + (SCR_Width / 2), gTransit2D.pPtsRot[(i*2) + 1 + (T1_PTS2D_NB/2)].nY + (SCR_Height / 2));
		LineBuf(gTransit2D.pPtsRot[(i*2) + 1 + (T1_PTS2D_NB/2)].nX + (SCR_Width / 2), gTransit2D.pPtsRot[(i*2) + 1 + (T1_PTS2D_NB/2)].nY + (SCR_Height / 2), gTransit2D.pPtsRot[(i*2) + (T1_PTS2D_NB/2)].nX + (SCR_Width / 2), gTransit2D.pPtsRot[(i*2) + (T1_PTS2D_NB/2)].nY + (SCR_Height / 2));
		FaceDraw();
	}

	SDL_UnlockSurface(gVar.pScreen);

}

//=============================================================================

// Initialisation de l'ouverture.
void T1_InitOpening(void)
{
	gTransit2D.nState = e_Transit2D_Opening;
	gTransit2D.nAngle = -10;
	gTransit2D.nOffset = 0;
	gTransit2D.nMultiplier = 0x0120;

}

// Initialisation de la fermeture.
void T1_InitClosing(void)
{
	gTransit2D.nState = e_Transit2D_Closing;
	gTransit2D.nAngle = 22;
	gTransit2D.nOffset = 128;
	gTransit2D.nMultiplier = 0x0320;

}

// Transition.
void T1_Manage(void)
{
	T1_MovePoints();
	Rotation2D(T1_PTS2D_NB, gTransit2D.pPtsOrg, gTransit2D.pPtsRot, gTransit2D.nAngle, gTransit2D.nMultiplier);
	T1_DrawFaces();

	if (gTransit2D.nState == e_Transit2D_ClosingEnded) return;	// Pour continuer les tracés une fois l'écran fermé (pour éviter le glitch d'une frame dans les menus).

	if (gTransit2D.nState == e_Transit2D_Opening)
	{
		if (gTransit2D.nOffset == 128)
		{
			gTransit2D.nState = e_Transit2D_OpeningEnded;
			return;
		}
		gTransit2D.nAngle += 1;
		gTransit2D.nMultiplier += 0x08;
		gTransit2D.nOffset += 2;
		gTransit2D.nMultiplier += 0x08;
		gTransit2D.nOffset += 2;
	}
	else
	{
		if (gTransit2D.nOffset == 0)
		{
			gTransit2D.nState = e_Transit2D_ClosingEnded;
			return;
		}
		gTransit2D.nAngle -= 1;
		gTransit2D.nMultiplier -= 0x08;
		gTransit2D.nOffset -= 2;
		gTransit2D.nMultiplier -= 0x08;
		gTransit2D.nOffset -= 2;
	}

}

//=============================================================================
//=============================================================================
// Transition 2 - Déchirure pour interlevel.

#define	T2_UNIT2D	(20)

// Tracé des faces - Version horizontale.
void T2H_DrawFaces(void)
{
	s32	i;
	s32	nOffsX, nOffsY;
	s32	d, d2;

	SDL_LockSurface(gVar.pScreen);

	nOffsY = (2 * T2_UNIT2D) / 3;
	for (i = -4; i <= 4; i++)
	{
		d = gTransit2D.nOffset;
		d += (gTransit2D.nState == e_Transit2D_Opening ? -(i + 4) : (i - 5)) * 3;
		if (d < 0) d = 0;
		if (d > 64) d = 64;
		d = 256 - gVar.pCos[d];

//todo : on doit pouvoir éviter un calcul en affectant d=d2 et en faisant un calcul à l'extérieur de la boucle la première fois...
		d2 = gTransit2D.nOffset;
		d2 += (gTransit2D.nState == e_Transit2D_Opening ? -((i+1) + 4) : ((i+1) - 5)) * 3;
		if (d2 < 0) d2 = 0;
		if (d2 > 64) d2 = 64;
		d2 = 256 - gVar.pCos[d2];

//todo : et là, on doit pouvoir faire un + au lieu du *.
		nOffsX = i * 2 * T2_UNIT2D;
		nOffsY = -nOffsY;	// Hauteur du pt en cours, inversé à chaque passage.

		// Partie haute.
		LinesBufferClear();
		LineBuf(-T2_UNIT2D + nOffsX + (SCR_Width / 2), (-7 * T2_UNIT2D) + nOffsY - d + (SCR_Height / 2), T2_UNIT2D + nOffsX + (SCR_Width / 2), (-7 * T2_UNIT2D) - nOffsY - d2 + (SCR_Height / 2));
		LineBuf(T2_UNIT2D + nOffsX + (SCR_Width / 2), (-7 * T2_UNIT2D) - nOffsY - d2 + (SCR_Height / 2), T2_UNIT2D + nOffsX + (SCR_Width / 2), (0 * T2_UNIT2D) - nOffsY - d2 + (SCR_Height / 2));
		LineBuf(T2_UNIT2D + nOffsX + (SCR_Width / 2), (0 * T2_UNIT2D) - nOffsY - d2  + (SCR_Height / 2), -T2_UNIT2D + nOffsX + (SCR_Width / 2), (0 * T2_UNIT2D) + nOffsY - d + (SCR_Height / 2));
		LineBuf(-T2_UNIT2D + nOffsX + (SCR_Width / 2), (0 * T2_UNIT2D) + nOffsY - d + (SCR_Height / 2), -T2_UNIT2D + nOffsX + (SCR_Width / 2), (-7 * T2_UNIT2D) + nOffsY - d + (SCR_Height / 2));
		FaceDraw();
		// Partie basse.
		LinesBufferClear();
		LineBuf(T2_UNIT2D + nOffsX + (SCR_Width / 2), (7 * T2_UNIT2D) - nOffsY + d2 + (SCR_Height / 2), -T2_UNIT2D + nOffsX + (SCR_Width / 2), (7 * T2_UNIT2D) + nOffsY + d + (SCR_Height / 2));
		LineBuf(-T2_UNIT2D + nOffsX + (SCR_Width / 2), (7 * T2_UNIT2D) + nOffsY + d + (SCR_Height / 2), -T2_UNIT2D + nOffsX + (SCR_Width / 2), (0 * T2_UNIT2D) + nOffsY + d + (SCR_Height / 2));
		LineBuf(-T2_UNIT2D + nOffsX + (SCR_Width / 2), (0 * T2_UNIT2D) + nOffsY + d + (SCR_Height / 2), T2_UNIT2D + nOffsX + (SCR_Width / 2), (0 * T2_UNIT2D) - nOffsY + d2 + (SCR_Height / 2));
		LineBuf(T2_UNIT2D + nOffsX + (SCR_Width / 2), (0 * T2_UNIT2D) - nOffsY + d2 + (SCR_Height / 2), T2_UNIT2D + nOffsX + (SCR_Width / 2), (7 * T2_UNIT2D) - nOffsY + d2 + (SCR_Height / 2));
		FaceDraw();
	}

	SDL_UnlockSurface(gVar.pScreen);

}

// Tracé des faces - Version verticale.
void T2V_DrawFaces(void)
{
	s32	i;
	s32	nOffsX, nOffsY;
	s32	d, d2;

	SDL_LockSurface(gVar.pScreen);

	nOffsX = (2 * T2_UNIT2D) / 3;
	for (i = -3; i <= 3; i++)
	{
		d = gTransit2D.nOffset;
		d += (gTransit2D.nState == e_Transit2D_Opening ? (i - 5) : -(i + 4)) * 3;
		if (d < 0) d = 0;
		if (d > 64) d = 64;
		d = 256 - gVar.pCos[d];

//todo : on doit pouvoir éviter un calcul en affectant d=d2 et en faisant un calcul à l'extérieur de la boucle la première fois...
		d2 = gTransit2D.nOffset;
		d2 += (gTransit2D.nState == e_Transit2D_Opening ? ((i+1) - 5) : -((i+1) + 4)) * 3;
		if (d2 < 0) d2 = 0;
		if (d2 > 64) d2 = 64;
		d2 = 256 - gVar.pCos[d2];

//todo : et là, on doit pouvoir faire un + au lieu du *.
		nOffsX = -nOffsX;	// Hauteur du pt en cours, inversé à chaque passage.
		nOffsY = i * 2 * T2_UNIT2D;

		// Partie gauche.
		LinesBufferClear();
		LineBuf((-9 * T2_UNIT2D) - nOffsX - d2 + (SCR_Width / 2), T2_UNIT2D + nOffsY + (SCR_Height / 2), (-9 * T2_UNIT2D) + nOffsX - d + (SCR_Width / 2), -T2_UNIT2D + nOffsY + (SCR_Height / 2));
		LineBuf((-9 * T2_UNIT2D) + nOffsX - d + (SCR_Width / 2), -T2_UNIT2D + nOffsY + (SCR_Height / 2), (0 * T2_UNIT2D) + nOffsX - d + (SCR_Width / 2), -T2_UNIT2D + nOffsY + (SCR_Height / 2));
		LineBuf((0 * T2_UNIT2D) + nOffsX - d + (SCR_Width / 2), -T2_UNIT2D + nOffsY + (SCR_Height / 2), (0 * T2_UNIT2D) - nOffsX - d2 + (SCR_Width / 2), T2_UNIT2D + nOffsY + (SCR_Height / 2));
		LineBuf((0 * T2_UNIT2D) - nOffsX - d2 + (SCR_Width / 2), T2_UNIT2D + nOffsY + (SCR_Height / 2), (-9 * T2_UNIT2D) - nOffsX - d2 + (SCR_Width / 2), T2_UNIT2D + nOffsY + (SCR_Height / 2));
		FaceDraw();
		// Partie droite.
		LinesBufferClear();
		LineBuf((9 * T2_UNIT2D) + nOffsX + d + (SCR_Width / 2), -T2_UNIT2D + nOffsY + (SCR_Height / 2), (9 * T2_UNIT2D) - nOffsX + d2 + (SCR_Width / 2), T2_UNIT2D + nOffsY + (SCR_Height / 2));
		LineBuf((9 * T2_UNIT2D) - nOffsX + d2 + (SCR_Width / 2), T2_UNIT2D + nOffsY + (SCR_Height / 2), (0 * T2_UNIT2D) - nOffsX + d2 + (SCR_Width / 2), T2_UNIT2D + nOffsY + (SCR_Height / 2));
		LineBuf((0 * T2_UNIT2D) - nOffsX + d2 + (SCR_Width / 2), T2_UNIT2D + nOffsY + (SCR_Height / 2), (0 * T2_UNIT2D) + nOffsX + d + (SCR_Width / 2), -T2_UNIT2D + nOffsY + (SCR_Height / 2));
		LineBuf((0 * T2_UNIT2D) + nOffsX + d + (SCR_Width / 2), -T2_UNIT2D + nOffsY + (SCR_Height / 2), (9 * T2_UNIT2D) + nOffsX + d + (SCR_Width / 2), -T2_UNIT2D + nOffsY + (SCR_Height / 2));
		FaceDraw();
	}

	SDL_UnlockSurface(gVar.pScreen);

}

//=============================================================================

// Initialisation de l'ouverture.
void T2_InitOpening(void)
{
	gTransit2D.nState = e_Transit2D_Opening;
	gTransit2D.nOffset = 0;

}

// Initialisation de la fermeture.
void T2_InitClosing(void)
{
	gTransit2D.nState = e_Transit2D_Closing;
	gTransit2D.nOffset = 70;

}

// Transition.
void T2_Manage(void)
{
//	T2_DrawFaces();

	if (gTransit2D.nState == e_Transit2D_ClosingEnded) return;	// Pour continuer les tracés une fois l'écran fermé (pour éviter le glitch d'une frame dans les menus).

	if (gTransit2D.nState == e_Transit2D_Opening)
	{
		if (gTransit2D.nOffset == 70)
		{
			gTransit2D.nState = e_Transit2D_OpeningEnded;
			return;
		}
		gTransit2D.nOffset += 2;
	}
	else
	{
		if (gTransit2D.nOffset == 0)
		{
			gTransit2D.nState = e_Transit2D_ClosingEnded;
			return;
		}
		gTransit2D.nOffset -= 2;
	}

}
// Encapsulation H.
void T2H_Manage(void)
{
	T2H_DrawFaces();
	T2_Manage();
}
// Encapsulation V.
void T2V_Manage(void)
{
	T2V_DrawFaces();
	T2_Manage();
}

//=============================================================================
//=============================================================================
// Transition 3 - Bandes verticales, how to play et credits.

#define T3_STRIPE_NB	14 //20
#define T3_STRIPE_SZ	24 //((SCR_Width / T3_STRIPE_NB) - 1)
// Tracé des faces.
void T3_DrawFaces(u32 nFillRight)
{
	s32	i;
	s32	nOffsX = -(3*(T3_STRIPE_SZ+1)) -16;// -8;//0;
	s32	nDiff;

	SDL_LockSurface(gVar.pScreen);
	if (nFillRight)
	{
		for (i = 0; i < T3_STRIPE_NB+3; i++, nOffsX += T3_STRIPE_SZ+1)
		{
			nDiff = (T3_STRIPE_SZ + i) - gTransit2D.nOffset;
			if (nDiff > T3_STRIPE_SZ) nDiff = T3_STRIPE_SZ;
			if (nDiff < 0) continue;

			LinesBufferClear();
/*
			LineBuf(nOffsX + T3_STRIPE_SZ - nDiff, 0, nOffsX + T3_STRIPE_SZ, 0);
			LineBuf(nOffsX + T3_STRIPE_SZ, 0, nOffsX + T3_STRIPE_SZ-(T3_STRIPE_SZ+1), SCR_Height-1);
			LineBuf(nOffsX + T3_STRIPE_SZ-(T3_STRIPE_SZ+1), SCR_Height-1, nOffsX + T3_STRIPE_SZ-(T3_STRIPE_SZ+1) - nDiff, SCR_Height-1);
			LineBuf(nOffsX + T3_STRIPE_SZ-(T3_STRIPE_SZ+1) - nDiff, SCR_Height-1, nOffsX + T3_STRIPE_SZ - nDiff, 0);
*/
			LineBuf(nOffsX + (gTransit2D.nOffset*2) + T3_STRIPE_SZ - nDiff, 0, nOffsX + (gTransit2D.nOffset*2) + T3_STRIPE_SZ, 0);
			LineBuf(nOffsX + (gTransit2D.nOffset*2) + T3_STRIPE_SZ, 0, nOffsX + T3_STRIPE_SZ-(0), SCR_Height-1);
			LineBuf(nOffsX + T3_STRIPE_SZ-(0), SCR_Height-1, nOffsX + T3_STRIPE_SZ-(0) - nDiff, SCR_Height-1);
			LineBuf(nOffsX + T3_STRIPE_SZ-(0) - nDiff, SCR_Height-1, nOffsX + (gTransit2D.nOffset*2) + T3_STRIPE_SZ - nDiff, 0);
/*
			LineBuf(nOffsX + (gTransit2D.nOffset) + T3_STRIPE_SZ - nDiff, 0, nOffsX + (gTransit2D.nOffset) + T3_STRIPE_SZ, 0);
			LineBuf(nOffsX + (gTransit2D.nOffset) + T3_STRIPE_SZ, 0, nOffsX + T3_STRIPE_SZ-(gTransit2D.nOffset), SCR_Height-1);
			LineBuf(nOffsX + T3_STRIPE_SZ-(gTransit2D.nOffset), SCR_Height-1, nOffsX + T3_STRIPE_SZ-(gTransit2D.nOffset) - nDiff, SCR_Height-1);
			LineBuf(nOffsX + T3_STRIPE_SZ-(gTransit2D.nOffset) - nDiff, SCR_Height-1, nOffsX + (gTransit2D.nOffset) + T3_STRIPE_SZ - nDiff, 0);
*/
			FaceDraw();
		}
	}
	else
	{
		for (i = 0; i < T3_STRIPE_NB+3; i++, nOffsX += T3_STRIPE_SZ+1)
		{
			nDiff = gTransit2D.nOffset - i;
			if (nDiff > T3_STRIPE_SZ) nDiff = T3_STRIPE_SZ;
			if (nDiff < 0) continue;

			LinesBufferClear();
/*
			LineBuf(nOffsX, 0, nOffsX + nDiff, 0);
			LineBuf(nOffsX + nDiff, 0, nOffsX-(T3_STRIPE_SZ+1) + nDiff, SCR_Height-1);
			LineBuf(nOffsX-(T3_STRIPE_SZ+1) + nDiff, SCR_Height-1, nOffsX-(T3_STRIPE_SZ+1), SCR_Height-1);
			LineBuf(nOffsX-(T3_STRIPE_SZ+1), SCR_Height-1, nOffsX, 0);
*/
			LineBuf(nOffsX + (gTransit2D.nOffset*2) , 0, nOffsX + (gTransit2D.nOffset*2)  + nDiff, 0);
			LineBuf(nOffsX + (gTransit2D.nOffset*2)  + nDiff, 0, nOffsX-(0) + nDiff, SCR_Height-1);
			LineBuf(nOffsX-(0) + nDiff, SCR_Height-1, nOffsX-(0), SCR_Height-1);
			LineBuf(nOffsX-(0), SCR_Height-1, nOffsX + (gTransit2D.nOffset*2) , 0);
			FaceDraw();
		}
	}
	SDL_UnlockSurface(gVar.pScreen);

}

//=============================================================================

// Initialisation de l'ouverture.
void T3_InitOpening(void)
{
	gTransit2D.nState = e_Transit2D_Opening;
	gTransit2D.nOffset = 0;

}

// Initialisation de la fermeture.
void T3_InitClosing(void)
{
	gTransit2D.nState = e_Transit2D_Closing;
	gTransit2D.nOffset = 0;

}

// Transition.
void T3_Manage(void)
{
	T3_DrawFaces(gTransit2D.nState == e_Transit2D_Opening ? 1 : 0);

	if (gTransit2D.nState == e_Transit2D_ClosingEnded) return;	// Pour continuer les tracés une fois l'écran fermé (pour éviter le glitch d'une frame dans les menus).

	if (gTransit2D.nOffset == 41)//37)
	{
//		gTransit2D.nState = e_Transit2D_OpeningEnded;
		gTransit2D.nState = (gTransit2D.nState == e_Transit2D_Opening ? e_Transit2D_OpeningEnded : e_Transit2D_ClosingEnded);
		return;
	}
	gTransit2D.nOffset++;

}

//=============================================================================
//=============================================================================
// Fonctions pour appel externe.

// Pour init au lancement du jeu.
void Transit2D_Reset(void)
{
	gTransit2D.nState = e_Transit2D_OpeningEnded;	// Dans cet état, pas de tracé, on sort tout de suite.
}

// Renvoie 1 si transition terminée, 0 sinon.
u32 Transit2D_CheckEnd(void)
{
	return (gTransit2D.nState == e_Transit2D_OpeningEnded || gTransit2D.nState == e_Transit2D_ClosingEnded ? 1 : 0);
}

typedef void (*pFctInitOC) (void);
// Init opening.
void Transit2D_InitOpening(u32 nTransitNo)
{
	static	pFctInitOC	pFctInitTb[e_Transit_Max] = { T0_InitOpening, T1_InitOpening, T2_InitOpening, T2_InitOpening, T3_InitOpening };
	gTransit2D.nTransitNo = nTransitNo;
	pFctInitTb[nTransitNo]();
}

// Init closing.
void Transit2D_InitClosing(u32 nTransitNo)
{
	static	pFctInitOC	pFctInitTb[e_Transit_Max] = { T0_InitClosing, T1_InitClosing, T2_InitClosing, T2_InitClosing, T3_InitClosing };
	gTransit2D.nTransitNo = nTransitNo;
	pFctInitTb[nTransitNo]();
}

typedef void (*pFctMain) (void);
// Manage.
void Transit2D_Manage(void)
{
	if (gTransit2D.nState == e_Transit2D_OpeningEnded) return;

	static	pFctMain	pFctMainTb[e_Transit_Max] = { T0_Manage, T1_Manage, T2H_Manage, T2V_Manage, T3_Manage };
	pFctMainTb[gTransit2D.nTransitNo]();

	if (gTransit2D.nState == e_Transit2D_ClosingEnded) Sfx_ClearChannels();		// Coupe les sons quand transit finie.

}


