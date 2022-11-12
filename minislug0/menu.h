

// Enums.
enum
{
	MENU_Null = 0,
	MENU_Main,
	MENU_Game,
	MENU_HallOfFame,
	MENU_HowToPlay,
	MENU_Quit,

};

#define	FX_Menu_Move	e_Sfx_MenuClic1
#define	FX_Menu_Clic	e_Sfx_MenuClic2

// Prototypes.
void CursorInit(void);
void CursorMove(void);
s32 CursorGetPos(void);

void MenuInit(void);

void MenuMain_Init(void);
u32 MenuMain_Main(void);
void MenuHighScores_Init(void);
u32 MenuHighScores_Main(void);
void MenuGetName_Init(void);
u32 MenuGetName_Main(void);
void MenuGameOver_Init(void);
u32 MenuGameOver_Main(void);

void Credits_NextSel(void);
void Credits_Display(s32 nCreditsNb);

s32 Scr_CheckHighSc(u32 nScorePrm);
void Scr_Load(void);
void Scr_Save(void);



// Enum touches de la config.
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
extern struct SMSCfg	gMSCfg;

u32 CfgLoad(void);


