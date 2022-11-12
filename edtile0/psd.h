

// Structure pour lire un PSD.
struct SPSDPicture
{
	u32	nWidth, nHeight, nNbPlanes;
	u8	*pPlanes;		// Tous les plans à la suite.
	SDL_Color	pColors[256];

};


// Prototypes.
struct SPSDPicture * PSDLoad(char *pPSDFilename);
//SDL_Surface * PSDLoadToSDLSurf(char *pPSDFilename);
struct SPSDPicture * XCFLoad(char *pXCFFilename);
//SDL_Surface * XCFLoadToSDLSurf(char *pXCFFilename);
