
#include "includes.h"

// Constructeur.
struct SKeyboard * Kbd_New(void)
{
	struct SKeyboard *psKbd;

	if ((psKbd = (struct SKeyboard *)malloc(sizeof(struct SKeyboard))) == NULL)
	{
		fprintf(stderr, "Kbd_New(): malloc failed (psKbd).\n");
		exit(1);
	}

	// Init du pointeur pKeys.
	psKbd->pKeys = SDL_GetKeyState(NULL);

	return (psKbd);
}

// Destructeur.
void Kbd_Delete(struct SKeyboard *psKbd)
{

	free(psKbd);
}

// Test le toggle des plans avec soit 0-9, soit KP0-9.
void Kbd_PlaneToggleCheck(struct SKeyboard *psKbd, u32 nStartKey, u32 *pnUpdateDisp)
{
/*
	for (u32 i = nStartKey; i <= nStartKey + 9; i++)
	{
		if (psKbd->pKeys[i])
		{
			gMap.PlaneToggle(i - nStartKey);
			psKbd->pKeys[i] = 0;
			nUpdateDisp |= 3;
			gInfoBox.nMouseOnMonster = 0;	// RAZ flag.
		}
	}
*/

	struct SMap		*psMap = gMisc.psMap;
	struct SMouse	*psMouse = gMisc.psMouse;
	struct SInfoBox	*psInfoBox = gMisc.psInfoBox;
	u32	i;

	for (i = 0; i <= 9 && i < psMap->nPlanesNb; i++)
	{
		if (psKbd->pKeys[i + nStartKey])
		{
			if (psKbd->pKeys[SDLK_LSHIFT] || psKbd->pKeys[SDLK_RSHIFT])
			{
				// Avec shift, on active et amène le plan.
				if (psMap->nActivePlane != i)
				{
					// Active le plan.
					Mouse_BrushRelease(psMouse);
					psMap->nActivePlane = i;
				}
				// Amène le plan dans les plans visibles.
				psMap->nFirstWindow = i;
				// Position de la souris.
				Mouse_LocationGet(psMouse);
				*pnUpdateDisp |= e_Update_All;
			}
			else
			{
				// Sans shift, on grise/dégrise le plan.
				Map_PlaneToggle(psMap, i);
				*pnUpdateDisp |= 3;
				psInfoBox->nMouseOnMonster = 0;	// RAZ flag.
			}
			psKbd->pKeys[i + nStartKey] = 0;
		}
	}

}

// Gestion du clavier.
void Kbd_Manage(struct SKeyboard *psKbd)
{
	struct SMap		*psMap = gMisc.psMap;
	struct SMouse	*psMouse = gMisc.psMouse;
	struct SInfoBox	*psInfoBox = gMisc.psInfoBox;
	u32	nUpdateDisp = 0;
	//u32	i;

	// Esc, libère la brosse.
	if (psKbd->pKeys[SDLK_ESCAPE])
	{
		Mouse_BrushRelease(psMouse);
		if (psMouse->pWhere != NULL)
		{
			switch (psMouse->pWhere->nWhere)
			{
			case e_MouseLoc_Map:
				nUpdateDisp |= e_Update_Map;
				break;

			case e_MouseLoc_Planes:
				nUpdateDisp |= e_Update_Planes;
				break;
			}
		}
		else
		{
			// Refresh total si la souris est dans le vide, pour être sûr de clearer l'image de la brosse.
			nUpdateDisp |= e_Update_All;
		}
		psKbd->pKeys[SDLK_ESCAPE] = 0;
	}

	// Flèches de direction.
	if (psKbd->pKeys[SDLK_UP] || psKbd->pKeys[SDLK_DOWN] || psKbd->pKeys[SDLK_LEFT] || psKbd->pKeys[SDLK_RIGHT])
	{
		if (psMouse->pWhere != NULL)
		{
			switch (psMouse->pWhere->nWhere)
			{
			case e_MouseLoc_Map:
				// Déplacement de la map.
				if (psKbd->pKeys[SDLK_UP]) Map_MapMoveXY(psMap, 0, -1);
				if (psKbd->pKeys[SDLK_DOWN]) Map_MapMoveXY(psMap, 0, 1);
				if (psKbd->pKeys[SDLK_LEFT]) Map_MapMoveXY(psMap, -1, 0);
				if (psKbd->pKeys[SDLK_RIGHT]) Map_MapMoveXY(psMap, 1, 0);
				nUpdateDisp |= e_Update_Map;
				break;

			case e_MouseLoc_Planes:
				{
				// Déplacement d'une planche dans sa fenêtre.
				u32	nPlaneNo = psMouse->pWhere->nVar;		// Plan sur lequel est la souris.
				if (psKbd->pKeys[SDLK_UP]) Map_PlaneWndMoveXY(psMap, nPlaneNo, 0, -1);
				if (psKbd->pKeys[SDLK_DOWN]) Map_PlaneWndMoveXY(psMap, nPlaneNo, 0, 1);
				if (psKbd->pKeys[SDLK_LEFT]) Map_PlaneWndMoveXY(psMap, nPlaneNo, -1, 0);
				if (psKbd->pKeys[SDLK_RIGHT]) Map_PlaneWndMoveXY(psMap, nPlaneNo, 1, 0);
				nUpdateDisp |= e_Update_Planes;
				}
				break;

			case e_MouseLoc_Visu:
				// Déplacement dans la visu.
				Visu_ScrollPosition(psMap->psVisu, psKbd->pKeys);
				break;
			}
		}
	}

	// Changement des valeurs du code d'un bloc.
	if (psKbd->pKeys[SDLK_PAGEUP] || psKbd->pKeys[SDLK_PAGEDOWN])
	{
		if (psMouse->pWhere != NULL)
		if (psMouse->pWhere->nWhere == e_MouseLoc_Planes || psMouse->pWhere->nWhere == e_MouseLoc_Map)
		// La souris pointe bien un bloc ?
		if (psMouse->nBlkPosX != -1)
		{
			struct SPlane	*pPln;
			u32	nPlaneNo;
			u32	nBlockNo;

			nPlaneNo = (psMouse->pWhere->nWhere == e_MouseLoc_Planes ? psMouse->pWhere->nVar : psMap->nActivePlane);
			// Le plan a-t'il des codes ?
			if (Map_PlaneHasCodes(psMap, nPlaneNo))
			{
				pPln = psMap->ppsPlanes[nPlaneNo];
				// Les codes sont ils affichés ?
				if (pPln->nFlags & e_FlgPln_Codes)
				{
					// Les codes sont ils affichés ?
					if (pPln->nFlags & e_FlgPln_Codes)
					{
						s8	nInc = (psKbd->pKeys[SDLK_PAGEUP] ? 1 : -1);
						if (psKbd->pKeys[SDLK_LSHIFT] || psKbd->pKeys[SDLK_RSHIFT]) nInc <<= 4;
						nBlockNo = (psMouse->pWhere->nWhere == e_MouseLoc_Planes ?
							(psMouse->nBlkPosY * (pPln->pGfx->w / 16)) + psMouse->nBlkPosX :
							*(pPln->pPlane + (psMouse->nBlkPosY * psMap->nMapLg) + psMouse->nBlkPosX) );
						Map_PlaneCodeAdd(psMap, nPlaneNo, nBlockNo, nInc);
						//
						Undo_Save(gMisc.psUndo);	// Sauvegarde de la map.
						nUpdateDisp |= e_Update_Planes | e_Update_Map;
					}
				}
			}
		}
		psKbd->pKeys[SDLK_PAGEUP] = psKbd->pKeys[SDLK_PAGEDOWN] = 0;
	}

	// Défilement des planches.
	if (psKbd->pKeys[SDLK_KP_PLUS] || psKbd->pKeys[SDLK_KP_MINUS])
	{
		if (psKbd->pKeys[SDLK_KP_PLUS])
		{
			Map_FirstWndMove(psMap, 1);
			psKbd->pKeys[SDLK_KP_PLUS] = 0;
		}
		if (psKbd->pKeys[SDLK_KP_MINUS])
		{
			Map_FirstWndMove(psMap, -1);
			psKbd->pKeys[SDLK_KP_MINUS] = 0;
		}
		nUpdateDisp |= e_Update_Planes;
	}

	// Toggle des plans.
	Kbd_PlaneToggleCheck(psKbd, SDLK_0, &nUpdateDisp);
	Kbd_PlaneToggleCheck(psKbd, SDLK_KP0, &nUpdateDisp);

	// A : Toggle alpha.
	if (psKbd->pKeys[SDLK_a])
	{
		Map_PlaneAlphaToggle(psMap);
		nUpdateDisp |= e_Update_All;
		psKbd->pKeys[SDLK_a] = 0;
	}

	// C : Toggle codes.
	if (psKbd->pKeys[SDLK_c])
	{
		Map_PlaneCodesToggle(psMap);
		nUpdateDisp |= e_Update_All;
		psKbd->pKeys[SDLK_c] = 0;
	}

	// T : Toggle transparence.
	if (psKbd->pKeys[SDLK_t])
	{
		Misc_TransparencyToggle(&gMisc);
		nUpdateDisp |= e_Update_Map;
		psKbd->pKeys[SDLK_t] = 0;
	}

	// V : Toggle visualisation.
	if (psKbd->pKeys[SDLK_v])
	{
		Map_VisuToggle(psMap);
		nUpdateDisp |= e_Update_All;
		psKbd->pKeys[SDLK_v] = 0;
	}

	// U : Undo.
	if (psKbd->pKeys[SDLK_u])
	{
		Undo_Undo(gMisc.psUndo);
		nUpdateDisp |= e_Update_Map | e_Update_Planes;
		psKbd->pKeys[SDLK_u] = 0;
	}

	// R : Redo.
	if (psKbd->pKeys[SDLK_r])
	{
		Undo_Redo(gMisc.psUndo);
		nUpdateDisp |= e_Update_Map | e_Update_Planes;
		psKbd->pKeys[SDLK_r] = 0;
	}

	// F1 : Help.
	if (psKbd->pKeys[SDLK_F1])
	{
		static	const char	*pHelpTitle = "EdTile v.2.00 by 17o2!!";
		static	const char	*pHelpStr = "\
F1: Help.\n\
F2: Toggle map full screen (map + blocks / map only).\n\
F3: Insert column / Shift+F3: Delete column.\n\
F4: Insert line / Shift+F4: Delete line.\n\
F10: Save file.\n\
F12: Change window resolution.\n\
\n\
0-9(KP0-KP9): Toggle plane x's display / Shift + key: Activate plane x\n\
KP+/KP-: Display next/previous plane.\n\
\n\
a: Toggle alpha layer display (if any) on active plane.\n\
c: Toggle blocks codes display (if any) on active plane.\n\
PgUp/PgDown with codes displayed:\n\
    +/- 1 on mouse pointed block code (Shift: +/- 0x10).\n\
t: Toggle transparency (all planes).\n\
v: Toggle visualisation.\n\
u: Undo.\n\
r: Redo.\n\
x: Quit.\n\
\n\
Return (mouse on monster): Edit monster parameters.\n\
> Arrows: Up/Down to select line, Left/Right to change value.\n\
> LR: -+1 / Shift+LR: -+10 / Ctrl+LR: -+100 / Shift+Ctrl+LR: -+1000.\n\
> Return to save changes, Esc to cancel.\n\
\n\
LMB: Activate plane. Grab/poke brush.\n\
RMB: Cut/release brush.\n\
\n\
* Command line options:\n\
-s:width:height : Set map size (in blocks) upon creation.\n\
    This parameter has to be specified BEFORE adding planes.\n\
-a:filename : Add plane.\n\
-i:position:filename : Insert plane at position.\n\
-r:plane_number : Remove plane x.\n\
-b:plane_number:<+||-> : Add/remove blocks codes on plane x.\n\
-c:plane_number:cmd_filename : Add command file on plane x.\n\
-t:R:G:B : Set transparency color (RGB values: 0-255).\n\
-v:width:height : Use custom visualisation size (in pixels).\n\
\n\
";
		psKbd->pKeys[SDLK_F1] = 0;
#ifdef WIN32
		MessageBox(NULL, pHelpStr, pHelpTitle, MB_OK);
#endif
#ifdef __LINUX__
//		// Avec printf.
//		printf("*** %s ***\n", pHelpTitle);
//		printf(pHelpStr);

		// Avec xmessage.
		static	const char	*pCmdStr = "xmessage -center \"*** %s ***\n\n%s\" -button OK";
		char *pCmdLn;

		if ((pCmdLn = (char *)malloc(strlen(pCmdStr) + strlen(pHelpTitle) + strlen(pHelpStr))) != NULL)
		{
			sprintf(pCmdLn, pCmdStr, pHelpTitle, pHelpStr);
			system(pCmdLn);
			free(pCmdLn);
			nUpdateDisp |= e_Update_All;
		}
#endif
	}

	// F2 : Toggle du map full screen.
	if (psKbd->pKeys[SDLK_F2])
	{
		if (psMouse->nState != e_MouseState_GrabStarted)		// Pas pendant une capture de brosse !
		{
			Misc_FullScreenToggle(&gMisc);
			nUpdateDisp |= 3;
		}
		psKbd->pKeys[SDLK_F2] = 0;
	}

	// F3 : Insert/delete column.
	if (psKbd->pKeys[SDLK_F3])
	{
		if (psKbd->pKeys[SDLK_RSHIFT] || psKbd->pKeys[SDLK_LSHIFT])
		{
			Map_MapColumnDelete(psMap);
		}
		else
		{
			Map_MapColumnInsert(psMap);
		}
		psKbd->pKeys[SDLK_F3] = 0;
		Undo_Save(gMisc.psUndo);	// Sauvegarde de la map.
		nUpdateDisp |= 3;
	}

	// F4 : Insert/delete line.
	if (psKbd->pKeys[SDLK_F4])
	{
		if (psKbd->pKeys[SDLK_RSHIFT] || psKbd->pKeys[SDLK_LSHIFT])
		{
			Map_MapLineDelete(psMap);
		}
		else
		{
			Map_MapLineInsert(psMap);
		}
		psKbd->pKeys[SDLK_F4] = 0;
		Undo_Save(gMisc.psUndo);	// Sauvegarde de la map.
		nUpdateDisp |= 3;
	}

	// F10 : Save file.
	if (psKbd->pKeys[SDLK_F10])
	{
		File_Save(&gFile);
		psKbd->pKeys[SDLK_F10] = 0;
		nUpdateDisp |= 3;
	}

	// F12 : Change resolution.
	if (psKbd->pKeys[SDLK_F12])
	{
//fprintf(stderr, "F12: val = %d\n", gMisc.nVideoIdx);
		Misc_SetVideoMode(&gMisc, 1);
		psKbd->pKeys[SDLK_F12] = 0;
		nUpdateDisp |= e_Update_All;
	}

	// Return : Edition d'un monstre.
	if (psKbd->pKeys[SDLK_RETURN])
	{
		if (psInfoBox->nMouseOnMonster == 1) psInfoBox->nMouseOnMonster = 2;
		psKbd->pKeys[SDLK_RETURN] = 0;
	}

	gMisc.nUpdateDisp |= nUpdateDisp;

}

// Gestion du clavier pour l'info box des monstres.
u32 Kbd_MstInfoBoxKbManage(struct SKeyboard *psKbd)
{
	struct SInfoBox	*psInfoBox = gMisc.psInfoBox;
	u32	nUpdateDisp = 0;
	s32	nMultiplier;

	// Flèches de direction.
	if (psKbd->pKeys[SDLK_UP] || psKbd->pKeys[SDLK_DOWN] ||
		psKbd->pKeys[SDLK_PAGEUP] || psKbd->pKeys[SDLK_PAGEDOWN] ||
		psKbd->pKeys[SDLK_LEFT] || psKbd->pKeys[SDLK_RIGHT])
	{
		// Up / Down : Changement de ligne.
		if (psKbd->pKeys[SDLK_UP])
		{
			InfoBox_SelMove(psInfoBox, -1);
			psKbd->pKeys[SDLK_UP] = 0;
		}
		if (psKbd->pKeys[SDLK_DOWN])
		{
			InfoBox_SelMove(psInfoBox, 1);
			psKbd->pKeys[SDLK_DOWN] = 0;
		}
		// Page Up / Page Down : Changement de ligne.
		if (psKbd->pKeys[SDLK_PAGEUP])
		{
			InfoBox_SelMove(psInfoBox, -128);
			psKbd->pKeys[SDLK_PAGEUP] = 0;
		}
		if (psKbd->pKeys[SDLK_PAGEDOWN])
		{
			InfoBox_SelMove(psInfoBox, 128);
			psKbd->pKeys[SDLK_PAGEDOWN] = 0;
		}
		// Left / Right : Changement de valeur.
		nMultiplier = 1;	// Base = +- 1 / Shift +- 10 / Ctrl +- 100 / Shift+Ctrl +- 1000.
		if (psKbd->pKeys[SDLK_LSHIFT] || psKbd->pKeys[SDLK_RSHIFT]) nMultiplier = 10;
		if (psKbd->pKeys[SDLK_LCTRL] || psKbd->pKeys[SDLK_RCTRL]) nMultiplier *= 100;
		//
		if (psKbd->pKeys[SDLK_LEFT])
		{
			InfoBox_SelChange(psInfoBox, -1 * nMultiplier);
			psKbd->pKeys[SDLK_LEFT] = 0;
		}
		if (psKbd->pKeys[SDLK_RIGHT])
		{
			InfoBox_SelChange(psInfoBox, 1 * nMultiplier);
			psKbd->pKeys[SDLK_RIGHT] = 0;
		}

		nUpdateDisp |= 1;
	}

	// Esc : Annule les changements et quitte.
	if (psKbd->pKeys[SDLK_ESCAPE])
	{
		psKbd->pKeys[SDLK_ESCAPE] = 0;
		return (e_KB_MST_Cancel);
	}

	// Return : Valide les changements et quitte.
	if (psKbd->pKeys[SDLK_RETURN])
	{
		psKbd->pKeys[SDLK_RETURN] = 0;
		return (e_KB_MST_Ok);
	}

	gMisc.nUpdateDisp |= nUpdateDisp;
	return (e_KB_MST_Null);
}

