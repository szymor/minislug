
#include "includes.h"

//#define	DEBUG_INFO	1	// Commenter pour supprimer.

//=============================================================================

// Lecture d'un u16 au format Big Endian.
u32	ReadBigEndianU16(u8 *pPtr)
{
	u32	nVar;
	nVar = (pPtr[0] << 8) | pPtr[1];
	return (nVar);
}

// Lecture d'un u32 au format Big Endian.
u32	ReadBigEndianU32(u8 *pPtr)
{
	u32	nVar;
	nVar = (pPtr[0] << 24) | (pPtr[1] << 16) | (pPtr[2] << 8) | pPtr[3];
	return (nVar);
}

//=============================================================================

// Déprotège un fichier PSD si nécessaire.
void PSD_Unprotect(u8 *pBuf)
{
	char	pNorm[4] = { '8'^0x80,'B'^0x81,'P'^0x82,'S'^0x83 };		// Avec un xor pour masquer la chaîne dans l'exe.
	char	pProt[4] = { '8'^0x80,'b'^0x81,'p'^0x82,'s'^0x83 };
	u32	i;
	u8	nVar8;

	// Enlève le masque.
	for (i = 0; i < 4; i++)
	{
		pNorm[i] ^= 0x80 + i;
		pProt[i] ^= 0x80 + i;
	}
	// Comparaison.
	if (strncmp((char *)pBuf, pProt, 4) != 0) return;

	// ** Le fichier est protégé, on retire la protection.

	// 4 premiers octets.
	strncpy((char *)pBuf, pNorm, 4);
	// Remise en ordre des valeurs H et W.
	nVar8 = *(((u8 *)pBuf) + 21) ^ 0x82;
	*(((u8 *)pBuf) + 21) = *(((u8 *)pBuf) + 16) ^ 0x14;
	*(((u8 *)pBuf) + 16) = nVar8;
	//
	nVar8 = ~*(((u8 *)pBuf) + 20);
	*(((u8 *)pBuf) + 20) = *(((u8 *)pBuf) + 17) ^ 0x28;
	*(((u8 *)pBuf) + 17) = nVar8;

}

//=============================================================================

// PSD loader.
struct SPSDPicture * PSDLoad(char *pPSDFilename)
{
	FILE	*fPt = NULL;
	u8	*pBuf = NULL;
	struct SPSDPicture	*pPic = NULL;

	u32	nSz, nSz2;
	u8	*pPtr;
	u8	*pSection;
	u32	nTmp;
	u32	nDepth, nMode;
	u32	nComp;
	u32	i;


	//> Chargement du fichier.
	fPt = fopen(pPSDFilename, "rb");
	if (fPt == NULL)
	{
		fprintf(stderr, "PSDLoad(): Error opening file '%s'.\n", pPSDFilename);
		goto _PSDErr;
	}
	// Taille du fichier.
	fseek(fPt, 0, SEEK_END);
	nSz = ftell(fPt);
	rewind(fPt);
	// malloc.
	if ((pBuf = (u8 *)malloc(nSz)) == NULL)
	{
		fprintf(stderr, "PSDLoad(): Error allocating memory.\n");
		goto _PSDErr;
	}
	// Lecture.
	nSz2 = fread(pBuf, 1, nSz, fPt);
	fclose(fPt); fPt = NULL;
	if (nSz2 != nSz)
	{
		fprintf(stderr, "PSDLoad(): Read error. %d bytes expected / %d bytes loaded.\n", (int)nSz, (int)nSz2);
		goto _PSDErr;
	}
	//< Fichier chargé.

	// Alloue un "objet" SPSDPicture.
	if ((pPic = (struct SPSDPicture *)malloc(sizeof(struct SPSDPicture))) == NULL)
	{
		fprintf(stderr, "PSDLoad(): Error allocating memory (SPSDPicture).\n");
		goto _PSDErr;
	}
	pPic->pPlanes = NULL;

	PSD_Unprotect(pBuf);	// Si fichier PSD protégé, retire la protection.

	// Tests sur le header.
	pPtr = pBuf;
	// 8BPS (4 bytes)
	if (strncmp((char *)pPtr, "8BPS", 4) != 0)
		{ fprintf(stderr, "PSDLoad(): Wrong header.\n"); goto _PSDErr; }
//	printf("8BPS\n");
	pPtr += 4;
	// 1 (2 bytes)
	nTmp = ReadBigEndianU16(pPtr);
	pPtr += 2;
	if (nTmp != 1)
		{ fprintf(stderr, "PSDLoad(): Wrong header. (Version = %d).\n", (int)nTmp); goto _PSDErr; }
//	printf("1\n");
	// 6 bytes de 0 (reserved)
	pPtr += 6;
	// Channels (2 bytes)
	pPic->nNbPlanes = ReadBigEndianU16(pPtr);
	pPtr += 2;
#ifdef DEBUG_INFO
	printf("Nb planes: %d\n", (int)pPic->nNbPlanes);
#endif
	// Height (4 bytes)
	pPic->nHeight = ReadBigEndianU32(pPtr);
	pPtr += 4;
#ifdef DEBUG_INFO
	printf("Height: %d\n", (int)pPic->nHeight);
#endif
	// Width (4 Bytes)
	pPic->nWidth = ReadBigEndianU32(pPtr);
	pPtr += 4;
#ifdef DEBUG_INFO
	printf("Width: %d\n", (int)pPic->nWidth);
#endif
	// Depth (2 bytes)
	nDepth = ReadBigEndianU16(pPtr);
	pPtr += 2;
//	printf("Depth: %d\n", (int)nDepth);
	// Mode (2 bytes)
	// 2 = Indexed colors.
	nMode = ReadBigEndianU16(pPtr);
	pPtr += 2;
//	printf("Mode: %d\n", (int)nMode);
	// Depth et Mode ok ?
	if (nDepth != 8 || nMode != 2)
		{ fprintf(stderr, "PSDLoad(): Unsupported format. Depth = %d, Mode = %d\n", (int)nDepth, (int)nMode); goto _PSDErr; }

	// Color mode data section. Pour le mode "2", toujours 768 bytes, non entremélés => 256 R, 256 G, 256 B.
//	printf("\nColor mode data section\n");
	nTmp = ReadBigEndianU32(pPtr);
	pSection = pPtr + 4;
	pPtr += nTmp + 4;
//	printf("Size: %d\n", (int)nTmp);
	//> Palette.
	if (nTmp != 768)
		{ fprintf(stderr, "PSDLoad(): Palette size != 768 (%d)\n", (int)nTmp); goto _PSDErr; }
	for (i = 0; i < 256; i++)
	{
		pPic->pColors[i].r = *(pSection + i);
		pPic->pColors[i].g = *(pSection + 256 + i);
		pPic->pColors[i].b = *(pSection + 512 + i);
//		printf("Color %d : R = %d - G = %d - B = %d\n", (int)i, (int)*(pSection + i), (int)*(pSection + 256 + i), (int)*(pSection + 512 + i));
	}
	//<


	// Image ressources section. (On skippe).
//	printf("\nImage ressources section\n");
	nTmp = ReadBigEndianU32(pPtr);
	pPtr += nTmp + 4;
//	printf("Size: %d\n", (int)nTmp);

	// Layer and mask information section. (On skippe).
//	printf("\nLayer and mask information section\n");
//printf("Offs = %x\n", (int)(pPtr - pBuf));
	nTmp = ReadBigEndianU32(pPtr);
	pPtr += nTmp + 4;
//	printf("Size: %d\n", (int)nTmp);

	// Image data section.
//	printf("\nImage data section\n");
//printf("Offs = %x\n", (int)(pPtr - pBuf));
	nComp = ReadBigEndianU16(pPtr);
	pPtr += 2;
//	printf("Compression method: %d\n", (int)nComp);


/*
	// Comp ok ?
	if (nComp != 0)
		{ fprintf(stderr, "PSDLoad(): Unsupported Compression method. Comp = %d\n", (int)nComp); goto _PSDErr; }
	// Allocation d'un buffer pour l'image (0 = RAW non compressé).
	u8	*pPlanesBuf;
	if ((pPlanesBuf = (u8 *)malloc(sPic.nNbPlanes * sPic.nWidth * sPic.nHeight)) == NULL)
	{
		fprintf(stderr, "PSDLoad(): Error allocating buffer.\n");
		free(pBuf);
		return (NULL);
	}
	memcpy(pPlanesBuf, pPtr, sPic.nWidth * sPic.nHeight);
*/



	// Comp ok ? (Pour l'instant, j'ai toujours eu du "1").
	if (nComp != 1)
		{ fprintf(stderr, "PSDLoad(): Unsupported Compression method. (Comp = %d).\n", (int)nComp); goto _PSDErr; }

//	// Allocation du gros buffer de décompression (w*h*nb planes*3(R+G+B)).
//	if ((pPic->pPlanes = (u8 *)malloc(pPic->nNbPlanes * pPic->nWidth * pPic->nHeight * 3)) == NULL)
	// Allocation du gros buffer de décompression (w*h*nb planes).
	if ((pPic->pPlanes = (u8 *)malloc(pPic->nNbPlanes * pPic->nWidth * pPic->nHeight)) == NULL)
		{ fprintf(stderr, "PSDLoad(): Error allocating decompression buffer.\n"); goto _PSDErr; }

	// PackBits :
	//Header byte    Data following the header byte
	//0 to 127       (1 + n) literal bytes of data
	//-1 to -127     One byte of data, repeated (1 – n) times in the decompressed output
	//-128           No operation (skip and treat next byte as a header byte)

	// Depack.
	u8	*pDst = pPic->pPlanes;

	u8	*pBytesPerLines = pPtr;		// Un ptr sur une table de ht * nb de plans de u16, contenant le nombre d'octets à dépacker sur chaque ligne.
	u8	*pSrc = pPtr + (pPic->nHeight * pPic->nNbPlanes * 2);	// Ensuite, les datas des lignes.

	// Plan 0 : w*h*R / w*h*G / w*h*B
	// Plan 1 : w*h*R / w*h*G / w*h*B
	// etc...
	// En mode indexé, 1 seul w*h (et pas R puis G puis B).

	u32	nCurLn;
	for (nCurLn = 0; nCurLn < pPic->nHeight * pPic->nNbPlanes; nCurLn++)
	{
		u32	nBytesOnLn;

		nBytesOnLn = ReadBigEndianU16(pBytesPerLines);
		pBytesPerLines += 2;
//		printf("\nLn %d - %d bytes\n", (int)nCurLn, (int)nBytesOnLn);

		u32	nRead = 0;
		while (nRead < nBytesOnLn)
		{
			u8	nByte0, nByte1;

			nByte0 = *pSrc++;
			nRead++;
			if ((s8)nByte0 >= 0)
			{
				for (i = 0; i < (u32)(1 + nByte0); i++)
				{
//					printf("%d ", (int)*pSrc);
					*pDst++ = *pSrc++;
					nRead++;
				}
			}
			else
			if ((s8)nByte0 != -128)
			{
				nByte1 = *pSrc++;
				nRead++;
				for (i = 0; i < (u32)(1 - (s8)nByte0); i++)
				{
//					printf("%d ", (int)nByte1);
					*pDst++ = nByte1;
				}
			}
		}
		if (nRead > nBytesOnLn) { fprintf(stderr, "PSDLoad(): Depack error!\n"); goto _PSDErr; }

	}

	// Ok !
	free(pBuf); pBuf = NULL;
	return (pPic);

	// Sortie en cas d'erreur.
_PSDErr:
	if (fPt != NULL) fclose(fPt);
	if (pBuf != NULL) free(pBuf);
	if (pPic != NULL)
	{
		if (pPic->pPlanes != NULL) free(pPic->pPlanes);
		free(pPic);
	}
	return (NULL);
}

//=============================================================================

enum
{
	PROP_END                =  0,
	PROP_COLORMAP           =  1,
	PROP_ACTIVE_LAYER       =  2,
	PROP_ACTIVE_CHANNEL     =  3,
	PROP_SELECTION          =  4,
	PROP_FLOATING_SELECTION =  5,
	PROP_OPACITY            =  6,
	PROP_MODE               =  7,
	PROP_VISIBLE            =  8,
	PROP_LINKED             =  9,
	PROP_LOCK_ALPHA         = 10,
	PROP_APPLY_MASK         = 11,
	PROP_EDIT_MASK          = 12,
	PROP_SHOW_MASK          = 13,
	PROP_SHOW_MASKED        = 14,
	PROP_OFFSETS            = 15,
	PROP_COLOR              = 16,
	PROP_COMPRESSION        = 17,
	PROP_GUIDES             = 18,
	PROP_RESOLUTION         = 19,
	PROP_TATTOO             = 20,
	PROP_PARASITES          = 21,
	PROP_UNIT               = 22,
	PROP_PATHS              = 23,
	PROP_USER_UNIT          = 24,
	PROP_VECTORS            = 25,
	PROP_TEXT_LAYER_FLAGS   = 26,
	PROP_SAMPLE_POINTS      = 27,
	PROP_LOCK_CONTENT       = 28,
	PROP_GROUP_ITEM         = 29,
	PROP_ITEM_PATH          = 30,
	PROP_GROUP_ITEM_FLAGS   = 31,

	PROP_Last
};

struct SXCFProperties
{
	u32	nComp;

	s32	nLayerOffsX, nLayerOffsY;	// Décalage de la couche.

};

#ifdef DEBUG_INFO
char *pXCFPropNames[PROP_Last] =
{
	"PROP_END",                //=  0,
	"PROP_COLORMAP",           //=  1,
	"PROP_ACTIVE_LAYER",       //=  2,
	"PROP_ACTIVE_CHANNEL",     //=  3,
	"PROP_SELECTION",          //=  4,
	"PROP_FLOATING_SELECTION", //=  5,
	"PROP_OPACITY",            //=  6,
	"PROP_MODE",               //=  7,
	"PROP_VISIBLE",            //=  8,
	"PROP_LINKED",             //=  9,
	"PROP_LOCK_ALPHA",         //= 10,
	"PROP_APPLY_MASK",         //= 11,
	"PROP_EDIT_MASK",          //= 12,
	"PROP_SHOW_MASK",          //= 13,
	"PROP_SHOW_MASKED",        //= 14,
	"PROP_OFFSETS",            //= 15,
	"PROP_COLOR",              //= 16,
	"PROP_COMPRESSION",        //= 17,
	"PROP_GUIDES",             //= 18,
	"PROP_RESOLUTION",         //= 19,
	"PROP_TATTOO",             //= 20,
	"PROP_PARASITES",          //= 21,
	"PROP_UNIT",               //= 22,
	"PROP_PATHS",              //= 23,
	"PROP_USER_UNIT",          //= 24,
	"PROP_VECTORS",            //= 25,
	"PROP_TEXT_LAYER_FLAGS",   //= 26,
	"PROP_SAMPLE_POINTS",      //= 27,
	"PROP_LOCK_CONTENT",       //= 28,
	"PROP_GROUP_ITEM",         //= 29,
	"PROP_ITEM_PATH",          //= 30,
	"PROP_GROUP_ITEM_FLAGS",   //= 31
};
#endif

// Gestion des "Image properties".
u32 XCF_ImageProperties(u8 **ppPtr, struct SPSDPicture *pPic, struct SXCFProperties *pProps)
{
	u32	i, k;
	u8	*pPtr;
	u32	nType, nPayload;

	pPtr = *ppPtr;

	while (1)
	{
		nType = ReadBigEndianU32(pPtr);
		nPayload = ReadBigEndianU32(pPtr + 4);
		pPtr += 8;

#ifdef DEBUG_INFO
if (nType < PROP_Last)
	printf("%s\n", pXCFPropNames[nType]);
#endif

		switch (nType)
		{
		case PROP_END:				// 0
			*ppPtr = pPtr;
			return (0);

		case PROP_COLORMAP:			// 1
			k = ReadBigEndianU32(pPtr);		// n: Number of colors.
#ifdef DEBUG_INFO
printf("payload = %d / nb colors = %d\n", nPayload, k);
#endif
			for (i = 0; i < k; i++)
			{
				pPic->pColors[i].r = *(pPtr + 4 + (i * 3));
				pPic->pColors[i].g = *(pPtr + 4 + (i * 3) + 1);
				pPic->pColors[i].b = *(pPtr + 4 + (i * 3) + 2);
#ifdef DEBUG_INFO
printf("color %d: %3d %3d %3d\n", i, pPic->pColors[i].r, pPic->pColors[i].g, pPic->pColors[i].b);
#endif
			}

			nPayload = (k * 3) + 4;		// (A cause du payload parfois invalide (doc)).
			break;

		case PROP_OFFSETS:			// 15 (essential)
			pProps->nLayerOffsX = ReadBigEndianU32(pPtr);
			pProps->nLayerOffsY = ReadBigEndianU32(pPtr+4);
#ifdef DEBUG_INFO
printf("offsx = %d / offsy = %d\n", pProps->nLayerOffsX, pProps->nLayerOffsY);
#endif
			break;

		case PROP_COMPRESSION:		// 17
			pProps->nComp = *pPtr;
#ifdef DEBUG_INFO
printf("Compression type = %d\n", pProps->nComp);
#endif
			break;

		// On skippe le reste.

		case PROP_ACTIVE_LAYER:		// 2 (editing state) (skip)
		case PROP_ACTIVE_CHANNEL:	// 3 (editing state) (skip)
		case PROP_SELECTION:		// 4 (editing state?) (skip)
		case PROP_FLOATING_SELECTION:	// 5 (essential) (skip)
		case PROP_OPACITY:			// 6 (essential) (skip)
		case PROP_MODE:				// 7 (essential) (skip)
		case PROP_VISIBLE:			// 8 (essential) (skip)
		case PROP_LINKED:			// 9 (editing state) (skip)
		case PROP_LOCK_ALPHA:		// 10 (editing state) (skip)
		case PROP_APPLY_MASK:		// 11 (essential) (skip)
		case PROP_EDIT_MASK:		// 12 (editing state) (skip)
		case PROP_SHOW_MASK:		// 13 (editing state) (skip)
		case PROP_SHOW_MASKED:		// 14 (editing state)
		case PROP_COLOR:			// 16
		case PROP_GUIDES:			// 18 (skip)
		case PROP_RESOLUTION:		// 19 (skip)
		case PROP_TATTOO:			// 20 (internal Gimp state) (skip)
		case PROP_PARASITES:		// 21 (skip)
		case PROP_UNIT:				// 22 (skip)
		case PROP_PATHS:			// 23 (skip)
		case PROP_USER_UNIT:		// 24 (skip)
		case PROP_VECTORS:			// 25 (skip)
		case PROP_TEXT_LAYER_FLAGS:	// 26 (skip)
		case PROP_SAMPLE_POINTS:	// 27 (undocumented) (skip)
		case PROP_LOCK_CONTENT:		// 28 (undocumented) (skip)
		case PROP_GROUP_ITEM:		// 29 (undocumented) (skip)
		case PROP_ITEM_PATH:		// 30 (undocumented) (skip)
		case PROP_GROUP_ITEM_FLAGS:	// 31 (undocumented) (skip)
			break;

		default:
			fprintf(stderr, "Unhandled image property: %d.\n", nType);
			break;
		}

		pPtr += nPayload;	// On passe à la propriété suivante.

	}

}


// Récupère un layer (layer ou channel).
// Out: NULL => fatal.
u8 * XCF_GetLayer(u8 *pBuf, u8 *pHptr, u32 nLayerWidth, u32 nLayerHeight)
{
	u32	i;
	u32	nTmp;

	u8	*pPicDst = NULL;

	// On est dans la hiérarchie (hptr).
	u32	nHierarchyWidth;
	u32	nHierarchyHeight;
	u32	nHierarchyLPtr;

	nHierarchyWidth = ReadBigEndianU32(pHptr); pHptr += 4;
	nHierarchyHeight = ReadBigEndianU32(pHptr); pHptr += 4;
#ifdef DEBUG_INFO
	u32	nHierarchyBPP;
	nHierarchyBPP = ReadBigEndianU32(pHptr);
#endif
	pHptr += 4;
	nHierarchyLPtr = ReadBigEndianU32(pHptr); pHptr += 4;
#ifdef DEBUG_INFO
printf("hierarchy: w = %d / h = %d / BPP = %d / lptr = $%x\n", nHierarchyWidth, nHierarchyHeight, nHierarchyBPP, nHierarchyLPtr);
#endif
	// Test.
	if (nHierarchyWidth != nLayerWidth || nHierarchyHeight != nLayerHeight)
		{ fprintf(stderr, "XCFLoad(): Layers: Hierarchy WL mismatch (hW=%d / LW=%d / hH=%d / LH=%d).\n", nHierarchyWidth, nLayerWidth, nHierarchyHeight, nLayerHeight); goto _LayerErr; }


	// Maintenant, on va au level.
	u8	*pLevel;
	pLevel = pBuf + nHierarchyLPtr;

	u32	nLevelWidth, nLevelHeight;
	nLevelWidth = ReadBigEndianU32(pLevel); pLevel += 4;
	nLevelHeight = ReadBigEndianU32(pLevel); pLevel += 4;
#ifdef DEBUG_INFO
printf("level: w = %d / h = %d\n", nLevelWidth, nLevelHeight);
#endif
	// Test.
	if (nLevelWidth != nLayerWidth || nLevelHeight != nLayerHeight)
		{ fprintf(stderr, "XCFLoad(): Layers: Level WL mismatch (lW=%d / LW=%d / lH=%d / LH=%d).\n", nLevelWidth, nLayerWidth, nLevelHeight, nLayerHeight); goto _LayerErr; }


	// Et enfin, les tiles !
	static	u8	pDepack[64 * 64];	// Buffer pour depack d'une tile (taille fixe).
	u8	*pData;

	// Allocation d'un buffer temporaire pour recréation du layer.
	pPicDst = (u8 *)malloc(nLayerWidth * nLayerHeight);
	if (pPicDst == NULL)
		{ fprintf(stderr, "XCFLoad(): Layers: malloc failed.\n"); goto _LayerErr; }

	s32	nTileHtRem, nTileLgRem;
	u32	nTilePosX, nTilePosY;

	nTileHtRem = nLevelHeight;
	nTilePosY = 0;
	while (nTileHtRem > 0)
	{
		nTileLgRem = nLevelWidth;
		nTilePosX = 0;
		while (nTileLgRem > 0)
		{
			// Pointeur sur la tile.
			nTmp = ReadBigEndianU32(pLevel); pLevel += 4;
//printf("tile ptr=$%x\n", nTmp);
			pData = pBuf + nTmp;

//printf("%dx%d : ", (nTileLgRem > 64 ? 64 : nTileLgRem), (nTileHtRem > 64 ? 64 : nTileHtRem) );

			// Depack (RLE).
			u32	nTileW, nTileH;
			s32	nTotBytes;
			u32	idx;
			u32	n, p, q, v;

			nTileW = (nTileLgRem > 64 ? 64 : nTileLgRem);
			nTileH = (nTileHtRem > 64 ? 64 : nTileHtRem);
			nTotBytes = nTileW * nTileH;	// Nb d'octets au bout duquel il faut arrêter le depack.
			idx = 0;
			while (idx < nTotBytes)
			{
				n = *pData++;
				if (n <= 126)
				{
					v = *pData++;
					for (i = 0; i < n+1; i++)
						pDepack[idx++] = v;
				}
				else
				if (n == 127)
				{
					p = *pData++;
					q = *pData++;
					v = *pData++;
					for (i = 0; i < (p * 256) + q; i++)
						pDepack[idx++] = v;
				}
				else
				if (n == 128)
				{
					p = *pData++;
					q = *pData++;
					for (i = 0; i < (p * 256) + q; i++)
						pDepack[idx++] = *pData++;
				}
				else	// 129+
				{
					for (i = 0; i < 256 - n; i++)
						pDepack[idx++] = *pData++;
				}

			}
//printf("idx = %d / nTotBytes = %d\n", idx, nTotBytes);
			if (idx != nTotBytes)
				{ fprintf(stderr, "XCFLoad(): Depack error.\n"); goto _LayerErr; }

			// Recopie la tile à sa place.
			for (q = 0; q < nTileH; q++)
			for (p = 0; p < nTileW; p++)
				*(pPicDst + ((nTilePosY + q) * nLayerWidth) + nTilePosX + p) = pDepack[(q * nTileW) + p];
//old					*(pPicDst + ((nTilePosY + q) * pPic->nWidth) + nTilePosX + p) = pDepack[(q * nTileW) + p];


			nTileLgRem -= 64;
			nTilePosX += 64;
		}
//printf("\n");
		nTileHtRem -= 64;
		nTilePosY += 64;
	}
	nTmp = ReadBigEndianU32(pLevel); pLevel += 4;
#ifdef DEBUG_INFO
printf("Last adr (should be 0)=$%x\n", nTmp);
#endif
	return (pPicDst);

_LayerErr:
	if (pPicDst != NULL) free(pPicDst);
	return (NULL);
}

// Recopie un plan dans le buffer image final.
void XCF_CopyLayer(struct SPSDPicture *pPic, u8 *pPicLayer, u32 nLayerWidth, u32 nLayerHeight, s32 nLayerOffsX, s32 nLayerOffsY)
{

	// realloc pour allouer un plan suppplémentaire sur l'image finale.
	pPic->pPlanes = (u8 *)realloc(pPic->pPlanes, pPic->nWidth * pPic->nHeight * (pPic->nNbPlanes + 1));
	if (pPic->pPlanes == NULL)
		{ fprintf(stderr, "XCFLoad(): Layers: realloc failed.\n"); return; }
	// RAZ du nouveau plan en cas de décalages.
	memset(pPic->pPlanes + (pPic->nWidth * pPic->nHeight * pPic->nNbPlanes), 0, pPic->nWidth * pPic->nHeight);

	// Recopie du layer sur l'image.
	s32	u, v;
	for (v = 0; v < nLayerHeight; v++)
	{
		if ((u32)(v + nLayerOffsY) < pPic->nHeight)
		for (u = 0; u < nLayerWidth; u++)
		{
			if ((u32)(u + nLayerOffsX) < pPic->nWidth)
			*(pPic->pPlanes + (pPic->nWidth * pPic->nHeight * pPic->nNbPlanes) + (pPic->nWidth * (v + nLayerOffsY)) + (u + nLayerOffsX)) =
				*(pPicLayer + (nLayerWidth * v) + u);
		}
	}

	pPic->nNbPlanes++;		// +1 plan.

}

// XCF loader.
struct SPSDPicture * XCFLoad(char *pXCFFilename)
{
	FILE	*fPt = NULL;
	u8	*pBuf = NULL;
	struct SPSDPicture	*pPic = NULL;

	u32	nSz, nSz2;
	u8	*pPtr;
	u32	nTmp;
	u32	nMode;

	struct SXCFProperties	sProps;


	//> Chargement du fichier.
	fPt = fopen(pXCFFilename, "rb");
	if (fPt == NULL)
	{
		fprintf(stderr, "XCFLoad(): Error opening file '%s'.\n", pXCFFilename);
		goto _XCFErr;
	}
	// Taille du fichier.
	fseek(fPt, 0, SEEK_END);
	nSz = ftell(fPt);
	rewind(fPt);
	// malloc.
	if ((pBuf = (u8 *)malloc(nSz)) == NULL)
	{
		fprintf(stderr, "XCFLoad(): Error allocating memory.\n");
		goto _XCFErr;
	}
	// Lecture.
	nSz2 = fread(pBuf, 1, nSz, fPt);
	fclose(fPt); fPt = NULL;
	if (nSz2 != nSz)
	{
		fprintf(stderr, "XCFLoad(): Read error. %d bytes expected / %d bytes loaded.\n", (int)nSz, (int)nSz2);
		goto _XCFErr;
	}
	//< Fichier chargé.

	// Alloue un "objet" SPSDPicture.
	if ((pPic = (struct SPSDPicture *)malloc(sizeof(struct SPSDPicture))) == NULL)
	{
		fprintf(stderr, "XCFLoad(): Error allocating memory (SPSDPicture).\n");
		goto _XCFErr;
	}
	memset(pPic, 0, sizeof(struct SPSDPicture));
	pPic->pPlanes = NULL;

	// Tests sur le header.
	pPtr = pBuf;
	// "gimp xcf " (9 bytes)
	if (strncmp((char *)pPtr, "gimp xcf ", 9) != 0)
		{ fprintf(stderr, "XCFLoad(): Wrong header.\n"); goto _XCFErr; }
	// "v001" (4 bytes)
	if (strncmp((char *)pPtr + 9, "v001 ", 4) != 0)
		{ fprintf(stderr, "XCFLoad(): Unhandled file version (v001 only).\n"); goto _XCFErr; }
	// 0 (1 byte)
	if (*(pPtr + 13) != 0)
		{ fprintf(stderr, "XCFLoad(): Header error.\n"); goto _XCFErr; }
	pPtr += 14;

	// Width (4 Bytes)
	pPic->nWidth = ReadBigEndianU32(pPtr);
	pPtr += 4;
	// Height (4 bytes)
	pPic->nHeight = ReadBigEndianU32(pPtr);
	pPtr += 4;
#ifdef DEBUG_INFO
printf("Width: %d\n", (int)pPic->nWidth);
printf("Height: %d\n", (int)pPic->nHeight);
#endif
	// Base type (4 bytes)
	// 2 = Indexed colors.
	nMode = ReadBigEndianU32(pPtr);
	pPtr += 4;
	if (nMode != 2)
		{ fprintf(stderr, "XCFLoad(): Unsupported mode (%d).\n", nMode); goto _XCFErr; }

	// Image properties.
	sProps.nComp = (u32)-1;
	sProps.nLayerOffsX = sProps.nLayerOffsY = 0;
	if (XCF_ImageProperties(&pPtr, pPic, &sProps))
		{ fprintf(stderr, "XCFLoad(): Image properties problem.\n"); goto _XCFErr; }

	// Compression ok ?
	if (sProps.nComp != 1)
		{ fprintf(stderr, "XCFLoad(): Unsupported compression type (%d).\n", sProps.nComp); goto _XCFErr; }

#ifdef DEBUG_INFO
printf("offs = $%x\n", (u32)(pPtr - pBuf) );
#endif

	// Layers. (Ordre inversé, donc on commence par aller jusqu'en fin de liste et on lit la liste à reculons).
	u8	*pPtrOrg;
	u8	*pPtrRev;

	pPtrOrg = pPtr;
	while (*(u32 *)pPtr != 0) pPtr += 4;		// Endianness ok parce que test avec 0.
	pPtr += 4;
	pPtrRev = pPtr - 8;

	// Layers.
//o	while (*(u32 *)pPtr != 0)		// Endianness ok parce que test avec 0.
	while (pPtrRev >= pPtrOrg)
	{
//o		nTmp = ReadBigEndianU32(pPtr); pPtr += 4;
		nTmp = ReadBigEndianU32(pPtrRev); pPtrRev -= 4;
#ifdef DEBUG_INFO
printf("Layer ptr=$%x\n", nTmp);
#endif

		u8	*pLayer;
		u32	nLayerWidth, nLayerHeight;

		pLayer = pBuf + nTmp;
		nLayerWidth = ReadBigEndianU32(pLayer); pLayer += 4;
		nLayerHeight = ReadBigEndianU32(pLayer); pLayer += 4;
		nTmp = ReadBigEndianU32(pLayer); pLayer += 4;
#ifdef DEBUG_INFO
printf("Width = %d / Height = %d / Color mode = %d\n", nLayerWidth, nLayerHeight, nTmp);
#endif
		if (nTmp != 4)
			{ fprintf(stderr, "XCFLoad(): Layers: Unsupported mode (%d).\n", nTmp); goto _XCFErr; }

		// Skip layer name.
		nTmp = ReadBigEndianU32(pLayer); pLayer += 4;
		pLayer += nTmp;
#ifdef DEBUG_INFO
printf("offs = $%x\n", (u32)(pLayer - pBuf) );
#endif

		// Image properties.
		sProps.nLayerOffsX = sProps.nLayerOffsY = 0;
		if (XCF_ImageProperties(&pLayer, pPic, &sProps))
			{ fprintf(stderr, "XCFLoad(): Layers: Image properties problem.\n"); goto _XCFErr; }

		// hptr & mptr
		u32	nHptr;
		nHptr = ReadBigEndianU32(pLayer); pLayer += 4;
#ifdef DEBUG_INFO
		u32	nMptr;
		nMptr = ReadBigEndianU32(pLayer);
printf("hptr = $%x / mptr = $%x\n", nHptr, nMptr);
#endif
		pLayer += 4;

		// Récupération du layer.
		u8 *pPicLayer;
		pPicLayer = XCF_GetLayer(pBuf, pBuf + nHptr, nLayerWidth, nLayerHeight);
		if (pPicLayer == NULL) goto _XCFErr;
		// Recopie dans le buffer final.
		XCF_CopyLayer(pPic, pPicLayer, nLayerWidth, nLayerHeight, sProps.nLayerOffsX, sProps.nLayerOffsY);
		free(pPicLayer);		// On libère le plan temporaire.
		if (pPic->pPlanes == NULL) goto _XCFErr;

	}
//o	pPtr += 4;

	// Channels. (Ordre inversé, donc on commence par aller jusqu'en fin de liste et on lit la liste à reculons).
	pPtrOrg = pPtr;
	while (*(u32 *)pPtr != 0) pPtr += 4;		// Endianness ok parce que test avec 0.
	pPtr += 4;
	pPtrRev = pPtr - 8;

	// Channels.
//o	while (*(u32 *)pPtr != 0)		// Endianness ok parce que test avec 0.
	while (pPtrRev >= pPtrOrg)
	{
//o		nTmp = ReadBigEndianU32(pPtr); pPtr += 4;
		nTmp = ReadBigEndianU32(pPtrRev); pPtrRev -= 4;
#ifdef DEBUG_INFO
printf("Channel ptr=$%x\n", nTmp);
#endif

		u8	*pChannel;
		u32	nChannelWidth, nChannelHeight;

		pChannel = pBuf + nTmp;
		nChannelWidth = ReadBigEndianU32(pChannel); pChannel += 4;
		nChannelHeight = ReadBigEndianU32(pChannel); pChannel += 4;
#ifdef DEBUG_INFO
printf("Width = %d / Height = %d\n", nChannelWidth, nChannelHeight);
#endif

		// Skip channel name.
		nTmp = ReadBigEndianU32(pChannel); pChannel += 4;
		pChannel += nTmp;
#ifdef DEBUG_INFO
printf("offs = $%x\n", (u32)(pChannel - pBuf) );
#endif

		// Image properties.
		sProps.nLayerOffsX = sProps.nLayerOffsY = 0;
		if (XCF_ImageProperties(&pChannel, pPic, &sProps))
			{ fprintf(stderr, "XCFLoad(): Channels: Image properties problem.\n"); goto _XCFErr; }

		// hptr = Ptr sur la hiérarchie.
		u32	nHptr;
		nHptr = ReadBigEndianU32(pChannel); pChannel += 4;

		// Récupération du layer.
		u8 *pPicLayer;
		pPicLayer = XCF_GetLayer(pBuf, pBuf + nHptr, nChannelWidth, nChannelHeight);
		if (pPicLayer == NULL) goto _XCFErr;
		// Recopie dans le buffer final.
		XCF_CopyLayer(pPic, pPicLayer, nChannelWidth, nChannelHeight, sProps.nLayerOffsX, sProps.nLayerOffsY);
		free(pPicLayer);		// On libère le plan temporaire.
		if (pPic->pPlanes == NULL) goto _XCFErr;

	}
//o	pPtr += 4;

#ifdef DEBUG_INFO
printf("Planes loaded: %d\n", pPic->nNbPlanes);
#endif

	// Ok !
	free(pBuf); pBuf = NULL;
	return (pPic);

	// Sortie en cas d'erreur.
_XCFErr:
	if (fPt != NULL) fclose(fPt);
	if (pBuf != NULL) free(pBuf);
	if (pPic != NULL)
	{
		if (pPic->pPlanes != NULL) free(pPic->pPlanes);
		free(pPic);
	}
	return (NULL);
}

//=============================================================================

/*
typedef struct SPSDPicture * (*pLoadingFct)(char *);

// Lecture d'une image PSD/XCF 8 bits et passage dans une surface SDL.
SDL_Surface * GeneLoadToSDLSurf(char *pFilename, pLoadingFct pFct)
{
	struct SPSDPicture	*pPic;
	SDL_Surface *pSDLSurf;
	u32	i;

	// Lecture du PSD.
	if ((pPic = pFct(pFilename)) == NULL)
	{
		fprintf(stderr, "GeneLoadToSDLSurf(): Loading fct returned NULL.\n");
		return (NULL);
	}

	// On passe l'image (le premier plan) dans une surface SDL.
	if ((pSDLSurf = SDL_CreateRGBSurface(SDL_SWSURFACE, pPic->nWidth, pPic->nHeight, 8, 0, 0, 0, 0)) == NULL)
	{
		fprintf(stderr, "GeneLoadToSDLSurf(): SDL_CreateRGBSurface() failed.\n");
	}
	else
	{
		// Recopie de la palette.
		SDL_SetColors(pSDLSurf, pPic->pColors, 0, 256);
		// Recopie du premier plan.
		SDL_LockSurface(pSDLSurf);
		for (i = 0; i < pPic->nHeight; i++)
			memcpy((u8 *)pSDLSurf->pixels + (i * pSDLSurf->pitch), pPic->pPlanes + (i * pPic->nWidth), pPic->nWidth);
//			memcpy((u8 *)pSDLSurf->pixels + (i * pSDLSurf->pitch), pPic->pPlanes + ((i+(pPic->nHeight*1)) * pPic->nWidth), pPic->nWidth);
		SDL_UnlockSurface(pSDLSurf);
	}

	free(pPic->pPlanes);
	free(pPic);
	return (pSDLSurf);

}

// Wrapper PSD.
SDL_Surface * PSDLoadToSDLSurf(char *pPSDFilename)
{
	return (GeneLoadToSDLSurf(pPSDFilename, PSDLoad));
}

// Wrapper XCF.
SDL_Surface * XCFLoadToSDLSurf(char *pXCFFilename)
{
	return (GeneLoadToSDLSurf(pXCFFilename, XCFLoad));
}
*/

