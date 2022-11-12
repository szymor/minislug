// PSD Protector.
// Protection archi basique des PSD.
// Clément '17o2!!' CORDE


#ifdef __LINUX__
//	#include <unistd.h>		// chdir()
	#include <strings.h>
	#include <sys/types.h>
	#include <dirent.h>
	#include <errno.h>
#endif
#ifdef WIN32
	#include <windows.h>
	#include <direct.h>		// _chdir()
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;

enum
{
	e_Mode_Unprotect = 0,
	e_Mode_Protect,
};
struct SPrm
{
	u8	nMode;
	u8	nRecursive;
	char	pFilename[256];
};
struct SPrm	gPrm;

//=============================================================================

enum
{
	e_PSD_Unprotected = 0,
	e_PSD_Protected,
};

// Teste si un fichier (lu en mémoire) est protégé ou pas.
// Out: 1 = fichier protégé / 0 = fichier non protégé.
u32 PSD_IsProtected(u8 *pBuf)
{
	char	pNorm[4] = { '8'^0x80,'B'^0x81,'P'^0x82,'S'^0x83 };		// Avec un xor pour masquer la chaîne dans l'exe.
	char	pProt[4] = { '8'^0x80,'b'^0x81,'p'^0x82,'s'^0x83 };
	u32	i;

	// Enlève le masque.
	for (i = 0; i < 4; i++)
	{
		pNorm[i] ^= 0x80 + i;
		pProt[i] ^= 0x80 + i;
	}
	// Comparaisons.
	if (strncmp((char *)pBuf, pProt, 4) == 0) return (e_PSD_Protected);
	if (strncmp((char *)pBuf, pNorm, 4) == 0) return (e_PSD_Unprotected);

	fprintf(stderr, "PSD_IsProtected(): Unknown file format!\n");
	exit (1);

}

// Protection.
// Out: 1 = Fichier déjà dans l'état souhaité. 0 = Modif effectuée.
u32 PSD_Protect(u8 *pBuf)
{
	if (PSD_IsProtected(pBuf) == gPrm.nMode) return (1);

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

	if (gPrm.nMode == e_Mode_Protect)
	{ // Protection.
		// 4 premiers octets.
		strncpy((char *)pBuf, pProt, 4);
		// Mélange des valeurs H et W.
		nVar8 = *(((u8 *)pBuf) + 16) ^ 0x82;
		*(((u8 *)pBuf) + 16) = *(((u8 *)pBuf) + 21) ^ 0x14;
		*(((u8 *)pBuf) + 21) = nVar8;
		//
		nVar8 = ~*(((u8 *)pBuf) + 17);
		*(((u8 *)pBuf) + 17) = *(((u8 *)pBuf) + 20) ^ 0x28;
		*(((u8 *)pBuf) + 20) = nVar8;

	}
	else
	{ // Déprotection.
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

	return (0);

}

//=============================================================================

#ifdef __LINUX__
// stricmp n'existe pas en Linux : C'est strcasecmp à la place, dans strings.h.
int stricmp(char *pStr1, char *pStr2)
{
	return (strcasecmp(pStr1, pStr2));
}
#endif

// Lecture du fichier.
void FileLoad(char *pFilename)
{
	FILE	*pFile = NULL;
	u8	*pBuf = NULL;
	u32	nSz;

	// Ouverture du fichier.
	pFile = fopen(pFilename, "rb");
	if (pFile == NULL)
	{
		printf("Error opening file(1).\n");
		goto _err1;
	}
	// Taille.
	fseek(pFile, 0, SEEK_END);
	nSz = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	printf("(%d bytes) ", (int)nSz);
	// Malloc.
	if ((pBuf = (u8 *)malloc(nSz)) == NULL)
	{
		printf("Error Allocating memory.\n");
		goto _err1;
	}
	// Lecture.
	fread(pBuf, 1, nSz, pFile);
	fclose(pFile); pFile = NULL;

	// Protection/déprotection du fichier.
	u32	nWrite = 0;
	if (PSD_Protect(pBuf) == 0)
	{
		nWrite = 1;
	}
	else
	{
		printf("File already %sprotected.", (gPrm.nMode == e_Mode_Protect ? "" : "un"));
	}

	// Sauvegarde du fichier.
	if (nWrite == 1)
	{
		// Ouverture du fichier.
		pFile = fopen(pFilename, "wb");
		if (pFile == NULL)
		{
			printf("Error opening file(2).\n");
			goto _err1;
		}
		// Ecriture.
		fwrite(pBuf, 1, nSz, pFile);
		fclose(pFile); pFile = NULL;

		printf("> File %sprotected.", (gPrm.nMode == e_Mode_Protect ? "" : "un"));
	}

	free(pBuf); pBuf = NULL;
	return;

_err1:
	if (pFile != NULL) fclose(pFile);
	if (pBuf != NULL) free(pBuf);
}

// Parcours des fichiers et répertoires.
u32 FilesSearch(char *pNewDir)
{
	u32	nFilesNb = 0;
	char	pCurDir[256];
	char	*pExt = ".psd";

	strcpy(pCurDir, pNewDir);
	strcat(pCurDir, "/");

#ifdef __LINUX__
	DIR	*dip;
	struct dirent	*dit;
	int	nDirPos;

	// Passe 1 sur les fichiers.

	// Ouverture du répertoire en cours.
	if ((dip = opendir(pCurDir)) == NULL)
	{
		fprintf(stderr, "FilesSearch(): 'opendir' error.\n");
		exit(0);
	}
	// Boucle dans les entrées.
	nDirPos = strlen(pCurDir);
	while ((dit = readdir(dip)) != NULL)
	{
		// Est-ce un fichier normal ?
		if (dit->d_type != DT_REG) continue;
		// C'est un ".psd" ?
		if (strlen(dit->d_name) > strlen(pExt))
		if (stricmp(&dit->d_name[strlen(dit->d_name) - strlen(pExt)], pExt) == 0)
		{
			strcpy(&pCurDir[nDirPos], dit->d_name);
			printf("%s ", pCurDir);
			FileLoad(pCurDir);
			nFilesNb++;
			printf("\n");
		}
	}
	pCurDir[nDirPos] = 0;
	// Fermeture du répertoire.
	if (closedir(dip) == -1)
	{
		fprintf(stderr, "FilesSearch(): 'closedir' error.\n");
		exit(0);
	}

	// Passe 2 sur les répertoires.
	if (gPrm.nRecursive == 0) return (nFilesNb);

	// Ouverture du répertoire en cours.
	if ((dip = opendir(pCurDir)) == NULL)
	{
		fprintf(stderr, "FilesSearch(): 'opendir' error.\n");
		exit(0);
	}
	// Boucle dans les entrées.
	while ((dit = readdir(dip)) != NULL)
	{
		// Est-ce un répertoire ?
		if (dit->d_type != DT_DIR) continue;

		if (strcmp(dit->d_name, ".") == 0 || strcmp(dit->d_name, "..") == 0) continue;
		strcpy(&pCurDir[nDirPos], dit->d_name);
		nFilesNb += FilesSearch(pCurDir);
	}
	// Fermeture du répertoire.
	if (closedir(dip) == -1)
	{
		fprintf(stderr, "FilesSearch(): 'closedir' error.\n");
		exit(0);
	}
#endif

#ifdef WIN32
	WIN32_FIND_DATA ffd;
	HANDLE hFind;

	// Passe 1 sur les fichiers.
	hFind = FindFirstFile(gPrm.pFilename, &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND) goto _dir;
		// Sinon erreur.
		printf("Error opening file...\n");
		exit(1);
	}

	do
	{
		// Répertoire ? => On skippe.
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;

		// Fichier.
		// C'est un ".psd" ?
		if (strlen(ffd.cFileName) > strlen(pExt))
		if (stricmp(&ffd.cFileName[strlen(ffd.cFileName) - strlen(pExt)], pExt) == 0)
		{
			printf("%s%s ", pCurDir, ffd.cFileName);
			FileLoad(ffd.cFileName);
			nFilesNb++;
			printf("\n");
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);

	// Passe 2 sur les répertoires.
_dir:
	if (gPrm.nRecursive == 0) return (nFilesNb);

	hFind = FindFirstFile("*", &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("Error opening file...\n");
		exit(1);
	}

	do
	{
		// Répertoire ?
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strcmp(ffd.cFileName, ".") == 0 || strcmp(ffd.cFileName, "..") == 0) continue;
			_chdir(ffd.cFileName);
			int	nLen = strlen(pCurDir);
			strcat(pCurDir, ffd.cFileName);
			nFilesNb += FilesSearch(pCurDir);
			pCurDir[nLen] = 0;
			_chdir("..");
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);
#endif

	return (nFilesNb);
}

// Main.
int main(int argc, char **argv)
{
	u32	nTotalFiles = 0;
	u32	i;
	u32	nModeSel = 0;

	printf("PSD Basic Protector v.0.2b\n");

	// RAZ params.
	gPrm.nMode = 0;
	gPrm.nRecursive = 0;
	strcpy(gPrm.pFilename, "*");	// Par défaut.

	// Aide si pas de paramètres.
	if (argc < 2)
	{
		printf("\nUsage: PSDProt <-p|-u> [-r]\n");
		printf("Switches:\n");
		printf("\t-p : Protect files.\n");
		printf("\t-u : Unprotect files.\n");
		printf("\t-r : Recursive.\n");
		return (0);
	}

	// Récupération des paramètres sur la ligne de commande.
	for (i = 1; i < argc; i++)
	{
		if (*(argv[i]) == '-')
		{
			switch (*(argv[i] + 1))
			{
			case 'p':
			case 'P':
				gPrm.nMode = e_Mode_Protect;
				nModeSel = 1;
				break;
			case 'u':
			case 'U':
				gPrm.nMode = e_Mode_Unprotect;
				nModeSel = 1;
				break;
			case 'r':
			case 'R':
				gPrm.nRecursive = 1;
				break;
			default:
				printf("'%s': Unrecognized switch encountered, ignored.\n", argv[i]);
				break;
			}
		}
		else
		{
			// Filename.
			strcpy(gPrm.pFilename, argv[i]);
//			printf("fn='%s'\n", gPrm.pFilename);	// Attention aux paramètres : * et "*" ont un comportement très différent !
		}
	}
	// Mode choisi ?
	if (nModeSel == 0)
	{
		printf("Protect/Unprotect flag not specified. Aborted.\n");
		return (1);
	}
	printf("%srotect mode%s.\n", (gPrm.nMode == e_Mode_Protect ? "P" : "Unp"), (gPrm.nRecursive ? ", recursive" : "") );

	// Traitement des fichiers.
	nTotalFiles = FilesSearch(".");
	printf("%d file(s) listed.\n", (int)nTotalFiles);
    return (0);

}


