
#include "includes.h"


// Constructeur.
struct SCmdfile * Cmdfile_New(void)
{
	struct SCmdfile *psCmdfile;

	if ((psCmdfile = (struct SCmdfile *)malloc(sizeof(struct SCmdfile))) == NULL)
	{
		fprintf(stderr, "Cmdfile_New(): malloc failed.\n");
		exit(1);
	}

	psCmdfile->pCmdfile = NULL;
	memset(psCmdfile->pCmdFilename, 0, sizeof(psCmdfile->pCmdFilename));
	psCmdfile->nFileDate = 0;

	return (psCmdfile);
}

// Destructeur.
void Cmdfile_Delete(struct SCmdfile *psCmdfile)
{
	if (psCmdfile->pCmdfile != NULL)
	{
		free(psCmdfile->pCmdfile);
		psCmdfile->pCmdfile = NULL;
	}

	free(psCmdfile);
}

// Lecture d'un fichier de commandes.
void Cmdfile_CmdfileLoad(struct SCmdfile *psCmdfile, char *pFilename)
{
	FILE	*fPt;
	s32	nSz;
	u32	i;

	// Sauvegarde du nom du fichier pour sauvegarde.
	strcpy(psCmdfile->pCmdFilename, pFilename);

	// Ouverture + lecture du fichier.
	if ((fPt = fopen(pFilename, "rb")) == NULL)
	{
		fprintf(stderr, "Cmdfile_CmdfileLoad(): Error opening file.\n");
		exit(1);
	}
	// Détermination de la taille.
	fseek(fPt, 0, SEEK_END);
	nSz = ftell(fPt);
	fseek(fPt, 0, SEEK_SET);

	// Date du fichier.
	struct stat sStat;
	if(stat(pFilename, &sStat) == -1)
	{
		// erreur (?)
		psCmdfile->nFileDate = 0;
	}
	else
	{
		//	time_t	st_atime;	// Accessed date (always 00:00 hrs local * on FAT)
		//	time_t	st_mtime;	// Modified time
		//	time_t	st_ctime;	// Creation time

		// Récupère la date la plus récente (créé/modifié).
		psCmdfile->nFileDate = sStat.st_ctime;
		if ((u32)sStat.st_mtime > psCmdfile->nFileDate) psCmdfile->nFileDate = sStat.st_mtime;
	}

	// Malloc du buffer de lecture.
	if ((psCmdfile->pCmdfile = (char *)malloc(nSz + 1)) == NULL)
	{
		fprintf(stderr, "Cmdfile_CmdfileLoad(): malloc failed (pCmdfile).\n");
		exit(1);
	}

	// Lecture.
	i = fread(psCmdfile->pCmdfile, 1, nSz, fPt);
	fclose(fPt);
	if (i != (u32)nSz)
	{
		fprintf(stderr, "Cmdfile_CmdfileLoad(): Read error.\n");
		exit(1);
	}
	psCmdfile->pCmdfile[nSz] = 0;	// Termine la chaîne;

}

// Lecture d'un fichier de commandes, depuis un buffer.
void Cmdfile_CmdfileLoadFromBuf(struct SCmdfile *psCmdfile, char *pFilename, char *pBuf, u32 nDate)
{
	s32	nSz;

	// Sauvegarde du nom du fichier pour sauvegarde.
	strcpy(psCmdfile->pCmdFilename, pFilename);

	nSz = strlen(pBuf);

	// Date du fichier.
	psCmdfile->nFileDate = nDate;

	// Malloc du buffer de lecture.
	if ((psCmdfile->pCmdfile = (char *)malloc(nSz + 1)) == NULL)
	{
		fprintf(stderr, "Cmdfile_CmdfileLoadFromBuf(): malloc failed (pCmdfile).\n");
		exit(1);
	}

	// Lecture.
	strcpy(psCmdfile->pCmdfile, pBuf);

}

// Exporte le fichier de commandes courant.
void Cmdfile_CmdFileExport(struct SCmdfile *psCmdfile)
{
	FILE	*fPt;
	s32	nSz;
	u32	nNewDate;

	// Ouverture + lecture du fichier.
	if ((fPt = fopen(psCmdfile->pCmdFilename, "wb")) == NULL)
	{
		fprintf(stderr, "Cmdfile_CmdFileExport(): Error opening file.\n");
		exit(1);
	}

	// Ecriture.
	nSz = fwrite(psCmdfile->pCmdfile, 1, strlen(psCmdfile->pCmdfile), fPt);
	fclose(fPt);

	// Date du fichier (update de la date de notre fichier, pour ne pas faire un update à la prochaine lecture).
	struct stat sStat;
	if (stat(psCmdfile->pCmdFilename, &sStat) == -1)
	{
		// erreur (?)
		printf("Cmdfile_CmdFileExport(): Unable to retrieve file date.\n");
	}
	else
	{
		//	time_t	st_atime;	// Accessed date (always 00:00 hrs local * on FAT)
		//	time_t	st_mtime;	// Modified time
		//	time_t	st_ctime;	// Creation time

		// Récupère la date la plus récente (créé/modifié).
		nNewDate = sStat.st_ctime;
		if ((u32)sStat.st_mtime > nNewDate) nNewDate = sStat.st_mtime;

		psCmdfile->nFileDate = nNewDate;	// Update.
	}

	printf("Command file '%s' exported.\n", psCmdfile->pCmdFilename);

}

// Recherche si le monstre n°x est présent dans le fichier de commandes.
// Renvoie NULL si non trouvé, un ptr dessus si trouvé.
char * CmdMstExistsInFile(char *pCmd, u32 nMstNo)
{
	char	pRech[50];
//	//char	pEndOfRec[] = "\r\n\r\n";
//	char	pNum[33];

	// On créé le texte "[mst<x>]" pour recherche.
//	strcpy(pRech, "[mst");
//	//itoa(nMstNo, pNum, 10);
//	snprintf(pNum, sizeof(pNum), "%d", (int)nMstNo);	// pas de itoa en Linux...
//	strcat(pRech, pNum);
//	strcat(pRech, "]");
	snprintf(pRech, sizeof(pRech), "[mst%d]", (int)nMstNo);	// pas de itoa en Linux...

	return (strstr(pCmd, pRech));

}
// La même, dans le fichier en cours.
char * Cmdfile_CmdMstExists(struct SCmdfile *psCmdfile, u32 nMstNo)
{
	return (CmdMstExistsInFile(psCmdfile->pCmdfile, nMstNo));
}

// Cherche une ligne vide.
char * StrSearchBlankLine(char *pSrc)
{
	char	pEndOfLine[] = "\n";
	char	*pDst, *pTmp;

	while (1)
	{
		pDst = strstr(pSrc, pEndOfLine);
		if (pDst == NULL) return (NULL);
		pTmp = pSrc;
		// On saute les caractères blancs.
		while ((*pSrc == ' ' || *pSrc == '\t' || *pSrc == '\r' || *pSrc == '\n') && pSrc != pDst) pSrc++;
		if (pSrc == pDst) return (pTmp);	// On renvoie le ptr sur le début de la ligne.
		pSrc = pDst + strlen(pEndOfLine);
	}

}

// Recherche d'un mst dans le fichier de commandes.
// In : pMst = buffer pour copier le monstre.
// Out : 0 si pas trouvé / 1 si trouvé + pMst contient le monstre.
u32 CmdMstSeekInFile(char *pCmd, u32 nMstNo, char *pMst)
{
	char	*pDst, *pDst2;

	pMst[0] = 0;

	// Recherche le monstre.
	pDst = CmdMstExistsInFile(pCmd, nMstNo);
	if (pDst == NULL)
	{
		// Chaîne non trouvée.
		return (0);
	}

	// Chaîne trouvée => recherche de la fin des prms du monstre.
	pDst2 = StrSearchBlankLine(pDst);
	if (pDst2 == NULL)
	{
		printf("Mst %d : End of params not found.\n", (int)nMstNo);
		return (0);
	}
	pDst2--;	// On se remet sur le '\n' précédent la chaîne vide.

	// Fin de params trouvée, on recopie ce qui concerne le monstre dans son buffer.
	u32	nSzMst;

	nSzMst = pDst2 - pDst;
	strncpy(pMst, pDst, nSzMst);
	pMst[nSzMst] = 0;

	// On supprime les caractères génants (On les remplace par des espaces).
	pDst = pMst;
	while (*pDst)
	{
		if (*pDst == '\r') *pDst = ' ';
		pDst++;
	}

//		printf("Found :\n%s\n", pMst);
	return (1);
}
// La même, mais sans le nom de fichier.
u32 Cmdfile_CmdMstSeek(struct SCmdfile *psCmdfile, u32 nMstNo, char *pMst)
{
	return (CmdMstSeekInFile(psCmdfile->pCmdfile, nMstNo, pMst));
}

// Récupère le paramètre x d'un monstre.
// In : pMst = Le monstre (récupéré par CmdMstSeek par ex) / nPrmNo = n° du prm
// Out : 0 si pas trouvé / 1 si trouvé + pPrm contient la ligne.
u32 CmdMstGetPrmNo(char *pMst, u32 nPrmNo, char *pPrm)
{
//	char	pEndOfLine[] = "\r\n";
	char	pEndOfLine[] = "\n";
	char	*pDst, *pDst2;
	u32	i;
	u32	nSzMst;

	i = nPrmNo + 1;
	pDst = pMst;
	while (i)
	{
		pDst = strstr(pDst, pEndOfLine);
		if (pDst == NULL)
		{
			return (0);
		}
		pDst += strlen(pEndOfLine);
		i--;
	}

	pDst2 = strstr(pDst, pEndOfLine);
	if (pDst2 == NULL)
	{
		// Si NULL, c'est qu'on était sur le dernier.
		pDst2 = pDst + strlen(pDst);
	}

	// Recopie du prm dans la chaîne destination.
	nSzMst = pDst2 - pDst;
	strncpy(pPrm, pDst, nSzMst);
	pPrm[nSzMst] = 0;

/*
//=> Fait dans CmdMstSeekInFile().
	// On dégage d'éventuels caractères génants (on les remplace par des espaces).
	for (i = 0; i < nSzMst; i++)
	{
		if (pPrm[i] == '\r') pPrm[i] = ' ';
	}
*/

//printf("rech prm : %s\n", pPrm);

	return (1);
}

// Récupère le nombre de paramètres d'un monstre.
// In : pMst = Le monstre (récupéré par CmdMstSeek par ex).
// Out : Nb de params du monstre.
u32 CmdMstGetPrmNb(char *pMst)
{
//	char	pEndOfLine[] = "\r\n";
	char	pEndOfLine[] = "\n";
	char	*pDst;
	u32	nNbPrm = 0;

	pDst = pMst;
	while (1)
	{
		pDst = strstr(pDst, pEndOfLine);
		if (pDst == NULL)
		{
			break;
		}
		pDst += strlen(pEndOfLine);
		nNbPrm++;
	}
//printf("nbprm = %d\n", (int)nNbPrm);

	return (nNbPrm);
}

// Encapsulation avec rajout d'un prm.
u32 CmdPrmSplit(char *pPrm, char *pPrmName, u32 *pPrmBitStart, u32 *pPrmBitEnd, u32 *pPrmSign, char *pPrmArgs)
{
	s32	nPrmDefaultValue;
	return (CmdPrmSplit2(pPrm, pPrmName, pPrmBitStart, pPrmBitEnd, pPrmSign, &nPrmDefaultValue, pPrmArgs));
}

// Splitte un paramètre en nom/bit start/bit end/arguments.
// Out : Nb d'infos récupérées. Si 3, valeur numérique. Si 4, prm normal avec des valeurs.
u32 CmdPrmSplit2(char *pPrm, char *pPrmName, u32 *pPrmBitStart, u32 *pPrmBitEnd, u32 *pPrmSign, s32 *pPrmDefaultValue, char *pPrmArgs)
{
	char	*pDst, *pDst2, *pDst3;
	u32	nStrLn;
	u32	nRetNb = 0;
	s32	nConv;
	char	pAtoi[256];

	pPrmName[0] = 0;
	pPrmArgs[0] = 0;
	*pPrmBitStart = 0;
	*pPrmBitEnd = 0;
	*pPrmSign = 0;
	*pPrmDefaultValue = 0;

	// Skippe les espaces en début de ligne.
	while (*pPrm == ' ') pPrm++;


	// Nom.
	pDst = pPrm;
	if ((pDst2 = strstr(pDst, "=")) == NULL) return (nRetNb);

	nStrLn = pDst2 - pDst;
	strncpy(pPrmName, pDst, nStrLn);
	pPrmName[nStrLn] = 0;
	nRetNb++;

	// Valeur par défaut ?
	if ((pDst3 = strstr(pPrmName, "(")) != NULL)
	{
		*pDst3++ = 0;	// Coupe la chaîne pPrmName au niveau de la parenthèse.
		*pPrmDefaultValue = atoi(pDst3);
//printf("prm '%s' : default value = %d\n", pPrmName, (int)*pPrmDefaultValue);
	}

	// Suppression des espaces en fin de ligne.
	nStrLn = strlen(pPrmName);
	while (nStrLn && pPrmName[nStrLn - 1] == ' ') pPrmName[--nStrLn] = 0;


	// Bit start. Si la valeur est précédée de '-' => signée.
	pDst = pDst2 + 1;
	if ((pDst2 = strstr(pDst, ":")) == NULL) return (nRetNb);

	nStrLn = pDst2 - pDst;
	strncpy(pAtoi, pDst, nStrLn);
	pAtoi[nStrLn] = 0;
	//*pPrmBitStart = atoi(pAtoi);
	nConv = atoi(pAtoi);
	if (nConv < 0)
	{
		nConv = -nConv;
		*pPrmSign = 1;
	}
	else if (nConv == 0 && strstr(pDst, "-") != NULL)	// Cas particulier du "-0". (Parce qu'évidement, -0...).
	{
		*pPrmSign = 1;
	}
	*pPrmBitStart = nConv;
	nRetNb++;

	// Bit end.
	pDst = pDst2 + 1;
	if ((pDst2 = strstr(pDst, ":")) == NULL) return (nRetNb);

	nStrLn = pDst2 - pDst;
	strncpy(pAtoi, pDst, nStrLn);
	pAtoi[nStrLn] = 0;
	*pPrmBitEnd = atoi(pAtoi);
	nRetNb++;

	// Prm list.
	pDst = pDst2 + 1;
	strcpy(pPrmArgs, pDst);
	if ((pDst2 = strstr(pPrmArgs, ";")) != NULL) *pDst2 = 0;	// A partir d'un ';', le reste de la ligne est du commentaire.
	if ((pDst2 = strstr(pPrmArgs, "-")) != NULL)
	{
		nRetNb++;		// Si il y a des séparateurs d'args, c'est qu'il y a des args.
		*pPrmSign = 0;	// Et on force le signe à 0 dans ce cas.
	}

	return (nRetNb);
}

// Récupère l'argument x d'une série d'arguments d'une ligne de paramètres.
// In : pArgs = Les arguments d'un prm (récupérés par CmdPrmSplit par ex) / nPrmNo = n° du prm
// Out : 0 si pas trouvé / 1 si trouvé + pArg contient la ligne.
u32 CmdMstGetArgNo(char *pArgList, u32 nArgNo, char *pArg)
{
	char	pSeparateur[] = "-";
	char	*pDst, *pDst2;
	u32	i;
	u32	nSzArg;

	pArg[0] = 0;

	i = nArgNo;
	pDst = pArgList;
	while (i)
	{
		pDst = strstr(pDst, pSeparateur);
		if (pDst == NULL)
		{
			return (0);
		}
		pDst += strlen(pSeparateur);
		i--;
	}

	pDst2 = strstr(pDst, pSeparateur);
	if (pDst2 == NULL)
	{
		// Si NULL, c'est qu'on était sur le dernier.
		pDst2 = pDst + strlen(pDst);
	}

	// Suppression des espaces au début.
	while (*pDst == ' ' && pDst < pDst2) pDst++;
	if (pDst == pDst2) return (0);		// pDst est arrivé à pDst2 => que des espaces.

	// Recopie du prm dans la chaîne destination.
	nSzArg = pDst2 - pDst;
	strncpy(pArg, pDst, nSzArg);
	pArg[nSzArg] = 0;

	// Suppression des espaces en fin de ligne.
	while (nSzArg && pArg[nSzArg - 1] == ' ') pArg[--nSzArg] = 0;

	return (1);
}

//=============================================================================
//=============================================================================
//=============================================================================

// Constructeur.
struct SMstUnit * MstUnit_New(void)
{
	struct SMstUnit	*psMstUnit;

	if ((psMstUnit = (struct SMstUnit *)malloc(sizeof(struct SMstUnit))) == NULL)
	{
		fprintf(stderr, "MstUnit_New(): malloc failed (psMstUnit).\n");
		exit(1);
	}

	psMstUnit->pData = NULL;
	psMstUnit->nMstNo = 0;		// = No du bloc.
	psMstUnit->nNbBytes = 0;	// Nb de bytes utilisés.

	return (psMstUnit);
}

// Destructeur.
void MstUnit_Delete(struct SMstUnit *psMstUnit)
{
	if (psMstUnit->pData != NULL)
	{
		free(psMstUnit->pData);
		psMstUnit->pData = NULL;
	}

	free(psMstUnit);
}

// Init depuis un buffer contenant déjà le texte du monstre (Utile pour les remaps + création de base).
void MstUnit_InitFromMstTxtBuf(struct SMstUnit *psMstUnit, char *pMstBuf, u32 nBlockNo)
{
	u32	i, nNbPrm, nHighest;

	psMstUnit->nMstNo = nBlockNo;

	// Nb d'octets : Recherche du plus grand bit utilisé par le monstre.
	nNbPrm = CmdMstGetPrmNb(pMstBuf);
printf("MstUnit_InitFromMstTxtBuf(): Mst %d / Nb prm = %d\n", (int)psMstUnit->nMstNo, (int)nNbPrm);
	nHighest = 0;
	for (i = 0; i < nNbPrm; i++)
	{
		char	pPrm[1024];
		if (CmdMstGetPrmNo(pMstBuf, i, pPrm))
		{
			u32	nPrmBitStart, nPrmBitEnd, nPrmSign;
			char	pPrmName[1024];
			char	pPrmList[1024];
			CmdPrmSplit(pPrm, pPrmName, &nPrmBitStart, &nPrmBitEnd, &nPrmSign, pPrmList);
			if (nPrmBitEnd > nHighest) nHighest = nPrmBitEnd;
		}
	}

	psMstUnit->nNbBytes = (nHighest / 8) + 1;
printf("Highest = %d / Nb bytes = %d\n", (int)nHighest, (int)psMstUnit->nNbBytes);
	if ((psMstUnit->pData = (u8 *)malloc(psMstUnit->nNbBytes)) == NULL)
	{
		fprintf(stderr, "MstUnit_InitFromMstTxtBuf(): malloc failed.\n");
		exit(1);
	}
	memset(psMstUnit->pData, 0, psMstUnit->nNbBytes);		// RAZ pData.


	// On refait une passe ici pour initialiser les paramètres par défaut.
	for (i = 0; i < nNbPrm; i++)
	{
		char	pPrm[1024];
		if (CmdMstGetPrmNo(pMstBuf, i, pPrm))
		{
			u32	nPrmBitStart, nPrmBitEnd, nPrmSign;
			s32	nPrmDefaultValue;
			char	pPrmName[1024];
			char	pPrmList[1024];
			CmdPrmSplit2(pPrm, pPrmName, &nPrmBitStart, &nPrmBitEnd, &nPrmSign, &nPrmDefaultValue, pPrmList);
			if (nPrmDefaultValue) SetBits(psMstUnit->pData, nPrmBitStart, nPrmBitEnd, nPrmDefaultValue);
		}
	}

}

// Init depuis une page de blocs avec fichier de commandes.
void MstUnit_InitFromCmdFile(struct SMstUnit *psMstUnit, u32 nBlockNo)
{
	struct SMap	*psMap = gMisc.psMap;

	psMstUnit->nMstNo = nBlockNo;

	char	pMstBuf[1024];
	struct SPlane	*pPln = *(psMap->ppsPlanes + psMap->nActivePlane);	// !!! Attention ici !!! On suppose que la fonction ne sera appelée que pour des pages de blocs !

	if (Plane_HasCmd(pPln) == 0)
	{
		fprintf(stderr, "MstUnit_InitFromCmdFile(): Command file not found!\n");
		return;
	}
//-	if (pPln->pMst->CmdMstSeek(psMstUnit->nMstNo, pMstBuf) == 0)
	if (Cmdfile_CmdMstSeek(pPln->pMst, psMstUnit->nMstNo, pMstBuf) == 0)
	{
		fprintf(stderr, "MstUnit_InitFromCmdFile(): Mst #%d not found in command file.\n", (int)psMstUnit->nMstNo);
		return;
	}

	MstUnit_InitFromMstTxtBuf(psMstUnit, pMstBuf, nBlockNo);

}

// Init depuis un monstre existant = depuis la map.
void MstUnit_InitFromMst(struct SMstUnit *psMstUnit, struct SMstUnit *psInitialMst)
{
	// Init pData + recopie des valeurs du monstre.
	psMstUnit->nMstNo = psInitialMst->nMstNo;
	psMstUnit->nNbBytes = psInitialMst->nNbBytes;

	if ((psMstUnit->pData = (u8 *)malloc(psMstUnit->nNbBytes)) == NULL)
	{
		fprintf(stderr, "MstUnit_InitFromMst(): malloc failed.\n");
		exit(1);
	}

	memcpy(psMstUnit->pData, psInitialMst->pData, psMstUnit->nNbBytes);	// Copie des datas.
}

// Init depuis des datas de sauvegarde.
void MstUnit_InitFromSavedFile(struct SMstUnit *psMstUnit, u8 *pSav)
{
	psMstUnit->nMstNo = ((struct SMst0 *)pSav)->nMstNo;
	psMstUnit->nNbBytes = ((struct SMst0 *)pSav)->nNbBytes;	// Nb de bytes utilisés.

	if ((psMstUnit->pData = (u8 *)malloc(psMstUnit->nNbBytes)) == NULL)
	{
		fprintf(stderr, "MstUnit_InitFromSavedFile(): malloc failed.\n");
		exit(1);
	}

	memcpy(psMstUnit->pData, pSav + sizeof(struct SMst0), psMstUnit->nNbBytes);	// Copie des datas.
}

// Lecture d'un champ de bits.
s32 GetBits(u8 *pData, u32 nBitStart, u32 nBitEnd, u32 nSign)
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

/*
// v1, voir ci-dessous pour beaucoup plus efficace !
	// Faut-il étendre le signe ?
	if (nSign)
	nDstMsk = 1 << (nBitEnd - nBitStart);
	if (nRet & nDstMsk)
	{
		while(nDstMsk)
		{
			nRet |= nDstMsk;
			nDstMsk <<= 1;
		}
	}
*/
	// Faut-il étendre le signe ?
	if (nSign)
	{
		nDstMsk = ((u32)-1) << (nBitEnd - nBitStart);
		// Comme les bits supérieurs sont à 0, ça ne va matcher que le bit de signe (sur la taille demandée).
		if (nRet & nDstMsk) nRet |= nDstMsk;
	}

	return (nRet);
}

// Ecriture d'un champ de bits.
void SetBits(u8 *pData, u32 nBitStart, u32 nBitEnd, s32 nVal)
{
	u32	nSrcMsk;
	u8 nDstMsk;
	u32	nCurByte;
	u32	i;

	if (nBitEnd < nBitStart) return;
	if (nBitEnd - nBitStart + 1 > 32) return;

	nSrcMsk = 1;
	nDstMsk = 1 << (nBitStart & 7);
	i = nBitEnd - nBitStart + 1;
	nCurByte = nBitStart >> 3;

	while (i)
	{
//printf("%d\n", (int)nDstMsk);
		pData[nCurByte] &= ~nDstMsk;	// Cleare le bit.      => 0
		if (nVal & nSrcMsk) pData[nCurByte] |=  nDstMsk;	// Ou 1.
		nSrcMsk <<= 1;
		nDstMsk <<= 1;
		if (nDstMsk == 0)
		{
			nDstMsk = 1;
			nCurByte++;
		}
		i--;
	}

}


