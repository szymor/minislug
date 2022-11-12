
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;

#if defined(__LP64__) || defined(_WIN64)
#define	CPU64	(1)
typedef uint64_t u64;
typedef int64_t s64;
#else
typedef uint32_t u64;
typedef int32_t s64;
#endif
