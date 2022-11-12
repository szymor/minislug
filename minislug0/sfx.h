
// Prototypes.
void Sfx_SoundInit(void);
void Sfx_SoundOn(void);
void Sfx_SoundOff(void);
void Sfx_LoadWavFiles(void);
void Sfx_FreeWavFiles(void);
void Sfx_LoadYMFiles(void);
void Sfx_FreeYMFiles(void);

void Sfx_PlaySfx(u32 nSfxNo, u32 nSfxPrio);
void Sfx_StopSfx(u32 nSfxNo);
u32 Sfx_IsPlaying(u32 nSfxNo);
void Sfx_ClearChannels(void);

void Sfx_ChannelsSave(void);
void Sfx_ChannelsRestore(void);

void Music_Start(s32 nMusicNo, u32 nForceRestart);
s32 Music_GetMusicNo(void);
u32 Music_IsOver(void);


// Enum YM.
enum
{
	e_YmMusic_NoMusic = -1,
	e_YmMusic_Musics = 0,

	e_YmMusic_Mission1 = e_YmMusic_Musics,
	e_YmMusic_Mission2,
	e_YmMusic_Mission3,
	e_YmMusic_MissionComplete,
	e_YmMusic_GameOver,
	e_YmMusic_Credits,
	e_YmMusic_HighScore,

	e_YmMusic_MAX
};

// Enums.
enum
{
	e_Sfx_MenuClic1 = 0,
	e_Sfx_MenuClic2,

	e_Sfx_Fx_Explosion2,
	e_Sfx_Fx_WaterSplash,
	e_Sfx_Fx_WaterSplash2,
	e_Sfx_Fx_GunReload,
	e_Sfx_Fx_Swoosh,
	e_Sfx_Fx_DoorClang,
	e_Sfx_Fx_TrainClang,
	e_Sfx_Fx_ZombieAcid,
	e_Sfx_Fx_Wood,
	e_Sfx_Fx_ThankYou,

	e_Sfx_Tank0,
	e_Sfx_Tank1,
	e_Sfx_Heli0,
	e_Sfx_Rocket0,
	e_Sfx_Rocket1,
	e_Sfx_Submarine0,
	e_Sfx_Submarine1,

	e_Sfx_Shot_Gun,
	e_Sfx_Shot_Flame,
	e_Sfx_Shot_Mortar,
	e_Sfx_Shot_Rifle,
	e_Sfx_Shot_Alien,
	e_Sfx_Shot_MiniRocket,
	e_Sfx_Shot_Zombie,
	e_Sfx_Shot_BombDrop,

	e_Sfx_Death_Scream1,
	e_Sfx_Death_Scream2,
	e_Sfx_Death_MarsPeople,
	e_Sfx_Death_Jellyfish,
	e_Sfx_Death_Zombie,

	e_Sfx_LAST
};

enum
{
	e_SfxPrio_0 = 0,
	e_SfxPrio_10 = 10,
	e_SfxPrio_20 = 20,
	e_SfxPrio_30 = 30,
	e_SfxPrio_40 = 40,
	e_SfxPrio_Max = 254,

	e_SfxPrio_Enemy_Shot = 10,
	e_SfxPrio_Enemy_Death = 12,
	e_SfxPrio_Explosion = 15,
	e_SfxPrio_Hero_Shot = 20,

};

