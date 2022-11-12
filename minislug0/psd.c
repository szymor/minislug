
#include "includes.h"



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


//#define	DEBUG_INFO	1	// Commenter pour supprimer.

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

// Lecture d'une image PSD 8 bits et passage dans une surface SDL.
SDL_Surface * PSDLoadToSDLSurf(char *pPSDFilename)
{
	struct SPSDPicture	*pPic;
	SDL_Surface *pSDLSurf;
	u32	i;

	// Lecture du PSD.
	if ((pPic = PSDLoad(pPSDFilename)) == NULL)
	{
		fprintf(stderr, "PSDLoadToSDLSurf(): PSDLoad() returned NULL.\n");
		return (NULL);
	}

	// On passe l'image (le premier plan) dans une surface SDL.
	if ((pSDLSurf = SDL_CreateRGBSurface(SDL_SWSURFACE, pPic->nWidth, pPic->nHeight, 8, 0, 0, 0, 0)) == NULL)
	{
		fprintf(stderr, "PSDLoadToSDLSurf(): SDL_CreateRGBSurface() failed.\n");
	}
	else
	{
		// Recopie de la palette.
		SDL_SetColors(pSDLSurf, pPic->pColors, 0, 256);
		// Recopie du premier plan.
		SDL_LockSurface(pSDLSurf);
		for (i = 0; i < pPic->nHeight; i++)
			memcpy((u8 *)pSDLSurf->pixels + (i * pSDLSurf->pitch), pPic->pPlanes + (i * pPic->nWidth), pPic->nWidth);
		SDL_UnlockSurface(pSDLSurf);
	}

	free(pPic->pPlanes);
	free(pPic);
	return (pSDLSurf);

}




