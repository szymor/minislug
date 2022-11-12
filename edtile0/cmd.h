

// Struct pour un monstre.
struct SMstUnit
{
	u8	*pData;

	u16	nMstNo;		// = No du bloc.
	u16	nNbBytes;	// Nb de bytes utilisés.

};

struct SMstUnit * MstUnit_New(void);
void MstUnit_Delete(struct SMstUnit *psMstUnit);

void MstUnit_InitFromCmdFile(struct SMstUnit *psMstUnit, u32 nBlockNo);			// Init depuis une page de blocs avec fichier de commandes.
void MstUnit_InitFromMst(struct SMstUnit *psMstUnit, struct SMstUnit *psInitialMst);	// Init depuis un monstre existant = depuis la map.
void MstUnit_InitFromSavedFile(struct SMstUnit *psMstUnit, u8 *pSav);			// Init depuis des datas de sauvegarde.
void MstUnit_InitFromMstTxtBuf(struct SMstUnit *psMstUnit, char *pMstBuf, u32 nBlockNo);

s32 GetBits(u8 *pData, u32 nBitStart, u32 nBitEnd, u32 nSign);
void SetBits(u8 *pData, u32 nBitStart, u32 nBitEnd, s32 nVal);



#define	CMD_PRM_SPLIT_MIN	3	// Pour tester la validité du split d'une ligne.

// Struct pour un fichier de commandes. Incluse dans les plans.
struct SCmdfile
{
	char	pCmdFilename[256];		// Nom du fichier de commandes.
	char	*pCmdfile;
	u32		nFileDate;				// Date du fichier.

};

struct SCmdfile * Cmdfile_New(void);
void Cmdfile_Delete(struct SCmdfile *psCmdfile);

void Cmdfile_CmdfileLoad(struct SCmdfile *psCmdfile, char *pFilename);
void Cmdfile_CmdfileLoadFromBuf(struct SCmdfile *psCmdfile, char *pFilename, char *pBuf, u32 nDate);
void Cmdfile_CmdFileExport(struct SCmdfile *psCmdfile);
char * Cmdfile_CmdMstExists(struct SCmdfile *psCmdfile, u32 nMstNo);
u32 Cmdfile_CmdMstSeek(struct SCmdfile *psCmdfile, u32 nMstNo, char *pMst);

//char * CmdMstExistsInFile(char *pCmd, u32 nMstNo);				// Dispo si nécessaire.
//u32 CmdMstSeekInFile(char *pCmd, u32 nMstNo, char *pMst);			// Dispo si nécessaire.

u32 CmdMstGetPrmNo(char *pMst, u32 nPrmNo, char *pPrm);
u32 CmdMstGetPrmNb(char *pMst);
u32 CmdPrmSplit(char *pPrm, char *pPrmName, u32 *pPrmBitStart, u32 *pPrmBitEnd, u32 *pPrmSign, char *pPrmArgs);
u32 CmdPrmSplit2(char *pPrm, char *pPrmName, u32 *pPrmBitStart, u32 *pPrmBitEnd, u32 *pPrmSign, s32 *pPrmDefaultValue, char *pPrmArgs);
u32 CmdMstGetArgNo(char *pArgList, u32 nArgNo, char *pArg);


