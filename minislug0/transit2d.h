

enum
{
	e_Transit_Mission = 0,
	e_Transit_Menu,
	e_Transit_InterLvl_H,
	e_Transit_InterLvl_V,
	e_Transit_HowToPlay,

	e_Transit_Max
};

// Prototypes.
void Transit2D_InitOpening(u32 nTransitNo);
void Transit2D_InitClosing(u32 nTransitNo);
void Transit2D_Manage(void);

void Transit2D_Reset(void);
u32 Transit2D_CheckEnd(void);

