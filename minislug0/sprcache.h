

#define	CACHE_ON	1	// 1 cache / 0 pas de cache.

enum
{
	e_Cache_Miss = 0,
	e_Cache_Hit,
};

void CacheClear(void);
u32 CacheGetMem(u32 nSprNo, u32 nSprSz, u16 **ppGfx);
void CacheClearOldSpr(void);

