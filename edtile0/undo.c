
#include "includes.h"


// Constructeur.
struct SUndo * Undo_New(void)
{
	struct SUndo *psUndo;

	if ((psUndo = (struct SUndo *)malloc(sizeof(struct SUndo))) == NULL)
	{
		fprintf(stderr, "Undo_New(): malloc failed (psUndo).\n");
		exit(1);
	}

	// Init.
	u32	i;

	for (i = 0; i < UNDO_MAX; i++)
	{
		psUndo->pMaps[i] = NULL;		// Les maps sauvegardées.
	}
	psUndo->nIndex = 0;		// Index dans la table.
	psUndo->nUndoNb = 0;		// Nb d'undo possibles.
	psUndo->nRedoIndex = 0;	// Index indiquant la position max du redo.

	return (psUndo);
}

// Destructeur.
void Undo_Delete(struct SUndo *psUndo)
{
	u32	i;

	for (i = 0; i < UNDO_MAX; i++)
	{
		if (psUndo->pMaps[i] != NULL)
		{
//-			u32	nMapLgSav, nMapHtSav;	// Lg et ht de la map.

//-			// !!! Très important !!! Comme j'ai codé comme un sagouin et que j'utilise gMap._nMapLg et Ht un peu partout (parce qu'à la base il n'y a qu'une map), on place temporairement les lg et ht de la map concernée dedans.
//-			nMapLgSav = gMap._nMapLg;
//-			nMapHtSav = gMap._nMapHt;
//-			gMap._nMapLg = _pMaps[i]->_nMapLg;
//-			gMap._nMapHt = _pMaps[i]->_nMapHt;
//-			delete _pMaps[i];
//-			gMap._nMapLg = nMapLgSav;
//-			gMap._nMapHt = nMapHtSav;

			Map_Delete(psUndo->pMaps[i]);
		}
	}

	free(psUndo);
}

// Sauvegarde de la map.
void Undo_Save(struct SUndo *psUndo)
{

	// Incrémentation index et nb d'undo.
	psUndo->nIndex = (psUndo->nIndex + 1) % UNDO_MAX;
	psUndo->nRedoIndex = psUndo->nIndex;
	if (++psUndo->nUndoNb > UNDO_MAX) psUndo->nUndoNb = UNDO_MAX;

	// Si le slot est occupé, on le vide.
	if (psUndo->pMaps[psUndo->nIndex] != NULL)
	{
//-		u32	nMapLgSav, nMapHtSav;	// Lg et ht de la map.

//-		// !!! Très important !!! Comme j'ai codé comme un sagouin et que j'utilise gMap._nMapLg et Ht un peu partout (parce qu'à la base il n'y a qu'une map), on place temporairement les lg et ht de la map concernée dedans.
//-		nMapLgSav = gMap._nMapLg;
//-		nMapHtSav = gMap._nMapHt;
//-		gMap._nMapLg = _pMaps[_nIndex]->_nMapLg;
//-		gMap._nMapHt = _pMaps[_nIndex]->_nMapHt;
//-		delete _pMaps[_nIndex];
//-		gMap._nMapLg = nMapLgSav;
//-		gMap._nMapHt = nMapHtSav;

		Map_Delete(psUndo->pMaps[psUndo->nIndex]);
	}
	// Allocation d'un nouvel objet.
	if ((psUndo->pMaps[psUndo->nIndex] = Map_New()) == NULL)
	{
		fprintf(stderr, "Undo_Save(): Map_New() failed. Action not saved.\n");
		return;
	}
	// Initialisation des valeurs.
//-	if (_pMaps[_nIndex]->UndoCopy(&gMap))
	if (Map_UndoCopy(psUndo->pMaps[psUndo->nIndex]))
	{
		fprintf(stderr, "Undo_Save(): Map_UndoCopy() failed. Action not saved.\n");
		return;
	}

	gMisc.psMap->nModified = 1;	// Flag modified : Quand on fait un Save, c'est qu'on à fait une modif !

}

// Undo (restore de la map précédente).
void Undo_Undo(struct SUndo *psUndo)
{
	if (psUndo->nUndoNb <= 1) return;

	// Index dans la table.
	psUndo->nIndex = (psUndo->nIndex - 1) % UNDO_MAX;
	psUndo->nUndoNb--;

	// Undo.
//-	gMap.UndoUndo(_pMaps[_nIndex]);
	Map_UndoUndo(psUndo->pMaps[psUndo->nIndex]);

	gMisc.psMap->nModified = 1;	// Flag modified.

}

// Redo.
void Undo_Redo(struct SUndo *psUndo)
{
	if (psUndo->nIndex == psUndo->nRedoIndex) return;

	// Incrémentation index et nb d'undo.
	psUndo->nIndex = (psUndo->nIndex + 1) % UNDO_MAX;
	if (++psUndo->nUndoNb > UNDO_MAX) psUndo->nUndoNb = UNDO_MAX;

	// Redo.
//-	gMap.UndoUndo(_pMaps[_nIndex]);
	Map_UndoUndo(psUndo->pMaps[psUndo->nIndex]);

	gMisc.psMap->nModified = 1;	// Flag modified.

}

