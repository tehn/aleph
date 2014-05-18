//prgm.c
//aleph-bfin

//standard libraries
#include <math.h>
#include <stdlib.h>
#include <string.h>

//aleph-common
#include "fix.h"
#include "slew.h"
#include "types.h"

//audiolib
#include "interpolate.h"
#include "table.h"
#include "conversion.h"

//bfin
#include "bfin_core.h"
#include "fract_math.h"
#include <fract2float_conv.h>

#include "module.h"
#include "params.h"
#include "prgm.h"


//data types
typedef struct _prgmData {
  ModuleData super;
  ParamData mParamData[eParamNumParams];
} prgmData;

ModuleData *gModuleData;

prgmOscillator *oscillator[N_OSCILLATORS];

static prgmData *data;

static const fract32 wavtab[WAVE_SHAPE_NUM][WAVE_TAB_SIZE] = { 
#include "wavtab_data_inc.c" 
};

//fixed variables
static u32 sr;


//static function declaration
static inline fract32 freq_to_phase(fix16 freq);

static inline void oscillator_calc_inc(prgmOscillator *oscillator);

PrgmOscillatorpointer init(void);

static void init_parameters(prgmOscillator *oscillator, wavtab_t tab, u32 sr);

//static s32 idx_reset(s32 val);

static void oscillator_set_freq(prgmOscillator *oscillator, fix16 freq);

static void oscillator_set_shape(prgmOscillator *oscillator, fract16 wave);

// set modulated phase
//extern void blosc_set_pm(blOsc* osc, fract32 pm);  //IN PROGRESS!

static fract32 oscillator_lookup(prgmOscillator *oscillator);

static fract32 oscillator_next(prgmOscillator *oscillator);

static void oscillator_advance(prgmOscillator *oscillator);

static void calc_frame(void);


//static functions
static inline fract32 freq_to_phase(fix16 f) {
    return add_fr1x32(
                      // int part
                      (fract32)( ((int)(f >> 16) * (int)(WAVE_IPS_NORM) ) ),
                      // fract part
                      mult_fr1x32x32( (f & 0xffff) >> 1, (fract16)(WAVE_IPS_NORM) )
                      );
}


static inline void oscillator_calc_inc(prgmOscillator *oscillator) {
    oscillator->incSlew.x = freq_to_phase(fix16_mul(FIX16_ONE, oscillator->freq));
}


PrgmOscillatorpointer init(void) {
    return(PrgmOscillatorpointer)malloc(sizeof(prgmOscillator));    
}

    
void init_parameters(prgmOscillator *osc, wavtab_t tab, u32 sr) {
    prgmOscillator *oscillator = (prgmOscillator*)osc;
    
    oscillator->tab = tab;
    slew_init(oscillator->incSlew, 0, 0, 0);
    slew_init(osc->shapeSlew, 0, 0, 0 );
    
    oscillator->frameVal = 0;
    oscillator->phase = 0;
    oscillator->idx = 0;
    oscillator->freq = FIX16_ONE;
    oscillator->wave = 0;
    oscillator->amp = INT32_MAX >> 4;
}


//s32 idx_reset(s32 val) {}


void oscillator_set_shape(prgmOscillator *oscillator, fract16 wave) {
    oscillator->shapeSlew.x = wave;
}


void oscillator_set_freq(prgmOscillator *oscillator, fix16 freq) {
    oscillator->freq = freq;
    oscillator_calc_inc(oscillator);
}


static inline fract16 param_unit_to_fr16(ParamValue v) {
    return (fract16)((v & 0xffff) >> 1);
}


static inline fract32 oscillator_lookup(prgmOscillator *oscillator) {
    u32 waveIdxA = oscillator->wave >> (WAVE_SHAPE_IDX_SHIFT);
    u32 waveIdxB = waveIdxA + 1;

    fract16 waveMulB = (oscillator->wave & (WAVE_SHAPE_MASK)) << (WAVE_SHAPE_MUL_SHIFT);
    fract16 waveMulA = sub_fr1x16(0x7fff, waveMulB);
        
    u32 signalIdxA = oscillator->phase >> WAVE_IDX_SHIFT; 
    u32 signalIdxB = (signalIdxA + 1) & WAVE_TAB_SIZE_1;
    fract16 signalMulB = (fract16)((oscillator->phase & (WAVE_IDX_MASK)) >> (WAVE_IDX_MUL_SHIFT));
    fract16 signalMulA = sub_fr1x16(0x7fff, signalMulB);     
    
    return add_fr1x32(
                      // table A
                      mult_fr1x32(
                                  trunc_fr1x32(
                                               add_fr1x32(
                                                          // signal A, scaled
                                                          mult_fr1x32( 
                                                                      trunc_fr1x32( (*(oscillator->tab))[waveIdxA][signalIdxA] ),
                                                                      signalMulA 
                                                                      ),
                                                          // signal B, scaled
                                                          mult_fr1x32( 
                                                                      trunc_fr1x32( (*(oscillator->tab))[waveIdxA][signalIdxB] ),
                                                                      signalMulB
                                                                      )
                                                          )
                                               ),
                                  waveMulA
                                  ),
                      // table B
                      mult_fr1x32(
                                  trunc_fr1x32(
                                               add_fr1x32(
                                                          // signal A, scaled
                                                          mult_fr1x32( 
                                                                      trunc_fr1x32( (*(oscillator->tab))[waveIdxB][signalIdxA] ),
                                                                      signalMulA 
                                                                      ),
                                                          // signal B, scaled
                                                          mult_fr1x32( 
                                                                      trunc_fr1x32( (*(oscillator->tab))[waveIdxB][signalIdxB] ),
                                                                      signalMulB
                                                                      )
                                                          )
                                               ),
                                  waveMulB
                                  )
                      );
}
/*
 fract32 oscillator_lookup(prgmOscillator *oscillator) {     //W SYNC TRIG
 #if 1
 u32 idxA = oscillator->wave >> WAVE_TAB_RSHIFT;
 u32 idxB = idxA + 1;
 
 fract32 mul = (oscillator->wave & WAVE_TAB_MASK) << WAVE_TAB_LSHIFT;
 fract32 mulInv = sub_fr1x32(FR32_MAX, mul);
 #else
 #endif
 
 state = OFF;
 
 if (idx_reset(v) == 0) {
 state = OFF;
 
 return add_fr1x32(
 mult_fr1x32x32(table_lookup_idx((fract32*)wavtab[idxA], WAVE_TAB_SIZE, oscillator->idx), mulInv),
 mult_fr1x32x32(table_lookup_idx((fract32*)wavtab[idxB], WAVE_TAB_SIZE, oscillator->idx), mul)
 );
 }
 
 else if (state == OFF) {
 state = IN;
 oscillator->idx = 0;
 
 return add_fr1x32(
 mult_fr1x32x32(table_lookup_idx((fract32*)wavtab[idxA], WAVE_TAB_SIZE, oscillator->idx), mulInv),
 mult_fr1x32x32(table_lookup_idx((fract32*)wavtab[idxB], WAVE_TAB_SIZE, oscillator->idx), mul)
 );
 }
 }
 */


fract32 oscillator_next(prgmOscillator *oscillator) {    
    slew16_calc (oscillator->shapeSlew);
    slew32_calc (oscillator->incSlew);
    
    oscillator->inc = oscillator->incSlew.y;
    oscillator->wave = oscillator->shapeSlew.y;
    
    oscillator_advance(oscillator);    
    
    return oscillator_lookup(oscillator);
}


void oscillator_advance(prgmOscillator *oscillator) {
    oscillator->phase = (((int)oscillator->phase) + ((int)(oscillator->inc)) ) & 0x7fffffff;
}


static void calc_frame(void) {
    oscillator[0]->frameVal = shr_fr1x32(oscillator_next(oscillator[0]), 1);
    oscillator[1]->frameVal = shr_fr1x32(oscillator_next(oscillator[1]), 1);
    oscillator[2]->frameVal = shr_fr1x32(oscillator_next(oscillator[2]), 1);
    oscillator[3]->frameVal = shr_fr1x32(oscillator_next(oscillator[3]), 1);
}


static inline void param_setup(u32 id, ParamValue v) {
    gModuleData->paramData[id].value = v;
    module_set_param(id, v);
}


//external functions
void module_init(void) {
    u8 i;
    data = (prgmData*)SDRAM_ADDRESS;
    gModuleData = &(data->super);
    strcpy(gModuleData->name, "aleph-prgm");
    gModuleData->paramData = data->mParamData;
    gModuleData->numParams = eParamNumParams;
    
    sr = SAMPLERATE;

    for(i=0; i<N_OSCILLATORS; i++) {
        oscillator[i] = init();
        init_parameters(oscillator[i], &wavtab, SAMPLERATE);
    }
    
    param_setup(eParamFreq0, 220 << 16);
    param_setup(eParamFreq1, 220 << 16);
    param_setup(eParamFreq2, 220 << 16);
    param_setup(eParamFreq3, 220 << 16);

    param_setup(eParamWave0, 0);
    param_setup(eParamWave1, 0);
    param_setup(eParamWave2, 0);
    param_setup(eParamWave3, 0);
    
//    param_setup(eParamIdxReset, 0);
        
    param_setup(eParamAmp0, PARAM_AMP_6);
    param_setup(eParamAmp1, PARAM_AMP_6);
    param_setup(eParamAmp2, PARAM_AMP_6);
    param_setup(eParamAmp3, PARAM_AMP_6);
}


void module_deinit(void) {}


extern u32 module_get_num_params(void) {
  return eParamNumParams;
}


void module_process_frame(void) {
  calc_frame();
    
    out[0] = add_fr1x32(oscillator[0]->frameVal, 0x7fffffff);
    out[1] = add_fr1x32(oscillator[1]->frameVal, 0x7fffffff);
    out[2] = add_fr1x32(oscillator[2]->frameVal, 0x7fffffff);
    out[3] = add_fr1x32(oscillator[3]->frameVal, 0x7fffffff);
}


void module_set_param(u32 idx, ParamValue v) {
    switch(idx) {
        case eParamFreq0:
            oscillator_set_freq(oscillator[0], v);
            break;
        case eParamFreq1:
            oscillator_set_freq(oscillator[1], v);
            break;
        case eParamFreq2:
            oscillator_set_freq(oscillator[2], v);
            break;
        case eParamFreq3:
            oscillator_set_freq(oscillator[3], v);
            break;
            
        case eParamWave0:
            oscillator_set_shape(oscillator[0], param_unit_to_fr16(v));
            break;
        case eParamWave1:
            oscillator_set_shape(oscillator[1], param_unit_to_fr16(v));
            break;
        case eParamWave2:
            oscillator_set_shape(oscillator[2], param_unit_to_fr16(v));
            break;
        case eParamWave3:
            oscillator_set_shape(oscillator[3], param_unit_to_fr16(v));
            break;

//        case eParamIdxReset:
//            idx_reset(v);
//            break;

/*
        case eParamIdxMod0:
            //function(&(oscillator[0]->idxMod), v);
            break;
        case eParamIdxMod1:
            //function(&(oscillator[0]->idxMod), v);
            break;
        case eParamIdxMod2:
            //function(&(oscillator[0]->idxMod), v);
            break;
        case eParamIdxMod3:
            //function(&(oscillator[0]->idxMod), v);
            break;
*/
        case eParamAmp0:
            oscillator[0]->amp = (v);
            break;
        case eParamAmp1:
            oscillator[1]->amp = (v);
            break;
        case eParamAmp2:
            oscillator[2]->amp = (v);
            break;
        case eParamAmp3:
            oscillator[3]->amp = (v);
            break;

        default:
            break;
    }
}
