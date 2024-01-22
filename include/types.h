#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

typedef uint8_t u8;       ///<   8-bit unsigned integer.
typedef uint16_t u16;     ///<  16-bit unsigned integer.
typedef uint32_t u32;     ///<  32-bit unsigned integer.
typedef uint64_t u64;     ///<  64-bit unsigned integer.
typedef __uint128_t u128; ///< 128-bit unsigned integer.

typedef int8_t s8;       ///<   8-bit signed integer.
typedef int16_t s16;     ///<  16-bit signed integer.
typedef int32_t s32;     ///<  32-bit signed integer.
typedef int64_t s64;     ///<  64-bit signed integer.
typedef __int128_t s128; ///< 128-bit unsigned integer.

typedef volatile u8 vu8;     ///<   8-bit volatile unsigned integer.
typedef volatile u16 vu16;   ///<  16-bit volatile unsigned integer.
typedef volatile u32 vu32;   ///<  32-bit volatile unsigned integer.
typedef volatile u64 vu64;   ///<  64-bit volatile unsigned integer.
typedef volatile u128 vu128; ///< 128-bit volatile unsigned integer.

typedef volatile s8 vs8;     ///<   8-bit volatile signed integer.
typedef volatile s16 vs16;   ///<  16-bit volatile signed integer.
typedef volatile s32 vs32;   ///<  32-bit volatile signed integer.
typedef volatile s64 vs64;   ///<  64-bit volatile signed integer.
typedef volatile s128 vs128; ///< 128-bit volatile signed integer.

typedef unsigned char   uchar;
typedef	unsigned short	ushort;
typedef	unsigned int	uint;	
typedef	unsigned long	ulong;


#define ALIGN_UP(x, a) ((((uintptr_t)x) + (((uintptr_t)a)-1)) & ~(((uintptr_t)a)-1))
#define ALIGN_DOWN(x, a) ((uintptr_t)(x) & ~(((uintptr_t)(a)) - 1))
#define ALIGNED(a)      __attribute__((aligned(a)))
#define ON_INIT         __attribute__((constructor))
#define NOINLINE        __attribute__((noinline))
#define NORETURN        __attribute__((noreturn))
#define UNREACHABLE __builtin_unreachable()
#define PAGE_SIZE (0x1000)
#define ALWAYS_INLINE inline __attribute__((always_inline))
#define BITSIZEOF(x) (sizeof(x) * CHAR_BIT)
#define BIT_LONG(n)                     (1UL << (n))

#define to64b(arr) (((uint64_t)(((uint8_t *)(arr))[7]) <<  0)+\
                    ((uint64_t)(((uint8_t *)(arr))[6]) <<  8)+\
                    ((uint64_t)(((uint8_t *)(arr))[5]) << 16)+\
                    ((uint64_t)(((uint8_t *)(arr))[4]) << 24)+\
                    ((uint64_t)(((uint8_t *)(arr))[3]) << 32)+\
                    ((uint64_t)(((uint8_t *)(arr))[2]) << 40)+\
                    ((uint64_t)(((uint8_t *)(arr))[1]) << 48)+\
                    ((uint64_t)(((uint8_t *)(arr))[0]) << 56))

#define to64l(arr) (((uint64_t)(((uint8_t *)(arr))[0]) <<  0)+\
                    ((uint64_t)(((uint8_t *)(arr))[1]) <<  8)+\
                    ((uint64_t)(((uint8_t *)(arr))[2]) << 16)+\
                    ((uint64_t)(((uint8_t *)(arr))[3]) << 24)+\
                    ((uint64_t)(((uint8_t *)(arr))[4]) << 32)+\
                    ((uint64_t)(((uint8_t *)(arr))[5]) << 40)+\
                    ((uint64_t)(((uint8_t *)(arr))[6]) << 48)+\
                    ((uint64_t)(((uint8_t *)(arr))[7]) << 56))

typedef struct {
    union {
        u32 value;
        struct {
            u32 relstep : 8;
            u32 micro   : 8;
            u32 minor   : 8;
            u32 major   : 8;
        };
    };
} SdkAddOnVersion;

typedef struct {
    union {
        u32 value;
        struct {
            u32 minor_relstep : 8;
            u32 major_relstep : 8;
            u32 micro         : 4;
            u32 minor         : 6;
            u32 major         : 6;
        } system_version;
        struct {
            u32 private_ver   : 16;
            u32 release_ver   : 16;
        } application_version;
    };
} Version;