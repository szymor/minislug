
// Animated GIF depacker.
// Code: Clément "17o2!!" CORDE.

// !!! Interlaced format not handled !!!

// Il faudrait se servir de ça :
// 4 passes
// pImgDesc->nInterlaceFlag
// static u32	pInterlaceEvery[] = { 8, 8, 4, 2 };	// Toutes les x lignes.
// static u32	pInterlaceStart[] = { 0, 4, 2, 1 };	// Départ à la ligne x.

#include "includes.h"

#define	LZW_REVERSE		1	// 1 = On renvoie les chaînes à l'envers, ce qui évite une recopie pour retourner les chaînes.

#define	GIF_TRAILER					0x3B
#define	GIF_EXTENSION_INTRODUCER	0x21
#define	GIF_IMAGE_SEPARATOR			0x2C



// Lecture de bits dans un buffer.
u32	GIF_GetBits(u8 *pBuf, u32 nBitStart, u32 nSz)
{
	s32	nRet = 0;
	u8	nSrcMsk;
	u32 nDstMsk;
	u32	nCurByte;

	nDstMsk = 1;
	nSrcMsk = 1 << (nBitStart & 7);
	nCurByte = nBitStart >> 3;

	while (nSz)
	{
		if (pBuf[nCurByte] & nSrcMsk) nRet |= nDstMsk;
		nDstMsk <<= 1;
		nSrcMsk <<= 1;
		if (nSrcMsk == 0)
		{
			nSrcMsk = 1;
			nCurByte++;
		}
		nSz--;
	}

	return (nRet);
}



struct SDictionnaryRecord	gpStrBuf[4096];
u32	gnDicNext;		// Next available position.

// Rajoute une chaîne au dictionnaire.
void LZW_Add2Dictionnary(u32 nPrevCode, u8 nAppendChar)
{
	if (gnDicNext + 1 == 4096) return;	// Pas d'ajout si la table est pleine, on utilise la table telle quelle en attendant le "clear code".

	gpStrBuf[gnDicNext].nPrevCode = nPrevCode;
	gpStrBuf[gnDicNext].nAppendChar = nAppendChar;
	gnDicNext++;
}

#if LZW_REVERSE == 0
// Renvoie une chaîne du dictionnaire.
void LZW_GetTranslation(u32 nStrCode, u8 *pStrDst, u32 *pnStrDstNb, u32 nClearCode)
{
	u8	pStrRev[4096];
	u32	nStrRevNb;
	u32	i;

	// Récupération de la chaîne.
	nStrRevNb = 0;
	while (1)
	{
		if (nStrCode >= gnDicNext)
		{
			fprintf(stderr, "LZW_GetTranslation(): Error, code not defined in dictionnary.\n");
			exit(1);
		}

		// Code < au clear code => direct.
		if (nStrCode < nClearCode)
		{
			pStrRev[nStrRevNb++] = nStrCode;
			break;
		}
		pStrRev[nStrRevNb++] = gpStrBuf[nStrCode].nAppendChar;
		nStrCode = gpStrBuf[nStrCode].nPrevCode;
	}

	// Retournement de la chaîne.
	for (i = 0; i < nStrRevNb; i++) pStrDst[i] = pStrRev[nStrRevNb - 1 - i];
	*pnStrDstNb = nStrRevNb;

}
#else
// Renvoie une chaîne à l'envers du dictionnaire.
void LZW_GetTranslation(u32 nStrCode, u8 *pStrRev, u32 *pnStrDstNb, u32 nClearCode)
{
	u32	nStrRevNb;

	// Récupération de la chaîne.
	nStrRevNb = 0;
	while (1)
	{
		if (nStrCode >= gnDicNext)
		{
			fprintf(stderr, "LZW_GetTranslation(): Error, code not defined in dictionnary.\n");
			exit(1);
		}

		// Code < au clear code => direct.
		if (nStrCode < nClearCode)
		{
			pStrRev[nStrRevNb++] = nStrCode;
			break;
		}
		pStrRev[nStrRevNb++] = gpStrBuf[nStrCode].nAppendChar;
		nStrCode = gpStrBuf[nStrCode].nPrevCode;
	}

	*pnStrDstNb = nStrRevNb;

}
#endif

// Copie de la chaîne LZW dans le buffer.
void GIF_LZWStrOutput(u8 **pImg, u8 *pStrDst, u32 nStrDstNb, u32 *l,
	u32 nRenderTransparentClrIdx, u32 nBlkWidth, u32 nGifWidth)
{
	s32	i;

#if LZW_REVERSE == 0
	for (i = 0; i < nStrDstNb; i++)
#else
	for (i = nStrDstNb - 1; i >= 0; i--)
#endif
	{
		if (nRenderTransparentClrIdx != pStrDst[i]) **pImg = pStrDst[i];
		*pImg += 1;
		if (--(*l) == 0)
		{
			*l = nBlkWidth;
			*pImg += nGifWidth - nBlkWidth;
		}
	}

}


// Parcours des blocs d'un fichier.
void GIF_GetNextImage(struct SGIFFile *pGif)
{
	s32	i;
	u8	*pBuf;
	u32	nDisposalMethod;


	// Phase d'init ?
	if (pGif->pLogicalScrDesc == NULL)
	{
		// Phase d'initialisation.
		pBuf = pGif->pData;

		// Header :
		// 3 bytes : Signature = "GIF".
		// 3 bytes : Version = "89a".
		if (strncmp((char *)pBuf, "GIF89a", 6))
		{
			fprintf(stderr, "GIF_GetNextImage(): Wrong header.\n");
			return;
		}
		pBuf += 6;

		// Logical Screen Descriptor.
		pGif->pLogicalScrDesc = (struct SGIFBlk_LogicalScrDesc *)pBuf;
		pBuf += 7;
		// Global Color Table (Optional).
		if (pGif->pLogicalScrDesc->nGlobalClrTableFlag)
		{
			i = 3 * (1 << (pGif->pLogicalScrDesc->nGlobalClrTableSz + 1));	// Size of color table.
			memcpy(pGif->pGlobalPal, pBuf, i);
			pBuf += i;
		}

		// malloc du buffer pour l'image dépackée + buffer de depack (de la taille de l'image).
		if ((pGif->pImg = (u8 *)malloc(pGif->pLogicalScrDesc->nLogScrWidth * pGif->pLogicalScrDesc->nLogScrHeight * 2)) == NULL)
		{
			fprintf(stderr, "GIF_GetNextImage(): malloc failed (1). Aborted.\n");
			pGif->pLogicalScrDesc = NULL;	// !! Sert pour l'init !!
			return;
		}
		pGif->pPackedBuf = pGif->pImg + (pGif->pLogicalScrDesc->nLogScrWidth * pGif->pLogicalScrDesc->nLogScrHeight);

		// Misc inits.
		pGif->nNextDisposal = 2;	// Pour le rendu de la première frame.
		pGif->nLoop = 0;			// Init.

		// Sauvegarde des pointeurs utiles.
		pGif->pCurPtr = pBuf;		// Pointeur en cours dans le parcours.
		pGif->pOrgPtr = pBuf;		// Pointeur sur le premier bloc suivant l'init pour loop.

		return;		// Il faudra rappeler la fonction pour dépacker la première image.
	}

	//
	// Traitement du data stream.
	// "Each image in the Data Stream is composed of an Image Descriptor, an optional Local Color Table, and the image data."
	//

	pBuf = pGif->pCurPtr;		// Pointeur en cours dans le parcours.
	nDisposalMethod = pGif->nNextDisposal;	// Init du 'disposal' précédent, çàd comment traiter la dernière image rendue.
	pGif->nTransparentColorIndex = 256;	// Couleur de transparence par défaut, et 256 ne risque pas d'arriver sur 8 bits.

	// Boucle dans les blocs.
	while (*pBuf == GIF_EXTENSION_INTRODUCER || *pBuf == GIF_IMAGE_SEPARATOR || *pBuf == GIF_TRAILER)
	{
		// Fin de fichier => On repart à la première image.
		if (*pBuf == GIF_TRAILER)
		{
			if (pGif->nLoop == 0) return;		// Pas de loop, on ne fait plus rien.
			pBuf = pGif->pOrgPtr;
			pGif->nNextDisposal = 2;			// On force l'effacement d'image pour la première frame.
			// ... et on enchaîne sur la prochaine image.
		}
		else
		// Image Descriptor.
		if (*pBuf == GIF_IMAGE_SEPARATOR)
		{
			struct SGIFBlk_ImageDescriptor	*pImgDesc;
			u8	*pTmp;
			u32	nMinCodeSz;

			pImgDesc = (struct SGIFBlk_ImageDescriptor *)pBuf;
			pBuf += 10;

			// Par défaut, on pointe sur la palette générale.
			pGif->pPal = pGif->pGlobalPal;
			// Local color table ?
			if (pImgDesc->nLocalClrTableFlag)
			{
				i = 3 * (1 << (pImgDesc->nLocalClrTableSz + 1));	// Size of color table.
				memcpy(pGif->pLocalPal, pBuf, i);
				pGif->pPal = pGif->pLocalPal;
				pBuf += i;
			}

			// LZW min code size.
			nMinCodeSz = *pBuf;
			pBuf += 1;
			// Sub Data Blocks.
			// "The image data for a table based image consists of a sequence of sub-blocks."
			// 1 - On copie toutes les données à dépacker dans un buffer.
			pTmp = pGif->pPackedBuf;
			i = 0;
			while (*pBuf)	// Tant qu'on ne tombe pas sur un block terminator...
			{
				memcpy(pTmp, pBuf + 1, *pBuf);
				i += *pBuf;
				pTmp += *pBuf;
				pBuf += (*pBuf) + 1;
			}
			// On skippe le block terminator.
			pBuf += 1;

			// Sauvegarde des infos utiles pour le prochain passage, car après le depack on quitte.
			pGif->pCurPtr = pBuf;		// Pointeur en cours dans le parcours.



			// iv) Disposal Method - Indicates the way in which the graphic is to
			// be treated after being displayed.
			// !!! C'est le *AFTER* qui est important !!!

			// Disposal method :
			//0 -   No disposal specified. The decoder is not required to take any action.
			//1 -   Do not dispose. The graphic is to be left in place.
			//2 -   Restore to background color. The area used by the graphic must be restored to the background color.
			//3 -   Restore to previous. The decoder is required to restore the area overwritten by the graphic with what was there prior to rendering the graphic.
			//4-7 - To be defined.

			// 0 & 1 : Rien à faire / 2 : ok / 3 : à faire ? (Il faudrait trouver un gif qui l'utilise).

			// 2 - On remplit le background avec la couleur de fond.
			// (A voir: Seulement sur le rectangle concerné ?).
			if (nDisposalMethod == 2)
			{
				// Byte par byte.
//				for (i = 0; i < pGif->pLogicalScrDesc->nLogScrWidth * pGif->pLogicalScrDesc->nLogScrHeight; i++)
//					pGif->pImg[i] = pGif->pLogicalScrDesc->nBkgClrIdx;

				// Par quads.
				u8	*pTmp8;
				u32	*pTmp32 = (u32 *)pGif->pImg;
// v1
//				u32	nClr = (pGif->pLogicalScrDesc->nBkgClrIdx << 24) | (pGif->pLogicalScrDesc->nBkgClrIdx << 16) |
//							(pGif->pLogicalScrDesc->nBkgClrIdx << 8) | pGif->pLogicalScrDesc->nBkgClrIdx;

// Test. (Certains cas posent problème dans certains GIFs, bkg color != transp color).
				u32	nClr;
				u32	nTmp = pGif->pLogicalScrDesc->nBkgClrIdx;
				if (pGif->nTransparentColorIndex < 256)
					if (pGif->nTransparentColorIndex != pGif->pLogicalScrDesc->nBkgClrIdx)
					{
						printf("GIF_GetNextImage(): Force bkg color(%d) to transparent(%d).\n", pGif->pLogicalScrDesc->nBkgClrIdx, pGif->nTransparentColorIndex);
						nTmp = pGif->nTransparentColorIndex;
					}
				nClr = (nTmp << 24) | (nTmp << 16) | (nTmp << 8) | nTmp;


				u32	n4 = (pGif->pLogicalScrDesc->nLogScrWidth * pGif->pLogicalScrDesc->nLogScrHeight);
				u32	n1 = n4 & 3;
				n4 >>= 2;
				for (; n4; n4--) *pTmp32++ = nClr;
				pTmp8 = (u8 *)pTmp32;
				for (; n1; n1--) *pTmp8++ = nClr;

//				memset(pGif->pImg, pGif->pLogicalScrDesc->nBkgClrIdx, pGif->pLogicalScrDesc->nLogScrWidth * pGif->pLogicalScrDesc->nLogScrHeight);

			}



			// 2 - Depack.

u32	l;
l = pImgDesc->nImgWidth;
u8	*pImg = pGif->pImg + (pImgDesc->nImgTop * pGif->pLogicalScrDesc->nLogScrWidth) + pImgDesc->nImgLeft;

			u8	pStrDst[4096];
			u32	nStrDstNb;
			u32	nBitPos = 0;	// Position dans les bits.
			u32	nCodeSz = nMinCodeSz + 1;	// Taille du code à lire.
			u32	nClearCode = 1 << nMinCodeSz;
			u32	nEndOfInfoCode = nClearCode + 1;

			u32	nOldCode, nNewCode;

			// Le premier code lu doit être un clear code.
			nOldCode = GIF_GetBits(pGif->pPackedBuf, nBitPos, nCodeSz);
			nBitPos += nCodeSz;
			if (nOldCode != nClearCode)
			{
				fprintf(stderr, "GIF_GetNextImage(): Depack: FATAL - First code is not a Clear Code.\n");
				exit (1);
			}

			// Algo de depack LZW utilisé.
			/*
				Read OLD_CODE
				output OLD_CODE
				//CHARACTER = OLD_CODE => inutile.
				WHILE there are still input characters DO
					Read NEW_CODE
					IF NEW_CODE is not in the translation table THEN
						STRING = get translation of OLD_CODE
						STRING = STRING + STRING[0]  // STRING = STRING+CHARACTER  // bug, c'est bien + STRING[0].
					ELSE
						STRING = get translation of NEW_CODE
					END of IF
					output STRING
					CHARACTER = first character in STRING
					add OLD_CODE + CHARACTER to the translation table
					OLD_CODE = NEW_CODE
				END of WHILE
			*/

_reset:
			gnDicNext = nEndOfInfoCode + 1;	// Première position dispo, à <clearcode + 2>.
			nCodeSz = nMinCodeSz + 1;		// Taille du code à lire.

			// Premier code.
			// Read OLD_CODE
			nOldCode = GIF_GetBits(pGif->pPackedBuf, nBitPos, nCodeSz);
			nBitPos += nCodeSz;
			// output OLD_CODE
			LZW_GetTranslation(nOldCode, pStrDst, &nStrDstNb, nClearCode);

			GIF_LZWStrOutput(&pImg, pStrDst, nStrDstNb, &l, pGif->nTransparentColorIndex, pImgDesc->nImgWidth, pGif->pLogicalScrDesc->nLogScrWidth);

			while (1)
			{
				// Read NEW_CODE
				nNewCode = GIF_GetBits(pGif->pPackedBuf, nBitPos, nCodeSz);
				nBitPos += nCodeSz;

				// C'est un code de chaîne.
				if (nNewCode < gnDicNext)
				{
					// Codes spéciaux ?
					if (nNewCode == nEndOfInfoCode) break;
					if (nNewCode == nClearCode) goto _reset;

					// Le code existe dans le dictionnaire.
					// STRING = get translation of NEW_CODE
					LZW_GetTranslation(nNewCode, pStrDst, &nStrDstNb, nClearCode);
				}
				else
				{
					// Le code n'existe pas dans le dictionnaire.
					if (nNewCode != gnDicNext)
					{
						fprintf(stderr, "GIF_GetNextImage(): Depack: FATAL - Unexpected code encountered.\n");
						exit (1);
					}
					// STRING = get translation of OLD_CODE
					// STRING = STRING + STRING[0]  // STRING = STRING+CHARACTER
				#if LZW_REVERSE == 0
					LZW_GetTranslation(nOldCode, pStrDst, &nStrDstNb, nClearCode);
					pStrDst[nStrDstNb++] = pStrDst[0];
				#else
					LZW_GetTranslation(nOldCode, &pStrDst[1], &nStrDstNb, nClearCode);
					pStrDst[0] = pStrDst[nStrDstNb];
					nStrDstNb++;
				#endif
				}

				// output STRING
				GIF_LZWStrOutput(&pImg, pStrDst, nStrDstNb, &l, pGif->nTransparentColorIndex, pImgDesc->nImgWidth, pGif->pLogicalScrDesc->nLogScrWidth);

				// CHARACTER = first character in STRING
				// add OLD_CODE + CHARACTER to the translation table
			#if LZW_REVERSE == 0
				LZW_Add2Dictionnary(nOldCode, pStrDst[0]);
			#else
				LZW_Add2Dictionnary(nOldCode, pStrDst[nStrDstNb - 1]);
			#endif
				// La taille du code augmente ?
				if (gnDicNext & (1 << nCodeSz))
				{
					if (nCodeSz < 12)	// 12 bits max. Si table pleine, plus de changements.
						nCodeSz++;
				}

				// OLD_CODE = NEW_CODE
				nOldCode = nNewCode;
			}

			return;		// Une image vient d'être dépackée, on quitte.

		}
		else
		// Autres codes (0x21 + code de l'extension).
		switch (*(pBuf + 1))
		{
		case 0x01:	// Plain Text Descriptor (non géré).
			pBuf += 2;
			pBuf += (*pBuf) + 1;
			// On pointe maintenant sur un data sub-block :
			while (*pBuf)	// Tant qu'on ne tombe pas sur un block terminator...
			{
				pBuf += (*pBuf) + 1;
			}
			// On skippe le block terminator.
			pBuf += 1;
			break;

		case 0xF9:	// Graphic Control Extension.
			pBuf += 2;
			{
				struct SGIFBlk_GraphicControlExtension	*pGCExt;
				pGCExt = (struct SGIFBlk_GraphicControlExtension *)pBuf;

				// Y a-t'il une couleur de transparence pour le rendu de la frame ?
				pGif->nTransparentColorIndex = 256;		// Par défaut, et 256 ne risque pas d'arriver sur 8 bits.
				if (pGCExt->nTransparentClrFlag)
				{
					pGif->nTransparentColorIndex = pGCExt->nTransparentClrIdx;
				}
				nDisposalMethod = pGif->nNextDisposal;			// Pour le rendu de cette frame.
				pGif->nNextDisposal = pGCExt->nDisposalMethod;	// Pour le rendu de l'image suivante.

				pBuf += pGCExt->nBlkSz + 1;
			}
			// On skippe le block terminator.
			pBuf += 1;
			break;

		case 0xFE:	// Comment Extension.
			pBuf += 2;
			// On pointe maintenant sur un data sub-block :
			while (*pBuf)	// Tant qu'on ne tombe pas sur un block terminator...
			{
				pBuf += (*pBuf) + 1;
			}
			// On skippe le block terminator.
			pBuf += 1;
			break;

		case 0xFF:	// Application Extension.
			pBuf += 2;
			{
				struct SGIFBlk_AppliExtension	*pAppliExt;
				pAppliExt = (struct SGIFBlk_AppliExtension *)pBuf;

				// Bloc de loop ? (Le bloc NETSCAPE2.0 ne sert qu'à ça, le compteur à l'intérieur est ignoré (cf. doc)).
				if (strncmp((char *)pAppliExt->pAppliId, "NETSCAPE2.0", 11) == 0) pGif->nLoop = 1;

				pBuf += pAppliExt->nBlkSz + 1;
				// On pointe maintenant sur un data sub-block :
				while (*pBuf)	// Tant qu'on ne tombe pas sur un block terminator...
				{
					pBuf += (*pBuf) + 1;
				}
				// On skippe le block terminator.
				pBuf += 1;
			}
			break;

		default :
			printf("Unhandled block id found: Code %x.\n", (int)*(pBuf + 1));
			return;
			break;
		}

	}

/*
	// Quand on sort, on doit tomber sur le trailer.
	if (*pBuf != GIF_TRAILER)
	{
		printf("GIF_GetNextImage(): Trailer not found.\n");
		return;
	}
	printf("End of file: GIF Trailer found.\n");
*/

}

// Libère une image.
void GIF_Free(struct SGIFFile *pGif)
{
	if (pGif->pData != NULL) free(pGif->pData);
	if (pGif->pImg != NULL) free(pGif->pImg);

	free(pGif);
}

// Lecture d'un GIF.
struct SGIFFile * GIF_Load(char *pFilename)
{
	struct	SGIFFile	*pGif = NULL;
	FILE	*pFile = NULL;
	s32	i;

	// Intialisation d'une structure pour l'image à lire.
	if ((pGif = (struct SGIFFile *)malloc(sizeof(struct SGIFFile))) == NULL)
	{
		fprintf(stderr, "GIF_Load(): malloc failed (1). Aborted.\n");
		goto _GIFErr;
	}
	pGif->pData = NULL;		// Fichier chargé.
	pGif->pImg = NULL;		// Image en cours.
	pGif->pLogicalScrDesc = NULL;	// !! Sert pour l'init !!
	//

	// Ouverture du fichier.
	if ((pFile = fopen(pFilename, "rb")) == NULL)
	{
		fprintf(stderr, "GIF_Load(): Unable to open file '%s'.\n", pFilename);
		goto _GIFErr;
	}
	// Taille du fichier.
	fseek(pFile, 0, SEEK_END);		// Fin de fichier.
	pGif->nDataSz = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);		// Début du fichier.
	// Malloc.
	if ((s32)pGif->nDataSz <= 0)
	{
		fprintf(stderr, "GIF_Load(): Error, wrong file size (%d).\n", (int)pGif->nDataSz);
		goto _GIFErr;
	}
	if ((pGif->pData = (u8 *)malloc(pGif->nDataSz)) == NULL)
	{
		fprintf(stderr, "GIF_Load(): malloc failed (2).\n");
		goto _GIFErr;
	}
	// Lecture.
	i = fread(pGif->pData, 1, pGif->nDataSz, pFile);
	fclose(pFile); pFile = NULL;
	if (i != pGif->nDataSz)
	{
		fprintf(stderr, "GIF_Load(): Load error.\n");
		goto _GIFErr;
	}
//	printf("File '%s' loaded.\n", pFilename);

	// Initialisation.
	GIF_GetNextImage(pGif);

	return (pGif);

_GIFErr:
	if (pFile != NULL) fclose(pFile);
	if (pGif != NULL) GIF_Free(pGif);
	return (NULL);
}




