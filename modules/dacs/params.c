// autogenerated from param_gen.scd 
 

#include <string.h>
#include "module.h"
#include "params.h" 

void fill_param_desc(void) {
  strcpy(gModuleData->paramDesc[eParam_dac0].label, "dac0");
  strcpy(gModuleData->paramDesc[eParam_dac0].unit, "");
  gModuleData->paramDesc[eParam_dac0].type = eParamTypeFract; 
  gModuleData->paramDesc[eParam_dac0].min = 0x00000000; 
  gModuleData->paramDesc[eParam_dac0].max = 0xffff; // 16 bits?

  strcpy(gModuleData->paramDesc[eParam_dac1].label, "dac1");
  strcpy(gModuleData->paramDesc[eParam_dac1].unit, "");
  gModuleData->paramDesc[eParam_dac1].type = eParamTypeFract; 
  gModuleData->paramDesc[eParam_dac1].min = 0x00000000; 
  gModuleData->paramDesc[eParam_dac1].max = 0xffff; // 16 bits?

  strcpy(gModuleData->paramDesc[eParam_dac2].label, "dac2");
  strcpy(gModuleData->paramDesc[eParam_dac2].unit, "");
  gModuleData->paramDesc[eParam_dac2].type = eParamTypeFract; 
  gModuleData->paramDesc[eParam_dac2].min = 0x00000000; 
  gModuleData->paramDesc[eParam_dac2].max = 0xffff; // 16 bits?

  strcpy(gModuleData->paramDesc[eParam_dac3].label, "dac3");
  strcpy(gModuleData->paramDesc[eParam_dac3].unit, "");
  gModuleData->paramDesc[eParam_dac3].type = eParamTypeFract; 
  gModuleData->paramDesc[eParam_dac3].min = 0x00000000; 
  gModuleData->paramDesc[eParam_dac3].max = 0xffff; // 16 bits?

  strcpy(gModuleData->paramDesc[eParam_slew0].label, "slew0");
  strcpy(gModuleData->paramDesc[eParam_slew0].unit, "");
  gModuleData->paramDesc[eParam_slew0].type = eParamTypeFract; 
  gModuleData->paramDesc[eParam_slew0].min = 0x00000000; 
  gModuleData->paramDesc[eParam_slew0].max = 0x7fffffff;

  strcpy(gModuleData->paramDesc[eParam_slew1].label, "slew1");
  strcpy(gModuleData->paramDesc[eParam_slew1].unit, "");
  gModuleData->paramDesc[eParam_slew1].type = eParamTypeFract; 
  gModuleData->paramDesc[eParam_slew1].min = 0x00000000; 
  gModuleData->paramDesc[eParam_slew1].max = 0x7fffffff;

  strcpy(gModuleData->paramDesc[eParam_slew2].label, "slew2");
  strcpy(gModuleData->paramDesc[eParam_slew2].unit, "");
  gModuleData->paramDesc[eParam_slew2].type = eParamTypeFract; 
  gModuleData->paramDesc[eParam_slew2].min = 0x00000000; 
  gModuleData->paramDesc[eParam_slew2].max = 0x7fffffff; 

  strcpy(gModuleData->paramDesc[eParam_slew3].label, "slew3");
  strcpy(gModuleData->paramDesc[eParam_slew3].unit, "");
  gModuleData->paramDesc[eParam_slew3].type = eParamTypeFract; 
  gModuleData->paramDesc[eParam_slew3].min = 0x00000000; 
  gModuleData->paramDesc[eParam_slew3].max = 0x7fffffff;
}

// EOF
