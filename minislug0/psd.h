

// Structure pour lire un PSD.
struct SPSDPicture
{
	u32	nWidth, nHeight, nNbPlanes;
	u8	*pPlanes;		// Tous les plans à la suite.
	SDL_Color	pColors[256];

};


struct SPSDPicture * PSDLoad(char *pPSDFilename);
SDL_Surface * PSDLoadToSDLSurf(char *pPSDFilename);


