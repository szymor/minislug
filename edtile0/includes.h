
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "SDL.h"
#include <sys/stat.h>

#ifdef __LINUX__
	#include <stdlib.h>
	#include <strings.h>
	#include <sys/types.h>
	#include <dirent.h>
	#include <errno.h>
#endif
#ifdef WIN32
//	#include <cstdlib>
	#include <windows.h>
#endif

#include "ctypes.h"
typedef enum { false = 0, true } bool;

#include "psd.h"
#include "file.h"
#include "cmd.h"
#include "plane.h"
#include "visu.h"
#include "map.h"
#include "font.h"
#include "keyboard.h"
#include "mouse.h"
#include "infobox.h"
#include "undo.h"


#define	ABS(x)	((x) < 0 ? -(x) : (x))
#define	SGN(x)	((x) >= 0 ? 1 : -1)
#define	MIN(x,y) ((x) < (y) ? (x) : (y))
#define NBELEM(tab) (sizeof(tab) / sizeof(tab[0]))


enum
{
	e_Flg_FullScreen = 1 << 0,
	e_Flg_Transparence = 1 << 1,

};

enum
{
	e_Update_Planes = 1 << 0,
	e_Update_Map = 1 << 1,
	e_Update_StatusBar = 1 << 2,
	e_Update_All = e_Update_Planes | e_Update_Map | e_Update_StatusBar,

};


// Struct générale avec tout le bordel.
struct SMisc
{
	SDL_Surface	*pScreen;	// L'écran.
	u32	nScrW, nScrH;		// Lg & Ht de l'écran.

	u32	nFlags;
	u32	nUpdateDisp;
	u8	nTransR, nTransG, nTransB;	// RGB de la couleur de transparence.

	u32	nVisuLg, nVisuHt;			// Pour taille visu (lecture, modif par ligne de commande...).
	u8	nVideoIdx;					// Taille écran.

	struct SKeyboard	*psKbd;
	struct SInfoBox		*psInfoBox;
	struct SMouse		*psMouse;

	struct SMap			*psMap;
	struct SUndo		*psUndo;

};

extern	struct SMisc	gMisc;

void Misc_Init(struct SMisc *psMisc);
void Misc_SetVideoMode(struct SMisc *psMisc, u32 nInc);
void Misc_FullScreenToggle(struct SMisc *psMisc);
u32 Misc_FullScreenGet(struct SMisc *psMisc);
void Misc_TransparencyToggle(struct SMisc *psMisc);
u32 Misc_TransparencyGet(struct SMisc *psMisc);


