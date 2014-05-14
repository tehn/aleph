//  prgm.h
//  aleph-blackfin

#ifndef _ALEPH_PRGM_H_
#define _ALEPH_PRGM_H_

#include "fix.h"
#include "slew.h"


//symbolic constants, macro substitutions
#define WAVE_TAB_NUM 5

#define WAVE_SHAPE_NUM 5
#define WAVE_SHAPE_BITS 3

#define WAVE_TAB_SIZE 1024

#define WAVE_TAB_BITS 10

#define WAVE_TAB_SIZE_1 (WAVE_TAB_SIZE - 1)

#define WAVE_TAB_MAX16 (WAVE_TAB_SIZE * FIX16_ONE - 1)
#define WAVE_TAB_RSHIFT 29
#define WAVE_TAB_MASK 0x1fffffff
#define WAVE_TAB_LSHIFT 2

#define WAVE_SHAPE_IDX_SHIFT 16 - (WAVE_SHAPE_BITS)
#define WAVE_SHAPE_MASK (1 << (WAVE_SHAPE_IDX_SHIFT)) - 1
#define WAVE_SHAPE_MUL_SHIFT 2
#define WAVE_IDX_SHIFT 21
#define WAVE_IDX_MASK (1 << (WAVE_IDX_SHIFT)) - 1
#define WAVE_IDX_MUL_SHIFT (WAVE_IDX_SHIFT) - 15
#define WAVE_IPS_NORM 0xae3c

#define N_OSCILLATORS 4


typedef const fract32 (*wavtab_t) [WAVE_SHAPE_NUM][WAVE_TAB_SIZE]; //new

typedef struct _prgmOscillator *PrgmOscillatorpointer;

typedef struct _prgmOscillator {
    fract32 frameVal;
    fract32 osc; //not used in new, same as frameVal?! output
    wavtab_t val; //new
    fix16 freq; //frequency in hz
    fix16 ratio;
    fract32 phase; //new
    fix16 idx; //not used in new, same as phase?!
    fix16 inc;
    fract32 wave; //not used in new, same as shape?!
    fract32 shape; //new
    fract32 amp;
    Slew32 incSlew;
    //    Slew16 shapeSlew;
} prgmOscillator;

#endif
