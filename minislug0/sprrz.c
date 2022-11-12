// Les sprites en zoom / rotozoom.
// Code: 17o2!! (Clément CORDE)

#include "includes.h"


//u8	gpRotBuf[ROT2D_BUF_Width * ROT2D_BUF_Height];	// Buffer pour rendu de la rotation. (=> faire un malloc à l'init).
u8	*gpRotBuf;	// Buffer pour rendu des roto/zooms. Sz = ROT2D_BUF_Width * ROT2D_BUF_Height.


// Une structure pour stocker tout le bordel intermédiaire entre le pré-rendu et le rendu.
struct SSprRotoZoomInternal
{
	struct SSprite	sRotSprDesc;	// Pour renvoyer les infos du sprite roto/zoomé.

	struct	S2DPoint	pPtsOrg[5];		// Les points à faire tourner (4 coins + pt de ref).
	struct	S2DPoint	pPtsRot[5];		// Les points après rotation.
	u32	nMaxX, nMaxY;
	struct SSprite	*pSprDesc;

};
struct SSprRotoZoomInternal	gSprRZ;

//=============================================================================

// Rotation 2D.
void Rotation2D(u32 nNbPts, struct S2DPoint *pPtsOrg, struct S2DPoint *pPtsRot, u8 nAngle, u16 nMultiplier)
{
	s32	incx, incy;
	u32	i;

	incx = (gVar.pCos[nAngle] * nMultiplier) >> 8;
	incy = (gVar.pSin[nAngle] * nMultiplier) >> 8;

//incx = gVar.pCos[nAngle];
//incy = gVar.pSin[nAngle];

//incx = (s32)(cos(nAngle * 2 * 3.1415927 / 256) * 4096);
//incy = (s32)(sin(nAngle * 2 * 3.1415927 / 256) * 4096);


	// Rotation 2D.
	// x' = x cos f - y sin f
	// y' = y cos f + x sin f
	for (i = 0; i < nNbPts; i++)
	{
		pPtsRot[i].nX = ((pPtsOrg[i].nX * incx - pPtsOrg[i].nY * incy) >> 8);// + (SCR_Width / 2);
		pPtsRot[i].nY = ((pPtsOrg[i].nY * incx + pPtsOrg[i].nX * incy) >> 8);// + (SCR_Height / 2);
//pPtsRot[i].nX = ((pPtsOrg[i].nX * incx - pPtsOrg[i].nY * incy) >> 12);// + (SCR_Width / 2);
//pPtsRot[i].nY = ((pPtsOrg[i].nY * incx + pPtsOrg[i].nX * incy) >> 12);// + (SCR_Height / 2);
	}

}

// Décalage pour revenir avec le rectangle englobant en (0,0) en haut à gauche. (En vue du tracé dans le buffer).
void RotOffsetAdd(u32 nNbPts, struct S2DPoint *pPts)
{
	s32	nMinX, nMinY;
	u32	i;

	// Recherche du min X et Y.
	nMinX = 1 << 30;
	nMinY = 1 << 30;
	for (i = 0; i < nNbPts; i++)
	{
		if (pPts[i].nX < nMinX) nMinX = pPts[i].nX;
		if (pPts[i].nY < nMinY) nMinY = pPts[i].nY;
	}
	// Décalage.
	for (i = 0; i < nNbPts; i++)
	{
		pPts[i].nX -= nMinX;
		pPts[i].nY -= nMinY;
	}

}

// Récupère les X et Y les plus grands. = Taille du buffer.
void RotGetMaxXY(u32 nNbPts, struct S2DPoint *pPts, u32 *pnMaxX, u32 *pnMaxY)
{
	u32	i;

	*pnMaxX = *pnMaxY = 0;
	for (i = 0; i < nNbPts; i++)
	{
		if (pPts[i].nX > *pnMaxX) *pnMaxX = pPts[i].nX;
		if (pPts[i].nY > *pnMaxY) *pnMaxY = pPts[i].nY;
	}
}

// "Tracé" d'une ligne, un point par ligne raster + interpolation X et Y des coordonées de mapping.
void RotLineBuf(s32 nX1, s32 nY1, s32 nX2, s32 nY2, s32 *pBufL, s32 *pBufR,
				s32 nMapX1, s32 nMapY1, s32 nMapX2, s32 nMapY2, s32 *pMapXBufL, s32 *pMapYBufL, s32 *pMapXBufR, s32 *pMapYBufR)
{
	s32	*pBuf;
	s32	*pMapX, *pMapY;
	s32	y;
	s32	nTmp;

	// Cas particulier de la ligne horizontale ou de 1 pt de haut. => On ne la trace pas, le cas sera règlé par les autres lignes de la face.
	if (nY1 == nY2) return;

	// La ligne monte ou descend ?
	if (nY2 > nY1)
	{
		// Descend. On trace du côté droit.
		pBuf = pBufR;
		pMapX = pMapXBufR;
		pMapY = pMapYBufR;
	}
	else
	{
		// Monte. On trace du côté gauche.
		// + Pour que les points soient exactement au mêmes endroits si deux lignes sont superposées, on fait descendre la ligne.
		pBuf = pBufL;
		pMapX = pMapXBufL;
		pMapY = pMapYBufL;
		// Swap.
		nTmp = nX1; nX1 = nX2; nX2 = nTmp;
		nTmp = nY1; nY1 = nY2; nY2 = nTmp;
		nTmp = nMapX1; nMapX1 = nMapX2; nMapX2 = nTmp;
		nTmp = nMapY1; nMapY1 = nMapY2; nMapY2 = nTmp;
	}

	s32	nDelta;
	s32	nIncX;
	s32	nMapIncX, nMapIncY;

	nDelta = ABS(nY2 - nY1);
	nIncX = ((nX2 - nX1) << 8) / nDelta;
	nX1 <<= 8;
	nX1 |= 0x7F;
//nIncX = ((nX2 - nX1) << 12) / nDelta;
//nX1 <<= 12;
//nX1 |= 0x7FF;

	nMapIncX = ((nMapX2 - nMapX1) << 8) / nDelta;
	nMapX1 <<= 8;
	nMapX1 |= 0x7F;

	nMapIncY = ((nMapY2 - nMapY1) << 8) / nDelta;
	nMapY1 <<= 8;
	nMapY1 |= 0x7F;

	// Tracé (remplissage du buffer).
	for (y = nY1; y <= nY2; y += 1)
	{
		// Draw coord.
		pBuf[y] = nX1 >> 8;
//pBuf[y] = nX1 >> 12;
		nX1 += nIncX;
		// Map coord (on garde les virgules pour un meilleur rendu).
		pMapX[y] = nMapX1;
		nMapX1 += nMapIncX;
		pMapY[y] = nMapY1;
		nMapY1 += nMapIncY;
	}

}

//=============================================================================
void SprRotoZoom_Render(void);

// Sprite en rotozoom - préparation.
// Note : L'angle est en u16 pour coller au prototype du prérendu du zoom, pour qu'on puisse faire des pointeurs de fonction.
struct SSprite * SprRotoZoom_PreRender(u32 nSpr, u16 nMultiplier, u16 nAngle, void **ppRenderFct)
{
	// On récupère le descripteur de sprite pour Lg/Ht.
	gSprRZ.pSprDesc = SprGetDesc(nSpr);

	// Préparation des points à faire tourner.
	gSprRZ.pPtsOrg[0].nX = gSprRZ.pPtsRot[0].nX = 0;	// Top Left (on ne le tourne pas ! (0,0) > (0,0)).
	gSprRZ.pPtsOrg[0].nY = gSprRZ.pPtsRot[0].nY = 0;
	gSprRZ.pPtsOrg[1].nX = gSprRZ.pSprDesc->nLg - 1;	// Top Right.
	gSprRZ.pPtsOrg[1].nY = 0;
	gSprRZ.pPtsOrg[2].nX = gSprRZ.pSprDesc->nLg - 1;	// Bottom Right.
	gSprRZ.pPtsOrg[2].nY = gSprRZ.pSprDesc->nHt - 1;
	gSprRZ.pPtsOrg[3].nX = 0;							// Bottom Left.
	gSprRZ.pPtsOrg[3].nY = gSprRZ.pSprDesc->nHt - 1;
	gSprRZ.pPtsOrg[4].nX = gSprRZ.pSprDesc->nPtRefX;	// Pt de ref.
	gSprRZ.pPtsOrg[4].nY = gSprRZ.pSprDesc->nPtRefY;
	// Rotation.
	Rotation2D(4, &gSprRZ.pPtsOrg[1], &gSprRZ.pPtsRot[1], nAngle, nMultiplier);
	// Décalage pour revenir avec le rectangle englobant en (0,0) en haut à gauche.
	RotOffsetAdd(5, &gSprRZ.pPtsRot[0]);
	// Récupère les X et Y les plus grands (on ne prend pas en compte le pt de ref). = Taille du buffer.
	RotGetMaxXY(4, &gSprRZ.pPtsRot[0], &gSprRZ.nMaxX, &gSprRZ.nMaxY);
	gSprRZ.nMaxX++;
	gSprRZ.nMaxY++;
	// Taille ok pour le buffer ?
	if (gSprRZ.nMaxX * gSprRZ.nMaxY > ROT2D_BUF_Width * ROT2D_BUF_Height)
	{
		fprintf(stderr, "SprRotoZoom_PreRender(): Aborted, resulting sprite bigger than buffer. (w = %d / h = %d)\n", (int)gSprRZ.nMaxX, (int)gSprRZ.nMaxY);
		return (NULL);
	}

	// On recopie les infos utiles dans la structure à renvoyer.
	gSprRZ.sRotSprDesc.nLg = gSprRZ.nMaxX;
	gSprRZ.sRotSprDesc.nHt = gSprRZ.nMaxY;
	gSprRZ.sRotSprDesc.nPtRefX = gSprRZ.pPtsRot[4].nX;
	gSprRZ.sRotSprDesc.nPtRefY = gSprRZ.pPtsRot[4].nY;
	gSprRZ.sRotSprDesc.pGfx8 = gpRotBuf;
	gSprRZ.sRotSprDesc.nRemapPalNo = gSprRZ.pSprDesc->nRemapPalNo;

	*ppRenderFct = (void *)SprRotoZoom_Render;

	return (&gSprRZ.sRotSprDesc);

}

// Sprite en rotozoom - rendu.
void SprRotoZoom_Render(void)
{
	s32	pLnBufL[ROT2D_BUF_Height];	// Buffer de lignes.
	s32	pLnBufR[ROT2D_BUF_Height];	// On remplira en x de L à R sur chaque ligne raster.
	s32	pMapXBufL[ROT2D_BUF_Height];	// Idem avec les coordonnées de mapping.
	s32	pMapYBufL[ROT2D_BUF_Height];
	s32	pMapXBufR[ROT2D_BUF_Height];
	s32	pMapYBufR[ROT2D_BUF_Height];
	u32	i;

	// Clear buffer.
	memset(gpRotBuf, 0, gSprRZ.nMaxX * gSprRZ.nMaxY);
	// "Tracé" des 4 côtés et calcul des coordonnées de map pour chaque ligne raster.
	for (i = 0; i < 3; i++)
	{
		RotLineBuf(gSprRZ.pPtsRot[i].nX, gSprRZ.pPtsRot[i].nY, gSprRZ.pPtsRot[i+1].nX, gSprRZ.pPtsRot[i+1].nY, pLnBufL, pLnBufR,
			gSprRZ.pPtsOrg[i].nX, gSprRZ.pPtsOrg[i].nY, gSprRZ.pPtsOrg[i+1].nX, gSprRZ.pPtsOrg[i+1].nY, pMapXBufL, pMapYBufL, pMapXBufR, pMapYBufR);
	}
	RotLineBuf(gSprRZ.pPtsRot[i].nX, gSprRZ.pPtsRot[i].nY, gSprRZ.pPtsRot[0].nX, gSprRZ.pPtsRot[0].nY, pLnBufL, pLnBufR,
		gSprRZ.pPtsOrg[i].nX, gSprRZ.pPtsOrg[i].nY, gSprRZ.pPtsOrg[0].nX, gSprRZ.pPtsOrg[0].nY, pMapXBufL, pMapYBufL, pMapXBufR, pMapYBufR);

	// Tracé dans le buffer de rendu.
	u32	x, y;
	s32	nDelta;
	s32	nMapIncX, nMapIncY;
	s32	nMapX1, nMapY1;
	u8	*pRenderBuf;

	pRenderBuf = gpRotBuf;
	for (y = 0; y < gSprRZ.nMaxY; y++)
	{
		nDelta = ABS(pLnBufR[y] - pLnBufL[y]);

		nMapIncX = (nDelta == 0 ? 0 : (pMapXBufR[y] - pMapXBufL[y]) / nDelta);
		nMapX1 = pMapXBufL[y];
		nMapX1 |= 0x7F;

		nMapIncY = (nDelta == 0 ? 0 : (pMapYBufR[y] - pMapYBufL[y]) / nDelta);
		nMapY1 = pMapYBufL[y];
		nMapY1 |= 0x7F;

		for (x = pLnBufL[y]; x <= pLnBufR[y]; x++)
		{
			*(pRenderBuf + x) = *(gSprRZ.pSprDesc->pGfx8 + ((nMapY1 >> 8) * gSprRZ.pSprDesc->nLg) + (nMapX1 >> 8));
			nMapX1 += nMapIncX;
			nMapY1 += nMapIncY;
		}
		pRenderBuf += gSprRZ.nMaxX;
	}

}

//=============================================================================
void SprZoom_Render(void);

// Sprite en zoom - préparation.
struct SSprite * SprZoom_PreRender(u32 nSpr, u16 nZoomX, u16 nZoomY, void **ppRenderFct)
{
	// On récupère le descripteur de sprite pour Lg/Ht.
	gSprRZ.pSprDesc = SprGetDesc(nSpr);
	// Calcul des dimensions max.
	gSprRZ.nMaxX = ((gSprRZ.pSprDesc->nLg - 1) * nZoomX) >> 8;
	gSprRZ.nMaxY = ((gSprRZ.pSprDesc->nHt - 1) * nZoomY) >> 8;
	gSprRZ.nMaxX++;
	gSprRZ.nMaxY++;
	// Taille ok pour le buffer ?
	if (gSprRZ.nMaxX * gSprRZ.nMaxY > ROT2D_BUF_Width * ROT2D_BUF_Height)
	{
		fprintf(stderr, "SprZoom_PreRender(): Aborted, resulting sprite bigger than buffer. (w = %d / h = %d)\n", (int)gSprRZ.nMaxX, (int)gSprRZ.nMaxY);
		return (NULL);
	}

	// On recopie les infos utiles dans la structure à renvoyer.
	gSprRZ.sRotSprDesc.nLg = gSprRZ.nMaxX;
	gSprRZ.sRotSprDesc.nHt = gSprRZ.nMaxY;
	gSprRZ.sRotSprDesc.nPtRefX = (gSprRZ.pSprDesc->nPtRefX * nZoomX) >> 8;
	gSprRZ.sRotSprDesc.nPtRefY = (gSprRZ.pSprDesc->nPtRefY * nZoomY) >> 8;
	gSprRZ.sRotSprDesc.pGfx8 = gpRotBuf;
	gSprRZ.sRotSprDesc.nRemapPalNo = gSprRZ.pSprDesc->nRemapPalNo;

	*ppRenderFct = (void *)SprZoom_Render;

	return (&gSprRZ.sRotSprDesc);

}

// Sprite en zoom - rendu.
void SprZoom_Render(void)
{
	// Clear buffer.
	memset(gpRotBuf, 0, gSprRZ.nMaxX * gSprRZ.nMaxY);

	// Tracé dans le buffer de rendu.
	u32	x, y;
	s32	nZoomIncX, nZoomIncY;
	s32	nX1, nY1;
	u8	*pRenderBuf;
	u8	*pGfx8;

	nZoomIncX = (gSprRZ.pSprDesc->nLg << 8) / gSprRZ.nMaxX;
	nZoomIncY = (gSprRZ.pSprDesc->nHt << 8) / gSprRZ.nMaxY;
	nY1 = 0;

	pRenderBuf = gpRotBuf;
	for (y = 0; y < gSprRZ.nMaxY; y++)
	{
		nX1 = 0;
		pGfx8 = gSprRZ.pSprDesc->pGfx8 + ((nY1 >> 8) * gSprRZ.pSprDesc->nLg);
		for (x = 0; x < gSprRZ.nMaxX; x++)
		{
			*pRenderBuf++ = *(pGfx8 + (nX1 >> 8));
			nX1 += nZoomIncX;
		}
		nY1 += nZoomIncY;
	}

}

