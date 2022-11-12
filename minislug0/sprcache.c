

#include "includes.h"

#define	CACHE_DEBUG_INFO	0		// Mettre � 0 pour ne pas afficher les infos de debug / 1 pour affichage.

#define	CACHE_BIG_BLK_IDX	1//0		// 1 : avec gnCacheFreeLastBig.
// Utile dans le cas de plusieurs allocs de blocs multiples dans la m�me frame.
// Ca acc�l�re les recherche de gros blocs, mais �a fragmente un peu plus.
	// => A tester en conditions r�elles.

extern	u16	*gpSprFlipBuf;	// Buffer pour cr�er les images flipp�es.


#define	CACHE_BLK_SZ8	(32*32 * 2 * 2)		// Taille d'un bloc en bytes.
#define	CACHE_BLK_SZ16	(CACHE_BLK_SZ8 / 2)	// Taille d'un bloc en words.
#define	CACHE_BLK_NB	256//128//64//128//16//128		// Nb de blocs maxi dans le cache.

u16	gpCacheData[CACHE_BLK_NB * CACHE_BLK_SZ16];	// Les blocs de datas dans la cache.
u8	gpCacheFree[CACHE_BLK_NB];					// 0 = pos libre / x = pos occup�e et nombre de cases � sauter pour la prochaine position (pleine ou vide).
// ou faire un hachage pour taper directement � la pos x ?
u32	gnCacheFreeLast;		// Pour acc�l�rer la recherche des cases vides.
#if CACHE_BIG_BLK_IDX == 1
u32	gnCacheFreeLastBig;		// Pour acc�l�rer la recherche des cases vides (gros blocs).
#endif

u32	gpLRU[CACHE_BLK_NB];	// Liste des sprites dans le cache. Buffer circulaire.
u32	gnLRUNb;				// Nb de sprites dans la liste.
u32	gnLRUIndex;				// Pos de d�part du buffer circulaire.

struct SSprCache
{
	s16	nPos;		// -1 = pas dans le cache / sinon index dans le cache.
	u16	nBlocksNb;	// Nb de blocs utilis�s.
	u16	nLRUPos;	// Pos dans le LRU.
	u16	nAge;		// Anciennet� dans la liste.
};
struct SSprCache	gpSprUse[e_Spr_NEXT * 4];	// Normal / flip y / flip x / flip xy.

//=============================================================================

// On met � jour la LRU.
// nSprNo devient le plus r�cent sprite utilis�.
void LRUUpdate(u32 nSprNo)
{
	u32	i;

// todo : on doit pouvoir simplifier tous les (gnLRUIndex + i) et utiliser directement le n� de la case.


if (nSprNo != gpLRU[gpSprUse[nSprNo].nLRUPos]) { fprintf(stderr, "LRUUpdate: PROBLEM!!! nSprNo=%d / gpSprUse[nSprNo].nLRUPos=%d / gpLRU[gpSprUse[nSprNo].nLRUPos]=%d\n", (int)nSprNo, (int)gpSprUse[nSprNo].nLRUPos, (int)gpLRU[gpSprUse[nSprNo].nLRUPos]); return; }	// debug.


	i = gpSprUse[nSprNo].nLRUPos;
	i = (i - gnLRUIndex) & (CACHE_BLK_NB - 1);

	if (i < gnLRUNb / 2)
	{
		// Dans les plus vieux. > Le plus vieux sprite de la liste prend la position du sprite a updater, et le sprite a updater passe en t�te en devenant le plus r�cent.
		// 1 - "Swap" avec le dernier. (En fait on n'a pas besoin de copier le nouveau � la fin, puisqu'on va avancer la position de d�part).
		gpLRU[(gnLRUIndex + i) & (CACHE_BLK_NB - 1)] = gpLRU[gnLRUIndex];
		gpSprUse[gpLRU[gnLRUIndex]].nLRUPos = (gnLRUIndex + i) & (CACHE_BLK_NB - 1);	// Nouvel index ancien sprite swapp� (valeur dans la case originale encore valide).
		// 2 - Ajout du spr en d�but de liste.
		gpLRU[(gnLRUIndex + gnLRUNb) & (CACHE_BLK_NB - 1)] = nSprNo;
		gpSprUse[nSprNo].nLRUPos = (gnLRUIndex + gnLRUNb) & (CACHE_BLK_NB - 1);			// Nouvel index du sprite qu'on replace en t�te.
		gpSprUse[nSprNo].nAge = 0;			// RAZ age.
		//
		gnLRUIndex = (gnLRUIndex + 1) & (CACHE_BLK_NB - 1);	// ...et on d�cale la liste dans le buffer circulaire.
	}
	else
	{
		// Dans les plus r�cents.
		// Swap avec le premier.
		gpLRU[(gnLRUIndex + i) & (CACHE_BLK_NB - 1)] = gpLRU[(gnLRUIndex + gnLRUNb - 1) & (CACHE_BLK_NB - 1)];
		gpSprUse[gpLRU[(gnLRUIndex + i) & (CACHE_BLK_NB - 1)]].nLRUPos = (gnLRUIndex + i) & (CACHE_BLK_NB - 1);		// Nouvel index du sprite qu'on swappe.
		//
		gpLRU[(gnLRUIndex + gnLRUNb - 1) & (CACHE_BLK_NB - 1)] = nSprNo;
		gpSprUse[nSprNo].nLRUPos = (gnLRUIndex + gnLRUNb - 1) & (CACHE_BLK_NB - 1);			// Nouvel index du sprite qu'on replace en t�te.
		gpSprUse[nSprNo].nAge = 0;			// RAZ age.
	}

}

// Ajoute un �l�ment � la liste (donc au bout).
void LRUNew(u32 nSprNo)
{
	gpLRU[(gnLRUIndex + gnLRUNb) & (CACHE_BLK_NB - 1)] = nSprNo;
	gpSprUse[nSprNo].nLRUPos = (gnLRUIndex + gnLRUNb) & (CACHE_BLK_NB - 1);
	gpSprUse[nSprNo].nAge = 0;			// RAZ age.
	gnLRUNb++;
//printf("gnLRUNb=%d\n", (int)gnLRUNb);		// normalement, �a ne doit pas d�border (nb max dans la liste est <= au nombre de spr max, puisque tous prennent au moins 1 bloc).
}

// Supprime le premier �l�ment de la liste.
void LRUDeleteFirst(void)
{
	gnLRUNb--;
	gnLRUIndex = (gnLRUIndex + 1) & (CACHE_BLK_NB - 1);
}

//=============================================================================

// Remise � z�ro du cache.
void CacheClear(void)
{
	u32	i;

	for (i = 0; i < CACHE_BLK_NB; i++) gpCacheFree[i] = 0;
	gnCacheFreeLast = 0;
#if CACHE_BIG_BLK_IDX == 1
	gnCacheFreeLastBig = 0;
#endif
	gnLRUNb = 0;		// Nb de sprites dans la liste.
	gnLRUIndex = 0;		// Pos de d�part du buffer circulaire.
	for (i = 0; i < e_Spr_NEXT * 4; i++) gpSprUse[i].nPos = -1;

}

// Renvoie la position de la premi�re case � partir de laquelle x blocs sont libres.
// -1 si pas d'espace assez grand.
s32 CacheGetFreeBlocks(u32 nNbBlocsReq)
{
	u32	i, j;

#if CACHE_DEBUG_INFO == 1
u32	k=0;
printf("gpCacheFree:");
for (i = 0; i < CACHE_BLK_NB; i++) printf("%d", (int)gpCacheFree[i]);
printf("\n");
#endif

////	for (i = gnCacheFreeLast; i < CACHE_BLK_NB; i++)
//	for (i = gnCacheFreeLast; i < CACHE_BLK_NB; )//i++)
#if CACHE_BIG_BLK_IDX == 1
	for (i = (nNbBlocsReq == 1 ? gnCacheFreeLast : gnCacheFreeLastBig); i < CACHE_BLK_NB; )
#else
	for (i = gnCacheFreeLast; i < CACHE_BLK_NB; )//i++)
#endif
	{
		// Case vide ?
		if (gpCacheFree[i] == 0)
		{
			// Oui, y-a-t'il assez de cases ?
			for (j = 0; (j < nNbBlocsReq) && (i + j < CACHE_BLK_NB); j++)
			{
#if CACHE_DEBUG_INFO == 1
k++;
#endif
				if (gpCacheFree[i + j]) break;
			}
//			if (j == nNbBlocsReq) return (i);
			if (j == nNbBlocsReq)
			{
#if CACHE_DEBUG_INFO == 1
printf("CacheGetFreeBlocks / found - loops: %d\n", (int)k);
#endif
				return (i);
			}
			i += j;		// Non, tant qu'� faire on saute par dessus l'espace vide test�.
		}
		//else			// Pas besoin du else, la recherche pr�c�dente s'arr�te sur une case pleine ! Donc autant skipper tout de suite.
			i += gpCacheFree[i];	// Skippe le nombre de cases pleines.
#if CACHE_DEBUG_INFO == 1
k++;
#endif
	}
#if CACHE_DEBUG_INFO == 1
printf("CacheGetFreeBlocks / not found - loops: %d\n", (int)k);
#endif
	return (-1);
}

// Supprime le plus vieil �l�ment du cache (enfin, le premier de la LRU).
void CacheDeleteFirst(void)
{
	u32	nRem;
	u32	i;

	if (gnLRUNb == 0) return;
	nRem = gpLRU[gnLRUIndex];	// On r�cup�re le n� du sprite � retirer.
	LRUDeleteFirst();			// Suppression de la LRU.
	// Lib�re les cases de gpCacheFree
	for (i = gpSprUse[nRem].nPos; i < gpSprUse[nRem].nPos + gpSprUse[nRem].nBlocksNb; i++) gpCacheFree[i] = 0;
	if (gpSprUse[nRem].nPos < gnCacheFreeLast) gnCacheFreeLast = gpSprUse[nRem].nPos;	// Pour acc�l�rer la recherche de cases vides.
#if CACHE_BIG_BLK_IDX == 1
	if (gpSprUse[nRem].nPos < gnCacheFreeLastBig) gnCacheFreeLastBig = gpSprUse[nRem].nPos;	// Pour acc�l�rer la recherche de cases vides.
#endif
	gpSprUse[nRem].nPos = -1;	// RAZ index.

}

// Demande un espace m�moire au cache.
u32 CacheGetMem(u32 nSprNo, u32 nSprSz, u16 **ppGfx)
{
	u32	nNbBlocsReq;
	s32	nPos;
	u32	i;

	nSprNo = ((nSprNo & ~(SPR_Flip_X | SPR_Flip_Y | SPR_Flag_HitPal)) * 4) + ((nSprNo >> 30) & 3);	// (sprno * 4) + index [0-3] en fct du flip.

	// Sprite d�j� dans le cache ?
	if (gpSprUse[nSprNo].nPos != -1)
	{
		// Oui.
#if CACHE_DEBUG_INFO == 1
//printf("spr #%d / cache hit\n", (int)nSprNo);
printf("spr #%d/%d / cache hit\n", (int)nSprNo>>2, (int)nSprNo&3);
#endif
		LRUUpdate(nSprNo);
		*ppGfx = &gpCacheData[gpSprUse[nSprNo].nPos * CACHE_BLK_SZ16];
		return (e_Cache_Hit);
	}

	// Non.
#if CACHE_DEBUG_INFO == 1
//printf("spr #%d / cache miss\n", (int)nSprNo);
//printf("spr #%d / cache miss", (int)nSprNo);
printf("spr #%d/%d / cache miss", (int)nSprNo>>2, (int)nSprNo&3);
#endif

	// Nb de blocs requis.
	nNbBlocsReq = (nSprSz * 2 * 2) / CACHE_BLK_SZ8;	// nSprSz * 2 => Pour taille du gfx + taille du masque / * 2 pour taille u16.
	if ((nSprSz * 2) & (CACHE_BLK_SZ8 - 1)) nNbBlocsReq++;

#if CACHE_DEBUG_INFO == 1
printf(" / sprsz: %d / nb blk req: %d\n", (int)nSprSz, (int)nNbBlocsReq);
#endif

	// On va chercher uns position libre.
	if ((nPos = CacheGetFreeBlocks(nNbBlocsReq)) < 0)
	{
		fprintf(stderr, "Cache alarm: cache full!\n");
		// Plus de place, tant pis, on va tracer dans le buffer en direct.
		*ppGfx = gpSprFlipBuf;
		return (e_Cache_Miss);
	}

	LRUNew(nSprNo);
	gpSprUse[nSprNo].nPos = nPos;
	gpSprUse[nSprNo].nBlocksNb = nNbBlocsReq;
	// Marque les blocs utilis�s.
//	for (i = 0; i < nNbBlocsReq; i++) gpCacheFree[nPos + i] = 1;
	for (i = 0; i < nNbBlocsReq; i++) gpCacheFree[nPos + i] = nNbBlocsReq - i;	// Marquage type 6 5 4 3 2 1.
/*
	if (nNbBlocsReq == 1) gnCacheFreeLast = nPos + 1;//nNbBlocsReq;	// La prochaine recherche de blocs vides commencera � la position suivante (update seulement si 1 case demand�e, sinon on peut sauter des cases vides et on y reviendra jamais).
#if CACHE_BIG_BLK_IDX == 1
	gnCacheFreeLastBig = nPos + nNbBlocsReq;
#endif
*/
	// Mini optim : En plus, on ajoute l'offset de la position suivante (0 ou x), pour possiblement gagner une it�ration lors des recherches.
	if (nNbBlocsReq == 1) gnCacheFreeLast = nPos + 1 + gpCacheFree[nPos + 1];	// La prochaine recherche de blocs vides commencera � la position suivante (update seulement si 1 case demand�e, sinon on peut sauter des cases vides et on y reviendra jamais).
#if CACHE_BIG_BLK_IDX == 1
	gnCacheFreeLastBig = nPos + nNbBlocsReq + gpCacheFree[nPos + nNbBlocsReq];
#endif

	*ppGfx = &gpCacheData[nPos * CACHE_BLK_SZ16];

	return (e_Cache_Miss);
}

//=============================================================================

#define	CACHE_AGE_MAX	1//2//4//8//32//8//32//16//32		// Age limite dans le buffer.
// Il y a �normement de sprites dans les anims et ils restent peu de temps. Il faut donc les discarder tr�s vite.
// Le but �tant simplement de ne pas d�packer et g�n�rer un masque pour des sprites x frames d'affil�e, voire x fois par frame (balles de mitrailleuse).

// Nettoyage des sprites trop anciens.
void CacheClearOldSpr(void)
{
	u32	i;
	u32	nNb, nIndex, nTmp;

#if CACHE_DEBUG_INFO == 1
printf ("CacheClearOldSpr: ");
#endif

/*
v1, seulement sur l'age limite.
*/
	nNb = gnLRUNb;			// On r�cup�re les valeurs qui peuvent changer au cours du for.
	nIndex = gnLRUIndex;
	for(i = 0; i < nNb; i++)
	{
		// Incr�mentation de l'age.
		if (++gpSprUse[gpLRU[(nIndex + i) & (CACHE_BLK_NB - 1)]].nAge > CACHE_AGE_MAX)
		{
			// Sprite � supprimer. On swappe l'�l�ment en cours (� supprimer) avec le premier de la liste on supprime le premier �l�ment.
#if CACHE_DEBUG_INFO == 1
			printf("(#%d/age %d/pos %d) ", (int)gpLRU[(nIndex + i) & (CACHE_BLK_NB - 1)], (int)gpSprUse[gpLRU[(nIndex + i) & (CACHE_BLK_NB - 1)]].nAge, (int)gpSprUse[gpLRU[(nIndex + i) & (CACHE_BLK_NB - 1)]].nPos);
#endif

			nTmp = gpLRU[(nIndex + i) & (CACHE_BLK_NB - 1)];
			gpLRU[(nIndex + i) & (CACHE_BLK_NB - 1)] = gpLRU[gnLRUIndex];
gpSprUse[gpLRU[gnLRUIndex]].nLRUPos = (nIndex + i) & (CACHE_BLK_NB - 1);	// Nouvel index ancien sprite swapp� (valeur dans la case originale encore valide).
			gpLRU[gnLRUIndex] = nTmp;
			CacheDeleteFirst();
		}
	}

#if CACHE_DEBUG_INFO == 1
printf ("\n");
#endif

}


