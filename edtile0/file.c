
#include "includes.h"

#define	e_FlgFile_Plane_Codes	(1 << 31)		// Lecture/sauvegarde d'une page de code.

struct SFile	gFile;

/*
// Constructeur.
CFile::CFile(void)
{

}

// Destructeur.
CFile::~CFile()
{

}
*/


// Recopie un nom de fichier.
void File_FilenameSet(struct SFile *psFile, char *pStr)
{
	strcpy(psFile->pLevFilename, pStr);
}

// Renvoie le nom du fichier en cours.
char * File_FilenameGet(struct SFile *psFile)
{
	return (psFile->pLevFilename);
}



// Pragma pack 1 sur les structures à lire/écrire.
#pragma pack(1)

// Header de chaque container.
struct SContainer0
{
	u32	nSz;
	u16	nType;
	u16	nPadding;
};

// Struct pour datas de la map.
struct SMap0
{
	u32	nMapLg, nMapHt, nMapPosX, nMapPosY, nPlanesNb, nFirstWnd, nActivePlane;
};

// Struct pour datas d'un plan.
/*
struct SPlane0
{
	u32	nPlaneNo, nFlags, nWndPosX, nWndPosY;
};
*/
/*
struct SPlane1
{
	u32	nPlaneNo, nFlags, nWndPosX, nWndPosY;
	u8	nHasCmd;				// Sera surtout utile à la lecture par le jeu, pour skipper le plan et récupérer les monstres dans la section suivante.
	u8	nPad0, nPad1, nPad2;	// Padding.
};
*/
struct SPlane2
{
	u32	nPlaneNo, nFlags, nWndPosX, nWndPosY;
	u32	nBlkLg, nBlkHt;			// Largeur et hauteur du plan en blocs 16. Pour éventuel remap à la relecture.
	u8	nHasCmd;				// Sera surtout utile à la lecture par le jeu, pour skipper le plan et récupérer les monstres dans la section suivante.
	u8	nPad0, nPad1, nPad2;	// Padding.
};

// Struct pour datas misc. (Hum, corrigée et paddée correctement pour pack).
struct SMisc0
{
	u32	nFlags;						// gMisc.nFlags.
	u8	nTransR, nTransG, nTransB;	// RGB de la couleur de transparence.
	u8	nVideoIdx;					// Index de la résolution en cours.
	u16	nVisuLg, nVisuHt;			// Taille de la visu (en pixels).

	u32	nBlank0, nBlank1, nBlank2, nBlank3, nChecksum;	// Au cas où on ait besoin de sauver plus de trucs par la suite.
};

#pragma pack()

#define	FILE_CHKSUM	1
u32 ChecksumGetFromFile(char *pFilename);

// Lecture d'un fichier EdTile.
void File_Load(struct SFile *psFile)
{
	FILE	*fPt;
	u8	*pBuf;	// Pour lire les containers.
	u8	*pCur;
	char	*pBufTxt;
	u32	nSz, nDate;
	u32	j;
	u32	nType;
	u32	nMapSection = 0;	// Flag, on doit trouver une section map avant de trouver des plans.
	u32	nPlaneNext = 0;		// Pour controler l'ordre de lecture des plans. Doit être dans l'ordre.
	char	pFilename[256];
	struct SPlane	*pPlane;
	struct SMap0	sMap;

	struct SMap	*psMap = gMisc.psMap;

	sMap.nPlanesNb = sMap.nFirstWnd = sMap.nActivePlane = 0;	// Pour supprimer le warning à la compilation.

#ifdef FILE_CHKSUM
	// Test d'intégrité : Calcul du checksum du fichier qu'on va lire.
	u32 nChecksum0 = ChecksumGetFromFile(psFile->pLevFilename);
#endif

	// Malloc du buffer de lecture.
	if ((pBuf = (u8 *)malloc(1000000)) == NULL)
	{
		fprintf(stderr, "File_Load(): malloc failed (pBuf).\n");
		exit(1);
	}

	// Ouverture du fichier.
	if ((fPt = fopen(psFile->pLevFilename, "rb")) == NULL)
	{
		fprintf(stderr, "File_Load(): Error opening file.\n");
		exit(1);
	}

#ifdef FILE_CHKSUM
	// Récupération du checksum sauvegardé : On se place à la fin, et on recule d'un u32.
	u32 nChecksum1 = 0;
	if (fseek(fPt, -sizeof(u32), SEEK_END) == 0)
		fread(&nChecksum1, 1, sizeof(u32), fPt);
	else
		fprintf(stderr, "File_Load(): Checksum: fseek errror.\n");

	fseek(fPt, 0L, SEEK_SET);
	// Ok ?
	if (nChecksum0 != nChecksum1)
	{
		fprintf(stderr, "File_Load(): Checksum error: Loaded = %08X / Calculated = %08X. Attempting to read file anyway.\n", nChecksum1, nChecksum0);

		static	char	*pMsgTitle = "Load error";
		char	pMsgStr[256];
	#ifdef WIN32
		sprintf(pMsgStr, "Checksum error:\nLoaded = %08X / Calculated = %08X.", nChecksum1, nChecksum0);
		MessageBox(NULL, pMsgStr, pMsgTitle, MB_OK);
	#endif
	#ifdef __LINUX__
// !!! A tester !!!
		// Avec xmessage.
		sprintf(pMsgStr, "xmessage -center \"*** %s ***\n\nChecksum error:\nLoaded = %08X / Calculated = %08X.\" -button OK", pMsgTitle, nChecksum1, nChecksum0);
		system(pMsgStr);
	#endif

	}
#endif

	// En-tête.
	fread(pBuf, 1, 5, fPt);
	// Vérif "EDT".
	if (strncmp((char *)pBuf, "EDT", 3) != 0)
	{
		fprintf(stderr, "CFile::Load(): Wrong header.\n");
		exit(1);
	}
	// Vérif version du fichier.
	if (*(u16 *)(pBuf + 3) != 0x0100)
	{
		fprintf(stderr, "CFile::Load(): Unknown file version.\n");
		exit(1);
	}

	// Lecture des containers.
	while (1)
	{
		// Lecture du container.
		nSz = fread(pBuf, 1, sizeof(struct SContainer0), fPt);
		if (nSz != sizeof(struct SContainer0)) break;

		nSz = ((struct SContainer0 *)pBuf)->nSz;
		nType = ((struct SContainer0 *)pBuf)->nType;

		// Lecture des données.
		nSz -= sizeof(struct SContainer0);
		fread(pBuf, 1, nSz, fPt);

		switch (nType)
		{
		case e_FileSection_Map:
//			printf("Map section\n");
			if (nMapSection)
			{
				printf("File_Load(): Duplicate map section found! Section ignored.\n");
				break;
			}
//			printf("lg=%d / ht=%d / posx=%d / posy=%d / nbpln=%d / firstwnd=%d / activepln=%d\n",
//			(int)((SMap0 *)pBuf)->nMapLg, (int)((SMap0 *)pBuf)->nMapHt,
//			(int)((SMap0 *)pBuf)->nMapPosX, (int)((SMap0 *)pBuf)->nMapPosY,
//			(int)((SMap0 *)pBuf)->nPlanesNb, (int)((SMap0 *)pBuf)->nFirstWnd, (int)((SMap0 *)pBuf)->nActivePlane);

			psMap->nMapLg = ((struct SMap0 *)pBuf)->nMapLg;
			psMap->nMapHt = ((struct SMap0 *)pBuf)->nMapHt;
			psMap->nMapPosX = ((struct SMap0 *)pBuf)->nMapPosX;
			psMap->nMapPosY = ((struct SMap0 *)pBuf)->nMapPosY;
			// Pour plus tard.
			sMap.nPlanesNb = ((struct SMap0 *)pBuf)->nPlanesNb;
			sMap.nFirstWnd = ((struct SMap0 *)pBuf)->nFirstWnd;
			sMap.nActivePlane = ((struct SMap0 *)pBuf)->nActivePlane;

			nMapSection = 1;	// Flag.
			break;

		case e_FileSection_Plane:
//			printf("Plane section\n");
			if (nMapSection == 0)
			{
				printf("File_Load(): Plane section found prior to map section! Section ignored.\n");
				break;
			}
//			printf("plnno=%d / flags=%d / wposx=%d / wposy=%d\n",
//			(int)((SPlane2 *)pBuf)->nPlaneNo, (int)((SPlane2 *)pBuf)->nFlags,
//			(int)((SPlane2 *)pBuf)->nWndPosX, (int)((SPlane2 *)pBuf)->nWndPosY);
			if (((struct SPlane2 *)pBuf)->nPlaneNo != nPlaneNext)
			{
				printf("File_Load(): Plane section: Wrong plane number found. Section ignored.\n");
				break;
			}

			pCur = pBuf + sizeof(struct SPlane2);

			// Gfx filename.
			j = *pCur;
			pCur++;
			strncpy(pFilename, (char *)pCur, j);
			pFilename[j] = 0;
			pCur += j;
//			printf("fn=%s\n", pFilename);

			// Ajout du plan.
			Map_PlaneAdd(psMap, pFilename);

			// Misc datas.
			pPlane = *(psMap->ppsPlanes + ((struct SPlane2 *)pBuf)->nPlaneNo);
			pPlane->nPlaneNo = ((struct SPlane2 *)pBuf)->nPlaneNo;
			pPlane->nFlags = ((struct SPlane2 *)pBuf)->nFlags;
			pPlane->nWndPosX = ((struct SPlane2 *)pBuf)->nWndPosX;
			pPlane->nWndPosY = ((struct SPlane2 *)pBuf)->nWndPosY;
			//((SPlane2 *)pBuf)->nWndPosY->nHasCmd
//printf("plane no %d : lg = %d / ht = %d\n", pPlane->nPlaneNo, ((SPlane2 *)pBuf)->nBlkLg, ((SPlane2 *)pBuf)->nBlkHt);

			// Doit on relire des codes ?
			if (pPlane->nFlags & e_FlgFile_Plane_Codes)
			{
				// Attention, malloc avec l'ANCIENNE taille de la planche !
//-				pPlane->CodesPlaneAdd(((SPlane2 *)pBuf)->nBlkLg, ((SPlane2 *)pBuf)->nBlkHt);	// Si ça rate, exit.
				Plane_CodesPlaneAdd(pPlane, ((struct SPlane2 *)pBuf)->nBlkLg, ((struct SPlane2 *)pBuf)->nBlkHt);	// Si ça rate, exit.
				memcpy(pPlane->pCodes, pCur, ((struct SPlane2 *)pBuf)->nBlkLg * ((struct SPlane2 *)pBuf)->nBlkHt);
				pCur += ((struct SPlane2 *)pBuf)->nBlkLg * ((struct SPlane2 *)pBuf)->nBlkHt * sizeof(u8);
				pPlane->nFlags &= ~e_FlgFile_Plane_Codes;
			}

			// Le plan.
			for (j = 0; j < psMap->nMapLg * psMap->nMapHt; j++)
			{
				*(pPlane->pPlane + j) = *(s32 *)pCur;	// < On pourrait tester les nos de blocs ici.
				pCur += sizeof(s32);
			}

			// La taille de la planche a changé ?
			if ((u32)pPlane->pGfx->w / 16 != ((struct SPlane2 *)pBuf)->nBlkLg ||
				(u32)pPlane->pGfx->h / 16 != ((struct SPlane2 *)pBuf)->nBlkHt)
			{
printf("Remaping... Plane %d: old sz %d %d > new sz %d %d\n", (int)pPlane->nPlaneNo, (int)((struct SPlane2 *)pBuf)->nBlkLg, (int)((struct SPlane2 *)pBuf)->nBlkHt, (int)pPlane->pGfx->w/16, (int)pPlane->pGfx->h / 16);
				u32	nOrgLg, nNewLg, nNewHt;
				s32	nBlkNo;
				u32	nBlkX, nBlkY;

				nOrgLg = ((struct SPlane2 *)pBuf)->nBlkLg;
				nNewLg = (u32)pPlane->pGfx->w / 16;
				nNewHt = (u32)pPlane->pGfx->h / 16;

				for (j = 0; j < psMap->nMapLg * psMap->nMapHt; j++)
				{
					nBlkNo = *(pPlane->pPlane + j);
					if (nBlkNo >= 0)
					{
/* v1
// Version simplette. Après le remap il y a un contrôle des n° de blocs, donc si ça déborde, pas de pb.
// Mais si nBlkX est plus grand que nNewLg, on place un code faux à coup sûr.
						nBlkY = nBlkNo / nOrgLg;
						nBlkX = nBlkNo - (nBlkY * nOrgLg);
						nBlkNo = (nBlkY * nNewLg) + nBlkX;
						*(pPlane->pPlane + j) = nBlkNo;
*/
						nBlkY = nBlkNo / nOrgLg;
						nBlkX = nBlkNo - (nBlkY * nOrgLg);
						nBlkNo = (nBlkX < nNewLg && nBlkY < nNewHt ? (nBlkY * nNewLg) + nBlkX : -1);
						*(pPlane->pPlane + j) = nBlkNo;

/*
// Cette ligne fait un warning sur c::b 8.02 :
						nBlkNo = (nBlkX < nNewLg && nBlkY < nNewHt ? (nBlkY * nNewLg) + nBlkX : -1);
warning: converting of negative value `-0x000000001' to `unsigned int'

// Et pas de warning avec ça :
						if (nBlkX < nNewLg && nBlkY < nNewHt)
							nBlkNo =  (nBlkY * nNewLg) + nBlkX;
						else
							nBlkNo = -1;
*/

					}
				}

//-				if (pPlane->HasCodes())
				if (Plane_HasCodes(pPlane))
				{
printf("Remaping blocks codes...\n");
					u8	*pNewCodes;
					u32	nOrgHt, nNewHt;
					u32	nLgMini, nHtMini;
					u32	i;

					nOrgHt = ((struct SPlane2 *)pBuf)->nBlkHt;
					nNewHt = (u32)pPlane->pGfx->h / 16;

					// malloc.
					if ((pNewCodes = (u8 *)malloc(nNewLg * nNewHt)) == NULL)
					{
						fprintf(stderr, "File_Load(): malloc failed (pNewCodes).\n");
						exit(1);
					}
					// RAZ mémoire.
					memset(pNewCodes, 0, nNewLg * nNewHt);
					// Remap.
					nLgMini = (nOrgLg < nNewLg ? nOrgLg : nNewLg);
					nHtMini = (nOrgHt < nNewHt ? nOrgHt : nNewHt);
//printf("org %d %d / new %d %d / mini %d %d\n", nOrgLg, nOrgHt, nNewLg, nNewHt, nLgMini, nHtMini);

					for (j = 0; j < nHtMini; j++)
					for (i = 0; i < nLgMini; i++)
					{
						*(pNewCodes + (j * nNewLg) + i) = *(pPlane->pCodes + (j * nOrgLg) + i);
					}

					// Switch pointeurs.
					free(pPlane->pCodes);
					pPlane->pCodes = pNewCodes;
				}

				// Enfin, si l'origine de la position de la fenêtre d'affichage se retrouve complètement en dehors, RAZ.
				if (pPlane->nWndPosX >= (u32)pPlane->pGfx->w / 16 ||
					pPlane->nWndPosY >= (u32)pPlane->pGfx->h / 16)
				{
					pPlane->nWndPosX = 0;
					pPlane->nWndPosY = 0;
				}

			} // if remap

			// Un dernier petit contrôle sur les nos de blocs lus...
			{
				s32	nBlkMax = (((u32)pPlane->pGfx->w / 16) * ((u32)pPlane->pGfx->h / 16)) - 1;	// Bloc max sur la nouvelle planche.
				for (j = 0; j < psMap->nMapLg * psMap->nMapHt; j++)
				{
					if (*(pPlane->pPlane + j) > nBlkMax) *(pPlane->pPlane + j) = -1;
				}
			}

			// Le prochain qui arrivera devrait être le...
			nPlaneNext++;
			break;

		case e_FileSection_Cmd:		// On rajoute le fichier de commandes au dernier plan ajouté.
//			printf("Cmd section\n");
			if (nMapSection == 0)
			{
				printf("Cmd section found prior to map section! Section ignored.\n");
				break;
			}

			// u8 : Nb de bytes du nom de fichier.
			// char * x : Nom de fichier, sans 0 terminal.
			// u32 : Date du fichier.
			// u32 : Nb de bytes dans le fichier de commandes.
			// char * x : Le fichier de commandes, sans 0 terminal.
			pCur = pBuf;

			j = *pCur;
			pCur++;
			strncpy(pFilename, (char *)pCur, j);
			pFilename[j] = 0;
			pCur += j;
//			printf("cmd fn=%s\n", pFilename);

			nDate = *(u32 *)pCur;
			pCur += 4;
//			printf("cmd date=%d\n", (int)nDate);

			j = *(u32 *)pCur;
			pCur += 4;
			//printf("sz=%d\n",(int)j);
			// Malloc du buffer de lecture.
			if ((pBufTxt = (char *)malloc(j + 1)) == NULL)
			{
				printf("Load/cmd: malloc failed.\n");
				break;
			}
			strncpy(pBufTxt, (char *)pCur, j);
			pBufTxt[j] = 0;
			pCur += j;
			//printf("cmd file:\n%s\n", pBufTxt);
			// Ajoute le fichier de commandes au dernier plan.
			Map_PlaneCmdfileAddFromBuf(psMap, psMap->nPlanesNb - 1, pFilename, pBufTxt, nDate);
			free(pBufTxt);

			// u32 : Nb de monstres.
			// x : Datas des monstres, les uns à la suite des autres : SMst0 + x bytes de data.
			j = *(u32 *)pCur;
			pCur += 4;
//			printf("nb mst=%d\n",(int)j);

			pPlane = *(psMap->ppsPlanes + (psMap->nPlanesNb - 1));
			while(j)
			{
//printf("x=%d y=%d no=%d nb bytes=%d\n",
//(int)((SMst0 *)pCur)->nPosX,(int)((SMst0 *)pCur)->nPosY,
//(int)((SMst0 *)pCur)->nMstNo,(int)((SMst0 *)pCur)->nNbBytes);

				*(pPlane->ppMstPlane + (((struct SMst0 *)pCur)->nPosY * psMap->nMapLg) + ((struct SMst0 *)pCur)->nPosX) = MstUnit_New();
//				(*(pPlane->_ppMstPlane + (((SMst0 *)pCur)->nPosY * psMap->nMapLg) + ((SMst0 *)pCur)->nPosX))->InitFromSavedFile(pCur);
				MstUnit_InitFromSavedFile( *(pPlane->ppMstPlane + (((struct SMst0 *)pCur)->nPosY * psMap->nMapLg) + ((struct SMst0 *)pCur)->nPosX), pCur);

				pCur += ((struct SMst0 *)pCur)->nNbBytes;
				pCur += sizeof(struct SMst0);
				j--;
			}

			// Compare le fichier de commandes chargé et celui sur disque. Et fait ce qu'il faut.
//-			pPlane->CmdFileCompare();
			Plane_CmdFileCompare(pPlane);

			break;

		case e_FileSection_Misc:
//			printf("Misc section\n");

			gMisc.nFlags = ((struct SMisc0 *)pBuf)->nFlags;
			gMisc.nTransR = ((struct SMisc0 *)pBuf)->nTransR;	// Valeur de transparence.
			gMisc.nTransG = ((struct SMisc0 *)pBuf)->nTransG;
			gMisc.nTransB = ((struct SMisc0 *)pBuf)->nTransB;
			// Taille de la visu (en pixels).
			gMisc.nVisuLg = ((struct SMisc0 *)pBuf)->nVisuLg;
			gMisc.nVisuHt = ((struct SMisc0 *)pBuf)->nVisuHt;
			// Index de la résolution en cours.
			gMisc.nVideoIdx = ((struct SMisc0 *)pBuf)->nVideoIdx;

			break;

		default:
			printf("File_Load(): Unknown section type: %d.\n", (int)nType);
			break;
		}

	}

	// Fermeture du fichier.
	fclose(fPt);
	free(pBuf);

	// Fini ! Derniers tests.
	if (psMap->nPlanesNb != sMap.nPlanesNb)
	{
		fprintf(stderr, "File_Load(): Wrong number of planes loaded! Aborted.\n");
		exit(1);
	}

	psMap->nFirstWindow = sMap.nFirstWnd;
	psMap->nActivePlane = sMap.nActivePlane;

}

#define	SAVE_BUF_SZ	(1000000)
// Lecture du fichier EdTile.
void File_Save(struct SFile *psFile)
{
	FILE	*fPt;
	u8	*pBuf;	// Pour créer les données à sauvegarder en mémoire avant de les écrire.
	u8	*pCur, *pCur2;
	u32	nSz;
	u32	i, j, nNbMst;

	struct SMap	*psMap = gMisc.psMap;

	// Malloc du buffer d'écriture.
	if ((pBuf = (u8 *)malloc(SAVE_BUF_SZ)) == NULL)
	{
		fprintf(stderr, "File_Save(): malloc failed (pBuf).\n");
		exit(1);
	}

	// Création du fichier.
	if ((fPt = fopen(psFile->pLevFilename, "wb")) == NULL)
	{
		fprintf(stderr, "File_Save(): Error opening file.\n");
		exit(1);
	}

	// En-tête.
	strcpy((char *)pBuf, "EDT");
	*(u16 *)(pBuf + 3) = 0x0100;	// Version du fichier.
	fwrite(pBuf, 1, 5, fPt);

	//
	// Datas de la map.
	//
	nSz = 0;
	pCur = pBuf + sizeof(struct SContainer0);	// Place un ptr après le header du conteneur (réservation de la place).
	nSz += sizeof(struct SContainer0);

	// Misc datas.
	((struct SMap0 *)pCur)->nMapLg = psMap->nMapLg;
	((struct SMap0 *)pCur)->nMapHt = psMap->nMapHt;
	((struct SMap0 *)pCur)->nMapPosX = psMap->nMapPosX;
	((struct SMap0 *)pCur)->nMapPosY = psMap->nMapPosY;
	((struct SMap0 *)pCur)->nPlanesNb = psMap->nPlanesNb;
	((struct SMap0 *)pCur)->nFirstWnd = psMap->nFirstWindow;
	((struct SMap0 *)pCur)->nActivePlane = psMap->nActivePlane;
	nSz += sizeof(struct SMap0);

	// Update container.
	((struct SContainer0 *)pBuf)->nSz = nSz;
	((struct SContainer0 *)pBuf)->nType = e_FileSection_Map;
	((struct SContainer0 *)pBuf)->nPadding = 0;
	fwrite(pBuf, 1, nSz, fPt);

	//
	// Datas des plans.
	//

	// Boucle sur les plans.
	for (i = 0; i < psMap->nPlanesNb; i++)
	{
		struct SPlane	*pPlane;

		pPlane = *(psMap->ppsPlanes + i);

		nSz = 0;
		pCur = pBuf + sizeof(struct SContainer0);	// Place un ptr après le header du conteneur (réservation de la place).
		nSz += sizeof(struct SContainer0);

		// Misc. datas.
		((struct SPlane2 *)pCur)->nPlaneNo = pPlane->nPlaneNo;
		((struct SPlane2 *)pCur)->nFlags = pPlane->nFlags;
			((struct SPlane2 *)pCur)->nFlags &= ~e_FlgFile_Plane_Codes;									// On force le clear du bit...
			((struct SPlane2 *)pCur)->nFlags |= (Plane_HasCodes(pPlane) ? e_FlgFile_Plane_Codes : 0);	// ...et si nécessaire, un OR.
		((struct SPlane2 *)pCur)->nWndPosX = pPlane->nWndPosX;
		((struct SPlane2 *)pCur)->nWndPosY = pPlane->nWndPosY;
		((struct SPlane2 *)pCur)->nHasCmd = (Plane_HasCmd(pPlane) ? 1 : 0);
		((struct SPlane2 *)pCur)->nBlkLg = pPlane->pGfx->w / 16;
		((struct SPlane2 *)pCur)->nBlkHt = pPlane->pGfx->h / 16;
//printf("sav lg:%d ht%d\n", ((SPlane2 *)pCur)->nBlkLg, ((SPlane2 *)pCur)->nBlkHt);
		pCur += sizeof(struct SPlane2);
		nSz += sizeof(struct SPlane2);

		// Gfx filename.
		j = strlen(pPlane->pPlnFilename);
		*pCur = (u8)j;
		pCur++;
		strcpy((char *)pCur, pPlane->pPlnFilename);
		pCur += j;
		nSz += j + 1;

		// Doit on sauver des codes ?
		if (Plane_HasCodes(pPlane))
		{
			memcpy(pCur, pPlane->pCodes, (pPlane->pGfx->w / 16) * (pPlane->pGfx->h / 16) );
			pCur += (pPlane->pGfx->w / 16) * (pPlane->pGfx->h / 16) * sizeof(u8);
			nSz += (pPlane->pGfx->w / 16) * (pPlane->pGfx->h / 16) * sizeof(u8);
		}

		// Les blocs du plan.
		for (j = 0; j < psMap->nMapLg * psMap->nMapHt; j++)
		{
			*(s32 *)pCur = *(pPlane->pPlane + j);
			pCur += sizeof(s32);
		}
		nSz += psMap->nMapLg * psMap->nMapHt * sizeof(s32);

		// Update container.
		((struct SContainer0 *)pBuf)->nSz = nSz;
		((struct SContainer0 *)pBuf)->nType = e_FileSection_Plane;
		((struct SContainer0 *)pBuf)->nPadding = 0;
		fwrite(pBuf, 1, nSz, fPt);

		//

		// Si le plan a des monstres, une section Cmd suit le plan.
		if (Plane_HasCmd(pPlane))
		{
		nSz = 0;
		pCur = pBuf + sizeof(struct SContainer0);	// Place un ptr après le header du conteneur (réservation de la place).
		nSz += sizeof(struct SContainer0);

		struct SCmdfile	*pMst = pPlane->pMst;

		// Cmd filename.
		j = strlen(pMst->pCmdFilename);
		*pCur = (u8)j;
		pCur++;
		strcpy((char *)pCur, pMst->pCmdFilename);
		pCur += j;
		nSz += j + 1;

		// Date du fichier.
		*(u32 *)pCur = pMst->nFileDate;
		pCur += 4;
		nSz += 4;

		// Cmd file.
		j = strlen(pMst->pCmdfile);
		*(u32 *)pCur = j;
		pCur += 4;
		strcpy((char *)pCur, pMst->pCmdfile);
		pCur += j;
		nSz += j + 4;

		// Nb de monstres.
		nNbMst = 0;
		pCur2 = pCur;		// On sauvegarde un ptr sur le u32.
		pCur += 4;
		nSz += 4;

		// Les monstres.
		u32	mx, my;
		s32	nBlk;
		struct SMstUnit	*pMstCur;
		for (my = 0; my < psMap->nMapHt; my++)
		{
			for (mx = 0; mx < psMap->nMapLg; mx++)
			{
				// Normalement, il n'y a des monstres que sous des blocs utilisés.
				nBlk = *(pPlane->pPlane + (psMap->nMapLg * my) + mx);
				if (nBlk != -1)
				{
					pMstCur = *(pPlane->ppMstPlane + (psMap->nMapLg * my) + mx);
					if (pMstCur != NULL)
					{
						((struct SMst0 *)pCur)->nPosX = mx;
						((struct SMst0 *)pCur)->nPosY = my;
						((struct SMst0 *)pCur)->nMstNo = pMstCur->nMstNo;
						((struct SMst0 *)pCur)->nNbBytes = pMstCur->nNbBytes;
						pCur += sizeof(struct SMst0);
						nSz += sizeof(struct SMst0);
						memcpy(pCur, pMstCur->pData, pMstCur->nNbBytes);
						pCur += pMstCur->nNbBytes;
						nSz += pMstCur->nNbBytes;
						// Nb monstres ++
						nNbMst++;
					}
				}
			}
		}
		*(u32 *)pCur2 = nNbMst;	// Sauvegarde le nb de monstres.

		// Update container.
		((struct SContainer0 *)pBuf)->nSz = nSz;
		((struct SContainer0 *)pBuf)->nType = e_FileSection_Cmd;
		((struct SContainer0 *)pBuf)->nPadding = 0;
		fwrite(pBuf, 1, nSz, fPt);
		}

	}

	//
	// Miscellaneous data.
	//
	nSz = 0;
	pCur = pBuf + sizeof(struct SContainer0);	// Place un ptr après le header du conteneur (réservation de la place).
	nSz += sizeof(struct SContainer0);

	memset(pCur, 0, sizeof(struct SMisc0));	// Mise à 0 de la structure.
	// Misc datas.
	((struct SMisc0 *)pCur)->nFlags = gMisc.nFlags;
	((struct SMisc0 *)pCur)->nTransR = gMisc.nTransR;		// Sauvegarde des valeurs de transparence.
	((struct SMisc0 *)pCur)->nTransG = gMisc.nTransG;
	((struct SMisc0 *)pCur)->nTransB = gMisc.nTransB;
	// Taille de la visu (en pixels).
	((struct SMisc0 *)pCur)->nVisuLg = (psMap->psVisu == NULL ? VISU_DEFAULT_Width : psMap->psVisu->nDisplayWidth);
	((struct SMisc0 *)pCur)->nVisuHt = (psMap->psVisu == NULL ? VISU_DEFAULT_Height : psMap->psVisu->nDisplayHeight);
	// Index de la résolution en cours.
	((struct SMisc0 *)pCur)->nVideoIdx = gMisc.nVideoIdx;
	nSz += sizeof(struct SMisc0);

	// Update container.
	((struct SContainer0 *)pBuf)->nSz = nSz;
	((struct SContainer0 *)pBuf)->nType = e_FileSection_Misc;
	((struct SContainer0 *)pBuf)->nPadding = 0;
	fwrite(pBuf, 1, nSz, fPt);

	// Fermeture du fichier.
	fclose(fPt);
	free(pBuf);

	psMap->nModified = 0;	// RAZ flag modified.


#ifdef FILE_CHKSUM
	// Checksum. Pas génial, on relit le fichier, on calcule, on sauve.
	u32	nSum;
	nSum = ChecksumGetFromFile(psFile->pLevFilename);
	// Ouverture du fichier.
	if ((fPt = fopen(psFile->pLevFilename, "rb+")) == NULL)
	{
		fprintf(stderr, "File_Save(): Checksum: Error opening file '%s'.\n", psFile->pLevFilename);
		return;
	}
	// On se place à la fin, et on recule d'un u32.
	if (fseek(fPt, -sizeof(u32), SEEK_END) == 0)
	{
		fwrite(&nSum, 1, sizeof(u32), fPt);
printf("Checksum = %08X\n", nSum);
	}
	else
	{
		fprintf(stderr, "File_Save(): Checksum: fseek errror.\n");
	}
	fclose (fPt);
#endif

}

#ifdef FILE_CHKSUM
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

// Lecture du fichier, calcul et renvoi du checksum.
u32 ChecksumGetFromFile(char *pFilename)
{
	FILE	*fPt = NULL;
	u8	*pBuf = NULL;

	// Ouverture du fichier.
	if ((fPt = fopen(pFilename, "rb")) == NULL)
	{
		fprintf(stderr, "ChecksumGetFromFile(): Error opening file '%s'.\n", pFilename);
		goto _err_exit1;
	}
	// Récupération de la taille du fichier.
	s32	nFileSzToLoad, nSz;
	fseek(fPt, 0L, SEEK_END);
	nFileSzToLoad = ftell(fPt);
	fseek(fPt, 0L, SEEK_SET);
//printf("fsize = %d\n", nFileSzToLoad);
	// Malloc du buffer de lecture.
	if ((pBuf = (u8 *)malloc(nFileSzToLoad)) == NULL)
	{
		fprintf(stderr, "ChecksumGetFromFile(): malloc failed (pBuf).\n");
		goto _err_exit1;
	}
	// Lecture.
	nSz = fread(pBuf, 1, nFileSzToLoad, fPt);
	if (nSz != nFileSzToLoad)
	{
		fprintf(stderr, "ChecksumGetFromFile(): fread error.\n");
		goto _err_exit1;
	}
	fclose(fPt); fPt = NULL;

	// Calcul du Checksum.
	u32	nSum;
	nSum = ChecksumCalc(pBuf, nFileSzToLoad - sizeof(u32));
//printf("Checksum = %08X\n", nSum);
	free(pBuf); pBuf = NULL;
	return (nSum);

_err_exit1:
	if (fPt != NULL) fclose(fPt);
	if (pBuf != NULL) free(pBuf);
	exit (1);
}
#endif



// fexist.
// Note : On pourrait aussi faire ça avec un stat() !
bool fexist(char *pFilename)
{
	bool	bRet = false;
	FILE	*fPt;
	if ((fPt = fopen(pFilename, "rb")) != NULL)
	{
		fclose(fPt);
		bRet = true;
	}
	return (bRet);
}


