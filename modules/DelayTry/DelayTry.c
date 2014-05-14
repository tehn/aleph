/* 
DelayTry.c
An attempt to make a very simple live processing delay module
*/

//-- blackfin toolchain headers (1.32 and 1.15 fixed-point arithmeti)
#include "fract_math.h"

//-- aleph/common headers
#include "types.h"

//-- aleph/bfin-lib headers
// global variables
#include "bfin_core.h"
#include "dac.h" // cv output driver
#include "gpio.h" // gpio pin numbers

//-- dsp class headers
// global declarations for module data
#include "module.h"

//--- custom headers
// parameter lists and constants
#include "params.h"

//Audio Parameters
#include "buffer.h" //Thinking this may be were audio is stored?

// customized module data structure
// this will be located at the top of SDRAM (64MB)
// all other variables (code, stack, heap) are located in SRAM (64K)
typedef struct _DelayTryData {
  //  volatile fract32 audioBuffer[NUM_BUFS][FRAMES_PER_BUF];
volatile fract32 audioBuffer[1][48000];

} DelayTryData;

//-------------------------
//----- extern vars (initialized here)

// global pointer to module descriptor.
// required by the aleph-bfin library!
ModuleData* gModuleData;

//-----------------------
//------ static variables

/* here's the actual memory for module descriptor and param data.g
 global pointers are to point at these here during module init.
 we do it in this indirect way, because
 a) modules have variable param count
 b) in an extreme case, might need to locate param data in SDRAM (
    ( until recently, SDRAM contained full param descriptors.)
*/
static ModuleData super;
static ParamData mParamData[eParamNumParams];

// input values
static fract32 inVal[4];

// audio output bus
static fract32 outBus = 0;

//-----------------
//--- static functions


//----------------------
//----- external functions

void module_init(void) {
  // initialize module superclass data
  // by setting global pointers to our own data
  gModuleData = &super;
  gModuleData->paramData = mParamData;
  gModuleData->numParams = eParamNumParams;

}

// de-init (never actually used on blackfin, but maybe by emulator)
void module_deinit(void) {
  ;;
}

// get number of parameters
u32 module_get_num_params(void) {
  return eParamNumParams;
}

// frame process function! 
// called each audio frame from codec interrupt handler
// ( bad, i know, see github issues list )
void module_process_frame(void) { 

  // update all the attenuation values
  inVal[0] = filter_1p_lo_next( &(inSlew[0]) );
  inVal[1] = filter_1p_lo_next( &(inSlew[1]) );
  inVal[2] = filter_1p_lo_next( &(inSlew[2]) );
  inVal[3] = filter_1p_lo_next( &(inSlew[3]) );

  // zero the output bus
  outBus = 0;

  // scale each input and add it to the bus
  // note the use of fract32 arithmetic intrinsics!
  // these are fast saturating multiplies/adds for 32bit signed fractions.
  // there are also intrinsics for fr16, mixed modes, and conversions.
  // see http://blackfin.uclinux.org/doku.php?id=toolchain:built-in_functions
  outBus = add_fr1x32( outBus, mult_fr1x32x32(in[0], inVal[0]) );
  outBus = add_fr1x32( outBus, mult_fr1x32x32(in[1], inVal[1]) );
  outBus = add_fr1x32( outBus, mult_fr1x32x32(in[2], inVal[2]) );
  outBus = add_fr1x32( outBus, mult_fr1x32x32(in[3], inVal[3]) );

  // copy the bus to all the outputs
  out[0] = outBus;
  out[1] = outBus;
  out[2] = outBus;
  out[3] = outBus;

  // upate cv outputs
  process_cv();
}


// parameter set function
void module_set_param(u32 idx, ParamValue v) {
  // switch on the param index
  switch(idx) {
    // cv output values
  case eParam_cv0 :
    filter_1p_lo_in( &(cvSlew[0]), v );
    break;
  case eParam_cv1 :
    filter_1p_lo_in( &(cvSlew[1]), v );
    break;
  case eParam_cv2 :
    filter_1p_lo_in( &(cvSlew[2]), v );
    break;
  case eParam_cv3 :
    filter_1p_lo_in( &(cvSlew[3]), v );

    // cv slew values
    break;
  case eParam_cvSlew0 :
   filter_1p_lo_set_slew(&(cvSlew[0]), v);
    break;
  case eParam_cvSlew1 :
    filter_1p_lo_set_slew(&(cvSlew[1]), v);
    break;
  case eParam_cvSlew2 :
    filter_1p_lo_set_slew(&(cvSlew[2]), v);
    break;
  case eParam_cvSlew3 :
    filter_1p_lo_set_slew(&(cvSlew[3]), v);
    break;

    // input attenuation values
  case eParam_in0 :
    filter_1p_lo_in( &(inSlew[0]), v );
    break;
  case eParam_in1 :
    filter_1p_lo_in( &(inSlew[1]), v );
    break;
  case eParam_in2 :
    filter_1p_lo_in( &(inSlew[2]), v );
    break;
  case eParam_in3 :
    filter_1p_lo_in( &(inSlew[3]), v );

    // input attenuation slew values
    break;
  case eParam_inSlew0 :
   filter_1p_lo_set_slew(&(inSlew[0]), v);
    break;
  case eParam_inSlew1 :
    filter_1p_lo_set_slew(&(inSlew[1]), v);
    break;
  case eParam_inSlew2 :
    filter_1p_lo_set_slew(&(inSlew[2]), v);
    break;
  case eParam_inSlew3 :
    filter_1p_lo_set_slew(&(inSlew[3]), v);
    break;

  default:
    break;
  }
}
