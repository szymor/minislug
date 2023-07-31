
// Includes.
#include <stdlib.h>
#include <unistd.h>
#include "SDL.h"
#include <math.h>
#include <time.h>
#include <assert.h>

#include "ctypes.h"

#define	MAP_PLANES_MAX	4//3//2//1
#define LEVEL_MAX	18	// (0 à 17)

//#define DEBUG_DISP	1	// Pour affichage du debug (nb de mst, d'anims, de tirs...).

#include "psd.h"
#include "gif.h"
#include "preca.h"
#include "loader.h"
#include "scroll.h"
#include "frame.h"
#include "sprcache.h"
#include "sprites.h"
#include "sprrz.h"
#include "animspr.h"
#include "anims.h"
#include "game.h"
#include "dust.h"
#include "fire.h"
#include "mst.h"
#include "boss.h"
#include "monsters.h"
#include "font.h"
#include "menu.h"
#include "sfx.h"
#include "spritesdef.h"
#include "blkanim.h"
#include "blkbkg.h"
#include "transit2d.h"
#include "interface.h"

//=====================================

#define	ABS(x)	((x) < 0 ? -(x) : (x))
#define	SGN(x)	((x) >= 0 ? 1 : -1)
#define	MIN(x,y) ((x) < (y) ? (x) : (y))
#define NBELEM(tab) (sizeof(tab) / sizeof(tab[0]))


// Define.
#define SCR_Width 320
#define SCR_Height 224	//240

#define	GRAVITY		0x60
#define SPDY_MAX	0x800

#define	MENU_NbBkg	2

// Types de variables.
struct SGene
{
	SDL_Surface *pScreen;	// Ptr sur le buffer écran.

	SDL_Surface *pBackground;		// Ptr sur l'image de fond des menus.
//	SDL_Rect	*pBkgRect;			// Ptr sur le rect pour déplacer le blit.
//	SDL_Rect	sBkgRect;			// Rect pour déplacer le blit.
	SDL_Surface *pBkg[MENU_NbBkg];	// Les images de fond.
	struct SGIFFile	*pGif;			// Gif animé (Présentation).

	u8	*pKeysSDL;			// Pointeur sur le buffer clavier de la SDL.
	u8	pKeys[SDLK_LAST];	// Buffer clavier.

	SDL_Joystick	*pJoystick;
	u16	nJoystickState;		// 8 bits pour la croix (la fct de Hat renvoie un u8) et les boutons sur les 8 bits de poids fort.
	u8	nJoystickNoHat;		// Pas de croix ?
	u8	nJoystickNoAxes;	// Pas de stick analogique ?

//	u8	nScreenMode;		// 0 = Windowed / 1 = Full screen.
	s8	nCreditsToUse;		// Nb de crédits à utiliser dans la partie.

//	s16	pSinCos[256 + 64];	// Table contenant les sin et cos * 256, sur 256 angles.
	s16	*pSin;			// Ptrs sur les tables.
	s16	*pCos;

	s32	nFadeVal;		// 0 = Noir / 256 = Couleurs finales.

};

//=====================================

#define	CHEATCODES_NB	7
#define	CHEATCODES_TBSZMAX	10
#define	CHEATCODES_TIMER	24
struct SCheatCodes
{
	char	nTb[CHEATCODES_TBSZMAX + 1];
	u8	nIdx;		// Index case en cours.
	u8	nCnt;		// Compteur pour reset idx.
	u8	nEnter;		// Mode entrée.
	u8	nLevel;		// Level en cours pour selecteur de niveau.

	u8	nCheat;		// Les cheats à tester.
};

enum
{
	e_Cheat_WeaponSelect = 1 << 0,
	e_Cheat_WeaponUnlimited = 1 << 1,
	e_Cheat_InfiniteLives = 1 << 2,
	e_Cheat_Invulnerability = 1 << 3,
	e_Cheat_LevelSelect = 1 << 4,
	e_Cheat_LevelNext = 1 << 5,
	e_Cheat_GodMode = (1 << 6) | e_Cheat_Invulnerability | e_Cheat_InfiniteLives | e_Cheat_WeaponUnlimited | e_Cheat_WeaponSelect | e_Cheat_LevelSelect | e_Cheat_LevelNext,

};

//=====================================

// Variables générales.
extern struct SGene gVar;
extern struct SCheatCodes	gCCodes;
