// Sprite management.
// Code: 17o2!! (Clément CORDE)

#include "includes.h"
#include "sprites_inc.h"

#define	SPRGRAB_DISPLAY_INFO	0		// Set to 0 not to display capture info / 1 to display.
#define	SPRPAL_SUB_ON	1				// Set to 1 to save palettes with x colors instead of 256.
//#define	DEBUG_INFO	1	// Comment for deletion.

#if	SPRPAL_SUB_ON == 1
#define	SPR_PAL_SZ	1	//8	//16
#else
#define	SPR_PAL_SZ	256
#endif

//#ifdef __LINUX__
#if defined (__LINUX__) || defined (__APPLE__)
// stricmp doesn't exist in Linux: strcasecmp is used instead, in strings.h.
int stricmp(char *pStr1, char *pStr2)
{
	return (strcasecmp(pStr1, pStr2));
}
#endif


// To capture sprites.
#define	SPRDEF_ALLOC_UNIT	256
struct SSprite	*gpSprDef;	// Sprite definitions.
u32	gnSprNbSprDefMax;	// Maximum number of sprites that can be captured until the next realloc.
u32	gnSprNbSprites;		// Number of sprites captured.

#define	SPRBUF_ALLOC_UNIT	(1024 * 1024)
u8	*gpSprBuf;		// Sprite data.
u32	gnSprBufSz;		// Data buffer size.
u32	gnSprBufAllocSz;	// Allocated data buffer size to avoid non-stop reallocations.

u16	*gpSprRemapPalettes;	// End-to-end sprite remapping palettes. 1 u16 per color in screen format (i.e. not RGB), x u16 per pal (see SPRPAL_SUB_ON and SPR_PAL_SZ).
u32	gnSprRemapPalettesNb;	// Number of pallets.
u8	*gpSprPal3Bytes;		// Colors on 3 bytes.

u16	*gpSprFlipBuf;		// Buffer to create flipped images.

extern u8	*gpRotBuf;	// Buffer for rotation rendering. Sz = ROT2D_BUF_Width * ROT2D_BUF_Height. Not in .H because it doesn't need to be known by anything other than the sprite engine.


// For sorting sprites at each frame.
struct SSprStockage
{
	u32 nSprNo;
	s32 nPosX, nPosY;
	u16 nPrio;
	// For Roto/Zoom :
	u16	nZoomX;				// Rot : nAngle + nZoomX / Zoom slt : nZoomX + nZoomY.
	union
	{
		u16	nZoomY;
		u8	nAngle;
	};
	void	*pFct;			// Ptr on pre-render fct then render (update by pre-render fct) zoom or rotozoom. NULL for a normal sprite.
};
#define	SPR_STO_MAX	512
struct SSprStockage	gpSprSto[SPR_STO_MAX];
struct SSprStockage	*gpSprSort[SPR_STO_MAX];	// For sorting.
u32	gnSprSto;			// Number of sprites stored for display.


// Engine initialization (once!).
void SprInitEngine(void)
{
//printf("SSprStockage sz = %d\n", sizeof(struct SSprStockage));

	gpSprDef = NULL;		// Sprite definitions.
	gnSprNbSprDefMax = 0;	// Maximum number of sprites that can be captured until the next realloc.
	gnSprNbSprites = 0;		// Number of sprites captured.

	gpSprBuf = NULL;	// Sprite data.
	gnSprBufSz = 0;		// Data buffer size.
	gnSprBufAllocSz = 0;		// Allocated data buffer size to avoid non-stop reallocations.

	gnSprSto = 0;		// Number of sprites stored for display.

	gpSprRemapPalettes = NULL;	// Remapping palettes.
	gnSprRemapPalettesNb = 0;	// Number of palettes.
	gpSprPal3Bytes = NULL;		// Colors on 3 bytes.

	gpSprFlipBuf = NULL;	// Buffer to create flipped images.
	gpRotBuf = NULL;		// Buffer for rotation rendering.

	#if CACHE_ON == 1
	CacheClear();		// Reset cache.
	#endif

}

// Secure fopen.
FILE * sec_fopen(char *pFilename, char *pMode)
{
	FILE	*fPt;

	fPt = fopen(pFilename, pMode);
	if (fPt == NULL)
	{
		fprintf(stderr, "SprBin_sub_fopen(): Error opening file '%s'.\n", pFilename);
		exit (1);
	}
	return (fPt);
}

// Calculation of a checksum for binary files.
u32 SprChecksum(u8 *pData, u32 nDataSz)
{
	u32	i;
	u32	nRem;
	u32	nSum;

	nRem = nDataSz & 3;
	nDataSz >>= 2;
	nSum = 0;
	// Quads.
	for (i = 0; i < nDataSz; i++)
	{
		nSum += *(u32 *)pData;
		pData += 4;
	}
	// Remaining bytes.
	for (i = 0; i < nRem; i++)
	{
		nSum += *pData;
		pData++;
	}
	return (nSum);
}

#if SPR_SAVE == 1		// Read graphics files and save data.
void SprBinSave_sub(char *pFilename, u8 *pSrc, u32 nSavSz)
{
	FILE	*fPt;
	u32	nSz;
	u32	nChecksum;

	nChecksum = SprChecksum(pSrc, nSavSz);

	fPt = sec_fopen(pFilename, "wb");
	nSz = fwrite(pSrc, 1, nSavSz, fPt);
	nSz += fwrite(&nChecksum, 1, sizeof(u32), fPt);
	fclose(fPt);
	if (nSz != nSavSz + sizeof(u32))
	{
		fprintf(stderr, "SprBinariesSave(): %s: Error, wrong file size written (%d bytes written, %d bytes expected).\n", pFilename, (int)nSz, (int)nSavSz);
		exit (1);
	}
printf("File '%s', Checksum: %x\n", pFilename, nChecksum);
}

// Save sprites as binary files.
void SprBinariesSave(void)
{
	// Definitions.
#if defined (CPU64)
	SprBinSave_sub("gfx/sprdef64.bin", (u8 *)gpSprDef, sizeof(struct SSprite) * gnSprNbSprites);
#else
	SprBinSave_sub("gfx/sprdef.bin", (u8 *)gpSprDef, sizeof(struct SSprite) * gnSprNbSprites);
#endif
	// Graphics.
	SprBinSave_sub("gfx/sprbuf.bin", gpSprBuf, gnSprBufSz);
	// Palletes.
	SprBinSave_sub("gfx/sprpal.bin", gpSprPal3Bytes, gnSprRemapPalettesNb * 3);

}
#else
u8 * SprBinLoad_sub(char *pFilename, u32 *pnSz)
{
	FILE	*fPt;
	u32	nSz1, nSz2, nSz3;
	u8	*pBuf;
	u32	nChkRead, nChkCalc;

	// Definitions.
	fPt = sec_fopen(pFilename, "rb");
	fseek(fPt, 0, SEEK_END);
	nSz1 = ftell(fPt);
	// Size <= checksum size?
	if (nSz1 <= sizeof(u32))
	{
		fprintf(stderr, "SprBinariesLoad(): %s: Wrong file size (%d bytes).\n", pFilename, (int)nSz1);
		fclose(fPt);
		exit (1);
	}
	nSz1 -= sizeof(u32);
	rewind(fPt);
	// malloc.
	if ((pBuf = (u8 *)malloc(nSz1)) == NULL)
	{
		fprintf(stderr, "SprBinariesLoad(): %s: Error allocating memory.\n", pFilename);
		fclose(fPt);
		exit (1);
	}
	// Reading.
	nSz2 = fread(pBuf, 1, nSz1, fPt);
	nSz3 = fread(&nChkRead, 1, sizeof(u32), fPt);
	fclose(fPt);
	if (nSz1 != nSz2)
	{
		fprintf(stderr, "SprBinariesLoad(): %s: Error, wrong file size loaded (%d bytes loaded, %d bytes expected).\n", pFilename, (int)nSz2, (int)nSz1);
		exit (1);
	}
	if (nSz3 != sizeof(u32))
	{
		fprintf(stderr, "SprBinariesLoad(): %s: Error while loading checksum, wrong size.\n", pFilename);
		exit (1);
	}
	// Checksum ok ?
	nChkCalc = SprChecksum(pBuf, nSz1);
	if (nChkCalc != nChkRead)
	{
		fprintf(stderr, "SprBinariesLoad(): %s: Checksum error (calc: %x, read: %x).\n", pFilename, (int)nChkCalc, (int)nChkRead);
		exit (1);
	}

	*pnSz = nSz1;
	return (pBuf);
}
// Read sprite binary files.
void SprBinariesLoad(void)
{
	u32	nSz;

	// Definitions.
#if defined (CPU64)
	gpSprDef = (struct SSprite *)SprBinLoad_sub("gfx/sprdef64.bin", &nSz);
#else
	gpSprDef = (struct SSprite *)SprBinLoad_sub("gfx/sprdef.bin", &nSz);
#endif
	gnSprNbSprites = nSz / sizeof(struct SSprite);
  #ifdef DEBUG_INFO
printf("SprLoad: gnSprNbSprites=%d\n", gnSprNbSprites);
  #endif
	// Graphics.
	gpSprBuf = SprBinLoad_sub("gfx/sprbuf.bin", &nSz);
	// Palettes.
	gpSprPal3Bytes = SprBinLoad_sub("gfx/sprpal.bin", &nSz);
	gnSprRemapPalettesNb = nSz / 3;
  #ifdef DEBUG_INFO
printf("SprLoad: gnSprRemapPalettesNb=%d\n", gnSprRemapPalettesNb);
  #endif

}
#endif

// Complete capture, set pointers (once!).
void SprEndCapture(void)
{
	u32	i;
	u32	nLgMax, nHtMax;		// For allocation of the flip buffer.

	nLgMax = 0;
	nHtMax = 0;
	for (i = 0; i < gnSprNbSprites; i++)
	{
		// Replaces the pointer (offset saved in the read union).
		gpSprDef[i].pGfx8 = gpSprBuf + gpSprDef[i].nGfx8Offset;
		// Search lg and ht max.
		if (gpSprDef[i].nLg > nLgMax) nLgMax = gpSprDef[i].nLg;
		if (gpSprDef[i].nHt > nHtMax) nHtMax = gpSprDef[i].nHt;
	}
#ifdef DEBUG_INFO
printf("Spr biggest sz: lg=%d ht=%d\n", (int)nLgMax, (int)nHtMax);
#endif
	// For Roto/Zooms management, we provide at least the size of the rotation rendering buffer.
	if (nLgMax < ROT2D_BUF_Width) nLgMax = ROT2D_BUF_Width;
	if (nHtMax < ROT2D_BUF_Height) nHtMax = ROT2D_BUF_Height;

	// Allocation of a buffer to depackage sprites from 8 to 16 bits, + mask generation.
	if (nLgMax == 0 || nHtMax == 0)
	{
		fprintf(stderr, "SprEndCapture(): Zero max with/height found. Aborted.\n");
		exit(1);
	}
	if ((gpSprFlipBuf = (u16 *)malloc(nLgMax * nHtMax * sizeof(u16) * 2)) == NULL)
	{
		fprintf(stderr, "SprEndCapture(): malloc failed (gpSprFlipBuf).\n");
		exit(1);
	}
	if ((gpRotBuf = (u8 *)malloc(ROT2D_BUF_Width * ROT2D_BUF_Height * sizeof(u8))) == NULL)
	{
		fprintf(stderr, "SprEndCapture(): malloc failed (gpRotBuf).\n");
		exit(1);
	}

	// Alloc memory remapping palettes.
	if ((gpSprRemapPalettes = (u16 *)malloc(gnSprRemapPalettesNb * SPR_PAL_SZ * sizeof(u16))) == NULL)
	{
		printf("SprEndCapture(): malloc failed (gpSprRemapPalettes).\n");
		exit(1);
	}
	SprPaletteConversion();		// RGB color conversion > u16.

#ifdef DEBUG_INFO
printf("Spr biggest sz (2): lg=%d ht=%d\n", (int)nLgMax, (int)nHtMax);
printf("Total mem: Allocated = %d / used = %d\n", (int)gnSprBufAllocSz, (int)gnSprBufSz);	// debug
printf("gnSprRemapPalettesNb=%d\n", gnSprRemapPalettesNb);
#endif

}

// Freeing (once!).
void SprRelease(void)
{
	free(gpSprBuf);		// Data.
	free(gpSprDef);		// Definitions.
	free(gpSprRemapPalettes);	// Remapping palettes.
	free(gpSprPal3Bytes);		// Colors on 3 bytes.
	free(gpSprFlipBuf);	// Buffer to create flipped images.
	free(gpRotBuf);		// Buffer to generate rotated/zoomed images.

}


#if SPR_SAVE == 1
// Realloc sprite definitions when all available structures have been filled.
void SprDefRealloc(void)
{
	gnSprNbSprDefMax += SPRDEF_ALLOC_UNIT;
	printf("SprDefRealloc(): New size=%d.\n", (int)gnSprNbSprDefMax);

	gpSprDef = (struct SSprite *)realloc(gpSprDef, gnSprNbSprDefMax * sizeof(struct SSprite));
	if (gpSprDef == NULL)
	{
		printf("SprDefRealloc: realloc failed.\n");
		SprRelease();
		exit(1);
	}
	memset((u8 *)gpSprDef + ((gnSprNbSprDefMax - SPRDEF_ALLOC_UNIT) * sizeof(struct SSprite)), 0, SPRDEF_ALLOC_UNIT * sizeof(struct SSprite));
}


// Realloc sprite data.
void SprBufRealloc(void)
{
	gnSprBufAllocSz += SPRBUF_ALLOC_UNIT;
	printf("SprBufRealloc(): New size=%d.\n", (int)gnSprBufAllocSz);

//	gpSprBuf = (u16 *)realloc(gpSprBuf, gnSprBufAllocSz);
	gpSprBuf = (u8 *)realloc(gpSprBuf, gnSprBufAllocSz);
	if (gpSprBuf == NULL)
	{
		printf("SprBufRealloc(): realloc failed.\n");
		SprRelease();
		exit(1);
	}
}
#endif

// Returns a ptr on a remapping palette.
u16 * SprRemapPalGet(u32 nPalNo)
{
	return (gpSprRemapPalettes + (nPalNo * SPR_PAL_SZ));
}

/*
// Allocates memory for a new remapping palette.
// In: nNbPalToAdd = Number of additional x color palettes to allocate.
u16 * SprRemapPalAlloc(u32 nNbPalToAdd)
{
	// Alloc memory remapping palettes.
	if ((gpSprRemapPalettes = (u16 *)realloc(gpSprRemapPalettes, (gnSprRemapPalettesNb + nNbPalToAdd) * SPR_PAL_SZ * sizeof(u16))) == NULL)
	{
		printf("SprRemapPalAlloc(): realloc failed.\n");
		exit(1);
	}
	gnSprRemapPalettesNb += nNbPalToAdd;
//printf("Palettes sz=%d bytes.\n", (int)(gnSprRemapPalettesNb * SPR_PAL_SZ * sizeof(u16)) );

	return (SprRemapPalGet(gnSprRemapPalettesNb - nNbPalToAdd));
}
*/

// Allocates memory for sprite palettes.
// In: nNbPalToAdd = Number of additional palettes of x colors to allocate.
u8 * SprPal3BytesAlloc(u32 nNbPalToAdd)
{
	// Alloc mémoire pour palettes RGB.
	if ((gpSprPal3Bytes = (u8 *)realloc(gpSprPal3Bytes, (gnSprRemapPalettesNb + nNbPalToAdd) * SPR_PAL_SZ * 3)) == NULL)
	{
		printf("SprPal3BytesAlloc(): realloc failed.\n");
		exit(1);
	}
	gnSprRemapPalettesNb += nNbPalToAdd;

	return (gpSprPal3Bytes + ((gnSprRemapPalettesNb - nNbPalToAdd) * SPR_PAL_SZ * 3));
}

// Converts 3-byte RGB palette to 16-bit SDL colors.
// Separate conversion so that it can be redone when video mode is changed.
void SprPaletteConversion(void)
{
	u32	i;

	for (i = 0; i < gnSprRemapPalettesNb; i++)
		*(gpSprRemapPalettes + i) = SDL_MapRGB(gVar.pScreen->format,
			gpSprPal3Bytes[(i * 3)],
			gpSprPal3Bytes[(i * 3) + 1],
			gpSprPal3Bytes[(i * 3) + 2]);

}

#if SPR_SAVE == 1
// Retrieve sprites from a board.
void SprLoadBMP(char *pFilename)
{
	SDL_Surface	*pPlanche;
	u32	nNbSprPlanche = 0;
//	u16	*pRemapRGB;		// Table for remapping 16-bit color indexes.
	u32	ix, iy;
	u8	nBkgClr;		// No. of the background color.

	#if	SPRPAL_SUB_ON == 1
	u8	nClrMax = 0;	// Max board color for no. of x color palettes to save.
	#else
	u8	nClrMax = 255;	// Max board color for no. of x color palettes to save.
	#endif

	// Read BMP.
	pPlanche = SDL_LoadBMP(pFilename);
	if (pPlanche == NULL) {
		fprintf(stderr, "Couldn't load picture: %s\n", SDL_GetError());
		exit(1);
	}
	//printf("Load ok!\n");

/*
	// Create remapping table.
	pRemapRGB = SprRemapPalAlloc(1);
	for (ix = 0; ix < 256; ix++)
	{
		pRemapRGB[ix] = SDL_MapRGB(gVar.pScreen->format,
			pPlanche->format->palette->colors[ix].r,
			pPlanche->format->palette->colors[ix].g,
			pPlanche->format->palette->colors[ix].b);
	}
	pRemapRGB[0] = 0;	// Color 0 to 0, as used in OR display.
*/

	// Scan the board to extract sprites.
	u8	*pPix = (u8 *)pPlanche->pixels;
	nBkgClr = *pPix;		// No. of the background color.
//printf("bkg clr idx=%d\n", nBkgClr);
	#if SPRGRAB_DISPLAY_INFO == 1
	printf("w = %d / h = %d\n", pPlanche->w, pPlanche->h);
	#endif

	for (iy = 0; iy < (u32)pPlanche->h; iy++)
	{
		for (ix = 0; ix < (u32)pPlanche->w; ix++)
		{
			// We come across a sprite?
			if (*(pPix + (iy * pPlanche->pitch) + ix) == 0)
			{
				// Do we still have room?
				if (gnSprNbSprites >= gnSprNbSprDefMax) SprDefRealloc();

				#if SPRGRAB_DISPLAY_INFO == 1
				printf("> Sprite at (%d, %d).\n", (int)ix, (int)iy);
				#endif

				u32	LgExt, HtExt;
				u32	PtRefX, PtRefY;		// Ref. points
				u32	ii, ij, ik;

				// Search for outside widths (1-pixel frame). + Ref points (notches at top and left side).
				PtRefX = 0;
				LgExt = 1;
				ii = ix + 1;
				while (*(pPix + (iy * pPlanche->pitch) + ii) == 0 || *(pPix + (iy * pPlanche->pitch) + ii + 1) == 0)
				{
					if (*(pPix + (iy * pPlanche->pitch) + ii) != 0) PtRefX = LgExt - 1;
					ii++;
					LgExt++;
				}

				PtRefY = 0;
				HtExt = 1;
				ii = iy + 1;
				while(*(pPix + (ii * pPlanche->pitch) + ix) == 0 || *(pPix + ((ii + 1) * pPlanche->pitch) + ix) == 0)
				{
					if (*(pPix + (ii * pPlanche->pitch) + ix) != 0) PtRefY = HtExt - 1;
					ii++;
					HtExt++;
				}
				#if SPRGRAB_DISPLAY_INFO == 1
				printf("lg ext = %d / ht ext = %d / ref point (%d, %d).\n", (int)LgExt, (int)HtExt, (int)PtRefX, (int)PtRefY);
				#endif

				// Store values.
				gpSprDef[gnSprNbSprites].nPtRefX = PtRefX;
				gpSprDef[gnSprNbSprites].nPtRefY = PtRefY;
				gpSprDef[gnSprNbSprites].nLg = LgExt - 2;
				gpSprDef[gnSprNbSprites].nHt = HtExt - 2;

				// Realloc buffer.
				u8	*pSpr8Gfx;
				u32	nSprBufAddSz;

				nSprBufAddSz = gpSprDef[gnSprNbSprites].nLg * gpSprDef[gnSprNbSprites].nHt;	// spr size in 8 bits.
				while (gnSprBufSz + nSprBufAddSz >= gnSprBufAllocSz) SprBufRealloc();	// Realloc when necessary.

				// Keep indexes in ptrs (union) for final reassignment AFTER all sprites have been read (with realloc, the block can move in memory).
				gpSprDef[gnSprNbSprites].nGfx8Offset = gnSprBufSz;
//				gpSprDef[gnSprNbSprites].nRemapPalNo = gnSprRemapPalettesNb - 1;	// Remapping palette number.
				gpSprDef[gnSprNbSprites].nRemapPalNo = gnSprRemapPalettesNb;	// Remapping palette number.

				pSpr8Gfx = gpSprBuf + gpSprDef[gnSprNbSprites].nGfx8Offset;
				// Sz.
				gnSprBufSz += nSprBufAddSz;

				// reset zones.
				for (ik = 0; ik < SPRRECT_MAX_ZONES; ik++)
				{
					gpSprDef[gnSprNbSprites].pRect[ik].nType = e_SprRect_NDef;
				}

				// Sprite retrieval.
				ik = 0;
				for (ij = 0; ij < HtExt - 2; ij++)
				{
					for (ii = 0; ii < LgExt - 2; ii++)
					{
						pSpr8Gfx[ik] = *(pPix + ((iy + ij + 1) * pPlanche->pitch) + (ix + ii + 1));	// Index de la couleur.
						#if	SPRPAL_SUB_ON == 1
						if (pSpr8Gfx[ik] > nClrMax) nClrMax = pSpr8Gfx[ik];
						#endif
						ik++;
					}
				}

				// Deletion of the sprite in the original plate.
				for (ij = 0; ij < HtExt; ij++)
				{
					for (ii = 0; ii < LgExt; ii++)
					{
						*(pPix + ((iy + ij) * pPlanche->pitch) + (ix + ii)) = nBkgClr;
					}
				}

				// Finished.
				nNbSprPlanche++;
				gnSprNbSprites++;

			}

		}
	}

	// Create remapping table.
	u32	nNbPal;
	#if	SPRPAL_SUB_ON == 1
	nNbPal = (nClrMax / SPR_PAL_SZ) + 1;
//printf("Color max: %d / Nb of palettes: %d.\n", (int)nClrMax, (int)nNbPal);
	#else
	nNbPal = 1;
	#endif
/*
	pRemapRGB = SprRemapPalAlloc(nNbPal);
	for (ix = 1; ix <= nClrMax; ix++)
	{
		pRemapRGB[ix] = SDL_MapRGB(gVar.pScreen->format,
			pPlanche->format->palette->colors[ix].r,
			pPlanche->format->palette->colors[ix].g,
			pPlanche->format->palette->colors[ix].b);
	}
	pRemapRGB[0] = 0;	// Color 0 to 0, as used in OR display.
*/
	// Simple storage.
	u8	*pRGB3;
	pRGB3 = SprPal3BytesAlloc(nNbPal);
	for (ix = 1; ix <= nClrMax; ix++)
	{
		pRGB3[(ix * 3)] =     pPlanche->format->palette->colors[ix].r;
		pRGB3[(ix * 3) + 1] = pPlanche->format->palette->colors[ix].g;
		pRGB3[(ix * 3) + 2] = pPlanche->format->palette->colors[ix].b;
	}
	pRGB3[0] = pRGB3[1] = pRGB3[2] = 0;	// Color 0 to 0, as used in OR display.

	printf(">\nTotal sprites in '%s': %d.\n", pFilename, (int)nNbSprPlanche);
	printf("Total sprites: %d.\n>\n", (int)gnSprNbSprites);

	// We free up the surface.
	SDL_FreeSurface(pPlanche);

}

// Lecture d'une planche de sprites en PSD.
void SprLoadPSD(char *pFilename)
{
	struct SPSDPicture	*pPlanche;
	u32	nNbSprPlanche = 0;
//	u16	*pRemapRGB;		// Table pour remapper les index en couleurs 16 bits.
	s32	ix, iy;
	u8	nBkgClr;		// N° de la couleur de fond de la planche.

	#if	SPRPAL_SUB_ON == 1
	u8	nClrMax = 0;	// Couleur max de la planche pour nb de palettes de x couleurs à sauver.
	#else
	u8	nClrMax = 255;	// Couleur max de la planche pour nb de palettes de x couleurs à sauver.
	#endif

	// Lecture de la planche.
	printf("Loading PSD '%s'.\n", pFilename);
	pPlanche = PSDLoad(pFilename);
	if (pPlanche == NULL)
	{
		fprintf(stderr, "Error while loading PSD file '%s'.\n", pFilename);
		exit(1);
	}
	//printf("Load ok!\n");

/*
	// Création de la table de remappage.
	pRemapRGB = SprRemapPalAlloc(1);
	for (ix = 0; ix < 256; ix++)
	{
		pRemapRGB[ix] = SDL_MapRGB(gVar.pScreen->format,
			pPlanche->pColors[ix].r,
			pPlanche->pColors[ix].g,
			pPlanche->pColors[ix].b);
	}
	pRemapRGB[0] = 0;	// Couleur 0 à 0, car utilisée dans l'affichage avec un OR.
*/

	// On parcourt la planche pour en extraire les sprites.
	u8	*pPix = pPlanche->pPlanes;
	nBkgClr = *pPix;		// N° de la couleur de fond de la planche.
//printf("bkg clr idx=%d\n", nBkgClr);
	#if SPRGRAB_DISPLAY_INFO == 1
	printf("w = %d / h = %d\n", (int)pPlanche->nWidth, (int)pPlanche->nHeight);
	#endif

	for (iy = 0; iy < pPlanche->nHeight; iy++)
	{
		for (ix = 0; ix < pPlanche->nWidth; ix++)
		{
			// On tombe sur un sprite ?
			if (*(pPix + (iy * pPlanche->nWidth) + ix) != nBkgClr)
			{
				// On a encore de la place ?
				if (gnSprNbSprites >= gnSprNbSprDefMax) SprDefRealloc();

				#if SPRGRAB_DISPLAY_INFO == 1
				printf("> Sprite at (%d, %d).\n", (int)ix, (int)iy);
				#endif

				u32	nLgExt, nHtExt;
				s32	ii, ij, ik, ip;

				// Recherche des largeurs extérieures.
				nLgExt = 0;
				ii = ix;
				while(*(pPix + (iy * pPlanche->nWidth) + ii) != nBkgClr)
				{
					ii++;
					nLgExt++;
				}
				nHtExt = 0;
				ii = iy;
				while(*(pPix + (ii * pPlanche->nWidth) + ix) != nBkgClr)
				{
					ii++;
					nHtExt++;
				}
				#if SPRGRAB_DISPLAY_INFO == 1
				printf("lg ext = %d / ht ext = %d\n", (int)nLgExt, (int)nHtExt);
				#endif

				// Taille ok ? (inutile ? (le cadre n'est plus obligatoire))
				if (nLgExt < 3 || nHtExt < 3)
				{
					printf("Pic '%s' sprite #%d : Wrong box size (%d,%d). Aborted.\n", pFilename, (int)nNbSprPlanche, (int)nLgExt, (int)nHtExt);
					SprRelease();
					exit(1);
				}


				// Recadrage du sprite au plus près (bounding box).
				u32	nLgInt, nHtInt;
				s32	nBBoxX1, nBBoxY1, nBBoxX2, nBBoxY2;
				// Haut.
				for (nBBoxY1 = iy; nBBoxY1 < iy + nHtExt; nBBoxY1++)
				{
					for (ii = 0; ii < nLgExt; ii++) if (*(pPix + (nBBoxY1 * pPlanche->nWidth) + (ix + ii)) != 0) break;
					if (ii < nLgExt) break;
				}
				// Bas.
				for (nBBoxY2 = iy + nHtExt - 1; nBBoxY2 >= iy; nBBoxY2--)
				{
					for (ii = 0; ii < nLgExt; ii++) if (*(pPix + (nBBoxY2 * pPlanche->nWidth) + (ix + ii)) != 0) break;
					if (ii < nLgExt) break;
				}
				// Gauche.
				for (nBBoxX1 = ix; nBBoxX1 < ix + nLgExt; nBBoxX1++)
				{
					for (ii = 0; ii < nHtExt; ii++) if (*(pPix + ((iy + ii) * pPlanche->nWidth) + nBBoxX1) != 0) break;
					if (ii < nHtExt) break;
				}
				// Droite.
				for (nBBoxX2 = ix + nLgExt - 1; nBBoxX2 >= ix; nBBoxX2--)
				{
					for (ii = 0; ii < nHtExt; ii++) if (*(pPix + ((iy + ii) * pPlanche->nWidth) + nBBoxX2) != 0) break;
					if (ii < nHtExt) break;
				}
				#if SPRGRAB_DISPLAY_INFO == 1
				printf("ext: %d,%d - %d,%d > int: %d,%d - %d,%d\n",
					(int)ix, (int)iy, (int)(ix + nLgExt - 1), (int)(iy + nHtExt - 1),
					(int)nBBoxX1, (int)nBBoxY1, (int)nBBoxX2, (int)nBBoxY2);
				#endif

				// Si jamais on tombe sur un sprite vide, on reprend les mesures externes.
				if (nBBoxX2 <= nBBoxX1)
//				if (nBBoxX2 < nBBoxX1)
				{
					nBBoxX1 = ix;
					nBBoxX2 = ix + nLgExt - 1;
					printf("Pic '%s' sprite #%d: Empty sprite?\n", pFilename, (int)nNbSprPlanche);
				}
				if (nBBoxY2 <= nBBoxY1)	// == => sprite de 1 pixel.
//				if (nBBoxY2 < nBBoxY1)	// le < au lieu du <= fonctionne à la capture, mais ça pose un pb à l'affichage.
				{
					nBBoxY1 = iy;
					nBBoxY2 = iy + nHtExt - 1;
					printf("Pic '%s' sprite #%d: Empty sprite?\n", pFilename, (int)nNbSprPlanche);
				}

				nLgInt = nBBoxX2 - nBBoxX1 + 1;
				nHtInt = nBBoxY2 - nBBoxY1 + 1;
//printf("lgint = %d / htint = %d\n", nLgInt, nHtInt);


				//... Insérer ici le parcours de l'image si jamais il faut faire le stockage sur 4 bits.


				// !!! On ne capture que la bounding box !!!
				// !!! Par contre, on recherche dans les couches alpha sur la totalité du rectangle ext !!!


				// Stockage des valeurs.
				gpSprDef[gnSprNbSprites].nPtRefX = 0;
				gpSprDef[gnSprNbSprites].nPtRefY = 0;
				gpSprDef[gnSprNbSprites].nLg = nLgInt;
				gpSprDef[gnSprNbSprites].nHt = nHtInt;

				// Realloc buffer.
				u8	*pSpr8Gfx;
				u32	nSprBufAddSz;// = gpSprDef[gnSprNbSprites].nLg * gpSprDef[gnSprNbSprites].nHt * 2 * sizeof(u16);

				nSprBufAddSz = gpSprDef[gnSprNbSprites].nLg * gpSprDef[gnSprNbSprites].nHt;	// Taille spr en 8 bits.
				while (gnSprBufSz + nSprBufAddSz >= gnSprBufAllocSz) SprBufRealloc();	// Realloc quand nécessaire.

				// On garde les index dans les ptrs (union) pour réaffectation finale APRES lecture de tous les sprites (avec le realloc, le bloc peut bouger en mémoire).
				gpSprDef[gnSprNbSprites].nGfx8Offset = gnSprBufSz;
//				gpSprDef[gnSprNbSprites].nRemapPalNo = gnSprRemapPalettesNb - 1;	// N° de la palette de remappage.
				gpSprDef[gnSprNbSprites].nRemapPalNo = gnSprRemapPalettesNb;	// N° de la palette de remappage.

				pSpr8Gfx = gpSprBuf + gpSprDef[gnSprNbSprites].nGfx8Offset;
				// Sz.
				gnSprBufSz += nSprBufAddSz;

				// RAZ zones.
				for (ip = 0; ip < SPRRECT_MAX_ZONES; ip++)
				{
					gpSprDef[gnSprNbSprites].pRect[ip].nType = e_SprRect_NDef;
				}

				// Récupération du sprite.
				ik = 0;
				for (ij = 0; ij < nHtInt; ij++)
				{
					for (ii = 0; ii < nLgInt; ii++)
					{
						pSpr8Gfx[ik] = *(pPix + ((nBBoxY1 + ij) * pPlanche->nWidth) + (nBBoxX1 + ii));	// Index de la couleur.
						#if	SPRPAL_SUB_ON == 1
						if (pSpr8Gfx[ik] > nClrMax) nClrMax = pSpr8Gfx[ik];
						#endif
						ik++;
					}
				}

				// Boucle dans les plans alpha pour points, rectangles... (O = vide / 255 = plein).
				for (ip = 1; ip < pPlanche->nNbPlanes && ip <= SPRRECT_MAX_ZONES; ip++)
				{
//printf("plane %d\n", (int)ip);
					for (ij = 0; ij < nHtExt; ij++)
					{
						for (ii = 0; ii < nLgExt; ii++)
						{
//printf("%d ", *(pPix + ((iy + ij + 1 + (ip * pPlanche->nHeight)) * pPlanche->nWidth) + (ix + ii + 1)) );

							// Un pixel ?
							if (*(pPix + ((iy + ij + (ip * pPlanche->nHeight)) * pPlanche->nWidth) + (ix + ii)) )
							{
								s32	rii, rij;
								gpSprDef[gnSprNbSprites].pRect[ip - 1].nX1 = ii + (ix - nBBoxX1);
								gpSprDef[gnSprNbSprites].pRect[ip - 1].nY1 = ij + (iy - nBBoxY1);
								// Parcours sur le X.
								rii = ii;
								rij = ij;
								while (*(pPix + ((iy + rij + (ip * pPlanche->nHeight)) * pPlanche->nWidth) + (ix + rii)) && rii < nLgExt) rii++;
								gpSprDef[gnSprNbSprites].pRect[ip - 1].nX2 = rii - 1 + (ix - nBBoxX1);
								// Parcours sur le Y.
								rii = ii;
								rij = ij;
								while (*(pPix + ((iy + rij + (ip * pPlanche->nHeight)) * pPlanche->nWidth) + (ix + rii)) && rij < nHtExt) rij++;
								gpSprDef[gnSprNbSprites].pRect[ip - 1].nY2 = rij - 1 + (iy - nBBoxY1);
								// Pt ou rect ?
								gpSprDef[gnSprNbSprites].pRect[ip - 1].nType =
								(gpSprDef[gnSprNbSprites].pRect[ip - 1].nX1 == gpSprDef[gnSprNbSprites].pRect[ip - 1].nX2 &&
								gpSprDef[gnSprNbSprites].pRect[ip - 1].nY1 == gpSprDef[gnSprNbSprites].pRect[ip - 1].nY2 ? e_SprRect_Point : e_SprRect_Rect);

//printf("Type=%d / x1=%d / y1=%d / x2=%d / y2=%d\n",
//gpSprDef[gnSprNbSprites].pRect[ip - 1].nType,
//gpSprDef[gnSprNbSprites].pRect[ip - 1].nX1, pSpr[gnSprNbSprites].pRect[ip - 1].nY1,
//gpSprDef[gnSprNbSprites].pRect[ip - 1].nX2, pSpr[gnSprNbSprites].pRect[ip - 1].nY2);

								// Force la fin des boucles for.
								ii = nLgExt;
								ij = nHtExt;
							} // if pixel
						} // for ii
//printf("\n");
					} // for ij
				} // for couche
				// On prend le point de la première couche alpha comme point de référence.
				if (gpSprDef[gnSprNbSprites].pRect[0].nType == e_SprRect_Point)
				{
					gpSprDef[gnSprNbSprites].nPtRefX = gpSprDef[gnSprNbSprites].pRect[0].nX1;
					gpSprDef[gnSprNbSprites].nPtRefY = gpSprDef[gnSprNbSprites].pRect[0].nY1;
					#if SPRGRAB_DISPLAY_INFO == 1
					printf("Ref point - x=%d / y=%d\n", (int)gpSprDef[gnSprNbSprites].nPtRefX, (int)gpSprDef[gnSprNbSprites].nPtRefY);
					#endif
				}
				else
				{
					printf("Picture '%s' sprite #%d: No ref point found!\n", pFilename, (int)nNbSprPlanche);
				}
				// On fait une passe pour décaler tout par rapport au point de ref (Sauf la première couche alpha, évidement).
				for (ip = 1; ip < SPRRECT_MAX_ZONES; ip++)
				{
					gpSprDef[gnSprNbSprites].pRect[ip].nX1 -= gpSprDef[gnSprNbSprites].nPtRefX;
					gpSprDef[gnSprNbSprites].pRect[ip].nY1 -= gpSprDef[gnSprNbSprites].nPtRefY;
					gpSprDef[gnSprNbSprites].pRect[ip].nX2 -= gpSprDef[gnSprNbSprites].nPtRefX;
					gpSprDef[gnSprNbSprites].pRect[ip].nY2 -= gpSprDef[gnSprNbSprites].nPtRefY;
					#if SPRGRAB_DISPLAY_INFO == 1
					if (gpSprDef[gnSprNbSprites].pRect[ip].nType == e_SprRect_NDef)
						printf("alpha #%d - Not defined.\n", (int)ip);
					else
						printf("alpha #%d - Type=%d / x1=%d / y1=%d / x2=%d / y2=%d\n",
							(int)ip, (int)gpSprDef[gnSprNbSprites].pRect[ip].nType,
							(int)gpSprDef[gnSprNbSprites].pRect[ip].nX1, (int)gpSprDef[gnSprNbSprites].pRect[ip].nY1,
							(int)gpSprDef[gnSprNbSprites].pRect[ip].nX2, (int)gpSprDef[gnSprNbSprites].pRect[ip].nY2);
					#endif
				}

				// Effacement du sprite dans la planche originale.
				for (ij = 0; ij < nHtExt; ij++)
				{
					for (ii = 0; ii < nLgExt; ii++)
					{
						*(pPix + ((iy + ij) * pPlanche->nWidth) + (ix + ii)) = nBkgClr;
					}
				}

				// Terminé.
				nNbSprPlanche++;
				gnSprNbSprites++;

			}

		}
	}


	// Création de la table de remappage.
	u32	nNbPal;
	#if	SPRPAL_SUB_ON == 1
	nNbPal = (nClrMax / SPR_PAL_SZ) + 1;
//printf("Color max: %d / Nb of palettes: %d.\n", (int)nClrMax, (int)nNbPal);
	#else
	nNbPal = 1;
	#endif
/*
	pRemapRGB = SprRemapPalAlloc(nNbPal);
	for (ix = 1; ix <= nClrMax; ix++)
	{
		pRemapRGB[ix] = SDL_MapRGB(gVar.pScreen->format,
			pPlanche->pColors[ix].r,
			pPlanche->pColors[ix].g,
			pPlanche->pColors[ix].b);
	}
	pRemapRGB[0] = 0;	// Couleur 0 à 0, car utilisée dans l'affichage avec un OR.
*/
	// Stockage simple.
	u8	*pRGB3;
	pRGB3 = SprPal3BytesAlloc(nNbPal);
	for (ix = 1; ix <= nClrMax; ix++)
	{
		pRGB3[(ix * 3)] =     pPlanche->pColors[ix].r;
		pRGB3[(ix * 3) + 1] = pPlanche->pColors[ix].g;
		pRGB3[(ix * 3) + 2] = pPlanche->pColors[ix].b;
	}
	pRGB3[0] = pRGB3[1] = pRGB3[2] = 0;	// Couleur 0 à 0, car utilisée dans l'affichage avec un OR.

	printf(">\nTotal sprites in '%s': %d.\n", pFilename, (int)nNbSprPlanche);
	printf("Total sprites: %d.\n>\n", (int)gnSprNbSprites);


	// Free.
	free(pPlanche->pPlanes);
	free(pPlanche);
}


// Lecture des sprites.
void SprLoadGfx(char *pFilename)//, SDL_Color *pSprPal, u32 nPalIdx)
{
	// On regarde l'extension du fichier.
	if (stricmp(pFilename + strlen(pFilename) - 3, "bmp") == 0)
	{
		// Extraction des sprites depuis un BMP.
		SprLoadBMP(pFilename);//pSprPal, nPalIdx);
	}
	else
	if (stricmp(pFilename + strlen(pFilename) - 3, "psd") == 0)
	{
		// Extraction des sprites depuis un PSD.
		SprLoadPSD(pFilename);//, pSprPal, nPalIdx);
	}
	else
	{
		printf("Unrecognised file format: %s\n", pFilename);
	}

}
#endif

// Returns a ptr for a sprite descriptor.
struct SSprite *SprGetDesc(u32 nSprNo)
{
	return (&gpSprDef[nSprNo & ~(SPR_Flip_X | SPR_Flip_Y | SPR_Flag_HitPal)]);
}



// Récupère des pointeurs sur l'image du sprite et son masque.
// L'image est dépackée (8 bits > 16 bits) et le masque généré.
void SprGetGfxMskPtr(u32 nSprFlags, u16 **ppGfx, u16 **ppMsk, struct SSprite *pSprDesc, struct SSprStockage *pSprSto)
{
	s32	i, j, nSz;
	u16	*pDstG, *pDstM, *pPal;
	u8	*pSrc8;

	nSz = pSprDesc->nLg * pSprDesc->nHt;

	#if CACHE_ON == 1
	// Cache pas pris en compte pour les rotations/zoom.
	if (pSprSto->pFct != NULL)
	{
		((pRZFctRender)pSprSto->pFct)();	// Appelle le rendu du zoom ou du rotozoom.
		*ppGfx = pDstG = gpSprFlipBuf;
		*ppMsk = pDstM = gpSprFlipBuf + nSz;
	}
	else
	{
		// Avec cache.
		i = CacheGetMem(nSprFlags, nSz, ppGfx);
		*ppMsk = *ppGfx + nSz;
		if (i == e_Cache_Hit) return;
		pDstG = *ppGfx;
		pDstM = *ppMsk;
	}
	#else
	// Sans cache
	if (pSprSto->pFct != NULL)
	{
		((pRZFctRender)pSprSto->pFct)();	// Appelle le rendu du zoom ou du rotozoom.
	}
	*ppGfx = pDstG = gpSprFlipBuf;
	*ppMsk = pDstM = gpSprFlipBuf + nSz;
	//*** Sans cache, on doit pouvoir se passer des paramètres ppGfx et ppMsk.
	#endif


	pPal = SprRemapPalGet(pSprDesc->nRemapPalNo);//gpSprDef[nSprNo].nRemapPalNo);	// Palette de remappage du sprite.

	// "Depack" du sprite (8 => 16 bits).
	if ((nSprFlags & (SPR_Flip_X | SPR_Flip_Y)) == 0)
	{
		// Pas de flips.
		pSrc8 = pSprDesc->pGfx8;
		i = 0;
		for (i = 0; i < nSz; i++)
		{
			*pDstG++ = *(pPal + *pSrc8);
			*pDstM++ = (*(pSrc8++) ? 0 : 0xFFFF);
		}
	}
	else if ((nSprFlags & (SPR_Flip_X | SPR_Flip_Y)) == (SPR_Flip_X | SPR_Flip_Y))
	{
		// Flip x et y.
		pSrc8 = pSprDesc->pGfx8 + nSz - 1;
		for (i = 0; i < nSz; i++)
		{
			*pDstG++ = *(pPal + *pSrc8);
			*pDstM++ = (*(pSrc8--) ? 0 : 0xFFFF);
		}
	}
	else if (nSprFlags & SPR_Flip_Y)
	{
		// Flip y.
		for (j = pSprDesc->nHt - 1; j >= 0; j--)
		{
			pSrc8 = pSprDesc->pGfx8 + (j * pSprDesc->nLg);
			for (i = 0; i < pSprDesc->nLg; i++)
			{
				*pDstG++ = *(pPal + *pSrc8);
				*pDstM++ = (*(pSrc8++) ? 0 : 0xFFFF);
			}
		}
	}
	else
	{
		// Flip x.
		for (j = 0; j < pSprDesc->nHt; j++)
		{
			pSrc8 = pSprDesc->pGfx8 + ((j + 1) * pSprDesc->nLg) - 1;
			for (i = 0; i < pSprDesc->nLg; i++)
			{
				*pDstG++ = *(pPal + *pSrc8);
				*pDstM++ = (*(pSrc8--) ? 0 : 0xFFFF);
			}
		}
	}

}

// Affichage d'un sprite.
// Avec écran locké.
void SprDisplayLock(struct SSprStockage *pSprSto)
{
	s32	nXMin, nXMax, nYMin, nYMax;
	s32	nPtRefX, nPtRefY;
	s32	nSprXMin, nSprXMax, nSprYMin, nSprYMax;
	s32	diff;
	u16	*pScr;
	struct SSprite *pSprDesc;

	u32	nSprFlags = pSprSto->nSprNo;		// Pour conserver les flags.

	// Descripteur de sprite.
	if (pSprSto->pFct == NULL)
	{
		// Sprite standard.
		pSprDesc = &gpSprDef[nSprFlags & ~(SPR_Flag_HitPal | SPR_Flip_X | SPR_Flip_Y)];
	}
	else
	{
		// Sprite roto/zoomé, appel de la fonction de pré-rendu qui va bien.
		pSprDesc = ((pRZFctPreRender)pSprSto->pFct)(nSprFlags, pSprSto->nZoomX, pSprSto->nZoomY, &pSprSto->pFct);
		if (pSprDesc == NULL) return;	// Il y a eu un pb, abort.
	}

	// Point de ref.
	nPtRefX = pSprDesc->nPtRefX;
	nPtRefY = pSprDesc->nPtRefY;
	// Décalage pt de ref selon les flags (flip x : refX = nLg - refX).
//	if (nSprFlags & SPR_Flip_X) nPtRefX = pSprDesc->nLg - nPtRefX;
//	if (nSprFlags & SPR_Flip_Y) nPtRefY = pSprDesc->nHt - nPtRefY;
	if (nSprFlags & SPR_Flip_X) nPtRefX = (pSprDesc->nLg - 1) - nPtRefX;
	if (nSprFlags & SPR_Flip_Y) nPtRefY = (pSprDesc->nHt - 1) - nPtRefY;

	// Préparation du tracé.
	nXMin = pSprSto->nPosX - nPtRefX;
	nXMax = nXMin + pSprDesc->nLg - 1;
	nYMin = pSprSto->nPosY - nPtRefY;
	nYMax = nYMin + pSprDesc->nHt - 1;

	nSprXMin = 0;
	nSprXMax = pSprDesc->nLg - 1;
	nSprYMin = 0;
	nSprYMax = pSprDesc->nHt - 1;

	// Clips.
	if (nXMin < 0)	//aaa0
	{
		diff = 0 - nXMin;	//aaa0
		nSprXMin += diff;
	}
	if (nXMax > SCR_Width - 1)
	{
		diff = nXMax - (SCR_Width - 1);
		nSprXMax -= diff;
	}
	// Sprite complètement en dehors ?
//	if (nSprXMin - nSprXMax >= 0) return;	//< bug
	if (nSprXMin - nSprXMax > 0) return;
	//
	if (nYMin < 0)	//aaa0
	{
		diff = 0 - nYMin;	//aaa0
		nSprYMin += diff;
	}
	if (nYMax > SCR_Height - 1)
	{
		diff = nYMax - (SCR_Height - 1);
		nSprYMax -= diff;
	}
	// Sprite complètement en dehors ?
//	if (nSprYMin - nSprYMax >= 0) return;	//< bug
	if (nSprYMin - nSprYMax > 0) return;


	s32	ix, iy;
	u32	b4, /*b1,*/ b4b, b1b;
	u16	*pGfx, *pMsk;
//	u32	nScrLg = gVar.pScreen->pitch / sizeof(u16);
	s32	nScrLg = gVar.pScreen->pitch / sizeof(u16);	// Bugfix 11/10/2012. u32 > s32, car unsigned * signed = unsigned. Et il faut le sign extend en 64 bits !

	SprGetGfxMskPtr(nSprFlags, &pGfx, &pMsk, pSprDesc, pSprSto);

	b1b = nSprXMax - nSprXMin + 1;
//8	b4b = b1b >> 2;		// Nb de quads.
//8	b1b &= 3;			// Nb d'octets restants ensuite.
	b4b = b1b >> 1;		// Nb de quads.
	b1b &= 1;			// Nb de words restants ensuite.
	pScr = (u16 *)gVar.pScreen->pixels;
//l	pScr += ((nYMin + nSprYMin) * SCR_Width) + nXMin;
	pScr += ((nYMin + nSprYMin) * nScrLg) + nXMin;
	pMsk += (nSprYMin * pSprDesc->nLg);
	pGfx += (nSprYMin * pSprDesc->nLg);

	if (nSprFlags & SPR_Flag_HitPal)
	{
		// Affichage sprite rougi pour le Hit.

//u32	tst = (gVar.pScreen->format->Rmask << 16) | gVar.pScreen->format->Rmask;
//u32	tst2 = ((gVar.pScreen->format->Gmask >> 1) & gVar.pScreen->format->Gmask) | ((gVar.pScreen->format->Bmask >> 1) & gVar.pScreen->format->Bmask);
//tst2 |= tst2 << 16;

//u32	tst3 = (gVar.pScreen->format->Rmask << 16) | gVar.pScreen->format->Rmask;
u32	tst3 = gVar.pScreen->format->Rmask | ((gVar.pScreen->format->Gmask >> 2) & gVar.pScreen->format->Gmask);
tst3 |= tst3 << 16;

		for (iy = nSprYMin; iy <= nSprYMax; iy++)
		{
			b4 = b4b;

//A			for (ix = nSprXMin; b4 && *(u32 *)(pMsk + ix) == 0xFFFFFFFF; b4--, ix += 2);	// Skippe les premiers pixels vides.
//A			for (; b4; b4--, ix += 2)
//A = timé, c'est kif kif, peut-être même pire, c'est dur à dire.
			for (ix = nSprXMin; b4; b4--, ix += 2)
			{
				*(u32 *)(pScr + ix) &= *(u32 *)(pMsk + ix);
//				*(u32 *)(pScr + ix) |= *(u32 *)(pGfx + ix);
				//	*(u32 *)(pScr + ix) |= *(u32 *)(pGfx + ix) |0x0F0F0F0F;	// Pour voir l'affichage.

//*(u32 *)(pScr + ix) |= *(u32 *)(pGfx + ix) & tst;	// juste en gardant le rouge.
//*(u32 *)(pScr + ix) |= ((*(u32 *)(pGfx + ix) >> 1) & tst2) | (*(u32 *)(pGfx + ix) & tst);	// rougi.
*(u32 *)(pScr + ix) |= *(u32 *)(pGfx + ix) | (~*(u32 *)(pMsk + ix) & tst3);	// rouge 2
			}
			if (b1b)	// Un dernier pixel ?
			{
				*(pScr + ix) &= *(pMsk + ix);
//				*(pScr + ix) |= *(pGfx + ix);

//*(pScr + ix) |= *(pGfx + ix) & tst;	// juste en gardant le rouge.
//*(pScr + ix) |= ((*(pGfx + ix) >> 1) & tst2) | (*(pGfx + ix) & tst);	// rougi.
*(pScr + ix) |= *(pGfx + ix) | (~*(pMsk + ix) & tst3);	// rouge 2
			}
//l			pScr += SCR_Width;
			pScr += nScrLg;
			pMsk += pSprDesc->nLg;
			pGfx += pSprDesc->nLg;
		}


	}
	else
	{
		// Affichage normal.

//u32	tst = (gVar.pScreen->format->Rmask << 16) | gVar.pScreen->format->Rmask;
//u32	tst2 = ((gVar.pScreen->format->Gmask >> 1) & gVar.pScreen->format->Gmask) | ((gVar.pScreen->format->Bmask >> 1) & gVar.pScreen->format->Bmask);
//tst2 |= tst2 << 16;
//u32	tst3 = (gVar.pScreen->format->Rmask << 16) | gVar.pScreen->format->Rmask;

		for (iy = nSprYMin; iy <= nSprYMax; iy++)
		{
			b4 = b4b;

//A			for (ix = nSprXMin; b4 && *(u32 *)(pMsk + ix) == 0xFFFFFFFF; b4--, ix += 2);	// Skippe les premiers pixels vides.
//A			for (; b4; b4--, ix += 2)
//A = timé, c'est kif kif, peut-être même pire, c'est dur à dire.
			for (ix = nSprXMin; b4; b4--, ix += 2)
			{
				*(u32 *)(pScr + ix) &= *(u32 *)(pMsk + ix);
				*(u32 *)(pScr + ix) |= *(u32 *)(pGfx + ix);
			//	*(u32 *)(pScr + ix) |= *(u32 *)(pGfx + ix) |0x0F0F0F0F;	// Pour voir l'affichage.

//*(u32 *)(pScr + ix) |= *(u32 *)(pGfx + ix) & tst;	// juste en gardant le rouge.
//*(u32 *)(pScr + ix) |= ((*(u32 *)(pGfx + ix) >> 1) & tst2) | (*(u32 *)(pGfx + ix) & tst);	// rougi.
//*(u32 *)(pScr + ix) |= *(u32 *)(pGfx + ix) | (~*(u32 *)(pMsk + ix) & tst3);	// rouge 2
			}
			if (b1b)	// Un dernier pixel ?
			{
				*(pScr + ix) &= *(pMsk + ix);
				*(pScr + ix) |= *(pGfx + ix);

//*(pScr + ix) |= *(pGfx + ix) & tst;	// juste en gardant le rouge.
//*(pScr + ix) |= ((*(pGfx + ix) >> 1) & tst2) | (*(pGfx + ix) & tst);	// rougi.
//*(pScr + ix) |= *(pGfx + ix) | (~*(pMsk + ix) & tst3);	// rouge 2
			}
//l			pScr += SCR_Width;
			pScr += nScrLg;
			pMsk += pSprDesc->nLg;
			pGfx += pSprDesc->nLg;
		}

	}

}


// Macros pour éviter des calls :
#define	SPR_ADD_TO_LIST(POSX, POSY, PRIO, FPTR) \
	if (gnSprSto >= SPR_STO_MAX) { fprintf(stderr, "Sprites: Out of slots!\n"); return; } \
	if ((nSprNo & ~(SPR_Flip_X | SPR_Flip_Y)) == SPR_NoSprite) return; \
	gpSprSto[gnSprSto].nSprNo = nSprNo; \
	gpSprSto[gnSprSto].nPosX = POSX; \
	gpSprSto[gnSprSto].nPosY = POSY; \
	gpSprSto[gnSprSto].nPrio = PRIO; \
	gpSprSto[gnSprSto].pFct = FPTR;
#define	SPR_ADD_TO_LIST_RZ(ZOOMX, ZOOMY) \
	gpSprSto[gnSprSto].nZoomX = ZOOMX; \
	gpSprSto[gnSprSto].nZoomY = ZOOMY;
#define	SPR_ADD_TO_LIST_END \
	gpSprSort[gnSprSto] = &gpSprSto[gnSprSto]; \
	gnSprSto++;

// Inscrit les sprites dans une liste, position relative par rapport à la map.
void SprDisplay(u32 nSprNo, s32 nPosX, s32 nPosY, u16 nPrio)
{
	SPR_ADD_TO_LIST(nPosX - (gScrollPos.nPosX >> 8), nPosY - (gScrollPos.nPosY >> 8), nPrio, NULL)
	SPR_ADD_TO_LIST_END
}
void SprDisplayZoom(u32 nSprNo, s32 nPosX, s32 nPosY, u16 nPrio, u16 nZoomX, u16 nZoomY)
{
	// Pas de zoom ? => On envoie un sprite normal.
	if (nZoomX == 0x0100 && nZoomY == 0x0100)
	{
		SprDisplay(nSprNo, nPosX, nPosY, nPrio);
		return;
	}
	// Stockage.
	SPR_ADD_TO_LIST(nPosX - (gScrollPos.nPosX >> 8), nPosY - (gScrollPos.nPosY >> 8), nPrio, (void *)SprZoom_PreRender)
	SPR_ADD_TO_LIST_RZ(nZoomX, nZoomY)
	SPR_ADD_TO_LIST_END
}
// Sprites en rotation.
void SprDisplayRotoZoom(u32 nSprNo, s32 nPosX, s32 nPosY, u16 nPrio, u8 nAngle, u16 nZoom)
{
	// Pas de rot ? => On envoie un sprite zoomé simple.
	if (nAngle == 0)
	{
		SprDisplayZoom(nSprNo, nPosX, nPosY, nPrio, nZoom, nZoom);
		return;
	}
	// Angle == 128 => On envoie un sprite zoomé simple avec flips X et Y.
	if (nAngle == 128)
	{
		SprDisplayZoom(nSprNo ^ SPR_Flip_X ^ SPR_Flip_Y, nPosX, nPosY, nPrio, nZoom, nZoom);
		return;
	}
	// Stockage.
	SPR_ADD_TO_LIST(nPosX - (gScrollPos.nPosX >> 8), nPosY - (gScrollPos.nPosY >> 8), nPrio, (void *)SprRotoZoom_PreRender)
	SPR_ADD_TO_LIST_RZ(nZoom, nAngle)	// > union sur nAngle et nZoomY.
	SPR_ADD_TO_LIST_END
}

// Pareil, mais position absolue.
void SprDisplayAbsolute(u32 nSprNo, s32 nPosX, s32 nPosY, u16 nPrio)
{
	SPR_ADD_TO_LIST(nPosX, nPosY, nPrio, NULL)
	SPR_ADD_TO_LIST_END
}
void SprDisplayZoomAbsolute(u32 nSprNo, s32 nPosX, s32 nPosY, u16 nPrio, u16 nZoomX, u16 nZoomY)
{
	// Pas de zoom ? => On envoie un sprite normal.
	if (nZoomX == 0x0100 && nZoomY == 0x0100)
	{
		SprDisplayAbsolute(nSprNo, nPosX, nPosY, nPrio);
		return;
	}
	// Stockage.
	SPR_ADD_TO_LIST(nPosX, nPosY, nPrio, (void *)SprZoom_PreRender)
	SPR_ADD_TO_LIST_RZ(nZoomX, nZoomY)
	SPR_ADD_TO_LIST_END
}
// Sprites en rotation.
void SprDisplayRotoZoomAbsolute(u32 nSprNo, s32 nPosX, s32 nPosY, u16 nPrio, u8 nAngle, u16 nZoom)
{
	// Pas de rot ? => On envoie un sprite zoomé simple.
	if (nAngle == 0)
	{
		SprDisplayZoomAbsolute(nSprNo, nPosX, nPosY, nPrio, nZoom, nZoom);
		return;
	}
	// Angle == 128 => On envoie un sprite zoomé simple avec flips X et Y.
	if (nAngle == 128)
	{
		SprDisplayZoomAbsolute(nSprNo ^ SPR_Flip_X ^ SPR_Flip_Y, nPosX, nPosY, nPrio, nZoom, nZoom);
		return;
	}
	// Stockage.
	SPR_ADD_TO_LIST(nPosX, nPosY, nPrio, (void *)SprRotoZoom_PreRender)
	SPR_ADD_TO_LIST_RZ(nZoom, nAngle)	// > union sur nAngle et nZoomY.
	SPR_ADD_TO_LIST_END
}



// La comparaison du qsort.
int qscmp(const void *pEl1, const void *pEl2)
{
	return ((*(struct SSprStockage **)pEl1)->nPrio - (*(struct SSprStockage **)pEl2)->nPrio);
}

extern	u8	gnFrameMissed;

// Trie la liste des sprites et les affiche.
// A appeler une fois par frame.
void SprDisplayAll(void)
{
	u32	i;

//	if (gnSprSto == 0)	// Rien à faire ?
	if (gnSprSto == 0 || gnFrameMissed)	// Rien à faire ?
	{
		gnSprSto = 0;			// RAZ pour le prochain tour (frame miss).
		#if CACHE_ON == 1
		CacheClearOldSpr();		// Nettoyage des sprites trop vieux du cache.
		#endif
		return;
	}

	// Tri sur la priorité.
	qsort(gpSprSort, gnSprSto, sizeof(struct SSprStockage *), qscmp);

	// Affichage.
	SDL_LockSurface(gVar.pScreen);
	// Sprites normaux.
	for (i = 0; i < gnSprSto; i++)
		SprDisplayLock(gpSprSort[i]);
	SDL_UnlockSurface(gVar.pScreen);

	// RAZ pour le prochain tour.
	gnSprSto = 0;

	#if CACHE_ON == 1
	CacheClearOldSpr();		// Nettoyage des sprites trop vieux du cache.
	#endif

}


u32	gnSprPass2Idx, gnSprPass2Last;

// Same as SprDisplayAll, but split into 2 calls for the masking plan.
// First pass.
void SprDisplayAll_Pass1(void)
{
	u32	i;

	// if (gnSprSto == 0) // Do nothing?
	if (gnSprSto == 0 || gnFrameMissed)	// Nothing to do?
	{
		gnSprSto = 0;			// reset for next round (frame miss).
		gnSprPass2Last = 0;		// For Pass2.
		return;
	}

	// Sort by priority.
	qsort(gpSprSort, gnSprSto, sizeof(struct SSprStockage *), qscmp);

	// Display.
	SDL_LockSurface(gVar.pScreen);
	for (i = 0; i < gnSprSto && gpSprSort[i]->nPrio < 0x100; i++)
		SprDisplayLock(gpSprSort[i]);
	SDL_UnlockSurface(gVar.pScreen);

	// Any left? => Prio > 0x100. The current values are noted.
	if (i < gnSprSto)
	{
		gnSprPass2Idx = i;
		gnSprPass2Last = gnSprSto;
	}
	else
		gnSprPass2Last = 0;

	// reset for next round.
	gnSprSto = 0;

}

// Second pass for sprites displayed above the masking plane.
// To be called once per frame, AFTER SprDisplayAll_Pass1 !
void SprDisplayAll_Pass2(void)
{
	u32	i;

	if (gnFrameMissed == 0)
	if (gnSprPass2Last)		// Anything to do?
	{
		// Display.
		SDL_LockSurface(gVar.pScreen);
		for (i = gnSprPass2Idx; i < gnSprPass2Last; i++)
			SprDisplayLock(gpSprSort[i]);
		SDL_UnlockSurface(gVar.pScreen);
	}

	#if CACHE_ON == 1
	CacheClearOldSpr();		// Clean up old sprites from the cache.
	#endif

}





// Tests a collision between 2 sprites.
// Out: 1 col, 0 no col.
u32 SprCheckColBox(u32 nSpr1, s32 nPosX1, s32 nPosY1, u32 nSpr2, s32 nPosX2, s32 nPosY2)
{
	s32	nXMin1, nXMax1, nYMin1, nYMax1;
	s32	nXMin2, nXMax2, nYMin2, nYMax2;
	struct SSprRect sRect1, sRect2;

	if (SprGetRect(nSpr1, e_SprRectZone_RectCol, &sRect1) == 0) return (0);
	if (SprGetRect(nSpr2, e_SprRectZone_RectCol, &sRect2) == 0) return (0);

	nXMin1 = nPosX1 + sRect1.nX1;
	nXMax1 = nPosX1 + sRect1.nX2;
	nYMin1 = nPosY1 + sRect1.nY1;
	nYMax1 = nPosY1 + sRect1.nY2;

	nXMin2 = nPosX2 + sRect2.nX1;
	nXMax2 = nPosX2 + sRect2.nX2;
	nYMin2 = nPosY2 + sRect2.nY1;
	nYMax2 = nPosY2 + sRect2.nY2;

	// Collisions between rectangles?
	if (nXMax1 >= nXMin2 && nXMin1 <= nXMax2 && nYMax1 >= nYMin2 && nYMin1 <= nYMax2)
	{
		return (1);
	}

	return (0);
}


// Retrieves a rectangle (or point) for a sprite. Flip bits taken into account.
// Out: 0 = No good. 1 = Ok, and rectangle copied to pRectDst.
u32 SprGetRect(u32 nSprNo, u32 nZone, struct SSprRect *pRectDst)
{
	struct SSprite *pSprDesc;
	s32	nTmp;

	if ((nSprNo & ~(SPR_Flip_X | SPR_Flip_Y)) == SPR_NoSprite) return (0);
	pSprDesc = SprGetDesc(nSprNo);			// < GetDesc will make the masks.
	if (pSprDesc->pRect[nZone].nType == e_SprRect_NDef) return (0);

	memcpy(pRectDst, &pSprDesc->pRect[nZone], sizeof(struct SSprRect));
	switch(pRectDst->nType)
	{
	case e_SprRect_Point:
		if (nSprNo & SPR_Flip_X) pRectDst->nX1 = -pRectDst->nX1;
		if (nSprNo & SPR_Flip_Y) pRectDst->nY1 = -pRectDst->nY1;
		break;

	case e_SprRect_Rect:
		if (nSprNo & SPR_Flip_X) { nTmp = pRectDst->nX1; pRectDst->nX1 = -pRectDst->nX2; pRectDst->nX2 = -nTmp; }
		if (nSprNo & SPR_Flip_Y) { nTmp = pRectDst->nY1; pRectDst->nY1 = -pRectDst->nY2; pRectDst->nY2 = -nTmp; }
		break;
	}

	return (1);
}
