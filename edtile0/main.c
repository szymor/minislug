
//
// Editeur de niveaux / Map editing tool.
// Code : Clément "17o2!!" CORDE
// email : c1702@yahoo.com
//

#include "includes.h"

//=============================================================================

struct SMisc	gMisc;

void Misc_VideoResGet(struct SMisc *psMisc);

void Misc_Init(struct SMisc *psMisc)
{
	psMisc->nFlags = 0;
	psMisc->nUpdateDisp = 0;
	psMisc->nTransR = psMisc->nTransG = psMisc->nTransB = 0;

	psMisc->pScreen = NULL;
	psMisc->nVisuLg = VISU_DEFAULT_Width;		// Taille par défaut de la visu.
	psMisc->nVisuHt = VISU_DEFAULT_Height;
	psMisc->nVideoIdx = 0;
	Misc_VideoResGet(psMisc);		// MAJ nScrW et nScrH.

	psMisc->psKbd = NULL;
	psMisc->psInfoBox = NULL;
	psMisc->psMouse = NULL;

	psMisc->psMap = NULL;
	psMisc->psUndo = NULL;

}

void Misc_FullScreenToggle(struct SMisc *psMisc)
{
	psMisc->nFlags ^= e_Flg_FullScreen;
}

u32 Misc_FullScreenGet(struct SMisc *psMisc)
{
	return (psMisc->nFlags & e_Flg_FullScreen);
}

void Misc_TransparencyToggle(struct SMisc *psMisc)
{
	psMisc->nFlags ^= e_Flg_Transparence;
}

u32 Misc_TransparencyGet(struct SMisc *psMisc)
{
	return (psMisc->nFlags & e_Flg_Transparence);
}


struct SRes
{
	u32	w, h;
};
struct SRes	gpsRes[] =
{
	{ 800, 600 },
	{ 1024, 768 },
};

// Met à jour les champs nScrW et nScrH.
void Misc_VideoResGet(struct SMisc *psMisc)
{
	psMisc->nScrW = gpsRes[psMisc->nVideoIdx].w;
	psMisc->nScrH = gpsRes[psMisc->nVideoIdx].h;
}

// Set video mode.
// IN : nInc = 0, on reste à la résolution en cours (init) / 1 = on passe à la suivante (cycle).
void Misc_SetVideoMode(struct SMisc *psMisc, u32 nInc)
{

	// Passage au suivant ?
	if (nInc)
	if (++psMisc->nVideoIdx >= NBELEM(gpsRes)) psMisc->nVideoIdx = 0;

	// Taille de l'écran.
	Misc_VideoResGet(psMisc);

	// SetVideoMode.
	psMisc->pScreen = SDL_SetVideoMode(psMisc->nScrW, psMisc->nScrH, 16, SDL_SWSURFACE);
    if (psMisc->pScreen == NULL)
    {
		fprintf(stderr, "Unable to set up video: %s\n", SDL_GetError());
		exit(1);
    }

	// Reinit visu.
	if (psMisc->psMap->psVisu != NULL)
	{
		if (Map_VisuOnOff(psMisc->psMap)) Map_VisuToggle(psMisc->psMap);	// Coupe la visu si nécessaire.
		Visu_Delete(psMisc->psMap->psVisu);
	}
	psMisc->psMap->psVisu = Visu_New(psMisc->nVisuLg, psMisc->nVisuHt);		// On recréé la visu dans tous les cas.

}

//=============================================================================

#define FPS_Default (1000 / 60)
u32 gnTimer1;

// Init timers.
void FrameInit(void)
{
	gnTimer1 = SDL_GetTicks();
}

// Attente de la frame.
void FrameWait(void)
{
/*
	u32 nTimer2;

	SDL_Delay(3);	// En plus, pour donner de force un peu de temps au système.
	// S'assurer qu'on ne va pas trop vite...
	while (1)
	{
		nTimer2 = SDL_GetTicks() - gnTimer1;
		if (nTimer2 >= FPS_Default) break;
		SDL_Delay(3);
	}
	gnTimer1 = SDL_GetTicks();
*/

	u32 nTimer2;

	nTimer2 = SDL_GetTicks() - gnTimer1;
	if (nTimer2 < FPS_Default)
		SDL_Delay(FPS_Default - nTimer2);
	gnTimer1 = SDL_GetTicks();

}

//=============================================================================

// Boîte de message Yes/No/Cancel.
enum
{
	e_MsgBoxCancel = -1,
	e_MsgBoxYes = 0,
	e_MsgBoxNo = 1,
};
s32 MyMsgBoxYesNo(const char *pStr, const char *pTitle)
{
	s32	nRetVal = e_MsgBoxCancel;
	int	nRet;
#ifdef WIN32
	nRet = MessageBox(NULL, pStr, pTitle, MB_YESNOCANCEL | MB_ICONSTOP);
	switch (nRet)
	{
	case IDYES:
		nRetVal = e_MsgBoxYes;
		break;
	case IDNO:
		nRetVal = e_MsgBoxNo;
		break;
	default:
		break;
	}
#endif
#ifdef __LINUX__
	static	const char	*pCmdStr = "xmessage -center \"*** %s ***\n\n%s\" -button Yes,No,Cancel";
	char *pCmdLn;

	if ((pCmdLn = (char *)malloc(strlen(pCmdStr) + strlen(pStr) + strlen(pTitle))) != NULL)
	{
		sprintf(pCmdLn, pCmdStr, pTitle, pStr);
		if ((nRet = system(pCmdLn)) != -1)
		{
			nRet = WEXITSTATUS(nRet);
			switch (nRet)
			{
			case 101:
				nRetVal = e_MsgBoxYes;
				break;
			case 102:
				nRetVal = e_MsgBoxNo;
				break;
			default:
				break;
			}
		}
		free(pCmdLn);
	}
#endif
	return (nRetVal);
}

//=============================================================================

// Gestionnaire d'évènements.
int EventHandler(void)
{
	struct SMouse	*psMouse = gMisc.psMouse;
	struct SInfoBox	*psInfoBox = gMisc.psInfoBox;

	// Message processing loop.
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		// Check for messages.
		switch (event.type)
		{
		// Sortie quand la fenêtre est fermée avec la croix.
		case SDL_QUIT:
			return (1);
			break;

		// Touche enfoncée.
		case SDL_KEYDOWN:
			// Sortie quand on appuie sur 'X'.
			if (event.key.keysym.sym == SDLK_x)
			{
				if (gMisc.psMap->nModified == 0) return (1);
				// Map modifiée, on propose une sauvegarde avant de quitter.
				s32	nRet;
				nRet = MyMsgBoxYesNo("Map has been modified.\nSave changes before leaving?", "Quit");
				switch (nRet)
				{
				case e_MsgBoxCancel:
					break;
				case e_MsgBoxYes:
					File_Save(&gFile);
					return (1);
					break;
				case e_MsgBoxNo:
					return (1);
					break;
				}
			}
			break;

		// Touche relachée.
		case SDL_KEYUP:
			break;

		// Déplacement de la souris.
		case SDL_MOUSEMOTION:
			//printf("x=%d y=%d\n", (int)event.motion.x, (int)event.motion.y);
			{
				s32	nLastBlkX = psMouse->nBlkPosX;
				s32	nLastBlkY = psMouse->nBlkPosY;
				u32	nRet;
				s32	nNewX, nNewY;

				s32	nMouseLastPos = (psMouse->pWhere != NULL && psMouse->nBlkPosX != -1 ? psMouse->pWhere->nWhere : -1);		// Last pos pour effacement de la brosse.

				nNewX = event.motion.x;
				nNewY = event.motion.y;
				// Déplacement possible ?
				if ((nRet = Mouse_LocationCheck(psMouse, event.motion.x, event.motion.y)))
				{
					// Limite le déplacement en X ou Y.
					nNewX = (nRet & 1 ? psMouse->nPixPosX : event.motion.x);
					nNewY = (nRet & 2 ? psMouse->nPixPosY : event.motion.y);
					SDL_WarpMouse(nNewX, nNewY);
				}
				// Note les coordonnées de la souris + get location.
				psMouse->nPixPosX = nNewX;
				psMouse->nPixPosY = nNewY;
				Mouse_LocationGet(psMouse);
				gMisc.nUpdateDisp |= 4;

				// Changement de bloc ?
				if (psMouse->nBlkPosX != nLastBlkX || psMouse->nBlkPosY != nLastBlkY)
				{
					Mouse_BrushTiling(psMouse);	// Tiling ?
					gMisc.nUpdateDisp |= 3;

					psInfoBox->nMouseOnMonster = 0;	// RAZ flag.

				}

				// On traine la visu ?
				if (Map_VisuOnOff(gMisc.psMap))
				if (psMouse->nVisuDrag)
				{
					Visu_WindowLocation(gMisc.psMap->psVisu, psMouse->nPixPosX + psMouse->nVisuOffsX, psMouse->nPixPosY + psMouse->nVisuOffsY);
					gMisc.nUpdateDisp |= e_Update_All;
				}

				// Effacement de la brosse si la souris est passée sur une zone
				if (psMouse->pWhere == NULL)
				{
					switch (nMouseLastPos)
					{
					case e_MouseLoc_Map:
						gMisc.nUpdateDisp |= e_Update_Map;
						break;
					case e_MouseLoc_Planes:
						gMisc.nUpdateDisp |= e_Update_Planes;
						break;
					}
				}

			}
			break;

		// Mouse button pressed.
		case SDL_MOUSEBUTTONDOWN:
			//printf("Mouse button pressed : %d %d %d\n", (int)event.button.button, (int)event.button.x, (int)event.button.y);
			//if (event.button.button == SDL_BUTTON_LEFT)//, SDL_BUTTON_MIDDLE, SDL_BUTTON_RIGHT
			//event.button.x + y

			Mouse_ButtonPressed(psMouse, (u32)event.button.button, (u32)event.button.x, (u32)event.button.y);
			break;

		// Mouse button released.
		case SDL_MOUSEBUTTONUP:
			//printf("Mouse button released : %d %d %d\n", (int)event.button.button, (int)event.button.x, (int)event.button.y);

			Mouse_ButtonReleased(psMouse, (u32)event.button.button, (u32)event.button.x, (u32)event.button.y);
			break;

		} // end switch
	} // end of message processing

	return (0);

}

// Gestionnaire d'évenements pour l'info box des monstres.
void MstInfoBox_EventHandler(void)
{
	// message processing loop
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		// check for messages
		switch (event.type)
		{
		// Touche enfoncée.
		case SDL_KEYDOWN:
			break;

		// Touche relachée.
		case SDL_KEYUP:
			break;
		}
	}
}

//=============================================================================

// Récupère un entier sur un bout de chaîne (délimité par ':' ou une fin de chaîne).
void Sub_GetArgInt(char **ppStr, u32 *pnVal)
{
	u32	i;
	char	pBuf[256];
	char	*pStr = *ppStr;

	i = 0;
	while (*pStr != ':' && *pStr)
	{
		pBuf[i++] = *pStr++;
	}
	pBuf[i] = 0;
	*pnVal = atoi(pBuf);
	*ppStr = pStr;

}

// Recherche un fichier .EDT dans le répertoire courant.
char * LookForEDT(void)
{
#ifdef __LINUX__
	DIR	*dip;
	struct dirent	*dit;

	// Ouverture du répertoire en cours.
	if ((dip = opendir("./")) == NULL)
	{
		fprintf(stderr, "LookForEDT(): 'opendir' error.\n");
		exit(0);
	}

	// Boucle dans les entrées.
	while ((dit = readdir(dip)) != NULL)
	{
		// Est-ce un fichier normal ?
		if (dit->d_type == DT_REG)
		{
			if (strlen(dit->d_name) > 4)
			if (strcmp(&dit->d_name[strlen(dit->d_name) - 4], ".edt") == 0)
			{
				Font_StrReset(&gFont);
				Font_StrAdd(&gFont, dit->d_name);
//printf("file: %s\n", gFont._pString);
				break;	// On sort au premier fichier trouvé.
			}
		}
	}

	// Fermeture du répertoire.
	if (closedir(dip) == -1)
	{
		fprintf(stderr, "LookForEDT(): 'closedir' error.\n");
		exit(0);
	}
	return (gFont.pString);
#endif

#ifdef WIN32
	const char	*pRechFiles = "*.edt";
	HANDLE	hFind;
	WIN32_FIND_DATA	ffd;

	hFind = FindFirstFile(pRechFiles, &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "LookForEDT(): 'FindFirstFile' error.\n");
		exit(0);
	}
	Font_StrReset(&gFont);
	Font_StrAdd(&gFont, ffd.cFileName);
//printf("file: %s\n", ffd.cFileName);
//printf("file: %s\n", gFont._pString);
	FindClose(hFind);
	return (gFont.pString);
#endif

	return (NULL);
}

// Test de la ligne de commande.
// Out : 0 = Fichier existant / 1 = New file.
u32 CheckCommandLine(int nArgc, char **ppArgv)
{
//	printf ("argc = %d.\n", argc);
	u32	nCur;
	u8	nFilename = 0;	// Nom de fichier passé ou pas.
	u8	nNewFile = 0;
	char	*pStr;

//v	gMisc.nTmpVisuLg = VISU_DEFAULT_Width;
//v	gMisc.nTmpVisuHt = VISU_DEFAULT_Height;

	struct SMap	*psMap = gMisc.psMap;


	// Passe 1 : Recherche du nom de fichier.
	nCur = 1;
	while (nCur < (u32)nArgc)
	{
		pStr = *(ppArgv + nCur);	// Ptr sur l'arg en cours.
		// Prm ou nom de fichier ?
		if (*pStr != '-')
		{
			// Nom de fichier.
			File_FilenameSet(&gFile, pStr);
			// Flag.
			nFilename = 1;
			nCur = (u32)nArgc;	// Sortie de boucle.
		}
		nCur++;		// arg suivant.
	}

	// On a un nom de fichier ?
	if (nFilename == 0)
	{
		// Non. On cherche si un fichier est présent dans le répertoire.
		if ((pStr = LookForEDT()) != NULL)
		{
			File_FilenameSet(&gFile, pStr);
			nFilename = 1;
		}
		else
		{
			fprintf(stderr, "No filename specified and no existing file found in current location.\n");
			exit(0);
		}
	}


	// Le fichier existe déjà ? Oui > lecture / non > création.
	if (fexist(File_FilenameGet(&gFile)) == false)
	{
		// New file.
		printf("New file.\n");
		nNewFile = 1;
	}
	else
	{
		// Fichier existant > lecture.
		printf("Read file.\n");
		File_Load(&gFile);
	}


	// Passe 2 : Traitement des args.
	u32	nPlaneNo;

	nCur = 1;
	while (nCur < (u32)nArgc)
	{

		pStr = *(ppArgv + nCur);	// Ptr sur l'arg en cours.
		// Prm ou nom de fichier ?
		if (*pStr == '-')
		{
			// Paramètre.
			pStr++;		// On saute le '-'.
			switch(*pStr)
			{
			case 's':	// s : Default map size. / -s:width:height
				if (psMap->nPlanesNb)	// Possible uniquement si aucun plan n'a encore été ajouté.
				{
					printf("Default map size: Map already has planes, parameter ignored.\n");
					break;
				}
				pStr++;
				if (*pStr == ':')
				{
					u32	nNewMapLg, nNewMapHt;
					pStr++;
					Sub_GetArgInt(&pStr, &nNewMapLg);	// Lg.
					if (*pStr == ':')
					{
						pStr++;
						Sub_GetArgInt(&pStr, &nNewMapHt);	// Ht.
						// C'est bon !
						if (nNewMapLg && nNewMapHt)
						{
							psMap->nMapLg = nNewMapLg;
							psMap->nMapHt = nNewMapHt;
						}
					}
				}
				break;

			case 'a':	// a : Ajoute un plan. / -a:filename
				pStr++;
				if (*pStr == ':')
				{
					pStr++;
					if (strlen(pStr))
					{
						Map_PlaneAdd(psMap, pStr);
						printf("New plane added.\n");
					}
				}
				break;

			case 'r':	// r : Supprime un plan. / -r:plan_no
				pStr++;
				if (*pStr == ':')
				{
					pStr++;
					Sub_GetArgInt(&pStr, &nPlaneNo);	// On récupère le no de plan.
					if (nPlaneNo >= psMap->nPlanesNb)
					{
						printf("Remove plane: Plane #%d does not exist.\n", (int)nPlaneNo);
					}
					else
					{
						Map_PlaneRemove(psMap, nPlaneNo);
						printf("Plane %d removed.\n", (int)nPlaneNo);
					}
				}
				break;

			case 'i':	// i : Insère un plan. / -i:ins_pos:filename
				pStr++;
				if (*pStr == ':')
				{
					pStr++;
					Sub_GetArgInt(&pStr, &nPlaneNo);	// On récupère le no de plan.
					if (nPlaneNo >= psMap->nPlanesNb)
					{
						printf("Insert plane: Plane #%d does not exist.\n", (int)nPlaneNo);
					}
					else
					{
						if (*pStr == ':')
						{
							pStr++;
							if (strlen(pStr))
							{
								Map_PlaneInsert(psMap, pStr, nPlaneNo);
								printf("Plane '%s' inserted.\n", pStr);
							}
						}
					}
				}
				break;

			case 'c':	// c : Ajoute un fichier de commandes. / -c:plan_no:filename
				pStr++;
				if (*pStr == ':')
				{
					pStr++;
					Sub_GetArgInt(&pStr, &nPlaneNo);	// On récupère le no du plan.
					if (nPlaneNo >= psMap->nPlanesNb)
					{
						printf("Add command file: Plane #%d does not exist.\n", (int)nPlaneNo);
					}
					else
					{
						printf("cmd: plane = %d\n", (int)nPlaneNo);
						if (*pStr == ':')
						{
							pStr++;
							// On récupère le nom du fichier.
							printf("cmd: filename = %s\n", pStr);

							Map_PlaneCmdfileAdd(psMap, nPlaneNo, pStr);

							printf("Add command file: Plane %d - Command file added.\n", (int)nPlaneNo);
						}
						else
						{
							printf("Add command file: Incomplete parameters.\n");
						}
					}
				}
				break;

			case 'b':	// b : Ajoute/retire un plan de codes (8 bits) au plan x. / -b:plan_no:<+||->
				pStr++;
				if (*pStr == ':')
				{
					pStr++;
					Sub_GetArgInt(&pStr, &nPlaneNo);	// On récupère le no du plan.
					if (nPlaneNo >= psMap->nPlanesNb)
					{
						printf("Add/remove blocks codes: Plane #%d does not exist.\n", (int)nPlaneNo);
					}
					else
					{
						printf("codes: plane = %d\n", (int)nPlaneNo);
						if (*pStr == ':' && (*(pStr + 1) == '+' || *(pStr + 1) == '-') )
						{
							pStr++;
							// On récupère le + ou le -.
							if (*pStr == '+')
							{
								Map_PlaneCodesAdd(psMap, nPlaneNo);
								printf("Add blocks codes: Plane %d - ok.\n", (int)nPlaneNo);
							}
							else
							{
								Map_PlaneCodesRemove(psMap, nPlaneNo);
								printf("Remove blocks codes: Plane %d - ok.\n", (int)nPlaneNo);
							}
						}
						else
						{
							printf("Add/remove blocks codes: Incomplete parameters.\n");
						}
					}
				}
				break;

			case 't':	// t : Couleur de transparence / -t:R:G:B
				pStr++;
				if (*pStr == ':')
				{
					u32	nTmpR, nTmpG, nTmpB;
					pStr++;
					Sub_GetArgInt(&pStr, &nTmpR);	// R.
					if (*pStr == ':')
					{
						pStr++;
						Sub_GetArgInt(&pStr, &nTmpG);	// G.
						if (*pStr == ':')
						{
							pStr++;
							Sub_GetArgInt(&pStr, &nTmpB);	// B.
							// C'est bon !
							gMisc.nTransR = nTmpR;
							gMisc.nTransG = nTmpG;
							gMisc.nTransB = nTmpB;
						}
					}
				}
				break;

			case 'v':	// t : Taille de la visu / -v:lg:ht
				pStr++;
				if (*pStr == ':')
				{
					u32	nNewVisuLg, nNewVisuHt;
					pStr++;
					Sub_GetArgInt(&pStr, &nNewVisuLg);	// Lg.
					if (*pStr == ':')
					{
						pStr++;
						Sub_GetArgInt(&pStr, &nNewVisuHt);	// Ht.
						// C'est bon !
						gMisc.nVisuLg = nNewVisuLg;
						gMisc.nVisuHt = nNewVisuHt;
					}
				}
				break;

// > voir si on garde.
			case 'z':	// z : Force des codes de blocs / -z:plane:blk start:blk end:code
				pStr++;
				if (*pStr == ':')
				{
					u32	nBlkPlane, nBlkStart, nBlkEnd, nBlkCode, i;
					pStr++;
					Sub_GetArgInt(&pStr, &nBlkPlane);	// Plane.
					if (nBlkPlane >= psMap->nPlanesNb) break;
					if (*pStr == ':')
					{
						pStr++;
						Sub_GetArgInt(&pStr, &nBlkStart);	// Blk start.
						if (*pStr == ':')
						{
							pStr++;
							Sub_GetArgInt(&pStr, &nBlkEnd);	// Blk end.
							if (*pStr == ':')
							{
								pStr++;
								Sub_GetArgInt(&pStr, &nBlkCode);	// Blk code.

								for (i = nBlkStart; i <= nBlkEnd; i++)
									Map_PlaneCodeSet(psMap, nBlkPlane, i, nBlkCode);

							}
						}
					}
				}
				break;
// < voir si on garde.

			}

		}

		//printf ("cur = %d / arg = %s.\n", (int)nCur, *(ppArgv + nCur));

		nCur++;		// arg suivant.
	}

	// On vérifie qu'il y a au moins un plan (nouveau fichier ou suppressions de plans).
	if (psMap->nPlanesNb == 0)
	{
		fprintf(stderr, "No planes: Please use the -a:<graphic file> option to add at least one plane.\n");
		exit(1);
	}

	Map_PlanesSetNo(psMap);	// Numérote les plans.

//v	psMap->pVisu = Visu_New(gMisc.nTmpVisuLg, gMisc.nTmpVisuHt);	// Initialise la visu.

	return (0);
}

/*
test peu concluant

#ifdef __LINUX__
#define	STD_OUTPUT	"/dev/tty"
#endif
#ifdef WIN32
#define	STD_OUTPUT	"CON"
#endif

//	freopen(STD_OUTPUT, "w", stdout);
//	freopen(STD_OUTPUT, "w", stderr);
*/

//=============================================================================

// Main.
int main(int argc, char **argv)
{
	Misc_Init(&gMisc);

	gMisc.psInfoBox = InfoBox_New();
	gMisc.psMouse = Mouse_New();
	Font_StrReset(&gFont);
	gMisc.psMap = Map_New();
	gMisc.psUndo = Undo_New();

//v	gMisc.nScrW = 800;//1024;//800;
//v	gMisc.nScrH = 600;//768;//600;		// Lg & Ht de l'écran. Init ici, car dimensions testées dans l'init de la visu.

	// Check des paramètres sur la ligne de commande.
	CheckCommandLine(argc, argv);

	// Initialize SDL video.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		return (1);
	}
	// Make sure SDL cleans up before exit.
	atexit(SDL_Quit);
	// Create a new window.
	putenv("SDL_VIDEO_WINDOW_POS=center");
/*
	gMisc.pScreen = SDL_SetVideoMode(gMisc.nScrW, gMisc.nScrH, 16, SDL_SWSURFACE);
    if (gMisc.pScreen == NULL)
    {
		fprintf(stderr, "Unable to set up video: %s\n", SDL_GetError());
		return (1);
    }
*/
	Misc_SetVideoMode(&gMisc, 0);

	gMisc.psKbd = Kbd_New();		// Init clavier. (APRES init SDL).

	Map_GfxConvert(gMisc.psMap);	// Conversion des planches en 16 bits. (Lecture terminée).

	// Barre de titre.
	char pWndTitle[256+10] = "EdTile: ";
	strcat(pWndTitle, File_FilenameGet(&gFile));
	SDL_WM_SetCaption(pWndTitle, NULL);



/*
// Capture de la fonte.
	// Lecture de la fonte.
	SDL_Surface	*pFnt = SDL_LoadBMP("fnt165_line.bmp");
	if (pFnt == NULL) {
		fprintf(stderr, "Couldn't load picture: %s\n", SDL_GetError());
		exit(1);
	}
	u8 *pSrc = (u8 *)pFnt->pixels;
	for (u32 ix = 0; ix < (u32)pFnt->w; ix += 8)
	{
		for (u32 yy = 0; yy < (u32)pFnt->h; yy++)
		{
			for (u32 xx = 0; xx < 8; xx++)
			{
				printf("%d,", (*(pSrc + ix + xx + (yy * pFnt->w)) ? 1 : 0));
			}
			printf("\n");
		}
		printf("\n");
	}
	SDL_FreeSurface(pFnt);
*/
//printf ("%c %x / %c %x", 'c', 'c', 'C', 'C');


	Map_PlanesDisplayWnds(gMisc.psMap);
	Map_MapDisplay(gMisc.psMap);
	Map_StatusBarDisplay(gMisc.psMap);

	Undo_Save(gMisc.psUndo);		// Premier undo.
	gMisc.psMap->nModified = 0;		// RAZ Flag après le premier undo.

	// Main loop.
	FrameInit();
	while (1)
	{
		gMisc.nUpdateDisp = 0;

		// Gestion des évenements.
		if (EventHandler() != 0) break;
		Kbd_Manage(gMisc.psKbd);		// Gestion du clavier.



		//>> Entrée en édition de monstre ?
		if (gMisc.psInfoBox->nMouseOnMonster == 2)
		{
			u32	nKbMst = e_KB_MST_Null;

			// Copie du monstre en cours pour annulation eventuelle.
			struct SMstUnit	*pMstCopy;
			pMstCopy = MstUnit_New();
			MstUnit_InitFromMst(pMstCopy, gMisc.psInfoBox->pMstCur);

			gMisc.psInfoBox->nLnSel = 0;	// Sélecteur.
			InfoBox_MstInfoBoxDraw(gMisc.psInfoBox);	// Premier tracé, pour afficher le sélecteur.

			gMisc.nUpdateDisp = 1;			// Pour forcer le flip du premier tracé.
			while (nKbMst == e_KB_MST_Null)
			{
//				gMisc.nUpdateDisp = 0;		// ... du coup décalé en fin de boucle.

				// Gestion des évenements.
				MstInfoBox_EventHandler();
				nKbMst = Kbd_MstInfoBoxKbManage(gMisc.psKbd);

				if (gMisc.nUpdateDisp)
					InfoBox_MstInfoBoxDraw(gMisc.psInfoBox);//.MstInfoDisplay();

				FrameWait();	// Pour donner un peu de temps à l'OS.
				// Flip.
				if (gMisc.nUpdateDisp)
					SDL_Flip(gMisc.pScreen);

				gMisc.nUpdateDisp = 0;
			}

			// Cancel : On recopie les valeurs originales dans le monstre.
			// !!! On ne peut pas switcher les pointeurs, ou alors il faut aussi updater le pointeur dans le tableau de ptr du plan !!!
			if (nKbMst == e_KB_MST_Cancel)
			{
				MstUnit_InitFromMst(gMisc.psInfoBox->pMstCur, pMstCopy);
			}
			else
			{
				Undo_Save(gMisc.psUndo);	// Sauvegarde de la modif.
			}
			MstUnit_Delete(pMstCopy);

			// Sortie, refresh.
			// On fait un warp mouse aux pos de départ pour forcer un refresh.
			SDL_WarpMouse(gMisc.psMouse->nPixPosX, gMisc.psMouse->nPixPosY);

			gMisc.nUpdateDisp |= e_Update_All;
		}
		//<< Entrée en édition de monstre ?


		// Update display.
		if (gMisc.nUpdateDisp)
		{
			if (gMisc.nUpdateDisp & 1) Map_PlanesDisplayWnds(gMisc.psMap);
			if (gMisc.nUpdateDisp & 2) Map_MapDisplay(gMisc.psMap);
			Mouse_LocationGet(gMisc.psMouse);
			Mouse_BrushDraw(gMisc.psMouse);
//			Mouse_LocationGet(gMisc.psMouse);	// Pourquoi après ??? test : remis au dessus.
			Map_StatusBarDisplay(gMisc.psMap);
			InfoBox_MstInfoDisplay(gMisc.psInfoBox);
		}
		// Visu (affichage par dessus le reste).
		if (Map_VisuOnOff(gMisc.psMap))
		{
			Map_VisuDisplay(gMisc.psMap);
			gMisc.nUpdateDisp |= 16;			// Pour éviter de créer un variable pour flip ou non.
		}

		FrameWait();	// Pour donner un peu de temps à l'OS.
		// Flip.
		if (gMisc.nUpdateDisp)
			SDL_Flip(gMisc.pScreen);

	}

	// Libère les ressources des pseudo objets.
	Kbd_Delete(gMisc.psKbd);
	Mouse_Delete(gMisc.psMouse);
	InfoBox_Delete(gMisc.psInfoBox);

	Map_Delete(gMisc.psMap);
	Undo_Delete(gMisc.psUndo);

    // The end.
    printf("Exited cleanly.\n");
    return (0);

}

