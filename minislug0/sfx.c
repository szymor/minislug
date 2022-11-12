
#include "includes.h"
#include "StSoundLibrary.h"


struct SSfxGene
{
	u8	nInit;		// Son initialisé (1) ou pas (0).

	SDL_AudioSpec	sAudioSpec;
    SDL_AudioCVT	pCvt[e_Sfx_LAST];

	YMMUSIC *ppMusic[e_YmMusic_MAX];
	s32	nMusicNo;	// e_YmMusic_NoMusic (-1) = Pas de musique.
};
struct SSfxGene	gSfx;

#define SFX_MAX_SOUNDS	4//3
struct SSample
{
	u8	*pData;
	u32	nDPos;
	u32	nDLen;
	u8	nPrio;	// Priorité du son en cours.
	u8	nSfxNo;
} gpSounds[SFX_MAX_SOUNDS];



// Démarre/redémarre une musique YM.
void Music_Start(s32 nMusicNo, u32 nForceRestart)
{
	if (!gSfx.nInit) return;

	if ((u32)nMusicNo >= e_YmMusic_MAX)
	{
		gSfx.nMusicNo = e_YmMusic_NoMusic;
		return;
	}

	if (gSfx.nMusicNo != nMusicNo || nForceRestart)
	{
		gSfx.nMusicNo = e_YmMusic_NoMusic;
		ymMusicRestart(gSfx.ppMusic[nMusicNo]);
		gSfx.nMusicNo = nMusicNo;
	}
}

// Renvoie le n° de la musique en cours. e_YmMusic_NoMusic (-1) si aucune.
s32 Music_GetMusicNo(void)
{
	if (!gSfx.nInit) return (e_YmMusic_NoMusic);
	return (gSfx.nMusicNo);
}

// Renvoie 1 si la musique en cours est terminée. (Toujours faux si loop !).
u32 Music_IsOver(void)
{
	if (!gSfx.nInit) return (0);
	if ((u32)gSfx.nMusicNo >= e_YmMusic_MAX) return (0);
	return (ymMusicIsOver(gSfx.ppMusic[gSfx.nMusicNo]));
}


#define	SFX_SAMPLES_CH	(512)

// Mixer, appelé par SDL.
void Sfx_MixAudio(void *unused, u8 *stream, int len)
{
	// YM replay.
	static s16	pYmBuffer[SFX_SAMPLES_CH];
	if (gSfx.nMusicNo >= 0)
	{
		int nbSample = len / sizeof(ymsample);
		ymMusicCompute((void*)gSfx.ppMusic[gSfx.nMusicNo], (ymsample *)pYmBuffer, nbSample);
	}
	else
		memset(pYmBuffer, 0, SFX_SAMPLES_CH * sizeof(pYmBuffer[0]));

	// Canaux de Sfx.
	int	i;
	u32	k;
	s32	nOutput;

	k = 0;
//	for (i = 0; i < len / 2; i++)
	for (i = 0; i < (int)(len / sizeof(s16)); i++)
	{
//		nOutput = 0;
		nOutput = pYmBuffer[i];

		for (k = 0; k < SFX_MAX_SOUNDS; k++)
		{
			if (gpSounds[k].nDPos < gpSounds[k].nDLen)
			{
				nOutput += (*(s16 *)&gpSounds[k].pData[gpSounds[k].nDPos]);// / 4;//2;		// / 2 pour réduire la saturation.
				gpSounds[k].nDPos += 2;
			}
		}
		nOutput /= 2;

		if (nOutput > 32767) nOutput = 32767;		// Clip the result.
		if (nOutput < -32768) nOutput = -32768;

		((s16 *)stream)[i] = (s16)nOutput;
	}


/*
    u32	i;
    u32	amount;

    for (i = 0; i < SFX_MAX_SOUNDS; i++)
    {
        amount = (gpSounds[i].nDLen - gpSounds[i].nDPos);
        if (amount > (u32)len)
        {
            amount = len;
        }
        SDL_MixAudio(stream, &gpSounds[i].pData[gpSounds[i].nDPos], amount, SDL_MIX_MAXVOLUME);
        gpSounds[i].nDPos += amount;
    }
*/

}

#ifdef DEBUG_DISP
// debug.
void sfx_tst_dispnb(u32 nPosY)
{
	u32	k;
	char	pFxNo[6+1] = "Fx:000";
	char	pPrio[6+1] = "Pr:000";
	char	pLen[6+1] = "000000";
	for (k = 0; k < SFX_MAX_SOUNDS; k++)
	{
		if (gpSounds[k].nDPos >= gpSounds[k].nDLen)
		{
			Font_Print(10, nPosY + (10 * k), "-", 0);
		}
		else
		{
			Font_Print(10, nPosY + (10 * k), "+", 0);
			pFxNo[3] = pFxNo[4] = pFxNo[5] = '0';
			pPrio[3] = pPrio[4] = pPrio[5] = '0';
			MyItoA(gpSounds[k].nSfxNo, pFxNo);
			MyItoA(gpSounds[k].nPrio, pPrio);
			Font_Print(10 + 10+8, nPosY + (10 * k), pFxNo, 0);
			Font_Print(10 + 10+8 + 60+8, nPosY + (10 * k), pPrio, 0);

			memset(pLen, '0', 6);
			MyItoA(gpSounds[k].nDPos, pLen);
			Font_Print(10 + 10+8 + 60+8 + 60+8, nPosY + (10 * k), pLen, 0);
			memset(pLen, '0', 6);
			MyItoA(gpSounds[k].nDLen, pLen);
			Font_Print(10 + 10+8 + 60+8 + 60+8 + 60+8, nPosY + (10 * k), pLen, 0);
/*
*/
		}
	}
}
#endif


// Nettoyage des canaux.
void Sfx_ClearChannels(void)
{
	u32	i;

	SDL_LockAudio();
    for (i = 0; i < SFX_MAX_SOUNDS; i++)
    {
		gpSounds[i].nDPos = 0;
		gpSounds[i].nDLen = 0;
	}
	SDL_UnlockAudio();

}


struct SSample	gpSoundsBkp[SFX_MAX_SOUNDS];	// Pour stop/reprise du son pendant la pause.
s32	gnMusicNoBkp;	// Bkp no de musique.


// Sauvegarde des datas + clear channels.
void Sfx_ChannelsSave(void)
{
	u32	i;

	if (!gSfx.nInit) return;

	gnMusicNoBkp = gSfx.nMusicNo;		// Sauvegarde du no de musique.
	gSfx.nMusicNo = e_YmMusic_NoMusic;

	for (i = 0; i < SFX_MAX_SOUNDS; i++)
		memcpy(&gpSoundsBkp[i], &gpSounds[i], sizeof(struct SSample));

	Sfx_ClearChannels();
}

// Remise en place des datas.
void Sfx_ChannelsRestore(void)
{
	u32	i;

	if (!gSfx.nInit) return;

	gSfx.nMusicNo = gnMusicNoBkp;		// Resume music.

	for (i = 0; i < SFX_MAX_SOUNDS; i++)
		memcpy(&gpSounds[i], &gpSoundsBkp[i], sizeof(struct SSample));
}



// Sound, initialisation. A appeler 1 fois.
void Sfx_SoundInit(void)
{
	gSfx.nInit = 0;

	// Set 16-bit stereo audio at 22Khz.
	gSfx.sAudioSpec.freq = 22050;
	gSfx.sAudioSpec.format = AUDIO_S16SYS;//AUDIO_S16;
	gSfx.sAudioSpec.channels = 2;
	gSfx.sAudioSpec.samples = SFX_SAMPLES_CH;//512;        // A good value for games.
	gSfx.sAudioSpec.callback = Sfx_MixAudio;
	gSfx.sAudioSpec.userdata = NULL;

	// Open the audio device and start playing sound!
	if (SDL_OpenAudio(&gSfx.sAudioSpec, NULL) < 0)
	{
		//fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
		//exit(1);
		printf("Unable to open audio: %s\n", SDL_GetError());
		printf("Sound disabled.\n");
		return;
	}

	gSfx.nInit = 1;		// Ok.

	Sfx_ClearChannels();	// Nettoyage des structures.
	gSfx.nMusicNo = e_YmMusic_NoMusic;		// Pas de musique en cours.

}

// Sound on. !!! Attention, appel à l'init, ce n'est pas fait pour couper/remettre le son !!!
void Sfx_SoundOn(void)
{
	if (!gSfx.nInit) return;
	SDL_PauseAudio(0);

}

// Sound off. !!! Attention, appel à l'init, ce n'est pas fait pour couper/remettre le son !!!
void Sfx_SoundOff(void)
{
	if (!gSfx.nInit) return;
	SDL_CloseAudio();

}


// Chargement de tous les fichiers WAV.
void Sfx_LoadWavFiles(void)
{
	u32	i;

	SDL_AudioSpec sWave;
	u8	*pData;
	Uint32	nDLen;

	char	*pSfxFilenames[e_Sfx_LAST] = {
		"sfx/menu_click1.wav",
		"sfx/menu_click2.wav",

		"sfx/explosion2.wav",
		"sfx/water_splash0.wav",
		"sfx/water_splash1.wav",
		"sfx/gun_reload0.wav",
		"sfx/swoosh0.wav",
		"sfx/door_clang0.wav",
		"sfx/train_clang0.wav",
		"sfx/zombie_acid.wav",
		"sfx/wood0.wav",
		"sfx/thankyou0.wav",

		"sfx/tank0.wav",
		"sfx/tank1.wav",
		"sfx/heli0.wav",
		"sfx/rocket0.wav",
		"sfx/rocket1.wav",
		"sfx/submarine0.wav",
		"sfx/submarine1.wav",

		"sfx/deserteagle-fire_gun.wav",
		"sfx/flame0.wav",
		"sfx/m79-fire_mortar.wav",
		"sfx/m249-fire_rifle.wav",
		"sfx/alien_shot0.wav",
		"sfx/minirocket0.wav",
		"sfx/zombie_shot0.wav",
		"sfx/bmbdrop0.wav",

		"sfx/doom2.wav",
		"sfx/doom3.wav",
		"sfx/marspeople_death.wav",
		"sfx/jellyfish_death.wav",
		"sfx/zombie_death.wav",
	};

	if (!gSfx.nInit) return;

	for (i = 0; i < e_Sfx_LAST; i++)
	{
		// Load the sound file and convert it to 16-bit stereo at 22kHz
		if (SDL_LoadWAV(pSfxFilenames[i], &sWave, &pData, &nDLen) == NULL)
		{
			fprintf(stderr, "Sfx_LoadWavFiles(): Couldn't load '%s': %s\n", pSfxFilenames[i], SDL_GetError());
			//return;
			exit(1);
		}
		SDL_BuildAudioCVT(&gSfx.pCvt[i], sWave.format, sWave.channels, sWave.freq,
			gSfx.sAudioSpec.format, gSfx.sAudioSpec.channels, gSfx.sAudioSpec.freq);
//		gSfx.pCvt[i].buf = (u8*)malloc(nDLen * gSfx.pCvt[i].len_mult);
		if ((gSfx.pCvt[i].buf = (u8 *)malloc(nDLen * gSfx.pCvt[i].len_mult)) == NULL)
		{
			fprintf(stderr, "Sfx_LoadWavFiles(): malloc failed (gSfx.pCvt[%d].buf).\n", (int)i);
			exit(1);
		}
		memcpy(gSfx.pCvt[i].buf, pData, nDLen);
		gSfx.pCvt[i].len = nDLen;
		SDL_ConvertAudio(&gSfx.pCvt[i]);
//printf("wav %d: len_ratio %f / len %d / len*len_ratio %f / len_cvt %d\n",i,gSfx.pCvt[i].len_ratio, gSfx.pCvt[i].len, gSfx.pCvt[i].len*gSfx.pCvt[i].len_ratio, gSfx.pCvt[i].len_cvt);
		SDL_FreeWAV(pData);

	}

}

// Libère les ressources occupées par les fichiers WAV.
void Sfx_FreeWavFiles(void)
{
	u32	i;

	if (!gSfx.nInit) return;

	for (i = 0; i < e_Sfx_LAST; i++)
	{
		free(gSfx.pCvt[i].buf);
	}

}


// Chargement de tous les fichiers YM.
void Sfx_LoadYMFiles(void)
{
	u32	i;

	// Les fichiers YM.
	static	char *pMusicFn[e_YmMusic_MAX] = {
		"sfx/mus_mission1.ym",
		"sfx/mus_mission2.ym",
		"sfx/mus_mission3.ym",
		"sfx/mus_mission_complete.ym",
		"sfx/mus_game_over.ym",
		"sfx/mus_credits.ym",
		"sfx/mus_high_score.ym",
	};
	// Loop ou non.
	static	ymbool pMusicLoop[e_YmMusic_MAX] = {
		YMTRUE,
		YMTRUE,
		YMTRUE,
		YMFALSE,
		YMFALSE,
		YMTRUE,
		YMTRUE,
	};

	if (!gSfx.nInit) return;

	// Lecture des fichiers YM.
	for (i = 0; i < e_YmMusic_MAX; i++)
	{
		if ((gSfx.ppMusic[i] = ymMusicCreate()) == NULL)
		{
			fprintf(stderr, "ymMusicCreate(): Error.\n");
			exit(1);
		}
		if (ymMusicLoad(gSfx.ppMusic[i], pMusicFn[i]) == 0)
		{
			fprintf(stderr, "ymMusicLoad(): Error.\n");
			exit(1);
		}
		ymMusicSetLoopMode(gSfx.ppMusic[i], pMusicLoop[i]);
	}

	gSfx.nMusicNo = -1;		// Pas de musique en cours.

}

// Libère les ressources des YMs.
void Sfx_FreeYMFiles(void)
{
	u32	i;

	if (!gSfx.nInit) return;

	for (i = 0; i < e_YmMusic_MAX; i++)
	{
		ymMusicStop(gSfx.ppMusic[i]);
		ymMusicDestroy(gSfx.ppMusic[i]);
	}
}


// Joue un son.
// Le minimum :
// On commence par chercher un canal vide.
// Si il n'y en a pas, on note celui qui à la priorité la plus faible.
// Si plusieurs ont la même priorité, on note celui qui est le plus proche de la fin.
// Enfin, si la prio du son à jouer est ok, on le joue dans le canal noté.
void Sfx_PlaySfx(u32 nSfxNo, u32 nSfxPrio)
{
	u32	index;

//printf("Sfx_PlaySfx, sfxno=%d, frm=%d\n", nSfxNo, gnFrame);

	u8	nPrioMinVal = 255;
	u32	nPrioMinPos = 0;
	u32	nPrioMinDiff = (u32)-1;

	if (nSfxNo >= e_Sfx_LAST) return;	// Sécurité.

	// Look for an empty (or finished) sound slot.
	for (index = 0; index < SFX_MAX_SOUNDS; index++)
	{
		if (gpSounds[index].nSfxNo == nSfxNo) break;	// Même wav (+ son interruptible), on redémarre le sfx sur ce canal.

		if (gpSounds[index].nDPos >= gpSounds[index].nDLen) break;
		//
		if (gpSounds[index].nPrio < nPrioMinVal)
		{
			nPrioMinVal = gpSounds[index].nPrio;
			nPrioMinPos = index;
			nPrioMinDiff = gpSounds[index].nDLen - gpSounds[index].nDPos;
		}
		else if (gpSounds[index].nPrio == nPrioMinVal)
		{
			if (gpSounds[index].nDLen - gpSounds[index].nDPos < nPrioMinDiff)
			{
				//nPrioMinVal = sounds[index].nPrio;
				nPrioMinPos = index;
				nPrioMinDiff = gpSounds[index].nDLen - gpSounds[index].nDPos;
			}
		}

	}

	// On a trouvé un emplacement libre ?
	if (index == SFX_MAX_SOUNDS)
	{
		// Non, la prio demandée est > ou == à la prio mini en cours ?
		if (nSfxPrio < nPrioMinVal) return;
		index = nPrioMinPos;
	}

	// Put the sound data in the slot (it starts playing immediately).
	SDL_LockAudio();
	gpSounds[index].pData = gSfx.pCvt[nSfxNo].buf;
	gpSounds[index].nDLen = gSfx.pCvt[nSfxNo].len_cvt;
	gpSounds[index].nDPos = 0;
	gpSounds[index].nPrio = (u8)nSfxPrio;
	gpSounds[index].nSfxNo = (u8)nSfxNo;
	SDL_UnlockAudio();

}

// Coupe un son.
void Sfx_StopSfx(u32 nSfxNo)
{
	u32	i;

	if (nSfxNo >= e_Sfx_LAST) return;	// Sécurité.

	// Recherche du son.
	for (i = 0; i < SFX_MAX_SOUNDS; i++)
	{
		if (gpSounds[i].nSfxNo == nSfxNo)
		{
			SDL_LockAudio();
			gpSounds[i].nDPos = 0;
			gpSounds[i].nDLen = 0;
			SDL_UnlockAudio();
		}
	}

}

// Teste si un son est en train d'être joué.
u32 Sfx_IsPlaying(u32 nSfxNo)
{
	u32	i;

	if (nSfxNo >= e_Sfx_LAST) return (0);	// Sécurité.

	// Recherche du son.
	for (i = 0; i < SFX_MAX_SOUNDS; i++)
	{
		if (gpSounds[i].nSfxNo == nSfxNo)
			if (gpSounds[i].nDPos < gpSounds[i].nDLen) return (1);
	}
	return (0);
}


// Exemples là :
// http://www.libsdl.org/intro.fr/usingsoundfr.html
// http://www.libsdl.org/intro.fr/usingsoundfr.html




/*
void _PlaySound(char *file)
{
    int index;
    SDL_AudioSpec wave;
    Uint8 *data;
    Uint32 dlen;
    SDL_AudioCVT cvt;

    // Look for an empty (or finished) sound slot
    for ( index=0; index<SFX_MAX_SOUNDS; ++index ) {
        if ( sounds[index].dpos == sounds[index].dlen ) {
            break;
        }
    }
    if ( index == SFX_MAX_SOUNDS )
        return;

    // Load the sound file and convert it to 16-bit stereo at 22kHz
    if ( SDL_LoadWAV(file, &wave, &data, &dlen) == NULL ) {
        fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
        return;
    }
    SDL_BuildAudioCVT(&cvt, wave.format, wave.channels, wave.freq,
                            AUDIO_S16,   2,             22050);
    cvt.buf = (u8*)malloc(dlen*cvt.len_mult);
    memcpy(cvt.buf, data, dlen);
    cvt.len = dlen;
    SDL_ConvertAudio(&cvt);
    SDL_FreeWAV(data);

    // Put the sound data in the slot (it starts playing immediately)
    if ( sounds[index].data ) {
        free(sounds[index].data);
    }
    SDL_LockAudio();
    sounds[index].data = cvt.buf;
    sounds[index].dlen = cvt.len_cvt;
    sounds[index].dpos = 0;
    SDL_UnlockAudio();
}
*/


/*
#include "SDL.h"
#include "SDL_audio.h"
{
    extern void mixaudio(void *unused, Uint8 *stream, int len);
    SDL_AudioSpec fmt;

    // Set 16-bit stereo audio at 22Khz
    fmt.freq = 22050;
    fmt.format = AUDIO_S16;
    fmt.channels = 2;
    fmt.samples = 512;        // A good value for games
    fmt.callback = mixaudio;
    fmt.userdata = NULL;

    // Open the audio device and start playing sound!
    if ( SDL_OpenAudio(&fmt, NULL) < 0 ) {
        fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_PauseAudio(0);

    ...

    SDL_CloseAudio();
}


#define NUM_SOUNDS 2
struct sample {
    Uint8 *data;
    Uint32 dpos;
    Uint32 dlen;
} sounds[NUM_SOUNDS];

void mixaudio(void *unused, Uint8 *stream, int len)
{
    int i;
    Uint32 amount;

    for ( i=0; i<NUM_SOUNDS; ++i ) {
        amount = (sounds[i].dlen-sounds[i].dpos);
        if ( amount > len ) {
            amount = len;
        }
        SDL_MixAudio(stream, &sounds[i].data[sounds[i].dpos], amount, SDL_MIX_MAXVOLUME);
        sounds[i].dpos += amount;
    }
}

void PlaySound(char *file)
{
    int index;
    SDL_AudioSpec wave;
    Uint8 *data;
    Uint32 dlen;
    SDL_AudioCVT cvt;

    // Look for an empty (or finished) sound slot
    for ( index=0; index<NUM_SOUNDS; ++index ) {
        if ( sounds[index].dpos == sounds[index].dlen ) {
            break;
        }
    }
    if ( index == NUM_SOUNDS )
        return;

    // Load the sound file and convert it to 16-bit stereo at 22kHz
    if ( SDL_LoadWAV(file, &wave, &data, &dlen) == NULL ) {
        fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
        return;
    }
    SDL_BuildAudioCVT(&cvt, wave.format, wave.channels, wave.freq,
                            AUDIO_S16,   2,             22050);
    cvt.buf = malloc(dlen*cvt.len_mult);
    memcpy(cvt.buf, data, dlen);
    cvt.len = dlen;
    SDL_ConvertAudio(&cvt);
    SDL_FreeWAV(data);

    // Put the sound data in the slot (it starts playing immediately)
    if ( sounds[index].data ) {
        free(sounds[index].data);
    }
    SDL_LockAudio();
    sounds[index].data = cvt.buf;
    sounds[index].dlen = cvt.len_cvt;
    sounds[index].dpos = 0;
    SDL_UnlockAudio();
}

*/


