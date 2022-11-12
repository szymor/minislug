
#include "includes.h"

//lev5:gros sous marin : 10x8 org 1142 / en (8,0) et (23,0)
//lev5:petit sous marin : 4x5 org 828 / en (56,3) et (87,3)

// Update les blocs d'un rectangle dans la map.
void BlkBkg(u32 nPlane, u32 nMapPosX, u32 nMapPosY, u32 nBlkLg, u32 nBlkHt, u32 nBlkOrg)
{
	u32	ix, iy;
	s32	*pBlk;
	u32	nBlkNo, nBlkMax;
	s32	nPosXMin, nPosYMin, nPosXMax, nPosYMax;

	if (nPlane >= gMap.nPlanesNb) return;

	// Pour MAJ de ce qui est déjà dans l'écran.
	ScrollGetPlanePosXY(&nPosXMin, &nPosYMin, nPlane);
	nPosXMin >>= 12;
	nPosYMin >>= 12;
	nPosXMax = nPosXMin + (SCR_Width / 16) + 1;
	if (nPosXMax > gMap.pPlanesLg[nPlane]) nPosXMax = gMap.pPlanesLg[nPlane];
	nPosYMax = nPosYMin + (SCR_Height / 16) + 1;
	if (nPosYMax > gMap.pPlanesHt[nPlane]) nPosYMax = gMap.pPlanesHt[nPlane];

	//
	pBlk = gMap.ppPlanesBlocks[nPlane];
	nBlkMax = ((gMap.ppPlanesGfx[nPlane])->w / 16) * ((gMap.ppPlanesGfx[nPlane])->h / 16);

	for (iy = 0; (iy < nBlkHt) && (nMapPosY + iy < gMap.pPlanesHt[nPlane]); iy++)
	for (ix = 0; (ix < nBlkLg) && (nMapPosX + ix < gMap.pPlanesLg[nPlane]); ix++)
	{
		// MAJ du n° de bloc dans la map.
		nBlkNo = nBlkOrg + (iy * ((gMap.ppPlanesGfx[nPlane])->w / 16)) + ix;
		if (nBlkNo >= nBlkMax) nBlkNo = 0;
		pBlk[((nMapPosY + iy) * gMap.nMapLg) + (nMapPosX + ix)] = nBlkNo;

		// MAJ du bloc à l'écran (si dans l'écran).
		if ((nMapPosX + ix >= nPosXMin) && (nMapPosX + ix < nPosXMax) &&
			(nMapPosY + iy >= nPosYMin) && (nMapPosY + iy < nPosYMax))
			Scroll_BlkAnm_BlockUpdate(nPlane, nMapPosX + ix, nMapPosY + iy, 0);
	}

}

