

// Struct pour les bo�tes d'info des monstres.
struct SInfoBox
{
	u32	nMouseOnMonster;	// On pointe un monstre �ditable ? 0 non / 1 oui.

	s32 nLnSel;				// La ligne s�lectionn�e.
	u32	nLnFirst;

	char	pMstBuf[1024];	// Le texte du monstre affich�/�dit�.
	struct SPlane	*pPln;			// Le plan en cours.
	struct SMstUnit *pMstCur;		// Le monstre en cours.

};

struct SInfoBox * InfoBox_New(void);
void InfoBox_Delete(struct SInfoBox *psInfoBox);

void InfoBox_ResetValues(struct SInfoBox *psInfoBox);
void InfoBox_MstInfoDisplay(struct SInfoBox *psInfoBox);
void InfoBox_MstInfoBoxDraw(struct SInfoBox *psInfoBox);

void InfoBox_SelMove(struct SInfoBox *psInfoBox, s32 nVal);
void InfoBox_SelChange(struct SInfoBox *psInfoBox, s32 nVal);




