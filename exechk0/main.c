
// Checksum dans l'exe du Mini Slug.
// Clément '17o2!!' CORDE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "ctypes.h"

// Calcul d'un Checksum.
u32 ChecksumCalc(u8 *pBuf, u32 nSz)
{
	u32	i;
	u32	nSum = 0;

	for (i = 0; i < nSz / 4; i++)
	{
		nSum ^= *(u32 *)pBuf;
		pBuf += sizeof(u32);
	}
	for (i = 0; i < (nSz & 3); i++)
		nSum += *pBuf++;

	return (nSum);
}

// Recherche d'une chaîne dans un buffer binaire.
u8 * StrFind(u8 *pToFind, u32 nToFindSz, u8 *pToSearch, u32 nToSearchSz)
{
	if (nToFindSz > nToSearchSz) return (NULL);

	u32	i;
	for (i = 0; i <= nToSearchSz - nToFindSz; i++)
	{
		// On ne compare que le premier char pour commencer.
		if (*pToSearch == *pToFind)
			if (strncmp((char *)pToSearch, (char *)pToFind, nToFindSz) == 0) return (pToSearch);
		pToSearch++;
	}
	return (NULL);
}

int main(int argc, char **argv)
{
	if (argc <= 1)
	{
		printf("Usage: exechk <exefile>\n");
		exit (0);
	}

	// Here we go.

	FILE	*fPt = NULL;
	u8	*pBuf = NULL;

	// Ouverture du fichier.
	if ((fPt = fopen(argv[1], "rb+")) == NULL)
	{
		fprintf(stderr, "Error opening file '%s'.\n", argv[1]);
		goto _err_exit1;
	}
	// Récupération de la taille du fichier.
	s32	nFileSzToLoad, nSz;
	fseek(fPt, 0L, SEEK_END);
	nFileSzToLoad = ftell(fPt);
	fseek(fPt, 0L, SEEK_SET);
	printf("fsize = %d\n", nFileSzToLoad);
	// Malloc du buffer de lecture.
	if ((pBuf = (u8 *)malloc(nFileSzToLoad)) == NULL)
	{
		fprintf(stderr, "malloc failed (pBuf).\n");
		goto _err_exit1;
	}
	// Lecture.
	nSz = fread(pBuf, 1, nFileSzToLoad, fPt);
	if (nSz != nFileSzToLoad)
	{
		fprintf(stderr, "fread error.\n");
		goto _err_exit1;
	}

	// Recherche de la chaîne qui va bien.
	char	*pStrToFind = "CC\x17\x02";
	u8	*pPtr;

	pPtr = StrFind((u8 *)pStrToFind, 4, pBuf, nFileSzToLoad);
	if (pPtr == NULL)
	{
		fprintf(stderr, "Substring not found. Aborted.\n");
		goto _err_exit1;
	}
	printf("Substring found at offset %X\n", (int)(pPtr - pBuf));
	// On efface les 4 cases du checksum.
	memset(pPtr + 4, 0, 4);
	// Calcul du Checksum.
	u32	nSum = 0;
	nSum = ChecksumCalc(pBuf, nFileSzToLoad);
	printf("Checksum = %X\n", nSum);

	// Ecriture du checksum.
	if (fseek(fPt, (u32)(pPtr - pBuf) + 4, SEEK_SET) == 0)
		fwrite(&nSum, 1, sizeof(u32), fPt);
	else
		fprintf(stderr, "fseek errror.\n");

	free(pBuf); pBuf = NULL;
	fclose(fPt); fPt = NULL;
	return (0);

_err_exit1:
	if (fPt != NULL) fclose(fPt);
	if (pBuf != NULL) free(pBuf);
	exit (1);

}


/*
	// Calcul du Checksum.
	u32	nSum = 0;
	if ((int)(pPtr - pBuf) > 0)
	{
		printf("Part 1: From %X to %X, sz = %d\n", 0, (int)(pPtr - pBuf) - 1, (int)(pPtr - pBuf));
		nSum = ChecksumCalc(pBuf, (pPtr - pBuf));
	}
	if ((int)((pBuf + nFileSzToLoad) - (pPtr + 8)) > 0)
	{
		printf("Part 2: From %X to %X, sz = %d\n", (int)(pPtr - pBuf) + 8, nFileSzToLoad - 1, (int)((pBuf + nFileSzToLoad) - (pPtr + 8)));
		nSum += ChecksumCalc(pPtr + 8, (pBuf + nFileSzToLoad) - (pPtr + 8));
	}
*/
