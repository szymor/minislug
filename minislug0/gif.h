


// Structures pour le GIF, alignées à l'octet.
#pragma pack(1)
struct SGIFBlk_LogicalScrDesc
{
	u16	nLogScrWidth;
	u16	nLogScrHeight;
	u8	nGlobalClrTableSz:3,
		nSortFlag:1,
		nClrResolution:3,
		nGlobalClrTableFlag:1;
	u8	nBkgClrIdx;
	u8	nPixelAspectRatio;
};

struct SGIFBlk_AppliExtension
{
	u8	nBlkSz;
	u8	pAppliId[8];
	u8	pAppliAuthCode[3];
};

struct SGIFBlk_GraphicControlExtension
{
	u8	nBlkSz;
	u8	nTransparentClrFlag:1,
		nUserInputFlag:1,
		nDisposalMethod:3,
		nReserved:3;
	u16	nDelayTime;
	u8	nTransparentClrIdx;
};

struct SGIFBlk_ImageDescriptor
{
	u8	nBlkId;		// 0x2C.
	u16	nImgLeft;
	u16	nImgTop;
	u16	nImgWidth;
	u16	nImgHeight;
	u8	nLocalClrTableSz:3,
		nReserved:2,
		nSortFlag:1,
		nInterlaceFlag:1,
		nLocalClrTableFlag:1;
};

struct SDictionnaryRecord
{
	u16	nPrevCode;		// Index du code précédent.
	u8	nAppendChar;
};
#pragma pack()


// Structure d'une image GIF.
struct	SGIFFile
{
	u8	*pData;			// Le fichier GIF chargé.
	u32	nDataSz;		// Taille du buffer pData.
	struct SGIFBlk_LogicalScrDesc	*pLogicalScrDesc;	// Pointe dans pData.
	u8	*pCurPtr;		// Pointeur en cours dans le parcours.
	u8	*pOrgPtr;		// Pointeur sur le premier bloc suivant l'init pour loop.

	u8	pGlobalPal[768];	// Palette générale.
	u8	pLocalPal[768];		// Palette spécifique pour une frame.
	u8	*pPal;			// Palette en cours pour l'image. Pointeur sur une des palettes précédentes.

	u8	*pImg;			// Image en cours.
	u8	*pPackedBuf;	// Va pointer à la suite de pImg.

	u16	nTransparentColorIndex;	// Si < à 256, index de la couleur de transparence.
	u8	nNextDisposal;	// Ce qu'il faut faire APRES l'affichage de la frame.

	u8	nLoop;			// Interne, pour savoir s'il faut boucler ou pas. Loop si on rencontre le bloc AppliExt NETSCAPE2.0.
};

// Prototypes.
struct SGIFFile * GIF_Load(char *pFilename);
void GIF_GetNextImage(struct SGIFFile *pGif);
void GIF_Free(struct SGIFFile *pGif);

