
// Prototypes.
void AnmInitEngine(void);
s32 AnmSet(u64 *pAnm, s32 nSlotNo);
s32 AnmSetIfNew(u64 *pAnm, s32 nSlotNo);
void AnmReleaseSlot(s32 nSlotNo);
s32 AnmGetImage(s32 nSlotNo);
s32 AnmGetLastImage(s32 nSlotNo);
u32 AnmGetKey(s32 nSlotNo);
u32 AnmCheckEnd(s32 nSlotNo);
u32 AnmCheckStepFlag(s32 nSlotNo);
u32 AnmCheckNewImgFlag(s32 nSlotNo);


