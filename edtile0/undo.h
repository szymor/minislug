
#define	UNDO_MAX	16

struct SUndo
{
	struct SMap	*pMaps[UNDO_MAX];		// Les maps sauvegardées.
	u32	nIndex;			// Index dans la table.
	u32	nUndoNb;		// Nb d'undo possibles.
	u32	nRedoIndex;		// Index max pour le redo.

};

struct SUndo * Undo_New(void);
void Undo_Delete(struct SUndo *psUndo);

void Undo_Save(struct SUndo *psUndo);
void Undo_Undo(struct SUndo *psUndo);
void Undo_Redo(struct SUndo *psUndo);

