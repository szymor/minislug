

// Struct pour lecture/sauvegarde d'un monstre.
#pragma pack(1)
struct SMst0
{
	u32	nPosX, nPosY;
	u16	nMstNo;		// = No du bloc.
	u16	nNbBytes;	// Nb de bytes utilisés.
};
#pragma pack()


// Structure pour fichier de niveau.
struct SFile
{
	char	pLevFilename[256];

};

extern	struct SFile	gFile;

void File_Save(struct SFile *psFile);
void File_Load(struct SFile *psFile);

void File_FilenameSet(struct SFile *psFile, char *pStr);
char * File_FilenameGet(struct SFile *psFile);



enum
{
	e_FileSection_Map = 0,
	e_FileSection_Plane,
	e_FileSection_Cmd,
	e_FileSection_Misc,
};

bool fexist(char *pFilename);

