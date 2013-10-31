#ifndef __COMMON_H__
#define __COMMON_H__

#include <stddef.h>
#include <stdint.h>

/* Size constants must match those used on host: include/asm-generic/sizes.h */
#define SZ_64K                          0x00010000
#define SZ_128K                         0x00020000
#define SZ_256K                         0x00040000
#define SZ_512K                         0x00080000
#define SZ_1M                           0x00100000
#define SZ_2M                           0x00200000
#define SZ_4M                           0x00400000
#define SZ_8M                           0x00800000
#define SZ_16M                          0x01000000
#define SZ_32M                          0x02000000
#define SZ_64M                          0x04000000
#define SZ_128M                         0x08000000
#define SZ_256M                         0x10000000
#define SZ_512M                         0x20000000

#define write32(ADDR, VAL)          ( *(unsigned int *)ADDR = (unsigned int)(VAL) )
#define read32(ADDR)                ( *(unsigned int *)ADDR )

#define toggle_bit(ADDR, BIT)       ( *(unsigned int *)ADDR ^= (1UL << (BIT)) )
#define reset_bit(ADDR, BIT)        ( *(unsigned int *)ADDR &= ~(1UL << (BIT)) )
#define set_bit(ADDR, BIT)          ( *(unsigned int *)ADDR |= (1UL << (BIT)) )

#define __packed __attribute__ ((packed))

typedef uint64_t    u64;
typedef uint32_t    u32;
typedef uint16_t    u16;
typedef uint8_t     u8;

typedef uint64_t    __u64;
typedef uint32_t    __u32;
typedef uint16_t    __u16;
typedef uint8_t     __u8;

#endif /* __COMMON_H__ */
