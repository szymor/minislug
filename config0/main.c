
#include "includes.h"

struct SMisc	gMisc;

//=====================================

enum
{
	e_CfgKey_Up = 0,
	e_CfgKey_Down,
	e_CfgKey_Left,
	e_CfgKey_Right,
	e_CfgKey_ButtonA,
	e_CfgKey_ButtonB,
	e_CfgKey_ButtonC,

	e_CfgKey_LAST,

	e_CfgKey_JoyButtonA = e_CfgKey_LAST,
	e_CfgKey_JoyButtonB,
	e_CfgKey_JoyButtonC,

	e_CfgKey_MAX
};

#pragma pack(1)
struct SMSCfg
{
	u16	pKeys[e_CfgKey_MAX];
	u16	nVideoMode;			// 0 = 320x224 / 1 = x2 / 2 = TV2x.
	u16	nChecksum;
};
#pragma pack()
struct SMSCfg	gMSCfg;

// Cmp du qsort.
int CfgCmp(const void *a, const void *b)
{
  return (gMSCfg.pKeys[*(u16 *)a] - gMSCfg.pKeys[*(u16 *)b]);
}

// Calcul d'un petit checksum.
u32	CfgChecksumCalc(void)
{
	u32	i;
	u32	nSum = 0;

	for (i = 0; i < e_CfgKey_MAX; i++) nSum += gMSCfg.pKeys[i];
	nSum += gMSCfg.nVideoMode;

	return (nSum);
}

// Teste si on a pas deux touches identiques.
u32 CfgCheck(void)
{
	u16	pKeysIdx[e_CfgKey_MAX];
	u32	i;

	// Tri de valeurs des touches.
	for (i = 0; i < e_CfgKey_MAX; i++) pKeysIdx[i] = i;
	qsort(pKeysIdx, e_CfgKey_MAX, sizeof(u16), CfgCmp);

/*
printf("----------\n");
for (i = 0; i < e_CfgKey_MAX; i++) printf("%d ", pKeysIdx[i]);
printf("\n");
for (i = 0; i < e_CfgKey_MAX; i++) printf("%d ", gMSCfg.pKeys[pKeysIdx[i]]);
printf("\n");
*/

	// Si il y a plus de 1 fois une touche, erreur.
	for (i = 0; i < e_CfgKey_MAX-1; i++)
	{
		if (gMSCfg.pKeys[pKeysIdx[i + 1]] == 0xFFFF) break;		// On arrête le test sur un 0xFFFF (== case vide).
		if (gMSCfg.pKeys[pKeysIdx[i + 1]] == gMSCfg.pKeys[pKeysIdx[i]]) return (1);
	}

	return (0);
}

// Teste le mode vidéo choisi, remise à la valeur par défaut si pb.
u32 CfgVideoModeCheck(void)
{
	u32	nRet = 0;

	switch(gMSCfg.nVideoMode)
	{
	case SDLK_0:
	case SDLK_1:
	case SDLK_2:
		gMSCfg.nVideoMode = gMSCfg.nVideoMode - SDLK_0;
		break;

	case SDLK_KP0:
	case SDLK_KP1:
	case SDLK_KP2:
		gMSCfg.nVideoMode = gMSCfg.nVideoMode - SDLK_KP0;
		break;

	default:
		gMSCfg.nVideoMode = 0;
		nRet = 1;
		break;
	}
//printf("Video mode: %d\n", (int)gMSCfg.nVideoMode);
	return (nRet);
};

#define	CFG_FILENAME	"mslug.cfg"
// Sauvegarde de la config.
u32 CfgSave(void)
{
	FILE	*fPt;
	u32	nSz;

	// On cleare le b15 sur les touches du joystick.
	gMSCfg.pKeys[e_CfgKey_JoyButtonA] &= 0x7FFF;
	gMSCfg.pKeys[e_CfgKey_JoyButtonB] &= 0x7FFF;
	gMSCfg.pKeys[e_CfgKey_JoyButtonC] &= 0x7FFF;

	gMSCfg.nChecksum = (u16)CfgChecksumCalc();

	fPt = fopen(CFG_FILENAME, "wb");
	if (fPt == NULL)
	{
		fprintf(stderr, "CfgSave(): fopen error.\n");
		return (1);
	}
	nSz = fwrite(&gMSCfg, 1, sizeof(struct SMSCfg), fPt);
	fclose(fPt);
	if (nSz != sizeof(struct SMSCfg))
	{
		fprintf(stderr, "CfgSave(): fwrite error.\n");
		return (1);
	}
	return (0);
}

/*
// Lecture de la config.
u32 CfgLoad(void)
{
	FILE	*fPt;
	u32	nSz;

	static struct SMSCfg	sCfgDefault = { { SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_v, SDLK_c, SDLK_x, 0, 1, 2 }, 0, 0 };	// Cfg par défaut.

	fPt = fopen(CFG_FILENAME, "rb");
	if (fPt == NULL)
	{
		fprintf(stderr, "CfgLoad(): fopen error.\n");
		goto _CfgLoad_def;
	}
	nSz = fread(&gMSCfg, 1, sizeof(struct SMSCfg), fPt);
	fclose(fPt);
	if (nSz != sizeof(struct SMSCfg))
	{
		fprintf(stderr, "CfgLoad(): fread error.\n");
		goto _CfgLoad_def;
	}
	// Test du checksum.
	nSz = CfgChecksumCalc();
	if (gMSCfg.nChecksum != (u16)nSz)
	{
		fprintf(stderr, "CfgLoad(): Checksum error.\n");
		goto _CfgLoad_def;
	}
	return (0);
_CfgLoad_def:
	fprintf(stderr, "CfgLoad(): Default configuration used.\n");
	memcpy(&gMSCfg, &sCfgDefault, sizeof(struct SMSCfg));
	return (1);
}
*/

//=====================================

#define	TXTMODE_LINE_HT	11

struct STxtMode
{
	u8	nUpdate;	// Screen à flipper ou pas.
	u8	nNewLine;	// Faut-il décaler d'une ligne avant de printf ?
	u16	nPosX;		// PosX sur la ligne.
	u16	nClr;		// Couleur du texte.
};
struct STxtMode	gTxtMode;

/*
void TxtMode_Init(void)
{
	gTxtMode.nUpdate = 0;	// Screen à flipper ou pas.
	gTxtMode.nPosX = 0;		// PosX sur la ligne.
}
*/

// Couleur du texte.
void TxtMode_ColorSet(u16 nClr)
{
	gTxtMode.nClr = nClr;
}

// Cls.
void TxtMode_CLS(void)
{
	SDL_FillRect(gMisc.pScreen, 0, SDL_MapRGB(gMisc.pScreen->format, 0, 0, 0));
	gTxtMode.nPosX = 0;
	gTxtMode.nNewLine = 0;
	gTxtMode.nUpdate = 1;
}

// New line.
void TxtMode_NewLine(void)
{
	SDL_Rect	sSrc, sDst;

	sSrc.x = 0;
	sSrc.y = TXTMODE_LINE_HT;
	sSrc.w = SCR_Width;
	sSrc.h = SCR_Height - TXTMODE_LINE_HT;

	sDst.x = 0;
	sDst.y = 0;
	sDst.w = SCR_Width;
	sDst.h = SCR_Height - TXTMODE_LINE_HT;

	SDL_BlitSurface(gMisc.pScreen, &sSrc, gMisc.pScreen, &sDst);

	sDst.x = 0;
	sDst.y = SCR_Height - TXTMODE_LINE_HT;
	sDst.w = SCR_Width;
	sDst.h = TXTMODE_LINE_HT;

	SDL_FillRect(gMisc.pScreen, &sDst, 0);

	gTxtMode.nNewLine = 0;
	gTxtMode.nPosX = 0;
	gTxtMode.nUpdate = 1;
}

// Printf.
void TxtMode_Printf(const char *pStr, u32 nNewLine)
{
	if (gTxtMode.nNewLine) TxtMode_NewLine();
	gTxtMode.nPosX += FntPrintf(8 + gTxtMode.nPosX, SCR_Height - TXTMODE_LINE_HT, pStr, gTxtMode.nClr);
//	if (nNewLine) TxtMode_NewLine();
	gTxtMode.nNewLine = nNewLine;	// Bufferisé pour le prochain printf.
	gTxtMode.nUpdate = 1;
}

//=====================================

// Message processing loop
u32 Events(void)
{
	u32	nRet = 0;

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		// Check for messages.
		switch (event.type)
		{
		case SDL_QUIT:		// Exit if the window is closed
			nRet = 1;
			break;

		case SDL_KEYDOWN:	// Check for keypresses
			if (event.key.keysym.sym == SDLK_ESCAPE)	// Exit if ESCAPE is pressed
				nRet = 1;
			break;
		} // end switch
	} // end of message processing

	return (nRet);
}

#define	VIDEOTXT_NB	4
#define	JOY_KEYS_MAX	16
int main(int argc, char **argv)
{
	u8	*pKeys;
	u32	i;
	u32	nLn = 0;

//CfgLoad();	// test

	// Initialize SDL video.
//	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	// Make sure SDL cleans up before exit.
	atexit(SDL_Quit);
	// Create a new window.
	gMisc.pScreen = SDL_SetVideoMode(SCR_Width, SCR_Height, 16, SDL_SWSURFACE);
	if (gMisc.pScreen == NULL)
	{
		fprintf(stderr, "Unable to set up graphics: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_WM_SetCaption("Mini Slug CFG", NULL);

	// Init joystick.
	SDL_Joystick *pJoystick = NULL;
	if (SDL_NumJoysticks() > 0)
	{
//		printf("Found: %s\n", SDL_JoystickName(0));
		SDL_JoystickEventState(SDL_ENABLE);
		pJoystick = SDL_JoystickOpen(0);		// Si pas NULL, joystick !
/*
		if (pJoystick != NULL)
		{
			printf("NumButtons: %d\n", SDL_JoystickNumButtons(pJoystick));
			printf("NumHats: %d\n", SDL_JoystickNumHats(pJoystick));
			printf("NumAxis: %d\n", SDL_JoystickNumAxes(pJoystick));
		}
*/
	}



	// Les couleurs de texte.
	u16	nClrWhite, nClrRed, nClrGreen;
	nClrWhite = SDL_MapRGB(gMisc.pScreen->format, 255, 255, 255);
	nClrRed = SDL_MapRGB(gMisc.pScreen->format, 255, 10, 10);
	nClrGreen = SDL_MapRGB(gMisc.pScreen->format, 10, 255, 10);

	// Cls.
	TxtMode_CLS();
	TxtMode_ColorSet(nClrWhite);
	TxtMode_Printf("*** Mini Slug Project.", 1);
	TxtMode_Printf("*** Configuration utility.", 1);

// Encapsulation pour retry.
//u8	nRedo;
//do
//{
//	nRedo = 0;

	const char *pKeysTxt[e_CfgKey_MAX + 1] = {
		"Up",
		"Down",
		"Left",
		"Right",
		"Button A (Shot)",
		"Button B (Jump)",
		"Button C (Bomb)",

		"Joystick Button A (Shot)",
		"Joystick Button B (Jump)",
		"Joystick Button C (Bomb)",

		"Video mode (0/1/2)",
	};
	u8	pJoyKeys[JOY_KEYS_MAX];

/*
	// On saute deux lignes.
	TxtMode_ColorSet(nClrWhite);
	TxtMode_NewLine();
	TxtMode_NewLine();

	TxtMode_Printf("Enter custom keys:", 1);
	nLn = 0;
	TxtMode_Printf(pKeysTxt[nLn], 0);
*/

	memset(gMSCfg.pKeys, 0xFF, e_CfgKey_MAX * sizeof(u16));		// Cases à 0xFFFF pour test à chaque touche.
	memset(pJoyKeys, 0, JOY_KEYS_MAX);

	pKeys = SDL_GetKeyState(NULL);	// Le clavier.
	for (i = 0; i < SDLK_LAST; i++) pKeys[i] = 0;	// RAZ Kb.
	// Program main loop.
	u8	nLoop = 1;
	u8	nStep = 0x80;	// b7 = Un truc à écrire.
//nStep = 0x81;	// b7 = Un truc à écrire.
	while (nLoop)
	{
		char	pStrOk[20];

		// Events.
		if (Events()) { nLoop = 0; break; }

		// Titre de chapitre.
		if (nStep & 0x80)
		{
			nStep &= 0x7F;

			TxtMode_ColorSet(nClrWhite);
			TxtMode_NewLine();
			TxtMode_NewLine();

			nLn = 0;
			switch (nStep)
			{
			case 0:		// Keyboard.
				TxtMode_Printf("Keyboard configuration.", 1);
				TxtMode_Printf("Please press:", 1);
				nLn = 0;
				memset(gMSCfg.pKeys, 0xFF, e_CfgKey_MAX * sizeof(u16));		// Cases à 0xFFFF pour test à chaque touche.
				break;

			case 1:		// Joystick.
				TxtMode_Printf("Joystick detected.", 1);
				TxtMode_Printf("Please press:", 1);
				nLn = e_CfgKey_JoyButtonA;
				gMSCfg.pKeys[e_CfgKey_JoyButtonA] = gMSCfg.pKeys[e_CfgKey_JoyButtonB] = gMSCfg.pKeys[e_CfgKey_JoyButtonC] = 0xFFFF;
				break;

			case 2:		// Video mode.
				{
				const char *pVideoTxt[VIDEOTXT_NB] = {
					"Now select video mode:",
					"0: 320 * 224",
					"1: 640 * 448 - 2x",
					"2: 640 * 448 - TV2x",
				};
				u32	j;
				for (j = 0; j < VIDEOTXT_NB; j++)
					TxtMode_Printf(pVideoTxt[j], 1);
				}
				nLn = e_CfgKey_MAX;
				break;
			}
			nLn |= 0x80;
		}

		// Lignes.
		if (nLn & 0x80)
		{
			nLn &= 0x7F;
			TxtMode_Printf(pKeysTxt[nLn], 0);	// Ligne en cours.
		}
		switch (nStep)
		{
		case 0:		// Keyboard.
			for (i = 0; i < SDLK_LAST; i++)
			if (pKeys[i] && (i >= SDLK_F1 && i <= SDLK_F12) == 0)
			{
				pKeys[i] = 0;

				TxtMode_Printf(" => ", 0);
				gMSCfg.pKeys[nLn] = i;
				snprintf(pStrOk, sizeof(pStrOk), "%3d", (int)i);	// pas de itoa en Linux...
				TxtMode_Printf(pStrOk, 1);

				// Des touches en double ?
				if (CfgCheck())
				{
					TxtMode_ColorSet(nClrRed);
					TxtMode_Printf("Duplicate keys found. Please retry.", 1);
					TxtMode_ColorSet(nClrWhite);
					// On recommence la saisie.
					nStep = 0 | 0x80;
				}
				else
				{
					if (nLn + 1 == e_CfgKey_LAST)	// Passage à l'étape suivante ?
					{
						nStep = 1 | 0x80;
						if (pJoystick == NULL)
						{
							// Pas de joystick, conf par défaut et on passe à la vidéo.
							gMSCfg.pKeys[e_CfgKey_JoyButtonA] = 0x8000;
							gMSCfg.pKeys[e_CfgKey_JoyButtonB] = 0x8001;
							gMSCfg.pKeys[e_CfgKey_JoyButtonC] = 0x8002;
							nStep = 2 | 0x80;
						}
					}
					else
						nLn = (nLn + 1) | 0x80;		// Ligne suivante.
				}

			}
			break;

		case 1:		// Joystick.
			// Si un bouton est toujours enfoncé, on quitte.
			for (i = 0; i < JOY_KEYS_MAX; i++)
				if (pJoyKeys[i])
					if (SDL_JoystickGetButton(pJoystick, i)) break;
			if (i < JOY_KEYS_MAX) break;
			// Ok, clear de la table.
			memset(pJoyKeys, 0, JOY_KEYS_MAX);

			// Test.
			for (i = 0; i < SDL_JoystickNumButtons(pJoystick); i++)
			if (SDL_JoystickGetButton(pJoystick, i))
			{
				pJoyKeys[i] = 1;	// Flag.

				TxtMode_Printf(" => ", 0);
				gMSCfg.pKeys[nLn] = i | 0x8000;		// b15 pour éviter des conflits avec les nos des touches. On le clearera à la sauvegarde.
				snprintf(pStrOk, sizeof(pStrOk), "%3d", (int)i);	// Car pas de itoa en Linux...
				TxtMode_Printf(pStrOk, 1);

				// Des touches en double ?
				if (CfgCheck())
				{
					TxtMode_ColorSet(nClrRed);
					TxtMode_Printf("Duplicate keys found. Please retry.", 1);
					TxtMode_ColorSet(nClrWhite);
					// On recommence la saisie.
					nStep = 1 | 0x80;
				}
				else
				{
					if (nLn + 1 == e_CfgKey_MAX)	// Passage à l'étape suivante ?
						nStep = 2 | 0x80;
					else
						nLn = (nLn + 1) | 0x80;		// Ligne suivante.
				}

			}
			break;

		case 2:		// Video mode.
			for (i = 0; i < SDLK_LAST; i++)
			if (pKeys[i] && (i >= SDLK_F1 && i <= SDLK_F12) == 0)
			{
				pKeys[i] = 0;

				TxtMode_Printf(" => ", 0);
				gMSCfg.nVideoMode = i;
				pStrOk[0] = '*';
				pStrOk[1] = 0;

				// Test du mode video.
				u32	nPb;
				if ((nPb = CfgVideoModeCheck()) == 0) pStrOk[0] = gMSCfg.nVideoMode + '0';
				TxtMode_Printf(pStrOk, 1);

				if (nPb)
				{
					TxtMode_ColorSet(nClrRed);
					TxtMode_Printf("Please select a valid video mode.", 1);
					TxtMode_ColorSet(nClrWhite);
					nLn |= 0x80;
				}
				else
					nStep = 3;	// Fin, et pas de bit 7 exprès !
			}
			break;

		case 3:		// Fin.
			nLoop = 0;
			// Sauvegarde (cfg déjà testée).
			if (CfgSave())
			{
				TxtMode_ColorSet(nClrRed);
				TxtMode_Printf("Unable to save cfg.", 1);
			}
			else
			{
				TxtMode_ColorSet(nClrGreen);
				TxtMode_Printf("Done. Press any key to exit.", 1);
			}
			break;
		}


/*
			// Si le bouton précédent est toujours enfoncé, on quitte.
//			if (nLn > e_CfgKey_JoyButtonA)
//			if (gMSCfg.pKeys[nLn - 1] != 0xFFFF)
//			if (SDL_JoystickGetButton(pJoystick, gMSCfg.pKeys[nLn - 1] & 0x7FFF)) break;
//	memset(pJoyKeys, 0, JOY_KEYS_MAX);

		// Une touche a été pressée ?
		for (i = 0; i < SDLK_LAST; i++)
		if (pKeys[i] && (i >= SDLK_F1 && i <= SDLK_F12) == 0)
		{
			// On note le code.
//			printf("Code = %d\n", (int)i);
			TxtMode_Printf(" => ", 0);
			char	pStrOk[20];
			if (nLn < e_CfgKey_MAX - 3)
			{	// Touches du jeu.
				gMSCfg.pKeys[nLn] = i;
				snprintf(pStrOk, sizeof(pStrOk), "%3d", (int)i);	// pas de itoa en Linux...
				TxtMode_Printf(pStrOk, 1);

				// Des touches en double ?
				if (CfgCheck())
				{
					TxtMode_ColorSet(nClrRed);
					TxtMode_Printf("Duplicate keys found. Please retry.", 1);
					TxtMode_ColorSet(nClrWhite);
					// On recommence la saisie.
//					nLoop = 0;
//					nRedo = 1;
//					break;

					nLn = -1;
					// On saute deux lignes.
					TxtMode_ColorSet(nClrWhite);
					TxtMode_NewLine();
					TxtMode_NewLine();
					TxtMode_Printf("Enter custom keys:", 1);
					memset(gMSCfg.pKeys, 0xFF, e_CfgKey_MAX * sizeof(u16));		// Cases à 0xFFFF pour test à chaque touche.
				}

/ *
				// Passage au mode vidéo à la prochaine question ?
				if (nLn + 1 == e_CfgKey_MAX - 3)
				{
					TxtMode_NewLine();
					TxtMode_NewLine();
					u32	j;
					for (j = 0; j < VIDEOTXT_NB; j++)
						TxtMode_Printf(pVideoTxt[j], 1);
				}
* /
				// Passage au joystick à la prochaine question ?
				if (nLn + 1 == e_CfgKey_MAX - 3)
				{
					TxtMode_NewLine();
					TxtMode_NewLine();
					if (pJoystick != NULL)
					{	// On passe au joystick.
						TxtMode_Printf("Joystick detected.", 1);
					}
					else
					{	// On passe au mode vidéo.
						u32	j;
						for (j = 0; j < VIDEOTXT_NB; j++)
							TxtMode_Printf(pVideoTxt[j], 1);
						nLn = e_CfgKey_MAX;
					}
				}

			}
			else
			if (nLn <= e_CfgKey_MAX - 1)
			{	// Boutons du joystick.


				gMSCfg.pKeys[nLn] = i;
				snprintf(pStrOk, sizeof(pStrOk), "%3d", (int)i);	// pas de itoa en Linux...
				TxtMode_Printf(pStrOk, 1);


				// Passage au mode vidéo à la prochaine question ?
				if (nLn + 1 == e_CfgKey_MAX)
				{
					TxtMode_NewLine();
					TxtMode_NewLine();
					u32	j;
					for (j = 0; j < VIDEOTXT_NB; j++)
						TxtMode_Printf(pVideoTxt[j], 1);
				}

			}
			else
			{	// Mode vidéo.
				u32	nPb;

				gMSCfg.nVideoMode = i;
				pStrOk[0] = '*';
				pStrOk[1] = 0;

				// Test du mode video.
				if ((nPb = CfgVideoModeCheck()) != 0)
					nLn--;		// On reposera la même question.
				else
					pStrOk[0] = gMSCfg.nVideoMode + '0';
				TxtMode_Printf(pStrOk, 1);

				if (nPb)
				{
					TxtMode_ColorSet(nClrRed);
					TxtMode_Printf("Please select a valid video mode.", 1);
					TxtMode_ColorSet(nClrWhite);
				}
			}
/ *
				else
				{
					if (pJoystick != NULL)
					{	// On passe au joystick.
						TxtMode_NewLine();
						TxtMode_NewLine();
						TxtMode_Printf("Joystick detected.", 1);
					}
					else
					{}
				}
			else
			{	// Joystick.

			}
* /
			// RAZ touche.
			pKeys[i] = 0;

			// Prochaine touche ou fin.
			nLn++;
			if (nLn >= e_CfgKey_MAX + 1)
			{	// Fin de la saisie.
				nLoop = 0;
				// Sauvegarde (cfg déjà testée).
				if (CfgSave())
				{
					TxtMode_ColorSet(nClrRed);
					TxtMode_Printf("Unable to save cfg.", 1);
				}
				else
				{
					TxtMode_ColorSet(nClrGreen);
					TxtMode_Printf("Done. Press any key to exit.", 1);
				}
				TxtMode_ColorSet(nClrWhite);
			}
			else
				TxtMode_Printf(pKeysTxt[nLn], 0);	// Next line.

			break;
		}
*/

		// Flip.
		if (gTxtMode.nUpdate)
		{
			SDL_Flip(gMisc.pScreen);
			gTxtMode.nUpdate = 0;
		}

		SDL_Delay(15);

	} // end main loop


	// Pause.
	nLoop = 1;
	while (nLoop)
	{
		// Events.
		if (Events()) nLoop = 0;
		// Any key ?
		for (i = 0; i < SDLK_LAST; i++) if (pKeys[i]) nLoop = 0;

		SDL_Delay(3);
	}

//}
//while (nRedo);

	if (pJoystick != NULL) SDL_JoystickClose(pJoystick);

	// all is well ;)
//	printf("Exited cleanly.\n");
	return (0);
}




