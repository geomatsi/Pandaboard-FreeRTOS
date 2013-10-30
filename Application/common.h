#ifndef _COMMON_H_
#define _COMMON_H_

#include <stddef.h>
#include <stdint.h>

#define write32(ADDR, VAL)          ( *(unsigned int *)ADDR = (unsigned int)(VAL) )
#define read32(ADDR)                ( *(unsigned int *)ADDR )

#define toggle_bit(ADDR, BIT)       ( *(unsigned int *)ADDR ^= (1UL << (BIT)) )
#define reset_bit(ADDR, BIT)        ( *(unsigned int *)ADDR &= ~(1UL << (BIT)) )
#define set_bit(ADDR, BIT)          ( *(unsigned int *)ADDR |= (1UL << (BIT)) )

#endif /* _COMMON_H_ */
