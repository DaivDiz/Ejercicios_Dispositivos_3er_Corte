# 1 "delay.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:/Users/dsdia/.mchp_packs/Microchip/PIC18Fxxxx_DFP/1.2.26/xc8\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "delay.c" 2
# 40 "delay.c"
# 1 "./delay.h" 1
# 42 "./delay.h"
# 1 "./stdutils.h" 1
# 44 "./stdutils.h"
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c99\\stdint.h" 1 3



# 1 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c99\\musl_xc8.h" 1 3
# 4 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c99\\stdint.h" 2 3
# 22 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c99\\stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c99\\bits/alltypes.h" 1 3
# 127 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c99\\bits/alltypes.h" 3
typedef unsigned long uintptr_t;
# 142 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c99\\bits/alltypes.h" 3
typedef long intptr_t;
# 158 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c99\\bits/alltypes.h" 3
typedef signed char int8_t;




typedef short int16_t;




typedef __int24 int24_t;




typedef long int32_t;





typedef long long int64_t;
# 188 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c99\\bits/alltypes.h" 3
typedef long long intmax_t;





typedef unsigned char uint8_t;




typedef unsigned short uint16_t;




typedef __uint24 uint24_t;




typedef unsigned long uint32_t;





typedef unsigned long long uint64_t;
# 229 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c99\\bits/alltypes.h" 3
typedef unsigned long long uintmax_t;
# 22 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c99\\stdint.h" 2 3


typedef int8_t int_fast8_t;

typedef int64_t int_fast64_t;


typedef int8_t int_least8_t;
typedef int16_t int_least16_t;

typedef int24_t int_least24_t;
typedef int24_t int_fast24_t;

typedef int32_t int_least32_t;

typedef int64_t int_least64_t;


typedef uint8_t uint_fast8_t;

typedef uint64_t uint_fast64_t;


typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;

typedef uint24_t uint_least24_t;
typedef uint24_t uint_fast24_t;

typedef uint32_t uint_least32_t;

typedef uint64_t uint_least64_t;
# 144 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c99\\stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c99\\bits/stdint.h" 1 3
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
# 144 "C:\\Program Files\\Microchip\\xc8\\v2.31\\pic\\include\\c99\\stdint.h" 2 3
# 44 "./stdutils.h" 2
# 67 "./stdutils.h"
typedef signed char sint8_t;
typedef unsigned char uint8_t;

typedef signed short int sint16_t;
typedef unsigned short int uint16_t;

typedef signed int sint32_t;
# 177 "./stdutils.h"
typedef enum
{
   E_FALSE,
   E_TRUE
}Boolean_et;


typedef enum
{
    E_FAILED,
    E_SUCCESS,
    E_BUSY,
    E_TIMEOUT
}Status_et;


typedef enum
{
    E_BINARY=2,
    E_DECIMAL = 10,
    E_HEX = 16
}NumericSystem_et;
# 42 "./delay.h" 2
# 65 "./delay.h"
void DELAY_us(uint16_t us_count);
void DELAY_ms(uint16_t ms_count);
void DELAY_sec(uint16_t var_delaySecCount_u16);
# 40 "delay.c" 2
# 52 "delay.c"
void DELAY_us(uint16_t us_count)
{
    while (us_count != 0)
    {
        us_count--;
    }
}
# 70 "delay.c"
void DELAY_ms(uint16_t ms_count)
{
    while (ms_count != 0)
    {
        DELAY_us(300u);
        ms_count--;
    }
}
# 96 "delay.c"
void DELAY_sec(uint16_t sec_count)
{
    while (sec_count != 0) {
        DELAY_ms(1000);
        sec_count--;
    }
}
